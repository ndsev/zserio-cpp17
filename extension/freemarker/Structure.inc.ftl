<#include "CompoundField.inc.ftl">
<#include "CompoundParameter.inc.ftl">
<#include "CompoundFunction.inc.ftl">
<#macro structure_field_initializer field>
    <#if field.initializer??>
        <#local initializer>
            <#if field.typeInfo.isNumeric>static_cast<${field.typeInfo.typeFullName}::ValueType>(</#if><#t>
            ${field.initializer}<#t>
            <#if field.typeInfo.isNumeric>)</#if><#t>
        </#local>
        <#if field.optional??>
            <#local initializer>
                ::std::in_place, allocator, ${initializer}<#t>
            </#local>
        <#elseif field.typeInfo.isTemplateParameter>
            <#local initializer>
                ::zserio::constructWithAllocator<<@structure_field_data_type_name field/>>(allocator, ${initializer})<#t>
            </#local>
        <#elseif field.typeInfo.needsAllocator>
            <#local initializer>
                ${initializer}, allocator<#t>
            </#local>
        </#if>
        ${initializer}<#t>
    <#elseif field.typeInfo.isTemplateParameter>
        ::zserio::constructWithAllocator<<@structure_field_data_type_name field/>>(allocator)<#t>
    <#elseif field.typeInfo.needsAllocator || field.optional?? || field.array??>
        allocator<#t>
    </#if>
</#macro>

<#macro structure_field_view_getter field, indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.isExtended>
${I}if (m_data-><@field_data_member_name field/>.isPresent())
${I}{
        <@structure_field_view_getter_optional field, indent+1/>
${I}}
${I}else
${I}{
${I}    auto <@field_view_local_name field/> = <#rt>
        <#if field.optional??><#-- TODO[Mi-L@]: What if non-present optional in data has value? -->
            <@structure_field_view_type_name field/>(::std::nullopt, <#t>
                    <#lt>m_data-><@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>get_allocator());
        <#else>
            <#lt><@structure_field_view_getter_inner field, indent+1/>;
        </#if>
${I}    <@field_view_local_name field/>.setPresent(false);
${I}    return <@field_view_local_name field/>;
${I}}
    <#else>
    <@structure_field_view_getter_optional field, indent/>
    </#if>
</#macro>

<#macro structure_field_view_getter_optional field, indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.optional??>
${I}if (m_data-><@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>has_value())
${I}{
${I}    return <@structure_field_view_getter_inner field, indent+1/>;
${I}}
${I}else
${I}{
${I}    return <@structure_field_view_type_name field/>(::std::nullopt, <#rt>
                <#lt>m_data-><@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>get_allocator());
${I}}
    <#else>
${I}return <@structure_field_view_getter_inner field, indent/>;
    </#if>
</#macro>

<#macro structure_field_view_getter_inner field, indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#lt><@structure_field_view_type_name field/>{<#rt>
            <#if field.optional??>::std::in_place, <#t>
            m_data-><@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>get_allocator(), </#if><#t>
            <#if field.isExtended>*</#if><#if field.optional??>*</#if><#t>
            m_data-><@field_data_member_name field/><@field_view_parameters field/>}<#t>
</#macro>

<#function structure_has_optional_field fieldList>
    <#list fieldList as field>
        <#if field.optional??>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>

<#function structure_has_recursive_optional_field fieldList>
    <#list fieldList as field>
        <#if field.optional?? && field.optional.isRecursive>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>

<#macro structure_field_ctor_type_name field>
    <#local fieldTypeName>
        <@field_data_type_name field/><#t>
    </#local>
    <#if field.optional??>
        <@optional_type_name fieldTypeName/><#t>
    <#else>
        ${fieldTypeName}<#t>
    </#if>
</#macro>

<#macro structure_field_data_type_name field>
    <#local fieldTypeName>
        <@field_data_type_name field/><#t>
    </#local>
    <#if field.isExtended>
        ::zserio::Extended<<#t>
    </#if>
    <#if field.optional??>
        <@optional_type_name fieldTypeName/><#t>
    <#else>
        ${fieldTypeName}<#t>
    </#if>
    <#if field.isExtended>
        ><#t>
    </#if>
</#macro>

<#macro structure_field_view_type_name field>
    <#local fieldViewTypeName>
        <@field_view_type_name field/><#t>
    </#local>
    <#if field.isExtended>
        ::zserio::Extended<<#t>
    </#if>
    <#if field.optional??>
        <@optional_type_name fieldViewTypeName/><#t>
    <#else>
        ${fieldViewTypeName}<#t>
    </#if>
    <#if field.isExtended>
        ><#t>
    </#if>
</#macro>

<#macro structure_field_view_type_full_name compoundName field>
    <#local fieldViewTypeFullName>
        <@field_view_type_full_name compoundName, field/><#t>
    </#local>
    <#if field.isExtended>
        ::zserio::Extended<<#t>
    </#if>
    <#if field.optional??>
        <@optional_type_name fieldViewTypeFullName/><#t>
    <#else>
        ${fieldViewTypeFullName}<#t>
    </#if>
    <#if field.isExtended>
        ><#t>
    </#if>
</#macro>

<#macro structure_offset_setter_name field>
    Zserio${field.name?cap_first}OffsetSetter<#t>
</#macro>

<#macro structure_offset_setters_declaration compoundFullName fieldList>
    <#list fieldList as field>
        <#if field.offset?? && field.offset.containsIndex>
    class <@structure_offset_setter_name field/>
    {
    public:
        explicit <@structure_offset_setter_name field/>(const View<${compoundFullName}>& owner) :
                m_owner(owner)
        {}

        void setOffset(size_t index, BitSize byteOffset) const;

    private:
        const View<${compoundFullName}>& m_owner;
    };

        </#if>
    </#list>
</#macro>

<#macro structure_offset_setters_definition compoundFullName fieldList>
    <#list fieldList as field>
        <#if field.offset?? && field.offset.containsIndex>

void ObjectTraits<${compoundFullName}>::<@structure_offset_setter_name field/>::setOffset(<#rt>
        <#lt>size_t index, BitSize byteOffset) const
{
    ${field.offset.ownerIndirectSetter} = static_cast<${field.offset.typeInfo.typeFullName}::ValueType>(byteOffset);
}
        </#if>
    </#list>
</#macro>

<#macro structure_offset_setters_template compoundFullName fieldList>
    <#list fieldList as field>
        <#if field.offset?? && field.offset.containsIndex>
    class <@structure_offset_setter_name field/>
    {
    public:
        explicit <@structure_offset_setter_name field/>(const View<${compoundFullName}>& owner) :
                m_owner(owner)
        {}

        void setOffset(size_t index, BitSize byteOffset) const
        {
            ${field.offset.ownerIndirectSetter} =
                    static_cast<typename ${field.offset.typeInfo.typeFullName}::ValueType>(byteOffset);
        }

    private:
        const View<${compoundFullName}>& m_owner;
    };

        </#if>
    </#list>
</#macro>

<#function structure_field_needs_allocator field>
    <#return field.typeInfo.needsAllocator || field.typeInfo.isTemplateParameter || field.optional?? || field.array??>
</#function>

<#function structure_fields_need_allocator fieldList>
    <#list fieldList as field>
        <#if structure_field_needs_allocator(field)>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>

<#macro structure_validate_field field numExtendedFields indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.isExtended>
        <#if numExtendedFields == 0>
${I}uint32_t numExtendedFields = 0;
        </#if>
${I}if (view.${field.getterName}().isPresent())
${I}{
${I}    if (++numExtendedFields != ${numExtendedFields})
${I}    {
${I}        throw ExtendedFieldException(
${I}                "Some of preceding extended fields is not present before '${name}.${field.name}'!");
${I}    }
        <@structure_validate_field_optional field, indent+1/>
${I}}
    <#else>
    <@structure_validate_field_optional field, indent/>
    </#if>
</#macro>

<#macro structure_validate_field_optional field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.optional??>
        <#if field.optional.viewIndirectClause??>
${I}// check non-auto optional
${I}if (${field.optional.viewIndirectClause})
${I}{
${I}    if (!view.${field.getterName}()<#if field.isExtended>.value()</#if>.has_value())
${I}    {
${I}        throw MissedOptionalException("Optional field '${name}.${field.name}' is used but not set!");
${I}    }
${I}}
${I}else
${I}{
${I}    if (view.${field.getterName}()<#if field.isExtended>.value()</#if>.has_value())
${I}    {
${I}        throw UnexpectedOptionalException(
${I}                "Optional field '${name}.${field.name}' is set but not used!");
${I}    }
${I}}
        </#if>
${I}if (view.${field.getterName}()<#if field.isExtended>-><#else>.</#if>has_value())
${I}{
        <@structure_validate_field_inner field, indent+1/>
${I}}
    <#else>
    <@structure_validate_field_inner field, indent/>
    </#if>
</#macro>

<#macro structure_validate_field_inner field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <@field_check_constraint field, indent/>
${I}detail::validate<@array_template_args field/>(<#rt>
        <#if field.isExtended>*</#if><#if field.optional??>*</#if>view.${field.getterName}(), <#t>
        "'${name}.${field.name}'"<#t>
        <#if field.array?? && field.array.viewIndirectLength??>
        , static_cast<size_t>(${field.array.viewIndirectLength})<#t>
        </#if>
        <#lt>);
</#macro>

<#macro structure_bitsizeof_field_extended field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.isExtended>
${I}if (<@field_view_local_name field/>.isPresent())
${I}{
${I}    endBitPosition = ::zserio::alignTo(8, endBitPosition);
        <@structure_bitsizeof_field field, indent+1, false/>
${I}}
    <#else>
    <@structure_bitsizeof_field field, indent, false/>
    </#if>
</#macro>

<#macro structure_bitsizeof_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.optional??>
        <#if !field.optional.viewIndirectClause??>
${I}endBitPosition += detail::bitSizeOf(Bool());
        </#if>
${I}if (<@field_view_local_name field/><#if field.isExtended>-><#else>.</#if>has_value())
${I}{
        <@structure_bitsizeof_field_inner field, indent+1, packed/>
${I}}
    <#else>
    <@structure_bitsizeof_field_inner field, indent, packed/>
    </#if>
</#macro>

<#macro structure_bitsizeof_field_inner field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.alignmentValue??>
${I}endBitPosition = alignTo(static_cast<BitSize>(${field.alignmentValue}), endBitPosition);
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}endBitPosition = alignTo(8, endBitPosition);
    </#if>
${I}endBitPosition += detail::bitSizeOf<@array_suffix field, packed/><@array_template_args field/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#if field.isExtended>*</#if><#if field.optional??>*</#if><@field_view_local_name field/>, endBitPosition<#lt>);
</#macro>

<#macro structure_write_field_extended field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.isExtended>
${I}if (<@field_view_local_name field/>.isPresent())
${I}{
${I}    writer.alignTo(8);
        <@structure_write_field field, indent+1, false/>
${I}}
    <#else>
    <@structure_write_field field, indent, false/>
    </#if>
</#macro>

<#macro structure_write_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.optional??>
${I}if (<@field_view_local_name field/><#if field.isExtended>-><#else>.</#if>has_value())
${I}{
        <#if !field.optional.viewIndirectClause??>
${I}    writer.writeBool(true);
        </#if>
        <@structure_write_field_inner field, indent+1, packed/>
${I}}
        <#if !field.optional.viewIndirectClause??>
${I}else
${I}{
${I}    writer.writeBool(false);
${I}}
        </#if>
    <#else>
    <@structure_write_field_inner field, indent, packed/>
    </#if>
</#macro>

<#macro structure_write_field_inner field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.alignmentValue??>
${I}writer.alignTo(static_cast<BitSize>(${field.alignmentValue}));
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}writer.alignTo(8);
    </#if>
${I}detail::write<@array_suffix field, packed/><@array_template_args field/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>writer, <#if field.isExtended>*</#if><#if field.optional??>*</#if><@field_view_local_name field/>);
</#macro>

<#macro structure_read_field_extended compoundName field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.isExtended>
${I}if (::zserio::alignTo(8U, static_cast<::zserio::BitSize>(reader.getBitPosition())) <
${I}        reader.getBufferBitSize())
${I}{
        reader.alignTo(8);
        <@structure_read_field compoundName, field, indent+1, false/>
${I}}
${I}else
${I}{
${I}    data.<@field_data_member_name field/>.setPresent(false);
${I}}
    <#else>
    <@structure_read_field compoundName, field, indent, false/>
    </#if>
</#macro>

<#macro structure_read_field compoundName field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.optional??>
        <#if field.optional.viewIndirectClause??>
${I}if (${field.optional.viewIndirectClause})
            <#else>
${I}if (reader.readBool())
            </#if>
${I}{
${I}    data.<@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>emplace();
        <@structure_read_field_inner compoundName, field, indent+1, packed/>
${I}}
    <#else>
    <@structure_read_field_inner compoundName, field, indent, packed/>
    </#if>
</#macro>

<#macro structure_read_field_inner compoundName field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.alignmentValue??>
${I}reader.alignTo(static_cast<BitSize>(${field.alignmentValue}));
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}reader.alignTo(8);
    </#if>
${I}<#if field.compound??>(void)</#if>detail::read<@array_read_suffix field, packed/><#rt>
        <@array_read_template_args fullName, field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        reader, <#if field.isExtended>*</#if><#if field.optional??>*</#if>data.<@field_data_member_name field/><#t>
        <#lt><@field_view_view_indirect_parameters field/>);
    <@field_check_constraint field, indent/>
</#macro>

<#macro structure_initialize_offsets_field_extended field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.isExtended>
${I}if (<@field_view_local_name field/>.isPresent())
${I}{
${I}    endBitPosition = ::zserio::alignTo(8, endBitPosition);
        <@structure_initialize_offsets_field field, indent+1, false/>
${I}}
    <#else>
    <@structure_initialize_offsets_field field, indent, false/>
    </#if>
</#macro>

<#macro structure_initialize_offsets_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.optional??>
        <#if !field.optional.viewIndirectClause??>
${I}endBitPosition += detail::bitSizeOf(Bool());
        </#if>
${I}if (<@field_view_local_name field/><#if field.isExtended>-><#else>.</#if>has_value())
${I}{
        <@structure_initialize_offsets_field_inner field, indent+1, packed/>
${I}}
    <#else>
    <@structure_initialize_offsets_field_inner field, indent, packed/>
    </#if>
</#macro>

<#macro structure_initialize_offsets_field_inner field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.alignmentValue??>
${I}endBitPosition = alignTo(static_cast<BitSize>(${field.alignmentValue}), endBitPosition);
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}endBitPosition = alignTo(8, endBitPosition);
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}${field.offset.viewIndirectSetter} = static_cast<<#if field.offset.typeInfo.isTemplateParameter>typename </#if><#rt>
            <#lt>${field.offset.typeInfo.typeFullName}::ValueType>(endBitPosition / 8);
    </#if>
${I}endBitPosition += <#rt>
        <#if field.compound?? || field.offset?? && field.offset.containsIndex>
        detail::initializeOffsets<#t>
        <#else>
        detail::bitSizeOf<#t>
        </#if>
        <@array_suffix field, packed/><@array_template_args field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#if field.isExtended>*</#if><#if field.optional??>*</#if><@field_view_local_name field/>, endBitPosition<#t>
        <#if field.offset?? && field.offset.containsIndex>
        , ObjectTraits<${fullName}>::<@structure_offset_setter_name field/>(view)<#t>
        </#if>
        <#lt>);
</#macro>

<#macro structure_reflectable isConst>
    class Reflectable : public ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>
    {
    public:
        using ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>::getField;
        using ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>::getAnyValue;

        explicit Reflectable(<#if isConst>const </#if>${fullName}& object, const ${types.allocator.default}& alloc = {}) :
                ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>(<#rt>
                        <#lt>typeInfo<${fullName}>(), alloc),
                m_object(object)
        {}
    <#if fieldList?has_content>

        <@reflectable_get_field name, fieldList, true/>
        <#if !isConst>

        <@reflectable_get_field name, fieldList, false/>

        <@reflectable_set_field name, fieldList/>

        <@reflectable_create_field name, fieldList/>
        </#if>
    </#if>

        ${types.any.name} getAnyValue(const ${types.allocator.default}& alloc) const override
        {
            return ${types.any.name}(::std::cref(m_object), alloc);
        }
    <#if !isConst>

        ${types.any.name} getAnyValue(const ${types.allocator.default}& alloc) override
        {
            return ${types.any.name}(::std::ref(m_object), alloc);
        }
    </#if>

    private:
        <#if isConst>const </#if>${fullName}& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
</#macro>

<#macro structure_introspectable isTemplate=false>
    class Introspectable : public ::zserio::detail::CompoundIntrospectableViewBase<${fullName}, ${types.allocator.default}>
    {
    public:
    <#if isTemplate>
        using ::zserio::detail::CompoundIntrospectableViewBase<${fullName}, ${types.allocator.default}>::getValue;
        using ::zserio::detail::CompoundIntrospectableViewBase<${fullName}, ${types.allocator.default}>::get_allocator;

    </#if>
        explicit Introspectable(const ::zserio::View<${fullName}>& view_, const ${types.allocator.default}& alloc = {}) :
                ::zserio::detail::CompoundIntrospectableViewBase<${fullName}, ${types.allocator.default}>(
                        view_, alloc)
        {}
    <#if fieldList?has_content>

        <@introspectable_get_field name, fieldList/>
    </#if>
    <#if parameterList?has_content>

        <@introspectable_get_parameter name, parameterList/>
    </#if>
    <#if functionList?has_content>

        <@introspectable_call_function name, functionList/>
    </#if>
    };

    return ::std::allocate_shared<Introspectable>(allocator, view);
</#macro>
