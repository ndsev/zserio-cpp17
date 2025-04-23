#include "choice_types/empty_choice/EmptyChoice.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace choice_types
{
namespace empty_choice
{

using AllocatorType = EmptyChoice::AllocatorType;

TEST(EmptyChoiceTest, constructors)
{
    {
        EmptyChoice data;
        ASSERT_EQ(EmptyChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyChoice data = {};
        ASSERT_EQ(EmptyChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyChoice data(AllocatorType{});
        ASSERT_EQ(EmptyChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyChoice data;
        zserio::View view(data, static_cast<zserio::UInt8>(0));
        ASSERT_EQ(EmptyChoice::Tag::ZSERIO_UNDEFINED, view.zserioChoiceTag());
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
    ASSERT_EQ(EmptyChoice::Tag::ZSERIO_UNDEFINED, view.zserioChoiceTag());
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
    EmptyChoice data;
    const size_t dataHash = 23; // hardcoded value to check that the hash code is stable
    EmptyChoice equalData;
    EmptyChoice diffData;
    test_utils::hashTest(data, dataHash, equalData);

    zserio::View view(data, zserio::UInt8(0));
    const size_t viewHash = 851; // hardcoded value to check that the hash code is stable
    zserio::View equalView(equalData, zserio::UInt8(0));
    zserio::View diffView(diffData, zserio::UInt8(1));
    const size_t diffViewHash = 852; // hardcoded value to check that the hash code is stable
    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace empty_choice
} // namespace choice_types
