#include "expressions/isset_operator/IsSetOperator.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace isset_operator
{

TEST(IsSetOperatorTest, hasNone)
{
    IsSetOperator data;
    zserio::View view(data);

    ASSERT_FALSE(view.hasInt());
    ASSERT_FALSE(view.hasString());
    ASSERT_FALSE(view.hasBoth());
    ASSERT_FALSE(view.parameterized().hasInt());
    ASSERT_FALSE(view.parameterized().hasString());
    ASSERT_FALSE(view.parameterized().hasBoth());

    ASSERT_FALSE(view.parameterized().intField());
    ASSERT_FALSE(view.parameterized().stringField());
}

TEST(IsSetOperatorTest, hasInt)
{
    IsSetOperator data;
    data.testBitmask = TestBitmask::Values::INT;
    data.parameterized.intField = 13;
    zserio::View view(data);

    ASSERT_TRUE(view.hasInt());
    ASSERT_FALSE(view.hasString());
    ASSERT_FALSE(view.hasBoth());
    ASSERT_TRUE(view.parameterized().hasInt());
    ASSERT_FALSE(view.parameterized().hasString());
    ASSERT_FALSE(view.parameterized().hasBoth());

    ASSERT_TRUE(view.parameterized().intField());
    ASSERT_FALSE(view.parameterized().stringField());
}

TEST(IsSetOperatorTest, hasString)
{
    IsSetOperator data;
    data.testBitmask = TestBitmask::Values::STRING;
    data.parameterized.stringField = "test";
    zserio::View view(data);

    ASSERT_FALSE(view.hasInt());
    ASSERT_TRUE(view.hasString());
    ASSERT_FALSE(view.hasBoth());
    ASSERT_FALSE(view.parameterized().hasInt());
    ASSERT_TRUE(view.parameterized().hasString());
    ASSERT_FALSE(view.parameterized().hasBoth());

    ASSERT_FALSE(view.parameterized().intField());
    ASSERT_TRUE(view.parameterized().stringField());
}

TEST(IsSetOperatorTest, hasBoth)
{
    IsSetOperator data;
    data.testBitmask = TestBitmask::Values::BOTH;
    data.parameterized.intField = 13;
    data.parameterized.stringField = "test";
    zserio::View view(data);

    ASSERT_TRUE(view.hasInt());
    ASSERT_TRUE(view.hasString());
    ASSERT_TRUE(view.hasBoth());
    ASSERT_TRUE(view.parameterized().hasInt());
    ASSERT_TRUE(view.parameterized().hasString());
    ASSERT_TRUE(view.parameterized().hasBoth());

    ASSERT_TRUE(view.parameterized().intField());
    ASSERT_TRUE(view.parameterized().stringField());
}

} // namespace isset_operator
} // namespace expressions
