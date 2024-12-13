#include "expressions/left_shift_operator/LeftShiftOperator.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace left_shift_operator
{

TEST(LeftShiftOperatorTest, defaultValues)
{
    LeftShiftOperator data;
    EXPECT_EQ(40, data.u32);
    EXPECT_EQ(-40, data.i32);
    EXPECT_EQ(32, data.u32Complex);
    EXPECT_EQ(-32, data.i32Complex);
    EXPECT_EQ(24, data.u32Plus);
    EXPECT_EQ(-64, data.i32Minus);
    EXPECT_EQ(12, data.u32PlusRhsExpr);
    EXPECT_EQ(-24, data.i32MinusRhsExpr);
    EXPECT_EQ(11534336, data.u63Complex);
    EXPECT_EQ(-9216, data.i64Complex);
}

TEST(LeftShiftOperatorTest, getU63LShift3)
{
    LeftShiftOperator data;
    ASSERT_EQ(104, zserio::View(data).getU63LShift3());
}

TEST(LeftShiftOperatorTest, getI64LShift4)
{
    LeftShiftOperator data;
    ASSERT_EQ(-208, zserio::View(data).getI64LShift4());
}

TEST(LeftShiftOperatorTest, getU63LShift)
{
    LeftShiftOperator data;
    ASSERT_EQ(13312, zserio::View(data).getU63LShift());
}

TEST(LeftShiftOperatorTest, getI64LShift)
{
    LeftShiftOperator data;
    ASSERT_EQ(-13312, zserio::View(data).getI64LShift());
}

TEST(LeftShiftOperatorTest, getPositiveI32LShift)
{
    LeftShiftOperator data;
    ASSERT_EQ(13312, zserio::View(data).getPositiveI32LShift());
}

TEST(LeftShiftOperatorTest, getI64ComplexLShift)
{
    LeftShiftOperator data;
    ASSERT_EQ(-3072, zserio::View(data).getI64ComplexLShift());
}

} // namespace left_shift_operator
} // namespace expressions
