<#include "FileHeader.inc.ftl">
<#include "DocComment.inc.ftl">
<#include "CompoundField.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <memory>
#include <zserio/Variant.h>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>

<#if docComments??>
<@doc_comments docComments/>
</#if>
struct ${name}
{
    using allocator_type = ${types.allocator.default};

    ${name}() noexcept;
    explicit ${name}(const allocator_type& allocator) noexcept;
<#list fieldList>

    enum ChoiceTag : size_t
    {
        UNDEFINED_CHOICE,
    <#items as field>
        <@choice_tag_name field/><#sep>,</#sep>
    </#items>
    };
</#list>
<#list fieldList>

    ::zserio::Variant<ChoiceTag,
            ::std::monostate,
    <#items as field>
            <@field_data_type_name field/><#if field?has_next>,<#else>> variant;</#if>
    </#items>
</#list>
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
