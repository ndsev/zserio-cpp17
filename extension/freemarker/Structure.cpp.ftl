<#include "FileHeader.inc.ftl">
<#include "Structure.inc.ftl">
<#include "TypeInfo.inc.ftl">
<#include "Reflectable.inc.ftl">
<#include "Introspectable.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
<#if withTypeInfoCode>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>
</#if>
<@system_includes cppSystemIncludes/>

<@user_include package.path, "${name}.h"/>
<@user_includes cppUserIncludes, false/>
<@namespace_begin package.path/>

${name}::${name}() noexcept :
        ${name}(allocator_type{})
{}

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
        <#elseif field.typeInfo.needsAllocator>
            <#local initializer>
                ${initializer}, allocator<#t>
            </#local>
        </#if>
        ${initializer}<#t>
    <#elseif field.typeInfo.needsAllocator || field.optional?? || field.array??>
        allocator<#t>
    </#if>
</#macro>
${name}::${name}(const allocator_type&<#if structure_fields_need_allocator(fieldList)> allocator</#if>) noexcept<#rt>
<#list fieldList>
        <#lt> :
    <#items as field>
        <@field_data_member_name field/>(<@structure_field_initializer field/>)<#sep>,</#sep>
    </#items>
<#else>

</#list>
{}

${name}::${name}(${name}&&<#if fieldList?has_content> other</#if>, const allocator_type&<#if structure_fields_need_allocator(fieldList)> allocator</#if>)<#rt>
<#list fieldList>
    <#lt> :
    <#items as field>
        <@field_data_member_name field/>(<#rt>
        <#if structure_field_needs_allocator(field)>
        <#lt>std::move(other.<@field_data_member_name field/>), allocator<#rt>
        <#else>
        <#lt>other.<@field_data_member_name field/><#rt>
        </#if>
        <#lt>)<#sep>,</#sep>
    </#items>
<#else>

</#list>
{}

${name}::${name}(const ${name}&<#if fieldList?has_content> other</#if>, const allocator_type&<#if structure_fields_need_allocator(fieldList)> allocator</#if>)<#rt>
<#list fieldList>
    <#lt> :
    <#items as field>
        <@field_data_member_name field/>(other.<@field_data_member_name field/><#rt>
        <#if structure_field_needs_allocator(field)>, allocator</#if><#t>
        <#lt>)<#sep>,</#sep>
    </#items>
<#else>

</#list>
{}
<#rt>
<#if fieldList?has_content>

${name}::${name}(
    <#list fieldList as field>
        <@structure_field_ctor_type_name field/> <@field_data_arg_name field/>,
    </#list>
        const allocator_type&<#if structure_fields_need_allocator(fieldList)> allocator</#if>) :
    <#list fieldList as field>
        <@field_data_member_name field/>(<#if structure_field_needs_allocator(field)>::std::move(</#if><#rt>
                <@field_data_arg_name field/><#t>
                <#lt><#if structure_field_needs_allocator(field)>), allocator</#if>)<#sep>,</#sep>
    </#list>
{}
</#if>

bool operator==(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content> rhs</#if>)
{
    <#if fieldList?has_content>
    return ::std::tie(
        <#list fieldList as field>
            lhs.<@field_data_member_name field/><#if field?has_next>,<#else>)</#if>
        </#list>
            == ::std::tie(
        <#list fieldList as field>
            rhs.<@field_data_member_name field/><#if field?has_next>,<#else>);</#if>
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
            lhs.<@field_data_member_name field/><#if field?has_next>,<#else>)</#if>
        </#list>
            < ::std::tie(
        <#list fieldList as field>
            rhs.<@field_data_member_name field/><#if field?has_next>,<#else>);</#if>
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
        m_data(&data)<#if parameterList?has_content>,</#if>
<#list parameterList as parameter>
        <@parameter_view_member_name parameter/>(<@parameter_view_arg_name parameter/>)<#if parameter?has_next>,</#if>
</#list>
{}

View<${fullName}>::View(const ${fullName}& data,
        const View&<#if parameterList?has_content> other</#if>) noexcept :
        m_data(&data)<#if parameterList?has_content>,</#if>
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
    <#lt><@structure_field_view_type_name field/>{
${I}        <#if field.optional??>::std::in_place, <#rt>
            m_data-><@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>get_allocator(), </#if><#t>
            <#if field.isExtended>*</#if><#if field.optional??>*</#if><#t>
            m_data-><@field_data_member_name field/><@field_view_parameters field/>}<#t>
</#macro>
<@structure_field_view_type_full_name fullName, field/> View<${fullName}>::${field.getterName}() const
{
    <#if !field.array?? && !field.typeInfo.isDynamicBitField && field.typeInfo.isSimple>
    <#-- simple -->
    return m_data-><@field_data_member_name field/>;
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
    return *m_data;
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
${I}validate<@array_template_args field/>(<#rt>
        <#if field.isExtended>*</#if><#if field.optional??>*</#if>view.${field.getterName}(), <#t>
        "'${name}.${field.name}'"<#t>
        <#if field.array?? && field.array.viewIndirectLength??>
        , static_cast<size_t>(${field.array.viewIndirectLength})<#t>
        </#if>
        <#lt>);
</#macro>
template <>
void validate(const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> view</#if>, ::std::string_view)
{
<#list parameterList as parameter>
    validate<@array_template_args parameter/>(view.${parameter.getterName}(), "'${name}.${parameter.name}'");
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
${I}endBitPosition = alignTo(static_cast<BitSize>(${field.alignmentValue}), endBitPosition);
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}endBitPosition = alignTo(8, endBitPosition);
    </#if>
${I}endBitPosition += bitSizeOf<@array_suffix field, packed/><@array_template_args field/>(<#rt>
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
${I}writer.alignTo(static_cast<BitSize>(${field.alignmentValue}));
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}writer.alignTo(8);
    </#if>
${I}write<@array_suffix field, packed/><@array_template_args field/>(<#rt>
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
${I}<#if field.compound??>(void)</#if>read<@array_read_suffix field, packed/><#rt>
        <@array_read_template_args fullName, field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        reader, <#if field.isExtended>*</#if><#if field.optional??>*</#if>data.<@field_data_member_name field/><#t>
        <#lt><@field_view_view_indirect_parameters field/>);
    <@field_check_constraint field, 1/>
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
${I}endBitPosition = alignTo(static_cast<BitSize>(${field.alignmentValue}), endBitPosition);
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
        <@array_suffix field, packed/><@array_template_args field/>(<#t>
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
<#if withTypeInfoCode>

const ${types.typeInfo.name}& TypeInfo<${fullName}, ${types.allocator.default}>::get()
{
    using AllocatorType = ${types.allocator.default};

    <@template_info_template_name_var "templateName", templateInstantiation!/>
    <@template_info_template_arguments_var "templateArguments", templateInstantiation!/>

    <#list fieldList as field>
    <@field_info_recursive_type_info_var field/>
    <@field_info_type_arguments_var field/>
    </#list>
    <@field_info_array_var "fields", fieldList/>

    <@parameter_info_array_var "parameters", parameterList/>

    <@function_info_array_var "functions", functionList/>

    static const ::zserio::detail::StructTypeInfo<AllocatorType> typeInfo = {
        "${schemaTypeName}",
        [](const AllocatorType& allocator) -> ${types.reflectablePtr.name}
        {
            return ::std::allocate_shared<::zserio::detail::ReflectableDataOwner<${fullName}>>(allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}
<@namespace_end ["detail"]/>

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
template <>
${types.reflectableConstPtr.name} reflectable(
        const ${fullName}& value, const ${types.allocator.default}& allocator)
{
    <@structure_reflectable true/>
}

template <>
${types.reflectablePtr.name} reflectable(
        ${fullName}& value, const ${types.allocator.default}& allocator)
{
    <@structure_reflectable false/>
}

template <>
${types.introspectableConstPtr.name} introspectable(const View<${fullName}>& view, <#rt>
        <#lt>const ${types.allocator.default}& allocator)
{
    class Introspectable : public ::zserio::detail::CompoundIntrospectableViewBase<${fullName}, ${types.allocator.default}>
    {
    public:
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
}
<@namespace_end ["zserio"]/>
<#else>
<@namespace_end ["zserio", "detail"]/>
</#if>
<@namespace_begin ["std"]/>

size_t hash<${fullName}>::operator()(const ${fullName}&<#if fieldList?has_content> data</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#list fieldList as field>
    result = ::zserio::calcHashCode(result, data.<@field_data_member_name field/>);
</#list>
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<${fullName}>>::operator()(<#rt>
        <#lt>const ::zserio::View<${fullName}>&<#if parameterList?has_content || fieldList?has_content> view</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#list parameterList as parameter>
    result = ::zserio::calcHashCode(result, view.${parameter.getterName}());
</#list>
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
