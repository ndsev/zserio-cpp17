<#function sql_table_has_non_virtual_field fieldList>
    <#list fieldList as field>
        <#if !field.isVirtual>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>

<#function sql_db_has_without_rowid_table fieldList>
    <#list fieldList as field>
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
    ${field.name?uncap_first}_<#t>
</#macro>

<#macro sql_row_member_name field>
    ${field.name?uncap_first}<#t>
</#macro>

<#macro sql_field_bit_size_name field>
    ${field.name?uncap_first}BitSize<#t>
</#macro>

<#macro sql_row_view_field_type_name field>
    <#local fieldViewTypeName>
        <#if field.typeInfo.isSimple && !field.typeInfo.isDynamicBitField>
            ${field.typeInfo.typeFullName}<#t>
        <#elseif field.typeInfo.isString>
            ::std::string_view<#t>
        <#elseif field.typeInfo.isExtern>
            ${types.bitBufferView.name}<#t>
        <#elseif field.typeInfo.isBytes>
            BytesView<#t>
        <#else>
            View<${field.typeInfo.typeFullName}><#t>
        </#if>
    </#local>
    <@optional_type_name fieldViewTypeName/>
</#macro>

<#macro sql_parameter_provider_getter_name parameter>
    ${parameter.expression?uncap_first}<#t>
</#macro>

<#macro sql_parameter_provider_return_type parameter>
    <#if parameter.typeInfo.isSimple && !parameter.typeInfo.isDynamicBitField>
        ${parameter.typeInfo.typeFullName}<#t>
    <#elseif parameter.typeInfo.isString>
        ::std::string_view<#t>
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

<#function sql_db_needs_parameter_provider fieldList>
    <#list fieldList as field>
        <#if field.hasExplicitParameters>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>
