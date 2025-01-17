#include "default_values/structure_default_values/BasicColor.h"
#include "default_values/structure_default_values/Permission.h"
#include "default_values/structure_default_values/StructureDefaultValues.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace default_values
{
namespace structure_default_values
{

TEST(StructureDefaultValuesTest, checkDefaultBoolValue)
{
    StructureDefaultValues data;
    ASSERT_EQ(true, data.boolValue);
}

TEST(StructureDefaultValuesTest, checkDefaultBit4Value)
{
    StructureDefaultValues data;
    ASSERT_EQ(0x0F, data.bit4Value);
}

TEST(StructureDefaultValuesTest, checkDefaultInt16Value)
{
    StructureDefaultValues data;
    ASSERT_EQ(0x0BEE, data.int16Value);
}

TEST(StructureDefaultValuesTest, checkDefaultFloat16Value)
{
    StructureDefaultValues data;
    float diff = 1.23F - data.float16Value;
    if (diff < 0.0F)
    {
        diff = -diff;
    }
    ASSERT_TRUE(diff <= std::numeric_limits<float>::epsilon());
}

TEST(StructureDefaultValuesTest, checkDefaultFloat32Value)
{
    StructureDefaultValues data;
    float diff = 1.234F - data.float32Value;
    if (diff < 0.0F)
    {
        diff = -diff;
    }
    ASSERT_TRUE(diff <= std::numeric_limits<float>::epsilon());
}

TEST(StructureDefaultValuesTest, checkDefaultFloat64Value)
{
    StructureDefaultValues data;
    double diff = 1.2345 - data.float64Value;
    if (diff < 0.0)
    {
        diff = -diff;
    }
    ASSERT_TRUE(diff <= std::numeric_limits<double>::epsilon());
}

TEST(StructureDefaultValuesTest, checkDefaultStringValue)
{
    StructureDefaultValues data;
    ASSERT_EQ("string", data.stringValue);
}

TEST(StructureDefaultValuesTest, checkDefaultEnumValue)
{
    StructureDefaultValues data;
    ASSERT_EQ(BasicColor::BLACK, data.enumValue);
}

TEST(StructureDefaultValuesTest, checkDefaultBitmaskValue)
{
    StructureDefaultValues data;
    ASSERT_EQ(Permission::Values::READ_WRITE, data.bitmaskValue);
}

TEST(StructureDefaultValuesTest, comparisonOperators)
{
    StructureDefaultValues data1;
    StructureDefaultValues data2;
    test_utils::comparisonOperatorsTest(data1, data2);
}

TEST(StructureDefaultValuesTest, writeRead)
{
    // Zserio equality operator doesn't work due double arithmetic
    StructureDefaultValues data;
    zserio::View view(data);

    auto bitBuffer = zserio::serialize(view);
    StructureDefaultValues readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);

    ASSERT_EQ(data.boolValue, readData.boolValue);
    ASSERT_EQ(data.bit4Value, readData.bit4Value);
    ASSERT_EQ(data.int16Value, readData.int16Value);
    ASSERT_EQ(data.stringValue, readData.stringValue);
    ASSERT_EQ(data.enumValue, readData.enumValue);
    ASSERT_EQ(data.bitmaskValue, readData.bitmaskValue);

    ASSERT_EQ(view.boolValue(), readView.boolValue());
    ASSERT_EQ(view.bit4Value(), readView.bit4Value());
    ASSERT_EQ(view.int16Value(), readView.int16Value());
    ASSERT_EQ(view.stringValue(), readView.stringValue());
    ASSERT_EQ(view.enumValue(), readView.enumValue());
    ASSERT_EQ(view.bitmaskValue(), readView.bitmaskValue());
}

} // namespace structure_default_values
} // namespace default_values
