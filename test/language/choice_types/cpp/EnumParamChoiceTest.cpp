#include "choice_types/enum_param_choice/EnumParamChoice.h"
#include "choice_types/enum_param_choice/Selector.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ChoiceCaseException.h"

namespace choice_types
{
namespace enum_param_choice
{

using AllocatorType = EnumParamChoice::AllocatorType;

class EnumParamChoiceTest : public ::testing::Test
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

TEST_F(EnumParamChoiceTest, constructor)
{
    {
        EnumParamChoice data;
        ASSERT_EQ(EnumParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EnumParamChoice data = {};
        ASSERT_EQ(EnumParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        EnumParamChoice data(AllocatorType{});
        ASSERT_EQ(EnumParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        const uint8_t value = 99;
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::black>, value);
        zserio::View view(data, Selector::BLACK);
        ASSERT_EQ(EnumParamChoice::Tag::black, view.zserioChoiceTag());
    }
}

TEST_F(EnumParamChoiceTest, selector)
{
    const Selector selector = Selector::BLACK;
    EnumParamChoice data;
    zserio::View view(data, selector);
    ASSERT_EQ(selector, view.selector());
}

TEST_F(EnumParamChoiceTest, black)
{
    const int8_t value = 99;
    EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::black>, value);

    const Selector selector = Selector::BLACK;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.black());
}

TEST_F(EnumParamChoiceTest, grey)
{
    const int16_t value = 234;
    EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::grey>, value);

    const Selector selector = Selector::GREY;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.grey());
}

TEST_F(EnumParamChoiceTest, white)
{
    const int32_t value = 65535;
    EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::white>, value);

    const Selector selector = Selector::WHITE;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.white());
}

TEST_F(EnumParamChoiceTest, zserioChoiceTag)
{
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::black>, 0);
        zserio::View view(data, Selector::BLACK);
        ASSERT_EQ(EnumParamChoice::Tag::black, data.index());
        ASSERT_EQ(EnumParamChoice::Tag::black, view.zserioChoiceTag());
    }
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::grey>, 0);
        zserio::View view(data, Selector::GREY);
        ASSERT_EQ(EnumParamChoice::Tag::grey, data.index());
        ASSERT_EQ(EnumParamChoice::Tag::grey, view.zserioChoiceTag());
    }
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::white>, 0);
        zserio::View view(data, Selector::WHITE);
        ASSERT_EQ(EnumParamChoice::Tag::white, data.index());
        ASSERT_EQ(EnumParamChoice::Tag::white, view.zserioChoiceTag());
    }
}

TEST_F(EnumParamChoiceTest, comparisonOperators)
{
    EnumParamChoice data;
    data.emplace<EnumParamChoice::Tag::white>(2);
    EnumParamChoice equalData;
    equalData.emplace<EnumParamChoice::Tag::white>(2);
    EnumParamChoice lessThenData;
    lessThenData.emplace<EnumParamChoice::Tag::black>(2);
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data, Selector::WHITE);
    zserio::View equalView(equalData, Selector::WHITE);
    zserio::View lessThenView(lessThenData, Selector::BLACK);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST_F(EnumParamChoiceTest, validate)
{
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::black>, 2);
        zserio::View viewB(data, Selector::BLACK);
        ASSERT_NO_THROW(zserio::detail::validate(viewB));
        zserio::View viewW(data, Selector::GREY);
        ASSERT_THROW(zserio::detail::validate(viewW), zserio::ChoiceCaseException);
        zserio::View viewBW(data, Selector::WHITE);
        ASSERT_THROW(zserio::detail::validate(viewBW), zserio::ChoiceCaseException);
    }
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::grey>, 2);
        zserio::View viewW(data, Selector::GREY);
        ASSERT_NO_THROW(zserio::detail::validate(viewW));
        zserio::View viewB(data, Selector::BLACK);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
        zserio::View viewBW(data, Selector::WHITE);
        ASSERT_THROW(zserio::detail::validate(viewBW), zserio::ChoiceCaseException);
    }
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::white>, 2);
        zserio::View viewBW(data, Selector::WHITE);
        ASSERT_NO_THROW(zserio::detail::validate(viewBW));
        zserio::View viewB(data, Selector::BLACK);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
        zserio::View viewW(data, Selector::GREY);
        ASSERT_THROW(zserio::detail::validate(viewW), zserio::ChoiceCaseException);
    }
}

TEST_F(EnumParamChoiceTest, bitSizeOf)
{
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::black>, 0);
        zserio::View view(data, Selector::BLACK);
        ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
    }
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::grey>, 1);
        zserio::View view(data, Selector::GREY);
        ASSERT_EQ(16, zserio::detail::bitSizeOf(view));
    }
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::white>, 2);
        zserio::View view(data, Selector::WHITE);
        ASSERT_EQ(32, zserio::detail::bitSizeOf(view));
    }
}

TEST_F(EnumParamChoiceTest, writeRead)
{
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::black>, 99);
        test_utils::writeReadTest(data, Selector(Selector::BLACK));
    }
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::grey>, 234);
        test_utils::writeReadTest(data, Selector(Selector::GREY));
    }
    {
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::white>, 65535);
        test_utils::writeReadTest(data, Selector(Selector::WHITE));
    }
}

TEST_F(EnumParamChoiceTest, read)
{
    {
        Selector selector = Selector::BLACK;
        const Black value = 99;
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::black>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
    {
        Selector selector = Selector::GREY;
        const Grey value = 234;
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::grey>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
    {
        Selector selector = Selector::WHITE;
        const White value = 65535;
        EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::white>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
}

TEST_F(EnumParamChoiceTest, stdHash)
{
    // hardcoded values used to check that the hash code is stable

    EnumParamChoice data(zserio::in_place_index<EnumParamChoice::Tag::black>, 99);
    const size_t dataHash = 31623;
    EnumParamChoice equalData(zserio::in_place_index<EnumParamChoice::Tag::black>, 99);
    EnumParamChoice diffData(zserio::in_place_index<EnumParamChoice::Tag::black>, 100);
    const size_t diffDataHash = 31624;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data, Selector::BLACK);
    const size_t viewHash = 63073;
    zserio::View equalView(equalData, Selector::BLACK);
    zserio::View diffView(diffData, Selector::BLACK);
    const size_t diffViewHash = 63074;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace enum_param_choice
} // namespace choice_types
