#include "default_values/structure_default_values/BasicColor.h"
#include "default_values/structure_default_values/Permission.h"
#include "default_values/structure_default_values/StructureDefaultValues.h"
#include "gtest/gtest.h"
#include "zserio/BitStreamReader.h"
#include "zserio/CppRuntimeException.h"

namespace default_values
{
namespace structure_default_values
{

TEST(StructureDefaultValuesDataTest, checkDefaultBoolValue)
{
    StructureDefaultValues structureDefaultValues;
    ASSERT_EQ(true, structureDefaultValues.boolValue);
}

TEST(StructureDefaultValuesDataTest, checkDefaultBit4Value)
{
    StructureDefaultValues structureDefaultValues;
    ASSERT_EQ(0x0F, structureDefaultValues.bit4Value);
}

TEST(StructureDefaultValuesDataTest, checkDefaultInt16Value)
{
    StructureDefaultValues structureDefaultValues;
    ASSERT_EQ(0x0BEE, structureDefaultValues.int16Value);
}

TEST(StructureDefaultValuesDataTest, checkDefaultFloat16Value)
{
    StructureDefaultValues structureDefaultValues;
    float diff = 1.23F - structureDefaultValues.float16Value;
    if (diff < 0.0F)
    {
        diff = -diff;
    }
    ASSERT_TRUE(diff <= std::numeric_limits<float>::epsilon());
}

TEST(StructureDefaultValuesDataTest, checkDefaultFloat32Value)
{
    StructureDefaultValues structureDefaultValues;
    float diff = 1.234F - structureDefaultValues.float32Value;
    if (diff < 0.0F)
    {
        diff = -diff;
    }
    ASSERT_TRUE(diff <= std::numeric_limits<float>::epsilon());
}

TEST(StructureDefaultValuesDataTest, checkDefaultFloat64Value)
{
    StructureDefaultValues structureDefaultValues;
    double diff = 1.2345 - structureDefaultValues.float64Value;
    if (diff < 0.0)
    {
        diff = -diff;
    }
    ASSERT_TRUE(diff <= std::numeric_limits<double>::epsilon());
}

TEST(StructureDefaultValuesDataTest, checkDefaultStringValue)
{
    StructureDefaultValues structureDefaultValues;
    ASSERT_EQ("string", structureDefaultValues.stringValue);
}

TEST(StructureDefaultValuesDataTest, checkDefaultEnumValue)
{
    StructureDefaultValues structureDefaultValues;
    ASSERT_EQ(BasicColor::BLACK, structureDefaultValues.enumValue);
}

TEST(StructureDefaultValuesDataTest, checkDefaultBitmaskValue)
{
    StructureDefaultValues structureDefaultValues;
    ASSERT_EQ(Permission::Values::READ_WRITE, structureDefaultValues.bitmaskValue);
}

TEST(StructureDefaultValuesDataTest, operatorEquality)
{
    StructureDefaultValues structureDefaultValues1;
    StructureDefaultValues structureDefaultValues2;
    ASSERT_EQ(structureDefaultValues1, structureDefaultValues2);
}

TEST(StructureDefaultValuesDataTest, operatorLessThan)
{
    StructureDefaultValues structureDefaultValues1;
    StructureDefaultValues structureDefaultValues2;
    ASSERT_FALSE(structureDefaultValues1 < structureDefaultValues2);
    ASSERT_FALSE(structureDefaultValues2 < structureDefaultValues1);
}

} // namespace structure_default_values
} // namespace default_values
