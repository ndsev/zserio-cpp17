<#include "FileHeader.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/HashCodeUtil.h>
#include <zserio/StringConvertUtil.h>
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
            static_cast<underlying_type>(m_value), allocator) + "[" + result + "]";
}
<@namespace_end package.path/>
<@namespace_begin ["std"]/>

size_t hash<${fullName}>::operator()(const ${fullName}& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result,
            static_cast<${fullName}::underlying_type>(value.getValue()));
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
