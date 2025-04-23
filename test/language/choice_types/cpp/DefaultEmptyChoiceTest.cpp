#include "choice_types/default_empty_choice/DefaultEmptyChoice.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace choice_types
{
namespace default_empty_choice
{

class DefaultEmptyChoiceTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, zserio::UInt3 tag, int32_t value)
    {
        switch (tag)
        {
        case 1:
            writer.writeSignedBits32(value, 8);
            break;

        case 2:
            writer.writeSignedBits32(value, 16);
            break;

        default:
            break;
        }
    }

    static constexpr zserio::UInt3 VARIANT_A_SELECTOR = 1;
    static constexpr zserio::UInt3 VARIANT_B_SELECTOR = 2;
    static constexpr zserio::UInt3 DEFAULT_SELECTOR = 3;
};

TEST_F(DefaultEmptyChoiceTest, choiceTag)
{
    {
        DefaultEmptyChoice data(zserio::in_place_index<DefaultEmptyChoice::Tag::valueA>, 99);
        zserio::View view(data, VARIANT_A_SELECTOR);
        ASSERT_EQ(DefaultEmptyChoice::Tag::valueA, data.index());
        ASSERT_EQ(DefaultEmptyChoice::Tag::valueA, view.zserioChoiceTag());
    }
    {
        DefaultEmptyChoice data(zserio::in_place_index<DefaultEmptyChoice::Tag::valueB>, 234);
        zserio::View view(data, VARIANT_B_SELECTOR);
        ASSERT_EQ(DefaultEmptyChoice::Tag::valueB, data.index());
        ASSERT_EQ(DefaultEmptyChoice::Tag::valueB, view.zserioChoiceTag());
    }
    {
        DefaultEmptyChoice data;
        zserio::View view(data, DEFAULT_SELECTOR);
        ASSERT_EQ(DefaultEmptyChoice::Tag::ZSERIO_UNDEFINED, data.index());
        ASSERT_EQ(DefaultEmptyChoice::Tag::ZSERIO_UNDEFINED, view.zserioChoiceTag());
    }
}

TEST_F(DefaultEmptyChoiceTest, writeRead)
{
    {
        const zserio::UInt3 tag = VARIANT_A_SELECTOR;
        const VariantA value = 99;
        DefaultEmptyChoice data(zserio::in_place_index<DefaultEmptyChoice::Tag::valueA>, value);

        test_utils::writeReadTest(data, tag);
    }
    {
        const zserio::UInt3 tag = VARIANT_B_SELECTOR;
        const VariantB value = 234;
        DefaultEmptyChoice data(zserio::in_place_index<DefaultEmptyChoice::Tag::valueB>, value);

        test_utils::writeReadTest(data, tag);
    }
    {
        const zserio::UInt3 tag = DEFAULT_SELECTOR;
        DefaultEmptyChoice data;

        test_utils::writeReadTest(data, tag);
    }
}

TEST_F(DefaultEmptyChoiceTest, read)
{
    {
        const zserio::UInt3 tag = VARIANT_A_SELECTOR;
        const VariantA value = 99;
        DefaultEmptyChoice data(zserio::in_place_index<DefaultEmptyChoice::Tag::valueA>, value);

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, tag, value), data, tag);
    }
    {
        const zserio::UInt3 tag = VARIANT_B_SELECTOR;
        const VariantB value = 234;
        DefaultEmptyChoice data(zserio::in_place_index<DefaultEmptyChoice::Tag::valueB>, value);

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, tag, value), data, tag);
    }

    {
        const zserio::UInt3 tag = DEFAULT_SELECTOR;
        DefaultEmptyChoice data;

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, tag, 0), data, tag);
    }
}

} // namespace default_empty_choice
} // namespace choice_types
