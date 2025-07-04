<#macro type_info typeInfo>
    ::zserio::typeInfo<${typeInfo.typeFullName}, ${types.allocator.default}>()<#t>
</#macro>

<#macro info_array_type infoType infoListSize>
    <#if infoListSize != 0>
        ::std::array<${infoType}, ${infoListSize}><#t>
    <#else>
<#--
We have to use Span instead of empty std::array, because MSVC complains that the infoType class doesn't have
user declared default constructor.
See https://developercommunity.visualstudio.com/t/Empty-std::array-constructor-requires-on/382468.
-->
        ::zserio::Span<${infoType}><#t>
    </#if>
</#macro>

<#macro field_info_array_var varName fieldList indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}static const <@info_array_type "::zserio::BasicFieldInfo<AllocatorType>", fieldList?size/> ${varName}<#rt>
    <#if fieldList?has_content>
        <#lt> = {
        <#list fieldList as field>
        <@field_info field, field?has_next, indent+1/>
        </#list>
${I}};
    <#else>
        <#lt>;
    </#if>
</#macro>

<#macro parameter_info_array_var varName parameterList indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}static const <@info_array_type "::zserio::BasicParameterInfo<AllocatorType>", parameterList?size/> ${varName}<#rt>
    <#if parameterList?has_content>
        <#lt> = {
        <#list parameterList as parameter>
        <@parameter_info parameter, parameter?has_next, indent+1/>
        </#list>
${I}};
    <#else>
        <#lt>;
    </#if>
</#macro>

<#macro function_info_array_var varName functionList indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}static const <@info_array_type "::zserio::BasicFunctionInfo<AllocatorType>", functionList?size/> ${varName}<#rt>
    <#if functionList?has_content>
        <#lt> = {
        <#list functionList as function>
        <@function_info function, function?has_next, indent+1/>
        </#list>
${I}};
    <#else>
        <#lt>;
    </#if>
</#macro>

<#macro case_info_array_var varName caseMemberList defaultMember indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}static const <@info_array_type "::zserio::BasicCaseInfo<AllocatorType>", caseMemberList?size + (defaultMember?has_content)?then(1, 0)/> ${varName}<#rt>
    <#if caseMemberList?has_content || defaultMember?has_content>
        <#lt> = {
        <@case_info_array caseMemberList, defaultMember, indent+1/>
${I}};
    <#else>
        <#lt>;
    </#if>
</#macro>

<#macro item_info_array_var varName items indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}static const <@info_array_type "::zserio::ItemInfo", items?size/> ${varName}<#rt>
    <#if items?has_content>
        <#lt> = {
        <#list items as item>
        <@item_info item.schemaName, item.value, item.isDeprecated, item.isRemoved, item?has_next, indent+1/>
        </#list>
${I}};
    <#else>
        <#lt>;
    </#if>
</#macro>

<#macro field_info field comma indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}::zserio::BasicFieldInfo<AllocatorType>{
${I}    "${field.name}", // schemaName
    <#if (field.optional?? && field.optional.isRecursive) || (field.array?? && field.array.isRecursive)>
${I}    <@field_info_recursive_type_info_var_name field/>, // typeInfo
    <#else>
${I}    <@type_info field.typeInfo/>, // typeInfo
    </#if>
    <#if field_info_type_arguments_count(field) != 0>
${I}    <@field_info_type_arguments_var_name field/>, // typeArguments
    <#else>
${I}    {}, // typeArguments
    </#if>
${I}    <#if field.isExtended>true<#else>false</#if>, // isExtended
${I}    <#if field.alignmentValue??>"${field.alignmentValue?j_string}"<#else>{}</#if>, // alignment
${I}    <#if field.offset??>"${field.offset.getter?j_string}"<#else>{}</#if>, // offset
<#-- We need to use j_string builtin here because initializer can be string literal with quotes. -->
${I}    <#if field.initializer??>"${field.initializer?j_string}"<#else>{}</#if>, // initializer
${I}    <#if field.optional??>true<#else>false</#if>, // isOptional
${I}    <#if field.optional?? && field.optional.clause??><#rt>
                <#lt>"${field.optional.clause?j_string}"<#else>{}</#if>, // optionalClause
${I}    <#if field.constraint??>"${field.constraint.expression?j_string}"<#else>{}</#if>, // constraint
${I}    <#if field.array??>true<#else>false</#if>, // isArray
${I}    <#if field.array?? && field.array.length??><#rt>
                <#lt>"${field.array.length?j_string}"<#else>{}</#if>, // arrayLength
${I}    <#if field.array?? && field.array.isPacked>true<#else>false</#if>, // isPacked
${I}    <#if field.array?? && field.array.isImplicit>true<#else>false</#if> // isImplicit
${I}}<#if comma>,</#if>
</#macro>

<#macro field_info_recursive_type_info_var_name field>
    ${field.name}RecursiveTypeInfo<#t>
</#macro>

<#macro field_info_recursive_type_info_var field indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.optional?? && field.optional.isRecursive>
${I}static const ::zserio::detail::RecursiveTypeInfo<AllocatorType> <@field_info_recursive_type_info_var_name field/>(
${I}        &typeInfo<${field.typeInfo.typeFullName}, AllocatorType>);
    <#elseif field.array?? && field.array.isRecursive>
${I}static const ::zserio::detail::RecursiveTypeInfo<AllocatorType> <@field_info_recursive_type_info_var_name field/>(
${I}        &typeInfo<${field.typeInfo.typeFullName}, AllocatorType>);
    </#if>
</#macro>

<#function field_info_type_arguments_count field>
    <#if field.array??>
        <#if field.compound??>
            <#return field.compound.instantiatedParameters?size/>
        <#elseif field.typeInfo.isDynamicBitField>
            <#return 1/>
        </#if>
    <#elseif field.compound??>
        <#return field.compound.instantiatedParameters?size/>
    <#elseif field.typeInfo.isDynamicBitField>
        <#return 1/>
    </#if>
    <#return 0/>
</#function>

<#macro field_info_type_arguments_var_name field>
    ${field.name}TypeArguments<#t>
</#macro>

<#macro field_info_type_arguments_var field indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#local count=field_info_type_arguments_count(field)>
    <#if count != 0>
${I}static const ::std::array<::std::string_view, ${count}> <@field_info_type_arguments_var_name field/> = {
        <#if field.array??>
            <#if field.compound??>
        <@field_info_compound_type_arguments field.compound.instantiatedParameters, indent+1/>
            <#elseif field.typeInfo.isDynamicBitField>
${I}    "${field.dynamicBitFieldLength.expression?j_string}"
            </#if>
        <#elseif field.compound??>
        <@field_info_compound_type_arguments field.compound.instantiatedParameters, indent+1/>
        <#elseif field.typeInfo.isDynamicBitField>
${I}    "${field.dynamicBitFieldLength.expression?j_string}"
        </#if>
    };
    </#if>
</#macro>

<#macro field_info_compound_type_arguments parameters indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#list parameters as parameter>
${I}"${parameter.expression?j_string}"<#if parameter?has_next>,</#if>
    </#list>
</#macro>

<#macro parameter_info parameter comma indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}::zserio::BasicParameterInfo<AllocatorType>{
${I}    "${parameter.name}",
${I}    <@type_info parameter.typeInfo/>
${I}}<#if comma>,</#if>
</#macro>

<#macro function_info function comma indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}::zserio::BasicFunctionInfo<AllocatorType>{
${I}    "${function.schemaName}",
${I}    <@type_info function.returnTypeInfo/>,
${I}    "${function.resultExpression?j_string}"
${I}}<#if comma>,</#if>
</#macro>

<#macro case_info_case_expressions_var_name index>
    case${index}Expressions<#t>
</#macro>

<#macro case_info_case_expressions_var caseMember index indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}static const std::array<::std::string_view, ${caseMember.expressionList?size}> <#rt>
            <#lt><@case_info_case_expressions_var_name index/> = {
    <#list caseMember.expressionList as caseExpression>
${I}    "${caseExpression?j_string}"<#if caseExpression?has_next>,</#if>
    </#list>
${I}};
</#macro>

<#macro case_info_array caseMemberList defaultMember indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#local fieldIndex=0>
    <#list caseMemberList as caseMember>
${I}::zserio::BasicCaseInfo<AllocatorType>{
${I}    <@case_info_case_expressions_var_name caseMember?index/>,
${I}    <#if caseMember.field??>&fields[${fieldIndex}]<#local fieldIndex+=1><#else>nullptr</#if>
${I}}<#if caseMember?has_next || defaultMember?has_content>,</#if>
    </#list>
    <#if defaultMember?has_content>
${I}::zserio::BasicCaseInfo<AllocatorType>{
${I}    {}, <#if defaultMember.field??>&fields[${fieldIndex}]<#local fieldIndex+=1><#else>nullptr</#if>
${I}}
    </#if>
</#macro>

<#macro item_info name value isDeprecated isRemoved comma indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}::zserio::ItemInfo{ "${name}", <#rt>
        <#lt>static_cast<uint64_t>(${value}), <#if isDeprecated>true<#else>false</#if>, <#rt>
        <#lt><#if isRemoved>true<#else>false</#if>}<#if comma>,</#if>
</#macro>

<#macro column_info field comma indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}::zserio::BasicColumnInfo<AllocatorType>{
${I}    "${field.name}", // schemaName
${I}    <@type_info field.typeInfo/>, // typeInfo
    <#if column_info_type_arguments_count(field) != 0>
${I}    <@field_info_type_arguments_var_name field/>, // typeArguments
    <#else>
${I}    {}, // typeArguments
    </#if>
    <#if field.sqlTypeData.isTemplate>
${I}    ::zserio::detail::ColumnTraits<${field.typeInfo.typeFullName}>::TYPE_NAME, // sqlTypeName
    <#else>
${I}    "${field.sqlTypeData.name}", // sqlTypeName
    </#if>
${I}    <#if field.sqlConstraint??>${field.sqlConstraint}<#else>{}</#if>, // sqlConstraint
${I}    <#if field.isVirtual>true<#else>false</#if>, // isVirtual
${I}}<#if comma>,</#if>
</#macro>

<#function column_info_type_arguments_count field>
    <#if field.typeInfo.isDynamicBitField>
        <#return 1/>
    <#else>
        <#return field.typeParameters?size/>
    </#if>
</#function>

<#macro column_info_type_arguments_var field indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#local count=column_info_type_arguments_count(field)>
    <#if count != 0>
${I}static const ::std::array<::std::string_view, ${count}> <@field_info_type_arguments_var_name field/> = {
        <#if field.typeInfo.isDynamicBitField>
${I}    "${field.dynamicBitFieldLength.expression?j_string}"
        <#else>
        <@column_info_compound_type_arguments field.typeParameters, indent+1/>
        </#if>
${I}};
    </#if>
</#macro>

<#macro column_info_compound_type_arguments parameters indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#list parameters as parameter>
${I}    "<#if parameter.isExplicit>explicit </#if>${parameter.expression?j_string}"<#if parameter?has_next>, </#if>
    </#list>
</#macro>

<#macro table_info field comma indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}::zserio::BasicTableInfo<AllocatorType>{ <#rt>
        <#lt>"${field.name}", <@type_info field.typeInfo/> }<#if comma>,</#if>
</#macro>

<#macro message_info message comma indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}::zserio::BasicMessageInfo<AllocatorType>{
${I}    "${message.name}", // schemaName
${I}    <@type_info message.typeInfo/>, // typeInfo
${I}    <#if message.isPublished>true<#else>false</#if>, // isPublished
${I}    <#if message.isSubscribed>true<#else>false</#if>, // isSubscribed
${I}    ${message.topicDefinition}, // topic
${I}}<#if comma>,</#if>
</#macro>

<#macro method_info method comma indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}::zserio::BasicMethodInfo<AllocatorType>{
${I}    "${method.name}", // schemaName
${I}    <@type_info method.responseTypeInfo/>, // responseTypeInfo
${I}    <@type_info method.requestTypeInfo/>, // requestTypeInfo
${I}}<#if comma>,</#if>
</#macro>

<#macro template_info_template_name_var varName templateInstantiation indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if templateInstantiation?has_content>
${I}static const ::std::string_view ${varName} = "${templateInstantiation.templateName}";
    <#else>
${I}static const ::std::string_view ${varName};
    </#if>
</#macro>

<#macro template_info_template_arguments_var varName templateInstantiation indent=1>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if templateInstantiation?has_content>
        <#if templateInstantiation.templateArgumentTypeInfos?has_content>
${I}static const ::std::array<::zserio::BasicTemplateArgumentInfo<AllocatorType><#rt>
            <#lt>, ${templateInstantiation.templateArgumentTypeInfos?size}> ${varName} = {
            <#list templateInstantiation.templateArgumentTypeInfos as typeInfo>
${I}    ::zserio::BasicTemplateArgumentInfo<AllocatorType>{<@type_info typeInfo/>}<#if typeInfo?has_next>,</#if>
            </#list>
        <#else>
${I}static const ::std::array<::zserio::BasicTemplateArgumentInfo<AllocatorType><#rt>
            <#lt>, ${templateInstantiation.templateParameters?size}> ${varName} = {
            <#list templateInstantiation.templateParameters as templateParameter>
${I}    ::zserio::BasicTemplateArgumentInfo<AllocatorType>{<#rt>
                <#lt>::zserio::typeInfo<${templateParameter}, AllocatorType>()}<#if templateParameter?has_next>,</#if>
            </#list>
        </#if>
${I}};
    <#else>
    <#-- See comment in info_array_type macro.  -->
${I}static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;
    </#if>
</#macro>
