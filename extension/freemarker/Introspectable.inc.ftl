<#include "TypeInfo.inc.ftl">
<#macro introspectable_get_field compoundName fieldList indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}${types.introspectableConstPtr.name} getField(::std::string_view name) const override
${I}{
    <#list fieldList as field>
${I}    if (name == "${field.name}")
${I}    {
        <#if field.isExtended>
${I}        if (!getValue().${field.getterName}().isPresent())
${I}        {
${I}            return nullptr;
${I}        }

        </#if>
        <#if field.optional??>
${I}        if (!getValue().${field.getterName}()<#if field.isExtended>.value()</#if>.has_value())
${I}        {
${I}            return nullptr;
${I}        }

        </#if>
${I}        return ::zserio::introspectable<#if field.array??>Array</#if>(<#rt>
                    <#if field.isExtended>*</#if><#if field.optional??>*</#if><#t>
                    <#lt>getValue().${field.getterName}(), get_allocator());
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>

<#macro introspectable_get_parameter compoundName parametersList indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}${types.introspectableConstPtr.name} getParameter(::std::string_view name) const override
${I}{
    <#list parametersList as parameter>
${I}    if (name == "${parameter.name}")
${I}    {
${I}        return ::zserio::introspectable(getValue().${parameter.getterName}(), get_allocator());
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Parameter '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>

<#macro introspectable_call_function compoundName functionList indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}${types.introspectableConstPtr.name} callFunction(::std::string_view name) const override
${I}{
    <#list functionList as function>
${I}    if (name == "${function.name}")
${I}    {
${I}        return ::zserio::introspectable(getValue().${function.name}(), get_allocator());
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Function '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>
