#include "gtest/gtest.h"
#include "parameterized_types/parameterized_param/ParameterizedParamHolder.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace parameterized_param
{

class ParameterizedParamTest : public ::testing::Test
{
protected:
    static void fillData(ParameterizedParamHolder& data)
    {
        Param& param = data.param;
        param.value = PARAM_VALUE;
        param.extraValue = PARAM_EXTRA_VALUE;

        ParameterizedParam& parameterizedParam = data.parameterizedParam;
        parameterizedParam.value = PARAMETERIZED_PARAM_VALUE;
        parameterizedParam.extraValue = PARAMETERIZED_PARAM_EXTRA_VALUE;
    }

    static void writeData(zserio::BitStreamWriter& writer, const ParameterizedParamHolder& data)
    {
        writer.writeUnsignedBits32(data.parameter, 16);

        const Param& param = data.param;
        writer.writeUnsignedBits32(param.value, 16);
        writer.writeUnsignedBits32(*param.extraValue, 32);

        const ParameterizedParam& parameterizedParam = data.parameterizedParam;
        writer.writeUnsignedBits32(parameterizedParam.value, 16);
        writer.writeUnsignedBits32(*parameterizedParam.extraValue, 32);
    }

private:
    static constexpr uint16_t PARAMETER = 11;
    static constexpr uint16_t PARAM_VALUE = 0x0BED;
    static constexpr uint32_t PARAM_EXTRA_VALUE = 0x0BEDDEAD;
    static constexpr uint16_t PARAMETERIZED_PARAM_VALUE = 0x0BAD;
    static constexpr uint32_t PARAMETERIZED_PARAM_EXTRA_VALUE = 0x0BADDEAD;
};

TEST_F(ParameterizedParamTest, writeRead)
{
    ParameterizedParamHolder data;
    fillData(data);

    test_utils::writeReadTest(data);
}

TEST_F(ParameterizedParamTest, read)
{
    ParameterizedParamHolder data;
    fillData(data);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, data), data);
}

} // namespace parameterized_param
} // namespace parameterized_types
