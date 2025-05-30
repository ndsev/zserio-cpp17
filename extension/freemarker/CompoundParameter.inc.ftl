<#macro parameter_view_type_name parameter>
    <#if parameter.typeInfo.isSimple && !parameter.typeInfo.isDynamicBitField>
        ${parameter.typeInfo.typeFullName}<#t>
    <#elseif parameter.typeInfo.isString>
        ::std::string_view<#t>
    <#elseif parameter.typeInfo.isExtern>
        ${types.bitBufferView.name}<#t>
    <#elseif parameter.typeInfo.isBytes>
        BytesView<#t>
    <#else>
        <#if parameter.typeInfo.isTemplateParameter>
            view_type_t<${parameter.typeInfo.typeFullName}><#t>
        <#else>
            View<${parameter.typeInfo.typeFullName}><#t>
        </#if>
    </#if>
</#macro>

<#macro parameter_view_arg_name parameter>
    ${parameter.name}_<#t>
</#macro>

<#macro parameter_view_member_name parameter>
    m_${parameter.name}_<#t>
</#macro>

<#macro parameter_traits parameters indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}using Params = std::tuple<
    <#list parameters as parameter>
${I}        ${parameter.typeInfo.typeFullName}<#if parameter?has_next>,<#else>>;</#if>
    </#list>
</#macro>