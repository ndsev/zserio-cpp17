<#include "FileHeader.inc.ftl">
<#include "TypeInfo.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/StringConvertUtil.h>
<#if withTypeInfoCode>
#include <zserio/TypeInfo.h>
#include <zserio/ReflectableData.h>
</#if>
<@system_includes cppSystemIncludes/>

<@user_include package.path, "${name}.h"/>
<@user_includes cppUserIncludes, false/>
<@namespace_begin ["zserio"]/>

template <>
size_t enumToOrdinal(${fullName} value)
{
    switch (value)
    {
<#list items as item>
    case ${item.fullName}:
        return ${item?index};
</#list>
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration ${name}: ") <<
                static_cast<typename ::std::underlying_type_t<${fullName}>>(value) << "!";
    }
}

template <>
${fullName} valueToEnum(${underlyingTypeInfo.typeFullName} rawValue)
{
    switch (rawValue)
    {
<#list items as item>
    case ${item.value}:
</#list>
        return static_cast<${fullName}>(static_cast<::std::underlying_type_t<${fullName}>>(rawValue));
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration ${name}: ") << rawValue << "!";
    }
}
<#if withTypeInfoCode>
<@namespace_begin ["detail"]/>

const ${types.typeInfo.name}& TypeInfo<${fullName}, ${types.allocator.default}>::get()
{
    using AllocatorType = ${types.allocator.default};

    <@item_info_array_var "items", items/>

    static const ::zserio::detail::EnumTypeInfo<AllocatorType> typeInfo = {
        "${schemaTypeName}",
        <@type_info underlyingTypeInfo/>,
        items
    };

    return typeInfo;
}
<@namespace_end ["detail"]/>

template <>
${types.reflectablePtr.name} reflectable(${fullName} value, const ${types.allocator.default}& allocator)
{
    class Reflectable : public ::zserio::ReflectableBase<${types.allocator.default}>
    {
    public:
        explicit Reflectable(${fullName} value) :
                ::zserio::ReflectableBase<${types.allocator.default}>(
                        typeInfo<${fullName}, ${types.allocator.default}>()),
                m_value(value)
        {}

        ${types.any.name} getAnyValue(const ${types.allocator.default}& alloc) const override
        {
            return ${types.any.name}(m_value, alloc);
        }

        ${types.any.name} getAnyValue(const ${types.allocator.default}& alloc) override
        {
            return ${types.any.name}(m_value, alloc);
        }

        ${underlyingTypeInfo.typeFullName}::ValueType get<#if !isSigned>U</#if>Int${numBits}() const override
        {
            return static_cast<typename ::std::underlying_type<${fullName}>::type>(m_value);
        }

        <#if isSigned>int64_t toInt()<#else>uint64_t toUInt()</#if> const override
        {
            return static_cast<typename ::std::underlying_type<${fullName}>::type>(m_value);
        }

        double toDouble() const override
        {
            return static_cast<double>(<#if isSigned>toInt()<#else>toUInt()</#if>);
        }

        ${types.string.name} toString(const ${types.allocator.default}& alloc) const override
        {
            return ${types.string.name}(::zserio::enumToString(m_value), alloc);
        }

    private:
        ${fullName} m_value;
    };

    return std::allocate_shared<Reflectable>(allocator, value);
}
</#if>
<@namespace_end ["zserio"]/>
<@namespace_begin ["std"]/>

size_t hash<${fullName}>::operator()(const ${fullName}& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result,
            static_cast<::std::underlying_type_t<${fullName}>>(::zserio::enumToValue(value)));
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
