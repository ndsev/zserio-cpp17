#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "union_types/empty_union/EmptyUnion.h"

namespace union_types
{
namespace empty_union
{

using AllocatorType = EmptyUnion::allocator_type;

TEST(EmptyUnionTest, emptyConstructor)
{
    {
        EmptyUnion data;
        ASSERT_EQ(EmptyUnion::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyUnion data = {};
        ASSERT_EQ(EmptyUnion::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyUnion data(AllocatorType{});
        ASSERT_EQ(EmptyUnion::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyUnion data;
        zserio::View view(data);
        ASSERT_EQ(EmptyUnion::Tag::ZSERIO_UNDEFINED, view.zserioChoiceTag());
    }
}

TEST(EmptyUnionTest, zserioChoiceTag)
{
    EmptyUnion data;
    zserio::View view(data);
    ASSERT_EQ(EmptyUnion::Tag::ZSERIO_UNDEFINED, view.zserioChoiceTag());
}

TEST(EmptyUnionTest, comparisonOperators)
{
    EmptyUnion data;
    EmptyUnion equalData;
    test_utils::comparisonOperatorsTest(data, equalData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    test_utils::comparisonOperatorsTest(view, equalView);
}

TEST(EmptyUnionTest, validate)
{
    EmptyUnion data;
    zserio::View view(data);
    EXPECT_NO_THROW(zserio::detail::validate(view));
}

TEST(EmptyUnionTest, bitSizeOf)
{
    EmptyUnion data;
    zserio::View view(data);
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view));
}

TEST(EmptyUnionTest, writeRead)
{
    EmptyUnion data;
    test_utils::writeReadTest(data);
}

TEST(EmptyUnionTest, read)
{
    EmptyUnion expectedReadData;
    test_utils::readTest(
            [](zserio::BitStreamWriter&) {
            },
            expectedReadData);
}

TEST(EmptyUnionTest, stdHash)
{
    EmptyUnion data;
    const size_t dataHash = 23; // hardcoded value to check that the hash code is stable
    EmptyUnion equalData;
    test_utils::hashTest(data, dataHash, equalData);

    zserio::View view(data);
    const size_t viewHash = dataHash;
    zserio::View equalView(equalData);
    test_utils::hashTest(view, viewHash, equalView);
}

} // namespace empty_union
} // namespace union_types
