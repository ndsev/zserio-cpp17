<#macro function_return_type_name function>
    <#if (function.returnTypeInfo.isSimple || function.returnTypeInfo.isEnum || function.returnTypeInfo.isBitmask) && !function.returnTypeInfo.isDynamicBitField>
        ${function.returnTypeInfo.typeFullName}<#t>
    <#elseif function.returnTypeInfo.isString>
        ::std::string_view<#t>
    <#elseif function.returnTypeInfo.isExtern>
        ${types.bitBufferView.name}<#t>
    <#elseif function.returnTypeInfo.isBytes>
        BytesView<#t>
    <#else>
        View<${function.returnTypeInfo.typeFullName}><#t>
    </#if>
</#macro>
