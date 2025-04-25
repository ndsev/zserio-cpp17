#include "gtest/gtest.h"
#include "parameterized_types/array_element_param_with_optional/Holder.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace array_element_param_with_optional
{

using AllocatorType = Holder::allocator_type;

class ArrayElementParamWithOptionalTest : public ::testing::Test
{
protected:
    static void fillData(Holder& data)
    {
        data.param = Param(HAS_EXTRA, EXTRA_PARAM);
        Value value;
        value.extraValue = ExtraValue(EXTRA_VALUE);
        data.values = {{value}};
    }

    static void writeData(zserio::BitStreamWriter& writer)
    {
        zserio::detail::write(writer, HAS_EXTRA);
        zserio::detail::write(writer, EXTRA_PARAM);
        zserio::detail::write(writer, zserio::VarSize(1));
        zserio::detail::write(writer, EXTRA_VALUE);
    }

    static constexpr std::string_view BLOB_NAME =
            "language/parameterized_types/array_element_param_with_optional.blob";

    static constexpr zserio::Bool HAS_EXTRA = true;
    static constexpr zserio::UInt7 EXTRA_PARAM = 0x00;
    static constexpr zserio::UInt64 EXTRA_VALUE = 0xDEAD;
    static constexpr zserio::BitSize HOLDER_BIT_SIZE = 1 + 7 + 8 /* bitSizeOf(VarSize(1)) */ + 64;
};

TEST_F(ArrayElementParamWithOptionalTest, constructors)
{
    {
        Holder data;
        ASSERT_FALSE(data.param.hasExtra);
        ASSERT_FALSE(data.param.extraParam);
        ASSERT_EQ(0, data.values.size());
    }

    {
        Holder data = {};
        ASSERT_FALSE(data.param.hasExtra);
        ASSERT_FALSE(data.param.extraParam);
        ASSERT_EQ(0, data.values.size());
    }

    {
        Holder data(AllocatorType{});
        ASSERT_FALSE(data.param.hasExtra);
        ASSERT_FALSE(data.param.extraParam);
        ASSERT_EQ(0, data.values.size());
    }

    {
        Holder data(Param{HAS_EXTRA, EXTRA_PARAM}, {{Value{std::nullopt, ExtraValue{EXTRA_VALUE}}}});
        zserio::View view(data);
        ASSERT_EQ(HAS_EXTRA, view.param().hasExtra());
        ASSERT_EQ(EXTRA_PARAM, view.param().extraParam().value());
        ASSERT_EQ(EXTRA_VALUE, view.values().at(0).extraValue()->value());
    }
}

TEST_F(ArrayElementParamWithOptionalTest, comparisonOperators)
{
    Holder data;
    fillData(data);

    Holder equalData;
    fillData(equalData);

    Holder lessThanData;
    fillData(lessThanData);
    lessThanData.values.at(0).extraValue->value = EXTRA_VALUE - 1;

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);
    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(ArrayElementParamWithOptionalTest, bitSizeOf)
{
    Holder data;
    fillData(data);
    zserio::View view(data);

    const size_t bitPosition = 2;
    ASSERT_EQ(HOLDER_BIT_SIZE, zserio::detail::bitSizeOf(view, bitPosition));
}

TEST_F(ArrayElementParamWithOptionalTest, writeRead)
{
    Holder data;
    fillData(data);

    test_utils::writeReadTest(data);
}

TEST_F(ArrayElementParamWithOptionalTest, writeReadFile)
{
    Holder data;
    fillData(data);

    test_utils::writeReadFileTest(BLOB_NAME, data);
}

TEST_F(ArrayElementParamWithOptionalTest, read)
{
    Holder expectedData;
    fillData(expectedData);

    test_utils::readTest(writeData, expectedData);
}

TEST_F(ArrayElementParamWithOptionalTest, stdHash)
{
    Holder data;
    fillData(data);
    const size_t dataHash = 1256582;

    Holder equalData;
    fillData(equalData);

    Holder diffData;
    fillData(diffData);
    diffData.values.at(0).extraValue->value = EXTRA_VALUE - 1;
    const size_t diffDataHash = 1256581;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 2484242;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 2484241;
    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace array_element_param_with_optional
} // namespace parameterized_types
