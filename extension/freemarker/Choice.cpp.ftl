<#include "FileHeader.inc.ftl">
<#include "CompoundField.inc.ftl">
<#include "CompoundFunction.inc.ftl">
<#include "CompoundParameter.inc.ftl">
<#include "TypeInfo.inc.ftl">
<#include "Reflectable.inc.ftl">
<#include "Introspectable.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/ChoiceCaseException.h>
#include <zserio/CppRuntimeException.h>
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

bool operator==(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content> rhs</#if>)
{
<#if fieldList?has_content>
    return static_cast<const ${fullName}::Base&>(lhs) == static_cast<const ${fullName}::Base&>(rhs);
<#else>
    return true;
</#if>
}

bool operator<(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content>  rhs</#if>)
{
<#if fieldList?has_content>
    return static_cast<const ${fullName}::Base&>(lhs) < static_cast<const ${fullName}::Base&>(rhs);
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

View<${fullName}>::View(<#if !usedAsOffset>const </#if>${fullName}& data<#rt>
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

View<${fullName}>::View(<#if !usedAsOffset>const </#if>${fullName}& data,
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

<@field_view_type_full_name fullName, field/> View<${fullName}>::${field.getterName}() const
{
    <#if !field.array?? && !field.typeInfo.isDynamicBitField && field.typeInfo.isSimple>
    <#-- field which does not need View -->
    return get<${fullName}::Tag::<@choice_tag_name field/>>(*m_data);
    <#else>
    <#-- field which needs View -->
    return <@field_view_type_name field/>{get<${fullName}::Tag::<@choice_tag_name field/>>(*m_data)<#rt>
            <#lt><@field_view_parameters field/>};
    </#if>
}
</#list>
<#list functionList as function>

<@function_return_type_name function/> View<${fullName}>::${function.name}() const
{
    return <@function_result_expression function/>;
}
</#list>

${fullName}::Tag View<${fullName}>::zserioChoiceTag() const
{
    return m_data->index();
}

const ${fullName}& View<${fullName}>::zserioData() const
{
    return *m_data;
}

<#macro choice_switch memberActionMacroName noMatchMacroName switchExpression indent=1 packed=false>
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
        <@.vars[noMatchMacroName] name, indent+1/>
            </#if>
${I}}
        </#if>
    </#if>
</#macro>
<#macro choice_compare_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}return (lhs.${member.field.getterName}() == rhs.${member.field.getterName}());
    <#else>
${I}return true; // empty
    </#if>
</#macro>
<#macro choice_no_match name indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw CppRuntimeException("No match in choice ${fullName}!");
</#macro>
bool operator==(const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> lhs</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> rhs</#if>)
{
<#list parameterList as parameter>
    if (lhs.${parameter.getterName}() != rhs.${parameter.getterName}())
    {
        return false;
    }

</#list>
<#if fieldList?has_content>
    <@choice_switch "choice_compare_member", "choice_no_match", lhsIndirectSelectorExpression/>
<#else>
    return true;
</#if>
}

<#macro choice_less_than_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}return (lhs.${member.field.getterName}() < rhs.${member.field.getterName}());
    <#else>
${I}return false; // empty
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
<#if fieldList?has_content>
    <@choice_switch "choice_less_than_member", "choice_no_match", lhsIndirectSelectorExpression/>
<#else>
    return false;
</#if>
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
${I}validate<@array_template_args member.field/>(view.${member.field.getterName}(), "'${name}.${member.field.name}'"<#rt>
        <#if member.field.array?? && member.field.array.viewIndirectLength??>
        , static_cast<size_t>(${member.field.array.viewIndirectLength})<#t>
        </#if>
        <#lt>);
    <#else>
${I}// empty
    </#if>
</#macro>
<#macro choice_validate_no_match name indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw ChoiceCaseException("No match in choice ${fullName}!");
</#macro>
template <>
void validate(const View<${fullName}>& view, ::std::string_view)
{
<#list parameterList as parameter>
    validate(view.${parameter.getterName}(), "'${name}.${parameter.name}'");
</#list>
<#if fieldList?has_content>
    <@choice_switch "choice_validate_member", "choice_validate_no_match", viewIndirectSelectorExpression/>
</#if>
}

<#macro choice_bitsizeof_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}endBitPosition += bitSizeOf<@array_suffix member.field, packed/><@array_template_args member.field/>(<#rt>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        <#lt>view.${member.field.getterName}(), endBitPosition);
    <#else>
${I}// empty
    </#if>
</#macro>
template <>
BitSize bitSizeOf(const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
<#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    <@choice_switch "choice_bitsizeof_member", "choice_no_match", viewIndirectSelectorExpression/>

    return endBitPosition - bitPosition;
<#else>
    return 0;
</#if>
}

<#macro choice_write_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}write<@array_suffix member.field, packed/><@array_template_args member.field/>(<#rt>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        <#lt>writer, view.${member.field.getterName}());
    <#else>
${I}// empty
    </#if>
</#macro>
template <>
void write(BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#if fieldList?has_content>
    <@choice_switch "choice_write_member", "choice_no_match", viewIndirectSelectorExpression/>
</#if>
}

<#macro choice_read_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}data.emplace<${fullName}::Tag::<@choice_tag_name member.field/>>(<#rt>
        <#lt><#if member.field.typeInfo.needsAllocator>data.get_allocator()</#if>);
${I}<#if member.field.compound??>(void)</#if>read<@array_read_suffix member.field, packed/><#rt>
        <@array_read_template_args fullName, member.field/>(<#t>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        reader, data.get<${fullName}::Tag::<@choice_tag_name member.field/>>()<#t>
        <#lt><@field_view_view_indirect_parameters member.field/>);
    <@field_check_constraint member.field, indent/>
    <#else>
${I}// empty
    </#if>
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
<#if fieldList?has_content>
    <@choice_switch "choice_read_member", "choice_no_match", viewIndirectSelectorExpression/>
</#if>

    return view;
}
<#if isPackable && usedInPackedArray>

<#macro choice_init_context member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field?? && field_needs_packing_context(member.field)>
${I}initContext(<@packing_context member.field/>, view.${member.field.getterName}());
    <#else>
${I}// empty
    </#if>
</#macro>
template <>
void initContext(PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if needs_packing_context(fieldList)> view</#if>)
{
    <#if fieldList?has_content>
    <@choice_switch "choice_init_context", "choice_no_match", viewIndirectSelectorExpression, 1, true/>
    </#if>
}

template <>
BitSize bitSizeOf(PackingContext<${fullName}>&<#if needs_packing_context(fieldList)> packingContext</#if>, <#rt>
        const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#t>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
    <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    <@choice_switch "choice_bitsizeof_member", "choice_no_match", viewIndirectSelectorExpression, 1, true/>

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
<#if fieldList?has_content>
    <@choice_switch "choice_write_member", "choice_no_match", viewIndirectSelectorExpression, 1, true/>
</#if>
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
<#if fieldList?has_content>
    <@choice_switch "choice_read_member", "choice_no_match", viewIndirectSelectorExpression, 1, true/>
</#if>
    (void)view;
}
</#if>
<#if containsOffset>

<#macro choice_initialize_offsets_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}endBitPosition += <#if member.field.compound??>initializeOffsets<#else>bitSizeOf</#if><#rt>
        <@array_suffix member.field, packed/><@array_template_args member.field/>(<#t>
        <#if packed && field_needs_packing_context(member.field)><@packing_context member.field/>, </#if><#t>
        <#lt>view.${member.field.getterName}(), endBitPosition);
    <#else>
${I}// empty
    </#if>
</#macro>
BitSize OffsetsInitializer<${fullName}>::initialize(
        const View<${fullName}>&<#if fieldList?has_content> view</#if><#rt>
        <#lt>, BitSize<#if fieldList?has_content> bitPosition</#if>)
{
    <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    <@choice_switch "choice_initialize_offsets_member", "choice_no_match", viewIndirectSelectorExpression/>

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
    <@choice_switch "choice_initilize_offsets_member", "choice_no_match", viewIndirectSelectorExpression, 1, true/>

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

    <#list caseMemberList as caseMember>
    <@case_info_case_expressions_var caseMember caseMember?index/>
    </#list>
    <@case_info_array_var "cases" caseMemberList defaultMember!/>

    static const ::zserio::detail::ChoiceTypeInfo<AllocatorType> typeInfo = {
        "${schemaTypeName}",
        [](const AllocatorType& allocator) -> ${types.reflectablePtr.name}
        {
            return ::std::allocate_shared<::zserio::detail::ReflectableDataOwner<${fullName}>>(allocator, allocator);
        },
        templateName, templateArguments,
        fields, parameters, functions, "${selectorExpression?j_string}", cases
    };

    return typeInfo;
}
<@namespace_end ["detail"]/>

<#macro choice_reflectable isConst>
    class Reflectable : public ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>
    {
    public:
        using ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>::getField;
        using ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>::getAnyValue;

        explicit Reflectable(<#if isConst>const </#if>${fullName}& object, const ${types.allocator.default}& alloc) :
                ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>(<#rt>
                        <#lt>typeInfo<${fullName}>(), alloc),
                m_object(object)
        {}
    <#if fieldList?has_content>

        <@reflectable_variant_get_field name, fieldList, true/>
        <#if !isConst>

        <@reflectable_variant_get_field name, fieldList, false/>

        <@reflectable_variant_set_field name, fieldList/>

        <@reflectable_variant_create_field name, fieldList/>
        </#if>
    </#if>

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
    <#list fieldList as field>
            case ${fullName}::Tag::<@choice_tag_name field/>:
                return "${field.name}";
    </#list>
            default:
                return "";
            }
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

    return ::std::allocate_shared<Reflectable>(allocator, value, allocator);
</#macro>
template <>
${types.reflectableConstPtr.name} reflectable(const ${fullName}& value, const ${types.allocator.default}& allocator)
{
    <@choice_reflectable true/>
}

template <>
${types.reflectablePtr.name} reflectable(${fullName}& value, const ${types.allocator.default}& allocator)
{
    <@choice_reflectable false/>
}

template <>
${types.introspectableConstPtr.name} introspectable(const View<${fullName}>& view, <#rt>
        <#lt>const ${types.allocator.default}& allocator)
{
    class Introspectable : public ::zserio::detail::CompoundIntrospectableViewBase<${fullName}, ${types.allocator.default}>
    {
    public:
        Introspectable(const ::zserio::View<${fullName}>& view_, const ${types.allocator.default}& allocator) :
                ::zserio::detail::CompoundIntrospectableViewBase<${fullName}, ${types.allocator.default}>(
                        view_, allocator)
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
            switch (getValue().zserioChoiceTag())
            {
    <#list fieldList as field>
            case ${fullName}::Tag::<@choice_tag_name field/>:
                return "${field.name}";
    </#list>
            default:
                return "";
            }
        }
    };

    return ::std::allocate_shared<Introspectable>(allocator, view, allocator);
}
<@namespace_end ["zserio"]/>
<#else>
<@namespace_end ["zserio", "detail"]/>
</#if>
<@namespace_begin ["std"]/>

size_t hash<${fullName}>::operator()(const ${fullName}&<#if fieldList?has_content> value</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#if fieldList?has_content>
    result = ::zserio::calcHashCode(result, static_cast<const ${fullName}::Base&>(value));
</#if>
    return static_cast<size_t>(result);
}

<#macro choice_hash_member member indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if member.field??>
${I}result = ::zserio::calcHashCode(result, view.${member.field.getterName}());
    <#else>
${I}// empty
    </#if>
</#macro>
<#macro choice_hash_no_match name indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw ::zserio::CppRuntimeException("No match in choice ${fullName}!");
</#macro>
size_t hash<::zserio::View<${fullName}>>::operator()(<#rt>
        <#lt>const ::zserio::View<${fullName}>&<#if parameterList?has_content || fieldList?has_content> view</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#list parameterList as parameter>
    result = ::zserio::calcHashCode(result, view.${parameter.getterName}());
</#list>
<#if fieldList?has_content>
    <@choice_switch "choice_hash_member", "choice_hash_no_match", viewIndirectSelectorExpression/>

</#if>
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
