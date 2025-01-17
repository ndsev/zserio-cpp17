#include "constraints/structure_constraints/BasicColor.h"
#include "constraints/structure_constraints/ExtendedColor.h"
#include "constraints/structure_constraints/StructureConstraints.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ConstraintException.h"

namespace constraints
{
namespace structure_constraints
{

class StructureConstraintsTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, BasicColor blackColor, BasicColor whiteColor,
            ExtendedColor purpleColor)
    {
        writer.writeUnsignedBits32(zserio::enumToValue(blackColor), 8);
        writer.writeBool(true);
        writer.writeUnsignedBits32(zserio::enumToValue(whiteColor), 8);
        writer.writeBool(true);
        writer.writeUnsignedBits32(zserio::enumToValue(purpleColor), 16);
    }
};

TEST_F(StructureConstraintsTest, readCorrectConstraints)
{
    const BasicColor blackColor = BasicColor::BLACK;
    const BasicColor whiteColor = BasicColor::WHITE;
    const ExtendedColor purpleColor = ExtendedColor::PURPLE;
    StructureConstraints data(blackColor, whiteColor, true, purpleColor);

    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, blackColor, whiteColor, purpleColor), data);
}

TEST_F(StructureConstraintsTest, readWrongBlackConstraint)
{
    const BasicColor blackColor = BasicColor::RED;
    const BasicColor whiteColor = BasicColor::WHITE;
    const ExtendedColor purpleColor = ExtendedColor::PURPLE;
    StructureConstraints data(blackColor, whiteColor, true, purpleColor);

    ASSERT_THROW(
            test_utils::readTest(
                    std::bind(writeData, std::placeholders::_1, blackColor, whiteColor, purpleColor), data),
            zserio::ConstraintException);
}

TEST_F(StructureConstraintsTest, readWrongWhiteConstraint)
{
    const BasicColor blackColor = BasicColor::BLACK;
    const BasicColor whiteColor = BasicColor::RED;
    const ExtendedColor purpleColor = ExtendedColor::PURPLE;
    StructureConstraints data(blackColor, whiteColor, true, purpleColor);

    ASSERT_THROW(
            test_utils::readTest(
                    std::bind(writeData, std::placeholders::_1, blackColor, whiteColor, purpleColor), data),
            zserio::ConstraintException);
}

TEST_F(StructureConstraintsTest, readWrongPurpleConstraint)
{
    const BasicColor blackColor = BasicColor::BLACK;
    const BasicColor whiteColor = BasicColor::WHITE;
    const ExtendedColor purpleColor = ExtendedColor::LIME;
    StructureConstraints data(blackColor, whiteColor, true, purpleColor);

    ASSERT_THROW(
            test_utils::readTest(
                    std::bind(writeData, std::placeholders::_1, blackColor, whiteColor, purpleColor), data),
            zserio::ConstraintException);
}

TEST_F(StructureConstraintsTest, writeCorrectConstraints)
{
    StructureConstraints data(BasicColor::BLACK, BasicColor::WHITE, true, ExtendedColor::PURPLE);
    test_utils::writeReadTest(data);
}

TEST_F(StructureConstraintsTest, writeWrongBlackConstraint)
{
    StructureConstraints data(BasicColor::RED, BasicColor::WHITE, true, ExtendedColor::PURPLE);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

TEST_F(StructureConstraintsTest, writeWrongWhiteConstraint)
{
    StructureConstraints data(BasicColor::BLACK, BasicColor::RED, true, ExtendedColor::PURPLE);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

TEST_F(StructureConstraintsTest, writeWrongPurpleConstraint)
{
    StructureConstraints data(BasicColor::BLACK, BasicColor::WHITE, true, ExtendedColor::LIME);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

} // namespace structure_constraints
} // namespace constraints
