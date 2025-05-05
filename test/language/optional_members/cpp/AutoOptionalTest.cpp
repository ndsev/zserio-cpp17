#include "gtest/gtest.h"
#include "optional_members/auto_optional/Container.h"
#include "test_utils/TestUtility.h"

namespace optional_members
{
namespace auto_optional
{

using AllocatorType = Container::allocator_type;

class AutoOptionalTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, zserio::Int32 nonOptionalIntValue,
            bool hasOptionalIntValue, zserio::Int32 optionalIntValue)
    {
        writer.writeSignedBits32(nonOptionalIntValue, 32);
        writer.writeBool(hasOptionalIntValue);
        if (hasOptionalIntValue)
        {
            writer.writeSignedBits32(optionalIntValue, 32);
        }
    }

    static constexpr zserio::Int32 NON_OPTIONAL_INT_VALUE = static_cast<int32_t>(0xDEADDEAD);
    static constexpr zserio::Int32 AUTO_OPTIONAL_INT_VALUE = static_cast<int32_t>(0xBEEFBEEF);

    static constexpr size_t CONTAINER_BIT_SIZE_WITHOUT_OPTIONAL = 32 + 1;
    static constexpr size_t CONTAINER_BIT_SIZE_WITH_OPTIONAL = 32 + 1 + 32;
};

TEST_F(AutoOptionalTest, constructors)
{
    {
        const Container data;
        ASSERT_FALSE(data.autoOptionalInt);
    }
    {
        const Container data = {};
        ASSERT_FALSE(data.autoOptionalInt);
    }
    {
        const Container data{AllocatorType()};
        ASSERT_FALSE(data.autoOptionalInt);
    }
    {
        const Container data = Container(NON_OPTIONAL_INT_VALUE, AUTO_OPTIONAL_INT_VALUE);
        zserio::View view(data);
        ASSERT_EQ(NON_OPTIONAL_INT_VALUE, view.nonOptionalInt());
        ASSERT_TRUE(view.autoOptionalInt());
        ASSERT_EQ(AUTO_OPTIONAL_INT_VALUE, view.autoOptionalInt().value());
    }
}

TEST_F(AutoOptionalTest, comparisionOperators)
{
    Container data = Container(NON_OPTIONAL_INT_VALUE, AUTO_OPTIONAL_INT_VALUE);
    Container equalData = Container(NON_OPTIONAL_INT_VALUE, AUTO_OPTIONAL_INT_VALUE);
    Container lessThanData = Container(NON_OPTIONAL_INT_VALUE, std::nullopt);

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(AutoOptionalTest, bitSizeOf)
{
    Container data;
    zserio::View view(data);

    data.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    ASSERT_EQ(CONTAINER_BIT_SIZE_WITHOUT_OPTIONAL, zserio::detail::bitSizeOf(view));

    data.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    ASSERT_EQ(CONTAINER_BIT_SIZE_WITH_OPTIONAL, zserio::detail::bitSizeOf(view));
}

TEST_F(AutoOptionalTest, writeRead)
{
    Container data;

    data.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    test_utils::writeReadTest(data);

    data.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    test_utils::writeReadTest(data);
}

TEST_F(AutoOptionalTest, read)
{
    Container data;

    data.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, NON_OPTIONAL_INT_VALUE, false, AUTO_OPTIONAL_INT_VALUE),
            data);

    data.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, NON_OPTIONAL_INT_VALUE, true, AUTO_OPTIONAL_INT_VALUE),
            data);
}

TEST_F(AutoOptionalTest, stdHash)
{
    // use hardcoded values to check that the hash code is stable

    Container data(NON_OPTIONAL_INT_VALUE, AUTO_OPTIONAL_INT_VALUE);
    const size_t dataHash = 3994118383;
    Container equalData(NON_OPTIONAL_INT_VALUE, AUTO_OPTIONAL_INT_VALUE);
    Container diffData(NON_OPTIONAL_INT_VALUE, std::nullopt);
    const size_t diffDataHash = 3735937536;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 3994118383;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 3735937536;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace auto_optional
} // namespace optional_members
