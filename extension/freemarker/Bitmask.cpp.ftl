<#include "FileHeader.inc.ftl">
<#include "TypeInfo.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/HashCodeUtil.h>
#include <zserio/StringConvertUtil.h>
<#if withTypeInfoCode>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/TypeInfo.h>
</#if>
<@system_includes cppSystemIncludes/>

<@user_include package.path, "${name}.h"/>
<@user_includes cppUserIncludes, false/>
<@namespace_begin package.path/>

${types.string.name} ${name}::toString(const ${types.string.name}::allocator_type& allocator) const
{
    ${types.string.name} result(allocator);
<#list values as value>
    <#if !value.isZero>
    if ((*this & ${name}::Values::${value.name}) == ${name}::Values::${value.name})
    {
        result += result.empty() ? "${value.name}" : " | ${value.name}";
    }
    <#else>
        <#assign zeroValueName=value.name/><#-- may be there only once -->
    </#if>
</#list>
<#if zeroValueName??>
    if (result.empty() && m_value == 0)
    {
        result += "${zeroValueName}";
    }
</#if>

    return ::zserio::toString<${types.string.name}::allocator_type>(
            static_cast<ZserioType::ValueType>(m_value), allocator) + "[" + result + "]";
}
<@namespace_end package.path/>
<#if withTypeInfoCode>
<@namespace_begin ["zserio", "detail"]/>

const ${types.typeInfo.name}& TypeInfo<${fullName}, ${types.allocator.default}>::get()
{
    using AllocatorType = ${types.allocator.default};

    <@item_info_array_var "values", values/>

    static const ::zserio::detail::BitmaskTypeInfo<AllocatorType> typeInfo = {
        "${schemaTypeName}",
        <@type_info underlyingTypeInfo/>,
        values
    };

    return typeInfo;
}
<@namespace_end ["detail"]/>

template <>
${types.reflectablePtr.name} reflectable(${fullName} value, const ${types.allocator.default}& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataBase<${types.allocator.default}>
    {
    public:
        using ::zserio::ReflectableDataBase<${types.allocator.default}>::getAnyValue;

        explicit Reflectable(${fullName} bitmask) :
                ::zserio::ReflectableDataBase<${types.allocator.default}>(
                        typeInfo<${fullName}, ${types.allocator.default}>()),
                m_bitmask(bitmask)
        {}

        ${types.any.name} getAnyValue(const ${types.allocator.default}& alloc) const override
        {
            return ${types.any.name}(m_bitmask, alloc);
        }

        ${types.any.name} getAnyValue(const ${types.allocator.default}& alloc) override
        {
            return ${types.any.name}(m_bitmask, alloc);
        }

        <#-- bitmask is always unsigned -->
        ${fullName}::ZserioType::ValueType getUInt${nativeNumBits}() const override
        {
            return m_bitmask.getValue();
        }

        uint64_t toUInt() const override
        {
            return m_bitmask.getValue();
        }

        double toDouble() const override
        {
            return static_cast<double>(toUInt());
        }

        ${types.string.name} toString(const ${types.allocator.default}& alloc) const override
        {
            return m_bitmask.toString(alloc);
        }

    private:
        ${fullName} m_bitmask;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

template <>
${types.introspectableConstPtr.name} introspectable(${fullName} value, const ${types.allocator.default}& allocator)
{
    class Introspectable : public ::zserio::SimpleIntrospectableViewBase<${fullName}, ${types.allocator.default}>
    {
    public:
        explicit Introspectable(${fullName} bitmask) :
                ::zserio::SimpleIntrospectableViewBase<${fullName}, ${types.allocator.default}>(
                        typeInfo<${fullName}, ${types.allocator.default}>(), bitmask)
        {}
        <#-- bitmask is always unsigned -->
        ${fullName}::ZserioType::ValueType getUInt${nativeNumBits}() const override
        {
            return getValue().getValue();
        }

        uint64_t toUInt() const override
        {
            return getValue().getValue();
        }

        double toDouble() const override
        {
            return static_cast<double>(toUInt());
        }

        ${types.string.name} toString(const ${types.allocator.default}& alloc) const override
        {
            return getValue().toString(alloc);
        }
    };

    return ::std::allocate_shared<Introspectable>(allocator, value);
}
<@namespace_end ["zserio"]/>
</#if>
<@namespace_begin ["std"]/>

size_t hash<${fullName}>::operator()(const ${fullName}& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result,
            static_cast<${fullName}::ZserioType::ValueType>(value.getValue()));
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
