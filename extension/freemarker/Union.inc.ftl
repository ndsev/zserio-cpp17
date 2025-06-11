<#macro union_switch fieldActionMacroName noMatchMacroName switchExpression indent=1 packed=false>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}switch (${switchExpression})
${I}{
    <#list fieldList as field>
${I}case ${fullName}::Tag::<@choice_tag_name field/>:
        <#assign caseCode><@.vars[fieldActionMacroName] field, indent+1, packed/></#assign>
        ${caseCode}<#t>
        <#if !caseCode?contains("return ")>
${I}    break;
        </#if>
    </#list>
${I}default:
        <@.vars[noMatchMacroName] fullName, indent+1/>
${I}}
</#macro>

<#macro union_compare_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return true;
</#macro>

<#macro union_compare_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return (lhs.${field.getterName}() == rhs.${field.getterName}());
</#macro>

<#macro union_less_than_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return false;
</#macro>

<#macro union_less_than_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return (lhs.${field.getterName}() < rhs.${field.getterName}());
</#macro>

<#macro union_validate_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <@field_check_constraint field, indent/>
${I}detail::validate<@array_template_args field/>(view.${field.getterName}(), "'${name}.${field.name}'"<#rt>
        <#if field.array?? && field.array.viewIndirectLength??>
        , static_cast<size_t>(${field.array.viewIndirectLength})<#t>
        </#if>
        <#lt>);
</#macro>

<#macro union_validate_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw UnionCaseException("No case set in union '${fullName}'!");
</#macro>

<#macro union_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}break;
</#macro>

<#macro union_bitsizeof_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}endBitPosition += detail::bitSizeOf<@array_suffix field, packed/><@array_template_args field/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>view.${field.getterName}(), endBitPosition);
</#macro>

<#macro union_write_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}detail::write<@array_suffix field, packed/><@array_template_args field/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>writer, view.${field.getterName}());
</#macro>

<#macro union_read_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}data.emplace<${fullName}::Tag::<@choice_tag_name field/>>();
${I}<#if field.compound??>(void)</#if>detail::read<@array_read_suffix field, packed/><#rt>
        <@array_read_template_args fullName, field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        reader, data.get<${fullName}::Tag::<@choice_tag_name field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters field/>);
    <@field_check_constraint field, indent/>
</#macro>

<#macro union_template_read_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}data.template emplace<${fullName}::Tag::<@choice_tag_name field/>>();
${I}<#if field.compound??>(void)</#if>detail::read<@array_read_suffix field, packed/><#rt>
        <@array_read_template_args fullName, field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        reader, data.template get<${fullName}::Tag::<@choice_tag_name field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters field/>);
    <@field_check_constraint field, indent/>
</#macro>

<#macro union_read_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw UnionCaseException("Unexpected choice tag during read of union '${fullName}'!");
</#macro>

<#macro union_hash_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}result = ::zserio::calcHashCode(result, view.${field.getterName}());
</#macro>

<#macro union_init_context field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field_needs_packing_context(field)>
${I}detail::initContext(<@packing_context field/>, view.${field.getterName}());
    </#if>
</#macro>

<#macro union_initialize_offsets_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#-- TODO[TEMPLATES]: Do we need to call initializeOffsets for template parameter types? -->
${I}endBitPosition += detail::<#if field.compound??>initializeOffsets<#else>bitSizeOf</#if><#rt>
        <@array_suffix field, packed/><@array_template_args field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>view.${field.getterName}(), endBitPosition);
</#macro>
