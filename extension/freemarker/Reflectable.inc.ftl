<#include "TypeInfo.inc.ftl">
<#macro reflectable_get_field compoundName fieldList isConst indent=2>
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
                    <#lt>m_object.<@field_data_member_name field/>, get_allocator());
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>

<#macro reflectable_variant_get_field compoundName fieldList isConst indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}<#if isConst>${types.reflectableConstPtr.name}<#else>${types.reflectablePtr.name}</#if> getField(<#rt>
        <#lt>::std::string_view name) <#if isConst>const </#if>override
${I}{
    <#list fieldList as field>
${I}    if (name == "${field.name}")
${I}    {
${I}        return ::zserio::reflectable<#if field.array??>Array</#if>(
${I}                get<${fullName}::ChoiceTag::<@choice_tag_name field/>>(m_object), get_allocator());
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>

<#macro reflectable_create_field compoundName fieldList indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}${types.reflectablePtr.name} createField(::std::string_view name) override
${I}{
    <#list fieldList as field>
${I}    if (name == "${field.name}")
${I}    {
${I}        <#if field.isExtended>*</#if>m_object.<@field_data_member_name field/> = <@field_data_type_name field/>(<#rt>
                    <#lt><#if field.typeInfo.needsAllocator || field.array??>get_allocator()</#if>);
${I}        return ::zserio::reflectable<#if field.array??>Array</#if>(<#rt>
                    <#if field.isExtended   >*</#if><#if field.optional??>*</#if><#t>
                    <#lt>m_object.<@field_data_member_name field/>, get_allocator());
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>

<#macro reflectable_variant_create_field compoundName fieldList indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}${types.reflectablePtr.name} createField(::std::string_view name) override
${I}{
    <#list fieldList as field>
${I}    if (name == "${field.name}")
${I}    {
${I}        m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(<#rt>
                    <#lt><#if field.typeInfo.needsAllocator || field.array??>get_allocator()</#if>);
${I}        return ::zserio::reflectable<#if field.array??>Array</#if>(<#rt>
                    <#lt>get<${fullName}::ChoiceTag::<@choice_tag_name field/>>(m_object), get_allocator());
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>

<#macro reflectable_set_field compoundName fieldList indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}void setField(::std::string_view name, const ${types.any.name}& value) override
${I}{
    <#list fieldList as field>
${I}    if (name == "${field.name}")
${I}    {
        <#if field.isExtended>
            m_object.<@field_data_member_name field/>.setPresent(true);

        </#if>
        <#if field.optional??>
${I}        if (value.isType<::std::nullptr_t>())
${I}        {
${I}            m_object.<@field_data_member_name field/><#if field.isExtended>-><#else>.</#if>reset();
${I}            return;
${I}        }

        </#if>
        <#if field.array??>
${I}        <#if field.isExtended>*</#if>m_object.<@field_data_member_name field/> = <#rt>
                    <#lt>value.get<<@field_data_type_name field/>>();
        <#elseif field.typeInfo.isBitmask>
${I}        if (value.isType<${field.typeInfo.typeFullName}>())
${I}        {
${I}            <#if field.isExtended>*</#if>m_object.<@field_data_member_name field/> = <#rt>
                        <#lt>value.get<${field.typeInfo.typeFullName}>();
${I}        }
${I}        else if (value.isType<${field.typeInfo.typeFullName}::ZserioType>())
${I}        {
${I}            <#if field.isExtended>*</#if>m_object.<@field_data_member_name field/> =
${I}                    ${field.typeInfo.typeFullName}(value.get<${field.typeInfo.typeFullName}::ZserioType>());
${I}        }
${I}        else
${I}        {
${I}            <#if field.isExtended>*</#if>m_object.<@field_data_member_name field/> =
${I}                    ${field.typeInfo.typeFullName}(<#rt>
                                <#lt>value.get<${field.typeInfo.typeFullName}::ZserioType::ValueType>());
${I}        }
        <#elseif field.typeInfo.isEnum>
${I}        if (value.isType<${field.typeInfo.typeFullName}>())
${I}        {
${I}            <#if field.isExtended>*</#if>m_object.<@field_data_member_name field/> = <#rt>
                        <#lt>value.get<${field.typeInfo.typeFullName}>();
${I}        }
${I}        else if (value.isType<typename EnumTraits<${field.typeInfo.typeFullName}>::ZserioType>())
${I}        {
${I}            <#if field.isExtended>*</#if>m_object.<@field_data_member_name field/> =
${I}                    valueToEnum<${field.typeInfo.typeFullName}>(<#rt>
                                <#lt>value.get<typename EnumTraits<${field.typeInfo.typeFullName}>::ZserioType>());
${I}        }
${I}        else
${I}        {
${I}            <#if field.isExtended>*</#if>m_object.<@field_data_member_name field/> =
${I}                    valueToEnum<${field.typeInfo.typeFullName}>(<#rt>
                                <#lt>value.get<std::underlying_type_t<${field.typeInfo.typeFullName}>>());
${I}        }
        <#elseif field.typeInfo.isNumeric>
${I}        if (value.isType<${field.typeInfo.typeFullName}>())
${I}        {
${I}            <#if field.isExtended>*</#if>m_object.<@field_data_member_name field/> = <#rt>
                        <#lt>value.get<${field.typeInfo.typeFullName}>();
${I}        }
${I}        else
${I}        {
${I}            <#if field.isExtended>*</#if>m_object.<@field_data_member_name field/> = <#rt>
                        <#lt>value.get<${field.typeInfo.typeFullName}::ValueType>();
${I}        }
        <#else>
${I}        <#if field.isExtended>*</#if>m_object.<@field_data_member_name field/> = <#rt>
                    <#lt>value.get<<@field_data_type_name field/>>();
        </#if>
${I}        return;
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>

<#macro reflectable_variant_set_field compoundName fieldList indent=2>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}void setField(::std::string_view name, const ${types.any.name}& value) override
${I}{
    <#list fieldList as field>
${I}    if (name == "${field.name}")
${I}    {
        <#if field.array??>
${I}        m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(
${I}                value.get<<@field_data_type_name field/>>());
        <#elseif field.typeInfo.isBitmask>
${I}        if (value.isType<${field.typeInfo.typeFullName}>())
${I}        {
${I}            m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(
${I}                    value.get<${field.typeInfo.typeFullName}>());
${I}        }
${I}        else if (value.isType<${field.typeInfo.typeFullName}::ZserioType>())
${I}        {
${I}            m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(
${I}                    ${field.typeInfo.typeFullName}(value.get<${field.typeInfo.typeFullName}::ZserioType>()));
${I}        }
${I}        else
${I}        {
${I}            m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(
${I}                    ${field.typeInfo.typeFullName}(<#rt>
                                <#lt>value.get<${field.typeInfo.typeFullName}::ZserioType::ValueType>()));
${I}        }
        <#elseif field.typeInfo.isEnum>
${I}        if (value.isType<${field.typeInfo.typeFullName}>())
${I}        {
${I}            m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(
${I}                    value.get<${field.typeInfo.typeFullName}>());
${I}        }
${I}        else if (value.isType<typename EnumTraits<${field.typeInfo.typeFullName}>::ZserioType>())
${I}        {
${I}            m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(
${I}                    valueToEnum<${field.typeInfo.typeFullName}>(<#rt>
                                <#lt>value.get<typename EnumTraits<${field.typeInfo.typeFullName}>::ZserioType>()));
${I}        }
${I}        else
${I}        {
${I}            m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(
${I}                    valueToEnum<${field.typeInfo.typeFullName}>(<#rt>
                                <#lt>value.get<std::underlying_type_t<${field.typeInfo.typeFullName}>>()));
${I}        }
        <#elseif field.typeInfo.isNumeric>
${I}        if (value.isType<${field.typeInfo.typeFullName}>())
${I}        {
${I}            m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(
${I}                    value.get<${field.typeInfo.typeFullName}>());
${I}        }
${I}        else
${I}        {
${I}            m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(
${I}                    value.get<${field.typeInfo.typeFullName}::ValueType>());
${I}        }
        <#else>
${I}        m_object.emplace<${fullName}::ChoiceTag::<@choice_tag_name field/>>(
${I}                value.get<<@field_data_type_name field/>>());
        </#if>
${I}        return;
${I}    }
    </#list>
${I}    throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in '${compoundName}'!";
${I}}
</#macro>
