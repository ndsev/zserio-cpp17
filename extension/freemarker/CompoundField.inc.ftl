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

<#macro field_view_local_name field>
    ${field.name}_<#t>
</#macro>

<#macro field_view_type_name field>
    <#if field.array??>
        <@array_type_name field/><#t>
    <#else>
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
    </#if>
</#macro>

<#macro field_view_type_full_name compoundName field>
    <#if field.array??>
        <@array_type_full_name compoundName, field/><#t>
    <#else>
        <@field_view_type_name field/><#t>
    </#if>
</#macro>

<#macro field_view_parameters field>
    <#if field.array??>
        <#if array_needs_owner(field)>
            , *this<#t>
        </#if>
        <#if field.array.length??>
            , static_cast<size_t>(${field.array.length})<#t>
        </#if>
    <#else>
        <#if field.compound??>
            <#list field.compound.instantiatedParameters as instantiatedParameter>
                , <#t>
                <#if instantiatedParameter.typeInfo.isNumeric>
                    <#if instantiatedParameter.typeInfo.isDynamicBitField>
                        View<${instantiatedParameter.typeInfo.typeFullName}>(<#t>
                                ${instantiatedParameter.typeInfo.typeFullName}(<#t>
                                        static_cast<${instantiatedParameter.typeInfo.typeFullName}::ValueType>(<#t>
                    <#else>
                        ${instantiatedParameter.typeInfo.typeFullName}(<#t>
                                static_cast<${instantiatedParameter.typeInfo.typeFullName}::ValueType>(<#t>
                    </#if>
                </#if>
                ${instantiatedParameter.expression}<#t>
                <#if instantiatedParameter.typeInfo.isNumeric>
                    <#if instantiatedParameter.typeInfo.isDynamicBitField>
                        )), 64)<#t>
                    <#else>
                        ))<#t>
                    </#if>
                </#if>
            </#list>
        <#elseif field.typeInfo.isDynamicBitField>
            , static_cast<uint8_t>(${field.dynamicBitFieldLength.expression})<#t>
        </#if>
    </#if>
</#macro>

<#macro field_view_view_indirect_parameters field>
    <#if field.array??>
        <#if array_needs_owner(field)>
            , view<#t>
        </#if>
        <#if field.array.viewIndirectLength??>
            , static_cast<size_t>(${field.array.viewIndirectLength})<#t>
        </#if>
    <#else>
        <#if field.compound??>
            <#list field.compound.instantiatedParameters as instantiatedParameter>
                , <#t>
                <#if instantiatedParameter.typeInfo.isNumeric>
                    <#if instantiatedParameter.typeInfo.isDynamicBitField>
                        View<${instantiatedParameter.typeInfo.typeFullName}>(<#t>
                                ${instantiatedParameter.typeInfo.typeFullName}(<#t>
                                        static_cast<${instantiatedParameter.typeInfo.typeFullName}::ValueType>(<#t>
                    <#else>
                        ${instantiatedParameter.typeInfo.typeFullName}(<#t>
                                static_cast<${instantiatedParameter.typeInfo.typeFullName}::ValueType>(<#t>
                    </#if>
                </#if>
                ${instantiatedParameter.viewIndirectExpression}<#t>
                <#if instantiatedParameter.typeInfo.isNumeric>
                    <#if instantiatedParameter.typeInfo.isDynamicBitField>
                        )), 64)<#t>
                    <#else>
                        ))<#t>
                    </#if>
                </#if>
            </#list>
        <#elseif field.typeInfo.isDynamicBitField>
            , static_cast<uint8_t>(${field.dynamicBitFieldLength.viewIndirectExpression})<#t>
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
                            ${instantiatedParameter.typeInfo.typeFullName}(<#t>
                                    static_cast<${instantiatedParameter.typeInfo.typeFullName}::ValueType>(<#t>
                <#else>
                    ${instantiatedParameter.typeInfo.typeFullName}(<#t>
                            static_cast<${instantiatedParameter.typeInfo.typeFullName}::ValueType>(<#t>
                </#if>
            </#if>
            ${instantiatedParameter.ownerIndirectExpression}<#t>
            <#if instantiatedParameter.typeInfo.isNumeric>
                <#if instantiatedParameter.typeInfo.isDynamicBitField>
                    )), 64)<#t>
                <#else>
                    ))<#t>
                </#if>
            </#if>
        </#list>
    <#elseif field.typeInfo.isDynamicBitField>
        , static_cast<uint8_t>(${field.dynamicBitFieldLength.ownerIndirectExpression})<#t>
    </#if>
</#macro>

<#function num_extended_fields fieldList>
    <#local numExtended=0/>
    <#list fieldList as field>
        <#if field.isExtended>
            <#local numExtended=numExtended+1/>
        </#if>
    </#list>
    <#return numExtended>
</#function>

<#macro choice_tag_name field>
    CHOICE_${field.name}<#t>
</#macro>

<#macro array_traits_name field>
    Zserio${field.name?cap_first}ArrayTraits<#t>
</#macro>

<#macro array_traits_declaration compoundFullName fieldList>
    <#list fieldList as field>
        <#if array_needs_custom_traits(field)>
    struct <@array_traits_name field/>
    {
            <#if array_needs_owner(field)>
        using OwnerType = View<${compoundFullName}>;

            </#if>
        static View<${field.typeInfo.typeFullName}> at(<#rt>
                <#lt>const <#if array_needs_owner(field)>OwnerType&<#else>detail::DummyArrayOwner&</#if> owner,
                const ${field.typeInfo.typeFullName}& element, size_t index);

        static void read(BitStreamReader& reader, <#rt>
                <#lt>const <#if array_needs_owner(field)>OwnerType&<#else>detail::DummyArrayOwner&</#if> owner,
                ${field.typeInfo.typeFullName}& element, size_t index);
            <#if field.isPackable && (field.array.isPacked || usedInPackedArray)>

        static void read(<@packing_context_type_name field, true/>& packingContext, BitStreamReader& reader,
                const <#if array_needs_owner(field)>OwnerType&<#else>detail::DummyArrayOwner&</#if>  owner, <#rt>
                <#lt>${field.typeInfo.typeFullName}& element, size_t index);
            </#if>
    };

        </#if>
    </#list>
</#macro>

<#macro array_traits_definition compoundFullName fieldList>
    <#list fieldList as field>
        <#if array_needs_custom_traits(field)>

View<${field.typeInfo.typeFullName}> View<${compoundFullName}>::<@array_traits_name field/>::at(<#rt>
        <#lt>const <#if array_needs_owner(field)>OwnerType& owner<#else>detail::DummyArrayOwner&</#if>,
        const ${field.typeInfo.typeFullName}& element, size_t<#if array_needs_index(field)> index</#if>)
{
    return View<${field.typeInfo.typeFullName}>(element<@field_view_owner_indirect_parameters field/>);
}

void View<${compoundFullName}>::<@array_traits_name field/>::read(BitStreamReader& reader, <#rt>
                <#lt>const <#if array_needs_owner(field)>OwnerType& owner<#else>detail::DummyArrayOwner&</#if>,
                ${field.typeInfo.typeFullName}& element, size_t<#if array_needs_index(field)> index</#if>)
{
    (void)detail::read(reader, element<@field_view_owner_indirect_parameters field/>);
}
            <#if field.isPackable && (field.array.isPacked || usedInPackedArray)>

void View<${compoundFullName}>::<@array_traits_name field/>::read(<@packing_context_type_name field, true/>& packingContext, BitStreamReader& reader,
        const <#if array_needs_owner(field)>OwnerType& owner<#else>detail::DummyArrayOwner&</#if>, <#rt>
            <#lt>${field.typeInfo.typeFullName}& element, size_t<#if array_needs_index(field)> index</#if>)
{
    detail::read(packingContext, reader, element<@field_view_owner_indirect_parameters field/>);
}
            </#if>
        </#if>
    </#list>
</#macro>

<#macro array_type_name field>
    Array<<@field_data_type_name field/>, <@array_type_enum field/><#t>
            <#if field.usedAsOffset>, ::zserio::ArrayStorage::MUTABLE</#if><#t>
            <#if array_needs_custom_traits(field)>
                <#if !field.usedAsOffset>
            , ::zserio::ArrayStorage::IMMUTABLE<#t>
                </#if>
            , <@array_traits_name field/><#t>
            </#if>
            ><#t>
</#macro>

<#macro array_type_full_name compoundName field>
    Array<<@field_data_type_name field/>, <@array_type_enum field/><#t>
            <#if field.usedAsOffset>, ::zserio::ArrayStorage::MUTABLE</#if><#t>
            <#if array_needs_custom_traits(field)>
                <#if !field.usedAsOffset>
            , ::zserio::ArrayStorage::IMMUTABLE<#t>
                </#if>
            , View<${compoundName}>::<@array_traits_name field/><#t>
            </#if>
            ><#t>
</#macro>

<#macro array_type_enum field>
    ArrayType::<#t>
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

<#macro array_packed_suffix field packed>
    <#if field.array?? && field.isPackable && (packed || field.array.isPacked)>
        Packed<#t>
    </#if>
</#macro>

<#macro packing_context_member_name field>
    ${field.name}<#t>
</#macro>

<#macro packing_context_type_name field needsNamespace=false>
    <#if field.compound??>
        <#if needsNamespace>detail::</#if>PackingContext<${field.typeInfo.typeFullName}><#t>
    <#else>
        DeltaContext<#t>
    </#if>
</#macro>

<#macro packing_context field>
    <#if field.optional?? && field.optional.isRecursive>
    packingContext<#t>
    <#else>
    packingContext.<@packing_context_member_name field/><#t>
    </#if>
</#macro>

<#function field_needs_packing_context field>
    <#if field.isPackable && !field.array??>
        <#return true>
    </#if>
    <#return false>
</#function>

<#function needs_packing_context fieldList>
    <#list fieldList as field>
        <#if field_needs_packing_context(field)>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>

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
            <#return field.dynamicBitFieldLength.needsOwner>
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
            <#return field.dynamicBitFieldLength.needsIndex>
        </#if>
    </#if>
    <#return false>
</#function>
