#include "choice_types/full_enum_param_choice/FullEnumParamChoice.h"
#include "choice_types/full_enum_param_choice/Selector.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ChoiceCaseException.h"

namespace choice_types
{
namespace full_enum_param_choice
{

using AllocatorType = FullEnumParamChoice::AllocatorType;
using ChoiceTag = FullEnumParamChoice::ChoiceTag;

class FullEnumParamChoiceTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, Selector selector, int32_t value)
    {
        switch (selector)
        {
        case Selector::BLACK:
            writer.writeSignedBits32(value, 8);
            break;

        case Selector::GREY:
            writer.writeSignedBits32(value, 16);
            break;

        case Selector::WHITE:
            writer.writeSignedBits32(value, 32);
            break;

        default:
            FAIL() << "Bad choice selector";
        }
    }
};

TEST_F(FullEnumParamChoiceTest, constructor)
{
    {
        FullEnumParamChoice data;
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        FullEnumParamChoice data = {};
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        FullEnumParamChoice data(AllocatorType{});
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        const uint8_t value = 99;
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_black>, value);
        zserio::View view(data, Selector::BLACK);
        ASSERT_EQ(ChoiceTag::CHOICE_black, view.zserioChoiceTag());
    }
}

TEST_F(FullEnumParamChoiceTest, selector)
{
    const Selector selector = Selector::BLACK;
    FullEnumParamChoice data;
    zserio::View view(data, selector);
    ASSERT_EQ(selector, view.selector());
}

TEST_F(FullEnumParamChoiceTest, black)
{
    const int8_t value = 99;
    FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_black>, value);

    const Selector selector = Selector::BLACK;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.black());
}

TEST_F(FullEnumParamChoiceTest, grey)
{
    const int16_t value = 234;
    FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_grey>, value);

    const Selector selector = Selector::GREY;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.grey());
}

TEST_F(FullEnumParamChoiceTest, white)
{
    const int32_t value = 65535;
    FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_white>, value);

    const Selector selector = Selector::WHITE;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.white());
}

TEST_F(FullEnumParamChoiceTest, zserioChoiceTag)
{
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_black>, 0);
        zserio::View view(data, Selector::BLACK);
        ASSERT_EQ(ChoiceTag::CHOICE_black, data.index());
        ASSERT_EQ(ChoiceTag::CHOICE_black, view.zserioChoiceTag());
    }
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_grey>, 0);
        zserio::View view(data, Selector::GREY);
        ASSERT_EQ(ChoiceTag::CHOICE_grey, data.index());
        ASSERT_EQ(ChoiceTag::CHOICE_grey, view.zserioChoiceTag());
    }
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_white>, 0);
        zserio::View view(data, Selector::WHITE);
        ASSERT_EQ(ChoiceTag::CHOICE_white, data.index());
        ASSERT_EQ(ChoiceTag::CHOICE_white, view.zserioChoiceTag());
    }
}

TEST_F(FullEnumParamChoiceTest, comparisonOperators)
{
    FullEnumParamChoice data;
    data.emplace<ChoiceTag::CHOICE_white>(2);
    FullEnumParamChoice equalData;
    equalData.emplace<ChoiceTag::CHOICE_white>(2);
    FullEnumParamChoice lessThenData;
    lessThenData.emplace<ChoiceTag::CHOICE_black>(2);
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data, Selector::WHITE);
    zserio::View equalView(equalData, Selector::WHITE);
    zserio::View lessThenView(lessThenData, Selector::BLACK);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST_F(FullEnumParamChoiceTest, validate)
{
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_black>, 2);
        zserio::View viewB(data, Selector::BLACK);
        ASSERT_NO_THROW(zserio::detail::validate(viewB));
        zserio::View viewW(data, Selector::GREY);
        ASSERT_THROW(zserio::detail::validate(viewW), zserio::ChoiceCaseException);
        zserio::View viewBW(data, Selector::WHITE);
        ASSERT_THROW(zserio::detail::validate(viewBW), zserio::ChoiceCaseException);
    }
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_grey>, 2);
        zserio::View viewW(data, Selector::GREY);
        ASSERT_NO_THROW(zserio::detail::validate(viewW));
        zserio::View viewB(data, Selector::BLACK);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
        zserio::View viewBW(data, Selector::WHITE);
        ASSERT_THROW(zserio::detail::validate(viewBW), zserio::ChoiceCaseException);
    }
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_white>, 2);
        zserio::View viewBW(data, Selector::WHITE);
        ASSERT_NO_THROW(zserio::detail::validate(viewBW));
        zserio::View viewB(data, Selector::BLACK);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
        zserio::View viewW(data, Selector::GREY);
        ASSERT_THROW(zserio::detail::validate(viewW), zserio::ChoiceCaseException);
    }
}

TEST_F(FullEnumParamChoiceTest, bitSizeOf)
{
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_black>, 0);
        zserio::View view(data, Selector::BLACK);
        ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
    }
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_grey>, 1);
        zserio::View view(data, Selector::GREY);
        ASSERT_EQ(16, zserio::detail::bitSizeOf(view));
    }
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_white>, 2);
        zserio::View view(data, Selector::WHITE);
        ASSERT_EQ(32, zserio::detail::bitSizeOf(view));
    }
}

TEST_F(FullEnumParamChoiceTest, writeRead)
{
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_black>, 99);
        test_utils::writeReadTest(data, Selector(Selector::BLACK));
    }
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_grey>, 234);
        test_utils::writeReadTest(data, Selector(Selector::GREY));
    }
    {
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_white>, 65535);
        test_utils::writeReadTest(data, Selector(Selector::WHITE));
    }
}

TEST_F(FullEnumParamChoiceTest, read)
{
    {
        Selector selector = Selector::BLACK;
        const int8_t value = 99;
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_black>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
    {
        Selector selector = Selector::GREY;
        const int16_t value = 234;
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_grey>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
    {
        Selector selector = Selector::WHITE;
        const int32_t value = 65535;
        FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_white>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
}

TEST_F(FullEnumParamChoiceTest, stdHash)
{
    // hardcoded values used to check that the hash code is stable

    FullEnumParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_black>, 99);
    const size_t dataHash = 31623;
    FullEnumParamChoice equalData(zserio::in_place_index<ChoiceTag::CHOICE_black>, 99);
    FullEnumParamChoice diffData(zserio::in_place_index<ChoiceTag::CHOICE_black>, 100);
    const size_t diffDataHash = 31624;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data, Selector::BLACK);
    const size_t viewHash = 63073;
    zserio::View equalView(equalData, Selector::BLACK);
    zserio::View diffView(diffData, Selector::BLACK);
    const size_t diffViewHash = 63074;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace full_enum_param_choice
} // namespace choice_types
