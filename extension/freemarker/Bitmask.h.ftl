<#include "FileHeader.inc.ftl">
<#include "DocComment.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

<@type_includes types.string/>
#include <zserio/Bitmasks.h>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>

<#if docComments??>
<@doc_comments docComments/>
</#if>
class ${name}
{
public:
    /** Definition for bitmask zserio type. */
    using ZserioType = ${underlyingTypeInfo.typeFullName};

    /** Enumeration of all bitmask values. */
    enum class Values : ZserioType::ValueType
    {
<#list values as value>
    <#if value.docComments??>
        <@doc_comments value.docComments, 2/>
    </#if>
        ${value.name} = ${value.value}<#if !value?is_last>,</#if>
</#list>
    };

    /** Default constructor. */
    constexpr ${name}() noexcept :
            m_value(0)
    {}

    /**
     * Bitmask value constructor.
     *
     * \param value Bitmask value to construct from.
     */
    constexpr ${name}(Values value) noexcept :
            m_value(static_cast<ZserioType::ValueType>(value))
    {}

    /**
     * Raw bitmask value constructor.
     *
     * \param value Raw bitmask value to construct from.
     */
    constexpr explicit ${name}(ZserioType value) noexcept :
            m_value(value)
    {}

    /** Default destructor. */
    ~${name}() = default;

    /** Default copy constructor. */
    ${name}(const ${name}&) = default;

    /** Default assignment operator. */
    ${name}& operator=(const ${name}&) = default;

    /** Default move constructor. */
    ${name}(${name}&&) = default;
    /** Default move assignment operator. */
    ${name}& operator=(${name}&&) = default;

    /**
     * Defines conversion to bitmask underlying type.
     *
     * \return Raw value which holds this bitmask.
     */
    constexpr explicit operator ZserioType() const
    {
        return m_value;
    }

    /**
     * Gets the bitmask raw value.
     *
     * \return Raw value which holds this bitmask.
     */
    constexpr ZserioType getValue() const
    {
        return m_value;
    }

    /**
     * Converts the bitmask value to the string.
     *
     * \param allocator Allocator to use for all dynamic memory allocations.
     *
     * \return String which represents this bitmask.
     */
    ${types.string.name} toString(const ${types.string.name}::allocator_type& allocator =
            ${types.string.name}::allocator_type()) const;

private:
    ZserioType m_value;
};

/**
 * Defines operator '==' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is equal to the rhs, otherwise false.
 */
inline constexpr bool operator==(const ${name}& lhs, const ${name}& rhs)
{
    return lhs.getValue() == rhs.getValue();
}

/**
 * Defines operator '!=' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is not equal to the rhs, otherwise false.
 */
inline constexpr bool operator!=(const ${name}& lhs, const ${name}& rhs)
{
    return lhs.getValue() != rhs.getValue();
}

/**
 * Defines operator '<' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is less than the rhs, otherwise false.
 */
inline constexpr bool operator<(const ${name}& lhs, const ${name}& rhs)
{
    return lhs.getValue() < rhs.getValue();
}

/**
 * Defines operator '>' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is greater than the rhs, otherwise false.
 */
inline constexpr bool operator>(const ${name}& lhs, const ${name}& rhs)
{
    return lhs.getValue() > rhs.getValue();
}

/**
 * Defines operator '<=' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is less than or equal to the rhs, otherwise false.
 */
inline constexpr bool operator<=(const ${name}& lhs, const ${name}& rhs)
{
    return lhs.getValue() <= rhs.getValue();
}

/**
 * Defines operator '>=' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is greater than or equal to the rhs, otherwise false.
 */
inline constexpr bool operator>=(const ${name}& lhs, const ${name}& rhs)
{
    return lhs.getValue() >= rhs.getValue();
}

/**
 * Defines operator '|' for the bitmask '${name}'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '|' on given operands.
 */
inline constexpr ${name} operator|(const ${name}& lhs, const ${name}& rhs)
{
    return ${name}(lhs.getValue() | rhs.getValue());
}

/**
 * Defines operator '|' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '|' on given operands.
 */
inline constexpr ${name} operator|(${name}::Values lhs, ${name}::Values rhs)
{
    return ${name}(lhs) | ${name}(rhs);
}

/**
 * Defines operator '&' for the bitmask '${name}'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '&' on given operands.
 */
inline constexpr ${name} operator&(const ${name}& lhs, const ${name}& rhs)
{
    return ${name}(lhs.getValue() & rhs.getValue());
}

/**
 * Defines operator '&' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '&' on given operands.
 */
inline constexpr ${name} operator&(${name}::Values lhs, ${name}::Values rhs)
{
    return ${name}(lhs) & ${name}(rhs);
}

/**
 * Defines operator '^' for the bitmask '${name}'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '^' on given operands.
 */
inline constexpr ${name} operator^(const ${name}& lhs, const ${name}& rhs)
{
    return ${name}(lhs.getValue() ^ rhs.getValue());
}

/**
 * Defines operator '^' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '^' on given operands.
 */
inline constexpr ${name} operator^(${name}::Values lhs, ${name}::Values rhs)
{
    return ${name}(lhs) ^ ${name}(rhs);
}

/**
 * Defines operator '~' for the bitmask '${name}'.
 *
 * \param lhs Const reference to the left operand.
 *
 * \return Bitmask which contains result after applying the operator '~' on given operand.
 */
inline constexpr ${name} operator~(const ${name}& lhs)
{
    return ${name}(~lhs.getValue());
}

/**
 * Defines operator '~' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 *
 * \return Bitmask which contains result after applying the operator '~' on given operand.
 */
inline constexpr ${name} operator~(${name}::Values lhs)
{
    return ~${name}(lhs);
}

/**
 * Defines operator '|=' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '|=' on given operands.
 */
inline ${name} operator|=(${name}& lhs, const ${name}& rhs)
{
    lhs = ${name}(lhs.getValue() | rhs.getValue());
    return lhs;
}

/**
 * Defines operator '&=' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '&=' on given operands.
 */
inline ${name} operator&=(${name}& lhs, const ${name}& rhs)
{
    lhs = ${name}(lhs.getValue() & rhs.getValue());
    return lhs;
}

/**
 * Defines operator '^=' for the bitmask '${name}'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '^=' on given operands.
 */
inline ${name} operator^=(${name}& lhs, const ${name}& rhs)
{
    lhs = ${name}(lhs.getValue() ^ rhs.getValue());
    return lhs;
}
<@namespace_end package.path/>
<@namespace_begin ["std"]/>

template <>
struct hash<${fullName}>
{
    size_t operator()(const ${fullName}& value) const;
};
<@namespace_end ["std"]/>

<@include_guard_end package.path, name/>
