#include "constraints/optional_constraints/OptionalConstraints.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ConstraintException.h"

namespace constraints
{
namespace optional_constraints
{

class OptionalConstraintsTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, zserio::UInt32 field,
            zserio::Optional<zserio::UInt32> autoOptionalField, zserio::Optional<zserio::UInt32> optionalField)
    {
        zserio::detail::write(writer, field);
        if (autoOptionalField)
        {
            writer.writeBool(true);
            zserio::detail::write(writer, *autoOptionalField);
        }
        else
        {
            writer.writeBool(false);
        }
        if (field > 10)
        {
            zserio::detail::write(writer, *optionalField);
        }
    }
};

TEST_F(OptionalConstraintsTest, readCorrectBothOptionals)
{
    zserio::UInt32 field = 13;
    zserio::UInt32 autoOptionalField = 14;
    zserio::UInt32 optionalField = 15;
    OptionalConstraints data(field, autoOptionalField, optionalField);

    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, field, autoOptionalField, optionalField), data);
}

TEST_F(OptionalConstraintsTest, readCorrectAutoOptional)
{
    zserio::UInt32 field = 9;
    zserio::UInt32 autoOptionalField = 10;
    OptionalConstraints data(field, autoOptionalField, std::nullopt);

    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, field, autoOptionalField, std::nullopt), data);
}

TEST_F(OptionalConstraintsTest, readCorrectOptional)
{
    zserio::UInt32 field = 13;
    zserio::UInt32 optionalField = 15;

    OptionalConstraints data(field, std::nullopt, optionalField);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, field, std::nullopt, optionalField), data);
}

TEST_F(OptionalConstraintsTest, readWrongBothOptionals)
{
    zserio::UInt32 field = 13;
    zserio::UInt32 autoOptionalField = 12;
    zserio::UInt32 optionalField = 11;
    OptionalConstraints data(field, autoOptionalField, optionalField);

    ASSERT_THROW(
            test_utils::readTest(
                    std::bind(writeData, std::placeholders::_1, field, autoOptionalField, optionalField), data),
            zserio::ConstraintException);
}

TEST_F(OptionalConstraintsTest, readWrongAutoOptional)
{
    zserio::UInt32 field = 9;
    zserio::UInt32 autoOptionalField = 8;
    OptionalConstraints data(field, autoOptionalField, std::nullopt);

    ASSERT_THROW(
            test_utils::readTest(
                    std::bind(writeData, std::placeholders::_1, field, autoOptionalField, std::nullopt), data),
            zserio::ConstraintException);
}

TEST_F(OptionalConstraintsTest, readWrongOptional)
{
    zserio::UInt32 field = 13;
    zserio::UInt32 optionalField = 11;
    OptionalConstraints data(field, std::nullopt, optionalField);

    ASSERT_THROW(test_utils::readTest(
                         std::bind(writeData, std::placeholders::_1, field, std::nullopt, optionalField), data),
            zserio::ConstraintException);
}

TEST_F(OptionalConstraintsTest, writeCorrectBothOptionals)
{
    zserio::UInt32 field = 13;
    zserio::UInt32 autoOptionalField = 14;
    zserio::UInt32 optionalField = 15;
    OptionalConstraints data(field, autoOptionalField, optionalField);

    test_utils::writeReadTest(data);
}

TEST_F(OptionalConstraintsTest, writeCorrectAutoOptional)
{
    zserio::UInt32 field = 9;
    zserio::UInt32 autoOptionalField = 10;
    OptionalConstraints data(field, autoOptionalField, std::nullopt);

    test_utils::writeReadTest(data);
}

TEST_F(OptionalConstraintsTest, writeCorrectOptional)
{
    zserio::UInt32 field = 13;
    zserio::UInt32 optionalField = 15;
    OptionalConstraints data(field, std::nullopt, optionalField);

    test_utils::writeReadTest(data);
}

TEST_F(OptionalConstraintsTest, writeWrongBothOptionals)
{
    zserio::UInt32 field = 13;
    zserio::UInt32 autoOptionalField = 12;
    zserio::UInt32 optionalField = 11;
    OptionalConstraints data(field, autoOptionalField, optionalField);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

TEST_F(OptionalConstraintsTest, writeWrongAutoOptional)
{
    zserio::UInt32 field = 9;
    zserio::UInt32 autoOptionalField = 8;
    OptionalConstraints data(field, autoOptionalField, std::nullopt);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

TEST_F(OptionalConstraintsTest, writeWrongOptional)
{
    zserio::UInt32 field = 13;
    zserio::UInt32 optionalField = 11;
    OptionalConstraints data(field, std::nullopt, optionalField);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

} // namespace optional_constraints
} // namespace constraints
