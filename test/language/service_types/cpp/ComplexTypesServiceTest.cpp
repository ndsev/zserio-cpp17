#include <algorithm>
#include <cmath>
#include <memory>

#include "gtest/gtest.h"
#include "service_types/complex_types_service/ComplexTypesService.h"
#include "test_utils/LocalServiceClient.h"
#include "zserio/RebindAlloc.h"

using namespace std::literals;

namespace service_types
{
namespace complex_types_service
{

using allocator_type = ComplexTypesService::Client::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;
using LocalServiceClient = test_utils::LocalServiceClient<allocator_type>;

namespace
{

void convertRgbToCmyk(
        uint8_t colR, uint8_t colG, uint8_t colB, uint8_t& colC, uint8_t& colM, uint8_t& colY, uint8_t& colK)
{
    // see https://www.rapidtables.com/convert/color/rgb-to-cmyk.html
    const double colRR = colR / 255. * 100;
    const double colGG = colG / 255. * 100;
    const double colBB = colB / 255. * 100;

    double colKK = 100. - std::max(std::max(colRR, colGG), colBB);

    colC = static_cast<uint8_t>(std::round((100. - colRR - colKK) / (100. - colKK) * 100));
    colM = static_cast<uint8_t>(std::round((100. - colGG - colKK) / (100. - colKK) * 100));
    colY = static_cast<uint8_t>(std::round((100. - colBB - colKK) / (100. - colKK) * 100));
    colK = static_cast<uint8_t>(std::round(colKK));
}

void convertCmykToRgb(
        uint8_t colC, uint8_t colM, uint8_t colY, uint8_t colK, uint8_t& colR, uint8_t& colG, uint8_t& colB)
{
    // see https://www.rapidtables.com/convert/color/cmyk-to-rgb.html
    colR = static_cast<uint8_t>(std::round(255 * (1 - colC / 100.) * (1 - colK / 100.)));
    colG = static_cast<uint8_t>(std::round(255 * (1 - colM / 100.) * (1 - colK / 100.)));
    colB = static_cast<uint8_t>(std::round(255 * (1 - colY / 100.) * (1 - colK / 100.)));
}

} // namespace

class ComplexTypesServiceImpl : public ComplexTypesService::Service
{
public:
    Response swapModelsImpl(const Request& request, void*) override
    {
        const RequestData& requestData = request.data;
        const auto& data = requestData.data;

        Response response{get_allocator_ref()};
        response.length = static_cast<uint32_t>(data.size());

        if (request.model == ColorModel::RGB)
        {
            rgbToCmyk(data, response);
        }
        else
        {
            cmykToRgb(data, response);
        }

        return response;
    }

    LengthResponse getLengthImpl(const Request& request, void*) override
    {
        const RequestData& requestData = request.data;
        return LengthResponse(static_cast<uint32_t>(requestData.data.size()));
    }

private:
    void rgbToCmyk(const VectorType<ColorModelChoice>& data, Response& response)
    {
        auto& cmykData = response.data.emplace<ResponseData::Tag::cmykData>();
        cmykData.resize(response.length);
        for (uint32_t i = 0; i < response.length; ++i)
        {
            const RGBModel& rgb = data.at(i).get<ColorModelChoice::Tag::rgb>();
            uint8_t colC = 0;
            uint8_t colM = 0;
            uint8_t colY = 0;
            uint8_t colK = 0;
            convertRgbToCmyk(rgb.red, rgb.green, rgb.blue, colC, colM, colY, colK);
            cmykData[i].cyan = colC;
            cmykData[i].magenta = colM;
            cmykData[i].yellow = colY;
            cmykData[i].key = colK;
        }
    }

    void cmykToRgb(const VectorType<ColorModelChoice>& data, Response& response)
    {
        auto& rgbData = response.data.emplace<ResponseData::Tag::rgbData>();
        rgbData.resize(response.length);
        for (uint32_t i = 0; i < response.length; ++i)
        {
            const CMYKModel& cmyk = data.at(i).get<ColorModelChoice::Tag::cmyk>();
            uint8_t colR = 0;
            uint8_t colG = 0;
            uint8_t colB = 0;
            convertCmykToRgb(cmyk.cyan, cmyk.magenta, cmyk.yellow, cmyk.key, colR, colG, colB);
            rgbData[i].red = colR;
            rgbData[i].green = colG;
            rgbData[i].blue = colB;
        }
    }
};

class ComplexTypesServiceTest : public ::testing::Test
{
public:
    ComplexTypesServiceTest() :
            localServiceClient(service),
            client(localServiceClient),
            cmykValues()
    {
        for (size_t i = 0; i < 3; ++i)
        {
            uint8_t colC = 0;
            uint8_t colM = 0;
            uint8_t colY = 0;
            uint8_t colK = 0;
            convertRgbToCmyk(rgbValues[i][0], rgbValues[i][1], rgbValues[i][2], colC, colM, colY, colK);
            cmykValues[i][0] = colC;
            cmykValues[i][1] = colM;
            cmykValues[i][2] = colY;
            cmykValues[i][3] = colK;
        }
    }

protected:
    ComplexTypesServiceImpl service;
    LocalServiceClient localServiceClient;
    ComplexTypesService::Client client;

    // note that conversion is slightly inaccurate and therefore this values are carefully chosen
    // to provide consistent results for the test needs
    static constexpr std::array<std::array<uint8_t, 3>, 3> rgbValues = {
            {{0, 128, 255}, {222, 222, 0}, {65, 196, 31}}};
    std::array<std::array<uint8_t, 4>, 3> cmykValues;
};

TEST_F(ComplexTypesServiceTest, serviceFullName)
{
    ASSERT_EQ("service_types.complex_types_service.ComplexTypesService"sv,
            ComplexTypesService::Service::serviceFullName());
}

TEST_F(ComplexTypesServiceTest, methodNames)
{
    ASSERT_EQ("swapModels"sv, ComplexTypesService::Service::methodNames()[0]);
    ASSERT_EQ("getLength"sv, ComplexTypesService::Service::methodNames()[1]);
}

TEST_F(ComplexTypesServiceTest, rgbToCmyk)
{
    Request request;
    request.model = ColorModel::RGB;
    RequestData& requestData = request.data;
    static const size_t length = 10000;
    requestData.offsets.resize(length);
    auto& data = requestData.data;
    data.resize(length);
    RGBModel model;
    for (size_t i = 0; i < data.size(); ++i)
    {
        model.red = rgbValues[i % 3][0];
        model.green = rgbValues[i % 3][1];
        model.blue = rgbValues[i % 3][2];
        data.at(i).emplace<Color::Tag::rgb>(model);
    }

    LengthResponse lengthResponse = client.getLengthMethod(request);
    ASSERT_EQ(length, lengthResponse.length);

    Response response = client.swapModelsMethod(request);
    ASSERT_EQ(length, response.length);

    const auto& cmykData = response.data.get<ResponseData::Tag::cmykData>();
    for (size_t i = 0; i < cmykData.size(); ++i)
    {
        const CMYKModel& cmyk = cmykData.at(i);
        ASSERT_EQ(cmykValues[i % 3][0], cmyk.cyan);
        ASSERT_EQ(cmykValues[i % 3][1], cmyk.magenta);
        ASSERT_EQ(cmykValues[i % 3][2], cmyk.yellow);
        ASSERT_EQ(cmykValues[i % 3][3], cmyk.key);
    }
}

TEST_F(ComplexTypesServiceTest, cmykToRgb)
{
    Request request;
    request.model = ColorModel::CMYK;
    RequestData& requestData = request.data;
    static const size_t length = 10000;
    requestData.offsets.resize(length);
    auto& data = requestData.data;
    data.resize(length);
    CMYKModel model;
    for (size_t i = 0; i < data.size(); ++i)
    {
        model.cyan = cmykValues[i % 3][0];
        model.magenta = cmykValues[i % 3][1];
        model.yellow = cmykValues[i % 3][2];
        model.key = cmykValues[i % 3][3];
        data.at(i).emplace<Color::Tag::cmyk>(model);
    }

    LengthResponse lengthResponse = client.getLengthMethod(request);
    ASSERT_EQ(length, lengthResponse.length);

    Response response = client.swapModelsMethod(request);
    ASSERT_EQ(length, response.length);

    const auto& rgbData = response.data.get<ResponseData::Tag::rgbData>();
    for (size_t i = 0; i < rgbData.size(); ++i)
    {
        const RGBModel& rgb = rgbData.at(i);
        ASSERT_EQ(rgbValues[i % 3][0], rgb.red);
        ASSERT_EQ(rgbValues[i % 3][1], rgb.green);
        ASSERT_EQ(rgbValues[i % 3][2], rgb.blue);
    }
}

TEST_F(ComplexTypesServiceTest, invalidServiceMethod)
{
    ASSERT_THROW(service.callMethod("nonexistentMethod"sv, {}, nullptr), zserio::ServiceException);
}

} // namespace complex_types_service
} // namespace service_types
