#include "choice_types/empty_choice/EmptyChoice.h"
#include "gtest/gtest.h"
#include "test_utils/ComparisonOperatorsTest.h"
#include "test_utils/ReadTest.h"
#include "test_utils/WriteReadFileTest.h"
#include "test_utils/WriteReadTest.h"

namespace choice_types
{
namespace empty_choice
{

using allocator_type = EmptyChoice::allocator_type;
using ChoiceTag = EmptyChoice::ChoiceTag;

TEST(EmptyChoiceTest, constructors)
{
    {
        EmptyChoice data;
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        EmptyChoice data = {};
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        EmptyChoice data(allocator_type{});
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        EmptyChoice data;
        zserio::View<EmptyChoice> view(data, static_cast<zserio::UInt8>(0));
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, view.zserioChoiceTag());
    }
}

TEST(EmptyChoiceTest, selector)
{
    EmptyChoice data;
    zserio::View<EmptyChoice> viewA(data, static_cast<zserio::UInt8>(0));
    ASSERT_EQ(0, viewA.selector());
}

TEST(EmptyChoiceTest, zserioChoiceTag)
{
    EmptyChoice data;
    zserio::View<EmptyChoice> view(data, static_cast<zserio::UInt8>(0));
    ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, view.zserioChoiceTag());
}

TEST(EmptyChoiceTest, comparisonOperators)
{
    EmptyChoice data;
    EmptyChoice equalData;
    ASSERT_TRUE(data == equalData);
    ASSERT_FALSE(data < equalData);

    zserio::View<EmptyChoice> view(data, static_cast<zserio::UInt8>(1));
    zserio::View<EmptyChoice> equalView(equalData, static_cast<zserio::UInt8>(1));
    zserio::View<EmptyChoice> lessThenView(equalData, static_cast<zserio::UInt8>(0));
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST(EmptyChoiceTest, validate)
{
    // TODO
}

TEST(EmptyChoiceTest, bitSizeOf)
{
    EmptyChoice data;
    zserio::View<EmptyChoice> view(data, static_cast<zserio::UInt8>(0));
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view));
}

TEST(EmptyChoiceTest, writeRead)
{
    EmptyChoice data;
    test_utils::writeReadTest(data, static_cast<zserio::UInt8>(0));
}

TEST(EmptyChoiceTest, read)
{
    zserio::BitBuffer bitBuffer;
    zserio::BitStreamWriter writer(bitBuffer);
    EmptyChoice expectedReadData;
    test_utils::readTest(writer, expectedReadData, static_cast<zserio::UInt8>(0));
}

TEST(EmptyChoiceTest, stdHash)
{
    std::hash<EmptyChoice> dataHasher;
    EmptyChoice data;
    const size_t dataHash = 23; // hardcoded value to check that the hash code is stable
    ASSERT_EQ(dataHash, dataHasher(data));

    std::hash<zserio::View<EmptyChoice>> viewHasher;
    zserio::View<EmptyChoice> view(data, static_cast<zserio::UInt8>(0));
    const size_t viewHash = dataHash;
    ASSERT_EQ(viewHash, viewHasher(view));
}

} // namespace empty_choice
} // namespace choice_types
