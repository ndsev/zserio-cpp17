<#include "FileHeader.inc.ftl">
<#include "CompoundField.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/HashCodeUtil.h>
<@system_includes cppSystemIncludes/>

<@user_include package.path, "${name}.h"/>
<@user_includes cppUserIncludes, false/>
<@namespace_begin package.path/>

${name}::${name}() noexcept :
        ${name}(allocator_type{})
{}

${name}::${name}(const allocator_type&<#if needs_allocator(fieldList)> allocator</#if>) noexcept<#rt>
<#list fieldList>
        <#lt> :
        <#items as field>
        ${field.name}(<#if field.needsAllocator>allocator</#if>)<#sep>,</#sep>
        </#items>
<#else>

</#list>
{}
<#if fieldList?has_content>

${name}::${name}(
    <#list fieldList as field>
        <@field_data_type_name field/> <@field_data_arg_name field/><#if field?has_next>,<#else>) noexcept :</#if>
    </#list>
    <#list fieldList as field>
        ${field.name}(<#if !field.typeInfo.isSimple>::std::move(</#if><@field_data_arg_name field/>)<#rt>
                <#lt><#if !field.typeInfo.isSimple>)</#if><#sep>,</#sep>
    </#list>
{}
</#if>

bool operator==(const ${fullName}& lhs, const ${fullName}& rhs)
{
    <#if fieldList?has_content>
    return ::std::tie(
        <#list fieldList as field>
            lhs.${field.name}<#if field?has_next>,<#else>)</#if>
        </#list>
            == std::tie(
        <#list fieldList as field>
            rhs.${field.name}<#if field?has_next>,<#else>);</#if>
        </#list>
    <#else>
    (void)lhs;
    (void)rhs;
    return true;
    </#if>
}

bool operator<(const ${fullName}& lhs, const ${fullName}& rhs)
{
    <#if fieldList?has_content>
    return ::std::tie(
        <#list fieldList as field>
            lhs.${field.name}<#if field?has_next>,<#else>)</#if>
        </#list>
            < std::tie(
        <#list fieldList as field>
            rhs.${field.name}<#if field?has_next>,<#else>);</#if>
        </#list>
    <#else>
    (void)lhs;
    (void)rhs;
    return false;
    </#if>
}

bool operator!=(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return !(lhs < rhs);
}
<@namespace_end package.path/>
<@namespace_begin ["std"]/>

size_t hash<${fullName}>::operator()(const ${fullName}& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#list fieldList as field>
    result = ::zserio::calcHashCode(result, value.${field.name});
<#else>
    (void)value;
</#list>
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
