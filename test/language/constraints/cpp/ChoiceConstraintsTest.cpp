#include "constraints/choice_constraints/ChoiceConstraints.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ConstraintException.h"

namespace constraints
{
namespace choice_constraints
{

class ChoiceConstraintsTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, bool selector, uint16_t value)
    {
        if (selector)
        {
            writer.writeUnsignedBits32(value, 8);
        }
        else
        {
            writer.writeUnsignedBits32(value, 16);
        }
    }

    static constexpr uint8_t VALUE8_CORRECT_CONSTRAINT = 1;
    static constexpr uint8_t VALUE8_WRONG_CONSTRAINT = 0;

    static constexpr uint16_t VALUE16_CORRECT_CONSTRAINT = 256;
    static constexpr uint16_t VALUE16_WRONG_CONSTRAINT = 255;
};

TEST_F(ChoiceConstraintsTest, readCorrectValue8Constraint)
{
    const zserio::Bool selector = true;
    ChoiceConstraints data(zserio::in_place_index<ChoiceConstraints::Tag::value8>, VALUE8_CORRECT_CONSTRAINT);

    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, selector, VALUE8_CORRECT_CONSTRAINT), data, selector);
}

TEST_F(ChoiceConstraintsTest, readWrongValue8Constraint)
{
    const zserio::Bool selector = true;
    ChoiceConstraints data(zserio::in_place_index<ChoiceConstraints::Tag::value8>, VALUE8_WRONG_CONSTRAINT);

    ASSERT_THROW(
            test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, VALUE8_WRONG_CONSTRAINT),
                    data, selector),
            zserio::ConstraintException);
}

TEST_F(ChoiceConstraintsTest, readCorrectValue16Constraint)
{
    const zserio::Bool selector = false;
    ChoiceConstraints data(zserio::in_place_index<ChoiceConstraints::Tag::value16>, VALUE16_CORRECT_CONSTRAINT);

    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, selector, VALUE16_CORRECT_CONSTRAINT), data, selector);
}

TEST_F(ChoiceConstraintsTest, readWrongValue16Constraint)
{
    const zserio::Bool selector = false;
    ChoiceConstraints data(zserio::in_place_index<ChoiceConstraints::Tag::value16>, VALUE16_WRONG_CONSTRAINT);

    ASSERT_THROW(test_utils::readTest(
                         std::bind(writeData, std::placeholders::_1, selector, VALUE16_WRONG_CONSTRAINT), data,
                         selector),
            zserio::ConstraintException);
}

TEST_F(ChoiceConstraintsTest, writeCorrectValue8Constraint)
{
    const zserio::Bool selector = true;
    ChoiceConstraints data(zserio::in_place_index<ChoiceConstraints::Tag::value8>, VALUE8_CORRECT_CONSTRAINT);

    test_utils::writeReadTest(data, selector);
}

TEST_F(ChoiceConstraintsTest, writeWrongValue8Constraint)
{
    const zserio::Bool selector = true;
    ChoiceConstraints data(zserio::in_place_index<ChoiceConstraints::Tag::value8>, VALUE8_WRONG_CONSTRAINT);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data, selector)), zserio::ConstraintException);

    ASSERT_THROW(test_utils::writeReadTest(data, selector), zserio::ConstraintException);
}

TEST_F(ChoiceConstraintsTest, writeCorrectValue16Constraint)
{
    const zserio::Bool selector = false;
    ChoiceConstraints data(zserio::in_place_index<ChoiceConstraints::Tag::value16>, VALUE16_CORRECT_CONSTRAINT);

    test_utils::writeReadTest(data, selector);
}

TEST_F(ChoiceConstraintsTest, writeWrongValue16Constraint)
{
    const zserio::Bool selector = false;
    ChoiceConstraints data(zserio::in_place_index<ChoiceConstraints::Tag::value16>, VALUE16_WRONG_CONSTRAINT);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data, selector)), zserio::ConstraintException);

    ASSERT_THROW(test_utils::writeReadTest(data, selector), zserio::ConstraintException);
}

} // namespace choice_constraints
} // namespace constraints
