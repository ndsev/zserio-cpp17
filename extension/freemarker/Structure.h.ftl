<#include "FileHeader.inc.ftl">
<#include "DocComment.inc.ftl">
<#include "CompoundField.inc.ftl">
<#include "CompoundParameter.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <memory>
#include <zserio/View.h>
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

    explicit ${name}(
    <#items as field>
        <@field_data_type_name field/> <@field_data_arg_name field/><#if field?has_next>,<#else>) noexcept;</#if>
    </#items>
</#list>

<#list fieldList as field>
    <@field_data_type_name field/> <@field_data_member_name field/>;
</#list>
};

bool operator==(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator<(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator!=(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator>(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator<=(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator>=(const ${fullName}& lhs, const ${fullName}& rhs);
<@namespace_end package.path/>
<@namespace_begin ["zserio"]/>

template <>
class View<${fullName}>
{
public:
    View(const ${fullName}& data<#rt>
<#list parameterList as parameter>
            <#lt>,
            <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
</#list>
            <#lt>) noexcept;
<#list parameterList>

    <#items as parameter>
    <@parameter_view_type_name parameter/> ${parameter.getterName}() const;
    </#items>
</#list>
<#list fieldList>

    <#items as field>
    <@field_view_getter_type_name field/> ${field.getterName}() const;
    </#items>
</#list>

private:
    const ${fullName}& m_data;
<#list parameterList as parameter>
    <@parameter_view_type_name parameter/> <@parameter_view_member_name parameter/>;
</#list>
};

bool operator==(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
bool operator<(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
bool operator!=(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
bool operator>(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
bool operator<=(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
bool operator>=(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
<@namespace_begin ["detail"]/>

template <>
void validate(const View<${fullName}>& view);

template <>
void write(::zserio::BitStreamWriter& writer, const View<${fullName}>& view);

View<${fullName}> read(::zserio::BitStreamReader& reader, ${fullName}& data);

template <typename... ARGS>
View<${fullName}> read(::zserio::BitStreamReader& reader, ${fullName}& data, ARGS&&... args)
{
    read(reader, data, std::forward<ARGS>(args)...);
}

template <>
BitSize bitSizeOf(const View<${fullName}>& view, BitSize bitPosition);
<@namespace_end ["detail"]/>
<@namespace_end ["zserio"]/>
<@namespace_begin ["std"]/>

template <>
struct hash<${fullName}>
{
    size_t operator()(const ${fullName}& value) const;
};
<@namespace_end ["std"]/>

<@include_guard_end package.path, name/>
