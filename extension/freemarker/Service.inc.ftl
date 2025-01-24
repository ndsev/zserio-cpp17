<#macro service_arg_type_name typeInfo>
    <#if typeInfo.isBytes>
        ::zserio::BytesView<#t>
    <#else>
        const ${typeInfo.typeFullName}&<#t>
    </#if>
</#macro>
