#include "gtest/gtest.h"
#include "structure_types/empty_structure_with_parameter/EmptyStructureWithParameter.h"
#include "test_utils/TestUtility.h"

namespace structure_types
{
namespace empty_structure_with_parameter
{

using AllocatorType = EmptyStructureWithParameter::AllocatorType;

TEST(EmptyStructureWithParameterTest, emptyConstructor)
{
    {
        EmptyStructureWithParameter data;
        (void)data;
    }
    {
        EmptyStructureWithParameter data = {};
        (void)data;
    }
    {
        EmptyStructureWithParameter data(AllocatorType{});
        (void)data;
    }
    {
        EmptyStructureWithParameter data;
        zserio::View view(data, 10);
        (void)view;
    }
}

TEST(EmptyStructureWithParameterTest, param)
{
    const zserio::Int32 param = 10;
    EmptyStructureWithParameter data;
    zserio::View view(data, param);
    ASSERT_EQ(param, view.param());
}

TEST(EmptyStructureWithParameterTest, comparisonOperators)
{
    EmptyStructureWithParameter data;
    EmptyStructureWithParameter equalData;
    EmptyStructureWithParameter lessThenData;
    test_utils::comparisonOperatorsTest(data, equalData);

    zserio::View view(data, 10);
    zserio::View equalView(equalData, 10);
    zserio::View lessThenView(lessThenData, 9);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST(EmptyStructureWithParameterTest, validate)
{
    EmptyStructureWithParameter data;
    zserio::View view(data, 10);
    EXPECT_NO_THROW(zserio::detail::validate(view));
}

TEST(EmptyStructureWithParameterTest, bitSizeOf)
{
    EmptyStructureWithParameter data;
    zserio::View view(data, 10);
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view));
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view, 1));
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view, 100));
}

TEST(EmptyStructureWithParameterTest, read)
{
    EmptyStructureWithParameter expectedReadData;
    test_utils::readTest(
            [](zserio::BitStreamWriter&) {
            },
            expectedReadData, zserio::Int32(10));
}

TEST(EmptyStructureWithParameterTest, writeRead)
{
    EmptyStructureWithParameter data;
    test_utils::writeReadTest(data, zserio::Int32(10));
}

TEST(EmptyStructureWithParameterTest, stdHash)
{
    EmptyStructureWithParameter data;
    const size_t dataHash = 23; // hardcoded value to check that the hash code is stable
    EmptyStructureWithParameter equalData;
    EmptyStructureWithParameter diffData;
    test_utils::hashTest(data, dataHash, equalData);

    zserio::View view(data, 10);
    const size_t viewHash = 861;
    zserio::View equalView(equalData, 10);
    zserio::View diffView(diffData, 9);
    const size_t diffHash = 860; // hardcoded value to check that the hash code is stable
    test_utils::hashTest(view, viewHash, equalView, diffView, diffHash);
}

} // namespace empty_structure_with_parameter
} // namespace structure_types
