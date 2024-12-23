#include "choice_types/empty_choice/EmptyChoice.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace choice_types
{
namespace empty_choice
{

using AllocatorType = EmptyChoice::AllocatorType;
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
        EmptyChoice data(AllocatorType{});
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        EmptyChoice data;
        zserio::View view(data, static_cast<zserio::UInt8>(0));
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, view.zserioChoiceTag());
    }
}

TEST(EmptyChoiceTest, selector)
{
    EmptyChoice data;
    zserio::View viewA(data, static_cast<zserio::UInt8>(0));
    ASSERT_EQ(0, viewA.selector());
}

TEST(EmptyChoiceTest, zserioChoiceTag)
{
    EmptyChoice data;
    zserio::View view(data, static_cast<zserio::UInt8>(0));
    ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, view.zserioChoiceTag());
}

TEST(EmptyChoiceTest, comparisonOperators)
{
    EmptyChoice data;
    EmptyChoice equalData;
    ASSERT_TRUE(data == equalData);
    ASSERT_FALSE(data < equalData);

    zserio::View view(data, static_cast<zserio::UInt8>(1));
    zserio::View equalView(equalData, static_cast<zserio::UInt8>(1));
    zserio::View lessThenView(equalData, static_cast<zserio::UInt8>(0));
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST(EmptyChoiceTest, validate)
{
    EmptyChoice data;
    zserio::View view(data, static_cast<zserio::UInt8>(0));
    EXPECT_NO_THROW(zserio::detail::validate(view));
}

TEST(EmptyChoiceTest, bitSizeOf)
{
    EmptyChoice data;
    zserio::View view(data, static_cast<zserio::UInt8>(0));
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view));
}

TEST(EmptyChoiceTest, writeRead)
{
    EmptyChoice data;
    test_utils::writeReadTest(data, static_cast<zserio::UInt8>(0));
}

TEST(EmptyChoiceTest, read)
{
    EmptyChoice expectedReadData;
    test_utils::readTest(
            [](zserio::BitStreamWriter&) {
            },
            expectedReadData, static_cast<zserio::UInt8>(0));
}

TEST(EmptyChoiceTest, stdHash)
{
    std::hash<EmptyChoice> dataHasher;
    EmptyChoice data;
    const size_t dataHash = 23; // hardcoded value to check that the hash code is stable
    ASSERT_EQ(dataHash, dataHasher(data));

    std::hash<zserio::View<EmptyChoice>> viewHasher;
    zserio::View view(data, static_cast<zserio::UInt8>(0));
    const size_t viewHash = dataHash;
    ASSERT_EQ(viewHash, viewHasher(view));
}

} // namespace empty_choice
} // namespace choice_types
