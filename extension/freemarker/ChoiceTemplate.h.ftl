<#include "FileHeader.inc.ftl">
<#include "CompoundField.inc.ftl">
<#include "CompoundFunction.inc.ftl">
<#include "CompoundParameter.inc.ftl">
<#include "DocComment.inc.ftl">
<#include "Introspectable.inc.ftl">
<#include "Reflectable.inc.ftl">
<#include "Template.inc.ftl">
<#include "TypeInfo.inc.ftl">
<#include "Choice.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <memory>

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
<@type_includes types.variant/>
#include <zserio/ChoiceCaseException.h>
#include <zserio/ChoiceTag.h>
<#if withTypeInfoCode>
<@type_includes types.typeInfo/>
<@type_includes types.reflectablePtr/>
<@type_includes types.introspectableConstPtr/>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>
</#if>
#include <zserio/GenericAccessor.h>
#include <zserio/View.h>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>

// forward declaration
<@template_definition templateParameters/>
struct ${name};
<@namespace_end package.path/>
<@namespace_begin ["zserio", "detail"]/>

<@template_definition templateParameters/>
struct ChoiceTag<${fullName}>
{
    enum Tag : size_t
    {
        ZSERIO_UNDEFINED,
<#list fieldList as field>
        <@choice_tag_name field/><#sep>,</#sep>
</#list>
    };
};
<@namespace_end ["zserio", "detail"]/>
<@namespace_begin package.path/>

<#if docComments??>
<@doc_comments docComments/>
</#if>
<@template_definition templateParameters/>
struct ${name} : <@variant_type_name_begin/>typename ::zserio::detail::ChoiceTag<${fullName}>::Tag,
        ::std::monostate<#rt>
<#list fieldList as field>
        <#lt>,
        <#nt><@field_data_type_name field/><#rt>
</#list>
        <#lt>>
{
    using Tag = typename ::zserio::detail::ChoiceTag<${name}>::Tag;
    using Base = <@variant_type_name_begin/>Tag,
            ::std::monostate<#rt>
<#list fieldList as field>
            <#lt>,
            <#nt><@field_data_type_name field/><#rt>
</#list>
            <#lt>>;
    using Base::Base;
};

<@template_definition templateParameters/>
bool operator==(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content> rhs</#if>)
{
<#if fieldList?has_content>
    return static_cast<const typename ${fullName}::Base&>(lhs) == static_cast<const typename ${fullName}::Base&>(rhs);
<#else>
    return true;
</#if>
}

<@template_definition templateParameters/>
bool operator<(const ${fullName}&<#if fieldList?has_content> lhs</#if>, <#rt>
        <#lt>const ${fullName}&<#if fieldList?has_content>  rhs</#if>)
{
<#if fieldList?has_content>
    return static_cast<const typename ${fullName}::Base&>(lhs) < static_cast<const typename ${fullName}::Base&>(rhs);
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
    explicit View(<#if !usedAsOffset>const </#if>${fullName}& data<#rt>
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
<#list parameterList as parameter>

    <@parameter_view_type_name parameter/> ${parameter.getterName}() const
    {
        return <@parameter_view_member_name parameter/>;
    }
</#list>
<#list fieldList as field>

    <@field_view_type_name field/> ${field.getterName}() const
    {
        return <@field_view_type_name field/>{get<${fullName}::Tag::<@choice_tag_name field/>>(*m_data)<#rt>
            <#lt><@field_view_parameters field/>};
    }
</#list>
<#list functionList as function>

    <@function_return_type_name function/> ${function.name}() const
    {
        return <@function_result_expression function/>;
    }
</#list>

    typename ${fullName}::Tag zserioChoiceTag() const
    {
        return m_data->index();
    }

    const ${fullName}& zserioData() const
    {
        return *m_data;
    }

protected:
    View(<#if !usedAsOffset>const </#if>${fullName}& data, const View&<#if parameterList?has_content> other</#if>) noexcept :
            m_data(&data)<#if parameterList?has_content>,</#if>
<#list parameterList as parameter>
            <@parameter_view_member_name parameter/>(other.${parameter.getterName}())<#if parameter?has_next>,</#if>
</#list>
{}

private:
    <#if !usedAsOffset>const </#if>${fullName}* m_data;
<#list parameterList as parameter>
    <@parameter_view_type_name parameter/> <@parameter_view_member_name parameter/>;
</#list>
};

<@template_definition templateParameters/>
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

    <@choice_switch "choice_compare_field", "choice_compare_no_match", "lhs.zserioChoiceTag()"/>
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
<#if fieldList?has_content>
    if (lhs.zserioChoiceTag() != rhs.zserioChoiceTag())
    {
        return lhs.zserioChoiceTag() < rhs.zserioChoiceTag();
    }

    <@choice_switch "choice_less_than_field", "choice_less_than_no_match", "lhs.zserioChoiceTag()"/>
<#else>
    return false;
</#if>
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
    static void validate(const View<${fullName}>& view, ::std::string_view)
    {
<#list parameterList as parameter>
        detail::validate(view.${parameter.getterName}(), "'${name}.${parameter.name}'");
</#list>
<#if fieldList?has_content>
        <@choice_expression_switch "choice_validate_member", "choice_no_match_exception",
                viewIndirectSelectorExpression, 2/>
</#if>
    }

    static BitSize bitSizeOf(const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
        <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
    {
<#if fieldList?has_content>
        BitSize endBitPosition = bitPosition;
        <@choice_switch "choice_bitsizeof_field", "choice_no_match", "view.zserioChoiceTag()", 2/>

        return endBitPosition - bitPosition;
<#else>
        return 0;
</#if>
    }

    static void write(BitStreamWriter&<#if fieldList?has_content> writer</#if>, <#rt>
        <#lt>const View<${fullName}>&<#if fieldList?has_content> view</#if>)
    {
<#if fieldList?has_content>
        <@choice_switch "choice_write_field", "choice_no_match", "view.zserioChoiceTag()", 2/>
</#if>
    }

    static View<${fullName}> read(BitStreamReader&<#if fieldList?has_content> reader</#if>, <#rt>
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
<#if fieldList?has_content>
        <@choice_expression_switch "choice_template_read_member", "choice_no_match_exception", viewIndirectSelectorExpression, 2/>
</#if>

        return view;
    }
<#if isPackable && usedInPackedArray>

    struct PackingContext
    {
    <#list fieldList as field>
        <#if field_needs_packing_context(field)>
        <@packing_context_type_name field/> <@packing_context_member_name field/>;
        </#if>
    </#list>
    };

    static void initContext(<#rt>
            PackingContext&<#if needs_packing_context(fieldList)> packingContext</#if>, <#t>
            <#lt>const View<${fullName}>&<#if needs_packing_context(fieldList)> view</#if>)
    {
    <#if fieldList?has_content>
        <@choice_switch "choice_init_context", "choice_no_match", "view.zserioChoiceTag()", 2, true/>
    </#if>
    }

    static BitSize bitSizeOf(<#rt>
            PackingContext&<#if needs_packing_context(fieldList)> packingContext</#if>, <#t>
            const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#t>
            <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
    {
    <#if fieldList?has_content>
        BitSize endBitPosition = bitPosition;
        <@choice_switch "choice_bitsizeof_field", "choice_no_match", "view.zserioChoiceTag()", 2, true/>

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
    <#if fieldList?has_content>
        <@choice_switch "choice_write_field", "choice_no_match", "view.zserioChoiceTag()", 2, true/>
    </#if>
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
    <#if fieldList?has_content>
        <@choice_expression_switch "choice_template_read_member", "choice_no_match_exception", viewIndirectSelectorExpression, 2, true/>
    </#if>
        (void)view;
    }
</#if>
<#if containsOffset>

    static BitSize initializeOffsets(
            const View<${fullName}>&<#if fieldList?has_content> view</#if><#rt>
            <#lt>, BitSize<#if fieldList?has_content> bitPosition</#if>)
    {
    <#if fieldList?has_content>
        BitSize endBitPosition = bitPosition;
        <@choice_switch "choice_initialize_offsets_field", "choice_no_match", "view.zserioChoiceTag()", 2/>

        return endBitPosition - bitPosition;
    <#else>
        return 0;
    </#if>
    }
    <#if isPackable && usedInPackedArray>

    static BitSize initializeOffsets(
            PackingContext&<#if needs_packing_context(fieldList)> packingContext</#if>,
            const View<${fullName}>&<#if fieldList?has_content> view</#if>, <#rt>
            <#lt>BitSize<#if fieldList?has_content> bitPosition</#if>)
    {
        <#if fieldList?has_content>
        BitSize endBitPosition = bitPosition;
        <@choice_switch "choice_initialize_offsets_field", "choice_no_match", "view.zserioChoiceTag()", 2, true/>

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

    <#list caseMemberList as caseMember>
        <@case_info_case_expressions_var caseMember, caseMember?index, 2/>
    </#list>
        <@case_info_array_var "cases" caseMemberList, defaultMember!, 2/>

        static const ::zserio::detail::ChoiceTypeInfo<AllocatorType> typeInfo = {
            "${schemaTypeName}",
            [](const AllocatorType& allocator) -> ${types.reflectablePtr.name}
            {
                return ::std::allocate_shared<::zserio::detail::ReflectableDataOwner<${fullName}>>(allocator);
            },
            templateName, templateArguments,
            fields, parameters, functions, "${selectorExpression?j_string}", cases
        };

        return typeInfo;
    }
};

<@template_definition templateParameters/>
struct Reflectable<${fullName}, ${types.allocator.default}>
{
    static ${types.reflectableConstPtr.name} create(
            const ${fullName}& value, const ${types.allocator.default}& allocator)
    {
        <@choice_reflectable true, 2, true/>
    }

    static ${types.reflectablePtr.name} create(
            ${fullName}& value, const ${types.allocator.default}& allocator)
    {
        <@choice_reflectable false, 2, true/>
    }
};

<@template_definition templateParameters/>
struct Introspectable<${fullName}, ${types.allocator.default}>
{
    static ${types.introspectableConstPtr.name} create(
            const View<${fullName}>& view, const ${types.allocator.default}& allocator)
    {
        <@choice_introspectable 2, true/>
    }
};
</#if>
<@namespace_end ["zserio", "detail"]/>
<@namespace_begin ["std"]/>

<@template_definition templateParameters/>
struct hash<${fullName}>
{
    size_t operator()(const ${fullName}&<#if fieldList?has_content> value</#if>) const
    {
        uint32_t result = ::zserio::HASH_SEED;
<#if fieldList?has_content>
        result = ::zserio::calcHashCode(result, static_cast<const typename ${fullName}::Base&>(value));
</#if>
        return static_cast<size_t>(result);
    }
};

<@template_definition templateParameters/>
struct hash<::zserio::View<${fullName}>>
{
    size_t operator()(<#rt>
            <#lt>const ::zserio::View<${fullName}>&<#if parameterList?has_content || fieldList?has_content> view</#if>) const
    {
        uint32_t result = ::zserio::HASH_SEED;
<#list parameterList as parameter>
        result = ::zserio::calcHashCode(result, view.${parameter.getterName}());
</#list>
<#if fieldList?has_content>
        <@choice_switch "choice_hash_field", "choice_no_match", "view.zserioChoiceTag()", 2/>

</#if>
        return static_cast<size_t>(result);
    }
};
<@namespace_end ["std"]/>

<@include_guard_end package.path, name/>
