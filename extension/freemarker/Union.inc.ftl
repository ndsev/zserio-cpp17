<#macro union_switch fieldActionMacroName noMatchMacroName switchExpression indent=1 packed=false>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}switch (${switchExpression})
${I}{
    <#list fieldList as field>
${I}case ${fullName}::Tag::<@choice_tag_name field/>:
        <#assign caseCode><@.vars[fieldActionMacroName] field, indent+1, packed/></#assign>
        ${caseCode}<#t>
        <#if !caseCode?contains("return ")>
${I}    break;
        </#if>
    </#list>
${I}default:
        <@.vars[noMatchMacroName] fullName, indent+1/>
${I}}
</#macro>

<#macro union_compare_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return true;
</#macro>

<#macro union_compare_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return (lhs.${field.getterName}() == rhs.${field.getterName}());
</#macro>

<#macro union_less_than_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return false;
</#macro>

<#macro union_less_than_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return (lhs.${field.getterName}() < rhs.${field.getterName}());
</#macro>

<#macro union_validate_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <@field_check_constraint field, indent/>
${I}detail::validate<@array_template_args field/>(view.${field.getterName}(), "'${name}.${field.name}'"<#rt>
        <#if field.array?? && field.array.viewIndirectLength??>
        , static_cast<size_t>(${field.array.viewIndirectLength})<#t>
        </#if>
        <#lt>);
</#macro>

<#macro union_validate_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw UnionCaseException("No case set in union '${fullName}'!");
</#macro>

<#macro union_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}break;
</#macro>

<#macro union_bitsizeof_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}endBitPosition += detail::bitSizeOf<@array_suffix field, packed/><@array_template_args field/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>view.${field.getterName}(), endBitPosition);
</#macro>

<#macro union_write_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}detail::write<@array_suffix field, packed/><@array_template_args field/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>writer, view.${field.getterName}());
</#macro>

<#macro union_read_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}data.emplace<${fullName}::Tag::<@choice_tag_name field/>>();
${I}<#if field.compound??>(void)</#if>detail::read<@array_read_suffix field, packed/><#rt>
        <@array_read_template_args fullName, field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        reader, data.get<${fullName}::Tag::<@choice_tag_name field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters field/>);
    <@field_check_constraint field, indent/>
</#macro>

<#macro union_template_read_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}data.template emplace<${fullName}::Tag::<@choice_tag_name field/>>();
${I}<#if field.compound??>(void)</#if>detail::read<@array_read_suffix field, packed/><#rt>
        <@array_read_template_args fullName, field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        reader, data.template get<${fullName}::Tag::<@choice_tag_name field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters field/>);
    <@field_check_constraint field, indent/>
</#macro>

<#macro union_read_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw UnionCaseException("Unexpected choice tag during read of union '${fullName}'!");
</#macro>

<#macro union_hash_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}result = ::zserio::calcHashCode(result, view.${field.getterName}());
</#macro>

<#macro union_init_context field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field_needs_packing_context(field)>
${I}detail::initContext(<@packing_context field/>, view.${field.getterName}());
    </#if>
</#macro>

<#macro union_initialize_offsets_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#-- TODO[TEMPLATES]: Do we need to call initializeOffsets for template parameter types? -->
${I}endBitPosition += detail::<#if field.compound??>initializeOffsets<#else>bitSizeOf</#if><#rt>
        <@array_suffix field, packed/><@array_template_args field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>view.${field.getterName}(), endBitPosition);
</#macro>

<#macro union_get_choice_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return "${field.name}";
</#macro>

<#macro union_get_choice_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return "";
</#macro>

<#macro union_reflectable isConst isTemplate=false>
    class Reflectable : public ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>
    {
    public:
        using ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>::getField;
        using ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>::getAnyValue;

        explicit Reflectable(<#if isConst>const </#if>${fullName}& object, const ${types.allocator.default}& alloc = {}) :
                ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>(typeInfo<${fullName}>(), alloc),
                m_object(object)
        {}
    <#if fieldList?has_content>

        <@reflectable_variant_get_field name, fieldList, true/>
        <#if !isConst>

        <@reflectable_variant_get_field name, fieldList, false/>

        <@reflectable_variant_set_field name, fieldList/>

        <@reflectable_variant_create_field name, fieldList, 2, isTemplate/>
        </#if>
    </#if>

        ::std::string_view getChoice() const override
        {
            <@union_switch "union_get_choice_field", "union_get_choice_no_match", "m_object.index()", 3/>
        }

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

<#macro union_introspectable isTemplate=false>
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

        ::std::string_view getChoice() const override
        {
            <@union_switch "union_get_choice_field", "union_get_choice_no_match",
                    "getValue().zserioChoiceTag()", 3/>
        }
    };

    return ::std::allocate_shared<Introspectable>(allocator, view);
</#macro>
