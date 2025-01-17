#include "constraints/union_constraints/UnionConstraints.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ConstraintException.h"

namespace constraints
{
namespace union_constraints
{

using ChoiceTag = UnionConstraints::ChoiceTag;

class UnionConstraintsTest : public ::testing::Test
{
protected:
    static void writeData8(zserio::BitStreamWriter& writer, uint8_t value8)
    {
        writer.writeVarSize(static_cast<uint32_t>(ChoiceTag::CHOICE_value8) - 1);
        writer.writeUnsignedBits32(value8, 8);
    }

    static void writeData16(zserio::BitStreamWriter& writer, uint16_t value16)
    {
        writer.writeVarSize(static_cast<uint32_t>(ChoiceTag::CHOICE_value16) - 1);
        writer.writeUnsignedBits32(value16, 16);
    }

    static constexpr uint8_t VALUE8_CORRECT_CONSTRAINT = 1;
    static constexpr uint8_t VALUE8_WRONG_CONSTRAINT = 0;

    static constexpr uint16_t VALUE16_CORRECT_CONSTRAINT = 256;
    static constexpr uint16_t VALUE16_WRONG_CONSTRAINT = 255;
};

TEST_F(UnionConstraintsTest, readCorrectValue8Constraint)
{
    UnionConstraints data(zserio::in_place_index<ChoiceTag::CHOICE_value8>, VALUE8_CORRECT_CONSTRAINT);

    test_utils::readTest(std::bind(writeData8, std::placeholders::_1, VALUE8_CORRECT_CONSTRAINT), data);
}

TEST_F(UnionConstraintsTest, readCorrectValue16Constraint)
{
    UnionConstraints data(zserio::in_place_index<ChoiceTag::CHOICE_value16>, VALUE16_CORRECT_CONSTRAINT);

    test_utils::readTest(std::bind(writeData16, std::placeholders::_1, VALUE16_CORRECT_CONSTRAINT), data);
}

TEST_F(UnionConstraintsTest, readWrongValue8Constraint)
{
    UnionConstraints data(zserio::in_place_index<ChoiceTag::CHOICE_value8>, VALUE8_WRONG_CONSTRAINT);

    ASSERT_THROW(
            test_utils::readTest(std::bind(writeData8, std::placeholders::_1, VALUE8_WRONG_CONSTRAINT), data),
            zserio::ConstraintException);
}

TEST_F(UnionConstraintsTest, readWrongValue16Constraint)
{
    UnionConstraints data(zserio::in_place_index<ChoiceTag::CHOICE_value16>, VALUE16_WRONG_CONSTRAINT);

    ASSERT_THROW(
            test_utils::readTest(std::bind(writeData16, std::placeholders::_1, VALUE16_WRONG_CONSTRAINT), data),
            zserio::ConstraintException);
}

TEST_F(UnionConstraintsTest, writeCorrectValue8Constraint)
{
    UnionConstraints data(zserio::in_place_index<ChoiceTag::CHOICE_value8>, VALUE8_CORRECT_CONSTRAINT);

    test_utils::writeReadTest(data);
}

TEST_F(UnionConstraintsTest, writeCorrectValue16Constraint)
{
    UnionConstraints data(zserio::in_place_index<ChoiceTag::CHOICE_value16>, VALUE16_CORRECT_CONSTRAINT);

    test_utils::writeReadTest(data);
}

TEST_F(UnionConstraintsTest, writeWrongValue8Constraint)
{
    UnionConstraints data(zserio::in_place_index<ChoiceTag::CHOICE_value8>, VALUE8_WRONG_CONSTRAINT);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

TEST_F(UnionConstraintsTest, writeWrongValue16Constraint)
{
    UnionConstraints data(zserio::in_place_index<ChoiceTag::CHOICE_value16>, VALUE16_WRONG_CONSTRAINT);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

} // namespace union_constraints
} // namespace constraints
