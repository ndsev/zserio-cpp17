<#macro function_return_type_name function>
    <#if function.returnTypeInfo.isSimple && !function.returnTypeInfo.isDynamicBitField>
        ${function.returnTypeInfo.typeFullName}<#t>
    <#elseif function.returnTypeInfo.isString>
        StringView<#t>
    <#elseif function.returnTypeInfo.isExtern>
        ${types.bitBufferView.name}<#t>
    <#elseif function.returnTypeInfo.isBytes>
        BytesView<#t>
    <#else>
        View<${function.returnTypeInfo.typeFullName}><#t>
    </#if>
</#macro>

<#macro function_result_expression function>
    <#if function.returnTypeInfo.isNumeric>static_cast<${function.returnTypeInfo.typeFullName}::ValueType>(</#if><#t>
            <#lt>${function.resultExpression}<#if function.returnTypeInfo.isNumeric>)</#if><#t>
</#macro>