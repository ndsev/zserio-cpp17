<#macro choice_expression_switch memberActionMacroName noMatchMacroName switchExpression indent=1 packed=false>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if canUseNativeSwitch>
${I}switch (${switchExpression})
${I}{
        <#list caseMemberList as caseMember>
            <#list caseMember.expressionList as expression>
${I}case ${expression}:
            </#list>
            <#assign caseCode><@.vars[memberActionMacroName] caseMember, indent+1, packed/></#assign>
        ${caseCode}<#t>
            <#if !caseCode?contains("return ")>
${I}    break;
            </#if>
        </#list>
        <#if !isDefaultUnreachable>
${I}default:
            <#if defaultMember??>
                <#assign defaultCode><@.vars[memberActionMacroName] defaultMember, indent+1, packed/></#assign>
        ${defaultCode}<#t>
                <#if !defaultCode?contains("return ")>
${I}    break;
                </#if>
            <#else>
        <@.vars[noMatchMacroName] name, indent+1/>
            </#if>
        </#if>
${I}}
    <#else>
        <#list caseMemberList as caseMember>
            <#if caseMember?has_next || !isDefaultUnreachable>
${I}<#if caseMember?index != 0>else </#if>if (<#rt>
                <#list caseMember.expressionList as expression>
                    <#lt>(${switchExpression}) == (${expression})<#sep> ||</#sep><#rt>
                </#list>
                <#lt>)
            <#else>
${I}else
            </#if>
${I}{
        <@.vars[memberActionMacroName] caseMember, indent+1, packed/>
${I}}
        </#list>
        <#if !isDefaultUnreachable>
${I}else
${I}{
            <#if defaultMember??>
        <@.vars[memberActionMacroName] defaultMember, indent+1, packed/>
            <#else>
        <@.vars[noMatchMacroName] fullName, indent+1/>
            </#if>
${I}}
        </#if>
    </#if>
</#macro>

<#macro choice_switch memberActionMacroName noMatchMacroName switchExpression indent=1 packed=false>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}switch (${switchExpression})
${I}{
    <#list fieldList as field>
${I}case ${fullName}::Tag::<@choice_tag_name field/>:
        <#assign caseCode><@.vars[memberActionMacroName] field, indent+1, packed/></#assign>
        ${caseCode}<#t>
        <#if !caseCode?contains("return ")>
${I}    break;
        </#if>
    </#list>
${I}default:
        <@.vars[noMatchMacroName] fullName, indent+1/>
${I}}
</#macro>

<#macro choice_compare_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return (lhs.${field.getterName}() == rhs.${field.getterName}());
</#macro>

<#macro choice_compare_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return true;
</#macro>

<#macro choice_less_than_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return false;
</#macro>

<#macro choice_less_than_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return (lhs.${field.getterName}() < rhs.${field.getterName}());
</#macro>

<#macro choice_validate_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}// check choice case
${I}if (view.zserioChoiceTag() != ${fullName}::Tag::<@choice_tag_name member.field/>)
${I}{
${I}    throw ChoiceCaseException("Wrong case set in choice '${name}' (") << static_cast<size_t>(view.zserioChoiceTag()) <<
${I}            " != " << static_cast<size_t>(${fullName}::Tag::<@choice_tag_name member.field/>) << ")!";
${I}}
    <@field_check_constraint member.field, indent/>
${I}detail::validate<@array_template_args member.field/>(view.${member.field.getterName}(), "'${name}.${member.field.name}'"<#rt>
        <#if member.field.array?? && member.field.array.viewIndirectLength??>
        , static_cast<size_t>(${member.field.array.viewIndirectLength})<#t>
        </#if>
        <#lt>);
    <#else>
${I}// empty
${I}if (view.zserioChoiceTag() != ${fullName}::Tag::ZSERIO_UNDEFINED)
${I}{
${I}    throw ChoiceCaseException("Wrong case set in choice '${name}' (should be empty)!");
${I}}
    </#if>
</#macro>

<#macro choice_no_match_exception fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw ChoiceCaseException("No match in choice ${fullName}!");
</#macro>

<#macro choice_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}break;
</#macro>

<#macro choice_bitsizeof_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}endBitPosition += detail::bitSizeOf<@array_suffix field, packed/><@array_template_args field/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>view.${field.getterName}(), endBitPosition);
</#macro>

<#macro choice_write_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}detail::write<@array_suffix field, packed/><@array_template_args field/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>writer, view.${field.getterName}());
</#macro>

<#macro choice_read_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}data.emplace<${fullName}::Tag::<@choice_tag_name member.field/>>();
${I}<#if member.field.compound??>(void)</#if>detail::read<@array_read_suffix member.field, packed/><#rt>
        <@array_read_template_args fullName, member.field/>(<#t>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        reader, data.get<${fullName}::Tag::<@choice_tag_name member.field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters member.field/>);
    <@field_check_constraint member.field, indent/>
    <#else>
${I}// empty
    </#if>
</#macro>

<#macro choice_template_read_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}data.template emplace<${fullName}::Tag::<@choice_tag_name member.field/>>();
${I}<#if member.field.compound??>(void)</#if>detail::read<@array_read_suffix member.field, packed/><#rt>
        <@array_read_template_args fullName, member.field/>(<#t>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        reader, data.template get<${fullName}::Tag::<@choice_tag_name member.field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters member.field/>);
    <@field_check_constraint member.field, indent/>
    <#else>
${I}// empty
    </#if>
</#macro>

<#macro choice_hash_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}result = ::zserio::calcHashCode(result, view.${field.getterName}());
</#macro>

<#macro choice_init_context field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field_needs_packing_context(field)>
${I}detail::initContext(<@packing_context field/>, view.${field.getterName}());
    </#if>
</#macro>

<#macro choice_initialize_offsets_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}endBitPosition += detail::<#if field.compound??>initializeOffsets<#else>bitSizeOf</#if><#rt>
        <@array_suffix field, packed/><@array_template_args field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>view.${field.getterName}(), endBitPosition);
</#macro>

<#macro choice_get_choice_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return "${field.name}";
</#macro>

<#macro choice_get_choice_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return "";
</#macro>

<#macro choice_reflectable isConst indent=1 isTemplate=false>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}class ReflectableImpl : public ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>
${I}{
${I}public:
${I}    using Base = ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>;
${I}    using Base::getField;
${I}    using Base::getAnyValue;

${I}    explicit ReflectableImpl(<#if isConst>const </#if>${fullName}& object, const ${types.allocator.default}& alloc = {}) :
${I}            ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>(<#rt>
                        <#lt>typeInfo<${fullName}>(), alloc),
${I}            m_object(object)
${I}    {}
    <#if fieldList?has_content>

        <@reflectable_variant_get_field name, fieldList, true, indent+1/>
        <#if !isConst>

        <@reflectable_variant_get_field name, fieldList, false, indent+1/>

        <@reflectable_variant_set_field name, fieldList, indent+1/>

        <@reflectable_variant_create_field name, fieldList, indent+1, isTemplate/>
        </#if>
    </#if>

${I}    ::std::string_view getChoice() const override
${I}    {
            <@choice_switch "choice_get_choice_field", "choice_get_choice_no_match", "m_object.index()", indent+2/>
${I}    }

${I}    ${types.any.name} getAnyValue(const ${types.allocator.default}& alloc) const override
${I}    {
${I}        return ${types.any.name}(::std::cref(m_object), alloc);
${I}    }
    <#if !isConst>

${I}    ${types.any.name} getAnyValue(const ${types.allocator.default}& alloc) override
${I}    {
${I}        return ${types.any.name}(::std::ref(m_object), alloc);
${I}    }
    </#if>

${I}private:
${I}    <#if isConst>const </#if>${fullName}& m_object;
${I}};

${I}return ::std::allocate_shared<ReflectableImpl>(allocator, value);
</#macro>

<#macro choice_introspectable indent=1 isTemplate=false>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}class IntrospectableImpl : public ::zserio::detail::CompoundIntrospectableViewBase<${fullName}, ${types.allocator.default}>
${I}{
${I}public:
    <#if isTemplate>
${I}    using Base = ::zserio::detail::CompoundIntrospectableViewBase<${fullName}, ${types.allocator.default}>;
${I}    using Base::getValue;
${I}    using Base::get_allocator;

    </#if>
${I}    explicit IntrospectableImpl(const ::zserio::View<${fullName}>& view_, const ${types.allocator.default}& alloc = {}) :
${I}            ::zserio::detail::CompoundIntrospectableViewBase<${fullName}, ${types.allocator.default}>(
${I}                    view_, alloc)
${I}    {}
    <#if fieldList?has_content>

        <@introspectable_get_field name, fieldList, indent+1/>
    </#if>
    <#if parameterList?has_content>

        <@introspectable_get_parameter name, parameterList, indent+1/>
    </#if>
    <#if functionList?has_content>

        <@introspectable_call_function name, functionList, indent+1/>
    </#if>

${I}    ::std::string_view getChoice() const override
${I}    {
            <@choice_switch "choice_get_choice_field", "choice_get_choice_no_match",
                    "getValue().zserioChoiceTag()", indent+2/>
${I}    }
${I}};

${I}return ::std::allocate_shared<IntrospectableImpl>(allocator, view);
</#macro>
