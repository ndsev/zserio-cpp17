<#include "FileHeader.inc.ftl">
<#include "Structure.inc.ftl">
<#include "TypeInfo.inc.ftl">
<#include "Reflectable.inc.ftl">
<#include "Introspectable.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
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

${name}::${name}() noexcept :
        ${name}(allocator_type{})
{}

${name}::${name}(const allocator_type&<#if structure_fields_need_allocator(fieldList)> allocator</#if>) noexcept<#rt>
<#list fieldList>
        <#lt> :
    <#items as field>
        <@field_data_member_name field/>(<@structure_field_initializer field/>)<#sep>,</#sep>
    </#items>
<#else>

</#list>
{}

${name}::${name}(${name}&&<#if fieldList?has_content> other</#if>, const allocator_type&<#if structure_fields_need_allocator(fieldList)> allocator</#if>)<#rt>
<#list fieldList>
    <#lt> :
    <#items as field>
        <@field_data_member_name field/>(<#rt>
        <#if structure_field_needs_allocator(field)>
                std::move(other.<@field_data_member_name field/>), allocator<#t>
        <#else>
                other.<@field_data_member_name field/><#t>
        </#if>
        <#lt>)<#sep>,</#sep>
    </#items>
<#else>

</#list>
{}

${name}::${name}(const ${name}&<#if fieldList?has_content> other</#if>, const allocator_type&<#if structure_fields_need_allocator(fieldList)> allocator</#if>)<#rt>
<#list fieldList>
    <#lt> :
    <#items as field>
        <@field_data_member_name field/>(other.<@field_data_member_name field/><#rt>
        <#if structure_field_needs_allocator(field)>, allocator</#if><#t>
        <#lt>)<#sep>,</#sep>
    </#items>
<#else>

</#list>
{}
<#if fieldList?has_content>

${name}::${name}(
    <#list fieldList as field>
        <@structure_field_ctor_type_name field/> <@field_data_arg_name field/>,
    </#list>
        const allocator_type&<#if structure_fields_need_allocator(fieldList)> allocator</#if>) :
    <#list fieldList as field>
        <@field_data_member_name field/>(<#if structure_field_needs_allocator(field)>::std::move(</#if><#rt>
                <@field_data_arg_name field/><#t>
                <#lt><#if structure_field_needs_allocator(field)>), allocator</#if>)<#sep>,</#sep>
    </#list>
{}
</#if>

bool operator==(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content> rhs</#if>)
{
    <#if fieldList?has_content>
    return ::std::tie(
        <#list fieldList as field>
            lhs.<@field_data_member_name field/><#if field?has_next>,<#else>)</#if>
        </#list>
            == ::std::tie(
        <#list fieldList as field>
            rhs.<@field_data_member_name field/><#if field?has_next>,<#else>);</#if>
        </#list>
    <#else>
    return true;
    </#if>
}

bool operator<(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content> rhs</#if>)
{
    <#if fieldList?has_content>
    return ::std::tie(
        <#list fieldList as field>
            lhs.<@field_data_member_name field/><#if field?has_next>,<#else>)</#if>
        </#list>
            < ::std::tie(
        <#list fieldList as field>
            rhs.<@field_data_member_name field/><#if field?has_next>,<#else>);</#if>
        </#list>
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
<@structure_offset_setters_definition fullName, fieldList/>

View<${fullName}>::View(const ${fullName}& data<#rt>
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

View<${fullName}>::View(const ${fullName}& data,
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

<@structure_field_view_type_full_name fullName, field/> View<${fullName}>::${field.getterName}() const
{
    <#if !field.array?? && !field.typeInfo.isDynamicBitField && field.typeInfo.isSimple>
    <#-- simple -->
    return m_data-><@field_data_member_name field/>;
    <#else>
    <@structure_field_view_getter field, 1/>
    </#if>
}
</#list>
<#list functionList as function>

<@function_return_type_name function/> View<${fullName}>::${function.name}() const
{
    return <@function_result_expression function/>;
}
</#list>

const ${fullName}& View<${fullName}>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> lhs</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> rhs</#if>)
{
<#if fieldList?has_content || parameterList?has_content>
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
        lhs.${field.getterName}() == rhs.${field.getterName}()<#t>
    </#list>
            <#lt>;
<#else>
    return true;
</#if>
}

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
    if (lhs.${field.getterName}() != rhs.${field.getterName}())
    {
        return lhs.${field.getterName}() < rhs.${field.getterName}();
    }
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

void ObjectTraits<${fullName}>::validate(<#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> view</#if>, ::std::string_view)
{
<#list parameterList as parameter>
    detail::validate<@array_template_args parameter/>(view.${parameter.getterName}(), "'${name}.${parameter.name}'");
</#list>
<#assign numExtendedFields=0/>
<#list fieldList as field>
    <#if field.isExtended>
        <#assign numExtendedFields++/>
    </#if>
    <@structure_validate_field field, numExtendedFields, 1/>
</#list>
}

BitSize ObjectTraits<${fullName}>::bitSizeOf(const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
<#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;

    <#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_bitsizeof_field_extended field, 1/>
    </#list>

    return endBitPosition - bitPosition;
<#else>
    return 0;
</#if>
}

void ObjectTraits<${fullName}>::write(BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
<#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_write_field_extended field, 1/>
</#list>
}

View<${fullName}> ObjectTraits<${fullName}>::read(BitStreamReader&<#if fieldList?has_content> reader</#if>, <#rt>
        ${fullName}& data<#t>
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
<#list fieldList as field>
    <@structure_read_field_extended fullName, field, 1/>
</#list>
    return view;
}
<#if isPackable && usedInPackedArray>

void ObjectTraits<${fullName}>::initContext(<#rt>
        PackingContext&<#if needs_packing_context(fieldList)> packingContext</#if>, <#t>
        <#lt>const View<${fullName}>&<#if needs_packing_context(fieldList)> view</#if>)
{
    <#list fieldList as field>
        <#if field_needs_packing_context(field)>
            <#if field.optional??>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    if (<@field_view_local_name field/>.has_value())
    {
        detail::initContext(<@packing_context field/>, *<@field_view_local_name field/>);
    }
            <#else>
    detail::initContext(<@packing_context field/>, view.${field.getterName}());
            </#if>
        </#if>
    </#list>
}

BitSize ObjectTraits<${fullName}>::bitSizeOf(<#rt>
        PackingContext&<#if needs_packing_context(fieldList)> packingContext</#if>, <#t>
        const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#t>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
    <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;

        <#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_bitsizeof_field field, 1, true/>
        </#list>

    return endBitPosition - bitPosition;
    <#else>
    return 0;
    </#if>
}

void ObjectTraits<${fullName}>::write(<#rt>
        PackingContext&<#if needs_packing_context(fieldList)> packingContext</#if>, <#t>
        BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#t>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
{
    <#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_write_field field, 1, true/>
    </#list>
}

void ObjectTraits<${fullName}>::read(<#rt>
        PackingContext&<#if needs_packing_context(fieldList)> packingContext</#if>, <#t>
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
    <#list fieldList as field>
    <@structure_read_field fullName, field, 1, true/>
    </#list>
    (void)view;
}
</#if>
<#if containsOffset>

BitSize ObjectTraits<${fullName}>::initializeOffsets(
        const View<${fullName}>&<#if fieldList?has_content> view</#if><#rt>
        <#lt>, BitSize<#if fieldList?has_content> bitPosition</#if>)
{
    <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;

        <#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_initialize_offsets_field_extended field, 1/>
        </#list>

    return endBitPosition - bitPosition;
    <#else>
    return 0;
    </#if>
}
    <#if isPackable && usedInPackedArray>

BitSize ObjectTraits<${fullName}>::initializeOffsets(
        PackingContext&<#if needs_packing_context(fieldList)> packingContext</#if>,
        const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
{
        <#if fieldList?has_content>
    BitSize endBitPosition = bitPosition;

            <#list fieldList as field>
    auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_initialize_offsets_field field, 1, true/>
            </#list>

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

    static const ::zserio::detail::StructTypeInfo<AllocatorType> typeInfo = {
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

<#macro structure_reflectable isConst>
    class Reflectable : public ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>
    {
    public:
        using ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>::getField;
        using ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>::getAnyValue;

        explicit Reflectable(<#if isConst>const </#if>${fullName}& object, const ${types.allocator.default}& alloc = {}) :
                ::zserio::detail::ReflectableData<#if isConst>Const</#if>AllocatorHolderBase<${types.allocator.default}>(<#rt>
                        <#lt>typeInfo<${fullName}>(), alloc),
                m_object(object)
        {}
    <#if fieldList?has_content>

        <@reflectable_get_field name, fieldList, true/>
        <#if !isConst>

        <@reflectable_get_field name, fieldList, false/>

        <@reflectable_set_field name, fieldList/>

        <@reflectable_create_field name, fieldList/>
        </#if>
    </#if>

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
${types.reflectableConstPtr.name} reflectable(
        const ${fullName}& value, const ${types.allocator.default}& allocator)
{
    <@structure_reflectable true/>
}

template <>
${types.reflectablePtr.name} reflectable(
        ${fullName}& value, const ${types.allocator.default}& allocator)
{
    <@structure_reflectable false/>
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
    };

    return ::std::allocate_shared<Introspectable>(allocator, view);
}
<@namespace_end ["zserio"]/>
<#else>
<@namespace_end ["zserio", "detail"]/>
</#if>
<@namespace_begin ["std"]/>

size_t hash<${fullName}>::operator()(const ${fullName}&<#if fieldList?has_content> data</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#list fieldList as field>
    result = ::zserio::calcHashCode(result, data.<@field_data_member_name field/>);
</#list>
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<${fullName}>>::operator()(<#rt>
        <#lt>const ::zserio::View<${fullName}>&<#if parameterList?has_content || fieldList?has_content> view</#if>) const
{
    uint32_t result = ::zserio::HASH_SEED;
<#list parameterList as parameter>
    result = ::zserio::calcHashCode(result, view.${parameter.getterName}());
</#list>
<#list fieldList as field>
    <#if field.optional??>
        <#if field.optional.viewIndirectClause??>
    if (${field.optional.viewIndirectClause})
        <#else>
    if (view.${field.getterName}())
        </#if>
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
