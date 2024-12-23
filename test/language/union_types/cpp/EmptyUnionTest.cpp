#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "union_types/empty_union/EmptyUnion.h"

namespace union_types
{
namespace empty_union
{

using AllocatorType = EmptyUnion::AllocatorType;
using ChoiceTag = EmptyUnion::ChoiceTag;

TEST(EmptyUnionTest, emptyConstructor)
{
    {
        EmptyUnion data;
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        EmptyUnion data = {};
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        EmptyUnion data(AllocatorType{});
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        EmptyUnion data;
        zserio::View view(data);
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, view.zserioChoiceTag());
    }
}

TEST(EmptyUnionTest, zserioChoiceTag)
{
    EmptyUnion data;
    zserio::View view(data);
    ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, view.zserioChoiceTag());
}

TEST(EmptyUnionTest, comparisonOperators)
{
    EmptyUnion data;
    EmptyUnion equalData;
    ASSERT_TRUE(data == equalData);
    ASSERT_FALSE(data < equalData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    ASSERT_TRUE(view == equalView);
    ASSERT_FALSE(view < equalView);
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
    std::hash<EmptyUnion> dataHasher;
    EmptyUnion data;
    const size_t dataHash = 23; // hardcoded value to check that the hash code is stable
    ASSERT_EQ(dataHash, dataHasher(data));

    std::hash<zserio::View<EmptyUnion>> viewHasher;
    zserio::View view(data);
    const size_t viewHash = dataHash;
    ASSERT_EQ(viewHash, viewHasher(view));
}

} // namespace empty_union
} // namespace union_types
