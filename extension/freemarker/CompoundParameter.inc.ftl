<#macro parameter_view_type_name parameter>
    <#if parameter.typeInfo.isSimple && !parameter.typeInfo.isDynamicBitField>
        ${parameter.typeInfo.typeFullName}<#t>
    <#elseif parameter.typeInfo.isString>
        StringView<#t>
    <#elseif parameter.typeInfo.isExtern>
        ${types.bitBufferView.name}<#t>
    <#elseif parameter.typeInfo.isBytes>
        BytesView<#t>
    <#else>
        View<${parameter.typeInfo.typeFullName}><#t>
    </#if>
</#macro>

<#macro parameter_view_arg_name parameter>
    ${parameter.name}_<#t>
</#macro>

<#macro parameter_view_member_name parameter>
    m_${parameter.name}_<#t>
</#macro>
