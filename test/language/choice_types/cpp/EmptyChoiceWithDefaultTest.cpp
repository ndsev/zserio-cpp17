#include "choice_types/empty_choice_with_default/EmptyChoiceWithDefault.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/CppRuntimeException.h"

namespace choice_types
{
namespace empty_choice_with_default
{

using AllocatorType = EmptyChoiceWithDefault::allocator_type;

TEST(EmptyChoiceWithDefaultTest, constructors)
{
    {
        EmptyChoiceWithDefault data;
        ASSERT_EQ(EmptyChoiceWithDefault::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyChoiceWithDefault data = {};
        ASSERT_EQ(EmptyChoiceWithDefault::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyChoiceWithDefault data(AllocatorType{});
        ASSERT_EQ(EmptyChoiceWithDefault::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EmptyChoiceWithDefault data;
        zserio::View view(data, zserio::UInt8(0));
        ASSERT_EQ(EmptyChoiceWithDefault::Tag::ZSERIO_UNDEFINED, view.zserioChoiceTag());
    }
}

TEST(EmptyChoiceWithDefaultTest, comparisonOperators)
{
    EmptyChoiceWithDefault data;
    EmptyChoiceWithDefault equalData;
    EmptyChoiceWithDefault lessThenData;
    test_utils::comparisonOperatorsTest(data, equalData);

    zserio::View view(data, zserio::UInt8(1));
    zserio::View equalView(equalData, zserio::UInt8(1));
    zserio::View lessThenView(lessThenData, zserio::UInt8(0));
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST(EmptyChoiceWithDefaultTest, bitSizeOf)
{
    EmptyChoiceWithDefault data;
    zserio::View view(data, zserio::UInt8(0));
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view));
}

TEST(EmptyChoiceWithDefaultTest, writeRead)
{
    EmptyChoiceWithDefault data;

    test_utils::writeReadTest(data, zserio::UInt8(0));
    test_utils::writeReadTest(data, zserio::UInt8(1));
}

TEST(EmptyChoiceWithDefaultTest, read)
{
    EmptyChoiceWithDefault data;

    test_utils::readTest(
            [](zserio::BitStreamWriter&) {
            },
            data, zserio::UInt8(0));
    test_utils::readTest(
            [](zserio::BitStreamWriter&) {
            },
            data, zserio::UInt8(1));
}

TEST(EmptyChoiceWithDefaultTest, hashCode)
{
    // hardcoded values used to check that the hash code is stable

    EmptyChoiceWithDefault data;
    EmptyChoiceWithDefault equalData;
    EmptyChoiceWithDefault diffData;
    const size_t dataHash = 23;

    test_utils::hashTest(data, dataHash, equalData);

    zserio::View view(data, zserio::UInt8(0));
    const size_t viewHash = 851;
    zserio::View equalView(equalData, zserio::UInt8(0));
    zserio::View diffView(diffData, zserio::UInt8(1));
    const size_t diffViewHash = 852;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace empty_choice_with_default
} // namespace choice_types
