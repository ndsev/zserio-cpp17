<#function sql_table_has_non_virtual_field fields>
    <#list fields as field>
        <#if !field.isVirtual>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>

<#function sql_db_has_without_rowid_table fields>
    <#list fields as field>
        <#if field.isWithoutRowIdTable>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>

<#macro sql_db_table_getter_name field>
    get${field.name?cap_first}<#t>
</#macro>

<#macro sql_db_table_name field>
    tableName${field.name?cap_first}<#t>
</#macro>

<#macro sql_field_member_name field>
    m_${field.name?uncap_first}<#t>
</#macro>

<#macro sql_field_argument_name field>
    ${field.name}_<#t>
</#macro>

<#macro sql_row_member_name field>
    ${field.name?uncap_first}<#t>
</#macro>

<#macro sql_row_view_field_type_name field>
    Optional<<#t>
    <#if field.typeInfo.isSimple && !field.typeInfo.isDynamicBitField>
        ${field.typeInfo.typeFullName}<#t>
    <#elseif field.typeInfo.isString>
        StringView<#t>
    <#elseif field.typeInfo.isExtern>
        ${types.bitBufferView.name}<#t>
    <#elseif field.typeInfo.isBytes>
        BytesView<#t>
    <#else>
        View<${field.typeInfo.typeFullName}><#t>
    </#if>
    ><#t>
</#macro>

<#macro sql_parameter_provider_getter_name parameter>
    ${parameter.expression?uncap_first}<#t>
</#macro>

<#macro sql_parameter_provider_return_type parameter>
    <#if parameter.typeInfo.isSimple && !parameter.typeInfo.isDynamicBitField>
        ${parameter.typeInfo.typeFullName}<#t>
    <#elseif parameter.typeInfo.isString>
        StringView<#t>
    <#elseif parameter.typeInfo.isExtern>
        ${types.bitBufferView.name}<#t>
    <#elseif parameter.typeInfo.isBytes>
        BytesView<#t>
    <#else>
        ::zserio::View<${parameter.typeInfo.typeFullName}><#t>
    </#if>
</#macro>

<#macro sql_parameter_variable_type parameter>
    <#if parameter.typeInfo.isSimple>
        const ${parameter.typeInfo.typeFullName}<#t>
    <#else>
        ${parameter.typeInfo.typeFullName}&<#t>
    </#if>
</#macro>

<#macro sql_db_table_parameter_provider_getter field>
    get${field.name?cap_first}ParameterProvider()<#t>
</#macro>

<#function sql_db_needs_parameter_provider fields>
    <#list fields as field>
        <#if field.hasExplicitParameters>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>
