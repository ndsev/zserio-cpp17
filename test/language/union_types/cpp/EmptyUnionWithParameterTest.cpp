#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "union_types/empty_union_with_parameter/EmptyUnionWithParameter.h"

namespace union_types
{
namespace empty_union_with_parameter
{

using AllocatorType = EmptyUnionWithParameter::allocator_type;

class EmptyUnionWithParameterTest : public ::testing::Test
{
protected:
    static constexpr zserio::Int32 PARAM = 13;
};

TEST_F(EmptyUnionWithParameterTest, emptyConstructor)
{
    {
        EmptyUnionWithParameter data;
        ASSERT_EQ(EmptyUnionWithParameter::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyUnionWithParameter data = {};
        ASSERT_EQ(EmptyUnionWithParameter::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyUnionWithParameter data(AllocatorType{});
        ASSERT_EQ(EmptyUnionWithParameter::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyUnionWithParameter data;
        zserio::View view(data, PARAM);
        ASSERT_EQ(EmptyUnionWithParameter::Tag::ZSERIO_UNDEFINED, view.zserioChoiceTag());
    }
}

TEST_F(EmptyUnionWithParameterTest, zserioChoiceTag)
{
    EmptyUnionWithParameter data;
    zserio::View view(data, PARAM);
    ASSERT_EQ(EmptyUnionWithParameter::Tag::ZSERIO_UNDEFINED, view.zserioChoiceTag());
}

TEST_F(EmptyUnionWithParameterTest, param)
{
    EmptyUnionWithParameter data;
    zserio::View view(data, PARAM);
    ASSERT_EQ(PARAM, view.param());
}

TEST_F(EmptyUnionWithParameterTest, comparisonOperators)
{
    EmptyUnionWithParameter data;
    EmptyUnionWithParameter equalData;
    EmptyUnionWithParameter lessThenData;
    test_utils::comparisonOperatorsTest(data, equalData);

    zserio::View view(data, PARAM);
    zserio::View equalView(equalData, PARAM);
    zserio::View lessThenView(lessThenData, PARAM - 1);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST_F(EmptyUnionWithParameterTest, validate)
{
    EmptyUnionWithParameter data;
    zserio::View view(data, PARAM);
    EXPECT_NO_THROW(zserio::detail::validate(view));
}

TEST_F(EmptyUnionWithParameterTest, bitSizeOf)
{
    EmptyUnionWithParameter data;
    zserio::View view(data, PARAM);
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view));
}

TEST_F(EmptyUnionWithParameterTest, writeRead)
{
    EmptyUnionWithParameter data;
    test_utils::writeReadTest(data, PARAM);
}

TEST_F(EmptyUnionWithParameterTest, read)
{
    EmptyUnionWithParameter expectedReadData;
    test_utils::readTest(
            [](zserio::BitStreamWriter&) {
            },
            expectedReadData, PARAM);
}

TEST_F(EmptyUnionWithParameterTest, stdHash)
{
    EmptyUnionWithParameter data;
    const size_t dataHash = 23; // hardcoded value to check that the hash code is stable
    EmptyUnionWithParameter equalData;
    EmptyUnionWithParameter diffData;
    test_utils::hashTest(data, dataHash, equalData);

    zserio::View view(data, PARAM);
    const size_t viewHash = 864; // hardcoded value to check that the hash code is stable
    zserio::View equalView(equalData, PARAM);
    zserio::View diffView(diffData, PARAM - 1);
    const size_t diffHash = 863; // hardcoded value to check that the hash code is stable
    test_utils::hashTest(view, viewHash, equalView, diffView, diffHash);
}

} // namespace empty_union_with_parameter
} // namespace union_types
