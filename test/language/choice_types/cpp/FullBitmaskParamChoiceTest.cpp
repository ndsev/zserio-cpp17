#include "choice_types/full_bitmask_param_choice/FullBitmaskParamChoice.h"
#include "choice_types/full_bitmask_param_choice/Selector.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ChoiceCaseException.h"

namespace choice_types
{
namespace full_bitmask_param_choice
{

using AllocatorType = FullBitmaskParamChoice::AllocatorType;

class FullBitmaskParamChoiceTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, Selector selector, uint16_t value)
    {
        switch (selector.getValue())
        {
        case static_cast<Selector::ZserioType::ValueType>(Selector::Values::BLACK):
        case static_cast<Selector::ZserioType::ValueType>(Selector::Values::WHITE):
            writer.writeUnsignedBits32(value, 8);
            break;

        case static_cast<Selector::ZserioType::ValueType>(Selector::Values::BLACK_AND_WHITE):
            writer.writeUnsignedBits32(value, 16);
            break;

        default:
            FAIL() << "Bad choice selector";
        }
    }
};

TEST_F(FullBitmaskParamChoiceTest, constructors)
{
    {
        FullBitmaskParamChoice data;
        ASSERT_EQ(FullBitmaskParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        FullBitmaskParamChoice data = {};
        ASSERT_EQ(FullBitmaskParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        FullBitmaskParamChoice data(AllocatorType{});
        ASSERT_EQ(FullBitmaskParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        const uint8_t value = 99;
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::black>, value);
        zserio::View view(data, Selector::Values::BLACK);
        ASSERT_EQ(FullBitmaskParamChoice::Tag::black, view.zserioChoiceTag());
    }
}

TEST_F(FullBitmaskParamChoiceTest, selector)
{
    const Selector selector = Selector::Values::BLACK;
    FullBitmaskParamChoice data;
    zserio::View view(data, selector);
    ASSERT_EQ(selector, view.selector());
}

TEST_F(FullBitmaskParamChoiceTest, black)
{
    const uint8_t value = 99;
    FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::black>, value);

    const Selector selector = Selector::Values::BLACK;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.black());
}

TEST_F(FullBitmaskParamChoiceTest, white)
{
    const uint8_t value = 234;
    FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::white>, value);

    const Selector selector = Selector::Values::WHITE;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.white());
}

TEST_F(FullBitmaskParamChoiceTest, blackAndWhite)
{
    const uint16_t value = 65535;
    FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::blackAndWhite>, value);

    const Selector selector = Selector::Values::BLACK_AND_WHITE;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.blackAndWhite());
}

TEST_F(FullBitmaskParamChoiceTest, zserioChoiceTag)
{
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::black>, 0);
        zserio::View view(data, Selector::Values::BLACK);
        ASSERT_EQ(FullBitmaskParamChoice::Tag::black, data.index());
        ASSERT_EQ(FullBitmaskParamChoice::Tag::black, view.zserioChoiceTag());
    }
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::white>, 0);
        zserio::View view(data, Selector::Values::WHITE);
        ASSERT_EQ(FullBitmaskParamChoice::Tag::white, data.index());
        ASSERT_EQ(FullBitmaskParamChoice::Tag::white, view.zserioChoiceTag());
    }
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::blackAndWhite>, 0);
        zserio::View view(data, Selector::Values::BLACK_AND_WHITE);
        ASSERT_EQ(FullBitmaskParamChoice::Tag::blackAndWhite, data.index());
        ASSERT_EQ(FullBitmaskParamChoice::Tag::blackAndWhite, view.zserioChoiceTag());
    }
}

TEST_F(FullBitmaskParamChoiceTest, comparisonOperators)
{
    FullBitmaskParamChoice data;
    data.emplace<FullBitmaskParamChoice::Tag::white>(2);
    FullBitmaskParamChoice equalData;
    equalData.emplace<FullBitmaskParamChoice::Tag::white>(2);
    FullBitmaskParamChoice lessThenData;
    lessThenData.emplace<FullBitmaskParamChoice::Tag::black>(2);
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data, Selector::Values::WHITE);
    zserio::View equalView(equalData, Selector::Values::WHITE);
    zserio::View lessThenView(lessThenData, Selector::Values::BLACK);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST_F(FullBitmaskParamChoiceTest, validate)
{
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::black>, 2);
        zserio::View viewB(data, Selector::Values::BLACK);
        ASSERT_NO_THROW(zserio::detail::validate(viewB));
        zserio::View viewW(data, Selector::Values::WHITE);
        ASSERT_THROW(zserio::detail::validate(viewW), zserio::ChoiceCaseException);
        zserio::View viewBW(data, Selector::Values::BLACK_AND_WHITE);
        ASSERT_THROW(zserio::detail::validate(viewBW), zserio::ChoiceCaseException);
    }
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::white>, 2);
        zserio::View viewW(data, Selector::Values::WHITE);
        ASSERT_NO_THROW(zserio::detail::validate(viewW));
        zserio::View viewB(data, Selector::Values::BLACK);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
        zserio::View viewBW(data, Selector::Values::BLACK_AND_WHITE);
        ASSERT_THROW(zserio::detail::validate(viewBW), zserio::ChoiceCaseException);
    }
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::blackAndWhite>, 2);
        zserio::View viewBW(data, Selector::Values::BLACK_AND_WHITE);
        ASSERT_NO_THROW(zserio::detail::validate(viewBW));
        zserio::View viewB(data, Selector::Values::BLACK);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
        zserio::View viewW(data, Selector::Values::WHITE);
        ASSERT_THROW(zserio::detail::validate(viewW), zserio::ChoiceCaseException);
    }
}

TEST_F(FullBitmaskParamChoiceTest, bitSizeOf)
{
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::black>, 0);
        zserio::View view(data, Selector::Values::BLACK);
        ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
    }
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::white>, 1);
        zserio::View view(data, Selector::Values::WHITE);
        ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
    }
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::blackAndWhite>, 2);
        zserio::View view(data, Selector::Values::BLACK_AND_WHITE);
        ASSERT_EQ(16, zserio::detail::bitSizeOf(view));
    }
}

TEST_F(FullBitmaskParamChoiceTest, writeRead)
{
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::black>, 99);
        test_utils::writeReadTest(data, Selector(Selector::Values::BLACK));
    }
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::white>, 234);
        test_utils::writeReadTest(data, Selector(Selector::Values::WHITE));
    }
    {
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::blackAndWhite>, 65535);
        test_utils::writeReadTest(data, Selector(Selector::Values::BLACK_AND_WHITE));
    }
}

TEST_F(FullBitmaskParamChoiceTest, read)
{
    {
        Selector selector = Selector::Values::BLACK;
        const uint8_t value = 99;
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::black>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
    {
        Selector selector = Selector::Values::WHITE;
        const uint8_t value = 234;
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::white>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
    {
        Selector selector = Selector::Values::BLACK_AND_WHITE;
        const uint16_t value = 65535;
        FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::blackAndWhite>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
}

TEST_F(FullBitmaskParamChoiceTest, stdHash)
{
    // hardcoded values used to check that the hash code is stable

    FullBitmaskParamChoice data(zserio::in_place_index<FullBitmaskParamChoice::Tag::black>, 99);
    const size_t dataHash = 31623;
    FullBitmaskParamChoice equalData(zserio::in_place_index<FullBitmaskParamChoice::Tag::black>, 99);
    FullBitmaskParamChoice diffData(zserio::in_place_index<FullBitmaskParamChoice::Tag::black>, 100);
    const size_t diffDataHash = 31624;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data, Selector::Values::BLACK);
    const size_t viewHash = 63110;
    zserio::View equalView(equalData, Selector::Values::BLACK);
    zserio::View diffView(diffData, Selector::Values::BLACK);
    const size_t diffViewHash = 63111;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace full_bitmask_param_choice
} // namespace choice_types
