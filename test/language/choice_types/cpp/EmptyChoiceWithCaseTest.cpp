#include "choice_types/empty_choice_with_case/EmptyChoiceWithCase.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace choice_types
{
namespace empty_choice_with_case
{

using ChoiceTag = EmptyChoiceWithCase::ChoiceTag;
using AllocatorType = EmptyChoiceWithCase::AllocatorType;

TEST(EmptyChoiceWithCaseTest, constructors)
{
    {
        EmptyChoiceWithCase data;
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        EmptyChoiceWithCase data = {};
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        EmptyChoiceWithCase data(AllocatorType{});
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        EmptyChoiceWithCase data;
        zserio::View view(data, zserio::UInt8(0));
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, view.zserioChoiceTag());
    }
}

TEST(EmptyChoiceWithCaseTest, comparisonOperators)
{
    EmptyChoiceWithCase data;
    EmptyChoiceWithCase equalData;
    EmptyChoiceWithCase lessThenData;
    test_utils::comparisonOperatorsTest(data, equalData);

    zserio::View view(data, zserio::UInt8(1));
    zserio::View equalView(equalData, zserio::UInt8(1));
    zserio::View lessThenView(lessThenData, zserio::UInt8(0));
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST(EmptyChoiceWithCaseTest, bitSizeOf)
{
    EmptyChoiceWithCase data;
    zserio::View view(data, zserio::UInt8(0));
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view));
}

TEST(EmptyChoiceWithCaseTest, writeRead)
{
    EmptyChoiceWithCase data;

    test_utils::writeReadTest(data, zserio::UInt8(0));
    test_utils::writeReadTest(data, zserio::UInt8(1));
}

TEST(EmptyChoiceWithCaseTest, read)
{
    EmptyChoiceWithCase data;

    test_utils::readTest(
            [](zserio::BitStreamWriter&) {
            },
            data, zserio::UInt8(0));
    test_utils::readTest(
            [](zserio::BitStreamWriter&) {
            },
            data, zserio::UInt8(1));
}

TEST(EmptyChoiceWithCaseTest, stdHash)
{
    // hardcoded values used to check that the hash code is stable

    EmptyChoiceWithCase data;
    EmptyChoiceWithCase equalData;
    EmptyChoiceWithCase diffData;
    const size_t dataHash = 23;

    test_utils::hashTest(data, dataHash, equalData);

    zserio::View view(data, zserio::UInt8(0));
    const size_t viewHash = 851;
    zserio::View equalView(equalData, zserio::UInt8(0));
    zserio::View diffView(diffData, zserio::UInt8(1));
    const size_t diffViewHash = 852;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace empty_choice_with_case
} // namespace choice_types
