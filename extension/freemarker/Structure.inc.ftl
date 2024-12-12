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
    <#if field.optional??>
        ::zserio::Optional<<@field_data_type_name field/>><#t>
    <#else>
        <@field_data_type_name field/><#t>
    </#if>
</#macro>

<#macro structure_field_data_type_name field>
    <#if field.isExtended>
        ::zserio::Extended<<#t>
    </#if>
    <#if field.optional??>
        ::zserio::Optional<<@field_data_type_name field/>><#t>
    <#else>
        <@field_data_type_name field/><#t>
    </#if>
    <#if field.isExtended>
        ><#t>
    </#if>
</#macro>

<#macro structure_field_view_type_name field>
    <#if field.isExtended>
        ::zserio::Extended<<#t>
    </#if>
    <#if field.optional??>
        ::zserio::Optional<<@field_view_type_name field/>><#t>
    <#else>
        <@field_view_type_name field/><#t>
    </#if>
    <#if field.isExtended>
        ><#t>
    </#if>
</#macro>

<#macro structure_field_view_type_full_name compoundName field>
    <#if field.isExtended>
        ::zserio::Extended<<#t>
    </#if>
    <#if field.optional??>
        ::zserio::Optional<<@field_view_type_full_name compoundName, field/>><#t>
    <#else>
        <@field_view_type_full_name compoundName, field/><#t>
    </#if>
    <#if field.isExtended>
        ><#t>
    </#if>
</#macro>