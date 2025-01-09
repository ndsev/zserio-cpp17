<#include "FileHeader.inc.ftl">
<#include "Structure.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
<@system_includes cppSystemIncludes/>

<@user_include package.path, "${name}.h"/>
<@user_includes cppUserIncludes, false/>
<@namespace_begin package.path/>

${name}::${name}() noexcept :
        ${name}(AllocatorType{})
{}

<#macro structure_field_initializer field>
    <#if field.initializer??>
        <#local initializer>
            <#if field.typeInfo.isNumeric>static_cast<${field.typeInfo.typeFullName}::ValueType>(</#if><#t>
            ${field.initializer}<#t>
            <#if field.typeInfo.isNumeric>)</#if><#t>
        </#local>
        <#local initializer>
            <#if field.typeInfo.needsAllocator>
                ${initializer}, allocator<#t>
            <#else>
                ${initializer}<#t>
            </#if>
        </#local>
        <#if field.optional??>
            <#local initializer>
                ::std::in_place, allocator, ${initializer}<#t>
            </#local>
        </#if>
        ${initializer}<#t>
    <#elseif field.typeInfo.needsAllocator || field.optional?? || field.array??>
        allocator<#t>
    </#if>
</#macro>
${name}::${name}(const AllocatorType&<#if structure_fields_need_allocator(fieldList)> allocator</#if>) noexcept<#rt>
<#list fieldList>
        <#lt> :
    <#items as field>
        ${field.name}(<@structure_field_initializer field/>)<#sep>,</#sep>
    </#items>
<#else>

</#list>
{}
<#if fieldList?has_content>

${name}::${name}(
    <#list fieldList as field>
        <@structure_field_ctor_type_name field/> <@field_data_arg_name field/><#if field?has_next>,<#else>) noexcept :</#if>
    </#list>
    <#list fieldList as field>
        ${field.name}(<#if structure_field_needs_allocator(field)>::std::move(</#if><#rt>
                <@field_data_arg_name field/>)<#t>
                <#lt><#if structure_field_needs_allocator(field)>)</#if><#sep>,</#sep>
    </#list>
{}
</#if>

bool operator==(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content> rhs</#if>)
{
    <#if fieldList?has_content>
    return ::std::tie(
        <#list fieldList as field>
            lhs.${field.name}<#if field?has_next>,<#else>)</#if>
        </#list>
            == ::std::tie(
        <#list fieldList as field>
            rhs.${field.name}<#if field?has_next>,<#else>);</#if>
        </#list>
    <#else>
    return true;
    </#if>
}

bool operator<(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content> rhs</#if>)
{
    <#if fieldList?has_content>
    return ::std::tie(
        <#list fieldList as field>
            lhs.${field.name}<#if field?has_next>,<#else>)</#if>
        </#list>
            < ::std::tie(
        <#list fieldList as field>
            rhs.${field.name}<#if field?has_next>,<#else>);</#if>
        </#list>
    <#else>
    return false;
    </#if>
}

bool operator!=(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return !(lhs < rhs);
}
<@namespace_end package.path/>
<@namespace_begin ["zserio"]/>

<@array_traits_definition fullName, fieldList/>
<@structure_offset_setters_definition fullName, fieldList/>
View<${fullName}>::View(const ${fullName}& data<#rt>
<#list parameterList as parameter>
        <#lt>,
        <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
</#list>
        <#lt>) noexcept :
        m_data(data)<#if parameterList?has_content>,</#if>
<#list parameterList as parameter>
        <@parameter_view_member_name parameter/>(<@parameter_view_arg_name parameter/>)<#if parameter?has_next>,</#if>
</#list>
{}

View<${fullName}>::View(const ${fullName}& data,
        const View&<#if parameterList?has_content> other</#if>) noexcept :
        m_data(data)<#if parameterList?has_content>,</#if>
<#list parameterList as parameter>
        <@parameter_view_member_name parameter/>(other.${parameter.getterName}())<#if parameter?has_next>,</#if>
</#list>
{}
<#list parameterList as parameter>

<@parameter_view_type_name parameter/> View<${fullName}>::${parameter.getterName}() const
{
    return <@parameter_view_member_name parameter/>;
}
</#list>
<#list fieldList as field>

<#macro structure_field_view_getter field, indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.isExtended>
${I}if (m_data.<@field_data_member_name field/>.isPresent())
${I}{
        <@structure_field_view_getter_optional field, indent+1/>
${I}}
${I}else
${I}{
${I}    auto <@field_view_local_name field/> = <#rt>
        <#if field.optional??><#-- TODO[Mi-L@]: What if non-present optional in data has value? -->
            <@structure_field_view_type_name field/>(::std::nullopt, <#t>
                    <#lt>m_data.<@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>get_allocator());
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
${I}if (m_data.<@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>has_value())
${I}{
${I}    return <@structure_field_view_getter_inner field, indent+1/>;
${I}}
${I}else
${I}{
${I}    return <@structure_field_view_type_name field/>(::std::nullopt, <#rt>
                <#lt>m_data.<@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>get_allocator());
${I}}
    <#else>
${I}return <@structure_field_view_getter_inner field, indent/>;
    </#if>
</#macro>
<#macro structure_field_view_getter_inner field, indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#lt><@structure_field_view_type_name field/>{
${I}        <#if field.optional??>std::in_place, <#rt>
            m_data.<@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>get_allocator(), </#if><#t>
            <#if field.isExtended>*</#if><#if field.optional??>*</#if><#t>
            m_data.<@field_data_member_name field/><@field_view_parameters field/>}<#t>
</#macro>
<@structure_field_view_type_full_name fullName, field/> View<${fullName}>::${field.getterName}() const
{
    <#if !field.array?? && !field.typeInfo.isDynamicBitField && field.typeInfo.isSimple>
    <#-- simple -->
    return m_data.<@field_data_member_name field/>;
    <#else>
    <@structure_field_view_getter field, 1/>
    </#if>
}
</#list>
<#list functionList as function>

<@function_return_type_name function/> View<${fullName}>::${function.name}() const
{
    return <@function_result_expression function/>;
}
</#list>

const ${fullName}& View<${fullName}>::zserioData() const
{
    return m_data;
}

bool operator==(const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> lhs</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> rhs</#if>)
{
<#if fieldList?has_content || parameterList?has_content>
    return <#rt>
    <#list parameterList as parameter>
        <#if !parameter?is_first>
            <#lt> &&
            <#nt><#rt>
        </#if>
        lhs.${parameter.getterName}() == rhs.${parameter.getterName}()<#t>
    </#list>
    <#list fieldList as field>
        <#if parameterList?has_content || !field?is_first>
            <#lt> &&
            <#nt><#rt>
        </#if>
        lhs.${field.getterName}() == rhs.${field.getterName}()<#t>
    </#list>
            <#lt>;
<#else>
    return true;
</#if>
}

bool operator<(const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> lhs</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> rhs</#if>)
{
<#list parameterList as parameter>
    if (lhs.${parameter.getterName}() != rhs.${parameter.getterName}())
    {
        return lhs.${parameter.getterName}() < rhs.${parameter.getterName}();
    }
</#list>
<#list fieldList as field>
    if (lhs.${field.getterName}() != rhs.${field.getterName}())
    {
        return lhs.${field.getterName}() < rhs.${field.getterName}();
    }
</#list>

    return false;
}

bool operator!=(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return !(lhs < rhs);
}
<@namespace_begin ["detail"]/>

<#assign numExtendedFields=0/>
<#macro structure_validate_field field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.isExtended>
        <#if numExtendedFields == 0>
${I}uint32_t numExtendedFields = 0;
        </#if>
        <#assign numExtendedFields++/>
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
${I}if ((${field.optional.viewIndirectClause}) && !view.${field.getterName}()<#if field.isExtended>.value()</#if>.has_value())
${I}{
${I}    throw MissedOptionalException("Optional field '${name}.${field.name}' is used but not set!");
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
    <@structure_check_constraint field, indent/>
${I}validate(<#if field.isExtended>*</#if><#if field.optional??>*</#if>view.${field.getterName}(), "'${name}.${field.name}'");
</#macro>
<#macro structure_check_constraint field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.constraint??>
${I}// check constraint
${I}if (!(${field.constraint.viewIndirectExpression}))
${I}{
${I}    throw ConstraintException("Constraint violated at '${name}.${field.name}'!");
${I}}
    </#if>
</#macro>
template <>
void validate(const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> view</#if>, ::std::string_view)
{
<#list parameterList as parameter>
    validate(view.${parameter.getterName}(), "'${name}.${parameter.name}'");
</#list>
<#list fieldList as field>
    <@structure_validate_field field, 1/>
</#list>
}

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
${I}endBitPosition += bitSizeOf(Bool());
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
${I}endBitPosition = alignTo(${field.alignmentValue}, endBitPosition);
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}endBitPosition = alignTo(8, endBitPosition);
    </#if>
${I}endBitPosition += bitSizeOf<@array_packed_suffix field, packed/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#if field.isExtended>*</#if><#if field.optional??>*</#if><@field_view_local_name field/>, endBitPosition<#lt>);
</#macro>
template <>
BitSize bitSizeOf(const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
<#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;

    <#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_bitsizeof_field_extended field, 1/>
    </#list>

    return endBitPosition - bitPosition;
<#else>
    return 0;
</#if>
}

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
${I}writer.alignTo(${field.alignmentValue});
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}writer.alignTo(8);
    </#if>
${I}write<@array_packed_suffix field, packed/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>writer, <#if field.isExtended>*</#if><#if field.optional??>*</#if><@field_view_local_name field/>);
</#macro>
template <>
void write(BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_write_field_extended field, 1/>
</#list>
}

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
${I}    data.<@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>emplace(<#rt>
            <#if field.typeInfo.needsAllocator>
                data.<@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>get_allocator()<#t>
            </#if>
                <#lt>);
        <@structure_read_field_inner compoundName, field, indent+1, packed/>
${I}}
    <#else>
    <@structure_read_field_inner compoundName, field, indent, packed/>
    </#if>
</#macro>
<#macro structure_read_field_inner compoundName field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.alignmentValue??>
${I}reader.alignTo(${field.alignmentValue});
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}reader.alignTo(8);
    </#if>
${I}<#if field.compound??>(void)</#if>read<@array_packed_suffix field, packed/><#rt>
        <#if field.array??><<@array_type_full_name compoundName, field/>></#if>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        reader, <#if field.isExtended>*</#if><#if field.optional??>*</#if>data.<@field_data_member_name field/><#t>
        <#lt><@field_view_view_indirect_parameters field/>);
</#macro>
template <>
View<${fullName}> read(BitStreamReader&<#if fieldList?has_content> reader</#if>, ${fullName}& data<#rt>
<#list parameterList as parameter>
        <#lt>,
        <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
</#list>
        <#lt>)
{
    View<${fullName}> view(data<#rt>
<#list parameterList as parameter>
            <#lt>,
            <#nt><@parameter_view_arg_name parameter/><#rt>
</#list>
            <#lt>);
<#list fieldList as field>
    <@structure_read_field_extended fullName, field, 1/>
    <@structure_check_constraint field, 1/>
</#list>
    return view;
}
<#if isPackable && usedInPackedArray>

template <>
void initContext(PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if needs_packing_context(fieldList)> view</#if>)
{
    <#list fieldList as field>
        <#if field_needs_packing_context(field)>
            <#if field.optional??>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    if (<@field_view_local_name field/>.has_value())
    {
        initContext(<@packing_context field/>, *<@field_view_local_name field/>);
    }
            <#else>
    initContext(<@packing_context field/>, view.${field.getterName}());
            </#if>
        </#if>
    </#list>
}

template <>
BitSize bitSizeOf(PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>, <#rt>
        const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#t>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
    <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;

        <#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_bitsizeof_field field, 1, true/>
        </#list>

    return endBitPosition - bitPosition;
    <#else>
    return 0;
    </#if>
}

template <>
void write(PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>, <#rt>
        BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#t>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_write_field field, 1, true/>
</#list>
}

template <>
void read(PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>, <#rt>
        BitStreamReader&<#if fieldList?has_content> reader</#if>, ${fullName}& data<#t>
    <#list parameterList as parameter>
        <#lt>,
        <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
    </#list>
        <#lt>)
{
    View<${fullName}> view(data<#rt>
<#list parameterList as parameter>
            <#lt>,
            <#nt><@parameter_view_arg_name parameter/><#rt>
</#list>
            <#lt>);
<#list fieldList as field>
    <@structure_read_field fullName, field, 1, true/>
    <@structure_check_constraint field, 1/>
</#list>
    (void)view;
}
</#if>
<#if containsOffset>

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
${I}endBitPosition += bitSizeOf(Bool());
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
${I}endBitPosition = alignTo(${field.alignmentValue}, endBitPosition);
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}endBitPosition = alignTo(8, endBitPosition);
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}${field.offset.viewIndirectSetter} = static_cast<${field.offset.typeInfo.typeFullName}::ValueType>(endBitPosition / 8);
    </#if>
${I}endBitPosition += <#rt>
        <#if field.compound?? || field.offset?? && field.offset.containsIndex>
        initializeOffsets<#t>
        <#else>
        bitSizeOf<#t>
        </#if>
        <@array_packed_suffix field, packed/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#if field.isExtended>*</#if><#if field.optional??>*</#if><@field_view_local_name field/>, endBitPosition<#t>
        <#if field.offset?? && field.offset.containsIndex>
        , View<${fullName}>::<@structure_offset_setter_name field/>(view)<#t>
        </#if>
        <#lt>);
</#macro>
BitSize OffsetsInitializer<${fullName}>::initialize(
        const View<${fullName}>&<#if fieldList?has_content> view</#if><#rt>
        <#lt>, BitSize<#if fieldList?has_content> bitPosition</#if>)
{
    <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;

        <#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_initialize_offsets_field_extended field, 1/>
        </#list>

    return endBitPosition - bitPosition;
    <#else>
    return 0;
    </#if>
}
    <#if isPackable && usedInPackedArray>

BitSize OffsetsInitializer<${fullName}>::initialize(
        PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>,
        const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
        <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;

            <#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_initialize_offsets_field field, 1, true/>
            </#list>

    return endBitPosition - bitPosition;
        <#else>
    return 0;
        </#if>
}
    </#if>
</#if>
<@namespace_end ["zserio", "detail"]/>
<@namespace_begin ["std"]/>

size_t hash<${fullName}>::operator()(const ${fullName}&<#if fieldList?has_content> data</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#list fieldList as field>
    result = ::zserio::calcHashCode(result, data.${field.name});
</#list>
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<${fullName}>>::operator()(<#rt>
        <#lt>const ::zserio::View<${fullName}>&<#if fieldList?has_content> view</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#list fieldList as field>
    <#if field.optional??>
        <#if field.optional.viewIndirectClause??>
    if (${field.optional.viewIndirectClause})
        <#else>
    if (view.${field.getterName}())
        </#if>
    {
        result = ::zserio::calcHashCode(result, view.${field.getterName}());
    }
    <#else>
    result = ::zserio::calcHashCode(result, view.${field.getterName}());
    </#if>
</#list>
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
