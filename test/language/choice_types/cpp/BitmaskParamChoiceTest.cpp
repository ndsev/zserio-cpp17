#include "choice_types/bitmask_param_choice/BitmaskParamChoice.h"
#include "choice_types/bitmask_param_choice/Selector.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ChoiceCaseException.h"

namespace choice_types
{
namespace bitmask_param_choice
{

using AllocatorType = BitmaskParamChoice::allocator_type;

class BitmaskParamChoiceTest : public ::testing::Test
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

TEST_F(BitmaskParamChoiceTest, constructors)
{
    {
        BitmaskParamChoice data;
        ASSERT_EQ(BitmaskParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        BitmaskParamChoice data = {};
        ASSERT_EQ(BitmaskParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        BitmaskParamChoice data(AllocatorType{});
        ASSERT_EQ(BitmaskParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        const uint8_t value = 99;
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::black>, value);
        zserio::View view(data, Selector::Values::BLACK);
        ASSERT_EQ(BitmaskParamChoice::Tag::black, view.zserioChoiceTag());
    }
}

TEST_F(BitmaskParamChoiceTest, selector)
{
    const Selector selector = Selector::Values::BLACK;
    BitmaskParamChoice data;
    zserio::View view(data, selector);
    ASSERT_EQ(selector, view.selector());
}

TEST_F(BitmaskParamChoiceTest, black)
{
    const uint8_t value = 99;
    BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::black>, value);

    const Selector selector = Selector::Values::BLACK;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.black());
}

TEST_F(BitmaskParamChoiceTest, white)
{
    const uint8_t value = 234;
    BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::white>, value);

    const Selector selector = Selector::Values::WHITE;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.white());
}

TEST_F(BitmaskParamChoiceTest, blackAndWhite)
{
    const uint16_t value = 65535;
    BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::blackAndWhite>, value);

    const Selector selector = Selector::Values::BLACK_AND_WHITE;
    zserio::View view(data, selector);
    ASSERT_EQ(value, view.blackAndWhite());
}

TEST_F(BitmaskParamChoiceTest, zserioChoiceTag)
{
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::black>, 0);
        zserio::View view(data, Selector::Values::BLACK);
        ASSERT_EQ(BitmaskParamChoice::Tag::black, data.index());
        ASSERT_EQ(BitmaskParamChoice::Tag::black, view.zserioChoiceTag());
    }
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::white>, 0);
        zserio::View view(data, Selector::Values::WHITE);
        ASSERT_EQ(BitmaskParamChoice::Tag::white, data.index());
        ASSERT_EQ(BitmaskParamChoice::Tag::white, view.zserioChoiceTag());
    }
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::blackAndWhite>, 0);
        zserio::View view(data, Selector::Values::BLACK_AND_WHITE);
        ASSERT_EQ(BitmaskParamChoice::Tag::blackAndWhite, data.index());
        ASSERT_EQ(BitmaskParamChoice::Tag::blackAndWhite, view.zserioChoiceTag());
    }
}

TEST_F(BitmaskParamChoiceTest, comparisonOperators)
{
    BitmaskParamChoice data;
    data.emplace<BitmaskParamChoice::Tag::white>(2);
    BitmaskParamChoice equalData;
    equalData.emplace<BitmaskParamChoice::Tag::white>(2);
    BitmaskParamChoice lessThenData;
    lessThenData.emplace<BitmaskParamChoice::Tag::black>(2);
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data, Selector::Values::WHITE);
    zserio::View equalView(equalData, Selector::Values::WHITE);
    zserio::View lessThenView(lessThenData, Selector::Values::BLACK);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST_F(BitmaskParamChoiceTest, validate)
{
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::black>, 2);
        zserio::View viewB(data, Selector::Values::BLACK);
        ASSERT_NO_THROW(zserio::detail::validate(viewB));
        zserio::View viewW(data, Selector::Values::WHITE);
        ASSERT_THROW(zserio::detail::validate(viewW), zserio::ChoiceCaseException);
        zserio::View viewBW(data, Selector::Values::BLACK_AND_WHITE);
        ASSERT_THROW(zserio::detail::validate(viewBW), zserio::ChoiceCaseException);
    }
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::white>, 2);
        zserio::View viewW(data, Selector::Values::WHITE);
        ASSERT_NO_THROW(zserio::detail::validate(viewW));
        zserio::View viewB(data, Selector::Values::BLACK);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
        zserio::View viewBW(data, Selector::Values::BLACK_AND_WHITE);
        ASSERT_THROW(zserio::detail::validate(viewBW), zserio::ChoiceCaseException);
    }
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::blackAndWhite>, 2);
        zserio::View viewBW(data, Selector::Values::BLACK_AND_WHITE);
        ASSERT_NO_THROW(zserio::detail::validate(viewBW));
        zserio::View viewB(data, Selector::Values::BLACK);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
        zserio::View viewW(data, Selector::Values::WHITE);
        ASSERT_THROW(zserio::detail::validate(viewW), zserio::ChoiceCaseException);
    }
}

TEST_F(BitmaskParamChoiceTest, bitSizeOf)
{
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::black>, 0);
        zserio::View view(data, Selector::Values::BLACK);
        ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
    }
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::white>, 1);
        zserio::View view(data, Selector::Values::WHITE);
        ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
    }
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::blackAndWhite>, 2);
        zserio::View view(data, Selector::Values::BLACK_AND_WHITE);
        ASSERT_EQ(16, zserio::detail::bitSizeOf(view));
    }
}

TEST_F(BitmaskParamChoiceTest, writeRead)
{
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::black>, 99);
        test_utils::writeReadTest(data, Selector(Selector::Values::BLACK));
    }
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::white>, 234);
        test_utils::writeReadTest(data, Selector(Selector::Values::WHITE));
    }
    {
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::blackAndWhite>, 65535);
        test_utils::writeReadTest(data, Selector(Selector::Values::BLACK_AND_WHITE));
    }
}

TEST_F(BitmaskParamChoiceTest, read)
{
    {
        Selector selector = Selector::Values::BLACK;
        const uint8_t value = 99;
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::black>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
    {
        Selector selector = Selector::Values::WHITE;
        const uint8_t value = 234;
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::white>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
    {
        Selector selector = Selector::Values::BLACK_AND_WHITE;
        const uint16_t value = 65535;
        BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::blackAndWhite>, value);
        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
}

TEST_F(BitmaskParamChoiceTest, stdHash)
{
    // hardcoded values used to check that the hash code is stable

    BitmaskParamChoice data(zserio::in_place_index<BitmaskParamChoice::Tag::black>, 99);
    const size_t dataHash = 31623;
    BitmaskParamChoice equalData(zserio::in_place_index<BitmaskParamChoice::Tag::black>, 99);
    BitmaskParamChoice diffData(zserio::in_place_index<BitmaskParamChoice::Tag::black>, 100);
    const size_t diffDataHash = 31624;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data, Selector::Values::BLACK);
    const size_t viewHash = 63110;
    zserio::View equalView(equalData, Selector::Values::BLACK);
    zserio::View diffView(diffData, Selector::Values::BLACK);
    const size_t diffViewHash = 63111;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace bitmask_param_choice
} // namespace choice_types
