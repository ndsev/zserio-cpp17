<#include "CompoundField.inc.ftl">
<#include "CompoundParameter.inc.ftl">
<#include "CompoundFunction.inc.ftl">
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
void View<${compoundFullName}>::<@structure_offset_setter_name field/>::setOffset(<#rt>
        <#lt>size_t index, BitSize byteOffset) const
{
    ${field.offset.ownerIndirectSetter} = static_cast<${field.offset.typeInfo.typeFullName}::ValueType>(byteOffset);
}

        </#if>
    </#list>
</#macro>

<#function structure_field_needs_allocator field>
    <#return field.typeInfo.needsAllocator || field.optional?? || field.array??>
</#function>

<#function structure_fields_need_allocator fieldList>
    <#list fieldList as field>
        <#if structure_field_needs_allocator(field)>
            <#return true>
        </#if>
    </#list>
    <#return false>
</#function>
