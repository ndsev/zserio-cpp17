#include "default_values/extended_default_values/ExtendedDefaultValues.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace default_values
{
namespace extended_default_values
{

TEST(ExtendedDefaultValuesTest, checkNoDefaultU32Field)
{
    ExtendedDefaultValues data;
    zserio::View view(data);
    ASSERT_EQ(zserio::UInt32{}, data.noDefaultU32Field);
    ASSERT_EQ(zserio::UInt32{}, view.noDefaultU32Field());
}

TEST(ExtendedDefaultValuesTest, checkNoDefaultStringField)
{
    ExtendedDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.noDefaultStringField.empty());
    ASSERT_TRUE(view.noDefaultStringField().empty());
}

TEST(ExtendedDefaultValuesTest, checkExtendedDefaultBoolField)
{
    ExtendedDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.extendedDefaultBoolField.isPresent());
    ASSERT_TRUE(data.extendedDefaultBoolField.value());
    ASSERT_TRUE(view.extendedDefaultBoolField().isPresent());
    ASSERT_TRUE(view.extendedDefaultBoolField().value());
}

TEST(ExtendedDefaultValuesTest, checkExtendedDefaultStringField)
{
    ExtendedDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.extendedDefaultStringField.isPresent());
    ASSERT_EQ("default", *data.extendedDefaultStringField);
    ASSERT_TRUE(view.extendedDefaultStringField().isPresent());
    ASSERT_EQ("default", *view.extendedDefaultStringField());
}

TEST(ExtendedDefaultValuesTest, checkExtendedOptionalDefaultFloatField)
{
    ExtendedDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.extendedOptionalDefaultFloatField.isPresent());
    ASSERT_TRUE(data.extendedOptionalDefaultFloatField.value().has_value());
    ASSERT_EQ(1.234F, **data.extendedOptionalDefaultFloatField);
    ASSERT_TRUE(view.extendedOptionalDefaultFloatField().isPresent());
    ASSERT_TRUE(view.extendedOptionalDefaultFloatField().value().has_value());
    ASSERT_EQ(1.234F, **view.extendedOptionalDefaultFloatField());
}

TEST(ExtendedDefaultValuesTest, checkExtendedOptionalDefaultStringField)
{
    ExtendedDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.extendedOptionalDefaultStringField.isPresent());
    ASSERT_TRUE(data.extendedOptionalDefaultStringField.value().has_value());
    ASSERT_EQ("default", **data.extendedOptionalDefaultStringField);
    ASSERT_TRUE(view.extendedOptionalDefaultStringField().isPresent());
    ASSERT_TRUE(view.extendedOptionalDefaultStringField().value().has_value());
    ASSERT_EQ("default", **view.extendedOptionalDefaultStringField());
}

TEST(ExtendedDefaultValuesTest, checkExtendedNoDefaultU32Field)
{
    ExtendedDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.extendedNoDefaultU32Field.isPresent());
    ASSERT_EQ(zserio::UInt32{}, *data.extendedNoDefaultU32Field);
    ASSERT_TRUE(view.extendedNoDefaultU32Field().isPresent());
    ASSERT_EQ(zserio::UInt32{}, *view.extendedNoDefaultU32Field());
}

TEST(ExtendedDefaultValuesTest, checkExtendedNoDefaultExternField)
{
    ExtendedDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.extendedNoDefaultExternField.isPresent());
    ASSERT_EQ(0, data.extendedNoDefaultExternField->getBitSize());
    ASSERT_TRUE(view.extendedNoDefaultExternField().isPresent());
    ASSERT_EQ(0, view.extendedNoDefaultExternField()->get().getBitSize());
}

TEST(ExtendedDefaultValuesTest, checkExtendedOptionalNoDefaultU32Field)
{
    ExtendedDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.extendedOptionalNoDefaultU32Field.isPresent());
    ASSERT_FALSE(data.extendedOptionalNoDefaultU32Field.value().has_value());
    ASSERT_TRUE(view.extendedOptionalNoDefaultU32Field().isPresent());
    ASSERT_FALSE(view.extendedOptionalNoDefaultU32Field().value().has_value());
}

TEST(ExtendedDefaultValuesTest, checkExtendedOptionalNoDefaultBytesField)
{
    ExtendedDefaultValues data;
    zserio::View view(data);
    ASSERT_TRUE(data.extendedOptionalNoDefaultBytesField.isPresent());
    ASSERT_FALSE(data.extendedOptionalNoDefaultBytesField.value().has_value());
    ASSERT_TRUE(view.extendedOptionalNoDefaultBytesField().isPresent());
    ASSERT_FALSE(view.extendedOptionalNoDefaultBytesField().value().has_value());
}

TEST(ExtendedDefaultValuesTest, writeRead)
{
    ExtendedDefaultValues data;
    test_utils::writeReadTest(data);
}

} // namespace extended_default_values
} // namespace default_values
