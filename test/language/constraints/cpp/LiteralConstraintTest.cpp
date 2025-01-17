#include "constraints/literal_constraint/LiteralConstraint.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ConstraintException.h"
#include "zserio/RebindAlloc.h"

namespace constraints
{
namespace literal_constraint
{

class LiteralConstraintTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, int32_t value)
    {
        writer.writeSignedBits32(value, 32);
    }

    static constexpr int32_t CORRECT_VALUE = 6;
    static constexpr int32_t WRONG_VALUE_ZERO = 0;
    static constexpr int32_t WRONG_VALUE_LESS = -268435456;
    static constexpr int32_t WRONG_VALUE_GREATER = 268435456;
};

TEST_F(LiteralConstraintTest, readCorrectValue)
{
    LiteralConstraint data(CORRECT_VALUE);
    test_utils::readTest(std::bind(writeData, std::placeholders::_1, CORRECT_VALUE), data);
}

TEST_F(LiteralConstraintTest, readWrongValueZero)
{
    LiteralConstraint data(WRONG_VALUE_ZERO);
    ASSERT_THROW(test_utils::readTest(std::bind(writeData, std::placeholders::_1, WRONG_VALUE_ZERO), data),
            zserio::ConstraintException);
}

TEST_F(LiteralConstraintTest, readWrongValueLess)
{
    LiteralConstraint data(WRONG_VALUE_LESS);
    ASSERT_THROW(test_utils::readTest(std::bind(writeData, std::placeholders::_1, WRONG_VALUE_LESS), data),
            zserio::ConstraintException);
}

TEST_F(LiteralConstraintTest, readWrongValueGreater)
{
    LiteralConstraint data(WRONG_VALUE_GREATER);
    ASSERT_THROW(test_utils::readTest(std::bind(writeData, std::placeholders::_1, WRONG_VALUE_GREATER), data),
            zserio::ConstraintException);
}

TEST_F(LiteralConstraintTest, writeCorrectValue)
{
    LiteralConstraint data(CORRECT_VALUE);
    test_utils::writeReadTest(data);
}

TEST_F(LiteralConstraintTest, writeWrongValueZero)
{
    LiteralConstraint data(WRONG_VALUE_ZERO);
    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

TEST_F(LiteralConstraintTest, writeWrongValueLess)
{
    LiteralConstraint data(WRONG_VALUE_LESS);
    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

TEST_F(LiteralConstraintTest, writeWrongValueGreater)
{
    LiteralConstraint data(WRONG_VALUE_GREATER);
    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

} // namespace literal_constraint
} // namespace constraints
