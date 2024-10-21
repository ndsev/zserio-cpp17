<#include "FileHeader.inc.ftl">
<#include "CompoundField.inc.ftl">
<#include "CompoundParameter.inc.ftl">
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

${name}::${name}(const allocator_type&<#if needs_allocator(fieldList)> allocator</#if>) noexcept<#rt>
<#list fieldList>
        <#lt> :
        <#items as field>
        ${field.name}(<#if field.needsAllocator>allocator</#if>)<#sep>,</#sep>
        </#items>
<#else>

</#list>
{}
<#if fieldList?has_content>

${name}::${name}(
    <#list fieldList as field>
        <@field_data_type_name field/> <@field_data_arg_name field/><#if field?has_next>,<#else>) noexcept :</#if>
    </#list>
    <#list fieldList as field>
        ${field.name}(<#if !field.typeInfo.isSimple>::std::move(</#if><@field_data_arg_name field/>)<#rt>
                <#lt><#if !field.typeInfo.isSimple>)</#if><#sep>,</#sep>
    </#list>
{}
</#if>

bool operator==(const ${fullName}& lhs, const ${fullName}& rhs)
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
    (void)lhs;
    (void)rhs;
    return true;
    </#if>
}

bool operator<(const ${fullName}& lhs, const ${fullName}& rhs)
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
    (void)lhs;
    (void)rhs;
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

<@parameter_view_type_name parameter/> View<${fullName}::${parameter.getterName}() const
{
    return <@parameter_view_member_name parameter/>;
}
</#list>
<#list fieldList as field>

<@field_view_getter_type_name field/> View<${fullName}>::${field.getterName}() const
{
    <#if field.optional??>
    if (!(${field.optional.clause}))
    {
        return {};
    }

    return *m_data.<@field_data_member_name field/>;
    <#else>
    return m_data.<@field_data_member_name field/>;
    </#if>
}
</#list>

bool operator==(const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> lhs</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> rhs</#if>)
{
<#if fieldList?has_content || parameterList?has_content>
    if (&lhs == &rhs)
    {
        return true;
    }

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
        <#if field.optional??>
            (lhs.${field.getterName}() ? !rhs.${field.getterName}() : <#t>
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

<#macro structure_view_field_less_than lhs rhs indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}if (${lhs} != ${rhs})
${I}{
${I}    return ${lhs} < ${rhs};
${I}}
</#macro>
<#macro structure_view_field_less_than_optional field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.optional??>
${I}if (${field.getterName}() && other.${field.getterName}())
${I}{
        <@structure_view_field_less_than "*lhs." + field.getterName + "()", "*rhs." + field.getterName + "()",
                indent+1/>
${I}}
${I}else if (${field.getterName}() != other.${field.getterName}())
${I}{
${I}    return !${field.getterName}();
${I}}
    <#else>
    <@structure_view_field_less_than "lhs." + field.getterName + "()", "rhs." + field.getterName + "()",
            indent/>
    </#if>
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
    <@structure_view_field_less_than_optional field, 1/>
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
    if (${field.optional.viewIndirectClause})
    {
        <@structure_view_write field, 2/>
    }
    <#else>
    <@structure_view_write field, 1/>
    </#if>
</#list>
}

<#macro structure_view_read field indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field.alignmentValue??>
${I}in.alignTo(${field.alignmentValue});
    </#if>
    <#if field.offset?? && !field.offset.containsIndex>
${I}in.alignTo(8);
    </#if>
    <#local compoundArguments><@field_view_parameters field/></#local>
${I}(void)::zserio::detail::read(reader, data.<@field_data_member_name field/><#rt>
        <#lt><#if compoundArguments?has_content>, ${compoundArguments}</#if>);
</#macro>
View<${fullName}> read(::zserio::BitStreamReader&<#if fieldList?has_content> reader</#if>, <#rt>
        <#lt>${fullName}& data)
{
    View<${fullName}> view(data);
<#list fieldList as field>
    <#if field.optional??>
    if (${field.optional.viewIndirectClause})
    {
        data.<@field_data_member_name field/> = <@field_data_type_name field/>(<#rt>
                <#lt><#if field.typeInfo.needsAllocator>data.<@field_data_member_name field/>.get_allocator()</#if>);
        <@structure_view_read field, 2/>
    }
    <#else>
    <@structure_view_read field, 1/>
    </#if>
</#list>
    return view;
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
    if (${field.optional.viewIndirectClause})
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
    if (${field.optional.viewIndirectClause})
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
