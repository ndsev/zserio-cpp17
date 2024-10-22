<#macro field_data_type_name field>
    <#if field.array??>
        <@vector_type_name field.array.elementTypeInfo.typeFullName/><#t>
    <#else>
        ${field.typeInfo.typeFullName}<#t>
    </#if>
</#macro>

<#macro structure_field_data_type_name field>
    <#if field.optional??>
        ::zserio::Optional<<@field_data_type_name field/>><#t>
    <#else>
        <@field_data_type_name field/><#t>
    </#if>
</#macro>

<#macro field_data_arg_name field>
    ${field.name}_<#t>
</#macro>

<#macro field_data_member_name field>
    ${field.name}<#t>
</#macro>

<#macro field_view_type_name field>
    <#if field.typeInfo.isSimple>
        ${field.typeInfo.typeFullName}<#t>
    <#elseif field.typeInfo.isString>
        ::std::string_view<#t>
    <#elseif field.typeInfo.isExtern>
        const ${types.bitBuffer.name}&<#t>
    <#elseif field.typeInfo.isBytes>
        ::zserio::Span<const uint8_t><#t>
    <#else>
        View<${field.typeInfo.typeFullName}><#t>
    </#if>
</#macro>

<#macro structure_field_view_type_name field>
    <#if field.optional??>
        ::zserio::Optional<<@field_view_type_name field/>><#t>
    <#else>
        <@field_view_type_name field/><#t>
    </#if>
</#macro>

<#macro field_view_getter_name field>
    get${field.name?cap_first}<#t>
</#macro>

<#macro field_view_parameters field>
    <#if field.compound??>
        <#list field.compound.instantiatedParameters as instantiatedParameter>
            ${instantiatedParameter.viewIndirectExpression}<#if instantiatedParameter?has_next>, </#if><#t>
        </#list>
    </#if>
</#macro>

<#macro choice_tag_name field>
    CHOICE_${field.name}<#t>
</#macro>

<#function has_optional_field fieldList>
    <#list fieldList as field>
        <#if field.optional??>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>

<#function needs_allocator fieldList>
    <#list fieldList as field>
        <#if field.needsAllocator>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>
