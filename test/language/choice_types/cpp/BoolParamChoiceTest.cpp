#include "choice_types/bool_param_choice/BoolParamChoice.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ChoiceCaseException.h"

namespace choice_types
{
namespace bool_param_choice
{

using AllocatorType = BoolParamChoice::allocator_type;

class BoolParamChoiceTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, bool selector, int16_t value)
    {
        if (selector)
        {
            writer.writeSignedBits32(value, 8);
        }
        else
        {
            writer.writeSignedBits32(value, 16);
        }
    }
};

TEST_F(BoolParamChoiceTest, constructors)
{
    {
        BoolParamChoice data;
        ASSERT_EQ(BoolParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        BoolParamChoice data = {};
        ASSERT_EQ(BoolParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        BoolParamChoice data(AllocatorType{});
        ASSERT_EQ(BoolParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        BoolParamChoice data{zserio::in_place_index<BoolParamChoice::Tag::black>, 99};
        zserio::View view(data, true);
        ASSERT_EQ(BoolParamChoice::Tag::black, view.zserioChoiceTag());
    }
}

TEST_F(BoolParamChoiceTest, choiceTag)
{
    {
        BoolParamChoice data(zserio::in_place_index<BoolParamChoice::Tag::black>, 99);
        zserio::View view(data, true);
        ASSERT_EQ(BoolParamChoice::Tag::black, data.index());
        ASSERT_EQ(BoolParamChoice::Tag::black, view.zserioChoiceTag());
    }

    {
        BoolParamChoice data(zserio::in_place_index<BoolParamChoice::Tag::grey>, 234);
        zserio::View view(data, false);
        ASSERT_EQ(BoolParamChoice::Tag::grey, data.index());
        ASSERT_EQ(BoolParamChoice::Tag::grey, view.zserioChoiceTag());
    }
}

TEST_F(BoolParamChoiceTest, validate)
{
    {
        BoolParamChoice data(zserio::in_place_index<BoolParamChoice::Tag::black>, 99);
        ASSERT_NO_THROW(zserio::detail::validate(zserio::View(data, true)));
        ASSERT_THROW(zserio::detail::validate(zserio::View(data, false)), zserio::ChoiceCaseException);
    }
    {
        BoolParamChoice data(zserio::in_place_index<BoolParamChoice::Tag::grey>, 234);
        ASSERT_NO_THROW(zserio::detail::validate(zserio::View(data, false)));
        ASSERT_THROW(zserio::detail::validate(zserio::View(data, true)), zserio::ChoiceCaseException);
    }
}

TEST_F(BoolParamChoiceTest, bitSizeOf)
{
    {
        BoolParamChoice data(zserio::in_place_index<BoolParamChoice::Tag::black>, 99);
        zserio::View view(data, true);
        ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
    }
    {
        BoolParamChoice data(zserio::in_place_index<BoolParamChoice::Tag::grey>, 234);
        zserio::View view(data, false);
        ASSERT_EQ(16, zserio::detail::bitSizeOf(view));
    }
}

TEST_F(BoolParamChoiceTest, comparisonOperators)
{
    BoolParamChoice data;
    data.emplace<BoolParamChoice::Tag::grey>(234);
    BoolParamChoice equalData;
    equalData.emplace<BoolParamChoice::Tag::grey>(234);
    BoolParamChoice lessThenData;
    lessThenData.emplace<BoolParamChoice::Tag::grey>(233);
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data, false);
    zserio::View equalView(equalData, false);
    zserio::View lessThenView(lessThenData, false);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST_F(BoolParamChoiceTest, writeRead)
{
    {
        zserio::Bool selector = true;
        const int8_t value = 99;
        BoolParamChoice data(zserio::in_place_index<BoolParamChoice::Tag::black>, value);
        test_utils::writeReadTest(data, selector);
    }
    {
        zserio::Bool selector = false;
        const int16_t value = 234;
        BoolParamChoice data(zserio::in_place_index<BoolParamChoice::Tag::grey>, value);
        test_utils::writeReadTest(data, selector);
    }
}

TEST_F(BoolParamChoiceTest, read)
{
    {
        const zserio::Bool selector = true;
        const int8_t value = 99;
        BoolParamChoice data(zserio::in_place_index<BoolParamChoice::Tag::black>, value);

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
    {
        const zserio::Bool selector = false;
        const int16_t value = 234;
        BoolParamChoice data(zserio::in_place_index<BoolParamChoice::Tag::grey>, value);

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
}

TEST_F(BoolParamChoiceTest, stdHash)
{
    // hardcoded values used to check that the hash code is stable

    BoolParamChoice data(zserio::in_place_index<BoolParamChoice::Tag::black>, 99);
    const size_t dataHash = 31623;
    BoolParamChoice equalData(zserio::in_place_index<BoolParamChoice::Tag::black>, 99);
    BoolParamChoice diffData(zserio::in_place_index<BoolParamChoice::Tag::black>, 100);
    const size_t diffDataHash = 31624;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data, true);
    const size_t viewHash = 31623;
    zserio::View equalView(equalData, true);
    zserio::View diffView(diffData, true);
    const size_t diffViewHash = 31624;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace bool_param_choice
} // namespace choice_types
