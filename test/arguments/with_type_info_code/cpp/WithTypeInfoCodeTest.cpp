#include <fstream>

#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "with_type_info_code/SimplePubsub.h"
#include "with_type_info_code/SimpleService.h"
#include "with_type_info_code/SqlDatabase.h"
#include "zserio/RebindAlloc.h"

#include "WithTypeInfoCodeCreator.h"

namespace with_type_info_code
{

using AllocatorType = WithTypeInfoCode::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

using BitBufferType = zserio::BasicBitBuffer<zserio::RebindAlloc<AllocatorType, uint8_t>>;
using ITypeInfoType = zserio::IBasicTypeInfo<AllocatorType>;
using FieldInfoType = zserio::BasicFieldInfo<AllocatorType>;
using ParameterInfoType = zserio::BasicParameterInfo<AllocatorType>;
using FunctionInfoType = zserio::BasicFunctionInfo<AllocatorType>;
using CaseInfoType = zserio::BasicCaseInfo<AllocatorType>;
using ColumnInfoType = zserio::BasicColumnInfo<AllocatorType>;
using TableInfoType = zserio::BasicTableInfo<AllocatorType>;
using ItemInfoType = zserio::ItemInfo;
using TemplateArgumentInfoType = zserio::BasicTemplateArgumentInfo<AllocatorType>;
using MessageInfoType = zserio::BasicMessageInfo<AllocatorType>;
using MethodInfoType = zserio::BasicMethodInfo<AllocatorType>;

class WithTypeInfoCodeTest : public ::testing::Test
{
protected:
    void checkSqlDatabase(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.SqlDatabase", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::SQL_DATABASE, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::SQL_DATABASE, typeInfo.getCppType());

        const zserio::Span<const TableInfoType> tables = typeInfo.getTables();
        ASSERT_EQ(5, tables.size());

        // sqlTable
        const TableInfoType& sqlTable = tables[0];
        ASSERT_EQ("sqlTable", sqlTable.schemaName);
        checkSqlTable(sqlTable.typeInfo);

        // templatedSqlTableU32
        const TableInfoType& templatedSqlTableU32 = tables[1];
        ASSERT_EQ("templatedSqlTableU32", templatedSqlTableU32.schemaName);
        checkTemplatedSqlTable_uint32(templatedSqlTableU32.typeInfo);

        // templatedSqlTableU8
        const TableInfoType& templatedSqlTableU8 = tables[2];
        ASSERT_EQ("templatedSqlTableU8", templatedSqlTableU8.schemaName);
        checkTemplatedSqlTableU8(templatedSqlTableU8.typeInfo);

        // fts4Table
        const TableInfoType& fts4Table = tables[3];
        ASSERT_EQ("fts4Table", fts4Table.schemaName);
        checkFts4Table(fts4Table.typeInfo);

        // withoutRowIdTable
        const TableInfoType& withoutRowIdTable = tables[4];
        ASSERT_EQ("withoutRowIdTable", withoutRowIdTable.schemaName);
        checkWithoutRowIdTable(withoutRowIdTable.typeInfo);
    }

    void checkSqlTable(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.SqlTable", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::SQL_TABLE, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::SQL_TABLE, typeInfo.getCppType());

        ASSERT_EQ("", typeInfo.getSqlConstraint());
        ASSERT_EQ("", typeInfo.getVirtualTableUsing());
        ASSERT_EQ(false, typeInfo.isWithoutRowId());

        ASSERT_EQ("", typeInfo.getTemplateName());
        ASSERT_EQ(0, typeInfo.getTemplateArguments().size());

        const zserio::Span<const ColumnInfoType> columns = typeInfo.getColumns();
        ASSERT_EQ(2, columns.size());

        // pk
        const ColumnInfoType& pkColumn = columns[0];
        ASSERT_EQ("pk", pkColumn.schemaName);

        ASSERT_EQ("bit:32", pkColumn.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, pkColumn.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, pkColumn.typeInfo.getCppType());
        ASSERT_EQ(32, pkColumn.typeInfo.getBitSize());

        ASSERT_EQ(0, pkColumn.typeArguments.size());
        ASSERT_EQ("INTEGER", pkColumn.sqlTypeName);
        ASSERT_EQ("PRIMARY KEY NOT NULL", pkColumn.sqlConstraint);
        ASSERT_EQ(false, pkColumn.isVirtual);

        // text
        const ColumnInfoType& textColumn = columns[1];
        ASSERT_EQ("text", textColumn.schemaName);

        ASSERT_EQ("string", textColumn.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::STRING, textColumn.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::STRING, textColumn.typeInfo.getCppType());

        ASSERT_EQ(0, textColumn.typeArguments.size());
        ASSERT_EQ("TEXT", textColumn.sqlTypeName);
        ASSERT_EQ("", textColumn.sqlConstraint);
        ASSERT_EQ(false, textColumn.isVirtual);
    }

    void checkTemplatedSqlTable_uint32(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.TemplatedSqlTable_uint32", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::SQL_TABLE, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::SQL_TABLE, typeInfo.getCppType());

        ASSERT_EQ("PRIMARY KEY(pk)", typeInfo.getSqlConstraint());
        ASSERT_EQ("", typeInfo.getVirtualTableUsing());
        ASSERT_EQ(false, typeInfo.isWithoutRowId());

        ASSERT_EQ("with_type_info_code.TemplatedSqlTable", typeInfo.getTemplateName());
        const zserio::Span<const TemplateArgumentInfoType> templateArgs = typeInfo.getTemplateArguments();
        ASSERT_EQ(1, templateArgs.size());
        const ITypeInfoType& templateArg0Info = templateArgs[0].typeInfo;
        ASSERT_EQ("bit:32", templateArg0Info.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, templateArg0Info.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, templateArg0Info.getCppType());
        ASSERT_EQ(32, templateArg0Info.getBitSize());

        const zserio::Span<const ColumnInfoType> columns = typeInfo.getColumns();
        ASSERT_EQ(2, columns.size());

        // pk
        const ColumnInfoType& pkColumn = columns[0];
        ASSERT_EQ("pk", pkColumn.schemaName);

        ASSERT_EQ("bit:32", pkColumn.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, pkColumn.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, pkColumn.typeInfo.getCppType());
        ASSERT_EQ(32, pkColumn.typeInfo.getBitSize());

        ASSERT_EQ(0, pkColumn.typeArguments.size());
        ASSERT_EQ("INTEGER", pkColumn.sqlTypeName);
        ASSERT_EQ("NOT NULL", pkColumn.sqlConstraint);
        ASSERT_EQ(false, pkColumn.isVirtual);

        // withTypeInfoCode
        const ColumnInfoType& withTypeInfoCodeColumn = columns[1];
        ASSERT_EQ("withTypeInfoCode", withTypeInfoCodeColumn.schemaName);
        checkWithTypeInfoCode(withTypeInfoCodeColumn.typeInfo);
        ASSERT_EQ(0, withTypeInfoCodeColumn.typeArguments.size());
        ASSERT_EQ("BLOB", withTypeInfoCodeColumn.sqlTypeName);
        ASSERT_EQ("", withTypeInfoCodeColumn.sqlConstraint);
        ASSERT_EQ(false, withTypeInfoCodeColumn.isVirtual);
    }

    void checkWithTypeInfoCode(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.WithTypeInfoCode", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::STRUCT, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::STRUCT, typeInfo.getCppType());

        ASSERT_EQ(0, typeInfo.getParameters().size());
        ASSERT_EQ(0, typeInfo.getFunctions().size());

        ASSERT_EQ("", typeInfo.getTemplateName());
        ASSERT_EQ(0, typeInfo.getTemplateArguments().size());

        const zserio::Span<const FieldInfoType> fields = typeInfo.getFields();
        ASSERT_EQ(15, fields.size());

        // simpleStruct
        const FieldInfoType& simpleStructField = fields[0];
        ASSERT_EQ("simpleStruct", simpleStructField.schemaName);

        checkSimpleStruct(simpleStructField.typeInfo);

        ASSERT_EQ(0, simpleStructField.typeArguments.size());
        ASSERT_EQ(false, simpleStructField.isExtended);
        ASSERT_EQ("", simpleStructField.alignment);
        ASSERT_EQ("", simpleStructField.offset);
        ASSERT_EQ("", simpleStructField.initializer);
        ASSERT_EQ(false, simpleStructField.isOptional);
        ASSERT_EQ("", simpleStructField.optionalCondition);
        ASSERT_EQ("", simpleStructField.constraint);
        ASSERT_EQ(false, simpleStructField.isArray);
        ASSERT_EQ("", simpleStructField.arrayLength);
        ASSERT_EQ(false, simpleStructField.isPacked);
        ASSERT_EQ(false, simpleStructField.isImplicit);

        // complexStruct
        const FieldInfoType& complexStructField = fields[1];
        ASSERT_EQ("complexStruct", complexStructField.schemaName);

        checkComplexStruct(complexStructField.typeInfo);

        ASSERT_EQ(0, complexStructField.typeArguments.size());
        ASSERT_EQ(false, complexStructField.isExtended);
        ASSERT_EQ("", complexStructField.alignment);
        ASSERT_EQ("", complexStructField.offset);
        ASSERT_EQ("", complexStructField.initializer);
        ASSERT_EQ(false, complexStructField.isOptional);
        ASSERT_EQ("", complexStructField.optionalCondition);
        ASSERT_EQ("", complexStructField.constraint);
        ASSERT_EQ(false, complexStructField.isArray);
        ASSERT_EQ("", complexStructField.arrayLength);
        ASSERT_EQ(false, complexStructField.isPacked);
        ASSERT_EQ(false, complexStructField.isImplicit);

        // parameterizedStruct
        const FieldInfoType& parameterizedStructField = fields[2];
        ASSERT_EQ("parameterizedStruct", parameterizedStructField.schemaName);

        checkParameterizedStruct(parameterizedStructField.typeInfo);

        ASSERT_EQ(1, parameterizedStructField.typeArguments.size());
        ASSERT_EQ("simpleStruct()", parameterizedStructField.typeArguments[0]);
        ASSERT_EQ(false, parameterizedStructField.isExtended);
        ASSERT_EQ("", parameterizedStructField.alignment);
        ASSERT_EQ("", parameterizedStructField.offset);
        ASSERT_EQ("", parameterizedStructField.initializer);
        ASSERT_EQ(false, parameterizedStructField.isOptional);
        ASSERT_EQ("", parameterizedStructField.optionalCondition);
        ASSERT_EQ("", parameterizedStructField.constraint);
        ASSERT_EQ(false, parameterizedStructField.isArray);
        ASSERT_EQ("", parameterizedStructField.arrayLength);
        ASSERT_EQ(false, parameterizedStructField.isPacked);
        ASSERT_EQ(false, parameterizedStructField.isImplicit);

        // recursiveStruct
        const FieldInfoType& recursiveStructField = fields[3];
        ASSERT_EQ("recursiveStruct", recursiveStructField.schemaName);

        checkRecursiveStruct(recursiveStructField.typeInfo);

        ASSERT_EQ(0, recursiveStructField.typeArguments.size());
        ASSERT_EQ(false, recursiveStructField.isExtended);
        ASSERT_EQ("", recursiveStructField.alignment);
        ASSERT_EQ("", recursiveStructField.offset);
        ASSERT_EQ("", recursiveStructField.initializer);
        ASSERT_EQ(false, recursiveStructField.isOptional);
        ASSERT_EQ("", recursiveStructField.optionalCondition);
        ASSERT_EQ("", recursiveStructField.constraint);
        ASSERT_EQ(false, recursiveStructField.isArray);
        ASSERT_EQ("", recursiveStructField.arrayLength);
        ASSERT_EQ(false, recursiveStructField.isPacked);
        ASSERT_EQ(false, recursiveStructField.isImplicit);

        // recursiveUnion
        const FieldInfoType& recursiveUnion = fields[4];
        ASSERT_EQ("recursiveUnion", recursiveUnion.schemaName);

        checkRecursiveUnion(recursiveUnion.typeInfo);

        ASSERT_EQ(0, recursiveUnion.typeArguments.size());
        ASSERT_EQ(false, recursiveUnion.isExtended);
        ASSERT_EQ("", recursiveUnion.alignment);
        ASSERT_EQ("", recursiveUnion.offset);
        ASSERT_EQ("", recursiveUnion.initializer);
        ASSERT_EQ(false, recursiveUnion.isOptional);
        ASSERT_EQ("", recursiveUnion.optionalCondition);
        ASSERT_EQ("", recursiveUnion.constraint);
        ASSERT_EQ(false, recursiveUnion.isArray);
        ASSERT_EQ("", recursiveUnion.arrayLength);
        ASSERT_EQ(false, recursiveUnion.isPacked);
        ASSERT_EQ(false, recursiveUnion.isImplicit);

        // recursiveChoice
        const FieldInfoType& recursiveChoice = fields[5];
        ASSERT_EQ("recursiveChoice", recursiveChoice.schemaName);

        checkRecursiveChoice(recursiveChoice.typeInfo);

        ASSERT_EQ(2, recursiveChoice.typeArguments.size());
        ASSERT_EQ("true", recursiveChoice.typeArguments[0]);
        ASSERT_EQ("false", recursiveChoice.typeArguments[1]);
        ASSERT_EQ(false, recursiveChoice.isExtended);
        ASSERT_EQ("", recursiveChoice.alignment);
        ASSERT_EQ("", recursiveChoice.offset);
        ASSERT_EQ("", recursiveChoice.initializer);
        ASSERT_EQ(false, recursiveChoice.isOptional);
        ASSERT_EQ("", recursiveChoice.optionalCondition);
        ASSERT_EQ("", recursiveChoice.constraint);
        ASSERT_EQ(false, recursiveChoice.isArray);
        ASSERT_EQ("", recursiveChoice.arrayLength);
        ASSERT_EQ(false, recursiveChoice.isPacked);
        ASSERT_EQ(false, recursiveChoice.isImplicit);

        // selector
        const FieldInfoType& selectorField = fields[6];
        ASSERT_EQ("selector", selectorField.schemaName);

        checkTestEnum(selectorField.typeInfo);

        ASSERT_EQ(0, selectorField.typeArguments.size());
        ASSERT_EQ(false, selectorField.isExtended);
        ASSERT_EQ("", selectorField.alignment);
        ASSERT_EQ("", selectorField.offset);
        ASSERT_EQ("", selectorField.initializer);
        ASSERT_EQ(false, selectorField.isOptional);
        ASSERT_EQ("", selectorField.optionalCondition);
        ASSERT_EQ("", selectorField.constraint);
        ASSERT_EQ(false, selectorField.isArray);
        ASSERT_EQ("", selectorField.arrayLength);
        ASSERT_EQ(false, selectorField.isPacked);
        ASSERT_EQ(false, selectorField.isImplicit);

        // simpleChoice
        const FieldInfoType& simpleChoiceField = fields[7];
        ASSERT_EQ("simpleChoice", simpleChoiceField.schemaName);

        checkSimpleChoice(simpleChoiceField.typeInfo);

        ASSERT_EQ(1, simpleChoiceField.typeArguments.size());
        ASSERT_EQ("selector()", simpleChoiceField.typeArguments[0]);
        ASSERT_EQ(false, simpleChoiceField.isExtended);
        ASSERT_EQ("", simpleChoiceField.alignment);
        ASSERT_EQ("", simpleChoiceField.offset);
        ASSERT_EQ("", simpleChoiceField.initializer);
        ASSERT_EQ(false, simpleChoiceField.isOptional);
        ASSERT_EQ("", simpleChoiceField.optionalCondition);
        ASSERT_EQ("", simpleChoiceField.constraint);
        ASSERT_EQ(false, simpleChoiceField.isArray);
        ASSERT_EQ("", simpleChoiceField.arrayLength);
        ASSERT_EQ(false, simpleChoiceField.isPacked);
        ASSERT_EQ(false, simpleChoiceField.isImplicit);

        // templatedStruct
        const FieldInfoType& templatedStructField = fields[8];
        ASSERT_EQ("templatedStruct", templatedStructField.schemaName);

        checkTS32(templatedStructField.typeInfo);

        ASSERT_EQ(0, templatedStructField.typeArguments.size());
        ASSERT_EQ(false, templatedStructField.isExtended);
        ASSERT_EQ("", templatedStructField.alignment);
        ASSERT_EQ("", templatedStructField.offset);
        ASSERT_EQ("", templatedStructField.initializer);
        ASSERT_EQ(false, templatedStructField.isOptional);
        ASSERT_EQ("", templatedStructField.optionalCondition);
        ASSERT_EQ("", templatedStructField.constraint);
        ASSERT_EQ(false, templatedStructField.isArray);
        ASSERT_EQ("", templatedStructField.arrayLength);
        ASSERT_EQ(false, templatedStructField.isPacked);
        ASSERT_EQ(false, templatedStructField.isImplicit);

        // templatedParameterizedStruct
        const FieldInfoType& templatedParameterizedStructField = fields[9];
        ASSERT_EQ("templatedParameterizedStruct", templatedParameterizedStructField.schemaName);

        checkTemplatedParameterizedStruct_TS32(templatedParameterizedStructField.typeInfo);

        ASSERT_EQ(1, templatedParameterizedStructField.typeArguments.size());
        ASSERT_EQ("templatedStruct()", templatedParameterizedStructField.typeArguments[0]);
        ASSERT_EQ(false, templatedParameterizedStructField.isExtended);
        ASSERT_EQ("", templatedParameterizedStructField.alignment);
        ASSERT_EQ("", templatedParameterizedStructField.offset);
        ASSERT_EQ("", templatedParameterizedStructField.initializer);
        ASSERT_EQ(false, templatedParameterizedStructField.isOptional);
        ASSERT_EQ("", templatedParameterizedStructField.optionalCondition);
        ASSERT_EQ("", templatedParameterizedStructField.constraint);
        ASSERT_EQ(false, templatedParameterizedStructField.isArray);
        ASSERT_EQ("", templatedParameterizedStructField.arrayLength);
        ASSERT_EQ(false, templatedParameterizedStructField.isPacked);
        ASSERT_EQ(false, templatedParameterizedStructField.isImplicit);

        // externData
        const FieldInfoType& externDataField = fields[10];
        ASSERT_EQ("externData", externDataField.schemaName);

        ASSERT_EQ("extern", externDataField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::EXTERN, externDataField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::BIT_BUFFER, externDataField.typeInfo.getCppType());

        ASSERT_EQ(0, externDataField.typeArguments.size());
        ASSERT_EQ(false, externDataField.isExtended);
        ASSERT_EQ("", externDataField.alignment);
        ASSERT_EQ("", externDataField.offset);
        ASSERT_EQ("", externDataField.initializer);
        ASSERT_EQ(false, externDataField.isOptional);
        ASSERT_EQ("", externDataField.optionalCondition);
        ASSERT_EQ("", externDataField.constraint);
        ASSERT_EQ(false, externDataField.isArray);
        ASSERT_EQ("", externDataField.arrayLength);
        ASSERT_EQ(false, externDataField.isPacked);
        ASSERT_EQ(false, externDataField.isImplicit);

        // externArray
        const FieldInfoType& externArrayField = fields[11];
        ASSERT_EQ("externArray", externArrayField.schemaName);

        ASSERT_EQ("extern", externArrayField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::EXTERN, externArrayField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::BIT_BUFFER, externArrayField.typeInfo.getCppType());

        ASSERT_EQ(0, externArrayField.typeArguments.size());
        ASSERT_EQ(false, externArrayField.isExtended);
        ASSERT_EQ("", externArrayField.alignment);
        ASSERT_EQ("", externArrayField.offset);
        ASSERT_EQ("", externArrayField.initializer);
        ASSERT_EQ(false, externArrayField.isOptional);
        ASSERT_EQ("", externArrayField.optionalCondition);
        ASSERT_EQ("", externArrayField.constraint);
        ASSERT_EQ(true, externArrayField.isArray);
        ASSERT_EQ("", externArrayField.arrayLength);
        ASSERT_EQ(false, externArrayField.isPacked);
        ASSERT_EQ(false, externArrayField.isImplicit);

        // bytesData
        const FieldInfoType& bytesDataField = fields[12];
        ASSERT_EQ("bytesData", bytesDataField.schemaName);

        ASSERT_EQ("bytes", bytesDataField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::BYTES, bytesDataField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::BYTES, bytesDataField.typeInfo.getCppType());

        ASSERT_EQ(0, bytesDataField.typeArguments.size());
        ASSERT_EQ(false, bytesDataField.isExtended);
        ASSERT_EQ("", bytesDataField.alignment);
        ASSERT_EQ("", bytesDataField.offset);
        ASSERT_EQ("", bytesDataField.initializer);
        ASSERT_EQ(false, bytesDataField.isOptional);
        ASSERT_EQ("", bytesDataField.optionalCondition);
        ASSERT_EQ("", bytesDataField.constraint);
        ASSERT_EQ(false, bytesDataField.isArray);
        ASSERT_EQ("", bytesDataField.arrayLength);
        ASSERT_EQ(false, bytesDataField.isPacked);
        ASSERT_EQ(false, bytesDataField.isImplicit);

        // bytesArray
        const FieldInfoType& bytesArrayField = fields[13];
        ASSERT_EQ("bytesArray", bytesArrayField.schemaName);

        ASSERT_EQ("bytes", bytesArrayField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::BYTES, bytesArrayField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::BYTES, bytesArrayField.typeInfo.getCppType());

        ASSERT_EQ(0, bytesArrayField.typeArguments.size());
        ASSERT_EQ(true, bytesArrayField.isExtended);
        ASSERT_EQ("", bytesArrayField.alignment);
        ASSERT_EQ("", bytesArrayField.offset);
        ASSERT_EQ("", bytesArrayField.initializer);
        ASSERT_EQ(false, bytesArrayField.isOptional);
        ASSERT_EQ("", bytesArrayField.optionalCondition);
        ASSERT_EQ("", bytesArrayField.constraint);
        ASSERT_EQ(true, bytesArrayField.isArray);
        ASSERT_EQ("", bytesArrayField.arrayLength);
        ASSERT_EQ(false, bytesArrayField.isPacked);
        ASSERT_EQ(false, bytesArrayField.isImplicit);

        // implicitArray
        const FieldInfoType& implicitArrayField = fields[14];
        ASSERT_EQ("implicitArray", implicitArrayField.schemaName);

        ASSERT_EQ("bit:32", implicitArrayField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, implicitArrayField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, implicitArrayField.typeInfo.getCppType());
        ASSERT_EQ(32, implicitArrayField.typeInfo.getBitSize());

        ASSERT_EQ(0, implicitArrayField.typeArguments.size());
        ASSERT_EQ(true, implicitArrayField.isExtended);
        ASSERT_EQ("", implicitArrayField.alignment);
        ASSERT_EQ("", implicitArrayField.offset);
        ASSERT_EQ("", implicitArrayField.initializer);
        ASSERT_EQ(false, implicitArrayField.isOptional);
        ASSERT_EQ("", implicitArrayField.optionalCondition);
        ASSERT_EQ("", implicitArrayField.constraint);
        ASSERT_EQ(true, implicitArrayField.isArray);
        ASSERT_EQ("", implicitArrayField.arrayLength);
        ASSERT_EQ(false, implicitArrayField.isPacked);
        ASSERT_EQ(true, implicitArrayField.isImplicit);
    }

    void checkSimpleStruct(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.SimpleStruct", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::STRUCT, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::STRUCT, typeInfo.getCppType());

        ASSERT_EQ(0, typeInfo.getParameters().size());
        ASSERT_EQ(0, typeInfo.getFunctions().size());

        ASSERT_EQ("", typeInfo.getTemplateName());
        ASSERT_EQ(0, typeInfo.getTemplateArguments().size());

        const zserio::Span<const FieldInfoType> fields = typeInfo.getFields();
        ASSERT_EQ(7, fields.size());

        // fieldU32
        const FieldInfoType& fieldU32Field = fields[0];
        ASSERT_EQ("fieldU32", fieldU32Field.schemaName);

        ASSERT_EQ("bit:32", fieldU32Field.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, fieldU32Field.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, fieldU32Field.typeInfo.getCppType());
        ASSERT_EQ(32, fieldU32Field.typeInfo.getBitSize());

        ASSERT_EQ(0, fieldU32Field.typeArguments.size());
        ASSERT_EQ(false, fieldU32Field.isExtended);
        ASSERT_EQ("8", fieldU32Field.alignment);
        ASSERT_EQ("", fieldU32Field.offset);
        ASSERT_EQ("10", fieldU32Field.initializer);
        ASSERT_EQ(false, fieldU32Field.isOptional);
        ASSERT_EQ("", fieldU32Field.optionalCondition);
        ASSERT_EQ("", fieldU32Field.constraint);
        ASSERT_EQ(false, fieldU32Field.isArray);
        ASSERT_EQ("", fieldU32Field.arrayLength);
        ASSERT_EQ(false, fieldU32Field.isPacked);
        ASSERT_EQ(false, fieldU32Field.isImplicit);

        // fieldOffset
        const FieldInfoType& fieldOffsetField = fields[1];
        ASSERT_EQ("fieldOffset", fieldOffsetField.schemaName);

        ASSERT_EQ("bit:32", fieldOffsetField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, fieldOffsetField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, fieldOffsetField.typeInfo.getCppType());
        ASSERT_EQ(32, fieldOffsetField.typeInfo.getBitSize());

        ASSERT_EQ(0, fieldOffsetField.typeArguments.size());
        ASSERT_EQ(false, fieldOffsetField.isExtended);
        ASSERT_EQ("", fieldOffsetField.alignment);
        ASSERT_EQ("", fieldOffsetField.offset);
        ASSERT_EQ("", fieldOffsetField.initializer);
        ASSERT_EQ(false, fieldOffsetField.isOptional);
        ASSERT_EQ("", fieldOffsetField.optionalCondition);
        ASSERT_EQ("", fieldOffsetField.constraint);
        ASSERT_EQ(false, fieldOffsetField.isArray);
        ASSERT_EQ("", fieldOffsetField.arrayLength);
        ASSERT_EQ(false, fieldOffsetField.isPacked);
        ASSERT_EQ(false, fieldOffsetField.isImplicit);

        // fieldString
        const FieldInfoType& fieldStringField = fields[2];
        ASSERT_EQ("fieldString", fieldStringField.schemaName);

        ASSERT_EQ("string", fieldStringField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::STRING, fieldStringField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::STRING, fieldStringField.typeInfo.getCppType());

        ASSERT_EQ(0, fieldStringField.typeArguments.size());
        ASSERT_EQ(false, fieldStringField.isExtended);
        ASSERT_EQ("", fieldStringField.alignment);
        ASSERT_EQ("fieldOffset()", fieldStringField.offset);
        ASSERT_EQ("::std::string_view{\"MyString\"}", fieldStringField.initializer);
        ASSERT_EQ(false, fieldStringField.isOptional);
        ASSERT_EQ("", fieldStringField.optionalCondition);
        ASSERT_EQ("", fieldStringField.constraint);
        ASSERT_EQ(false, fieldStringField.isArray);
        ASSERT_EQ("", fieldStringField.arrayLength);
        ASSERT_EQ(false, fieldStringField.isPacked);
        ASSERT_EQ(false, fieldStringField.isImplicit);

        // fieldBool
        const FieldInfoType& fieldBoolField = fields[3];
        ASSERT_EQ("fieldBool", fieldBoolField.schemaName);

        ASSERT_EQ("bool", fieldBoolField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::BOOL, fieldBoolField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::BOOL, fieldBoolField.typeInfo.getCppType());
        ASSERT_EQ(1, fieldBoolField.typeInfo.getBitSize());

        ASSERT_EQ(0, fieldBoolField.typeArguments.size());
        ASSERT_EQ(false, fieldBoolField.isExtended);
        ASSERT_EQ("", fieldBoolField.alignment);
        ASSERT_EQ("", fieldBoolField.offset);
        ASSERT_EQ("false", fieldBoolField.initializer);
        ASSERT_EQ(false, fieldBoolField.isOptional);
        ASSERT_EQ("", fieldBoolField.optionalCondition);
        ASSERT_EQ("", fieldBoolField.constraint);
        ASSERT_EQ(false, fieldBoolField.isArray);
        ASSERT_EQ("", fieldBoolField.arrayLength);
        ASSERT_EQ(false, fieldBoolField.isPacked);
        ASSERT_EQ(false, fieldBoolField.isImplicit);

        // fieldFloat16
        const FieldInfoType& fieldFloat16Field = fields[4];
        ASSERT_EQ("fieldFloat16", fieldFloat16Field.schemaName);

        ASSERT_EQ("float16", fieldFloat16Field.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::FLOAT16, fieldFloat16Field.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::FLOAT, fieldFloat16Field.typeInfo.getCppType());
        ASSERT_EQ(16, fieldFloat16Field.typeInfo.getBitSize());

        ASSERT_EQ(0, fieldFloat16Field.typeArguments.size());
        ASSERT_EQ(false, fieldFloat16Field.isExtended);
        ASSERT_EQ("", fieldFloat16Field.alignment);
        ASSERT_EQ("", fieldFloat16Field.offset);
        ASSERT_EQ("1.0f", fieldFloat16Field.initializer);
        ASSERT_EQ(false, fieldFloat16Field.isOptional);
        ASSERT_EQ("", fieldFloat16Field.optionalCondition);
        ASSERT_EQ("", fieldFloat16Field.constraint);
        ASSERT_EQ(false, fieldFloat16Field.isArray);
        ASSERT_EQ("", fieldFloat16Field.arrayLength);
        ASSERT_EQ(false, fieldFloat16Field.isPacked);
        ASSERT_EQ(false, fieldFloat16Field.isImplicit);

        // fieldFloat32
        const FieldInfoType& fieldFloat32Field = fields[5];
        ASSERT_EQ("fieldFloat32", fieldFloat32Field.schemaName);

        ASSERT_EQ("float32", fieldFloat32Field.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::FLOAT32, fieldFloat32Field.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::FLOAT, fieldFloat32Field.typeInfo.getCppType());
        ASSERT_EQ(32, fieldFloat32Field.typeInfo.getBitSize());

        ASSERT_EQ(0, fieldFloat32Field.typeArguments.size());
        ASSERT_EQ(false, fieldFloat32Field.isExtended);
        ASSERT_EQ("", fieldFloat32Field.alignment);
        ASSERT_EQ("", fieldFloat32Field.offset);
        ASSERT_EQ("", fieldFloat32Field.initializer);
        ASSERT_EQ(false, fieldFloat32Field.isOptional);
        ASSERT_EQ("", fieldFloat32Field.optionalCondition);
        ASSERT_EQ("", fieldFloat32Field.constraint);
        ASSERT_EQ(false, fieldFloat32Field.isArray);
        ASSERT_EQ("", fieldFloat32Field.arrayLength);
        ASSERT_EQ(false, fieldFloat32Field.isPacked);
        ASSERT_EQ(false, fieldFloat32Field.isImplicit);

        // fieldFloat64
        const FieldInfoType& fieldFloat64Field = fields[6];
        ASSERT_EQ("fieldFloat64", fieldFloat64Field.schemaName);

        ASSERT_EQ("float64", fieldFloat64Field.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::FLOAT64, fieldFloat64Field.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::DOUBLE, fieldFloat64Field.typeInfo.getCppType());
        ASSERT_EQ(64, fieldFloat64Field.typeInfo.getBitSize());

        ASSERT_EQ(0, fieldFloat64Field.typeArguments.size());
        ASSERT_EQ(false, fieldFloat64Field.isExtended);
        ASSERT_EQ("", fieldFloat64Field.alignment);
        ASSERT_EQ("", fieldFloat64Field.offset);
        ASSERT_EQ("2.0", fieldFloat64Field.initializer);
        ASSERT_EQ(false, fieldFloat64Field.isOptional);
        ASSERT_EQ("", fieldFloat64Field.optionalCondition);
        ASSERT_EQ("", fieldFloat64Field.constraint);
        ASSERT_EQ(false, fieldFloat64Field.isArray);
        ASSERT_EQ("", fieldFloat64Field.arrayLength);
        ASSERT_EQ(false, fieldFloat64Field.isPacked);
        ASSERT_EQ(false, fieldFloat64Field.isImplicit);
    }

    void checkComplexStruct(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.ComplexStruct", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::STRUCT, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::STRUCT, typeInfo.getCppType());

        ASSERT_EQ(0, typeInfo.getParameters().size());
        const zserio::Span<const FunctionInfoType> functions = typeInfo.getFunctions();
        ASSERT_EQ(1, functions.size());

        const FunctionInfoType& function0 = functions[0];
        ASSERT_EQ("firstArrayElement", function0.schemaName);
        ASSERT_EQ("bit:32", function0.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, function0.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, function0.typeInfo.getCppType());
        ASSERT_EQ(32, function0.typeInfo.getBitSize());
        ASSERT_EQ("(array().size() > 0) ? +(array().at(0)) : +(0)", function0.functionResult);

        ASSERT_EQ("", typeInfo.getTemplateName());
        ASSERT_EQ(0, typeInfo.getTemplateArguments().size());

        const zserio::Span<const FieldInfoType> fields = typeInfo.getFields();
        ASSERT_EQ(14, fields.size());

        // simpleStruct
        const FieldInfoType& simpleStructField = fields[0];
        ASSERT_EQ("simpleStruct", simpleStructField.schemaName);

        checkSimpleStruct(simpleStructField.typeInfo);

        ASSERT_EQ(0, simpleStructField.typeArguments.size());
        ASSERT_EQ(false, simpleStructField.isExtended);
        ASSERT_EQ("", simpleStructField.alignment);
        ASSERT_EQ("", simpleStructField.offset);
        ASSERT_EQ("", simpleStructField.initializer);
        ASSERT_EQ(false, simpleStructField.isOptional);
        ASSERT_EQ("", simpleStructField.optionalCondition);
        ASSERT_EQ("", simpleStructField.constraint);
        ASSERT_EQ(false, simpleStructField.isArray);
        ASSERT_EQ("", simpleStructField.arrayLength);
        ASSERT_EQ(false, simpleStructField.isPacked);
        ASSERT_EQ(false, simpleStructField.isImplicit);

        // anotherSimpleStruct
        const FieldInfoType& anotherSimpleStructField = fields[1];
        ASSERT_EQ("anotherSimpleStruct", anotherSimpleStructField.schemaName);

        checkSimpleStruct(anotherSimpleStructField.typeInfo);

        ASSERT_EQ(0, anotherSimpleStructField.typeArguments.size());
        ASSERT_EQ(false, anotherSimpleStructField.isExtended);
        ASSERT_EQ("", anotherSimpleStructField.alignment);
        ASSERT_EQ("", anotherSimpleStructField.offset);
        ASSERT_EQ("", anotherSimpleStructField.initializer);
        ASSERT_EQ(false, anotherSimpleStructField.isOptional);
        ASSERT_EQ("", anotherSimpleStructField.optionalCondition);
        ASSERT_EQ("", anotherSimpleStructField.constraint);
        ASSERT_EQ(false, anotherSimpleStructField.isArray);
        ASSERT_EQ("", anotherSimpleStructField.arrayLength);
        ASSERT_EQ(false, anotherSimpleStructField.isPacked);
        ASSERT_EQ(false, anotherSimpleStructField.isImplicit);

        // optionalSimpleStruct
        const FieldInfoType& optionalSimpleStructField = fields[2];
        ASSERT_EQ("optionalSimpleStruct", optionalSimpleStructField.schemaName);

        checkSimpleStruct(optionalSimpleStructField.typeInfo);

        ASSERT_EQ(0, optionalSimpleStructField.typeArguments.size());
        ASSERT_EQ(false, optionalSimpleStructField.isExtended);
        ASSERT_EQ("", optionalSimpleStructField.alignment);
        ASSERT_EQ("", optionalSimpleStructField.offset);
        ASSERT_EQ("", optionalSimpleStructField.initializer);
        ASSERT_EQ(true, optionalSimpleStructField.isOptional);
        ASSERT_EQ("", optionalSimpleStructField.optionalCondition);
        ASSERT_EQ("", optionalSimpleStructField.constraint);
        ASSERT_EQ(false, optionalSimpleStructField.isArray);
        ASSERT_EQ("", optionalSimpleStructField.arrayLength);
        ASSERT_EQ(false, optionalSimpleStructField.isPacked);
        ASSERT_EQ(false, optionalSimpleStructField.isImplicit);

        // array
        const FieldInfoType& arrayField = fields[3];
        ASSERT_EQ("array", arrayField.schemaName);

        ASSERT_EQ("bit:32", arrayField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, arrayField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, arrayField.typeInfo.getCppType());
        ASSERT_EQ(32, arrayField.typeInfo.getBitSize());

        ASSERT_EQ(0, arrayField.typeArguments.size());
        ASSERT_EQ(false, arrayField.isExtended);
        ASSERT_EQ("", arrayField.alignment);
        ASSERT_EQ("", arrayField.offset);
        ASSERT_EQ("", arrayField.initializer);
        ASSERT_EQ(false, arrayField.isOptional);
        ASSERT_EQ("", arrayField.optionalCondition);
        ASSERT_EQ("array().size() > 0", arrayField.constraint);
        ASSERT_EQ(true, arrayField.isArray);
        ASSERT_EQ("", arrayField.arrayLength);
        ASSERT_EQ(false, arrayField.isPacked);
        ASSERT_EQ(false, arrayField.isImplicit);

        // arrayWithLen
        const FieldInfoType& arrayWithLenField = fields[4];
        ASSERT_EQ("arrayWithLen", arrayWithLenField.schemaName);

        ASSERT_EQ("int:5", arrayWithLenField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::INT5, arrayWithLenField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::INT8, arrayWithLenField.typeInfo.getCppType());
        ASSERT_EQ(5, arrayWithLenField.typeInfo.getBitSize());

        ASSERT_EQ(0, arrayWithLenField.typeArguments.size());
        ASSERT_EQ(false, arrayWithLenField.isExtended);
        ASSERT_EQ("", arrayWithLenField.alignment);
        ASSERT_EQ("", arrayWithLenField.offset);
        ASSERT_EQ("", arrayWithLenField.initializer);
        ASSERT_EQ(true, arrayWithLenField.isOptional);
        ASSERT_EQ("array().at(0) > 0", arrayWithLenField.optionalCondition);
        ASSERT_EQ("", arrayWithLenField.constraint);
        ASSERT_EQ(true, arrayWithLenField.isArray);
        ASSERT_EQ("array().at(0)", arrayWithLenField.arrayLength);
        ASSERT_EQ(false, arrayWithLenField.isPacked);
        ASSERT_EQ(false, arrayWithLenField.isImplicit);

        // paramStructArray
        const FieldInfoType& paramStructArrayField = fields[5];
        ASSERT_EQ("paramStructArray", paramStructArrayField.schemaName);

        checkParameterizedStruct(paramStructArrayField.typeInfo);

        ASSERT_EQ(1, paramStructArrayField.typeArguments.size());
        ASSERT_EQ("((index % 2) == 0) ? simpleStruct() : anotherSimpleStruct()",
                paramStructArrayField.typeArguments[0]);
        ASSERT_EQ(false, paramStructArrayField.isExtended);
        ASSERT_EQ("", paramStructArrayField.alignment);
        ASSERT_EQ("", paramStructArrayField.offset);
        ASSERT_EQ("", paramStructArrayField.initializer);
        ASSERT_EQ(true, paramStructArrayField.isOptional);
        ASSERT_EQ("", paramStructArrayField.optionalCondition);
        ASSERT_EQ("", paramStructArrayField.constraint);
        ASSERT_EQ(true, paramStructArrayField.isArray);
        ASSERT_EQ("", paramStructArrayField.arrayLength);
        ASSERT_EQ(false, paramStructArrayField.isPacked);
        ASSERT_EQ(false, paramStructArrayField.isImplicit);

        // dynamicBitField
        const FieldInfoType& dynamicBitFieldField = fields[6];
        ASSERT_EQ("dynamicBitField", dynamicBitFieldField.schemaName);

        ASSERT_EQ("bit<>", dynamicBitFieldField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::DYNAMIC_UNSIGNED_BITFIELD, dynamicBitFieldField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT64, dynamicBitFieldField.typeInfo.getCppType());

        ASSERT_EQ(1, dynamicBitFieldField.typeArguments.size());
        ASSERT_EQ("simpleStruct().fieldU32()", dynamicBitFieldField.typeArguments[0]);
        ASSERT_EQ(false, dynamicBitFieldField.isExtended);
        ASSERT_EQ("", dynamicBitFieldField.alignment);
        ASSERT_EQ("", dynamicBitFieldField.offset);
        ASSERT_EQ("", dynamicBitFieldField.initializer);
        ASSERT_EQ(false, dynamicBitFieldField.isOptional);
        ASSERT_EQ("", dynamicBitFieldField.optionalCondition);
        ASSERT_EQ("", dynamicBitFieldField.constraint);
        ASSERT_EQ(false, dynamicBitFieldField.isArray);
        ASSERT_EQ("", dynamicBitFieldField.arrayLength);
        ASSERT_EQ(false, dynamicBitFieldField.isPacked);
        ASSERT_EQ(false, dynamicBitFieldField.isImplicit);

        // dynamicBitFieldArray
        const FieldInfoType& dynamicBitFieldArrayField = fields[7];
        ASSERT_EQ("dynamicBitFieldArray", dynamicBitFieldArrayField.schemaName);

        ASSERT_EQ("bit<>", dynamicBitFieldArrayField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::DYNAMIC_UNSIGNED_BITFIELD,
                dynamicBitFieldArrayField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT64, dynamicBitFieldArrayField.typeInfo.getCppType());

        ASSERT_EQ(1, dynamicBitFieldArrayField.typeArguments.size());
        ASSERT_EQ("dynamicBitField().value() * 2", dynamicBitFieldArrayField.typeArguments[0]);
        ASSERT_EQ(false, dynamicBitFieldArrayField.isExtended);
        ASSERT_EQ("", dynamicBitFieldArrayField.alignment);
        ASSERT_EQ("", dynamicBitFieldArrayField.offset);
        ASSERT_EQ("", dynamicBitFieldArrayField.initializer);
        ASSERT_EQ(false, dynamicBitFieldArrayField.isOptional);
        ASSERT_EQ("", dynamicBitFieldArrayField.optionalCondition);
        ASSERT_EQ("", dynamicBitFieldArrayField.constraint);
        ASSERT_EQ(true, dynamicBitFieldArrayField.isArray);
        ASSERT_EQ("", dynamicBitFieldArrayField.arrayLength);
        ASSERT_EQ(true, dynamicBitFieldArrayField.isPacked);
        ASSERT_EQ(false, dynamicBitFieldArrayField.isImplicit);

        // optionalEnum
        const FieldInfoType& optionalEnumField = fields[8];
        ASSERT_EQ("optionalEnum", optionalEnumField.schemaName);

        checkTestEnum(optionalEnumField.typeInfo);

        ASSERT_EQ(0, optionalEnumField.typeArguments.size());
        ASSERT_EQ(false, optionalEnumField.isExtended);
        ASSERT_EQ("", optionalEnumField.alignment);
        ASSERT_EQ("", optionalEnumField.offset);
        ASSERT_EQ("", optionalEnumField.initializer);
        ASSERT_EQ(true, optionalEnumField.isOptional);
        ASSERT_EQ("", optionalEnumField.optionalCondition);
        ASSERT_EQ("", optionalEnumField.constraint);
        ASSERT_EQ(false, optionalEnumField.isArray);
        ASSERT_EQ("", optionalEnumField.arrayLength);
        ASSERT_EQ(false, optionalEnumField.isPacked);
        ASSERT_EQ(false, optionalEnumField.isImplicit);

        // optionalBitmask
        const FieldInfoType& optionalBitmaskField = fields[9];
        ASSERT_EQ("optionalBitmask", optionalBitmaskField.schemaName);

        checkTestBitmask(optionalBitmaskField.typeInfo);

        ASSERT_EQ(0, optionalBitmaskField.typeArguments.size());
        ASSERT_EQ(false, optionalBitmaskField.isExtended);
        ASSERT_EQ("", optionalBitmaskField.alignment);
        ASSERT_EQ("", optionalBitmaskField.offset);
        ASSERT_EQ("", optionalBitmaskField.initializer);
        ASSERT_EQ(true, optionalBitmaskField.isOptional);
        ASSERT_EQ("", optionalBitmaskField.optionalCondition);
        ASSERT_EQ("", optionalBitmaskField.constraint);
        ASSERT_EQ(false, optionalBitmaskField.isArray);
        ASSERT_EQ("", optionalBitmaskField.arrayLength);
        ASSERT_EQ(false, optionalBitmaskField.isPacked);
        ASSERT_EQ(false, optionalBitmaskField.isImplicit);

        // optionalExtern
        const FieldInfoType& optionalExternField = fields[10];
        ASSERT_EQ("optionalExtern", optionalExternField.schemaName);

        ASSERT_EQ("extern", optionalExternField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::EXTERN, optionalExternField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::BIT_BUFFER, optionalExternField.typeInfo.getCppType());

        ASSERT_EQ(0, optionalExternField.typeArguments.size());
        ASSERT_EQ(false, optionalExternField.isExtended);
        ASSERT_EQ("", optionalExternField.alignment);
        ASSERT_EQ("", optionalExternField.offset);
        ASSERT_EQ("", optionalExternField.initializer);
        ASSERT_EQ(true, optionalExternField.isOptional);
        ASSERT_EQ("", optionalExternField.optionalCondition);
        ASSERT_EQ("", optionalExternField.constraint);
        ASSERT_EQ(false, optionalExternField.isArray);
        ASSERT_EQ("", optionalExternField.arrayLength);
        ASSERT_EQ(false, optionalExternField.isPacked);
        ASSERT_EQ(false, optionalExternField.isImplicit);

        // optionalBytes
        const FieldInfoType& optionalBytesField = fields[11];
        ASSERT_EQ("optionalBytes", optionalBytesField.schemaName);

        ASSERT_EQ("bytes", optionalBytesField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::BYTES, optionalBytesField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::BYTES, optionalBytesField.typeInfo.getCppType());

        ASSERT_EQ(0, optionalBytesField.typeArguments.size());
        ASSERT_EQ(false, optionalBytesField.isExtended);
        ASSERT_EQ("", optionalBytesField.alignment);
        ASSERT_EQ("", optionalBytesField.offset);
        ASSERT_EQ("", optionalBytesField.initializer);
        ASSERT_EQ(true, optionalBytesField.isOptional);
        ASSERT_EQ("", optionalBytesField.optionalCondition);
        ASSERT_EQ("", optionalBytesField.constraint);
        ASSERT_EQ(false, optionalBytesField.isArray);
        ASSERT_EQ("", optionalBytesField.arrayLength);
        ASSERT_EQ(false, optionalBytesField.isPacked);
        ASSERT_EQ(false, optionalBytesField.isImplicit);

        // enumArray
        const FieldInfoType& enumArrayField = fields[12];
        ASSERT_EQ("enumArray", enumArrayField.schemaName);

        checkTestEnum(enumArrayField.typeInfo);

        ASSERT_EQ(0, enumArrayField.typeArguments.size());
        ASSERT_EQ(false, enumArrayField.isExtended);
        ASSERT_EQ("", enumArrayField.alignment);
        ASSERT_EQ("", enumArrayField.offset);
        ASSERT_EQ("", enumArrayField.initializer);
        ASSERT_EQ(false, enumArrayField.isOptional);
        ASSERT_EQ("", enumArrayField.optionalCondition);
        ASSERT_EQ("", enumArrayField.constraint);
        ASSERT_EQ(true, enumArrayField.isArray);
        ASSERT_EQ("2", enumArrayField.arrayLength);
        ASSERT_EQ(false, enumArrayField.isPacked);
        ASSERT_EQ(false, enumArrayField.isImplicit);

        // bitmaskArray
        const FieldInfoType& bitmaskArrayField = fields[13];
        ASSERT_EQ("bitmaskArray", bitmaskArrayField.schemaName);

        checkTestBitmask(bitmaskArrayField.typeInfo);

        ASSERT_EQ(0, bitmaskArrayField.typeArguments.size());
        ASSERT_EQ(false, bitmaskArrayField.isExtended);
        ASSERT_EQ("", bitmaskArrayField.alignment);
        ASSERT_EQ("", bitmaskArrayField.offset);
        ASSERT_EQ("", bitmaskArrayField.initializer);
        ASSERT_EQ(false, bitmaskArrayField.isOptional);
        ASSERT_EQ("", bitmaskArrayField.optionalCondition);
        ASSERT_EQ("", bitmaskArrayField.constraint);
        ASSERT_EQ(true, bitmaskArrayField.isArray);
        ASSERT_EQ(
                "::zserio::enumToValue(::with_type_info_code::TestEnum::_TWO)", bitmaskArrayField.arrayLength);
        ASSERT_EQ(false, bitmaskArrayField.isPacked);
        ASSERT_EQ(false, bitmaskArrayField.isImplicit);
    }

    void checkParameterizedStruct(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.ParameterizedStruct", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::STRUCT, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::STRUCT, typeInfo.getCppType());

        const zserio::Span<const ParameterInfoType> parameters = typeInfo.getParameters();
        ASSERT_EQ(1, parameters.size());

        const ParameterInfoType& parameter0 = parameters[0];
        ASSERT_EQ("simple", parameter0.schemaName);
        checkSimpleStruct(parameter0.typeInfo);

        ASSERT_EQ(0, typeInfo.getFunctions().size());

        ASSERT_EQ("", typeInfo.getTemplateName());
        ASSERT_EQ(0, typeInfo.getTemplateArguments().size());

        const zserio::Span<const FieldInfoType> fields = typeInfo.getFields();
        ASSERT_EQ(1, fields.size());

        // array
        const FieldInfoType& arrayField = fields[0];
        ASSERT_EQ("array", arrayField.schemaName);

        ASSERT_EQ("bit:8", arrayField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT8, arrayField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT8, arrayField.typeInfo.getCppType());
        ASSERT_EQ(8, arrayField.typeInfo.getBitSize());

        ASSERT_EQ(0, arrayField.typeArguments.size());
        ASSERT_EQ(false, arrayField.isExtended);
        ASSERT_EQ("", arrayField.alignment);
        ASSERT_EQ("", arrayField.offset);
        ASSERT_EQ("", arrayField.initializer);
        ASSERT_EQ(false, arrayField.isOptional);
        ASSERT_EQ("", arrayField.optionalCondition);
        ASSERT_EQ("", arrayField.constraint);
        ASSERT_EQ(true, arrayField.isArray);
        ASSERT_EQ("simple().fieldU32()", arrayField.arrayLength);
        ASSERT_EQ(false, arrayField.isPacked);
        ASSERT_EQ(false, arrayField.isImplicit);
    }

    void checkRecursiveStruct(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.RecursiveStruct", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::STRUCT, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::STRUCT, typeInfo.getCppType());

        ASSERT_EQ(0, typeInfo.getParameters().size());
        ASSERT_EQ(0, typeInfo.getFunctions().size());

        ASSERT_EQ("", typeInfo.getTemplateName());
        ASSERT_EQ(0, typeInfo.getTemplateArguments().size());

        const zserio::Span<const FieldInfoType> fields = typeInfo.getFields();
        ASSERT_EQ(3, fields.size());

        // fieldU32
        const FieldInfoType& fieldU32Field = fields[0];
        ASSERT_EQ("fieldU32", fieldU32Field.schemaName);

        ASSERT_EQ("bit:32", fieldU32Field.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, fieldU32Field.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, fieldU32Field.typeInfo.getCppType());
        ASSERT_EQ(32, fieldU32Field.typeInfo.getBitSize());

        ASSERT_EQ(0, fieldU32Field.typeArguments.size());
        ASSERT_EQ(false, fieldU32Field.isExtended);
        ASSERT_EQ("", fieldU32Field.alignment);
        ASSERT_EQ("", fieldU32Field.offset);
        ASSERT_EQ("", fieldU32Field.initializer);
        ASSERT_EQ(false, fieldU32Field.isOptional);
        ASSERT_EQ("", fieldU32Field.optionalCondition);
        ASSERT_EQ("", fieldU32Field.constraint);
        ASSERT_EQ(false, fieldU32Field.isArray);
        ASSERT_EQ("", fieldU32Field.arrayLength);
        ASSERT_EQ(false, fieldU32Field.isPacked);
        ASSERT_EQ(false, fieldU32Field.isImplicit);

        // fieldRecursion
        const FieldInfoType& fieldRecursion = fields[1];
        ASSERT_EQ("fieldRecursion", fieldRecursion.schemaName);

        ASSERT_EQ(typeInfo.getSchemaName(), fieldRecursion.typeInfo.getSchemaName());
        ASSERT_EQ(typeInfo.getSchemaType(), fieldRecursion.typeInfo.getSchemaType());
        ASSERT_EQ(typeInfo.getCppType(), fieldRecursion.typeInfo.getCppType());
        ASSERT_EQ(typeInfo.getFields().data(), fieldRecursion.typeInfo.getFields().data());

        ASSERT_EQ(0, fieldRecursion.typeArguments.size());
        ASSERT_EQ(false, fieldRecursion.isExtended);
        ASSERT_EQ("", fieldRecursion.alignment);
        ASSERT_EQ("", fieldRecursion.offset);
        ASSERT_EQ("", fieldRecursion.initializer);
        ASSERT_EQ(true, fieldRecursion.isOptional);
        ASSERT_EQ("", fieldRecursion.optionalCondition);
        ASSERT_EQ("", fieldRecursion.constraint);
        ASSERT_EQ(false, fieldRecursion.isArray);
        ASSERT_EQ("", fieldRecursion.arrayLength);
        ASSERT_EQ(false, fieldRecursion.isPacked);
        ASSERT_EQ(false, fieldRecursion.isImplicit);

        // arrayRecursion
        const FieldInfoType& arrayRecursion = fields[2];
        ASSERT_EQ("arrayRecursion", arrayRecursion.schemaName);

        ASSERT_EQ(typeInfo.getSchemaName(), arrayRecursion.typeInfo.getSchemaName());
        ASSERT_EQ(typeInfo.getSchemaType(), arrayRecursion.typeInfo.getSchemaType());
        ASSERT_EQ(typeInfo.getCppType(), arrayRecursion.typeInfo.getCppType());
        ASSERT_EQ(typeInfo.getFields().data(), arrayRecursion.typeInfo.getFields().data());

        ASSERT_EQ(0, arrayRecursion.typeArguments.size());
        ASSERT_EQ(false, arrayRecursion.isExtended);
        ASSERT_EQ("", arrayRecursion.alignment);
        ASSERT_EQ("", arrayRecursion.offset);
        ASSERT_EQ("", arrayRecursion.initializer);
        ASSERT_EQ(false, arrayRecursion.isOptional);
        ASSERT_EQ("", arrayRecursion.optionalCondition);
        ASSERT_EQ("", arrayRecursion.constraint);
        ASSERT_EQ(true, arrayRecursion.isArray);
        ASSERT_EQ("", arrayRecursion.arrayLength);
        ASSERT_EQ(false, arrayRecursion.isPacked);
        ASSERT_EQ(false, arrayRecursion.isImplicit);
    }

    void checkRecursiveUnion(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.RecursiveUnion", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UNION, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UNION, typeInfo.getCppType());

        const zserio::Span<const FieldInfoType> fields = typeInfo.getFields();
        ASSERT_EQ(2, fields.size());

        // fieldU32
        const FieldInfoType& fieldU32Field = fields[0];
        ASSERT_EQ("fieldU32", fieldU32Field.schemaName);

        ASSERT_EQ("bit:32", fieldU32Field.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, fieldU32Field.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, fieldU32Field.typeInfo.getCppType());
        ASSERT_EQ(32, fieldU32Field.typeInfo.getBitSize());

        ASSERT_EQ(0, fieldU32Field.typeArguments.size());
        ASSERT_EQ(false, fieldU32Field.isExtended);
        ASSERT_EQ("", fieldU32Field.alignment);
        ASSERT_EQ("", fieldU32Field.offset);
        ASSERT_EQ("", fieldU32Field.initializer);
        ASSERT_EQ(false, fieldU32Field.isOptional);
        ASSERT_EQ("", fieldU32Field.optionalCondition);
        ASSERT_EQ("", fieldU32Field.constraint);
        ASSERT_EQ(false, fieldU32Field.isArray);
        ASSERT_EQ("", fieldU32Field.arrayLength);
        ASSERT_EQ(false, fieldU32Field.isPacked);
        ASSERT_EQ(false, fieldU32Field.isImplicit);

        // recursive
        const FieldInfoType& recursive = fields[1];
        ASSERT_EQ("recursive", recursive.schemaName);

        ASSERT_EQ(typeInfo.getSchemaName(), recursive.typeInfo.getSchemaName());
        ASSERT_EQ(typeInfo.getSchemaType(), recursive.typeInfo.getSchemaType());
        ASSERT_EQ(typeInfo.getCppType(), recursive.typeInfo.getCppType());
        ASSERT_EQ(typeInfo.getFields().data(), recursive.typeInfo.getFields().data());

        ASSERT_EQ(0, recursive.typeArguments.size());
        ASSERT_EQ(false, recursive.isExtended);
        ASSERT_EQ("", recursive.alignment);
        ASSERT_EQ("", recursive.offset);
        ASSERT_EQ("", recursive.initializer);
        ASSERT_EQ(false, recursive.isOptional);
        ASSERT_EQ("", recursive.optionalCondition);
        ASSERT_EQ("", recursive.constraint);
        ASSERT_EQ(true, recursive.isArray);
        ASSERT_EQ("", recursive.arrayLength);
        ASSERT_EQ(false, recursive.isPacked);
        ASSERT_EQ(false, recursive.isImplicit);
    }

    void checkRecursiveChoice(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.RecursiveChoice", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::CHOICE, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::CHOICE, typeInfo.getCppType());

        const zserio::Span<const ParameterInfoType> parameters = typeInfo.getParameters();
        ASSERT_EQ(2, parameters.size());

        // param1
        const ParameterInfoType& param1 = parameters[0];
        ASSERT_EQ("param1", param1.schemaName);
        ASSERT_EQ("bool", param1.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::BOOL, param1.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::BOOL, param1.typeInfo.getCppType());
        ASSERT_EQ(1, param1.typeInfo.getBitSize());

        // param2
        const ParameterInfoType& param2 = parameters[1];
        ASSERT_EQ("param2", param2.schemaName);
        ASSERT_EQ("bool", param2.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::BOOL, param2.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::BOOL, param2.typeInfo.getCppType());
        ASSERT_EQ(1, param2.typeInfo.getBitSize());

        const zserio::Span<const FieldInfoType> fields = typeInfo.getFields();
        ASSERT_EQ(2, fields.size());

        // recursive
        const FieldInfoType& recursive = fields[0];
        ASSERT_EQ("recursive", recursive.schemaName);

        ASSERT_EQ(typeInfo.getSchemaName(), recursive.typeInfo.getSchemaName());
        ASSERT_EQ(typeInfo.getSchemaType(), recursive.typeInfo.getSchemaType());
        ASSERT_EQ(typeInfo.getCppType(), recursive.typeInfo.getCppType());
        ASSERT_EQ(typeInfo.getFields().data(), recursive.typeInfo.getFields().data());

        ASSERT_EQ(2, recursive.typeArguments.size());
        ASSERT_EQ("param2()", recursive.typeArguments[0]);
        ASSERT_EQ("false", recursive.typeArguments[1]);
        ASSERT_EQ(false, recursive.isExtended);
        ASSERT_EQ("", recursive.alignment);
        ASSERT_EQ("", recursive.offset);
        ASSERT_EQ("", recursive.initializer);
        ASSERT_EQ(false, recursive.isOptional);
        ASSERT_EQ("", recursive.optionalCondition);
        ASSERT_EQ("", recursive.constraint);
        ASSERT_EQ(true, recursive.isArray);
        ASSERT_EQ("", recursive.arrayLength);
        ASSERT_EQ(false, recursive.isPacked);
        ASSERT_EQ(false, recursive.isImplicit);

        // fieldU32
        const FieldInfoType& fieldU32Field = fields[1];
        ASSERT_EQ("fieldU32", fieldU32Field.schemaName);

        ASSERT_EQ("bit:32", fieldU32Field.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, fieldU32Field.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, fieldU32Field.typeInfo.getCppType());
        ASSERT_EQ(32, fieldU32Field.typeInfo.getBitSize());

        ASSERT_EQ(0, fieldU32Field.typeArguments.size());
        ASSERT_EQ(false, fieldU32Field.isExtended);
        ASSERT_EQ("", fieldU32Field.alignment);
        ASSERT_EQ("", fieldU32Field.offset);
        ASSERT_EQ("", fieldU32Field.initializer);
        ASSERT_EQ(false, fieldU32Field.isOptional);
        ASSERT_EQ("", fieldU32Field.optionalCondition);
        ASSERT_EQ("", fieldU32Field.constraint);
        ASSERT_EQ(false, fieldU32Field.isArray);
        ASSERT_EQ("", fieldU32Field.arrayLength);
        ASSERT_EQ(false, fieldU32Field.isPacked);
        ASSERT_EQ(false, fieldU32Field.isImplicit);
    }

    void checkTestEnum(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.TestEnum", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::ENUM, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::ENUM, typeInfo.getCppType());

        ASSERT_EQ("bit:16", typeInfo.getUnderlyingType().getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT16, typeInfo.getUnderlyingType().getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT16, typeInfo.getUnderlyingType().getCppType());
        ASSERT_EQ(16, typeInfo.getUnderlyingType().getBitSize());

        const zserio::Span<const ItemInfoType> items = typeInfo.getEnumItems();
        ASSERT_EQ(3, items.size());

        // One
        const ItemInfoType& OneItem = items[0];
        ASSERT_EQ("One", OneItem.schemaName);
        ASSERT_EQ(0, OneItem.value);
        ASSERT_FALSE(OneItem.isDeprecated);
        ASSERT_FALSE(OneItem.isRemoved);

        // TWO
        const ItemInfoType& TwoItem = items[1];
        ASSERT_EQ("_TWO", TwoItem.schemaName);
        ASSERT_EQ(5, TwoItem.value);
        ASSERT_FALSE(TwoItem.isDeprecated);
        ASSERT_FALSE(TwoItem.isRemoved);

        // ItemThree
        const ItemInfoType& ItemThreeItem = items[2];
        ASSERT_EQ("ItemThree", ItemThreeItem.schemaName);
        ASSERT_EQ(6, ItemThreeItem.value);
        ASSERT_FALSE(ItemThreeItem.isDeprecated);
        ASSERT_FALSE(ItemThreeItem.isRemoved);
    }

    void checkSimpleChoice(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.SimpleChoice", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::CHOICE, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::CHOICE, typeInfo.getCppType());

        const zserio::Span<const ParameterInfoType> parameters = typeInfo.getParameters();
        ASSERT_EQ(1, parameters.size());

        const ParameterInfoType& parameter0 = parameters[0];
        ASSERT_EQ("selector", parameter0.schemaName);
        checkTestEnum(parameter0.typeInfo);

        const zserio::Span<const FunctionInfoType> functions = typeInfo.getFunctions();
        ASSERT_EQ(1, functions.size());

        const FunctionInfoType& function0 = functions[0];
        ASSERT_EQ("fieldTwoFuncCall", function0.schemaName);
        ASSERT_EQ("bit:32", function0.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, function0.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, function0.typeInfo.getCppType());
        ASSERT_EQ(32, function0.typeInfo.getBitSize());
        ASSERT_EQ("fieldTwo().simpleStructFieldU32()", function0.functionResult);

        ASSERT_EQ("selector()", typeInfo.getSelector());

        ASSERT_EQ("", typeInfo.getTemplateName());
        ASSERT_EQ(0, typeInfo.getTemplateArguments().size());

        const zserio::Span<const FieldInfoType> fields = typeInfo.getFields();
        ASSERT_EQ(2, fields.size());

        // fieldTwo
        const FieldInfoType& fieldTwoField = fields[0];
        ASSERT_EQ("fieldTwo", fieldTwoField.schemaName);

        checkSimpleUnion(fieldTwoField.typeInfo);

        ASSERT_EQ(0, fieldTwoField.typeArguments.size());
        ASSERT_EQ(false, fieldTwoField.isExtended);
        ASSERT_EQ("", fieldTwoField.alignment);
        ASSERT_EQ("", fieldTwoField.offset);
        ASSERT_EQ("", fieldTwoField.initializer);
        ASSERT_EQ(false, fieldTwoField.isOptional);
        ASSERT_EQ("", fieldTwoField.optionalCondition);
        ASSERT_EQ("", fieldTwoField.constraint);
        ASSERT_EQ(false, fieldTwoField.isArray);
        ASSERT_EQ("", fieldTwoField.arrayLength);
        ASSERT_EQ(false, fieldTwoField.isPacked);
        ASSERT_EQ(false, fieldTwoField.isImplicit);

        // fieldDefault
        const FieldInfoType& fieldDefaultField = fields[1];
        ASSERT_EQ("fieldDefault", fieldDefaultField.schemaName);

        ASSERT_EQ("string", fieldDefaultField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::STRING, fieldDefaultField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::STRING, fieldDefaultField.typeInfo.getCppType());

        ASSERT_EQ(0, fieldDefaultField.typeArguments.size());
        ASSERT_EQ(false, fieldDefaultField.isExtended);
        ASSERT_EQ("", fieldDefaultField.alignment);
        ASSERT_EQ("", fieldDefaultField.offset);
        ASSERT_EQ("", fieldDefaultField.initializer);
        ASSERT_EQ(false, fieldDefaultField.isOptional);
        ASSERT_EQ("", fieldDefaultField.optionalCondition);
        ASSERT_EQ("", fieldDefaultField.constraint);
        ASSERT_EQ(false, fieldDefaultField.isArray);
        ASSERT_EQ("", fieldDefaultField.arrayLength);
        ASSERT_EQ(false, fieldDefaultField.isPacked);
        ASSERT_EQ(false, fieldDefaultField.isImplicit);

        const zserio::Span<const CaseInfoType> cases = typeInfo.getCases();
        ASSERT_EQ(3, cases.size());

        // case One
        const CaseInfoType& case0 = cases[0];
        ASSERT_EQ(1, case0.caseExpressions.size());
        ASSERT_EQ("::with_type_info_code::TestEnum::One", case0.caseExpressions[0]);
        ASSERT_EQ(nullptr, case0.field);

        // case TWO
        const CaseInfoType& case1 = cases[1];
        ASSERT_EQ(1, case1.caseExpressions.size());
        ASSERT_EQ("::with_type_info_code::TestEnum::_TWO", case1.caseExpressions[0]);
        ASSERT_EQ(&fieldTwoField, case1.field);

        // default
        const CaseInfoType& case2 = cases[2];
        ASSERT_EQ(0, case2.caseExpressions.size());
        ASSERT_EQ(&fieldDefaultField, case2.field);
    }

    void checkSimpleUnion(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.SimpleUnion", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UNION, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UNION, typeInfo.getCppType());

        ASSERT_EQ(0, typeInfo.getParameters().size());

        const zserio::Span<const FunctionInfoType> functions = typeInfo.getFunctions();
        ASSERT_EQ(1, functions.size());

        const FunctionInfoType& function0 = functions[0];
        ASSERT_EQ("simpleStructFieldU32", function0.schemaName);
        ASSERT_EQ("bit:32", function0.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, function0.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, function0.typeInfo.getCppType());
        ASSERT_EQ(32, function0.typeInfo.getBitSize());
        ASSERT_EQ("simpleStruct().fieldU32()", function0.functionResult);

        ASSERT_EQ("", typeInfo.getTemplateName());
        ASSERT_EQ(0, typeInfo.getTemplateArguments().size());

        const zserio::Span<const FieldInfoType> fields = typeInfo.getFields();
        ASSERT_EQ(2, fields.size());

        // testBitmask
        const FieldInfoType& testBitmaskField = fields[0];
        ASSERT_EQ("testBitmask", testBitmaskField.schemaName);

        checkTestBitmask(testBitmaskField.typeInfo);

        ASSERT_EQ(0, testBitmaskField.typeArguments.size());
        ASSERT_EQ(false, testBitmaskField.isExtended);
        ASSERT_EQ("", testBitmaskField.alignment);
        ASSERT_EQ("", testBitmaskField.offset);
        ASSERT_EQ("", testBitmaskField.initializer);
        ASSERT_EQ(false, testBitmaskField.isOptional);
        ASSERT_EQ("", testBitmaskField.optionalCondition);
        ASSERT_EQ("", testBitmaskField.constraint);
        ASSERT_EQ(false, testBitmaskField.isArray);
        ASSERT_EQ("", testBitmaskField.arrayLength);
        ASSERT_EQ(false, testBitmaskField.isPacked);
        ASSERT_EQ(false, testBitmaskField.isImplicit);

        // simpleStruct
        const FieldInfoType& simpleStructField = fields[1];
        ASSERT_EQ("simpleStruct", simpleStructField.schemaName);

        checkSimpleStruct(simpleStructField.typeInfo);

        ASSERT_EQ(0, simpleStructField.typeArguments.size());
        ASSERT_EQ(false, simpleStructField.isExtended);
        ASSERT_EQ("", simpleStructField.alignment);
        ASSERT_EQ("", simpleStructField.offset);
        ASSERT_EQ("", simpleStructField.initializer);
        ASSERT_EQ(false, simpleStructField.isOptional);
        ASSERT_EQ("", simpleStructField.optionalCondition);
        ASSERT_EQ("", simpleStructField.constraint);
        ASSERT_EQ(false, simpleStructField.isArray);
        ASSERT_EQ("", simpleStructField.arrayLength);
        ASSERT_EQ(false, simpleStructField.isPacked);
        ASSERT_EQ(false, simpleStructField.isImplicit);
    }

    void checkTestBitmask(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.TestBitmask", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::BITMASK, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::BITMASK, typeInfo.getCppType());

        ASSERT_EQ("bit:10", typeInfo.getUnderlyingType().getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT10, typeInfo.getUnderlyingType().getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT16, typeInfo.getUnderlyingType().getCppType());
        ASSERT_EQ(10, typeInfo.getUnderlyingType().getBitSize());

        const zserio::Span<const ItemInfoType> values = typeInfo.getBitmaskValues();
        ASSERT_EQ(3, values.size());

        // RED
        const ItemInfoType& redValue = values[0];
        ASSERT_EQ("RED", redValue.schemaName);
        ASSERT_EQ(1, redValue.value);

        // Green
        const ItemInfoType& greenValue = values[1];
        ASSERT_EQ("_Green", greenValue.schemaName);
        ASSERT_EQ(2, greenValue.value);

        // ColorBlue
        const ItemInfoType& colorBlueValue = values[2];
        ASSERT_EQ("ColorBlue", colorBlueValue.schemaName);
        ASSERT_EQ(4, colorBlueValue.value);
    }

    void checkTS32(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.TS32", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::STRUCT, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::STRUCT, typeInfo.getCppType());

        ASSERT_EQ(0, typeInfo.getParameters().size());
        ASSERT_EQ(0, typeInfo.getFunctions().size());

        ASSERT_EQ("with_type_info_code.TemplatedStruct", typeInfo.getTemplateName());

        ASSERT_EQ(1, typeInfo.getTemplateArguments().size());

        const TemplateArgumentInfoType& templateArgument0 = typeInfo.getTemplateArguments()[0];
        ASSERT_EQ("bit:32", templateArgument0.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, templateArgument0.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, templateArgument0.typeInfo.getCppType());
        ASSERT_EQ(32, templateArgument0.typeInfo.getBitSize());

        const zserio::Span<const FieldInfoType> fields = typeInfo.getFields();
        ASSERT_EQ(1, fields.size());

        // field
        const FieldInfoType& fieldField = fields[0];
        ASSERT_EQ("field", fieldField.schemaName);

        ASSERT_EQ("bit:32", fieldField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, fieldField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, fieldField.typeInfo.getCppType());
        ASSERT_EQ(32, fieldField.typeInfo.getBitSize());

        ASSERT_EQ(0, fieldField.typeArguments.size());
        ASSERT_EQ(false, fieldField.isExtended);
        ASSERT_EQ("", fieldField.alignment);
        ASSERT_EQ("", fieldField.offset);
        ASSERT_EQ("", fieldField.initializer);
        ASSERT_EQ(false, fieldField.isOptional);
        ASSERT_EQ("", fieldField.optionalCondition);
        ASSERT_EQ("", fieldField.constraint);
        ASSERT_EQ(false, fieldField.isArray);
        ASSERT_EQ("", fieldField.arrayLength);
        ASSERT_EQ(false, fieldField.isPacked);
        ASSERT_EQ(false, fieldField.isImplicit);
    }

    void checkTemplatedParameterizedStruct_TS32(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.TemplatedParameterizedStruct_TS32", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::STRUCT, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::STRUCT, typeInfo.getCppType());

        ASSERT_EQ(1, typeInfo.getParameters().size());
        const ParameterInfoType& parameter0 = typeInfo.getParameters()[0];
        ASSERT_EQ("param", parameter0.schemaName);
        checkTS32(parameter0.typeInfo);

        ASSERT_EQ(0, typeInfo.getFunctions().size());

        ASSERT_EQ("with_type_info_code.TemplatedParameterizedStruct", typeInfo.getTemplateName());

        ASSERT_EQ(1, typeInfo.getTemplateArguments().size());

        const TemplateArgumentInfoType& templateArgument0 = typeInfo.getTemplateArguments()[0];
        checkTS32(templateArgument0.typeInfo);

        const zserio::Span<const FieldInfoType> fields = typeInfo.getFields();
        ASSERT_EQ(1, fields.size());

        // array
        const FieldInfoType& arrayField = fields[0];
        ASSERT_EQ("array", arrayField.schemaName);

        ASSERT_EQ("bit:32", arrayField.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, arrayField.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, arrayField.typeInfo.getCppType());
        ASSERT_EQ(32, arrayField.typeInfo.getBitSize());

        ASSERT_EQ(0, arrayField.typeArguments.size());
        ASSERT_EQ(false, arrayField.isExtended);
        ASSERT_EQ("", arrayField.alignment);
        ASSERT_EQ("", arrayField.offset);
        ASSERT_EQ("", arrayField.initializer);
        ASSERT_EQ(false, arrayField.isOptional);
        ASSERT_EQ("", arrayField.optionalCondition);
        ASSERT_EQ("", arrayField.constraint);
        ASSERT_EQ(true, arrayField.isArray);
        ASSERT_EQ("param().field()", arrayField.arrayLength);
        ASSERT_EQ(false, arrayField.isPacked);
        ASSERT_EQ(false, arrayField.isImplicit);
    }

    void checkTemplatedSqlTableU8(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.TemplatedSqlTableU8", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::SQL_TABLE, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::SQL_TABLE, typeInfo.getCppType());

        ASSERT_EQ("PRIMARY KEY(pk)", typeInfo.getSqlConstraint());
        ASSERT_EQ("", typeInfo.getVirtualTableUsing());
        ASSERT_EQ(false, typeInfo.isWithoutRowId());

        ASSERT_EQ("with_type_info_code.TemplatedSqlTable", typeInfo.getTemplateName());
        const zserio::Span<const TemplateArgumentInfoType> templateArgs = typeInfo.getTemplateArguments();
        ASSERT_EQ(1, templateArgs.size());
        const ITypeInfoType& templateArg0Info = templateArgs[0].typeInfo;
        ASSERT_EQ("bit:8", templateArg0Info.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT8, templateArg0Info.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT8, templateArg0Info.getCppType());
        ASSERT_EQ(8, templateArg0Info.getBitSize());

        const zserio::Span<const ColumnInfoType> columns = typeInfo.getColumns();
        ASSERT_EQ(2, columns.size());

        // pk
        const ColumnInfoType& pkColumn = columns[0];
        ASSERT_EQ("pk", pkColumn.schemaName);

        ASSERT_EQ("bit:8", pkColumn.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT8, pkColumn.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT8, pkColumn.typeInfo.getCppType());
        ASSERT_EQ(8, pkColumn.typeInfo.getBitSize());

        ASSERT_EQ(0, pkColumn.typeArguments.size());
        ASSERT_EQ("INTEGER", pkColumn.sqlTypeName);
        ASSERT_EQ("NOT NULL", pkColumn.sqlConstraint);
        ASSERT_EQ(false, pkColumn.isVirtual);

        // withTypeInfoCode
        const ColumnInfoType& withTypeInfoCodeColumn = columns[1];
        ASSERT_EQ("withTypeInfoCode", withTypeInfoCodeColumn.schemaName);
        checkWithTypeInfoCode(withTypeInfoCodeColumn.typeInfo);
        ASSERT_EQ(0, withTypeInfoCodeColumn.typeArguments.size());
        ASSERT_EQ("BLOB", withTypeInfoCodeColumn.sqlTypeName);
        ASSERT_EQ("", withTypeInfoCodeColumn.sqlConstraint);
        ASSERT_EQ(false, withTypeInfoCodeColumn.isVirtual);
    }

    void checkFts4Table(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.Fts4Table", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::SQL_TABLE, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::SQL_TABLE, typeInfo.getCppType());

        ASSERT_EQ("", typeInfo.getSqlConstraint());
        ASSERT_EQ("fts4", typeInfo.getVirtualTableUsing());
        ASSERT_EQ(false, typeInfo.isWithoutRowId());

        ASSERT_EQ("", typeInfo.getTemplateName());
        ASSERT_EQ(0, typeInfo.getTemplateArguments().size());

        const zserio::Span<const ColumnInfoType> columns = typeInfo.getColumns();
        ASSERT_EQ(2, columns.size());

        // docId
        const ColumnInfoType& docIdColumn = columns[0];
        ASSERT_EQ("docId", docIdColumn.schemaName);

        ASSERT_EQ("int:64", docIdColumn.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::INT64, docIdColumn.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::INT64, docIdColumn.typeInfo.getCppType());
        ASSERT_EQ(64, docIdColumn.typeInfo.getBitSize());

        ASSERT_EQ(0, docIdColumn.typeArguments.size());
        ASSERT_EQ("INTEGER", docIdColumn.sqlTypeName);
        ASSERT_EQ("", docIdColumn.sqlConstraint);
        ASSERT_EQ(true, docIdColumn.isVirtual);

        // searchTags
        const ColumnInfoType& searchTagsColumn = columns[1];
        ASSERT_EQ("searchTags", searchTagsColumn.schemaName);

        ASSERT_EQ("string", searchTagsColumn.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::STRING, searchTagsColumn.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::STRING, searchTagsColumn.typeInfo.getCppType());

        ASSERT_EQ(0, searchTagsColumn.typeArguments.size());
        ASSERT_EQ("TEXT", searchTagsColumn.sqlTypeName);
        ASSERT_EQ("", searchTagsColumn.sqlConstraint);
        ASSERT_EQ(false, searchTagsColumn.isVirtual);
    }

    void checkWithoutRowIdTable(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.WithoutRowIdTable", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::SQL_TABLE, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::SQL_TABLE, typeInfo.getCppType());

        ASSERT_EQ("PRIMARY KEY(pk1, pk2)", typeInfo.getSqlConstraint());
        ASSERT_EQ("", typeInfo.getVirtualTableUsing());
        ASSERT_EQ(true, typeInfo.isWithoutRowId());

        ASSERT_EQ("", typeInfo.getTemplateName());
        ASSERT_EQ(0, typeInfo.getTemplateArguments().size());

        const zserio::Span<const ColumnInfoType> columns = typeInfo.getColumns();
        ASSERT_EQ(2, columns.size());

        // pk1
        const ColumnInfoType& pk1Column = columns[0];
        ASSERT_EQ("pk1", pk1Column.schemaName);

        ASSERT_EQ("bit:32", pk1Column.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, pk1Column.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, pk1Column.typeInfo.getCppType());
        ASSERT_EQ(32, pk1Column.typeInfo.getBitSize());

        ASSERT_EQ(0, pk1Column.typeArguments.size());
        ASSERT_EQ("INTEGER", pk1Column.sqlTypeName);
        ASSERT_EQ("NOT NULL", pk1Column.sqlConstraint);
        ASSERT_EQ(false, pk1Column.isVirtual);

        // pk2
        const ColumnInfoType& pk2Column = columns[1];
        ASSERT_EQ("pk2", pk2Column.schemaName);

        ASSERT_EQ("bit:32", pk2Column.typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::UINT32, pk2Column.typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, pk2Column.typeInfo.getCppType());
        ASSERT_EQ(32, pk2Column.typeInfo.getBitSize());

        ASSERT_EQ(0, pk2Column.typeArguments.size());
        ASSERT_EQ("INTEGER", pk2Column.sqlTypeName);
        ASSERT_EQ("NOT NULL", pk2Column.sqlConstraint);
        ASSERT_EQ(false, pk2Column.isVirtual);
    }

    void checkSimplePubsub(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.SimplePubsub", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::PUBSUB, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::PUBSUB, typeInfo.getCppType());

        ASSERT_THROW(typeInfo.getTemplateName(), zserio::CppRuntimeException);
        ASSERT_THROW(typeInfo.getTemplateArguments().size(), zserio::CppRuntimeException);

        const zserio::Span<const MessageInfoType> messages = typeInfo.getMessages();
        ASSERT_EQ(2, messages.size());

        // pubSimpleStruct
        const MessageInfoType& pubSimpleStructMessage = messages[0];
        ASSERT_EQ("pubSimpleStruct", pubSimpleStructMessage.schemaName);
        checkSimpleStruct(pubSimpleStructMessage.typeInfo);
        ASSERT_EQ(true, pubSimpleStructMessage.isPublished);
        ASSERT_EQ(false, pubSimpleStructMessage.isSubscribed);
        ASSERT_EQ("simpleStruct", pubSimpleStructMessage.topic);

        // subSimpleStruct
        const MessageInfoType& subSimpleStructMessage = messages[1];
        ASSERT_EQ("subSimpleStruct", subSimpleStructMessage.schemaName);
        checkSimpleStruct(subSimpleStructMessage.typeInfo);
        ASSERT_EQ(false, subSimpleStructMessage.isPublished);
        ASSERT_EQ(true, subSimpleStructMessage.isSubscribed);
        ASSERT_EQ("simpleStruct", subSimpleStructMessage.topic);
    }

    void checkSimpleService(const ITypeInfoType& typeInfo)
    {
        ASSERT_EQ("with_type_info_code.SimpleService", typeInfo.getSchemaName());
        ASSERT_EQ(zserio::SchemaType::SERVICE, typeInfo.getSchemaType());
        ASSERT_EQ(zserio::CppType::SERVICE, typeInfo.getCppType());

        ASSERT_THROW(typeInfo.getTemplateName(), zserio::CppRuntimeException);
        ASSERT_THROW(typeInfo.getTemplateArguments().size(), zserio::CppRuntimeException);

        const zserio::Span<const MethodInfoType> methods = typeInfo.getMethods();
        ASSERT_EQ(1, methods.size());

        // getSimpleStruct
        const MethodInfoType& getSimpleStructMethod = methods[0];
        ASSERT_EQ("getSimpleStruct", getSimpleStructMethod.schemaName);

        checkSimpleStruct(getSimpleStructMethod.responseTypeInfo);
        checkSimpleUnion(getSimpleStructMethod.requestTypeInfo);
    }
};

TEST_F(WithTypeInfoCodeTest, checkSqlDatabase)
{
    checkSqlDatabase(zserio::typeInfo<SqlDatabase>());
}

TEST_F(WithTypeInfoCodeTest, checkSimplePubsub)
{
    checkSimplePubsub(zserio::typeInfo<SimplePubsub>());
}

TEST_F(WithTypeInfoCodeTest, checkSimpleService)
{
    checkSimpleService(zserio::typeInfo<SimpleService::Service>());
    checkSimpleService(zserio::typeInfo<SimpleService::Client>());
}

TEST_F(WithTypeInfoCodeTest, writeReadFileWithOptionals)
{
    WithTypeInfoCode data;
    fillWithTypeInfoCode(data);
    test_utils::writeReadFileTest("arguments/with_type_info_code/with_type_info_code_optionals.blob", data);
}

TEST_F(WithTypeInfoCodeTest, writeReadFileWithoutOptionals)
{
    WithTypeInfoCode data;
    const bool createOptionals = false;
    fillWithTypeInfoCode(data, createOptionals);
    test_utils::writeReadFileTest("arguments/with_type_info_code/with_type_info_code.blob", data);
}

} // namespace with_type_info_code
