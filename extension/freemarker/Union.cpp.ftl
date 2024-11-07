<#include "FileHeader.inc.ftl">
<#include "CompoundField.inc.ftl">
<#include "CompoundParameter.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/SizeConvertUtil.h>
<@system_includes cppSystemIncludes/>

<@user_include package.path, "${name}.h"/>
<@user_includes cppUserIncludes, false/>
<@namespace_begin package.path/>

bool operator==(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content> rhs</#if>)
{
<#if fieldList?has_content>
    return static_cast<const ${fullName}::Base&>(lhs) == static_cast<const ${fullName}::Base&>(rhs);
<#else>
    return true;
</#if>
}

bool operator<(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content>  rhs</#if>)
{
<#if fieldList?has_content>
    return static_cast<const ${fullName}::Base&>(lhs) < static_cast<const ${fullName}::Base&>(rhs);
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
<@namespace_begin ["zserio"]/>

View<${fullName}>::View(const ${fullName}& data<#rt>
<#list parameterList as parameter>
        <#lt>,
        <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
</#list>
        <#lt>) noexcept :
        m_data(data)<#rt>
<#list parameterList as parameter>
        <#lt>,
        <@parameter_view_member_name parameter/>(<@parameter_view_arg_name parameter/>)<#rt>
</#list>

{}
<#list parameterList as parameter>

<@parameter_view_type_name parameter/> View<${fullName}>::${parameter.getterName}() const
{
    return <@parameter_view_member_name parameter/>;
}
</#list>

${fullName}::ChoiceTag View<${fullName}>::zserioChoiceTag() const
{
    return m_data.index();
}
<#list fieldList as field>

<@field_view_type_name field/> View<${fullName}>::${field.getterName}() const
{
    <#if !field.array?? && !field.typeInfo.isDynamicBitField && field.typeInfo.isSimple>
    <#-- field which does not need View -->
    return get<${fullName}::ChoiceTag::<@choice_tag_name field/>>(m_data);
    <#else>
    <#-- field which needs View -->
    return <@field_view_type_name field/>{get<${fullName}::ChoiceTag::<@choice_tag_name field/>>(m_data)<#rt>
            <#lt><@field_view_parameters field/>};
    </#if>
}
</#list>

<#macro union_switch fieldActionMacroName switchExpression indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}switch (${switchExpression})
${I}{
    <#list fieldList as field>
${I}case ${fullName}::ChoiceTag::<@choice_tag_name field/>:
        <#assign caseCode><@.vars[fieldActionMacroName] field, indent+1/></#assign>
        ${caseCode}<#t>
        <#if !caseCode?contains("return")>
${I}    break;
        </#if>
    </#list>
${I}default:
        throw ::zserio::CppRuntimeException("No match in union ${fullName}!");
${I}}
</#macro>
<#macro union_compare_field field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return (lhs.${field.getterName}() == rhs.${field.getterName}());
</#macro>
bool operator==(const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> lhs</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> rhs</#if>)
{
<#list parameterList as parameter>
    if (lhs.${parameter.getterName}() != rhs.${parameter.getterName}())
    {
        return false;
    }

</#list>
<#if fieldList?has_content>
    <@union_switch "union_compare_field", "lhs.zserioChoiceTag()"/>
<#else>
    return true;
</#if>
}

<#macro union_less_than_field field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return (lhs.${field.getterName}() < rhs.${field.getterName}());
</#macro>
bool operator<(const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> lhs</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> rhs</#if>)
{
<#list parameterList as parameter>
    if (lhs.${parameter.getterName}() != rhs.${parameter.getterName}())
    {
        return lhs.${parameter.getterName}() < rhs.${parameter.getterName}();
    }

</#list>
<#if fieldList?has_content>
    <@union_switch "union_less_than_field", "lhs.zserioChoiceTag()"/>
<#else>
    return false;
</#if>
}

bool operator!=(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return !(lhs < rhs);
}
<@namespace_begin ["detail"]/>

template <>
void validate(const View<${fullName}>&)
{
    // TODO:
}

<#macro union_bitsizeof_field field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}endBitPosition += ::zserio::detail::bitSizeOf(view.${field.getterName}(), endBitPosition);
</#macro>
template <>
BitSize bitSizeOf(const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
<#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    <@union_switch "union_bitsizeof_field", "view.zserioChoiceTag()"/>

    return endBitPosition - bitPosition;
<#else>
    return 0;
</#if>
}

<#macro union_write_field field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}::zserio::detail::write(writer, view.${field.getterName}());
</#macro>
template <>
void write(::zserio::BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#if fieldList?has_content>
    const ${fullName}::ChoiceTag choiceTag = view.zserioChoiceTag();
    ::zserio::detail::write(writer, static_cast<::zserio::VarSize>(::zserio::convertSizeToUInt32(choiceTag)));
    <@union_switch "union_write_field", "view.zserioChoiceTag()"/>
</#if>
}

<#macro union_read_field field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}data.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>();
${I}<#if field.compound??>(void)</#if>::zserio::detail::read<#rt>
        <#if field.array??><<@array_type_full_name fullName, field/>></#if>(<#t>
        reader, data.get<${fullName}::ChoiceTag::<@choice_tag_name field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters field/>);
</#macro>
template <>
View<${fullName}> read(::zserio::BitStreamReader&<#if fieldList?has_content> reader</#if>, ${fullName}& data<#rt>
<#list parameterList as parameter>
        <#lt>,
        <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
</#list>
        <#lt>)
{
    View<${fullName}> view(data<#rt>
<#list parameterList as parameter>
            <#lt>,
            <#nt><@parameter_view_arg_name parameter/><#rt>
</#list>
            <#lt>);
<#if fieldList?has_content>

    ::zserio::VarSize choiceTag;
    ::zserio::detail::read(reader, choiceTag);
    <@union_switch "union_read_field", "choiceTag"/>
</#if>

    return view;
}
<@namespace_end ["detail"]/>
<@namespace_end ["zserio"]/>
<@namespace_begin ["std"]/>

size_t hash<${fullName}>::operator()(const ${fullName}&<#if fieldList?has_content> value</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#if fieldList?has_content>
    result = ::zserio::calcHashCode(result, static_cast<const ${fullName}::Base&>(value));
</#if>
    return static_cast<size_t>(result);
}

<#macro union_hash_field field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}result = ::zserio::calcHashCode(result, view.${field.getterName}());
</#macro>
size_t hash<::zserio::View<${fullName}>>::operator()(<#rt>
        <#lt>const ::zserio::View<${fullName}>&<#if fieldList?has_content> view</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#if fieldList?has_content>
    <@union_switch "union_hash_field", "view.zserioChoiceTag()"/>

</#if>
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
