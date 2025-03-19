<#include "TypeInfo.inc.ftl">
<#macro reflectable_get_field compoundName fieldList isConst useVariant=false indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}<#if isConst>${types.reflectableConstPtr.name}<#else>${types.reflectablePtr.name}</#if> getField(<#rt>
        <#lt>::std::string_view name) <#if isConst>const </#if>override
${I}{
    <#list fieldList as field>
${I}    if (name == "${field.name}")
${I}    {
        <#if field.isExtended>
${I}        if (!m_object.<@field_data_member_name field/>.isPresent())
${I}        {
${I}            return nullptr;
${I}        }
        </#if>
        <#if field.optional??>
${I}        if (!m_object.<@field_data_member_name field/><#if field.isExtended>.value()</#if>.has_value())
${I}        {
${I}            return nullptr;
${I}        }

        </#if>
${I}        return ::zserio::reflectable<#if field.array??>Array</#if>(<#rt>
                    <#if field.isExtended>*</#if><#if field.optional??>*</#if><#t>
        <#if useVariant>
                    get<${fullName}::ChoiceTag::<@choice_tag_name field/>>(m_object), <#t>
        <#else>
                    m_object.<@field_data_member_name field/>, <#t>
        </#if>
                    <#lt>get_allocator());
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>

<#macro reflectable_create_field compoundName fieldList useVariant=false indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}${types.reflectablePtr.name} createField(::std::string_view name) override
${I}{
    <#list fieldList as field>
${I}    if (name == "${field.name}")
${I}    {
            <@reflectable_set_field_impl fullName, field, useVariant, indent+2>
${I}                <#if !useVariant><@field_data_type_name field/>(</#if><#rt>
                    <#if field.typeInfo.needsAllocator || field.array??>get_allocator()</#if><#t>
                    <#if !useVariant>)</#if><#t>
            </@reflectable_set_field_impl>
${I}        return ::zserio::reflectable<#if field.array??>Array</#if>(<#rt>
                    <#if field.isExtended>*</#if><#if field.optional??>*</#if><#t>
        <#if useVariant>
                    get<${fullName}::ChoiceTag::<@choice_tag_name field/>>(m_object), <#t>
        <#else>
                    m_object.<@field_data_member_name field/>, <#t>
        </#if>
                    <#lt>get_allocator());
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>

<#macro reflectable_set_field compoundName fieldList useVariant=false indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}void setField(::std::string_view name, const ${types.any.name}& value) override
${I}{
    <#list fieldList as field>
${I}    if (name == "${field.name}")
${I}    {
        <#if field.optional??>
${I}        if (value.isType<::std::nullptr_t>())
${I}        {
${I}            m_object.<@field_data_member_name field/>.reset();
${I}            return;
${I}        }

        </#if>
        <#if field.typeInfo.isBitmask>
${I}        if (value.isType<${field.typeInfo.typeFullName}>())
${I}        {
                <@reflectable_set_field_impl fullName, field, useVariant, indent+3>
${I}                    value.get<${field.typeInfo.typeFullName}>()<#rt>
                </@reflectable_set_field_impl>
${I}        }
${I}        else if (value.isType<${field.typeInfo.typeFullName}::ZserioType>())
${I}        {
                <@reflectable_set_field_impl fullName, field, useVariant, indent+3>
${I}                    ${field.typeInfo.typeFullName}(value.get<${field.typeInfo.typeFullName}::ZserioType>())<#rt>
                </@reflectable_set_field_impl>
${I}        }
${I}        else
${I}        {
                <@reflectable_set_field_impl fullName, field, useVariant, indent+3>
${I}                    ${field.typeInfo.typeFullName}(<#rt>
                                value.get<${field.typeInfo.typeFullName}::ZserioType::ValueType>())<#t>
                </@reflectable_set_field_impl>
${I}        }
        <#elseif field.typeInfo.isEnum>
${I}        if (value.isType<${field.typeInfo.typeFullName}>())
${I}        {
                <@reflectable_set_field_impl fullName, field, useVariant, indent+3>
${I}                    value.get<${field.typeInfo.typeFullName}>()<#rt>
                </@reflectable_set_field_impl>
${I}        }
${I}        else if (value.isType<typename EnumTraits<${field.typeInfo.typeFullName}>::ZserioType>())
${I}        {
                <@reflectable_set_field_impl fullName, field, useVariant, indent+3>
${I}                    valueToEnum<${field.typeInfo.typeFullName}>(<#rt>
                                value.get<typename EnumTraits<${field.typeInfo.typeFullName}>::ZserioType>())<#t>
                </@reflectable_set_field_impl>
${I}        }
${I}        else
${I}        {
                <@reflectable_set_field_impl fullName, field, useVariant, indent+3>
${I}                    valueToEnum<${field.typeInfo.typeFullName}>(<#rt>
                                value.get<std::underlying_type_t<${field.typeInfo.typeFullName}>>())<#t>
                </@reflectable_set_field_impl>
${I}        }
        <#else>
            <@reflectable_set_field_impl fullName, field, useVariant, indent+2>
${I}                value.get<<@field_data_type_name field/>>()<#rt>
            </@reflectable_set_field_impl>
        </#if>
${I}        return;
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>

<#macro reflectable_set_field_impl fullName field useVariant indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if useVariant>
${I}m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(
        <#lt><#nested>

${I});
    <#else>
${I}m_object.<@field_data_member_name field/> =
        <#lt><#nested>;
    </#if>
</#macro>
