<#include "FileHeader.inc.ftl">
<#include "DocComment.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <array>

<#function has_deprecated_items items>
    <#list items as item>
        <#if item.isDeprecated>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>
<#if has_deprecated_items(items)>
#include <zserio/DeprecatedAttribute.h>
</#if>
#include <zserio/Enums.h>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>

<#if docComments??>
<@doc_comments docComments/>
</#if>
enum class ${name} : ${underlyingTypeInfo.typeFullName}::ValueType
{
<#list items as item>
    <#if item.docComments??>
    <@doc_comments item.docComments, 1/>
    </#if>
    ${item.name}<#if item.isDeprecated> ZSERIO_DEPRECATED</#if> = ${item.value}<#if item?has_next>,</#if>
</#list>
};
<@namespace_end package.path/>
<@namespace_begin ["zserio"]/>

// This is full specialization of enumeration traits and methods for ${name} enumeration.
template <>
struct EnumTraits<${fullName}>
{
    static constexpr ::std::array<::std::string_view, ${items?size}> names =
    {{
<#list items as item>
        "${item.name}"<#if item?has_next>,</#if>
</#list>
    }};

    static constexpr ::std::array<${fullName}, ${items?size}> values =
    {{
<#list items as item>
        ${item.fullName}<#if item?has_next>,</#if>
</#list>
    }};

    static constexpr ::std::string_view enumName = "${name}";

    using ZserioType = ${underlyingTypeInfo.typeFullName};
};

template <>
size_t enumToOrdinal<${fullName}>(${fullName} value);

template <>
${fullName} valueToEnum<${fullName}>(${underlyingTypeInfo.typeFullName} rawValue);
<@namespace_end ["zserio"]/>
<@namespace_begin ["std"]/>

template <>
struct hash<${fullName}>
{
    size_t operator()(const ${fullName}& value) const;
};
<@namespace_end ["std"]/>

<@include_guard_end package.path, name/>
