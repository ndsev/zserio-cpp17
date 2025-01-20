#include "gtest/gtest.h"
#include "identifiers/structure_name_clashing_with_java/StructureNameClashingWithJava.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"

namespace identifiers
{
namespace structure_name_clashing_with_java
{

class StructureNameClashingWithJavaTest : public ::testing::Test
{
protected:
    static constexpr size_t BIT_SIZE = 8 * 1 + // all auto optionals
            8 + // Byte
            16 + // Short
            32 + // Integer
            64 + // Long
            64 + // BigInteger
            32 + // Float
            64 + // Double
            8; // String '\0'
};

TEST_F(StructureNameClashingWithJavaTest, emptyConstructor)
{
    {
        StructureNameClashingWithJava data;
        ASSERT_FALSE(data.byteField.value);
        ASSERT_FALSE(data.shortField.value);
        ASSERT_FALSE(data.integerField.value);
        ASSERT_FALSE(data.longField.value);
        ASSERT_FALSE(data.bigIntegerField.value);
        ASSERT_FALSE(data.floatField.value);
        ASSERT_FALSE(data.doubleField.value);
        ASSERT_FALSE(data.stringField.value);
    }

    {
        StructureNameClashingWithJava data = {};
        ASSERT_FALSE(data.byteField.value);
        ASSERT_FALSE(data.shortField.value);
        ASSERT_FALSE(data.integerField.value);
        ASSERT_FALSE(data.longField.value);
        ASSERT_FALSE(data.bigIntegerField.value);
        ASSERT_FALSE(data.floatField.value);
        ASSERT_FALSE(data.doubleField.value);
        ASSERT_FALSE(data.stringField.value);
    }

    {
        StructureNameClashingWithJava data = {};
        zserio::View view(data);
        ASSERT_FALSE(view.byteField().value());
        ASSERT_FALSE(view.shortField().value());
        ASSERT_FALSE(view.integerField().value());
        ASSERT_FALSE(view.longField().value());
        ASSERT_FALSE(view.bigIntegerField().value());
        ASSERT_FALSE(view.floatField().value());
        ASSERT_FALSE(view.doubleField().value());
        ASSERT_FALSE(view.stringField().value());
    }
}

TEST_F(StructureNameClashingWithJavaTest, bitSizeOf)
{
    StructureNameClashingWithJava data{Byte{zserio::Int8(0)}, Short{zserio::Int16(0)},
            Integer{zserio::Int32(0)}, Long{zserio::Int64(0)}, BigInteger{zserio::UInt64(0)},
            Float{zserio::Float32(0.0F)}, Double{zserio::Float64(0.0)}, String{zserio::String("")}};
    zserio::View view(data);

    ASSERT_EQ(BIT_SIZE, zserio::detail::bitSizeOf(view));
}

} // namespace structure_name_clashing_with_java
} // namespace identifiers
