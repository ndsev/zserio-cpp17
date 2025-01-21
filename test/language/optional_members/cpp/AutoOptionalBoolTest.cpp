#include "gtest/gtest.h"
#include "optional_members/auto_optional_bool/Container.h"
#include "test_utils/TestUtility.h"

namespace optional_members
{
namespace auto_optional_bool
{

using AllocatorType = Container::AllocatorType;

class AutoOptionalBoolTest : public ::testing::Test
{
protected:
    static void fillData(Container& data, bool hasOptional)
    {
        data.nonOptionalBool = NON_OPTIONAL_BOOL_VALUE;
        if (hasOptional)
        {
            data.autoOptionalBool = AUTO_OPTIONAL_BOOL_VALUE;
        }
    }

    static void writeData(zserio::BitStreamWriter& writer, bool hasOptional)
    {
        writer.writeBool(NON_OPTIONAL_BOOL_VALUE);
        writer.writeBool(hasOptional);
        if (hasOptional)
        {
            writer.writeBool(AUTO_OPTIONAL_BOOL_VALUE);
        }
    }

    static constexpr size_t CONTAINER_BIT_SIZE_WITHOUT_OPTIONAL = 2;
    static constexpr size_t CONTAINER_BIT_SIZE_WITH_OPTIONAL = 3;

    static constexpr zserio::Bool NON_OPTIONAL_BOOL_VALUE = false;
    static constexpr zserio::Bool AUTO_OPTIONAL_BOOL_VALUE = true;
};

TEST_F(AutoOptionalBoolTest, constructors)
{
    {
        const Container data;
        ASSERT_FALSE(data.nonOptionalBool);
        ASSERT_FALSE(data.autoOptionalBool);
    }
    {
        const Container data = {};
        ASSERT_FALSE(data.nonOptionalBool);
        ASSERT_FALSE(data.autoOptionalBool);
    }
    {
        const Container data{AllocatorType()};
        ASSERT_FALSE(data.nonOptionalBool);
        ASSERT_FALSE(data.autoOptionalBool);
    }
    {
        const Container data = Container(NON_OPTIONAL_BOOL_VALUE, AUTO_OPTIONAL_BOOL_VALUE);
        zserio::View view(data);
        ASSERT_EQ(NON_OPTIONAL_BOOL_VALUE, view.nonOptionalBool());
        ASSERT_TRUE(view.autoOptionalBool());
        ASSERT_EQ(AUTO_OPTIONAL_BOOL_VALUE, view.autoOptionalBool().value());
    }
}

TEST_F(AutoOptionalBoolTest, bitSizeOfWithOptional)
{
    const bool hasOptional = true;
    Container data;
    fillData(data, hasOptional);
    zserio::View view(data);

    ASSERT_EQ(CONTAINER_BIT_SIZE_WITH_OPTIONAL, zserio::detail::bitSizeOf(view));
}

TEST_F(AutoOptionalBoolTest, bitSizeOfWithoutOptional)
{
    const bool hasOptional = false;
    Container data;
    fillData(data, hasOptional);
    zserio::View view(data);

    ASSERT_EQ(CONTAINER_BIT_SIZE_WITHOUT_OPTIONAL, zserio::detail::bitSizeOf(view));
}

TEST_F(AutoOptionalBoolTest, comparisionOperators)
{
    Container data;
    fillData(data, true);
    Container equalData;
    fillData(equalData, true);
    Container lessThanData;
    fillData(lessThanData, false);
    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);
    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(AutoOptionalBoolTest, readWithOptional)
{
    Container data;
    fillData(data, true);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, true), data);
}

TEST_F(AutoOptionalBoolTest, readWithoutOptional)
{
    Container data;
    fillData(data, false);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, false), data);
}

TEST_F(AutoOptionalBoolTest, writeReadWithOptional)
{
    Container data;
    fillData(data, true);

    test_utils::writeReadTest(data);
}

TEST_F(AutoOptionalBoolTest, writeReadWithoutOptional)
{
    Container data;
    fillData(data, false);

    test_utils::writeReadTest(data);
}

TEST_F(AutoOptionalBoolTest, stdHash)
{
    Container data;
    fillData(data, true);
    const size_t dataHash = 31488;

    Container equalData;
    fillData(equalData, true);

    Container diffData;
    fillData(diffData, false);
    const size_t diffDataHash = 851;
    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 31488;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 851;
    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace auto_optional_bool
} // namespace optional_members
