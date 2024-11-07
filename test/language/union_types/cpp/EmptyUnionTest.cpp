#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "union_types/empty_union/EmptyUnion.h"

namespace union_types
{
namespace empty_union
{

using allocator_type = EmptyUnion::allocator_type;
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
        EmptyUnion data(allocator_type{});
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        EmptyUnion data;
        zserio::View<EmptyUnion> view(data);
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, view.zserioChoiceTag());
    }
}

TEST(EmptyUnionTest, zserioChoiceTag)
{
    EmptyUnion data;
    zserio::View<EmptyUnion> view(data);
    ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, view.zserioChoiceTag());
}

TEST(EmptyUnionTest, comparisonOperators)
{
    EmptyUnion data;
    EmptyUnion equalData;
    ASSERT_TRUE(data == equalData);
    ASSERT_FALSE(data < equalData);

    zserio::View<EmptyUnion> view(data);
    zserio::View<EmptyUnion> equalView(equalData);
    ASSERT_TRUE(view == equalView);
    ASSERT_FALSE(view < equalView);
}

TEST(EmptyUnionTest, validate)
{
    // TODO
}

TEST(EmptyUnionTest, bitSizeOf)
{
    EmptyUnion data;
    zserio::View<EmptyUnion> view(data);
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view));
}

TEST(EmptyUnionTest, writeRead)
{
    EmptyUnion data;
    test_utils::writeReadTest(data);
}

TEST(EmptyUnionTest, read)
{
    zserio::BitBuffer bitBuffer;
    zserio::BitStreamWriter writer(bitBuffer);
    EmptyUnion expectedReadData;
    test_utils::readTest(writer, expectedReadData);
}

TEST(EmptyUnionTest, stdHash)
{
    std::hash<EmptyUnion> dataHasher;
    EmptyUnion data;
    const size_t dataHash = 23; // hardcoded value to check that the hash code is stable
    ASSERT_EQ(dataHash, dataHasher(data));

    std::hash<zserio::View<EmptyUnion>> viewHasher;
    zserio::View<EmptyUnion> view(data);
    const size_t viewHash = dataHash;
    ASSERT_EQ(viewHash, viewHasher(view));
}

} // namespace empty_union
} // namespace union_types
