<#include "FileHeader.inc.ftl">
<#include "DocComment.inc.ftl">
<#include "Introspectable.inc.ftl">
<#include "Structure.inc.ftl">
<#include "Reflectable.inc.ftl">
<#include "Template.inc.ftl">
<#include "TypeInfo.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <memory>

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
<#if isPackable && usedInPackedArray>
#include <zserio/DeltaContext.h>
</#if>
<#if structure_has_optional_field(fieldList)>
<@type_includes types.optional/>
</#if>
<#if withTypeInfoCode>
<@type_includes types.typeInfo/>
<@type_includes types.reflectablePtr/>
<@type_includes types.introspectableConstPtr/>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>
</#if>
#include <zserio/View.h>
#include <zserio/GenericAccessor.h>
<@type_includes types.allocator/>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>

<#if docComments??>
<@doc_comments docComments/>
</#if>
<@template_definition templateParameters/>
struct ${name}
{
    using allocator_type = ${types.allocator.default};
<#if structure_has_recursive_optional_field(fieldList)>
    using IS_RECURSIVE = void;
</#if>

    ${name}() noexcept
            : ${name}(allocator_type{})
    {}

    explicit ${name}(const allocator_type& allocator) noexcept<#rt>
<#list fieldList>
            <#lt> :
    <#items as field>
            <@field_data_member_name field/>(<@structure_field_initializer field/>)<#sep>,</#sep>
    </#items>
<#else>

</#list>
    {}

    ${name}(${name}&&) = default;

    ${name}(${name}&&<#if fieldList?has_content> other</#if>, <#rt>
            const allocator_type&<#if structure_fields_need_allocator(fieldList)> allocator</#if>)<#t>
<#list fieldList>
    <#lt> :
    <#items as field>
            <@field_data_member_name field/>(<#rt>
        <#if field.typeInfo.isTemplateParameter>
                    ::zserio::constructWithAllocator<<@structure_field_data_type_name field/>>(<#t>
                    allocator, std::move(other.<@field_data_member_name field/>))<#t>
        <#elseif structure_field_needs_allocator(field)>
                    std::move(other.<@field_data_member_name field/>), allocator<#t>
        <#else>
                    other.<@field_data_member_name field/><#t>
        </#if>
            <#lt>)<#sep>,</#sep>
    </#items>
<#else>

</#list>
    {}

    ${name}(const ${name}&) = default;

    ${name}(const ${name}&<#if fieldList?has_content> other</#if>, <#rt>
            const allocator_type&<#if structure_fields_need_allocator(fieldList)> allocator</#if>)<#t>
<#list fieldList>
    <#lt> :
    <#items as field>
            <@field_data_member_name field/>(<#rt>
        <#if field.typeInfo.isTemplateParameter>
                    ::zserio::constructWithAllocator<<@structure_field_data_type_name field/>>(<#t>
                    allocator, other.<@field_data_member_name field/>)<#t>
        <#else>
                    other.<@field_data_member_name field/><#t>
                    <#if structure_field_needs_allocator(field)>, allocator</#if><#t>
        </#if>
            <#lt>)<#sep>,</#sep>
    </#items>
<#else>

</#list>
    {}

    ${name}& operator=(${name}&&) = default;
    ${name}& operator=(const ${name}&) = default;

    ~${name}() = default;
<#if fieldList?has_content>

    <#if fieldList?size == 1>explicit </#if>${name}(
    <#list fieldList as field>
            <@structure_field_ctor_type_name field/> <@field_data_arg_name field/>,
    </#list>
            const allocator_type&<#if structure_fields_need_allocator(fieldList)> allocator</#if> = {}) :
    <#list fieldList as field>
            <@field_data_member_name field/>(<#rt>
        <#if field.typeInfo.isTemplateParameter>
                    ::zserio::constructWithAllocator<<@structure_field_data_type_name field/>>(<#t>
                    allocator, ::std::move(<@field_data_arg_name field/>))<#t>
        <#elseif structure_field_needs_allocator(field)>
                    ::std::move(<@field_data_arg_name field/>), allocator<#t>
        <#else>
                    <@field_data_arg_name field/>
        </#if>
            <#lt>)<#sep>,</#sep>
    </#list>
    {}
</#if>
<#list fieldList>

    <#items as field>
    <#if field.usedAsOffset>mutable </#if><@structure_field_data_type_name field/> <@field_data_member_name field/>;
    </#items>
</#list>
};

<@template_definition templateParameters/>
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

<@template_definition templateParameters/>
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

<@template_definition templateParameters/>
bool operator!=(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return !(lhs == rhs);
}

<@template_definition templateParameters/>
bool operator>(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return rhs < lhs;
}

<@template_definition templateParameters/>
bool operator<=(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return !(rhs < lhs);
}

<@template_definition templateParameters/>
bool operator>=(const ${fullName}& lhs, const ${fullName}& rhs)
{
    return !(lhs < rhs);
}
<@namespace_end package.path/>
<@namespace_begin ["zserio"]/>

<@template_definition templateParameters/>
class View<${fullName}>
{
public:
    <@array_traits_template fullName, fieldList/>
    explicit View(const ${fullName}& data<#rt>
<#list parameterList as parameter>
            <#lt>,
            <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
</#list>
            <#lt>) noexcept:
            m_data(&data)<#if parameterList?has_content>,</#if>
<#list parameterList as parameter>
            <@parameter_view_member_name parameter/>(<@parameter_view_arg_name parameter/>)<#if parameter?has_next>,</#if>
</#list>
    {}
<#list parameterList as parameter>

    <@parameter_view_type_name parameter/> ${parameter.getterName}() const
    {
        return <@parameter_view_member_name parameter/>;
    }
</#list>
<#list fieldList as field>

    <@structure_field_view_type_name field/> ${field.getterName}() const
    {
        <@structure_field_view_getter field, 2/>
    }
</#list>
<#list functionList>
    <#items as function>

    <@function_return_type_name function/> ${function.name}() const
    {
        return <@function_result_expression function/>;
    }
    </#items>
</#list>

    const ${fullName}& zserioData() const;

protected:
    View(const ${fullName}& data, const View& other) noexcept :
            m_data(&data)<#if parameterList?has_content>,</#if>
<#list parameterList as parameter>
            <@parameter_view_member_name parameter/>(other.${parameter.getterName}())<#if parameter?has_next>,</#if>
</#list>
    {}

private:
    const ${fullName}* m_data;
<#list parameterList as parameter>
    <@parameter_view_type_name parameter/> <@parameter_view_member_name parameter/>;
</#list>
};

<@template_definition templateParameters/>
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

<@template_definition templateParameters/>
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

<@template_definition templateParameters/>
bool operator!=(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return !(lhs == rhs);
}

<@template_definition templateParameters/>
bool operator>(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return rhs < lhs;
}

<@template_definition templateParameters/>
bool operator<=(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return !(rhs < lhs);
}

<@template_definition templateParameters/>
bool operator>=(const View<${fullName}>& lhs, const View<${fullName}>& rhs)
{
    return !(lhs < rhs);
}
<@namespace_begin ["detail"]/>

<@template_definition templateParameters/>
struct ObjectTraits<${fullName}>
{
<#if parameterList?has_content>
    <@parameter_traits parameterList, 1/>

</#if>
    <@structure_offset_setters_template fullName, fieldList/>
    static void validate(const View<${fullName}>&<#if fieldList?has_content || parameterList?has_content> view</#if>, <#rt>
            <#lt>::std::string_view fieldName)
    {
<#list parameterList as parameter>
        detail::validate<@array_template_args parameter/>(view.${parameter.getterName}(), "'${name}.${parameter.name}'");
</#list>
<#assign numExtendedFields=0/>
<#list fieldList as field>
    <#if field.isExtended>
        <#assign numExtendedFields++/>
    </#if>
        <@structure_validate_field field, numExtendedFields, 2/>
</#list>
    }

    static BitSize bitSizeOf(const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
            <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
    {
<#if fieldList?has_content>
        BitSize endBitPosition = bitPosition;

    <#list fieldList as field>
        auto <@field_view_local_name field/> = view.${field.getterName}();
        <@structure_bitsizeof_field_extended field, 2/>
    </#list>

        return endBitPosition - bitPosition;
<#else>
        return 0;
</#if>
    }

    static void write(BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#rt>
            <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
    {
<#list fieldList as field>
        auto <@field_view_local_name field/> = view.${field.getterName}();
        <@structure_write_field_extended field, 2/>
</#list>
    }

    static View<${fullName}> read(BitStreamReader&<#if fieldList?has_content> reader</#if>, ${fullName}& data<#rt>
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
        <@structure_read_field_extended fullName, field, 2/>
</#list>
        return view;
    }
<#if isPackable && usedInPackedArray>

    struct PackingContext
    {
    <#list fieldList as field>
        <#if field_needs_packing_context(field) && !(field.optional?? && field.optional.isRecursive)>
        <@packing_context_type_name field/> <@packing_context_member_name field/>;
        </#if>
    </#list>
    };

    static void initContext(<#rt>
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

    static BitSize bitSizeOf(<#rt>
            PackingContext&<#if needs_packing_context(fieldList)> packingContext</#if>, <#t>
            const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#t>
            <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
    {
    <#if fieldList?has_content>
        BitSize endBitPosition = bitPosition;

        <#list fieldList as field>
        auto <@field_view_local_name field/> = view.${field.getterName}();
    <@structure_bitsizeof_field field, 2, true/>
        </#list>

        return endBitPosition - bitPosition;
    <#else>
        return 0;
    </#if>
    }

    static void write(<#rt>
            PackingContext&<#if needs_packing_context(fieldList)> packingContext</#if>, <#t>
            BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#t>
            <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
    {
    <#list fieldList as field>
        auto <@field_view_local_name field/> = view.${field.getterName}();
        <@structure_write_field field, 2, true/>
    </#list>
    }

    static void read(<#rt>
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
        <@structure_read_field fullName, field, 2, true/>
    </#list>
        (void)view;
    }
</#if>
<#if containsOffset>

    static BitSize initializeOffsets(const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
            <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
    {
    <#if fieldList?has_content>
        BitSize endBitPosition = bitPosition;

        <#list fieldList as field>
        auto <@field_view_local_name field/> = view.${field.getterName}();
        <@structure_initialize_offsets_field_extended field, 2/>
        </#list>

        return endBitPosition - bitPosition;
    <#else>
        return 0;
    </#if>
    }
    <#if isPackable && usedInPackedArray>
    
    static BitSize initializeOffsets(PackingContext&<#if needs_packing_context(fieldList)> packingContext</#if>,
            const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
            <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
    {
        <#if fieldList?has_content>
        BitSize endBitPosition = bitPosition;

            <#list fieldList as field>
        auto <@field_view_local_name field/> = view.${field.getterName}();
        <@structure_initialize_offsets_field field, 2, true/>
            </#list>

        return endBitPosition - bitPosition;
        <#else>
        return 0;
        </#if>
    }
    </#if>
</#if>
};
<#if withTypeInfoCode>

<@template_definition templateParameters/>
struct TypeInfo<${fullName}, ${types.allocator.default}>
{
    static const ${types.typeInfo.name}& get()
    {
        using AllocatorType = ${types.allocator.default};

        <@template_info_template_name_var "templateName", templateInstantiation!, 2/>
        <@template_info_template_arguments_var "templateArguments", templateInstantiation!, 2/>

    <#list fieldList as field>
        <@field_info_recursive_type_info_var field/>
        <@field_info_type_arguments_var field, 2/>
    </#list>
        <@field_info_array_var "fields", fieldList, 2/>

        <@parameter_info_array_var "parameters", parameterList, 2/>

        <@function_info_array_var "functions", functionList, 2/>

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
};

<@template_definition templateParameters/>
struct Reflectable<${fullName}, ${types.allocator.default}>
{
    static ${types.reflectableConstPtr.name} create(const ${fullName}& value,
            const ${types.allocator.default}& allocator)
    {
        <@structure_reflectable true, 2, true/>
    }

    static ${types.reflectablePtr.name} create(${fullName}& value,
            const ${types.allocator.default}& allocator)
    {
        <@structure_reflectable false, 2, true/>
    }
};

<@template_definition templateParameters/>
struct Introspectable<${fullName}, ${types.allocator.default}>
{
    static ${types.introspectableConstPtr.name} create(const View<${fullName}>& view,
            const ${types.allocator.default}& allocator)
    {
        <@structure_introspectable 2, true/>
    }
};
</#if>
<@namespace_end ["zserio", "detail"]/>
<@namespace_begin ["std"]/>

<@template_definition templateParameters/>
struct hash<${fullName}>
{
    size_t operator()(const ${fullName}& data) const
    {
        uint32_t result = ::zserio::HASH_SEED;
<#list fieldList as field>
        result = ::zserio::calcHashCode(result, data.<@field_data_member_name field/>);
</#list>
        return static_cast<size_t>(result);
    }
};

<@template_definition templateParameters/>
struct hash<::zserio::View<${fullName}>>
{
    size_t operator()(const ::zserio::View<${fullName}>& view) const
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
};
<@namespace_end ["std"]/>

<@include_guard_end package.path, name/>
