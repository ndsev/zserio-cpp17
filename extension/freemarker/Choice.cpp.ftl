<#include "FileHeader.inc.ftl">
<#include "CompoundField.inc.ftl">
<#include "CompoundFunction.inc.ftl">
<#include "CompoundParameter.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/ChoiceCaseException.h>
#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
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
        m_data(&data)<#if parameterList?has_content>,</#if>
<#list parameterList as parameter>
        <@parameter_view_member_name parameter/>(<@parameter_view_arg_name parameter/>)<#if parameter?has_next>,</#if>
</#list>
{}

View<${fullName}>::View(const ${fullName}& data,
        const View&<#if parameterList?has_content> other</#if>) noexcept :
        m_data(&data)<#if parameterList?has_content>,</#if>
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
<#list fieldList as field>

<@field_view_type_full_name fullName, field/> View<${fullName}>::${field.getterName}() const
{
    <#if !field.array?? && !field.typeInfo.isDynamicBitField && field.typeInfo.isSimple>
    <#-- field which does not need View -->
    return get<${fullName}::ChoiceTag::<@choice_tag_name field/>>(*m_data);
    <#else>
    <#-- field which needs View -->
    return <@field_view_type_name field/>{get<${fullName}::ChoiceTag::<@choice_tag_name field/>>(*m_data)<#rt>
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

${fullName}::ChoiceTag View<${fullName}>::zserioChoiceTag() const
{
    return m_data->index();
}

const ${fullName}& View<${fullName}>::zserioData() const
{
    return *m_data;
}

<#macro choice_switch memberActionMacroName noMatchMacroName switchExpression indent=1 packed=false>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if canUseNativeSwitch>
${I}switch (${switchExpression})
${I}{
        <#list caseMemberList as caseMember>
            <#list caseMember.expressionList as expression>
${I}case ${expression}:
            </#list>
            <#assign caseCode><@.vars[memberActionMacroName] caseMember, indent+1, packed/></#assign>
        ${caseCode}<#t>
            <#if !caseCode?contains("return ")>
${I}    break;
            </#if>
        </#list>
        <#if !isDefaultUnreachable>
${I}default:
            <#if defaultMember??>
                <#assign defaultCode><@.vars[memberActionMacroName] defaultMember, indent+1, packed/></#assign>
        ${defaultCode}<#t>
                <#if !defaultCode?contains("return ")>
${I}    break;
                </#if>
            <#else>
        <@.vars[noMatchMacroName] name, indent+1/>
            </#if>
        </#if>
${I}}
    <#else>
        <#list caseMemberList as caseMember>
            <#if caseMember?has_next || !isDefaultUnreachable>
${I}<#if caseMember?index != 0>else </#if>if (<#rt>
                <#list caseMember.expressionList as expression>
                    <#lt>(${switchExpression}) == (${expression})<#sep> ||</#sep><#rt>
                </#list>
                <#lt>)
            <#else>
${I}else
            </#if>
${I}{
        <@.vars[memberActionMacroName] caseMember, indent+1, packed/>
${I}}
        </#list>
        <#if !isDefaultUnreachable>
${I}else
${I}{
            <#if defaultMember??>
        <@.vars[memberActionMacroName] defaultMember, indent+1, packed/>
            <#else>
        <@.vars[noMatchMacroName] name, indent+1/>
            </#if>
${I}}
        </#if>
    </#if>
</#macro>
<#macro choice_compare_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}return (lhs.${member.field.getterName}() == rhs.${member.field.getterName}());
    <#else>
${I}return true; // empty
    </#if>
</#macro>
<#macro choice_no_match name indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw CppRuntimeException("No match in choice ${fullName}!");
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
    <@choice_switch "choice_compare_member", "choice_no_match", lhsIndirectSelectorExpression/>
<#else>
    return true;
</#if>
}

<#macro choice_less_than_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}return (lhs.${member.field.getterName}() < rhs.${member.field.getterName}());
    <#else>
${I}return false; // empty
    </#if>
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
    <@choice_switch "choice_less_than_member", "choice_no_match", lhsIndirectSelectorExpression/>
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

<#macro choice_validate_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}// check choice case
${I}if (view.zserioChoiceTag() != ${fullName}::ChoiceTag::<@choice_tag_name member.field/>)
${I}{
${I}    throw ChoiceCaseException("Wrong case set in choice '${name}' (") << static_cast<size_t>(view.zserioChoiceTag()) <<
${I}            " != " << static_cast<size_t>(${fullName}::ChoiceTag::<@choice_tag_name member.field/>) << ")!";
${I}}
    <@field_check_constraint member.field, indent/>
${I}validate(view.${member.field.getterName}(), "'${name}.${member.field.name}'");
    <#else>
${I}// empty
    </#if>
</#macro>
<#macro choice_validate_no_match name indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw ChoiceCaseException("No match in choice ${fullName}!");
</#macro>
template <>
void validate(const View<${fullName}>& view, ::std::string_view)
{
<#list parameterList as parameter>
    validate(view.${parameter.getterName}(), "'${name}.${parameter.name}'");
</#list>
<#if fieldList?has_content>
    <@choice_switch "choice_validate_member", "choice_validate_no_match", viewIndirectSelectorExpression/>
</#if>
}

<#macro choice_bitsizeof_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}endBitPosition += bitSizeOf<@array_packed_suffix member.field, packed/>(<#rt>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        <#lt>view.${member.field.getterName}(), endBitPosition);
    <#else>
${I}// empty
    </#if>
</#macro>
template <>
BitSize bitSizeOf(const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
<#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    <@choice_switch "choice_bitsizeof_member", "choice_no_match", viewIndirectSelectorExpression/>

    return endBitPosition - bitPosition;
<#else>
    return 0;
</#if>
}

<#macro choice_write_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}write<@array_packed_suffix member.field, packed/>(<#rt>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        <#lt>writer, view.${member.field.getterName}());
    <#else>
${I}// empty
    </#if>
</#macro>
template <>
void write(BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#if fieldList?has_content>
    <@choice_switch "choice_write_member", "choice_no_match", viewIndirectSelectorExpression/>
</#if>
}

<#macro choice_read_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}data.emplace<${fullName}::ChoiceTag::<@choice_tag_name member.field/>>(<#rt>
        <#lt><#if member.field.typeInfo.needsAllocator>data.get_allocator()</#if>);
${I}<#if member.field.compound??>(void)</#if>read<@array_packed_suffix member.field, packed/><#rt>
        <#if member.field.array??><<@array_type_full_name fullName, member.field/>></#if>(<#t>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        reader, data.get<${fullName}::ChoiceTag::<@choice_tag_name member.field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters member.field/>);
    <@field_check_constraint member.field, indent/>
    <#else>
${I}// empty
    </#if>
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
    <@choice_switch "choice_read_member", "choice_no_match", viewIndirectSelectorExpression/>
</#if>

    return view;
}
<#if isPackable && usedInPackedArray>

<#macro choice_init_context member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field?? && field_needs_packing_context(member.field)>
${I}initContext(<@packing_context member.field/>, view.${member.field.getterName}());
    <#else>
${I}// empty
    </#if>
</#macro>
template <>
void initContext(PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if needs_packing_context(fieldList)> view</#if>)
{
    <#if fieldList?has_content>
    <@choice_switch "choice_init_context", "choice_no_match", viewIndirectSelectorExpression, 1, true/>
    </#if>
}

template <>
BitSize bitSizeOf(PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>, <#rt>
        const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#t>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
    <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    <@choice_switch "choice_bitsizeof_member", "choice_no_match", viewIndirectSelectorExpression, 1, true/>

    return endBitPosition - bitPosition;
    <#else>
    return 0;
    </#if>
}

template <>
void write(PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>, <#rt>
        BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#t>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#if fieldList?has_content>
    <@choice_switch "choice_write_member", "choice_no_match", viewIndirectSelectorExpression, 1, true/>
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
    <@choice_switch "choice_read_member", "choice_no_match", viewIndirectSelectorExpression, 1, true/>
</#if>
    (void)view;
}
</#if>
<#if containsOffset>

<#macro choice_initialize_offsets_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}endBitPosition += <#if member.field.compound??>initializeOffsets<#else>bitSizeOf</#if><#rt>
        <@array_packed_suffix member.field, packed/>(<#t>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        <#lt>view.${member.field.getterName}(), endBitPosition);
    <#else>
${I}// empty
    </#if>
</#macro>
BitSize OffsetsInitializer<${fullName}>::initialize(
        const View<${fullName}>&<#if fieldList?has_content> view</#if><#rt>
        <#lt>, BitSize<#if fieldList?has_content> bitPosition</#if>)
{
    <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    <@choice_switch "choice_initialize_offsets_member", "choice_no_match", viewIndirectSelectorExpression/>

    return endBitPosition - bitPosition;
    <#else>
    return 0;
    </#if>
}
    <#if isPackable && usedInPackedArray>

BitSize OffsetsInitializer<${fullName}>::initialize(
        PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>,
        const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
        <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    <@choice_switch "choice_initilize_offsets_member", "choice_no_match", viewIndirectSelectorExpression, 1, true/>

    return endBitPosition - bitPosition;
        <#else>
    return 0;
        </#if>
}
    </#if>
</#if>
<@namespace_end ["zserio", "detail"]/>
<@namespace_begin ["std"]/>

size_t hash<${fullName}>::operator()(const ${fullName}&<#if fieldList?has_content> value</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#if fieldList?has_content>
    result = ::zserio::calcHashCode(result, static_cast<const ${fullName}::Base&>(value));
</#if>
    return static_cast<size_t>(result);
}

<#macro choice_hash_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}result = ::zserio::calcHashCode(result, view.${member.field.getterName}());
    <#else>
${I}// empty
    </#if>
</#macro>
<#macro choice_hash_no_match name indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw ::zserio::CppRuntimeException("No match in choice ${fullName}!");
</#macro>
size_t hash<::zserio::View<${fullName}>>::operator()(<#rt>
        <#lt>const ::zserio::View<${fullName}>&<#if parameterList?has_content || fieldList?has_content> view</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#list parameterList as parameter>
    result = ::zserio::calcHashCode(result, view.${parameter.getterName}());
</#list>
<#if fieldList?has_content>
    <@choice_switch "choice_hash_member", "choice_hash_no_match", viewIndirectSelectorExpression/>

</#if>
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
