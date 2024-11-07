<#include "FileHeader.inc.ftl">
<#include "Structure.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
<@system_includes cppSystemIncludes/>

<@user_include package.path, "${name}.h"/>
<@user_includes cppUserIncludes, false/>
<@namespace_begin package.path/>

${name}::${name}() noexcept :
        ${name}(allocator_type{})
{}

${name}::${name}(const allocator_type&<#if fields_need_allocator(fieldList)> allocator</#if>) noexcept<#rt>
<#list fieldList>
        <#lt> :
    <#items as field>
        <#if field.initializer??>
        ${field.name}(${field.initializer}<#if field_needs_allocator(field)>, allocator</#if>)<#sep>,</#sep>
        <#else>
        ${field.name}(<#if field_needs_allocator(field)>allocator</#if>)<#sep>,</#sep>
        </#if>
    </#items>
<#else>

</#list>
{}
<#if fieldList?has_content>

${name}::${name}(
    <#list fieldList as field>
        <@structure_field_data_type_name field/> <@field_data_arg_name field/><#if field?has_next>,<#else>) noexcept :</#if>
    </#list>
    <#list fieldList as field>
        ${field.name}(<#if field.array?? || field.optional?? || !field.typeInfo.isSimple>::std::move(</#if><#rt>
                <@field_data_arg_name field/>)<#t>
                <#lt><#if field.array?? || field.optional?? || !field.typeInfo.isSimple>)</#if><#sep>,</#sep>
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
            == std::tie(
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
            < std::tie(
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

View<${fullName}>::View(const ${fullName}& data<#rt>
<#list parameterList as parameter>
        <#lt>,
        <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
</#list>
        <#lt>) noexcept :
        m_data(data)<#rt>
<#list parameterList as parameter>
        <#lt>,
        <@parameter_view_member_name parameter/>(<@parameter_view_arg_name parameter/>)<#rt>
</#list>

{}
<#list parameterList as parameter>

<@parameter_view_type_name parameter/> View<${fullName}>::${parameter.getterName}() const
{
    return <@parameter_view_member_name parameter/>;
}
</#list>
<#list fieldList as field>

<@structure_field_view_type_full_name fullName, field/> View<${fullName}>::${field.getterName}() const
{
    <#if !field.array?? && !field.typeInfo.isDynamicBitField && field.typeInfo.isSimple &&
            !(field.optional?? && field.optional.clause??)>
    <#-- simple or auto optional simple -->
    return m_data.<@field_data_member_name field/>;
    <#else>
        <#if field.optional??>
    if (<#if field.optional.clause??>!(${field.optional.clause})<#else>!m_data.<@field_data_member_name field/></#if>)
    {
        return {};
    }

    return <@field_view_type_name field/>{*m_data.<@field_data_member_name field/><@field_view_parameters field/>};
        <#else>
    return <@field_view_type_name field/>{m_data.<@field_data_member_name field/><@field_view_parameters field/>};
        </#if>
    </#if>
}
</#list>

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
        <#if field.optional?? && field.optional.lhsIndirectClause??>
            (!(${field.optional.lhsIndirectClause}) ? !(${field.optional.rhsIndirectClause}) : <#t>
                    *lhs.${field.getterName}() == *rhs.${field.getterName}())<#t>
        <#else>
            lhs.${field.getterName}() == rhs.${field.getterName}()<#t>
        </#if>
    </#list>
            <#lt>;
<#else>
    return true;
</#if>
}

<#macro structure_view_field_less_than field indent>
    <#local resolveOptional=field.optional?? && field.optional.lhsIndirectClause??/>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}if (<#if resolveOptional>*</#if>lhs.${field.getterName}() != <#if resolveOptional>*</#if>rhs.${field.getterName}())
${I}{
    <#if field.typeInfo.isBoolean>
        <#-- TODO[Mi-L@]: Remove once operator< for zserio::Bool is implemented in runtime! -->
${I}    return static_cast<int>(<#if resolveOptional>*</#if>lhs.${field.getterName}()) < <#rt>
        <#lt>static_cast<int>(<#if resolveOptional>*</#if>rhs.${field.getterName}());
    <#else>
${I}    return <#if resolveOptional>*</#if>lhs.${field.getterName}() < <#if resolveOptional>*</#if>rhs.${field.getterName}();
    </#if>
${I}}
</#macro>
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
    <#if field.optional?? && field.optional.lhsIndirectClause??>
    if ((${field.optional.lhsIndirectClause}) && (${field.optional.rhsIndirectClause}))
    {
        <@structure_view_field_less_than field, 2/>
    }
    else if ((${field.optional.lhsIndirectClause}) != (${field.optional.rhsIndirectClause}))
    {
        return !(${field.optional.lhsIndirectClause});
    }
    <#else>
    <@structure_view_field_less_than field, 1/>
    </#if>
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

template <>
void validate(const View<${fullName}>&)
{
    // TODO:
}

<#macro structure_view_bitsizeof field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.alignmentValue??>
${I}endBitPosition = ::zserio::alignTo(${field.alignmentValue}, endBitPosition);
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}endBitPosition = ::zserio::alignTo(8, endBitPosition);
    </#if>
${I}endBitPosition += ::zserio::detail::bitSizeOf(<#if field.optional??>*</#if>view.${field.getterName}(), <#rt>
        <#lt>endBitPosition);
</#macro>
template <>
BitSize bitSizeOf(const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
<#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;

    <#list fieldList as field>
        <#if field.optional??>
            <#if field.optional.viewIndirectClause??>
    if (${field.optional.viewIndirectClause})
            <#else>
    endBitPosition += 1;
    if (view.${field.getterName}())
            </#if>
    {
        <@structure_view_bitsizeof field, 2/>
    }
        <#else>
    <@structure_view_bitsizeof field, 1/>
        </#if>
    </#list>

    return endBitPosition - bitPosition;
<#else>
    return 0;
</#if>
}

<#macro structure_view_write field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.alignmentValue??>
${I}writer.alignTo(${field.alignmentValue});
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}writer.alignTo(8);
    </#if>
${I}::zserio::detail::write(writer, <#if field.optional??>*</#if>view.${field.getterName}());
</#macro>
template <>
void write(::zserio::BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#list fieldList as field>
    <#if field.optional??>
        <#if field.optional.viewIndirectClause??>
    if (${field.optional.viewIndirectClause})
        <#else>
    if (view.${field.getterName}())
        </#if>
    {
        <#if !field.optional.viewIndirectClause??>
        writer.writeBool(true);
        </#if>
        <@structure_view_write field, 2/>
    }
        <#if !field.optional.viewIndirectClause??>
    else
    {
        writer.writeBool(false);
    }
        </#if>
    <#else>
    <@structure_view_write field, 1/>
    </#if>
</#list>
}

<#macro structure_view_read compoundName field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.alignmentValue??>
${I}in.alignTo(${field.alignmentValue});
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}in.alignTo(8);
    </#if>
${I}<#if field.compound??>(void)</#if>::zserio::detail::read<#if field.array??><<@array_type_full_name compoundName, field/>></#if>(<#rt>
        reader, <#if field.optional??>*</#if>data.<@field_data_member_name field/><#t>
        <#lt><@field_view_view_indirect_parameters field/>);
</#macro>
template <>
View<${fullName}> read(::zserio::BitStreamReader&<#if fieldList?has_content> reader</#if>, ${fullName}& data<#rt>
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
    <#if field.optional??>
        <#if field.optional.viewIndirectClause??>
    if (${field.optional.viewIndirectClause})
        <#else>
    if (reader.readBool())
        </#if>
    {
        data.<@field_data_member_name field/> = <@field_data_type_name field/>(<#rt>
                <#lt><#if field.typeInfo.needsAllocator>data.<@field_data_member_name field/>.get_allocator()</#if>);
        <@structure_view_read fullName, field, 2/>
    }
    <#else>
    <@structure_view_read fullName, field, 1/>
    </#if>
</#list>
    return view;
}
<@namespace_end ["detail"]/>
<@namespace_end ["zserio"]/>
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
