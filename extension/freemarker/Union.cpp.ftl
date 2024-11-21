<#include "FileHeader.inc.ftl">
<#include "CompoundField.inc.ftl">
<#include "CompoundFunction.inc.ftl">
<#include "CompoundParameter.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/SizeConvertUtil.h>
#include <zserio/UnionCaseException.h>
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
<@array_traits_definition fullName, fieldList/>

View<${fullName}>::View(const ${fullName}& data<#rt>
<#list parameterList as parameter>
        <#lt>,
        <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
</#list>
        <#lt>) noexcept :
        m_data(data)<#if parameterList?has_content>,</#if>
<#list parameterList as parameter>
        <@parameter_view_member_name parameter/>(<@parameter_view_arg_name parameter/>)<#if parameter?has_next>,</#if>
</#list>
{}

View<${fullName}>::View(const ${fullName}& data,
        const View&<#if parameterList?has_content> other</#if>) noexcept :
        m_data(data)<#if parameterList?has_content>,</#if>
<#list parameterList as parameter>
        <@parameter_view_member_name parameter/>(other.${parameter.getterName}())<#if parameter?has_next>,</#if>
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

<@field_view_type_full_name fullName, field/> View<${fullName}>::${field.getterName}() const
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
<#list functionList as function>

<@function_return_type_name function/> View<${fullName}>::${function.name}() const
{
    return <@function_result_expression function/>;
}
</#list>

<#macro union_switch fieldActionMacroName noMatchMacroName switchExpression indent=1 packed=false>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}switch (${switchExpression})
${I}{
    <#list fieldList as field>
${I}case ${fullName}::ChoiceTag::<@choice_tag_name field/>:
        <#assign caseCode><@.vars[fieldActionMacroName] field, indent+1, packed/></#assign>
        ${caseCode}<#t>
        <#if !caseCode?contains("return")>
${I}    break;
        </#if>
    </#list>
${I}default:
        <@.vars[noMatchMacroName] name, indent+1/>
${I}}
</#macro>
<#macro union_no_match name indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw CppRuntimeException("No case set in union ${fullName}!");
</#macro>
<#macro union_compare_field field indent packed>
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
    if (lhs.zserioChoiceTag() != rhs.zserioChoiceTag())
    {
        return false;
    }

    <@union_switch "union_compare_field", "union_no_match", "lhs.zserioChoiceTag()"/>
<#else>
    return true;
</#if>
}

<#macro union_less_than_field field indent packed>
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
    if (lhs.zserioChoiceTag() != rhs.zserioChoiceTag())
    {
        return lhs.zserioChoiceTag() < rhs.zserioChoiceTag();
    }

    <@union_switch "union_less_than_field", "union_no_match", "lhs.zserioChoiceTag()"/>
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

<#macro union_validate_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <@array_check_length field, indent/>
${I}validate(view.${field.getterName}(), "'${name}.${field.name}'");
</#macro>
<#macro union_validate_no_match name indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw UnionCaseException("No case set in union '${name}'!");
</#macro>
template <>
void validate(const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> view</#if>, ::std::string_view)
{
<#list parameterList>
    <#items as parameter>
    validate(view.${parameter.getterName}(), "'${name}.${parameter.name}'");
    </#items>

</#list>
<#if fieldList?has_content>
    <@union_switch "union_validate_field", "union_validate_no_match", "view.zserioChoiceTag()"/>
</#if>
}

<#macro union_bitsizeof_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}endBitPosition += bitSizeOf<@array_packed_suffix field, packed/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>view.${field.getterName}(), endBitPosition);
</#macro>
template <>
BitSize bitSizeOf(const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
<#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    endBitPosition += bitSizeOf(fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag())));
    <@union_switch "union_bitsizeof_field", "union_no_match", "view.zserioChoiceTag()"/>

    return endBitPosition - bitPosition;
<#else>
    return 0;
</#if>
}

<#macro union_write_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}write<@array_packed_suffix field, packed/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>writer, view.${field.getterName}());
</#macro>
template <>
void write(BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#if fieldList?has_content>
    write(writer, fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    <@union_switch "union_write_field", "union_no_match", "view.zserioChoiceTag()"/>
</#if>
}

<#macro union_read_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}data.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>();
${I}<#if field.compound??>(void)</#if>read<@array_packed_suffix field, packed/><#rt>
        <#if field.array??><<@array_type_full_name fullName, field/>></#if>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        reader, data.get<${fullName}::ChoiceTag::<@choice_tag_name field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters field/>);
</#macro>
template <>
View<${fullName}> read(BitStreamReader&<#if fieldList?has_content> reader</#if>, ${fullName}& data<#rt>
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

    VarSize choiceTag;
    read(reader, choiceTag);
    <@union_switch "union_read_field", "union_no_match", "static_cast<${fullName}::ChoiceTag>(choiceTag + 1)"/>
</#if>

    return view;
}
<#if isPackable && usedInPackedArray>

<#macro union_init_context field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field_needs_packing_context(field)>
${I}initContext(<@packing_context field/>, view.${field.getterName}());
    <#else>
${I}// empty
    </#if>
</#macro>
template <>
void initContext(PackingContext<${fullName}>&<#if fieldList?has_content> packingContext</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
    <#if fieldList?has_content>
    initContext(packingContext.zserioChoiceTag, fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag())));
    <@union_switch "union_init_context", "union_no_match", "view.zserioChoiceTag()", 1, true/>
    </#if>
}

template <>
BitSize bitSizeOf(PackingContext<${fullName}>&<#if fieldList?has_content> packingContext</#if>, <#rt>
        const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#t>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
    <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    endBitPosition += bitSizeOf(packingContext.zserioChoiceTag,
            fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag())));
    <@union_switch "union_bitsizeof_field", "union_no_match", "view.zserioChoiceTag()", 1, true/>

    return endBitPosition - bitPosition;
    <#else>
    return 0;
    </#if>
}

template <>
void write(PackingContext<${fullName}>&<#if fieldList?has_content> packingContext</#if>, <#rt>
        BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#t>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#if fieldList?has_content>
    write(packingContext.zserioChoiceTag, writer,
            fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag())));
    <@union_switch "union_write_field", "union_no_match", "view.zserioChoiceTag()", 1, true/>
</#if>
}

template <>
void read(PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>, <#rt>
        BitStreamReader&<#if fieldList?has_content> reader</#if>, ${fullName}& data<#t>
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

    VarSize choiceTag;
    read(packingContext.zserioChoiceTag, reader, choiceTag);
    <@union_switch "union_read_field", "union_no_match", "choiceTag", 1, true/>
</#if>
    (void)view;
}
</#if>
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

<#macro union_hash_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}result = ::zserio::calcHashCode(result, view.${field.getterName}());
</#macro>
<#macro union_hash_no_match name indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw ::zserio::CppRuntimeException("No case set in union ${fullName}!");
</#macro>
size_t hash<::zserio::View<${fullName}>>::operator()(<#rt>
        <#lt>const ::zserio::View<${fullName}>&<#if fieldList?has_content> view</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#if fieldList?has_content>
    <@union_switch "union_hash_field", "union_hash_no_match", "view.zserioChoiceTag()"/>

</#if>
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
