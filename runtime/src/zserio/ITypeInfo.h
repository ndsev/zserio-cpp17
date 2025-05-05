#ifndef ZSERIO_I_TYPE_INFO_INC_H
#define ZSERIO_I_TYPE_INFO_INC_H

#include <string_view>

#include "zserio/IReflectableData.h"
#include "zserio/Span.h"
#include "zserio/Traits.h"

namespace zserio
{

/** Enumeration which specifies C++ type used in type information. */
enum class CppType
{
    BOOL, /**< C++ bool type */
    INT8, /**< C++ int8_t type */
    INT16, /**< C++ int16_t type */
    INT32, /**< C++ int32_t type */
    INT64, /**< C++ int64_t type */
    UINT8, /**< C++ uint8_t type */
    UINT16, /**< C++ uint16_t type */
    UINT32, /**< C++ uint32_t type */
    UINT64, /**< C++ uint64_t type */
    FLOAT, /**< C++ float type */
    DOUBLE, /**< C++ double type */
    BYTES, /**< C++ bytes type (mapped as std::vector<uint8_t>) */
    STRING, /**< C++ string type */
    BIT_BUFFER, /**< C++ zserio::BitBuffer type */
    ENUM, /**< C++ enumeration generated from zserio enumeration type */
    BITMASK, /**< C++ object generated from zserio bitmask type */
    STRUCT, /**< C++ object generated from zserio structure type */
    CHOICE, /**< C++ object generated from zserio choice type */
    UNION, /**< C++ object generated from zserio union type */
    SQL_TABLE, /**< C++ object generated from zserio SQL table type */
    SQL_DATABASE, /**< C++ object generated from zserio SQL database type */
    SERVICE, /**< C++ object generated from zserio service type */
    PUBSUB /**< C++ object generated from zserio pubsub type */
};

// TODO[Mi-L@]: Do we still need STD integers?
// Currently not used since runtime won't recognize bit:16 and uint16.
/** Enumeration which specifies zserio type used in type information. */
enum class SchemaType
{
    BOOL, /**< zserio bool type */
    INT1, /**< zserio int1 type */
    INT2, /**< zserio int2 type */
    INT3, /**< zserio int3 type */
    INT4, /**< zserio int4 type */
    INT5, /**< zserio int5 type */
    INT6, /**< zserio int6 type */
    INT7, /**< zserio int7 type */
    INT8, /**< zserio int8 type */
    INT9, /**< zserio int9 type */
    INT10, /**< zserio int10 type */
    INT11, /**< zserio int11 type */
    INT12, /**< zserio int12 type */
    INT13, /**< zserio int13 type */
    INT14, /**< zserio int14 type */
    INT15, /**< zserio int15 type */
    INT16, /**< zserio int16 type */
    INT17, /**< zserio int17 type */
    INT18, /**< zserio int18 type */
    INT19, /**< zserio int19 type */
    INT20, /**< zserio int20 type */
    INT21, /**< zserio int21 type */
    INT22, /**< zserio int22 type */
    INT23, /**< zserio int23 type */
    INT24, /**< zserio int24 type */
    INT25, /**< zserio int25 type */
    INT26, /**< zserio int26 type */
    INT27, /**< zserio int27 type */
    INT28, /**< zserio int28 type */
    INT29, /**< zserio int29 type */
    INT30, /**< zserio int30 type */
    INT31, /**< zserio int31 type */
    INT32, /**< zserio int32 type */
    INT33, /**< zserio int33 type */
    INT34, /**< zserio int34 type */
    INT35, /**< zserio int35 type */
    INT36, /**< zserio int36 type */
    INT37, /**< zserio int37 type */
    INT38, /**< zserio int38 type */
    INT39, /**< zserio int39 type */
    INT40, /**< zserio int40 type */
    INT41, /**< zserio int41 type */
    INT42, /**< zserio int42 type */
    INT43, /**< zserio int43 type */
    INT44, /**< zserio int44 type */
    INT45, /**< zserio int45 type */
    INT46, /**< zserio int46 type */
    INT47, /**< zserio int47 type */
    INT48, /**< zserio int48 type */
    INT49, /**< zserio int49 type */
    INT50, /**< zserio int50 type */
    INT51, /**< zserio int51 type */
    INT52, /**< zserio int52 type */
    INT53, /**< zserio int53 type */
    INT54, /**< zserio int54 type */
    INT55, /**< zserio int55 type */
    INT56, /**< zserio int56 type */
    INT57, /**< zserio int57 type */
    INT58, /**< zserio int58 type */
    INT59, /**< zserio int59 type */
    INT60, /**< zserio int60 type */
    INT61, /**< zserio int61 type */
    INT62, /**< zserio int62 type */
    INT63, /**< zserio int63 type */
    INT64, /**< zserio int64 type */
    UINT1, /**< zserio uint1 type */
    UINT2, /**< zserio uint2 type */
    UINT3, /**< zserio uint3 type */
    UINT4, /**< zserio uint4 type */
    UINT5, /**< zserio uint5 type */
    UINT6, /**< zserio uint6 type */
    UINT7, /**< zserio uint7 type */
    UINT8, /**< zserio uint8 type */
    UINT9, /**< zserio uint9 type */
    UINT10, /**< zserio uint10 type */
    UINT11, /**< zserio uint11 type */
    UINT12, /**< zserio uint12 type */
    UINT13, /**< zserio uint13 type */
    UINT14, /**< zserio uint14 type */
    UINT15, /**< zserio uint15 type */
    UINT16, /**< zserio uint16 type */
    UINT17, /**< zserio uint17 type */
    UINT18, /**< zserio uint18 type */
    UINT19, /**< zserio uint19 type */
    UINT20, /**< zserio uint20 type */
    UINT21, /**< zserio uint21 type */
    UINT22, /**< zserio uint22 type */
    UINT23, /**< zserio uint23 type */
    UINT24, /**< zserio uint24 type */
    UINT25, /**< zserio uint25 type */
    UINT26, /**< zserio uint26 type */
    UINT27, /**< zserio uint27 type */
    UINT28, /**< zserio uint28 type */
    UINT29, /**< zserio uint29 type */
    UINT30, /**< zserio uint30 type */
    UINT31, /**< zserio uint31 type */
    UINT32, /**< zserio uint32 type */
    UINT33, /**< zserio uint33 type */
    UINT34, /**< zserio uint34 type */
    UINT35, /**< zserio uint35 type */
    UINT36, /**< zserio uint36 type */
    UINT37, /**< zserio uint37 type */
    UINT38, /**< zserio uint38 type */
    UINT39, /**< zserio uint39 type */
    UINT40, /**< zserio uint40 type */
    UINT41, /**< zserio uint41 type */
    UINT42, /**< zserio uint42 type */
    UINT43, /**< zserio uint43 type */
    UINT44, /**< zserio uint44 type */
    UINT45, /**< zserio uint45 type */
    UINT46, /**< zserio uint46 type */
    UINT47, /**< zserio uint47 type */
    UINT48, /**< zserio uint48 type */
    UINT49, /**< zserio uint49 type */
    UINT50, /**< zserio uint50 type */
    UINT51, /**< zserio uint51 type */
    UINT52, /**< zserio uint52 type */
    UINT53, /**< zserio uint53 type */
    UINT54, /**< zserio uint54 type */
    UINT55, /**< zserio uint55 type */
    UINT56, /**< zserio uint56 type */
    UINT57, /**< zserio uint57 type */
    UINT58, /**< zserio uint58 type */
    UINT59, /**< zserio uint59 type */
    UINT60, /**< zserio uint60 type */
    UINT61, /**< zserio uint61 type */
    UINT62, /**< zserio uint62 type */
    UINT63, /**< zserio uint63 type */
    UINT64, /**< zserio uint64 type */
    VARINT16, /**< zserio varint16 type */
    VARINT32, /**< zserio varint32 type */
    VARINT64, /**< zserio varint64 type */
    VARINT, /**< zserio varint type */
    VARUINT16, /**< zserio varuint16 type */
    VARUINT32, /**< zserio varuint32 type */
    VARUINT64, /**< zserio varuint64 type */
    VARUINT, /**< zserio varuint type */
    VARSIZE, /**< zserio varsize type */
    DYNAMIC_SIGNED_BITFIELD, /**< zserio dynamic signed bitfield type */
    DYNAMIC_UNSIGNED_BITFIELD, /**< zserio dynamic unsigned bitfield type */
    FLOAT16, /**< zserio float16 type */
    FLOAT32, /**< zserio float32 type */
    FLOAT64, /**< zserio float64 type */
    BYTES, /**< zserio bytes type */
    STRING, /**< zserio string type */
    EXTERN, /**< zserio extern type */
    ENUM, /**< zserio enumeration type */
    BITMASK, /**< zserio bitmask type */
    STRUCT, /**< zserio structure type */
    CHOICE, /**< zserio choice type */
    UNION, /**< zserio union type */
    SQL_TABLE, /**< zserio SQL table type */
    SQL_DATABASE, /**< zserio SQL database type */
    SERVICE, /**< zserio service type */
    PUBSUB /**< zserio pubsub type */
};

// forward declarations
template <typename ALLOC>
struct BasicFieldInfo;
template <typename ALLOC>
struct BasicParameterInfo;
template <typename ALLOC>
struct BasicFunctionInfo;
template <typename ALLOC>
struct BasicCaseInfo;
template <typename ALLOC>
struct BasicColumnInfo;
template <typename ALLOC>
struct BasicTableInfo;
struct ItemInfo;
template <typename ALLOC>
struct BasicTemplateArgumentInfo;
template <typename ALLOC>
struct BasicMessageInfo;
template <typename ALLOC>
struct BasicMethodInfo;

/**
 * Type information interface which is returned from the generated zserio objects.
 *
 * This interface provides additional schema information of the corresponded zserio object, like schema
 * name, schema type, etc...
 *
 * Not all methods are implemented for all zserio objects. For example, the method getFields() is implemented
 * for compound types only. To check the zserio object type consider to use TypeInfoUtil helper methods.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class IBasicTypeInfo
{
public:
    /**
     * Virtual destructor.
     */
    virtual ~IBasicTypeInfo() = default;

    /**
     * Gets the schema name.
     *
     * \return The zserio full name stored in schema.
     */
    virtual std::string_view getSchemaName() const = 0;

    /**
     * Gets the schema type.
     *
     * \return The zserio type stored in schema.
     */
    virtual SchemaType getSchemaType() const = 0;

    /**
     * Gets the C++ type.
     *
     * \return The C++ type to which zserio type is mapped.
     */
    virtual CppType getCppType() const = 0;

    // method for fixed size integral types

    /**
     * Gets the bit size of the fixed size integral schema type.
     *
     * \return The bit size of zserio type.
     *
     * \throw CppRuntimeException If the zserio type is not fixed size integral (e.g. varint).
     */
    virtual uint8_t getBitSize() const = 0;

    // methods for compound types

    /**
     * Gets the type information for compound type fields.
     *
     * \return Sequence of type information for fields.
     *
     * \throw CppRuntimeException If the zserio type is not compound type.
     */
    virtual Span<const BasicFieldInfo<ALLOC>> getFields() const = 0;

    /**
     * Gets the type information for compound type parameters.
     *
     * \return Sequence of type information for parameters.
     *
     * \throw CppRuntimeException If the zserio type is not compound type.
     */
    virtual Span<const BasicParameterInfo<ALLOC>> getParameters() const = 0;

    /**
     * Gets the type information for compound type functions.
     *
     * \return Sequence of type information for functions.
     *
     * \throw CppRuntimeException If the zserio type is not compound type.
     */
    virtual Span<const BasicFunctionInfo<ALLOC>> getFunctions() const = 0;

    // methods for choice type

    /**
     * Gets the selector for choice type.
     *
     * \return Selector expression of choice type.
     *
     * \throw CppRuntimeException If the zserio type is not choice type.
     */
    virtual std::string_view getSelector() const = 0;

    /**
     * Gets the type information for choice type cases.
     *
     * \return Sequence of type information for choice type cases.
     *
     * \throw CppRuntimeException If the zserio type is not choice type.
     */
    virtual Span<const BasicCaseInfo<ALLOC>> getCases() const = 0;

    // methods for enumeration and bitmask types

    /**
     * Gets the reference to type information of underlying zserio type.
     *
     * \return Reference to type information of underlying zserio type.
     *
     * \throw CppRuntimeException If the zserio type is not enumeration or bitmask type.
     */
    virtual const IBasicTypeInfo<ALLOC>& getUnderlyingType() const = 0;

    /**
     * Gets the type information for enumeration type items.
     *
     * \return Sequence of type information for enumeration type items.
     *
     * \throw CppRuntimeException If the zserio type is not enumeration type.
     */
    virtual Span<const ItemInfo> getEnumItems() const = 0;

    /**
     * Gets the type information for bitmask type values.
     *
     * \return Sequence of type information for bitmask type values.
     *
     * \throw CppRuntimeException If the zserio type is not bitmask type.
     */
    virtual Span<const ItemInfo> getBitmaskValues() const = 0;

    // methods for SQL table types

    /**
     * Gets the type information for SQL table columns.
     *
     * \return Sequence of type information for SQL table columns.
     *
     * \throw CppRuntimeException If the zserio type is not SQL table type.
     */
    virtual Span<const BasicColumnInfo<ALLOC>> getColumns() const = 0;

    /**
     * Gets the SQL table constraint.
     *
     * \return The SQL table constraint.
     *
     * \throw CppRuntimeException If the zserio type is not SQL table type.
     */
    virtual std::string_view getSqlConstraint() const = 0;

    /**
     * Gets the SQL virtual table using specification.
     *
     * \return The SQL virtual table using specification.
     *
     * \throw CppRuntimeException If the zserio type is not SQL table type.
     */
    virtual std::string_view getVirtualTableUsing() const = 0;

    /**
     * Checks if SQL table is without row id table.
     *
     * \return true if SQL table is without row id table, otherwise false.
     *
     * \throw CppRuntimeException If the zserio type is not SQL table type.
     */
    virtual bool isWithoutRowId() const = 0;

    // method for SQL database type

    /**
     * Gets the type information for SQL database tables.
     *
     * \return Sequence of type information for SQL database tables.
     *
     * \throw CppRuntimeException If the zserio type is not SQL database type.
     */
    virtual Span<const BasicTableInfo<ALLOC>> getTables() const = 0;

    // methods for templatable types

    /**
     * Gets the full schema template name.
     *
     * \return The full schema template name.
     *
     * \throw CppRuntimeException If the zserio type is not templatable.
     */
    virtual std::string_view getTemplateName() const = 0;

    /**
     * Gets the type information for template arguments.
     *
     * \return Sequence of type information for template arguments.
     *
     * \throw CppRuntimeException If the zserio type is not templatable.
     */
    virtual Span<const BasicTemplateArgumentInfo<ALLOC>> getTemplateArguments() const = 0;

    // method for pubsub type

    /**
     * Gets the type information for pubsub messages.
     *
     * \return Sequence of type information for pubsub messages.
     *
     * \throw CppRuntimeException If the zserio type is not pubsub type.
     */
    virtual Span<const BasicMessageInfo<ALLOC>> getMessages() const = 0;

    // method for service type

    /**
     * Gets the type information for service methods.
     *
     * \return Sequence of type information for service methods.
     *
     * \throw CppRuntimeException If the zserio type is not service type.
     */
    virtual Span<const BasicMethodInfo<ALLOC>> getMethods() const = 0;

    /**
     * Creates new instance of the zserio compound type.
     *
     * \param allocator Allocator to use for allocation of new instance.
     *
     * \return New instance of zserio compound type.
     *
     * \throw CppRuntimeException If the zserio type is not compound type.
     */
    virtual IBasicReflectableDataPtr<ALLOC> createInstance(const ALLOC& allocator) const = 0;

    /**
     * Creates new instance of the zserio compound type.
     *
     * \note Default constructed allocator is used for allocation of new instance.
     *
     * \return New instance of zserio compound type.
     *
     * \throw CppRuntimeException If the zserio type is not compound type.
     */
    virtual IBasicReflectableDataPtr<ALLOC> createInstance() const = 0;
};

/**
 * Type information for compound type field.
 */
template <typename ALLOC = std::allocator<uint8_t>>
struct BasicFieldInfo
{
    BasicFieldInfo(std::string_view schemaName_, const IBasicTypeInfo<ALLOC>& typeInfo_,
            Span<const std::string_view> typeArguments_, bool isExtended_, std::string_view alignment_,
            std::string_view offset_, std::string_view initializer_, bool isOptional_,
            std::string_view optionalCondition_, std::string_view constraint_, bool isArray_,
            std::string_view arrayLength_, bool isPacked_, bool isImplicit_) :
            schemaName(schemaName_),
            typeInfo(typeInfo_),
            typeArguments(typeArguments_),
            isExtended(isExtended_),
            alignment(alignment_),
            offset(offset_),
            initializer(initializer_),
            isOptional(isOptional_),
            optionalCondition(optionalCondition_),
            constraint(constraint_),
            isArray(isArray_),
            arrayLength(arrayLength_),
            isPacked(isPacked_),
            isImplicit(isImplicit_)
    {}

    std::string_view schemaName; /**< field schema name */
    const IBasicTypeInfo<ALLOC>& typeInfo; /**< reference to type information for a field type */
    Span<const std::string_view> typeArguments; /**< sequence of field type arguments */
    bool isExtended; /**< true if field is extended */
    std::string_view alignment; /**< field alignment or empty in case of no alignment */
    std::string_view offset; /**< field offset or empty in case of no alignment */
    std::string_view initializer; /**< field initializer or empty in case of no alignment */
    bool isOptional; /**< true if field is optional */
    std::string_view optionalCondition; /**< optional condition or empty if field is not optional */
    std::string_view constraint; /**< constraint or empty if field does not have constraint */
    bool isArray; /**< true if field is array */
    std::string_view arrayLength; /**< array length or empty if field is not array or is auto/implicit array */
    bool isPacked; /**< true if field is array and packed */
    bool isImplicit; /**< true if field is array and implicit */
};

/**
 * Type information for compound type parameter.
 */
template <typename ALLOC = std::allocator<uint8_t>>
struct BasicParameterInfo
{
    std::string_view schemaName; /**< parameter schema name */
    const IBasicTypeInfo<ALLOC>& typeInfo; /**< reference to type information for a parameter type */
};

/**
 * Type information for compound type function.
 */
template <typename ALLOC = std::allocator<uint8_t>>
struct BasicFunctionInfo
{
    std::string_view schemaName; /**< function schema name */
    const IBasicTypeInfo<ALLOC>& typeInfo; /**< reference to type information for a resulting function type */
    std::string_view functionResult; /**< specifies the function result */
};

/**
 * Type information for choice type case.
 */
template <typename ALLOC = std::allocator<uint8_t>>
struct BasicCaseInfo
{
    Span<const std::string_view> caseExpressions; /**< sequence of case expressions */
    const BasicFieldInfo<ALLOC>* field; /**< pointer to type information for a case field */
};

/**
 * Type information for enumeration type item or for bitmask type value.
 */
struct ItemInfo
{
    ItemInfo(std::string_view schemaName_, uint64_t value_, bool isDeprecated_, bool isRemoved_) :
            schemaName(schemaName_),
            value(value_),
            isDeprecated(isDeprecated_),
            isRemoved(isRemoved_)
    {}

    std::string_view schemaName; /**< enumeration item or bitmask value schema name */
    uint64_t value; /**< enumeration item or bitmask value cast to uint64_t */
    bool isDeprecated; /**< flag whether the item is deprecated */
    bool isRemoved; /**< flag whether the item is removed */
};

/**
 * Type information for SQL table column.
 */
template <typename ALLOC = std::allocator<uint8_t>>
struct BasicColumnInfo
{
    BasicColumnInfo(std::string_view schemaName_, const IBasicTypeInfo<ALLOC>& typeInfo_,
            Span<const std::string_view> typeArguments_, std::string_view sqlTypeName_,
            std::string_view sqlConstraint_, bool isVirtual_) :
            schemaName(schemaName_),
            typeInfo(typeInfo_),
            typeArguments(typeArguments_),
            sqlTypeName(sqlTypeName_),
            sqlConstraint(sqlConstraint_),
            isVirtual(isVirtual_)
    {}

    std::string_view schemaName; /**< column schema name */
    const IBasicTypeInfo<ALLOC>& typeInfo; /**< reference to type information for a column type */
    Span<const std::string_view> typeArguments; /**< sequence of column type arguments */
    std::string_view sqlTypeName; /**< column SQL type name */
    std::string_view sqlConstraint; /**< column constraint or empty if column does not have any constraint */
    bool isVirtual; /**< true if SQL table is virtual */
};

/**
 * Type information for SQL database table.
 */
template <typename ALLOC = std::allocator<uint8_t>>
struct BasicTableInfo
{
    std::string_view schemaName; /**< table schema name */
    const IBasicTypeInfo<ALLOC>& typeInfo; /**< reference to type information for a table */
};

/**
 * Type information for template argument.
 */
template <typename ALLOC = std::allocator<uint8_t>>
struct BasicTemplateArgumentInfo
{
    const IBasicTypeInfo<ALLOC>& typeInfo; /**< reference to type information for a template argument */
};

/**
 * Type information for pubsub message.
 */
template <typename ALLOC = std::allocator<uint8_t>>
struct BasicMessageInfo
{
    BasicMessageInfo(std::string_view schemaName_, const IBasicTypeInfo<ALLOC>& typeInfo_, bool isPublished_,
            bool isSubscribed_, std::string_view topic_) :
            schemaName(schemaName_),
            typeInfo(typeInfo_),
            isPublished(isPublished_),
            isSubscribed(isSubscribed_),
            topic(topic_)
    {}

    std::string_view schemaName; /**< message schema name */
    const IBasicTypeInfo<ALLOC>& typeInfo; /**< reference to type information for a message type */
    bool isPublished; /**< true if the message is published */
    bool isSubscribed; /**< true if the message is subscribed */
    std::string_view topic; /**< pubsub topic for a message */
};

/**
 * Type information for service method.
 */
template <typename ALLOC = std::allocator<uint8_t>>
struct BasicMethodInfo
{
    /** service schema name */
    std::string_view schemaName;
    /** reference to type information for a method response type */
    const IBasicTypeInfo<ALLOC>& responseTypeInfo;
    /** reference to type information for a method request type */
    const IBasicTypeInfo<ALLOC>& requestTypeInfo;
};

namespace detail
{

/** Helper struct for typeInfo free function implementation. Functionality provided via specializations. */
template <typename T, typename ALLOC>
struct TypeInfo;

template <typename T, typename V = void>
struct type_info_default_alloc
{
    using type = std::allocator<uint8_t>;
};

template <typename T>
struct type_info_default_alloc<T, std::enable_if_t<has_allocator_v<T>>>
{
    using type = RebindAlloc<typename T::allocator_type, uint8_t>;
};

template <typename T, typename V = void>
using type_info_default_alloc_t = typename type_info_default_alloc<T, V>::type;

} // namespace detail

/**
 * Global function for type info of a generated type provided via specializations.
 *
 * \return Type info.
 */
template <typename T, typename ALLOC = detail::type_info_default_alloc_t<T>>
const IBasicTypeInfo<ALLOC>& typeInfo()
{
    return detail::TypeInfo<T, ALLOC>::get();
}

/** Typedef provided for convenience - using default std::allocator<uint8_t>. */
/** \{ */
using ITypeInfo = IBasicTypeInfo<>;
using FieldInfo = BasicFieldInfo<>;
using ParameterInfo = BasicParameterInfo<>;
using FunctionInfo = BasicFunctionInfo<>;
using CaseInfo = BasicCaseInfo<>;
using ColumnInfo = BasicColumnInfo<>;
using TableInfo = BasicTableInfo<>;
using TemplateArgumentInfo = BasicTemplateArgumentInfo<>;
using MessageInfo = BasicMessageInfo<>;
using MethodInfo = BasicMethodInfo<>;
/** \} */

} // namespace zserio

#endif // ZSERIO_I_TYPE_INFO_INC_H
