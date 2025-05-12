#include <string>

#include "gtest/gtest.h"
#include "zserio/TypeInfo.h"

namespace zserio
{

namespace
{

class RecursiveObject
{
public:
    static const ITypeInfo& typeInfo()
    {
        static const detail::RecursiveTypeInfo<std::allocator<uint8_t>> recursiveTypeInfo(
                &RecursiveObject::typeInfo);
        static const std::array<FieldInfo, 1> fields = {FieldInfo{"recursive", recursiveTypeInfo, {},
                false, // isExtended
                {}, {}, {},
                true, // isOptional
                {}, {}, false, {}, false, false}};

        static const detail::StructTypeInfo<std::allocator<uint8_t>> structTypeInfo(
                "RecursiveObject", nullptr, "", {}, fields, {}, {});
        return structTypeInfo;
    }
};

} // namespace

class TypeInfoTest : public ::testing::Test
{
protected:
    void checkBuiltinTypeInfo(const ITypeInfo& typeInfo, std::string_view schemaName, SchemaType schemaType,
            CppType cppType, uint8_t bitSize = 0)
    {
        ASSERT_EQ(schemaName, typeInfo.getSchemaName());
        ASSERT_EQ(schemaType, typeInfo.getSchemaType());
        ASSERT_EQ(cppType, typeInfo.getCppType());
        if (bitSize > 0)
        {
            ASSERT_EQ(bitSize, typeInfo.getBitSize());
        }
        else
        {
            ASSERT_THROW(typeInfo.getBitSize(), CppRuntimeException);
        }

        ASSERT_THROW(typeInfo.getFields(), CppRuntimeException);
        ASSERT_THROW(typeInfo.getParameters(), CppRuntimeException);
        ASSERT_THROW(typeInfo.getFunctions(), CppRuntimeException);

        ASSERT_THROW(typeInfo.getSelector(), CppRuntimeException);
        ASSERT_THROW(typeInfo.getCases(), CppRuntimeException);

        ASSERT_THROW(typeInfo.getUnderlyingType(), CppRuntimeException);
        ASSERT_THROW(typeInfo.getEnumItems(), CppRuntimeException);
        ASSERT_THROW(typeInfo.getBitmaskValues(), CppRuntimeException);

        ASSERT_THROW(typeInfo.getColumns(), CppRuntimeException);
        ASSERT_THROW(typeInfo.getSqlConstraint(), CppRuntimeException);
        ASSERT_THROW(typeInfo.getVirtualTableUsing(), CppRuntimeException);
        ASSERT_THROW(typeInfo.isWithoutRowId(), CppRuntimeException);

        ASSERT_THROW(typeInfo.getTables(), CppRuntimeException);

        ASSERT_THROW(typeInfo.getTemplateName(), CppRuntimeException);
        ASSERT_THROW(typeInfo.getTemplateArguments(), CppRuntimeException);

        ASSERT_THROW(typeInfo.getMessages(), CppRuntimeException);

        ASSERT_THROW(typeInfo.getMethods(), CppRuntimeException);

        ASSERT_THROW(typeInfo.createInstance(), CppRuntimeException);
    }
};

// TODO[Mi-L@]: Test all structures (e.g. FieldInfo).

TEST_F(TypeInfoTest, builtinTypeInfo)
{
    checkBuiltinTypeInfo(typeInfo<Bool>(), "bool", SchemaType::BOOL, CppType::BOOL, 1);

    checkBuiltinTypeInfo(typeInfo<VarInt16>(), "varint16", SchemaType::VARINT16, CppType::INT16);
    checkBuiltinTypeInfo(typeInfo<VarInt32>(), "varint32", SchemaType::VARINT32, CppType::INT32);
    checkBuiltinTypeInfo(typeInfo<VarInt64>(), "varint64", SchemaType::VARINT64, CppType::INT64);
    checkBuiltinTypeInfo(typeInfo<VarInt>(), "varint", SchemaType::VARINT, CppType::INT64);
    checkBuiltinTypeInfo(typeInfo<VarUInt16>(), "varuint16", SchemaType::VARUINT16, CppType::UINT16);
    checkBuiltinTypeInfo(typeInfo<VarUInt32>(), "varuint32", SchemaType::VARUINT32, CppType::UINT32);
    checkBuiltinTypeInfo(typeInfo<VarUInt64>(), "varuint64", SchemaType::VARUINT64, CppType::UINT64);
    checkBuiltinTypeInfo(typeInfo<VarUInt>(), "varuint", SchemaType::VARUINT, CppType::UINT64);
    checkBuiltinTypeInfo(typeInfo<VarSize>(), "varsize", SchemaType::VARSIZE, CppType::UINT32);

    checkBuiltinTypeInfo(typeInfo<Float16>(), "float16", SchemaType::FLOAT16, CppType::FLOAT, 16);
    checkBuiltinTypeInfo(typeInfo<Float32>(), "float32", SchemaType::FLOAT32, CppType::FLOAT, 32);
    checkBuiltinTypeInfo(typeInfo<Float64>(), "float64", SchemaType::FLOAT64, CppType::DOUBLE, 64);

    checkBuiltinTypeInfo(typeInfo<String>(), "string", SchemaType::STRING, CppType::STRING);
    checkBuiltinTypeInfo(typeInfo<std::string_view>(), "string", SchemaType::STRING, CppType::STRING);

    checkBuiltinTypeInfo(typeInfo<BitBuffer>(), "extern", SchemaType::EXTERN, CppType::BIT_BUFFER);

    checkBuiltinTypeInfo(typeInfo<Bytes>(), "bytes", SchemaType::BYTES, CppType::BYTES);

    checkBuiltinTypeInfo(typeInfo<Int1>(), "int:1", SchemaType::INT1, CppType::INT8, 1);
    checkBuiltinTypeInfo(typeInfo<Int7>(), "int:7", SchemaType::INT7, CppType::INT8, 7);
    checkBuiltinTypeInfo(typeInfo<Int8>(), "int:8", SchemaType::INT8, CppType::INT8, 8);
    checkBuiltinTypeInfo(typeInfo<Int9>(), "int:9", SchemaType::INT9, CppType::INT16, 9);
    checkBuiltinTypeInfo(typeInfo<Int15>(), "int:15", SchemaType::INT15, CppType::INT16, 15);
    checkBuiltinTypeInfo(typeInfo<Int16>(), "int:16", SchemaType::INT16, CppType::INT16, 16);
    checkBuiltinTypeInfo(typeInfo<Int17>(), "int:17", SchemaType::INT17, CppType::INT32, 17);
    checkBuiltinTypeInfo(typeInfo<Int31>(), "int:31", SchemaType::INT31, CppType::INT32, 31);
    checkBuiltinTypeInfo(typeInfo<Int32>(), "int:32", SchemaType::INT32, CppType::INT32, 32);
    checkBuiltinTypeInfo(typeInfo<Int33>(), "int:33", SchemaType::INT33, CppType::INT64, 33);
    checkBuiltinTypeInfo(typeInfo<Int63>(), "int:63", SchemaType::INT63, CppType::INT64, 63);
    checkBuiltinTypeInfo(typeInfo<Int64>(), "int:64", SchemaType::INT64, CppType::INT64, 64);

    checkBuiltinTypeInfo(typeInfo<UInt1>(), "bit:1", SchemaType::UINT1, CppType::UINT8, 1);
    checkBuiltinTypeInfo(typeInfo<UInt7>(), "bit:7", SchemaType::UINT7, CppType::UINT8, 7);
    checkBuiltinTypeInfo(typeInfo<UInt8>(), "bit:8", SchemaType::UINT8, CppType::UINT8, 8);
    checkBuiltinTypeInfo(typeInfo<UInt9>(), "bit:9", SchemaType::UINT9, CppType::UINT16, 9);
    checkBuiltinTypeInfo(typeInfo<UInt15>(), "bit:15", SchemaType::UINT15, CppType::UINT16, 15);
    checkBuiltinTypeInfo(typeInfo<UInt16>(), "bit:16", SchemaType::UINT16, CppType::UINT16, 16);
    checkBuiltinTypeInfo(typeInfo<UInt17>(), "bit:17", SchemaType::UINT17, CppType::UINT32, 17);
    checkBuiltinTypeInfo(typeInfo<UInt31>(), "bit:31", SchemaType::UINT31, CppType::UINT32, 31);
    checkBuiltinTypeInfo(typeInfo<UInt32>(), "bit:32", SchemaType::UINT32, CppType::UINT32, 32);
    checkBuiltinTypeInfo(typeInfo<UInt33>(), "bit:33", SchemaType::UINT33, CppType::UINT64, 33);
    checkBuiltinTypeInfo(typeInfo<UInt63>(), "bit:63", SchemaType::UINT63, CppType::UINT64, 63);
    checkBuiltinTypeInfo(typeInfo<UInt64>(), "bit:64", SchemaType::UINT64, CppType::UINT64, 64);

    checkBuiltinTypeInfo(typeInfo<Int<1>>(), "int:1", SchemaType::INT1, CppType::INT8, 1);
    checkBuiltinTypeInfo(typeInfo<Int<7>>(), "int:7", SchemaType::INT7, CppType::INT8, 7);
    checkBuiltinTypeInfo(typeInfo<Int<9>>(), "int:9", SchemaType::INT9, CppType::INT16, 9);
    checkBuiltinTypeInfo(typeInfo<Int<15>>(), "int:15", SchemaType::INT15, CppType::INT16, 15);
    checkBuiltinTypeInfo(typeInfo<Int<17>>(), "int:17", SchemaType::INT17, CppType::INT32, 17);
    checkBuiltinTypeInfo(typeInfo<Int<31>>(), "int:31", SchemaType::INT31, CppType::INT32, 31);
    checkBuiltinTypeInfo(typeInfo<Int<33>>(), "int:33", SchemaType::INT33, CppType::INT64, 33);
    checkBuiltinTypeInfo(typeInfo<Int<63>>(), "int:63", SchemaType::INT63, CppType::INT64, 63);

    checkBuiltinTypeInfo(typeInfo<UInt<1>>(), "bit:1", SchemaType::UINT1, CppType::UINT8, 1);
    checkBuiltinTypeInfo(typeInfo<UInt<7>>(), "bit:7", SchemaType::UINT7, CppType::UINT8, 7);
    checkBuiltinTypeInfo(typeInfo<UInt<9>>(), "bit:9", SchemaType::UINT9, CppType::UINT16, 9);
    checkBuiltinTypeInfo(typeInfo<UInt<15>>(), "bit:15", SchemaType::UINT15, CppType::UINT16, 15);
    checkBuiltinTypeInfo(typeInfo<UInt<17>>(), "bit:17", SchemaType::UINT17, CppType::UINT32, 17);
    checkBuiltinTypeInfo(typeInfo<UInt<31>>(), "bit:31", SchemaType::UINT31, CppType::UINT32, 31);
    checkBuiltinTypeInfo(typeInfo<UInt<33>>(), "bit:33", SchemaType::UINT33, CppType::UINT64, 33);
    checkBuiltinTypeInfo(typeInfo<UInt<63>>(), "bit:63", SchemaType::UINT63, CppType::UINT64, 63);

    checkBuiltinTypeInfo(typeInfo<DynInt8>(), "int<>", SchemaType::DYNAMIC_SIGNED_BITFIELD, CppType::INT8);
    checkBuiltinTypeInfo(typeInfo<DynInt16>(), "int<>", SchemaType::DYNAMIC_SIGNED_BITFIELD, CppType::INT16);
    checkBuiltinTypeInfo(typeInfo<DynInt32>(), "int<>", SchemaType::DYNAMIC_SIGNED_BITFIELD, CppType::INT32);
    checkBuiltinTypeInfo(typeInfo<DynInt64>(), "int<>", SchemaType::DYNAMIC_SIGNED_BITFIELD, CppType::INT64);
    checkBuiltinTypeInfo(typeInfo<DynUInt8>(), "bit<>", SchemaType::DYNAMIC_UNSIGNED_BITFIELD, CppType::UINT8);
    checkBuiltinTypeInfo(
            typeInfo<DynUInt16>(), "bit<>", SchemaType::DYNAMIC_UNSIGNED_BITFIELD, CppType::UINT16);
    checkBuiltinTypeInfo(
            typeInfo<DynUInt32>(), "bit<>", SchemaType::DYNAMIC_UNSIGNED_BITFIELD, CppType::UINT32);
    checkBuiltinTypeInfo(
            typeInfo<DynUInt64>(), "bit<>", SchemaType::DYNAMIC_UNSIGNED_BITFIELD, CppType::UINT64);
}

TEST_F(TypeInfoTest, structTypeInfo)
{
    const detail::StructTypeInfo<std::allocator<uint8_t>> structTypeInfo("", nullptr, "", {}, {}, {}, {});
    ASSERT_EQ("", structTypeInfo.getSchemaName());
    ASSERT_EQ(SchemaType::STRUCT, structTypeInfo.getSchemaType());
    ASSERT_EQ(CppType::STRUCT, structTypeInfo.getCppType());
    ASSERT_THROW(structTypeInfo.getBitSize(), CppRuntimeException);

    ASSERT_EQ(0, structTypeInfo.getFields().size());
    ASSERT_EQ(0, structTypeInfo.getParameters().size());
    ASSERT_EQ(0, structTypeInfo.getFunctions().size());

    ASSERT_THROW(structTypeInfo.getSelector(), CppRuntimeException);
    ASSERT_THROW(structTypeInfo.getCases(), CppRuntimeException);

    ASSERT_THROW(structTypeInfo.getUnderlyingType(), CppRuntimeException);
    ASSERT_THROW(structTypeInfo.getEnumItems(), CppRuntimeException);
    ASSERT_THROW(structTypeInfo.getBitmaskValues(), CppRuntimeException);

    ASSERT_THROW(structTypeInfo.getColumns(), CppRuntimeException);
    ASSERT_THROW(structTypeInfo.getSqlConstraint(), CppRuntimeException);
    ASSERT_THROW(structTypeInfo.getVirtualTableUsing(), CppRuntimeException);
    ASSERT_THROW(structTypeInfo.isWithoutRowId(), CppRuntimeException);

    ASSERT_THROW(structTypeInfo.getTables(), CppRuntimeException);

    ASSERT_EQ("", structTypeInfo.getTemplateName());
    ASSERT_EQ(0, structTypeInfo.getTemplateArguments().size());

    ASSERT_THROW(structTypeInfo.getMessages(), CppRuntimeException);

    ASSERT_THROW(structTypeInfo.getMethods(), CppRuntimeException);

    ASSERT_THROW(structTypeInfo.createInstance(std::allocator<uint8_t>()), CppRuntimeException);
}

TEST_F(TypeInfoTest, unionTypeInfo)
{
    const detail::UnionTypeInfo<std::allocator<uint8_t>> unionTypeInfo("", nullptr, "", {}, {}, {}, {});
    ASSERT_EQ("", unionTypeInfo.getSchemaName());
    ASSERT_EQ(SchemaType::UNION, unionTypeInfo.getSchemaType());
    ASSERT_EQ(CppType::UNION, unionTypeInfo.getCppType());
    ASSERT_THROW(unionTypeInfo.getBitSize(), CppRuntimeException);

    ASSERT_EQ(0, unionTypeInfo.getFields().size());
    ASSERT_EQ(0, unionTypeInfo.getParameters().size());
    ASSERT_EQ(0, unionTypeInfo.getFunctions().size());

    ASSERT_THROW(unionTypeInfo.getSelector(), CppRuntimeException);
    ASSERT_THROW(unionTypeInfo.getCases(), CppRuntimeException);

    ASSERT_THROW(unionTypeInfo.getUnderlyingType(), CppRuntimeException);
    ASSERT_THROW(unionTypeInfo.getEnumItems(), CppRuntimeException);
    ASSERT_THROW(unionTypeInfo.getBitmaskValues(), CppRuntimeException);

    ASSERT_THROW(unionTypeInfo.getColumns(), CppRuntimeException);
    ASSERT_THROW(unionTypeInfo.getSqlConstraint(), CppRuntimeException);
    ASSERT_THROW(unionTypeInfo.getVirtualTableUsing(), CppRuntimeException);
    ASSERT_THROW(unionTypeInfo.isWithoutRowId(), CppRuntimeException);

    ASSERT_THROW(unionTypeInfo.getTables(), CppRuntimeException);

    ASSERT_EQ("", unionTypeInfo.getTemplateName());
    ASSERT_EQ(0, unionTypeInfo.getTemplateArguments().size());

    ASSERT_THROW(unionTypeInfo.getMessages(), CppRuntimeException);

    ASSERT_THROW(unionTypeInfo.getMethods(), CppRuntimeException);

    ASSERT_THROW(unionTypeInfo.createInstance(std::allocator<uint8_t>()), CppRuntimeException);
}

TEST_F(TypeInfoTest, choiceTypeInfo)
{
    const detail::ChoiceTypeInfo<std::allocator<uint8_t>> choiceTypeInfo(
            "", nullptr, "", {}, {}, {}, {}, "", {});
    ASSERT_EQ("", choiceTypeInfo.getSchemaName());
    ASSERT_EQ(SchemaType::CHOICE, choiceTypeInfo.getSchemaType());
    ASSERT_EQ(CppType::CHOICE, choiceTypeInfo.getCppType());
    ASSERT_THROW(choiceTypeInfo.getBitSize(), CppRuntimeException);

    ASSERT_EQ(0, choiceTypeInfo.getFields().size());
    ASSERT_EQ(0, choiceTypeInfo.getParameters().size());
    ASSERT_EQ(0, choiceTypeInfo.getFunctions().size());

    ASSERT_EQ("", choiceTypeInfo.getSelector());
    ASSERT_EQ(0, choiceTypeInfo.getCases().size());

    ASSERT_THROW(choiceTypeInfo.getUnderlyingType(), CppRuntimeException);
    ASSERT_THROW(choiceTypeInfo.getEnumItems(), CppRuntimeException);
    ASSERT_THROW(choiceTypeInfo.getBitmaskValues(), CppRuntimeException);

    ASSERT_THROW(choiceTypeInfo.getColumns(), CppRuntimeException);
    ASSERT_THROW(choiceTypeInfo.getSqlConstraint(), CppRuntimeException);
    ASSERT_THROW(choiceTypeInfo.getVirtualTableUsing(), CppRuntimeException);
    ASSERT_THROW(choiceTypeInfo.isWithoutRowId(), CppRuntimeException);

    ASSERT_THROW(choiceTypeInfo.getTables(), CppRuntimeException);

    ASSERT_EQ("", choiceTypeInfo.getTemplateName());
    ASSERT_EQ(0, choiceTypeInfo.getTemplateArguments().size());

    ASSERT_THROW(choiceTypeInfo.getMessages(), CppRuntimeException);

    ASSERT_THROW(choiceTypeInfo.getMethods(), CppRuntimeException);

    ASSERT_THROW(choiceTypeInfo.createInstance(std::allocator<uint8_t>()), CppRuntimeException);
}

TEST_F(TypeInfoTest, sqlTableTypeInfo)
{
    const detail::SqlTableTypeInfo<std::allocator<uint8_t>> sqlTableTypeInfo("", "", {}, {}, "", "", false);
    ASSERT_EQ("", sqlTableTypeInfo.getSchemaName());
    ASSERT_EQ(SchemaType::SQL_TABLE, sqlTableTypeInfo.getSchemaType());
    ASSERT_EQ(CppType::SQL_TABLE, sqlTableTypeInfo.getCppType());
    ASSERT_THROW(sqlTableTypeInfo.getBitSize(), CppRuntimeException);

    ASSERT_THROW(sqlTableTypeInfo.getFields(), CppRuntimeException);
    ASSERT_THROW(sqlTableTypeInfo.getParameters(), CppRuntimeException);
    ASSERT_THROW(sqlTableTypeInfo.getFunctions(), CppRuntimeException);

    ASSERT_THROW(sqlTableTypeInfo.getSelector(), CppRuntimeException);
    ASSERT_THROW(sqlTableTypeInfo.getCases(), CppRuntimeException);

    ASSERT_THROW(sqlTableTypeInfo.getUnderlyingType(), CppRuntimeException);
    ASSERT_THROW(sqlTableTypeInfo.getEnumItems(), CppRuntimeException);
    ASSERT_THROW(sqlTableTypeInfo.getBitmaskValues(), CppRuntimeException);

    ASSERT_EQ(0, sqlTableTypeInfo.getColumns().size());
    ASSERT_EQ("", sqlTableTypeInfo.getSqlConstraint());
    ASSERT_EQ("", sqlTableTypeInfo.getVirtualTableUsing());
    ASSERT_EQ(false, sqlTableTypeInfo.isWithoutRowId());

    ASSERT_THROW(sqlTableTypeInfo.getTables(), CppRuntimeException);

    ASSERT_EQ("", sqlTableTypeInfo.getTemplateName());
    ASSERT_EQ(0, sqlTableTypeInfo.getTemplateArguments().size());

    ASSERT_THROW(sqlTableTypeInfo.getMessages(), CppRuntimeException);

    ASSERT_THROW(sqlTableTypeInfo.getMethods(), CppRuntimeException);

    ASSERT_THROW(sqlTableTypeInfo.createInstance(std::allocator<uint8_t>()), CppRuntimeException);
}

TEST_F(TypeInfoTest, sqlDatabaseTypeInfo)
{
    const detail::SqlDatabaseTypeInfo<std::allocator<uint8_t>> sqlDatabaseTypeInfo("", {});
    ASSERT_EQ("", sqlDatabaseTypeInfo.getSchemaName());
    ASSERT_EQ(SchemaType::SQL_DATABASE, sqlDatabaseTypeInfo.getSchemaType());
    ASSERT_EQ(CppType::SQL_DATABASE, sqlDatabaseTypeInfo.getCppType());
    ASSERT_THROW(sqlDatabaseTypeInfo.getBitSize(), CppRuntimeException);

    ASSERT_THROW(sqlDatabaseTypeInfo.getFields(), CppRuntimeException);
    ASSERT_THROW(sqlDatabaseTypeInfo.getParameters(), CppRuntimeException);
    ASSERT_THROW(sqlDatabaseTypeInfo.getFunctions(), CppRuntimeException);

    ASSERT_THROW(sqlDatabaseTypeInfo.getSelector(), CppRuntimeException);
    ASSERT_THROW(sqlDatabaseTypeInfo.getCases(), CppRuntimeException);

    ASSERT_THROW(sqlDatabaseTypeInfo.getUnderlyingType(), CppRuntimeException);
    ASSERT_THROW(sqlDatabaseTypeInfo.getEnumItems(), CppRuntimeException);
    ASSERT_THROW(sqlDatabaseTypeInfo.getBitmaskValues(), CppRuntimeException);

    ASSERT_THROW(sqlDatabaseTypeInfo.getColumns(), CppRuntimeException);
    ASSERT_THROW(sqlDatabaseTypeInfo.getSqlConstraint(), CppRuntimeException);
    ASSERT_THROW(sqlDatabaseTypeInfo.getVirtualTableUsing(), CppRuntimeException);
    ASSERT_THROW(sqlDatabaseTypeInfo.isWithoutRowId(), CppRuntimeException);

    ASSERT_EQ(0, sqlDatabaseTypeInfo.getTables().size());

    ASSERT_THROW(sqlDatabaseTypeInfo.getTemplateName(), CppRuntimeException);
    ASSERT_THROW(sqlDatabaseTypeInfo.getTemplateArguments(), CppRuntimeException);

    ASSERT_THROW(sqlDatabaseTypeInfo.getMessages(), CppRuntimeException);

    ASSERT_THROW(sqlDatabaseTypeInfo.getMethods(), CppRuntimeException);

    ASSERT_THROW(sqlDatabaseTypeInfo.createInstance(std::allocator<uint8_t>()), CppRuntimeException);
}

TEST_F(TypeInfoTest, enumTypeInfo)
{
    const ITypeInfo& underlyingTypeInfo = typeInfo<Int8>();
    std::array<ItemInfo, 1> enumItems = {ItemInfo("ONE", 1, false, false)};
    const detail::EnumTypeInfo<std::allocator<uint8_t>> enumTypeInfo("", underlyingTypeInfo, enumItems);
    ASSERT_EQ("", enumTypeInfo.getSchemaName());
    ASSERT_EQ(SchemaType::ENUM, enumTypeInfo.getSchemaType());
    ASSERT_EQ(CppType::ENUM, enumTypeInfo.getCppType());
    ASSERT_THROW(enumTypeInfo.getBitSize(), CppRuntimeException);

    ASSERT_THROW(enumTypeInfo.getFields(), CppRuntimeException);
    ASSERT_THROW(enumTypeInfo.getParameters(), CppRuntimeException);
    ASSERT_THROW(enumTypeInfo.getFunctions(), CppRuntimeException);

    ASSERT_THROW(enumTypeInfo.getSelector(), CppRuntimeException);
    ASSERT_THROW(enumTypeInfo.getCases(), CppRuntimeException);

    ASSERT_EQ(&underlyingTypeInfo, &enumTypeInfo.getUnderlyingType());
    ASSERT_EQ(1, enumTypeInfo.getEnumItems().size());
    ASSERT_EQ("ONE", enumTypeInfo.getEnumItems()[0].schemaName);
    ASSERT_EQ(1, enumTypeInfo.getEnumItems()[0].value);
    ASSERT_FALSE(enumTypeInfo.getEnumItems()[0].isDeprecated);
    ASSERT_FALSE(enumTypeInfo.getEnumItems()[0].isRemoved);
    ASSERT_THROW(enumTypeInfo.getBitmaskValues(), CppRuntimeException);

    ASSERT_THROW(enumTypeInfo.getColumns(), CppRuntimeException);
    ASSERT_THROW(enumTypeInfo.getSqlConstraint(), CppRuntimeException);
    ASSERT_THROW(enumTypeInfo.getVirtualTableUsing(), CppRuntimeException);
    ASSERT_THROW(enumTypeInfo.isWithoutRowId(), CppRuntimeException);

    ASSERT_THROW(enumTypeInfo.getTables(), CppRuntimeException);

    ASSERT_THROW(enumTypeInfo.getTemplateName(), CppRuntimeException);
    ASSERT_THROW(enumTypeInfo.getTemplateArguments(), CppRuntimeException);

    ASSERT_THROW(enumTypeInfo.getMessages(), CppRuntimeException);

    ASSERT_THROW(enumTypeInfo.getMethods(), CppRuntimeException);

    ASSERT_THROW(enumTypeInfo.createInstance(std::allocator<uint8_t>()), CppRuntimeException);
}

TEST_F(TypeInfoTest, bitmaskTypeInfo)
{
    const ITypeInfo& underlyingTypeInfo = typeInfo<Int8>();
    std::array<ItemInfo, 1> bitmaskValues = {ItemInfo("FIRST_BIT", 1, false, false)};
    const detail::BitmaskTypeInfo<std::allocator<uint8_t>> bitmaskTypeInfo(
            "", underlyingTypeInfo, bitmaskValues);
    ASSERT_EQ("", bitmaskTypeInfo.getSchemaName());
    ASSERT_EQ(SchemaType::BITMASK, bitmaskTypeInfo.getSchemaType());
    ASSERT_EQ(CppType::BITMASK, bitmaskTypeInfo.getCppType());
    ASSERT_THROW(bitmaskTypeInfo.getBitSize(), CppRuntimeException);

    ASSERT_THROW(bitmaskTypeInfo.getFields(), CppRuntimeException);
    ASSERT_THROW(bitmaskTypeInfo.getParameters(), CppRuntimeException);
    ASSERT_THROW(bitmaskTypeInfo.getFunctions(), CppRuntimeException);

    ASSERT_THROW(bitmaskTypeInfo.getSelector(), CppRuntimeException);
    ASSERT_THROW(bitmaskTypeInfo.getCases(), CppRuntimeException);

    ASSERT_EQ(&underlyingTypeInfo, &bitmaskTypeInfo.getUnderlyingType());
    ASSERT_THROW(bitmaskTypeInfo.getEnumItems(), CppRuntimeException);
    ASSERT_EQ(1, bitmaskTypeInfo.getBitmaskValues().size());
    ASSERT_EQ("FIRST_BIT", bitmaskTypeInfo.getBitmaskValues()[0].schemaName);
    ASSERT_EQ(1, bitmaskTypeInfo.getBitmaskValues()[0].value);
    ASSERT_EQ(false, bitmaskTypeInfo.getBitmaskValues()[0].isDeprecated);
    ASSERT_EQ(false, bitmaskTypeInfo.getBitmaskValues()[0].isRemoved);

    ASSERT_THROW(bitmaskTypeInfo.getColumns(), CppRuntimeException);
    ASSERT_THROW(bitmaskTypeInfo.getSqlConstraint(), CppRuntimeException);
    ASSERT_THROW(bitmaskTypeInfo.getVirtualTableUsing(), CppRuntimeException);
    ASSERT_THROW(bitmaskTypeInfo.isWithoutRowId(), CppRuntimeException);

    ASSERT_THROW(bitmaskTypeInfo.getTables(), CppRuntimeException);

    ASSERT_THROW(bitmaskTypeInfo.getTemplateName(), CppRuntimeException);
    ASSERT_THROW(bitmaskTypeInfo.getTemplateArguments(), CppRuntimeException);

    ASSERT_THROW(bitmaskTypeInfo.getMessages(), CppRuntimeException);

    ASSERT_THROW(bitmaskTypeInfo.getMethods(), CppRuntimeException);

    ASSERT_THROW(bitmaskTypeInfo.createInstance(std::allocator<uint8_t>()), CppRuntimeException);
}

TEST_F(TypeInfoTest, pubsubTypeInfo)
{
    const detail::PubsubTypeInfo<std::allocator<uint8_t>> pubsubTypeInfo("", {});
    ASSERT_EQ("", pubsubTypeInfo.getSchemaName());
    ASSERT_EQ(SchemaType::PUBSUB, pubsubTypeInfo.getSchemaType());
    ASSERT_EQ(CppType::PUBSUB, pubsubTypeInfo.getCppType());
    ASSERT_THROW(pubsubTypeInfo.getBitSize(), CppRuntimeException);

    ASSERT_THROW(pubsubTypeInfo.getFields(), CppRuntimeException);
    ASSERT_THROW(pubsubTypeInfo.getParameters(), CppRuntimeException);
    ASSERT_THROW(pubsubTypeInfo.getFunctions(), CppRuntimeException);

    ASSERT_THROW(pubsubTypeInfo.getSelector(), CppRuntimeException);
    ASSERT_THROW(pubsubTypeInfo.getCases(), CppRuntimeException);

    ASSERT_THROW(pubsubTypeInfo.getUnderlyingType(), CppRuntimeException);
    ASSERT_THROW(pubsubTypeInfo.getEnumItems(), CppRuntimeException);
    ASSERT_THROW(pubsubTypeInfo.getBitmaskValues(), CppRuntimeException);

    ASSERT_THROW(pubsubTypeInfo.getColumns(), CppRuntimeException);
    ASSERT_THROW(pubsubTypeInfo.getSqlConstraint(), CppRuntimeException);
    ASSERT_THROW(pubsubTypeInfo.getVirtualTableUsing(), CppRuntimeException);
    ASSERT_THROW(pubsubTypeInfo.isWithoutRowId(), CppRuntimeException);

    ASSERT_THROW(pubsubTypeInfo.getTables(), CppRuntimeException);

    ASSERT_THROW(pubsubTypeInfo.getTemplateName(), CppRuntimeException);
    ASSERT_THROW(pubsubTypeInfo.getTemplateArguments(), CppRuntimeException);

    ASSERT_EQ(0, pubsubTypeInfo.getMessages().size());

    ASSERT_THROW(pubsubTypeInfo.getMethods(), CppRuntimeException);

    ASSERT_THROW(pubsubTypeInfo.createInstance(std::allocator<uint8_t>()), CppRuntimeException);
}

TEST_F(TypeInfoTest, serviceTypeInfo)
{
    const detail::ServiceTypeInfo<std::allocator<uint8_t>> serviceTypeInfo("", {});
    ASSERT_EQ("", serviceTypeInfo.getSchemaName());
    ASSERT_EQ(SchemaType::SERVICE, serviceTypeInfo.getSchemaType());
    ASSERT_EQ(CppType::SERVICE, serviceTypeInfo.getCppType());
    ASSERT_THROW(serviceTypeInfo.getBitSize(), CppRuntimeException);

    ASSERT_THROW(serviceTypeInfo.getFields(), CppRuntimeException);
    ASSERT_THROW(serviceTypeInfo.getParameters(), CppRuntimeException);
    ASSERT_THROW(serviceTypeInfo.getFunctions(), CppRuntimeException);

    ASSERT_THROW(serviceTypeInfo.getSelector(), CppRuntimeException);
    ASSERT_THROW(serviceTypeInfo.getCases(), CppRuntimeException);

    ASSERT_THROW(serviceTypeInfo.getUnderlyingType(), CppRuntimeException);
    ASSERT_THROW(serviceTypeInfo.getEnumItems(), CppRuntimeException);
    ASSERT_THROW(serviceTypeInfo.getBitmaskValues(), CppRuntimeException);

    ASSERT_THROW(serviceTypeInfo.getColumns(), CppRuntimeException);
    ASSERT_THROW(serviceTypeInfo.getSqlConstraint(), CppRuntimeException);
    ASSERT_THROW(serviceTypeInfo.getVirtualTableUsing(), CppRuntimeException);
    ASSERT_THROW(serviceTypeInfo.isWithoutRowId(), CppRuntimeException);

    ASSERT_THROW(serviceTypeInfo.getTables(), CppRuntimeException);

    ASSERT_THROW(serviceTypeInfo.getTemplateName(), CppRuntimeException);
    ASSERT_THROW(serviceTypeInfo.getTemplateArguments(), CppRuntimeException);

    ASSERT_THROW(serviceTypeInfo.getMessages(), CppRuntimeException);

    ASSERT_EQ(0, serviceTypeInfo.getMethods().size());

    ASSERT_THROW(serviceTypeInfo.createInstance(std::allocator<uint8_t>()), CppRuntimeException);
}

TEST_F(TypeInfoTest, recursiveTypeInfo)
{
    const ITypeInfo& typeInfo = RecursiveObject::typeInfo();
    const ITypeInfo& recursiveTypeInfo = typeInfo.getFields()[0].typeInfo;

    ASSERT_EQ(typeInfo.getSchemaName(), recursiveTypeInfo.getSchemaName());
    ASSERT_EQ(typeInfo.getSchemaType(), recursiveTypeInfo.getSchemaType());
    ASSERT_EQ(typeInfo.getCppType(), recursiveTypeInfo.getCppType());
    ASSERT_THROW(recursiveTypeInfo.getBitSize(), CppRuntimeException);

    ASSERT_EQ(typeInfo.getFields().data(), recursiveTypeInfo.getFields().data());
    ASSERT_EQ(0, recursiveTypeInfo.getParameters().size());
    ASSERT_EQ(0, recursiveTypeInfo.getFunctions().size());

    ASSERT_THROW(recursiveTypeInfo.getSelector(), CppRuntimeException);
    ASSERT_THROW(recursiveTypeInfo.getCases(), CppRuntimeException);

    ASSERT_THROW(recursiveTypeInfo.getUnderlyingType(), CppRuntimeException);
    ASSERT_THROW(recursiveTypeInfo.getEnumItems(), CppRuntimeException);
    ASSERT_THROW(recursiveTypeInfo.getBitmaskValues(), CppRuntimeException);

    ASSERT_THROW(recursiveTypeInfo.getColumns(), CppRuntimeException);
    ASSERT_THROW(recursiveTypeInfo.getSqlConstraint(), CppRuntimeException);
    ASSERT_THROW(recursiveTypeInfo.getVirtualTableUsing(), CppRuntimeException);
    ASSERT_THROW(recursiveTypeInfo.isWithoutRowId(), CppRuntimeException);

    ASSERT_THROW(recursiveTypeInfo.getTables(), CppRuntimeException);

    ASSERT_EQ(typeInfo.getTemplateName(), recursiveTypeInfo.getTemplateName());
    ASSERT_EQ(0, recursiveTypeInfo.getTemplateArguments().size());

    ASSERT_THROW(recursiveTypeInfo.getMessages(), CppRuntimeException);

    ASSERT_THROW(recursiveTypeInfo.getMethods(), CppRuntimeException);

    ASSERT_THROW(recursiveTypeInfo.createInstance(std::allocator<uint8_t>()), CppRuntimeException);
    ASSERT_THROW(recursiveTypeInfo.createInstance(), CppRuntimeException);
}

} // namespace zserio
