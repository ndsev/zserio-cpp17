#include "gtest/gtest.h"
#include "parameterized_types/simple_param/Item.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace simple_param
{

using AllocatorType = Item::AllocatorType;

class SimpleParamTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, zserio::UInt32 version, zserio::UInt16 param,
            zserio::UInt32 extraParam)
    {
        writer.writeUnsignedBits32(param, 16);
        if (version >= HIGHER_VERSION)
        {
            writer.writeUnsignedBits32(extraParam, 32);
        }
    }

    static constexpr size_t ITEM_BIT_SIZE_WITHOUT_OPTIONAL = 16;
    static constexpr size_t ITEM_BIT_SIZE_WITH_OPTIONAL = 16 + 32;

    static constexpr zserio::UInt32 LOWER_VERSION = 9;
    static constexpr zserio::UInt32 HIGHER_VERSION = 10;

    static constexpr zserio::UInt16 ITEM_PARAM = 0xAA;
    static constexpr zserio::UInt32 ITEM_EXTRA_PARAM = 0xBB;
};

TEST_F(SimpleParamTest, constructors)
{
    {
        Item data;
        ASSERT_EQ(0, data.param);
        ASSERT_FALSE(data.extraParam);
    }

    {
        Item data = {};
        ASSERT_EQ(0, data.param);
        ASSERT_FALSE(data.extraParam);
    }

    {
        Item data{AllocatorType()};
        ASSERT_EQ(0, data.param);
        ASSERT_FALSE(data.extraParam);
    }

    {
        Item data(ITEM_PARAM, ITEM_EXTRA_PARAM);
        zserio::View view(data, HIGHER_VERSION);
        ASSERT_EQ(ITEM_PARAM, view.param());
        ASSERT_EQ(ITEM_EXTRA_PARAM, view.extraParam());
    }
}

TEST_F(SimpleParamTest, comparisionOperators)
{
    Item data(ITEM_PARAM, ITEM_EXTRA_PARAM);
    Item equalData(ITEM_PARAM, ITEM_EXTRA_PARAM);
    Item lessThanData(ITEM_PARAM, std::nullopt);

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data, HIGHER_VERSION);
    zserio::View equalView(equalData, HIGHER_VERSION);
    zserio::View lessThanView(lessThanData, LOWER_VERSION);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(SimpleParamTest, bitSizeOf)
{
    Item data1;
    data1.param = ITEM_PARAM;
    zserio::View view1(data1, LOWER_VERSION);
    ASSERT_EQ(ITEM_BIT_SIZE_WITHOUT_OPTIONAL, zserio::detail::bitSizeOf(view1, 0));

    Item data2{ITEM_PARAM, ITEM_EXTRA_PARAM};
    ::zserio::View view2(data2, HIGHER_VERSION);
    ASSERT_EQ(ITEM_BIT_SIZE_WITH_OPTIONAL, zserio::detail::bitSizeOf(view2, 0));
}

TEST_F(SimpleParamTest, writeRead)
{
    Item data;
    data.param = ITEM_PARAM;
    test_utils::writeReadTest(data, LOWER_VERSION);

    data.extraParam = ITEM_EXTRA_PARAM;
    test_utils::writeReadTest(data, HIGHER_VERSION);
}

TEST_F(SimpleParamTest, read)
{
    Item data;
    data.param = ITEM_PARAM;
    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, LOWER_VERSION, ITEM_PARAM, ITEM_EXTRA_PARAM), data,
            LOWER_VERSION);

    data.extraParam = ITEM_EXTRA_PARAM;
    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, HIGHER_VERSION, ITEM_PARAM, ITEM_EXTRA_PARAM), data,
            HIGHER_VERSION);
}

TEST_F(SimpleParamTest, stdHash)
{
    Item data(ITEM_PARAM, ITEM_EXTRA_PARAM);
    const size_t dataHash = 37964;
    Item equalData(ITEM_PARAM, ITEM_EXTRA_PARAM);
    Item diffData(ITEM_PARAM, std::nullopt);
    const size_t diffDataHash = 1021;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data, HIGHER_VERSION);
    const size_t viewHash = 1185186;
    zserio::View equalView(equalData, HIGHER_VERSION);
    zserio::View diffView(diffData, LOWER_VERSION);
    const size_t diffViewHash = 31990;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace simple_param
} // namespace parameterized_types
