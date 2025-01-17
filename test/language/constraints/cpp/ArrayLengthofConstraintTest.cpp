#include "constraints/array_lengthof_constraint/ArrayLengthofConstraint.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ConstraintException.h"
#include "zserio/RebindAlloc.h"

namespace constraints
{
namespace array_lengthof_constraint
{

using AllocatorType = ArrayLengthofConstraint::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class ArrayLengthofConstraintTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, size_t length)
    {
        // all lengths in this test fits in a single byte
        writer.writeUnsignedBits32(static_cast<uint32_t>(length), 8);
        for (size_t i = 0; i < length; ++i)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(i), 32);
        }
    }

    void fillData(ArrayLengthofConstraint& data, size_t length)
    {
        for (size_t i = 0; i < length; ++i)
        {
            data.array.push_back(static_cast<uint32_t>(i));
        }
    }

    static constexpr size_t CORRECT_LENGTH = 6;
    static constexpr size_t WRONG_LENGTH_LESS = 3;
    static constexpr size_t WRONG_LENGTH_GREATER = 12;
};

TEST_F(ArrayLengthofConstraintTest, readCorrectLength)
{
    const size_t length = CORRECT_LENGTH;
    ArrayLengthofConstraint data;
    fillData(data, length);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, length), data);
}

TEST_F(ArrayLengthofConstraintTest, readWrongLengthLess)
{
    const size_t length = WRONG_LENGTH_LESS;
    ArrayLengthofConstraint data;
    fillData(data, length);

    ASSERT_THROW(test_utils::readTest(std::bind(writeData, std::placeholders::_1, length), data),
            zserio::ConstraintException);
}

TEST_F(ArrayLengthofConstraintTest, readWrongLengthGreater)
{
    const size_t length = WRONG_LENGTH_GREATER;
    ArrayLengthofConstraint data;
    fillData(data, length);

    ASSERT_THROW(test_utils::readTest(std::bind(writeData, std::placeholders::_1, length), data),
            zserio::ConstraintException);
}

TEST_F(ArrayLengthofConstraintTest, writeCorrectLength)
{
    ArrayLengthofConstraint data;
    fillData(data, CORRECT_LENGTH);

    test_utils::writeReadTest(data);
}

TEST_F(ArrayLengthofConstraintTest, writeWrongLengthLess)
{
    ArrayLengthofConstraint data;
    fillData(data, WRONG_LENGTH_LESS);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);

    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

TEST_F(ArrayLengthofConstraintTest, writeWrongLengthGreater)
{
    ArrayLengthofConstraint data;
    fillData(data, WRONG_LENGTH_GREATER);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::ConstraintException);

    ASSERT_THROW(test_utils::writeReadTest(data), zserio::ConstraintException);
}

} // namespace array_lengthof_constraint
} // namespace constraints
