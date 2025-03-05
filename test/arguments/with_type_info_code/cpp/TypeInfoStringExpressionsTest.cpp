#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "type_info_string_expressions/TypeInfoStringExpressions.h"
#include "zserio/SerializeUtil.h"

namespace type_info_string_expressions
{

using AllocatorType = TypeInfoStringExpressions::AllocatorType;
using ITypeInfoType = zserio::IBasicTypeInfo<AllocatorType>;

class TypeInfoStringExpressionsTest : public ::testing::Test
{
protected:
    static void fill(TypeInfoStringExpressions& data)
    {
        TestStruct testStruct;
        testStruct.arrayField.resize(LENGTHOF_LITERAL * 2);
        testStruct.dynBitField = 0;
        testStruct.bitField = 0;
        testStruct.enumField = TestEnum::ONE;
        testStruct.bitmaskField = TestBitmask::Values::READ;
        data.choiceField.emplace<TestChoice::ChoiceTag::CHOICE_structField>(testStruct);
    }

private:
    static constexpr size_t LENGTHOF_LITERAL = 7;
};

TEST_F(TypeInfoStringExpressionsTest, writeRead)
{
    TypeInfoStringExpressions data;
    fill(data);

    test_utils::writeReadTest(data);
}

TEST_F(TypeInfoStringExpressionsTest, typeInfo)
{
    const ITypeInfoType& typeInfo = zserio::typeInfo<TypeInfoStringExpressions>();

    // choiceField
    const auto& choiceFieldInfo = typeInfo.getFields()[0];
    ASSERT_EQ("::std::string_view{\"literal\"}.size()", choiceFieldInfo.typeArguments[0]);

    // TestChoice
    const auto& choiceTypeInfo = choiceFieldInfo.typeInfo;
    ASSERT_EQ("selector() + ::std::string_view{\"literal\"}.size()", choiceTypeInfo.getSelector());
    ASSERT_EQ("0 + ::std::string_view{\"literal\"}.size()", choiceTypeInfo.getCases()[0].caseExpressions[0]);

    // structField
    const auto& structFieldInfo = choiceTypeInfo.getFields()[1];
    ASSERT_EQ("::std::string_view{\"literal\"}.size()", structFieldInfo.typeArguments[0]);

    // TestStruct
    const auto& structTypeInfo = structFieldInfo.typeInfo;

    // arrayField
    const auto& arrayFieldInfo = structTypeInfo.getFields()[0];
    ASSERT_EQ("param() + ::std::string_view{\"literal\"}.size()", arrayFieldInfo.arrayLength);
    ASSERT_EQ("arrayField().size() > ::std::string_view{\"literal\"}.size()", arrayFieldInfo.constraint);

    // dynBitField
    const auto& dynBitFieldInfo = structTypeInfo.getFields()[1];
    ASSERT_EQ("bit:7", dynBitFieldInfo.typeInfo.getSchemaName());
    ASSERT_EQ(zserio::SchemaType::UINT7, dynBitFieldInfo.typeInfo.getSchemaType());
    ASSERT_EQ(zserio::CppType::UINT8, dynBitFieldInfo.typeInfo.getCppType());
    ASSERT_EQ("param() + ::std::string_view{\"literal\"}.size() != 0", dynBitFieldInfo.optionalCondition);

    // bitField
    const auto& bitFieldInfo = structTypeInfo.getFields()[2];
    ASSERT_EQ("::std::string_view{\"literal\"}.size() + 1", bitFieldInfo.alignment);

    // enumField
    const auto& enumFieldInfo = structTypeInfo.getFields()[3];

    // TestEnum
    const auto& enumTypeInfo = enumFieldInfo.typeInfo;
    ASSERT_EQ("int:7", enumTypeInfo.getUnderlyingType().getSchemaName());
    ASSERT_EQ(zserio::SchemaType::INT7, enumTypeInfo.getUnderlyingType().getSchemaType());
    ASSERT_EQ(zserio::CppType::INT8, enumTypeInfo.getUnderlyingType().getCppType());
    ASSERT_EQ(7, enumTypeInfo.getUnderlyingType().getBitSize());

    // bitmaskField
    const auto& bitmaskFieldInfo = structTypeInfo.getFields()[4];

    // TestBitmask
    const auto& bitmaskTypeInfo = bitmaskFieldInfo.typeInfo;
    ASSERT_EQ("bit:7", bitmaskTypeInfo.getUnderlyingType().getSchemaName());
    ASSERT_EQ(zserio::SchemaType::UINT7, bitmaskTypeInfo.getUnderlyingType().getSchemaType());
    ASSERT_EQ(zserio::CppType::UINT8, bitmaskTypeInfo.getUnderlyingType().getCppType());
    ASSERT_EQ(7, bitmaskTypeInfo.getUnderlyingType().getBitSize());
}

} // namespace type_info_string_expressions
