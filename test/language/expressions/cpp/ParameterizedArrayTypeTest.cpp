#include "expressions/parameterized_array_type/ParameterizedArrayTypeExpression.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"

namespace expressions
{
namespace parameterized_array_type
{

using AllocatorType = ParameterizedArrayTypeExpression::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

TEST(ParameterizedArrayTypeTest, bitSizeOfWithOptional)
{
    ParameterizedArrayHolder holder;
    const size_t arrayLength = 2;
    VectorType<ParameterizedArrayElement> array;
    array.reserve(arrayLength);
    for (size_t i = 1; i <= arrayLength; ++i)
    {
        ParameterizedArrayElement element;
        element.value1 = 0;
        array.push_back(element);
    }
    holder.array = array;

    ParameterizedArrayTypeExpression data;
    data.holder = holder;
    data.isValue1Zero = true;

    zserio::View view(data);

    ASSERT_NO_THROW(zserio::detail::validate(view));
    const size_t parameterizedArrayTypeExpressionBitSizeWithOptional = 33;
    ASSERT_EQ(parameterizedArrayTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(ParameterizedArrayTypeTest, bitSizeOfWithoutOptional)
{
    ParameterizedArrayHolder holder;
    const size_t arrayLength = 2;
    VectorType<ParameterizedArrayElement> array;
    array.reserve(arrayLength);
    for (size_t i = 1; i <= arrayLength; ++i)
    {
        ParameterizedArrayElement element;
        element.value1 = 1;
        array.push_back(element);
    }
    holder.array = array;

    ParameterizedArrayTypeExpression data;
    data.holder = holder;

    zserio::View view(data);

    ASSERT_NO_THROW(zserio::detail::validate(view));
    const size_t parameterizedArrayTypeExpressionBitSizeWithOptional = 32;
    ASSERT_EQ(parameterizedArrayTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

} // namespace parameterized_array_type
} // namespace expressions
