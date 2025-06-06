<#include "FileHeader.inc.ftl">
<#include "CompoundField.inc.ftl">
<#include "CompoundFunction.inc.ftl">
<#include "CompoundParameter.inc.ftl">
<#include "TypeInfo.inc.ftl">
<#include "Reflectable.inc.ftl">
<#include "Introspectable.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/SizeConvertUtil.h>
<#if withTypeInfoCode>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>
</#if>
#include <zserio/UnionCaseException.h>
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
    if (lhs.zserioChoiceTag() != rhs.zserioChoiceTag())
    {
        return false;
    }

    <@union_switch "union_compare_field", "union_compare_no_match", "lhs.zserioChoiceTag()"/>
<#else>
    return true;
</#if>
}

<#macro union_less_than_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return false;
</#macro>
<#macro union_less_than_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return (lhs.${field.getterName}() < rhs.${field.getterName}());
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
    if (lhs.zserioChoiceTag() != rhs.zserioChoiceTag())
    {
        return lhs.zserioChoiceTag() < rhs.zserioChoiceTag();
    }

    <@union_switch "union_less_than_field", "union_less_than_no_match", "lhs.zserioChoiceTag()"/>
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
void ObjectTraits<${fullName}>::validate(<#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> view</#if>, ::std::string_view)
{
<#list parameterList as parameter>
    detail::validate(view.${parameter.getterName}(), "'${name}.${parameter.name}'");
</#list>
<#if fieldList?has_content>
    <@union_switch "union_validate_field", "union_validate_no_match", "view.zserioChoiceTag()"/>
</#if>
}

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
BitSize ObjectTraits<${fullName}>::bitSizeOf(const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
<#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    endBitPosition += detail::bitSizeOf(
            fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    <@union_switch "union_bitsizeof_field", "union_no_match", "view.zserioChoiceTag()"/>

    return endBitPosition - bitPosition;
<#else>
    return 0;
</#if>
}

<#macro union_write_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}detail::write<@array_suffix field, packed/><@array_template_args field/>(<#rt>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>writer, view.${field.getterName}());
</#macro>
void ObjectTraits<${fullName}>::write(BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#if fieldList?has_content>
    detail::write(writer, fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    <@union_switch "union_write_field", "union_no_match", "view.zserioChoiceTag()"/>
</#if>
}

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
<#macro union_read_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}throw UnionCaseException("Unexpected choice tag during read of union '${fullName}'!");
</#macro>
View<${fullName}> ObjectTraits<${fullName}>::read(<#rt>
        <#lt>BitStreamReader&<#if fieldList?has_content> reader</#if>, ${fullName}& data<#rt>
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

    VarSize choiceTag;
    detail::read(reader, choiceTag);
    <@union_switch "union_read_field", "union_read_no_match", "static_cast<${fullName}::Tag>(choiceTag + 1)"/>
</#if>

    return view;
}
<#if isPackable && usedInPackedArray>

<#macro union_init_context field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#if field_needs_packing_context(field)>
${I}detail::initContext(<@packing_context field/>, view.${field.getterName}());
    </#if>
</#macro>
void ObjectTraits<${fullName}>::initContext(PackingContext<${fullName}>&<#if fieldList?has_content> packingContext</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
    <#if fieldList?has_content>
    detail::initContext(packingContext.zserioChoiceTag, fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    <@union_switch "union_init_context", "union_no_match", "view.zserioChoiceTag()", 1, true/>
    </#if>
}

BitSize ObjectTraits<${fullName}>::bitSizeOf(PackingContext<${fullName}>&<#if fieldList?has_content> packingContext</#if>, <#rt>
        const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#t>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
    <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    endBitPosition += detail::bitSizeOf(packingContext.zserioChoiceTag,
            fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    <@union_switch "union_bitsizeof_field", "union_no_match", "view.zserioChoiceTag()", 1, true/>

    return endBitPosition - bitPosition;
    <#else>
    return 0;
    </#if>
}

void ObjectTraits<${fullName}>::write(PackingContext<${fullName}>&<#if fieldList?has_content> packingContext</#if>, <#rt>
        BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#t>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#if fieldList?has_content>
    detail::write(packingContext.zserioChoiceTag, writer,
            fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    <@union_switch "union_write_field", "union_no_match", "view.zserioChoiceTag()", 1, true/>
</#if>
}

void ObjectTraits<${fullName}>::read(PackingContext<${fullName}>&<#if fieldList?has_content> packingContext</#if>, <#rt>
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

    VarSize choiceTag;
    detail::read(packingContext.zserioChoiceTag, reader, choiceTag);
    <@union_switch "union_read_field", "union_read_no_match", "static_cast<${fullName}::Tag>(choiceTag + 1)", 1, true/>
</#if>
    (void)view;
}
</#if>
<#if containsOffset>

<#macro union_initialize_offsets_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
    <#-- TODO[TEMPLATES]: Do we need to call initializeOffsets for template parameter types? -->
${I}endBitPosition += detail::<#if field.compound??>initializeOffsets<#else>bitSizeOf</#if><#rt>
        <@array_suffix field, packed/><@array_template_args field/>(<#t>
        <#if packed && field_needs_packing_context(field)><@packing_context field/>, </#if><#t>
        <#lt>view.${field.getterName}(), endBitPosition);
</#macro>
BitSize ObjectTraits<${fullName}>::initializeOffsets(
        const View<${fullName}>&<#if fieldList?has_content> view</#if><#rt>
        <#lt>, BitSize<#if fieldList?has_content> bitPosition</#if>)
{
    <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    endBitPosition += detail::bitSizeOf(fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    <@union_switch "union_initialize_offsets_field", "union_no_match", "view.zserioChoiceTag()"/>

    return endBitPosition - bitPosition;
    <#else>
    return 0;
    </#if>
}
    <#if isPackable && usedInPackedArray>

BitSize ObjectTraits<${fullName}>::initializeOffsets(
        PackingContext<${fullName}>&<#if fieldList?has_content> packingContext</#if>,
        const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
        <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;
    endBitPosition += detail::bitSizeOf(packingContext.zserioChoiceTag,
            fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    <@union_switch "union_initialize_offsets_field", "union_no_match", "view.zserioChoiceTag()", 1, true/>

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

    static const ::zserio::detail::UnionTypeInfo<AllocatorType> typeInfo = {
        "${schemaTypeName}",
        [](const AllocatorType& allocator) -> ${types.reflectablePtr.name}
        {
            return ::std::allocate_shared<::zserio::detail::ReflectableDataOwner<${fullName}>>(allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}
<@namespace_end ["detail"]/>

<#macro union_get_choice_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return "${field.name}";
</#macro>
<#macro union_get_choice_no_match fullName indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}return "";
</#macro>
<#macro union_reflectable isConst>
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

        <@reflectable_variant_create_field name, fieldList/>
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
template <>
${types.reflectableConstPtr.name} reflectable(const ${fullName}& value, const ${types.allocator.default}& allocator)
{
    <@union_reflectable true/>
}

template <>
${types.reflectablePtr.name} reflectable(${fullName}& value, const ${types.allocator.default}& allocator)
{
    <@union_reflectable false/>
}

template <>
${types.introspectableConstPtr.name} introspectable(const View<${fullName}>& view, <#rt>
        <#lt>const ${types.allocator.default}& allocator)
{
    class Introspectable : public ::zserio::detail::CompoundIntrospectableViewBase<${fullName}, ${types.allocator.default}>
    {
    public:
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

<#macro union_hash_field field indent packed>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}result = ::zserio::calcHashCode(result, view.${field.getterName}());
</#macro>
size_t hash<::zserio::View<${fullName}>>::operator()(<#rt>
        <#lt>const ::zserio::View<${fullName}>&<#if parameterList?has_content || fieldList?has_content> view</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#list parameterList as parameter>
    result = ::zserio::calcHashCode(result, view.${parameter.getterName}());
</#list>
<#if fieldList?has_content>
    <@union_switch "union_hash_field", "union_no_match", "view.zserioChoiceTag()"/>

</#if>
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
