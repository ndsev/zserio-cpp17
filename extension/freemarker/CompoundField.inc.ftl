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
        <#if field.typeInfo.isSimple && !field.typeInfo.isDynamicBitField>
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

<#macro field_view_type_full_name compoundName field>
    <#if field.array??>
        <@array_type_full_name compoundName, field/><#t>
    <#else>
        <@field_view_type_name field/><#t>
    </#if>
</#macro>

<#macro field_view_getter_name field>
    get${field.name?cap_first}<#t>
</#macro>

<#macro field_view_parameters field>
    <#if field.array??>
        <#if array_needs_owner(field)>
            , *this<#t>
        </#if>
    <#else>
        <#if field.compound??>
            <#list field.compound.instantiatedParameters as instantiatedParameter>
                , <#t>
                <#if instantiatedParameter.typeInfo.isNumeric>
                    <#if instantiatedParameter.typeInfo.isDynamicBitField>
                        View<${instantiatedParameter.typeInfo.typeFullName}>(<#t>
                                typeCast<${instantiatedParameter.typeInfo.typeFullName}>(<#t>
                    <#else>
                        typeCast<${instantiatedParameter.typeInfo.typeFullName}>(<#t>
                    </#if>
                </#if>
                ${instantiatedParameter.expression}<#t>
                <#if instantiatedParameter.typeInfo.isNumeric>
                    <#if instantiatedParameter.typeInfo.isDynamicBitField>
                        ), 64)<#t>
                    <#else>
                        )<#t>
                    </#if>
                </#if>
            </#list>
        <#elseif field.typeInfo.isDynamicBitField>
            , static_cast<uint8_t>(${field.dynamicBitLength.expression})<#t>
        </#if>
    </#if>
</#macro>

<#macro field_view_view_indirect_parameters field>
    <#if field.array??>
        <#if array_needs_owner(field)>
            , view<#t>
        </#if>
        <#if field.array.viewIndirectLength??>
            , ${field.array.viewIndirectLength}<#t>
        </#if>
    <#else>
        <#if field.compound??>
            <#list field.compound.instantiatedParameters as instantiatedParameter>
                , <#t>
                <#if instantiatedParameter.typeInfo.isNumeric>
                    <#if instantiatedParameter.typeInfo.isDynamicBitField>
                        View<${instantiatedParameter.typeInfo.typeFullName}>(<#t>
                                typeCast<${instantiatedParameter.typeInfo.typeFullName}>(<#t>
                    <#else>
                        typeCast<${instantiatedParameter.typeInfo.typeFullName}>(<#t>
                    </#if>
                </#if>
                ${instantiatedParameter.viewIndirectExpression}<#t>
                <#if instantiatedParameter.typeInfo.isNumeric>
                    <#if instantiatedParameter.typeInfo.isDynamicBitField>
                        ), 64)<#t>
                    <#else>
                        )<#t>
                    </#if>
                </#if>
            </#list>
        <#elseif field.typeInfo.isDynamicBitField>
            , static_cast<uint8_t>(${field.dynamicBitLength.viewIndirectExpression})<#t>
        </#if>
    </#if>
</#macro>

<#macro field_view_owner_indirect_parameters field>
    <#if field.compound??>
        <#list field.compound.instantiatedParameters as instantiatedParameter>
            , <#t>
            <#if instantiatedParameter.typeInfo.isNumeric>
                <#if instantiatedParameter.typeInfo.isDynamicBitField>
                    View<${instantiatedParameter.typeInfo.typeFullName}>(<#t>
                            typeCast<${instantiatedParameter.typeInfo.typeFullName}>(<#t>
                <#else>
                    typeCast<${instantiatedParameter.typeInfo.typeFullName}>(<#t>
                </#if>
            </#if>
            ${instantiatedParameter.ownerIndirectExpression}<#t>
            <#if instantiatedParameter.typeInfo.isNumeric>
                <#if instantiatedParameter.typeInfo.isDynamicBitField>
                    ), 64)<#t>
                <#else>
                    )<#t>
                </#if>
            </#if>
        </#list>
    <#elseif field.typeInfo.isDynamicBitField>
        , static_cast<uint8_t>(${field.dynamicBitLength.ownerIndirectExpression})<#t>
    </#if>
</#macro>

<#macro choice_tag_name field>
    CHOICE_${field.name}<#t>
</#macro>

<#macro array_traits_name field>
    Zserio${field.name?cap_first}ArrayTraits<#t>
</#macro>

<#macro array_type_name field>
    ::zserio::Array<<@field_data_type_name field/>, <@array_type_enum field/><#t>
            <#if array_needs_custom_traits(field)>, <@array_traits_name field/></#if>><#t>
</#macro>

<#macro array_type_full_name compoundName field>
    ::zserio::Array<<@field_data_type_name field/>, <@array_type_enum field/><#t>
            <#if array_needs_custom_traits(field)>, View<${compoundName}>::<@array_traits_name field/></#if>><#t>
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

<#function array_needs_custom_traits field>
    <#return field.array?? && ((field.compound?? && field.compound.parameters?has_content) ||
            field.typeInfo.isDynamicBitField)>
</#function>

<#function array_needs_owner field>
    <#if field.array??>
        <#if field.compound??>
            <#list field.compound.instantiatedParameters as instantiatedParameter>
                <#if instantiatedParameter.needsOwner>
                    <#return true>
                </#if>
            </#list>
        <#elseif field.typeInfo.isDynamicBitField>
            <#return field.dynamicBitLength.needsOwner>
        </#if>
    </#if>
    <#return false>
</#function>

<#function array_needs_index field>
    <#if field.array??>
        <#if field.compound??>
            <#list field.compound.instantiatedParameters as instantiatedParameter>
                <#if instantiatedParameter.needsIndex>
                    <#return true>
                </#if>
            </#list>
        <#elseif field.typeInfo.isDynamicBitField>
            <#return field.dynamicBitLength.needsIndex>
        </#if>
    </#if>
    <#return false>
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
