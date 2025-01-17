#include "constraints/structure_bitmask_constraints/Availability.h"
#include "constraints/structure_bitmask_constraints/StructureBitmaskConstraints.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ConstraintException.h"

namespace constraints
{
namespace structure_bitmask_constraints
{

class StructureBitmaskConstraintsTest : public ::testing::Test
{
protected:
    static void writeData(
            zserio::BitStreamWriter& writer, Availability mask, uint8_t coordX, uint8_t coordY, uint8_t coordZ)
    {
        writer.writeUnsignedBits32(mask.getValue(), 3);
        writer.writeUnsignedBits32(coordX, 8);
        writer.writeUnsignedBits32(coordY, 8);
        writer.writeUnsignedBits32(coordZ, 8);
    }
};

TEST_F(StructureBitmaskConstraintsTest, readCorrectConstraints)
{
    Availability availability =
            Availability::Values::COORD_X | Availability::Values::COORD_Y | Availability::Values::COORD_Z;
    StructureBitmaskConstraints data(availability, 1, 1, 1);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, availability, 1, 1, 1), data);
}

TEST_F(StructureBitmaskConstraintsTest, readWrongCoordZConstraint)
{
    Availability availability = Availability::Values::COORD_X | Availability::Values::COORD_Y;
    StructureBitmaskConstraints data(availability, 1, 1, 1);

    ASSERT_THROW(test_utils::readTest(std::bind(writeData, std::placeholders::_1, availability, 1, 1, 1), data),
            zserio::ConstraintException);
}

TEST_F(StructureBitmaskConstraintsTest, readWrongCoordYConstraint)
{
    Availability availability = Availability::Values::COORD_X | Availability::Values::COORD_Z;
    StructureBitmaskConstraints data(availability, 1, 1, 1);

    ASSERT_THROW(test_utils::readTest(std::bind(writeData, std::placeholders::_1, availability, 1, 1, 1), data),
            zserio::ConstraintException);
}

TEST_F(StructureBitmaskConstraintsTest, readWrongCoordXConstraint)
{
    Availability availability = Availability::Values::COORD_Y | Availability::Values::COORD_Z;
    StructureBitmaskConstraints data(availability, 1, 1, 1);

    ASSERT_THROW(test_utils::readTest(std::bind(writeData, std::placeholders::_1, availability, 1, 1, 1), data),
            zserio::ConstraintException);
}

TEST_F(StructureBitmaskConstraintsTest, writeCorrectConstraints)
{
    StructureBitmaskConstraints data;
    data.availability = Availability::Values::COORD_X | Availability::Values::COORD_Y;
    data.coordX = 1;
    data.coordY = 1;
    data.coordZ = 0;

    test_utils::writeReadTest(data);
}

TEST_F(StructureBitmaskConstraintsTest, writeWrongCoordZConstraint)
{
    StructureBitmaskConstraints data;
    data.availability = Availability::Values::COORD_X | Availability::Values::COORD_Y;
    data.coordX = 1;
    data.coordY = 1;
    data.coordZ = 1;

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

TEST_F(StructureBitmaskConstraintsTest, writeWrongCoordYConstraint)
{
    StructureBitmaskConstraints data;
    data.availability = Availability::Values::COORD_X | Availability::Values::COORD_Z;
    data.coordX = 1;
    data.coordY = 1;
    data.coordZ = 1;

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

TEST_F(StructureBitmaskConstraintsTest, writeWrongCoordXConstraint)
{
    StructureBitmaskConstraints data;
    data.availability = Availability::Values::COORD_Y | Availability::Values::COORD_Z;
    data.coordX = 1;
    data.coordY = 1;
    data.coordZ = 1;

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

} // namespace structure_bitmask_constraints
} // namespace constraints
