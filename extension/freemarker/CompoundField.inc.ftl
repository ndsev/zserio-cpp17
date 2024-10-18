<#macro field_data_type_name field>
    <#if field.array??>
        <@vector_type_name field.typeInfo.typeFullName/><#t>
    <#else>
        ${field.typeInfo.typeFullName}<#t>
    </#if>
</#macro>

<#macro field_data_arg_name field>
    ${field.name}_<#t>
</#macro>

<#macro field_data_member_name field>
    ${field.name}<#t>
</#macro>

<#macro field_view_type_name field>
    <#if field.array??>
        <@array_type_name field/><#t>
    <#else>
        <#if field.typeInfo.isSimple>
            ${field.typeInfo.typeFullName}<#t>
        <#elseif field.typeInfo.isString>
            ::std::string_view<#t>
        <#elseif field.typeInfo.isExtern>
            const ${types.bitBuffer.name}&<#t>
        <#elseif field.typeInfo.isBytes>
            ::zserio::BytesView<#t>
        <#else>
            View<${field.typeInfo.typeFullName}><#t>
        </#if>
    </#if>
</#macro>

<#macro field_view_getter_name field>
    get${field.name?cap_first}<#t>
</#macro>

<#macro field_view_parameters field>
    <#if field.compound??>
        <#list field.compound.instantiatedParameters as instantiatedParameter>
            , ${instantiatedParameter.expression}<#t>
        </#list>
    </#if>
</#macro>

<#macro field_view_indirect_parameters field>
    <#if field.compound??>
        <#list field.compound.instantiatedParameters as instantiatedParameter>
            , ${instantiatedParameter.viewIndirectExpression}<#t>
        </#list>
    </#if>
</#macro>

<#macro choice_tag_name field>
    CHOICE_${field.name}<#t>
</#macro>

<#macro array_type_name field>
    ::zserio::Array<<@field_data_type_name field/>, <@array_type_enum field/>><#t>
</#macro>

<#macro array_type_enum field>
    ::zserio::ArrayType::<#t>
    <#local arrayType="">
    <#if field.offset?? && field.offset.containsIndex>
        <#local arrayType="ALIGNED">
    </#if>
    <#if !field.array.length??>
        <#if field.array.isImplicit>
            <#local arrayType="IMPLICIT">
        <#else>
            <#if arrayType?has_content><#local arrayType+="_AUTO"><#else><#local arrayType="AUTO"></#if>
        </#if>
    </#if>
    <#if !arrayType?has_content>
        <#local arrayType="NORMAL">
    </#if>
    ${arrayType}<#t>
</#macro>

<#function array_needs_owner field>
    <#return field.array?? && field.compound?? && field.compound.parameters?has_content>
</#function>

<#function field_needs_allocator field>
    <#return field.typeInfo.needsAllocator || field.optional?? || field.array??>
</#function>

<#function fields_need_allocator fieldList>
    <#list fieldList as field>
        <#if field_needs_allocator(field)>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>
