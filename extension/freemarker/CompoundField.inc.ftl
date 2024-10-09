<#macro field_data_type_name field>
    <#if field.array??>
        <@vector_type_name field.array.elementTypeInfo.typeFullName/><#t>
    <#else>
        ${field.typeInfo.typeFullName}<#t>
    </#if>
</#macro>

<#macro field_data_arg_name field>
    ${field.name}_<#t>
</#macro>