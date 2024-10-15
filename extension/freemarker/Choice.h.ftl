<#include "FileHeader.inc.ftl">
<#include "DocComment.inc.ftl">
<#include "CompoundField.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <memory>
#include <zserio/ChoiceTag.h>
#include <zserio/Variant.h>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>

// forward declaration
struct ${name};
<@namespace_end package.path/>
<@namespace_begin ["zserio", "detail"]/>

template <>
struct ChoiceTag<${fullName}>
{
    enum Tag : size_t
    {
        UNDEFINED_CHOICE,
    <#list fieldList as field>
        <@choice_tag_name field/><#sep>,</#sep>
    </#list>
    };
};
<@namespace_end ["zserio", "detail"]/>
<@namespace_begin package.path/>

<#if docComments??>
<@doc_comments docComments/>
</#if>
struct ${name} : ::zserio::Variant<::zserio::detail::ChoiceTag<${name}>::Tag,
        ::std::monostate<#rt>
<#list fieldList as field>
        <#lt>,
        <#nt><@field_data_type_name field/><#rt>
</#list>
        <#lt>>
{
    using ChoiceTag = ::zserio::detail::ChoiceTag<${name}>::Tag;
    using Base = ::zserio::Variant<ChoiceTag,
            ::std::monostate<#rt>
<#list fieldList as field>
            <#lt>,
            <#nt><@field_data_type_name field/><#rt>
</#list>
            <#lt>>;
    using Base::BasicVariant;
};

bool operator==(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator<(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator!=(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator>(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator<=(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator>=(const ${fullName}& lhs, const ${fullName}& rhs);
<@namespace_end package.path/>
<@namespace_begin ["std"]/>

template <>
struct hash<${fullName}>
{
    size_t operator()(const ${fullName}& value) const;
};
<@namespace_end ["std"]/>

<@include_guard_end package.path, name/>
