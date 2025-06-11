<#macro choice_expression_switch memberActionMacroName noMatchMacroName switchExpression indent=1 packed=false>
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
        <@.vars[noMatchMacroName] fullName, indent+1/>
            </#if>
${I}}
        </#if>
    </#if>
</#macro>

<#macro choice_switch memberActionMacroName noMatchMacroName switchExpression indent=1 packed=false>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}switch (${switchExpression})
${I}{
    <#list fieldList as field>
${I}case ${fullName}::Tag::<@choice_tag_name field/>:
        <#assign caseCode><@.vars[memberActionMacroName] field, indent+1, packed/></#assign>
        ${caseCode}<#t>
        <#if !caseCode?contains("return ")>
${I}    break;
        </#if>
    </#list>
${I}default:
        <@.vars[noMatchMacroName] fullName, indent+1/>
${I}}
</#macro>

<#macro choice_compare_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return (lhs.${field.getterName}() == rhs.${field.getterName}());
</#macro>

<#macro choice_compare_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return true;
</#macro>

<#macro choice_less_than_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return false;
</#macro>

<#macro choice_less_than_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return (lhs.${field.getterName}() < rhs.${field.getterName}());
</#macro>

<#macro choice_validate_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}// check choice case
${I}if (view.zserioChoiceTag() != ${fullName}::Tag::<@choice_tag_name member.field/>)
${I}{
${I}    throw ChoiceCaseException("Wrong case set in choice '${name}' (") << static_cast<size_t>(view.zserioChoiceTag()) <<
${I}            " != " << static_cast<size_t>(${fullName}::Tag::<@choice_tag_name member.field/>) << ")!";
${I}}
    <@field_check_constraint member.field, indent/>
${I}detail::validate<@array_template_args member.field/>(view.${member.field.getterName}(), "'${name}.${member.field.name}'"<#rt>
        <#if member.field.array?? && member.field.array.viewIndirectLength??>
        , static_cast<size_t>(${member.field.array.viewIndirectLength})<#t>
        </#if>
        <#lt>);
    <#else>
${I}// empty
${I}if (view.zserioChoiceTag() != ${fullName}::Tag::ZSERIO_UNDEFINED)
${I}{
${I}    throw ChoiceCaseException("Wrong case set in choice '${name}' (should be empty)!");
${I}}
    </#if>
</#macro>

<#macro choice_no_match_exception fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw ChoiceCaseException("No match in choice ${fullName}!");
</#macro>

<#macro choice_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}break;
</#macro>

<#macro choice_bitsizeof_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}endBitPosition += detail::bitSizeOf<@array_suffix field, packed/><@array_template_args field/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>view.${field.getterName}(), endBitPosition);
</#macro>

<#macro choice_write_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}detail::write<@array_suffix field, packed/><@array_template_args field/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>writer, view.${field.getterName}());
</#macro>

<#macro choice_read_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}data.emplace<${fullName}::Tag::<@choice_tag_name member.field/>>();
${I}<#if member.field.compound??>(void)</#if>detail::read<@array_read_suffix member.field, packed/><#rt>
        <@array_read_template_args fullName, member.field/>(<#t>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        reader, data.get<${fullName}::Tag::<@choice_tag_name member.field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters member.field/>);
    <@field_check_constraint member.field, indent/>
    <#else>
${I}// empty
    </#if>
</#macro>

<#macro choice_template_read_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}data.template emplace<${fullName}::Tag::<@choice_tag_name member.field/>>();
${I}<#if member.field.compound??>(void)</#if>detail::read<@array_read_suffix member.field, packed/><#rt>
        <@array_read_template_args fullName, member.field/>(<#t>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        reader, data.template get<${fullName}::Tag::<@choice_tag_name member.field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters member.field/>);
    <@field_check_constraint member.field, indent/>
    <#else>
${I}// empty
    </#if>
</#macro>

<#macro choice_hash_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}result = ::zserio::calcHashCode(result, view.${field.getterName}());
</#macro>

<#macro choice_init_context field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field_needs_packing_context(field)>
${I}detail::initContext(<@packing_context field/>, view.${field.getterName}());
    </#if>
</#macro>

<#macro choice_initialize_offsets_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}endBitPosition += detail::<#if field.compound??>initializeOffsets<#else>bitSizeOf</#if><#rt>
        <@array_suffix field, packed/><@array_template_args field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>view.${field.getterName}(), endBitPosition);
</#macro>
