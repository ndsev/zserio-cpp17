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
        <#elseif field.typeInfo.isTemplateParameter>
            view_type_t<${field.typeInfo.typeFullName}><#t>
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

<#macro sql_table_view_parameters field parameterProviderVarName rowVarName="rowView">
    <#if field.typeParameters?has_content>
        <#list field.typeParameters as parameter>
            , <#t>
            <#if parameter.isExplicit>
                ${parameterProviderVarName}.<@sql_parameter_provider_getter_name parameter/>(${rowVarName})<#t>
            <#else>
                <#if parameter.typeInfo.isNumeric>
                ${parameter.typeInfo.typeFullName}(static_cast<${parameter.typeInfo.typeFullName}::ValueType>(<#t>
                </#if>
                ${parameter.expression}<#t>
                <#if parameter.typeInfo.isNumeric>
                ))<#t>
                </#if>
            </#if>
        </#list>
    <#elseif field.typeInfo.isDynamicBitField>
        , static_cast<uint8_t>(${field.dynamicBitFieldLength.expression})<#t>
    </#if>
</#macro>

<#macro sql_table_range_check_field name field types indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}if (<#if field.sqlRangeCheckData.checkLowerBound>rangeCheckValue < lowerBound || </#if>rangeCheckValue > upperBound)
${I}{
${I}    const auto rowKeyValuesHolder = getRowKeyValuesHolder(statement);
${I}    ${types.string.name} errorMessage = ${types.string.name}("Value ", get_allocator_ref());
${I}    errorMessage += ::zserio::toString(rangeCheckValue, get_allocator_ref());
${I}    errorMessage += " of ${name}.${field.name} exceeds the range of ";
${I}    errorMessage += ::zserio::toString(lowerBound, get_allocator_ref());
${I}    errorMessage += "..";
${I}    errorMessage += ::zserio::toString(upperBound, get_allocator_ref());
${I}    continueValidation = validationObserver.reportError(m_name, "${field.name}",
${I}            getRowKeyValues(rowKeyValuesHolder),
${I}            ::zserio::IValidationObserver::VALUE_OUT_OF_RANGE, errorMessage);
${I}    return false;
${I}}
</#macro>

<#macro sqlite_type_field field>
    <#if field.sqlTypeData.isInteger>
        SQLITE_INTEGER<#t>
    <#elseif field.sqlTypeData.isReal>
        SQLITE_FLOAT<#t>
    <#elseif field.sqlTypeData.isBlob>
        SQLITE_BLOB<#t>
    <#else>
        SQLITE_TEXT<#t>
    </#if>
</#macro>

<#function sql_table_has_primary_key fieldList>
    <#list fieldList as field>
        <#if field.isPrimaryKey>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>

<#function sql_table_holder_needs_statement fieldList>
    <#local hasPrimaryKeyField=false/>
    <#list fieldList as field>
        <#if field.isPrimaryKey>
            <#local hasPrimaryKeyField=true/>
            <#if !field.sqlTypeData.isBlob>
                <#return true>
            </#if>
         </#if>
    </#list>
    <#return !hasPrimaryKeyField>
</#function>
