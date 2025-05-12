#ifndef ZSERIO_TYPE_INFO_INC_H
#define ZSERIO_TYPE_INFO_INC_H

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>

#include "zserio/BitBuffer.h"
#include "zserio/Bytes.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/ITypeInfo.h"
#include "zserio/String.h"

#include "Types.h"

namespace zserio
{

namespace detail
{

/**
 * Type information abstract base class.
 *
 * This base class implements fully the methods getSchemaName(), getSchemaName() and getCppType(). All other
 * interface methods just throw an exception.
 */
template <typename ALLOC>
class TypeInfoBase : public IBasicTypeInfo<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param schemaType The schema type to be stored in type information.
     * \param cppType The C++ type to be stored in type information.
     */
    TypeInfoBase(std::string_view schemaName, SchemaType schemaType, CppType cppType);

    /**
     * Copying and moving is disallowed!
     * \{
     */
    TypeInfoBase(const TypeInfoBase&) = delete;
    TypeInfoBase& operator=(const TypeInfoBase&) = delete;

    TypeInfoBase(const TypeInfoBase&&) = delete;
    TypeInfoBase& operator=(const TypeInfoBase&&) = delete;
    /**
     * \}
     */

    ~TypeInfoBase() override = 0;

    std::string_view getSchemaName() const override;
    SchemaType getSchemaType() const override;
    CppType getCppType() const override;
    uint8_t getBitSize() const override;

    Span<const BasicFieldInfo<ALLOC>> getFields() const override;
    Span<const BasicParameterInfo<ALLOC>> getParameters() const override;
    Span<const BasicFunctionInfo<ALLOC>> getFunctions() const override;

    std::string_view getSelector() const override;
    Span<const BasicCaseInfo<ALLOC>> getCases() const override;

    const IBasicTypeInfo<ALLOC>& getUnderlyingType() const override;
    Span<const ItemInfo> getEnumItems() const override;
    Span<const ItemInfo> getBitmaskValues() const override;

    Span<const BasicColumnInfo<ALLOC>> getColumns() const override;
    std::string_view getSqlConstraint() const override;
    std::string_view getVirtualTableUsing() const override;
    bool isWithoutRowId() const override;

    Span<const BasicTableInfo<ALLOC>> getTables() const override;

    std::string_view getTemplateName() const override;
    Span<const BasicTemplateArgumentInfo<ALLOC>> getTemplateArguments() const override;

    Span<const BasicMessageInfo<ALLOC>> getMessages() const override;
    Span<const BasicMethodInfo<ALLOC>> getMethods() const override;

    IBasicReflectableDataPtr<ALLOC> createInstance(const ALLOC& allocator) const override;
    IBasicReflectableDataPtr<ALLOC> createInstance() const override;

private:
    std::string_view m_schemaName;
    SchemaType m_schemaType;
    CppType m_cppType;
};

/**
 * Type information abstract base class for builtin types.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BuiltinTypeInfo : public TypeInfoBase<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param schemaType The schema type to be stored in type information.
     * \param cppType The C++ type to be stored in type information.
     */
    BuiltinTypeInfo(std::string_view schemaName, SchemaType schemaType, CppType cppType);

    template <typename T>
    static const BuiltinTypeInfo& getDynamicBitField();
};

/**
 * Type information abstract base class for fixed size builtin types.
 */
template <typename ALLOC>
class FixedSizeBuiltinTypeInfo : public BuiltinTypeInfo<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param schemaType The schema type to be stored in type information.
     * \param cppType The C++ type to be stored in type information.
     * \param bitSize The bit size of the fixed size integral schema type.
     */
    FixedSizeBuiltinTypeInfo(
            std::string_view schemaName, SchemaType schemaType, CppType cppType, uint8_t bitSize);

    uint8_t getBitSize() const override;

    template <BitSize BIT_SIZE, bool IS_SIGNED>
    static const FixedSizeBuiltinTypeInfo& getFixedBitField();

private:
    uint8_t m_bitSize;
};

/**
 * Type information abstract base class for templatable types.
 */
template <typename ALLOC>
class TemplatableTypeInfoBase : public TypeInfoBase<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param schemaType The schema type to be stored in type information.
     * \param cppType The C++ type to be stored in type information.
     * \param templateName The full schema template name.
     * \param templateArguments The sequence of type information for template arguments.
     */
    TemplatableTypeInfoBase(std::string_view schemaName, SchemaType schemaType, CppType cppType,
            std::string_view templateName, Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments);

    ~TemplatableTypeInfoBase() override = 0;

    TemplatableTypeInfoBase(const TemplatableTypeInfoBase&) = default;
    TemplatableTypeInfoBase& operator=(const TemplatableTypeInfoBase&) = default;

    TemplatableTypeInfoBase(TemplatableTypeInfoBase&&) = default;
    TemplatableTypeInfoBase& operator=(TemplatableTypeInfoBase&&) = default;

    std::string_view getTemplateName() const override;
    Span<const BasicTemplateArgumentInfo<ALLOC>> getTemplateArguments() const override;

private:
    std::string_view m_templateName;
    Span<const BasicTemplateArgumentInfo<ALLOC>> m_templateArguments;
};

/**
 * Type information abstract base class for compound types.
 */
template <typename ALLOC>
class CompoundTypeInfoBase : public TemplatableTypeInfoBase<ALLOC>
{
public:
    using TypeInfoBase<ALLOC>::getSchemaName;
    using CreateInstanceFunc = IBasicReflectableDataPtr<ALLOC> (*)(const ALLOC&);

    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param schemaType The schema type to be stored in type information.
     * \param cppType The C++ type to be stored in type information.
     * \param templateName The full schema template name.
     * \param templateArguments The sequence of type information for template arguments.
     * \param fields The sequence of type information for fields.
     * \param parameters The sequence of type information for parameters.
     * \param functions The sequence of type information for functions.
     */
    CompoundTypeInfoBase(std::string_view schemaName, CreateInstanceFunc createInstanceFunc,
            SchemaType schemaType, CppType cppType, std::string_view templateName,
            Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments,
            Span<const BasicFieldInfo<ALLOC>> fields, Span<const BasicParameterInfo<ALLOC>> parameters,
            Span<const BasicFunctionInfo<ALLOC>> functions);

    ~CompoundTypeInfoBase() override = 0;

    CompoundTypeInfoBase(const CompoundTypeInfoBase&) = default;
    CompoundTypeInfoBase& operator=(const CompoundTypeInfoBase&) = default;

    CompoundTypeInfoBase(CompoundTypeInfoBase&&) = default;
    CompoundTypeInfoBase& operator=(CompoundTypeInfoBase&&) = default;

    Span<const BasicFieldInfo<ALLOC>> getFields() const override;
    Span<const BasicParameterInfo<ALLOC>> getParameters() const override;
    Span<const BasicFunctionInfo<ALLOC>> getFunctions() const override;

    IBasicReflectableDataPtr<ALLOC> createInstance(const ALLOC& allocator) const override;

private:
    CreateInstanceFunc m_createInstanceFunc;
    Span<const BasicFieldInfo<ALLOC>> m_fields;
    Span<const BasicParameterInfo<ALLOC>> m_parameters;
    Span<const BasicFunctionInfo<ALLOC>> m_functions;
};

/**
 * Type information class for structure types.
 */
template <typename ALLOC>
class StructTypeInfo : public detail::CompoundTypeInfoBase<ALLOC>
{
public:
    using CreateInstanceFunc = typename CompoundTypeInfoBase<ALLOC>::CreateInstanceFunc;

    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param templateName The full schema template name.
     * \param templateArguments The sequence of type information for template arguments.
     * \param fields The sequence of type information for fields.
     * \param parameters The sequence of type information for parameters.
     * \param functions The sequence of type information for functions.
     */
    StructTypeInfo(std::string_view schemaName, CreateInstanceFunc createInstanceFunc,
            std::string_view templateName, Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments,
            Span<const BasicFieldInfo<ALLOC>> fields, Span<const BasicParameterInfo<ALLOC>> parameters,
            Span<const BasicFunctionInfo<ALLOC>> functions);
};

/**
 * Type information class for union types.
 */
template <typename ALLOC>
class UnionTypeInfo : public CompoundTypeInfoBase<ALLOC>
{
public:
    using CreateInstanceFunc = typename CompoundTypeInfoBase<ALLOC>::CreateInstanceFunc;

    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param templateName The full schema template name.
     * \param templateArguments The sequence of type information for template arguments.
     * \param fields The sequence of type information for fields.
     * \param parameters The sequence of type information for parameters.
     * \param functions The sequence of type information for functions.
     */
    UnionTypeInfo(std::string_view schemaName, CreateInstanceFunc createInstanceFunc,
            std::string_view templateName, Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments,
            Span<const BasicFieldInfo<ALLOC>> fields, Span<const BasicParameterInfo<ALLOC>> parameters,
            Span<const BasicFunctionInfo<ALLOC>> functions);
};

/**
 * Type information class for choice types.
 */
template <typename ALLOC>
class ChoiceTypeInfo : public CompoundTypeInfoBase<ALLOC>
{
public:
    using CreateInstanceFunc = typename CompoundTypeInfoBase<ALLOC>::CreateInstanceFunc;

    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param templateName The full schema template name.
     * \param templateArguments The sequence of type information for template arguments.
     * \param fields The sequence of type information for fields.
     * \param parameters The sequence of type information for parameters.
     * \param functions The sequence of type information for functions.
     * \param selector The selector expression.
     * \param cases The sequence of type information for cases.
     */
    ChoiceTypeInfo(std::string_view schemaName, CreateInstanceFunc createInstanceFunc,
            std::string_view templateName, Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments,
            Span<const BasicFieldInfo<ALLOC>> fields, Span<const BasicParameterInfo<ALLOC>> parameters,
            Span<const BasicFunctionInfo<ALLOC>> functions, std::string_view selector,
            Span<const BasicCaseInfo<ALLOC>> cases);

    std::string_view getSelector() const override;
    Span<const BasicCaseInfo<ALLOC>> getCases() const override;

private:
    std::string_view m_selector;
    Span<const BasicCaseInfo<ALLOC>> m_cases;
};

/**
 * Type information abstract base class for enumeration and bitmask types.
 */
template <typename ALLOC>
class TypeInfoWithUnderlyingTypeBase : public TypeInfoBase<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param schemaType The schema type to be stored in type information.
     * \param cppType The C++ type to be stored in type information.
     * \param underlyingType The reference to type information of underlying zserio type.
     */
    TypeInfoWithUnderlyingTypeBase(std::string_view schemaName, SchemaType schemaType, CppType cppType,
            const IBasicTypeInfo<ALLOC>& underlyingType);

    const IBasicTypeInfo<ALLOC>& getUnderlyingType() const override;

private:
    const IBasicTypeInfo<ALLOC>& m_underlyingType;
};

/**
 * Type information class for enumeration types.
 */
template <typename ALLOC>
class EnumTypeInfo : public TypeInfoWithUnderlyingTypeBase<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param underlyingType The reference to type information of underlying zserio type.
     * \param enumItems The sequence of type information for enumeration items.
     */
    EnumTypeInfo(std::string_view schemaName, const IBasicTypeInfo<ALLOC>& underlyingType,
            Span<const ItemInfo> enumItems);

    Span<const ItemInfo> getEnumItems() const override;

private:
    Span<const ItemInfo> m_enumItems;
};

/**
 * Type information class for bitmask types.
 */
template <typename ALLOC>
class BitmaskTypeInfo : public TypeInfoWithUnderlyingTypeBase<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param underlyingType The reference to type information of underlying zserio type.
     * \param bitmaskValues The sequence of type information for bitmask values.
     */
    BitmaskTypeInfo(std::string_view schemaName, const IBasicTypeInfo<ALLOC>& underlyingType,
            Span<const ItemInfo> bitmaskValues);

    Span<const ItemInfo> getBitmaskValues() const override;

private:
    Span<const ItemInfo> m_bitmaskValues;
};

/**
 * Type information class for SQL table types.
 */
template <typename ALLOC>
class SqlTableTypeInfo : public TemplatableTypeInfoBase<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param templateName The full schema template name.
     * \param templateArguments The sequence of type information for template arguments.
     * \param columns The sequence of type information for columns.
     * \param sqlConstraint The SQL table constraint.
     * \param virtualTableUsing The SQL virtual table using specification.
     * \param isWithoutRowId true if SQL table is without row id table, otherwise false.
     */
    SqlTableTypeInfo(std::string_view schemaName, std::string_view templateName,
            Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments,
            Span<const BasicColumnInfo<ALLOC>> columns, std::string_view sqlConstraint,
            std::string_view virtualTableUsing, bool isWithoutRowId);

    Span<const BasicColumnInfo<ALLOC>> getColumns() const override;
    std::string_view getSqlConstraint() const override;
    std::string_view getVirtualTableUsing() const override;
    bool isWithoutRowId() const override;

private:
    Span<const BasicColumnInfo<ALLOC>> m_columns;
    std::string_view m_sqlConstraint;
    std::string_view m_virtualTableUsing;
    bool m_isWithoutRowId;
};

/**
 * Type information class for SQL database types.
 */
template <typename ALLOC>
class SqlDatabaseTypeInfo : public TypeInfoBase<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param tables The sequence of type information for tables.
     */
    SqlDatabaseTypeInfo(std::string_view schemaName, Span<const BasicTableInfo<ALLOC>> tables);

    Span<const BasicTableInfo<ALLOC>> getTables() const override;

private:
    Span<const BasicTableInfo<ALLOC>> m_tables;
};

/**
 * Type information class for pubsub types.
 */
template <typename ALLOC>
class PubsubTypeInfo : public TypeInfoBase<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param messages The sequence of type information for pubsub messages.
     */
    PubsubTypeInfo(std::string_view schemaName, Span<const BasicMessageInfo<ALLOC>> messages);

    Span<const BasicMessageInfo<ALLOC>> getMessages() const override;

private:
    Span<const BasicMessageInfo<ALLOC>> m_messages;
};

/**
 * Type information class for service types.
 */
template <typename ALLOC>
class ServiceTypeInfo : public TypeInfoBase<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param schemaName The schema name to be stored in type information.
     * \param methods The sequence of type information for service methods.
     */
    ServiceTypeInfo(std::string_view schemaName, Span<const BasicMethodInfo<ALLOC>> methods);

    Span<const BasicMethodInfo<ALLOC>> getMethods() const override;

private:
    Span<const BasicMethodInfo<ALLOC>> m_methods;
};

/**
 * Type info for recursive types used as a wrapper around generated static typeInfo method to prevent
 * infinite recursion in type info definition.
 */
template <typename ALLOC>
class RecursiveTypeInfo : public IBasicTypeInfo<ALLOC>
{
public:
    /** Typedef to pointer to static typeInfo method on generated objects. */
    using TypeInfoFunc = const IBasicTypeInfo<ALLOC>& (*)();

    /**
     * Constructor.
     *
     * \param typeInfoFunc Pointer to static typeInfo method.
     */
    explicit RecursiveTypeInfo(TypeInfoFunc typeInfoFunc) :
            m_typeInfoFunc(typeInfoFunc)
    {}

    ~RecursiveTypeInfo() override = default;

    /**
     * Copying and moving is disallowed!
     * \{
     */
    RecursiveTypeInfo(const RecursiveTypeInfo&) = delete;
    RecursiveTypeInfo& operator=(const RecursiveTypeInfo&) = delete;

    RecursiveTypeInfo(const RecursiveTypeInfo&&) = delete;
    RecursiveTypeInfo& operator=(const RecursiveTypeInfo&&) = delete;
    /**
     * \}
     */

    std::string_view getSchemaName() const override;
    SchemaType getSchemaType() const override;
    CppType getCppType() const override;
    uint8_t getBitSize() const override;

    Span<const BasicFieldInfo<ALLOC>> getFields() const override;
    Span<const BasicParameterInfo<ALLOC>> getParameters() const override;
    Span<const BasicFunctionInfo<ALLOC>> getFunctions() const override;

    std::string_view getSelector() const override;
    Span<const BasicCaseInfo<ALLOC>> getCases() const override;

    const IBasicTypeInfo<ALLOC>& getUnderlyingType() const override;
    Span<const ItemInfo> getEnumItems() const override;
    Span<const ItemInfo> getBitmaskValues() const override;

    Span<const BasicColumnInfo<ALLOC>> getColumns() const override;
    std::string_view getSqlConstraint() const override;
    std::string_view getVirtualTableUsing() const override;
    bool isWithoutRowId() const override;

    Span<const BasicTableInfo<ALLOC>> getTables() const override;

    std::string_view getTemplateName() const override;
    Span<const BasicTemplateArgumentInfo<ALLOC>> getTemplateArguments() const override;

    Span<const BasicMessageInfo<ALLOC>> getMessages() const override;
    Span<const BasicMethodInfo<ALLOC>> getMethods() const override;

    IBasicReflectableDataPtr<ALLOC> createInstance(const ALLOC& allocator) const override;
    IBasicReflectableDataPtr<ALLOC> createInstance() const override;

private:
    TypeInfoFunc m_typeInfoFunc;
};

template <typename ALLOC>
TypeInfoBase<ALLOC>::TypeInfoBase(std::string_view schemaName, SchemaType schemaType, CppType cppType) :
        m_schemaName(schemaName),
        m_schemaType(schemaType),
        m_cppType(cppType)
{}

template <typename ALLOC>
TypeInfoBase<ALLOC>::~TypeInfoBase() = default;

template <typename ALLOC>
std::string_view TypeInfoBase<ALLOC>::getSchemaName() const
{
    return m_schemaName;
}

template <typename ALLOC>
SchemaType TypeInfoBase<ALLOC>::getSchemaType() const
{
    return m_schemaType;
}

template <typename ALLOC>
CppType TypeInfoBase<ALLOC>::getCppType() const
{
    return m_cppType;
}

template <typename ALLOC>
uint8_t TypeInfoBase<ALLOC>::getBitSize() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a fixed size type!";
}

template <typename ALLOC>
Span<const BasicFieldInfo<ALLOC>> TypeInfoBase<ALLOC>::getFields() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a compound type!";
}

template <typename ALLOC>
Span<const BasicParameterInfo<ALLOC>> TypeInfoBase<ALLOC>::getParameters() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a compound type!";
}

template <typename ALLOC>
Span<const BasicFunctionInfo<ALLOC>> TypeInfoBase<ALLOC>::getFunctions() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a compound type!";
}

template <typename ALLOC>
std::string_view TypeInfoBase<ALLOC>::getSelector() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a choice type!";
}

template <typename ALLOC>
Span<const BasicCaseInfo<ALLOC>> TypeInfoBase<ALLOC>::getCases() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a choice type!";
}

template <typename ALLOC>
const IBasicTypeInfo<ALLOC>& TypeInfoBase<ALLOC>::getUnderlyingType() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' does not have underlying type!";
}

template <typename ALLOC>
Span<const ItemInfo> TypeInfoBase<ALLOC>::getEnumItems() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not an enum type!";
}

template <typename ALLOC>
Span<const ItemInfo> TypeInfoBase<ALLOC>::getBitmaskValues() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a bitmask type!";
}

template <typename ALLOC>
Span<const BasicColumnInfo<ALLOC>> TypeInfoBase<ALLOC>::getColumns() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a SQL table type!";
}

template <typename ALLOC>
std::string_view TypeInfoBase<ALLOC>::getSqlConstraint() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a SQL table type!";
}

template <typename ALLOC>
std::string_view TypeInfoBase<ALLOC>::getVirtualTableUsing() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a SQL table type!";
}

template <typename ALLOC>
bool TypeInfoBase<ALLOC>::isWithoutRowId() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a SQL table type!";
}

template <typename ALLOC>
Span<const BasicTableInfo<ALLOC>> TypeInfoBase<ALLOC>::getTables() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a SQL database type!";
}

template <typename ALLOC>
std::string_view TypeInfoBase<ALLOC>::getTemplateName() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a templatable type!";
}

template <typename ALLOC>
Span<const BasicTemplateArgumentInfo<ALLOC>> TypeInfoBase<ALLOC>::getTemplateArguments() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a templatable type!";
}

template <typename ALLOC>
Span<const BasicMessageInfo<ALLOC>> TypeInfoBase<ALLOC>::getMessages() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a pubsub type!";
}

template <typename ALLOC>
Span<const BasicMethodInfo<ALLOC>> TypeInfoBase<ALLOC>::getMethods() const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a service type!";
}

template <typename ALLOC>
IBasicReflectableDataPtr<ALLOC> TypeInfoBase<ALLOC>::createInstance(const ALLOC&) const
{
    throw CppRuntimeException("Type '") << getSchemaName() << "' is not a compound type!";
}

template <typename ALLOC>
IBasicReflectableDataPtr<ALLOC> TypeInfoBase<ALLOC>::createInstance() const
{
    return createInstance(ALLOC());
}

template <typename ALLOC>
BuiltinTypeInfo<ALLOC>::BuiltinTypeInfo(std::string_view schemaName, SchemaType schemaType, CppType cppType) :
        TypeInfoBase<ALLOC>(schemaName, schemaType, cppType)
{}

template <typename ALLOC>
template <typename T>
const BuiltinTypeInfo<ALLOC>& BuiltinTypeInfo<ALLOC>::getDynamicBitField()
{
    static constexpr BitSize maxBitSize = sizeof(T) * 8;

    static_assert(maxBitSize <= 64, "Max bit size out of range!");

    if constexpr (std::is_signed_v<T>)
    {
        if (maxBitSize <= 8)
        {
            static const BuiltinTypeInfo<ALLOC> typeInfo = {
                    "int<>", SchemaType::DYNAMIC_SIGNED_BITFIELD, CppType::INT8};
            return typeInfo;
        }
        else if (maxBitSize <= 16)
        {
            static const BuiltinTypeInfo<ALLOC> typeInfo = {
                    "int<>", SchemaType::DYNAMIC_SIGNED_BITFIELD, CppType::INT16};
            return typeInfo;
        }
        else if (maxBitSize <= 32)
        {
            static const BuiltinTypeInfo<ALLOC> typeInfo = {
                    "int<>", SchemaType::DYNAMIC_SIGNED_BITFIELD, CppType::INT32};
            return typeInfo;
        }
        else
        {
            static const BuiltinTypeInfo<ALLOC> typeInfo = {
                    "int<>", SchemaType::DYNAMIC_SIGNED_BITFIELD, CppType::INT64};
            return typeInfo;
        }
    }
    else
    {
        if (maxBitSize <= 8)
        {
            static const BuiltinTypeInfo<ALLOC> typeInfo = {
                    "bit<>", SchemaType::DYNAMIC_UNSIGNED_BITFIELD, CppType::UINT8};
            return typeInfo;
        }
        else if (maxBitSize <= 16)
        {
            static const BuiltinTypeInfo<ALLOC> typeInfo = {
                    "bit<>", SchemaType::DYNAMIC_UNSIGNED_BITFIELD, CppType::UINT16};
            return typeInfo;
        }
        else if (maxBitSize <= 32)
        {
            static const BuiltinTypeInfo<ALLOC> typeInfo = {
                    "bit<>", SchemaType::DYNAMIC_UNSIGNED_BITFIELD, CppType::UINT32};
            return typeInfo;
        }
        else
        {
            static const BuiltinTypeInfo<ALLOC> typeInfo = {
                    "bit<>", SchemaType::DYNAMIC_UNSIGNED_BITFIELD, CppType::UINT64};
            return typeInfo;
        }
    }
}

template <typename ALLOC>
FixedSizeBuiltinTypeInfo<ALLOC>::FixedSizeBuiltinTypeInfo(
        std::string_view schemaName, SchemaType schemaType, CppType cppType, uint8_t bitSize) :
        BuiltinTypeInfo<ALLOC>(schemaName, schemaType, cppType),
        m_bitSize(bitSize)
{}

template <typename ALLOC>
uint8_t FixedSizeBuiltinTypeInfo<ALLOC>::getBitSize() const
{
    return m_bitSize;
}

template <typename ALLOC>
template <BitSize BIT_SIZE, bool IS_SIGNED>
const FixedSizeBuiltinTypeInfo<ALLOC>& FixedSizeBuiltinTypeInfo<ALLOC>::getFixedBitField()
{
    static_assert(BIT_SIZE > 0 && BIT_SIZE <= 64, "Bit size out of range!");

    if constexpr (IS_SIGNED)
    {
        static const std::array<FixedSizeBuiltinTypeInfo<ALLOC>, 64> bitFieldTypeInfoArray = {
                {{"int:1", SchemaType::INT1, CppType::INT8, 1}, //
                        {"int:2", SchemaType::INT2, CppType::INT8, 2},
                        {"int:3", SchemaType::INT3, CppType::INT8, 3},
                        {"int:4", SchemaType::INT4, CppType::INT8, 4},
                        {"int:5", SchemaType::INT5, CppType::INT8, 5},
                        {"int:6", SchemaType::INT6, CppType::INT8, 6},
                        {"int:7", SchemaType::INT7, CppType::INT8, 7},
                        {"int:8", SchemaType::INT8, CppType::INT8, 8},
                        {"int:9", SchemaType::INT9, CppType::INT16, 9},
                        {"int:10", SchemaType::INT10, CppType::INT16, 10},
                        {"int:11", SchemaType::INT11, CppType::INT16, 11},
                        {"int:12", SchemaType::INT12, CppType::INT16, 12},
                        {"int:13", SchemaType::INT13, CppType::INT16, 13},
                        {"int:14", SchemaType::INT14, CppType::INT16, 14},
                        {"int:15", SchemaType::INT15, CppType::INT16, 15},
                        {"int:16", SchemaType::INT16, CppType::INT16, 16},
                        {"int:17", SchemaType::INT17, CppType::INT32, 17},
                        {"int:18", SchemaType::INT18, CppType::INT32, 18},
                        {"int:19", SchemaType::INT19, CppType::INT32, 19},
                        {"int:20", SchemaType::INT20, CppType::INT32, 20},
                        {"int:21", SchemaType::INT21, CppType::INT32, 21},
                        {"int:22", SchemaType::INT22, CppType::INT32, 22},
                        {"int:23", SchemaType::INT23, CppType::INT32, 23},
                        {"int:24", SchemaType::INT24, CppType::INT32, 24},
                        {"int:25", SchemaType::INT25, CppType::INT32, 25},
                        {"int:26", SchemaType::INT26, CppType::INT32, 26},
                        {"int:27", SchemaType::INT27, CppType::INT32, 27},
                        {"int:28", SchemaType::INT28, CppType::INT32, 28},
                        {"int:29", SchemaType::INT29, CppType::INT32, 29},
                        {"int:30", SchemaType::INT30, CppType::INT32, 30},
                        {"int:31", SchemaType::INT31, CppType::INT32, 31},
                        {"int:32", SchemaType::INT32, CppType::INT32, 32},
                        {"int:33", SchemaType::INT33, CppType::INT64, 33},
                        {"int:34", SchemaType::INT34, CppType::INT64, 34},
                        {"int:35", SchemaType::INT35, CppType::INT64, 35},
                        {"int:36", SchemaType::INT36, CppType::INT64, 36},
                        {"int:37", SchemaType::INT37, CppType::INT64, 37},
                        {"int:38", SchemaType::INT38, CppType::INT64, 38},
                        {"int:39", SchemaType::INT39, CppType::INT64, 39},
                        {"int:40", SchemaType::INT40, CppType::INT64, 40},
                        {"int:41", SchemaType::INT41, CppType::INT64, 41},
                        {"int:42", SchemaType::INT42, CppType::INT64, 42},
                        {"int:43", SchemaType::INT43, CppType::INT64, 43},
                        {"int:44", SchemaType::INT44, CppType::INT64, 44},
                        {"int:45", SchemaType::INT45, CppType::INT64, 45},
                        {"int:46", SchemaType::INT46, CppType::INT64, 46},
                        {"int:47", SchemaType::INT47, CppType::INT64, 47},
                        {"int:48", SchemaType::INT48, CppType::INT64, 48},
                        {"int:49", SchemaType::INT49, CppType::INT64, 49},
                        {"int:50", SchemaType::INT50, CppType::INT64, 50},
                        {"int:51", SchemaType::INT51, CppType::INT64, 51},
                        {"int:52", SchemaType::INT52, CppType::INT64, 52},
                        {"int:53", SchemaType::INT53, CppType::INT64, 53},
                        {"int:54", SchemaType::INT54, CppType::INT64, 54},
                        {"int:55", SchemaType::INT55, CppType::INT64, 55},
                        {"int:56", SchemaType::INT56, CppType::INT64, 56},
                        {"int:57", SchemaType::INT57, CppType::INT64, 57},
                        {"int:58", SchemaType::INT58, CppType::INT64, 58},
                        {"int:59", SchemaType::INT59, CppType::INT64, 59},
                        {"int:60", SchemaType::INT60, CppType::INT64, 60},
                        {"int:61", SchemaType::INT61, CppType::INT64, 61},
                        {"int:62", SchemaType::INT62, CppType::INT64, 62},
                        {"int:63", SchemaType::INT63, CppType::INT64, 63},
                        {"int:64", SchemaType::INT64, CppType::INT64, 64}}};

        return bitFieldTypeInfoArray[BIT_SIZE - 1U];
    }
    else
    {
        static const std::array<FixedSizeBuiltinTypeInfo<ALLOC>, 64> bitFieldTypeInfoArray = {
                {{"bit:1", SchemaType::UINT1, CppType::UINT8, 1},
                        {"bit:2", SchemaType::UINT2, CppType::UINT8, 2},
                        {"bit:3", SchemaType::UINT3, CppType::UINT8, 3},
                        {"bit:4", SchemaType::UINT4, CppType::UINT8, 4},
                        {"bit:5", SchemaType::UINT5, CppType::UINT8, 5},
                        {"bit:6", SchemaType::UINT6, CppType::UINT8, 6},
                        {"bit:7", SchemaType::UINT7, CppType::UINT8, 7},
                        {"bit:8", SchemaType::UINT8, CppType::UINT8, 8},
                        {"bit:9", SchemaType::UINT9, CppType::UINT16, 9},
                        {"bit:10", SchemaType::UINT10, CppType::UINT16, 10},
                        {"bit:11", SchemaType::UINT11, CppType::UINT16, 11},
                        {"bit:12", SchemaType::UINT12, CppType::UINT16, 12},
                        {"bit:13", SchemaType::UINT13, CppType::UINT16, 13},
                        {"bit:14", SchemaType::UINT14, CppType::UINT16, 14},
                        {"bit:15", SchemaType::UINT15, CppType::UINT16, 15},
                        {"bit:16", SchemaType::UINT16, CppType::UINT16, 16},
                        {"bit:17", SchemaType::UINT17, CppType::UINT32, 17},
                        {"bit:18", SchemaType::UINT18, CppType::UINT32, 18},
                        {"bit:19", SchemaType::UINT19, CppType::UINT32, 19},
                        {"bit:20", SchemaType::UINT20, CppType::UINT32, 20},
                        {"bit:21", SchemaType::UINT21, CppType::UINT32, 21},
                        {"bit:22", SchemaType::UINT22, CppType::UINT32, 22},
                        {"bit:23", SchemaType::UINT23, CppType::UINT32, 23},
                        {"bit:24", SchemaType::UINT24, CppType::UINT32, 24},
                        {"bit:25", SchemaType::UINT25, CppType::UINT32, 25},
                        {"bit:26", SchemaType::UINT26, CppType::UINT32, 26},
                        {"bit:27", SchemaType::UINT27, CppType::UINT32, 27},
                        {"bit:28", SchemaType::UINT28, CppType::UINT32, 28},
                        {"bit:29", SchemaType::UINT29, CppType::UINT32, 29},
                        {"bit:30", SchemaType::UINT30, CppType::UINT32, 30},
                        {"bit:31", SchemaType::UINT31, CppType::UINT32, 31},
                        {"bit:32", SchemaType::UINT32, CppType::UINT32, 32},
                        {"bit:33", SchemaType::UINT33, CppType::UINT64, 33},
                        {"bit:34", SchemaType::UINT34, CppType::UINT64, 34},
                        {"bit:35", SchemaType::UINT35, CppType::UINT64, 35},
                        {"bit:36", SchemaType::UINT36, CppType::UINT64, 36},
                        {"bit:37", SchemaType::UINT37, CppType::UINT64, 37},
                        {"bit:38", SchemaType::UINT38, CppType::UINT64, 38},
                        {"bit:39", SchemaType::UINT39, CppType::UINT64, 39},
                        {"bit:40", SchemaType::UINT40, CppType::UINT64, 40},
                        {"bit:41", SchemaType::UINT41, CppType::UINT64, 41},
                        {"bit:42", SchemaType::UINT42, CppType::UINT64, 42},
                        {"bit:43", SchemaType::UINT43, CppType::UINT64, 43},
                        {"bit:44", SchemaType::UINT44, CppType::UINT64, 44},
                        {"bit:45", SchemaType::UINT45, CppType::UINT64, 45},
                        {"bit:46", SchemaType::UINT46, CppType::UINT64, 46},
                        {"bit:47", SchemaType::UINT47, CppType::UINT64, 47},
                        {"bit:48", SchemaType::UINT48, CppType::UINT64, 48},
                        {"bit:49", SchemaType::UINT49, CppType::UINT64, 49},
                        {"bit:50", SchemaType::UINT50, CppType::UINT64, 50},
                        {"bit:51", SchemaType::UINT51, CppType::UINT64, 51},
                        {"bit:52", SchemaType::UINT52, CppType::UINT64, 52},
                        {"bit:53", SchemaType::UINT53, CppType::UINT64, 53},
                        {"bit:54", SchemaType::UINT54, CppType::UINT64, 54},
                        {"bit:55", SchemaType::UINT55, CppType::UINT64, 55},
                        {"bit:56", SchemaType::UINT56, CppType::UINT64, 56},
                        {"bit:57", SchemaType::UINT57, CppType::UINT64, 57},
                        {"bit:58", SchemaType::UINT58, CppType::UINT64, 58},
                        {"bit:59", SchemaType::UINT59, CppType::UINT64, 59},
                        {"bit:60", SchemaType::UINT60, CppType::UINT64, 60},
                        {"bit:61", SchemaType::UINT61, CppType::UINT64, 61},
                        {"bit:62", SchemaType::UINT62, CppType::UINT64, 62},
                        {"bit:63", SchemaType::UINT63, CppType::UINT64, 63},
                        {"bit:64", SchemaType::UINT64, CppType::UINT64, 64}}};

        return bitFieldTypeInfoArray[static_cast<size_t>(BIT_SIZE - 1)];
    }
}

template <typename ALLOC>
TemplatableTypeInfoBase<ALLOC>::TemplatableTypeInfoBase(std::string_view schemaName, SchemaType schemaType,
        CppType cppType, std::string_view templateName,
        Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments) :
        TypeInfoBase<ALLOC>(schemaName, schemaType, cppType),
        m_templateName(templateName),
        m_templateArguments(templateArguments)
{}

template <typename ALLOC>
TemplatableTypeInfoBase<ALLOC>::~TemplatableTypeInfoBase() = default;

template <typename ALLOC>
std::string_view TemplatableTypeInfoBase<ALLOC>::getTemplateName() const
{
    return m_templateName;
}

template <typename ALLOC>
Span<const BasicTemplateArgumentInfo<ALLOC>> TemplatableTypeInfoBase<ALLOC>::getTemplateArguments() const
{
    return m_templateArguments;
}

template <typename ALLOC>
CompoundTypeInfoBase<ALLOC>::CompoundTypeInfoBase(std::string_view schemaName,
        CreateInstanceFunc createInstanceFunc, SchemaType schemaType, CppType cppType,
        std::string_view templateName, Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments,
        Span<const BasicFieldInfo<ALLOC>> fields, Span<const BasicParameterInfo<ALLOC>> parameters,
        Span<const BasicFunctionInfo<ALLOC>> functions) :
        TemplatableTypeInfoBase<ALLOC>(schemaName, schemaType, cppType, templateName, templateArguments),
        m_createInstanceFunc(createInstanceFunc),
        m_fields(fields),
        m_parameters(parameters),
        m_functions(functions)
{}

template <typename ALLOC>
CompoundTypeInfoBase<ALLOC>::~CompoundTypeInfoBase() = default;

template <typename ALLOC>
Span<const BasicFieldInfo<ALLOC>> CompoundTypeInfoBase<ALLOC>::getFields() const
{
    return m_fields;
}

template <typename ALLOC>
Span<const BasicParameterInfo<ALLOC>> CompoundTypeInfoBase<ALLOC>::getParameters() const
{
    return m_parameters;
}

template <typename ALLOC>
Span<const BasicFunctionInfo<ALLOC>> CompoundTypeInfoBase<ALLOC>::getFunctions() const
{
    return m_functions;
}

template <typename ALLOC>
IBasicReflectableDataPtr<ALLOC> CompoundTypeInfoBase<ALLOC>::createInstance(const ALLOC& allocator) const
{
    if (!m_createInstanceFunc)
    {
        throw CppRuntimeException("Reflectable '")
                << getSchemaName() << "': Cannot create instance, not implemented!";
    }
    return m_createInstanceFunc(allocator);
}

template <typename ALLOC>
StructTypeInfo<ALLOC>::StructTypeInfo(std::string_view schemaName, CreateInstanceFunc createInstanceFunc,
        std::string_view templateName, Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments,
        Span<const BasicFieldInfo<ALLOC>> fields, Span<const BasicParameterInfo<ALLOC>> parameters,
        Span<const BasicFunctionInfo<ALLOC>> functions) :
        CompoundTypeInfoBase<ALLOC>(schemaName, createInstanceFunc, SchemaType::STRUCT, CppType::STRUCT,
                templateName, templateArguments, fields, parameters, functions)
{}

template <typename ALLOC>
UnionTypeInfo<ALLOC>::UnionTypeInfo(std::string_view schemaName, CreateInstanceFunc createInstanceFunc,
        std::string_view templateName, Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments,
        Span<const BasicFieldInfo<ALLOC>> fields, Span<const BasicParameterInfo<ALLOC>> parameters,
        Span<const BasicFunctionInfo<ALLOC>> functions) :
        CompoundTypeInfoBase<ALLOC>(schemaName, createInstanceFunc, SchemaType::UNION, CppType::UNION,
                templateName, templateArguments, fields, parameters, functions)
{}

template <typename ALLOC>
ChoiceTypeInfo<ALLOC>::ChoiceTypeInfo(std::string_view schemaName, CreateInstanceFunc createInstanceFunc,
        std::string_view templateName, Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments,
        Span<const BasicFieldInfo<ALLOC>> fields, Span<const BasicParameterInfo<ALLOC>> parameters,
        Span<const BasicFunctionInfo<ALLOC>> functions, std::string_view selector,
        Span<const BasicCaseInfo<ALLOC>> cases) :
        CompoundTypeInfoBase<ALLOC>(schemaName, createInstanceFunc, SchemaType::CHOICE, CppType::CHOICE,
                templateName, templateArguments, fields, parameters, functions),
        m_selector(selector),
        m_cases(cases)
{}

template <typename ALLOC>
std::string_view ChoiceTypeInfo<ALLOC>::getSelector() const
{
    return m_selector;
}

template <typename ALLOC>
Span<const BasicCaseInfo<ALLOC>> ChoiceTypeInfo<ALLOC>::getCases() const
{
    return m_cases;
}

template <typename ALLOC>
SqlTableTypeInfo<ALLOC>::SqlTableTypeInfo(std::string_view schemaName, std::string_view templateName,
        Span<const BasicTemplateArgumentInfo<ALLOC>> templateArguments,
        Span<const BasicColumnInfo<ALLOC>> columns, std::string_view sqlConstraint,
        std::string_view virtualTableUsing, bool isWithoutRowId) :
        TemplatableTypeInfoBase<ALLOC>(
                schemaName, SchemaType::SQL_TABLE, CppType::SQL_TABLE, templateName, templateArguments),
        m_columns(columns),
        m_sqlConstraint(sqlConstraint),
        m_virtualTableUsing(virtualTableUsing),
        m_isWithoutRowId(isWithoutRowId)
{}

template <typename ALLOC>
Span<const BasicColumnInfo<ALLOC>> SqlTableTypeInfo<ALLOC>::getColumns() const
{
    return m_columns;
}

template <typename ALLOC>
std::string_view SqlTableTypeInfo<ALLOC>::getSqlConstraint() const
{
    return m_sqlConstraint;
}

template <typename ALLOC>
std::string_view SqlTableTypeInfo<ALLOC>::getVirtualTableUsing() const
{
    return m_virtualTableUsing;
}

template <typename ALLOC>
bool SqlTableTypeInfo<ALLOC>::isWithoutRowId() const
{
    return m_isWithoutRowId;
}

template <typename ALLOC>
SqlDatabaseTypeInfo<ALLOC>::SqlDatabaseTypeInfo(
        std::string_view schemaName, Span<const BasicTableInfo<ALLOC>> tables) :
        TypeInfoBase<ALLOC>(schemaName, SchemaType::SQL_DATABASE, CppType::SQL_DATABASE),
        m_tables(tables)
{}

template <typename ALLOC>
Span<const BasicTableInfo<ALLOC>> SqlDatabaseTypeInfo<ALLOC>::getTables() const
{
    return m_tables;
}

template <typename ALLOC>
TypeInfoWithUnderlyingTypeBase<ALLOC>::TypeInfoWithUnderlyingTypeBase(std::string_view schemaName,
        SchemaType schemaType, CppType cppType, const IBasicTypeInfo<ALLOC>& underlyingType) :
        TypeInfoBase<ALLOC>(schemaName, schemaType, cppType),
        m_underlyingType(underlyingType)
{}

template <typename ALLOC>
const IBasicTypeInfo<ALLOC>& TypeInfoWithUnderlyingTypeBase<ALLOC>::getUnderlyingType() const
{
    return m_underlyingType;
}

template <typename ALLOC>
EnumTypeInfo<ALLOC>::EnumTypeInfo(std::string_view schemaName, const IBasicTypeInfo<ALLOC>& underlyingType,
        Span<const ItemInfo> enumItems) :
        TypeInfoWithUnderlyingTypeBase<ALLOC>(schemaName, SchemaType::ENUM, CppType::ENUM, underlyingType),
        m_enumItems(enumItems)
{}

template <typename ALLOC>
Span<const ItemInfo> EnumTypeInfo<ALLOC>::getEnumItems() const
{
    return m_enumItems;
}

template <typename ALLOC>
BitmaskTypeInfo<ALLOC>::BitmaskTypeInfo(std::string_view schemaName,
        const IBasicTypeInfo<ALLOC>& underlyingType, Span<const ItemInfo> bitmaskValues) :
        TypeInfoWithUnderlyingTypeBase<ALLOC>(
                schemaName, SchemaType::BITMASK, CppType::BITMASK, underlyingType),
        m_bitmaskValues(bitmaskValues)
{}

template <typename ALLOC>
Span<const ItemInfo> BitmaskTypeInfo<ALLOC>::getBitmaskValues() const
{
    return m_bitmaskValues;
}

template <typename ALLOC>
PubsubTypeInfo<ALLOC>::PubsubTypeInfo(
        std::string_view schemaName, Span<const BasicMessageInfo<ALLOC>> messages) :
        TypeInfoBase<ALLOC>(schemaName, SchemaType::PUBSUB, CppType::PUBSUB),
        m_messages(messages)
{}

template <typename ALLOC>
Span<const BasicMessageInfo<ALLOC>> PubsubTypeInfo<ALLOC>::getMessages() const
{
    return m_messages;
}

template <typename ALLOC>
ServiceTypeInfo<ALLOC>::ServiceTypeInfo(
        std::string_view schemaName, Span<const BasicMethodInfo<ALLOC>> methods) :
        TypeInfoBase<ALLOC>(schemaName, SchemaType::SERVICE, CppType::SERVICE),
        m_methods(methods)
{}

template <typename ALLOC>
Span<const BasicMethodInfo<ALLOC>> ServiceTypeInfo<ALLOC>::getMethods() const
{
    return m_methods;
}

template <typename ALLOC>
std::string_view RecursiveTypeInfo<ALLOC>::getSchemaName() const
{
    return m_typeInfoFunc().getSchemaName();
}

template <typename ALLOC>
SchemaType RecursiveTypeInfo<ALLOC>::getSchemaType() const
{
    return m_typeInfoFunc().getSchemaType();
}

template <typename ALLOC>
CppType RecursiveTypeInfo<ALLOC>::getCppType() const
{
    return m_typeInfoFunc().getCppType();
}

template <typename ALLOC>
uint8_t RecursiveTypeInfo<ALLOC>::getBitSize() const
{
    return m_typeInfoFunc().getBitSize();
}

template <typename ALLOC>
Span<const BasicFieldInfo<ALLOC>> RecursiveTypeInfo<ALLOC>::getFields() const
{
    return m_typeInfoFunc().getFields();
}

template <typename ALLOC>
Span<const BasicParameterInfo<ALLOC>> RecursiveTypeInfo<ALLOC>::getParameters() const
{
    return m_typeInfoFunc().getParameters();
}

template <typename ALLOC>
Span<const BasicFunctionInfo<ALLOC>> RecursiveTypeInfo<ALLOC>::getFunctions() const
{
    return m_typeInfoFunc().getFunctions();
}

template <typename ALLOC>
std::string_view RecursiveTypeInfo<ALLOC>::getSelector() const
{
    return m_typeInfoFunc().getSelector();
}

template <typename ALLOC>
Span<const BasicCaseInfo<ALLOC>> RecursiveTypeInfo<ALLOC>::getCases() const
{
    return m_typeInfoFunc().getCases();
}

template <typename ALLOC>
const IBasicTypeInfo<ALLOC>& RecursiveTypeInfo<ALLOC>::getUnderlyingType() const
{
    return m_typeInfoFunc().getUnderlyingType();
}

template <typename ALLOC>
Span<const ItemInfo> RecursiveTypeInfo<ALLOC>::getEnumItems() const
{
    return m_typeInfoFunc().getEnumItems();
}

template <typename ALLOC>
Span<const ItemInfo> RecursiveTypeInfo<ALLOC>::getBitmaskValues() const
{
    return m_typeInfoFunc().getBitmaskValues();
}

template <typename ALLOC>
Span<const BasicColumnInfo<ALLOC>> RecursiveTypeInfo<ALLOC>::getColumns() const
{
    return m_typeInfoFunc().getColumns();
}

template <typename ALLOC>
std::string_view RecursiveTypeInfo<ALLOC>::getSqlConstraint() const
{
    return m_typeInfoFunc().getSqlConstraint();
}

template <typename ALLOC>
std::string_view RecursiveTypeInfo<ALLOC>::getVirtualTableUsing() const
{
    return m_typeInfoFunc().getVirtualTableUsing();
}

template <typename ALLOC>
bool RecursiveTypeInfo<ALLOC>::isWithoutRowId() const
{
    return m_typeInfoFunc().isWithoutRowId();
}

template <typename ALLOC>
Span<const BasicTableInfo<ALLOC>> RecursiveTypeInfo<ALLOC>::getTables() const
{
    return m_typeInfoFunc().getTables();
}

template <typename ALLOC>
std::string_view RecursiveTypeInfo<ALLOC>::getTemplateName() const
{
    return m_typeInfoFunc().getTemplateName();
}

template <typename ALLOC>
Span<const BasicTemplateArgumentInfo<ALLOC>> RecursiveTypeInfo<ALLOC>::getTemplateArguments() const
{
    return m_typeInfoFunc().getTemplateArguments();
}

template <typename ALLOC>
Span<const BasicMessageInfo<ALLOC>> RecursiveTypeInfo<ALLOC>::getMessages() const
{
    return m_typeInfoFunc().getMessages();
}

template <typename ALLOC>
Span<const BasicMethodInfo<ALLOC>> RecursiveTypeInfo<ALLOC>::getMethods() const
{
    return m_typeInfoFunc().getMethods();
}

template <typename ALLOC>
IBasicReflectableDataPtr<ALLOC> RecursiveTypeInfo<ALLOC>::createInstance(const ALLOC& allocator) const
{
    return m_typeInfoFunc().createInstance(allocator);
}

template <typename ALLOC>
IBasicReflectableDataPtr<ALLOC> RecursiveTypeInfo<ALLOC>::createInstance() const
{
    return createInstance(ALLOC());
}

template <typename ALLOC>
struct TypeInfo<Bool, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const FixedSizeBuiltinTypeInfo<ALLOC> typeInfo = {"bool", SchemaType::BOOL, CppType::BOOL, 1};
        return typeInfo;
    }
};

template <BitSize BIT_SIZE, bool IS_SIGNED, typename ALLOC>
struct TypeInfo<detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED>, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        return FixedSizeBuiltinTypeInfo<ALLOC>::template getFixedBitField<BIT_SIZE, IS_SIGNED>();
    }
};

template <typename T, typename ALLOC>
struct TypeInfo<detail::DynIntWrapper<T>, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        return BuiltinTypeInfo<ALLOC>::template getDynamicBitField<T>();
    }
};

template <typename ALLOC>
struct TypeInfo<VarInt16, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"varint16", SchemaType::VARINT16, CppType::INT16};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<VarInt32, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"varint32", SchemaType::VARINT32, CppType::INT32};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<VarInt64, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"varint64", SchemaType::VARINT64, CppType::INT64};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<VarInt, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"varint", SchemaType::VARINT, CppType::INT64};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<VarUInt16, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"varuint16", SchemaType::VARUINT16, CppType::UINT16};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<VarUInt32, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"varuint32", SchemaType::VARUINT32, CppType::UINT32};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<VarUInt64, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"varuint64", SchemaType::VARUINT64, CppType::UINT64};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<VarUInt, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"varuint", SchemaType::VARUINT, CppType::UINT64};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<VarSize, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"varsize", SchemaType::VARSIZE, CppType::UINT32};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<Float16, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const FixedSizeBuiltinTypeInfo<ALLOC> typeInfo = {
                "float16", SchemaType::FLOAT16, CppType::FLOAT, 16};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<Float32, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const FixedSizeBuiltinTypeInfo<ALLOC> typeInfo = {
                "float32", SchemaType::FLOAT32, CppType::FLOAT, 32};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<Float64, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const FixedSizeBuiltinTypeInfo<ALLOC> typeInfo = {
                "float64", SchemaType::FLOAT64, CppType::DOUBLE, 64};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<BasicBytes<ALLOC>, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"bytes", SchemaType::BYTES, CppType::BYTES};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<BasicString<ALLOC>, RebindAlloc<ALLOC, uint8_t>>
{
    static const IBasicTypeInfo<RebindAlloc<ALLOC, uint8_t>>& get()
    {
        static const BuiltinTypeInfo<RebindAlloc<ALLOC, uint8_t>> typeInfo = {
                "string", SchemaType::STRING, CppType::STRING};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<std::string_view, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"string", SchemaType::STRING, CppType::STRING};
        return typeInfo;
    }
};

template <typename ALLOC>
struct TypeInfo<BasicBitBuffer<ALLOC>, ALLOC>
{
    static const IBasicTypeInfo<ALLOC>& get()
    {
        static const BuiltinTypeInfo<ALLOC> typeInfo = {"extern", SchemaType::EXTERN, CppType::BIT_BUFFER};
        return typeInfo;
    }
};

} // namespace detail

} // namespace zserio

#endif // ZSERIO_TYPE_INFO_INC_H
