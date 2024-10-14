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

${name}::${name}(const allocator_type&<#if fieldList?has_content> allocator</#if>) noexcept<#rt>
<#if fieldList?has_content>
        <#lt> :
        variant(allocator)
<#else>

</#if>
{}

bool operator==(const ${fullName}&<#if fieldList?has_content>  lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content>  rhs</#if>)
{
    <#if fieldList?has_content>
    return lhs.variant == rhs.variant; 
    <#else>
    return true;
    </#if>
}

bool operator<(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content>  rhs</#if>)
{
    <#if fieldList?has_content>
    return lhs.variant < rhs.variant;
    <#else>
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

size_t hash<${fullName}>::operator()(const ${fullName}&<#if fieldList?has_content> value</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#if fieldList?has_content>
    result = ::zserio::calcHashCode(result, value.variant);
</#if>
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
