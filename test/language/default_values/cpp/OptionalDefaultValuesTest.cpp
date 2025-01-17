#include "default_values/optional_default_values/OptionalDefaultValues.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace default_values
{
namespace optional_default_values
{

TEST(OptionalDefaultValuesTest, checkOptionalNoDefaultBoolField)
{
    OptionalDefaultValues data;
    zserio::View view(data);
    ASSERT_FALSE(data.optionalNoDefaultBoolField);
    ASSERT_FALSE(view.optionalNoDefaultBoolField());
}

TEST(OptionalDefaultValuesTest, checkOptionalNoDefaultStringField)
{
    OptionalDefaultValues data;
    zserio::View view(data);
    ASSERT_FALSE(data.optionalNoDefaultStringField);
    ASSERT_FALSE(view.optionalNoDefaultStringField());
}

TEST(OptionalDefaultValuesTest, checkOptionalDefaultU32Field)
{
    OptionalDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.optionalDefaultU32Field);
    ASSERT_EQ(13, *data.optionalDefaultU32Field);
    ASSERT_TRUE(view.optionalDefaultU32Field());
    ASSERT_EQ(13, *view.optionalDefaultU32Field());
}

TEST(OptionalDefaultValuesTest, checkOptionalDefaultF64Field)
{
    OptionalDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.optionalDefaultF64Field);
    ASSERT_EQ(1.234, *data.optionalDefaultF64Field);
    ASSERT_TRUE(view.optionalDefaultF64Field());
    ASSERT_EQ(1.234, *view.optionalDefaultF64Field());
}

TEST(OptionalDefaultValuesTest, checkOptionalDefaultStringField)
{
    OptionalDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.optionalDefaultStringField);
    ASSERT_EQ("default", *data.optionalDefaultStringField);
    ASSERT_TRUE(view.optionalDefaultStringField());
    ASSERT_EQ("default", *view.optionalDefaultStringField());
}

TEST(OptionalDefaultValuesTest, writeRead)
{
    OptionalDefaultValues data;
    test_utils::writeReadTest(data);
}

} // namespace optional_default_values
} // namespace default_values
