#include "expressions/array_type/ArrayTypeExpression.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"

namespace expressions
{
namespace array_type
{

using AllocatorType = ArrayTypeExpression::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

TEST(ArrayTypeTest, bitSizeOfWithOptional)
{
    ArrayTypeExpression data;
    const size_t arrayLength = 2;
    VectorType<zserio::Int8> array(arrayLength);
    data.array = array;
    data.isZerosArrayValid = true;

    zserio::View view(data);

    const size_t arrayTypeExpressionBitSizeWithOptional = 17;
    ASSERT_EQ(arrayTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(ArrayTypeTest, bitSizeOfWithoutOptional)
{
    ArrayTypeExpression data;
    const size_t arrayLength = 2;
    VectorType<zserio::Int8> array;
    array.reserve(arrayLength);
    for (size_t i = 1; i <= arrayLength; ++i)
    {
        array.push_back(static_cast<int8_t>(i));
    }
    data.array = array;

    zserio::View view(data);

    const size_t arrayTypeExpressionBitSizeWithoutOptional = 16;
    ASSERT_EQ(arrayTypeExpressionBitSizeWithoutOptional, zserio::detail::bitSizeOf(view));
}

} // namespace array_type
} // namespace expressions
