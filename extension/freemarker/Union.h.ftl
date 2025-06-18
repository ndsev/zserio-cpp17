<#include "FileHeader.inc.ftl">
<#include "DocComment.inc.ftl">
<#include "CompoundField.inc.ftl">
<#include "CompoundFunction.inc.ftl">
<#include "CompoundParameter.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <memory>

<@type_includes types.variant/>
#include <zserio/ChoiceTag.h>
<#if withTypeInfoCode>
<@type_includes types.typeInfo/>
<@type_includes types.reflectablePtr/>
<@type_includes types.introspectableConstPtr/>
</#if>
#include <zserio/View.h>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>

// forward declaration
struct ${name};
<@namespace_end package.path/>
<@namespace_begin ["zserio", "detail"]/>

template <>
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
struct ${name} : <@variant_type_name_begin/>::zserio::detail::ChoiceTag<${name}>::Tag,
        ::std::monostate<#rt>
<#list fieldList as field>
        <#lt>,
        <#nt><@field_data_type_name field/><#rt>
</#list>
        <#lt>>
{
    using Tag = ::zserio::detail::ChoiceTag<${name}>::Tag;
    using Base = <@variant_type_name_begin/>Tag,
            ::std::monostate<#rt>
<#list fieldList as field>
            <#lt>,
            <#nt><@field_data_type_name field/><#rt>
</#list>
            <#lt>>;
    using Base::Base;
};

bool operator==(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator<(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator!=(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator>(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator<=(const ${fullName}& lhs, const ${fullName}& rhs);
bool operator>=(const ${fullName}& lhs, const ${fullName}& rhs);
<@namespace_end package.path/>
<@namespace_begin ["zserio"]/>

template <>
class View<${fullName}>
{
public:
    <@array_traits_declaration fullName, fieldList/>
    explicit View(<#if !usedAsOffset>const </#if>${fullName}& data<#rt>
<#list parameterList as parameter>
            <#lt>,
            <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
</#list>
            <#lt>) noexcept;
<#list parameterList>

    <#items as parameter>
    <@parameter_view_type_name parameter/> ${parameter.getterName}() const;
    </#items>
</#list>
<#list fieldList>

    <#items as field>
    <@field_view_type_name field/> ${field.getterName}() const;
    </#items>
</#list>
<#list functionList>

    <#items as function>
    <@function_return_type_name function/> ${function.name}() const;
    </#items>
</#list>

    ${fullName}::Tag zserioChoiceTag() const;
    const ${fullName}& zserioData() const;

protected:
    View(<#if !usedAsOffset>const </#if>${fullName}& data, const View& other) noexcept;

private:
    <#if !usedAsOffset>const </#if>${fullName}* m_data;
<#list parameterList as parameter>
    <@parameter_view_type_name parameter/> <@parameter_view_member_name parameter/>;
</#list>
};

bool operator==(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
bool operator<(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
bool operator!=(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
bool operator>(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
bool operator<=(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
bool operator>=(const View<${fullName}>& lhs, const View<${fullName}>& rhs);
<@namespace_begin ["detail"]/>

template <>
struct ObjectTraits<${fullName}>
{
<#if parameterList?has_content>
    <@parameter_traits parameterList, 1/>

</#if>
    static void validate(const View<${fullName}>& view, ::std::string_view fieldName);

    static BitSize bitSizeOf(const View<${fullName}>& view, BitSize bitPosition);

    static void write(BitStreamWriter& writer, const View<${fullName}>& view);

    static View<${fullName}> read(BitStreamReader& reader, ${fullName}& data<#rt>
<#list parameterList as parameter>
            <#lt>,
            <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
</#list>
            <#lt>);
<#if isPackable && usedInPackedArray>

    struct PackingContext
    {
        DeltaContext zserioChoiceTag;
    <#list fieldList as field>
        <#if field_needs_packing_context(field)>
        <@packing_context_type_name field/> <@packing_context_member_name field/>;
        </#if>
    </#list>
    };

    static void initContext(PackingContext<${fullName}>& packingContext, const View<${fullName}>& view);

    static BitSize bitSizeOf(PackingContext<${fullName}>& packingContext, const View<${fullName}>& view,
            BitSize bitPosition);

    static void write(PackingContext<${fullName}>& packingContext, BitStreamWriter& writer,
            const View<${fullName}>& view);

    static void read(PackingContext<${fullName}>& packingContext, BitStreamReader& reader, ${fullName}& data<#rt>
    <#list parameterList as parameter>
            <#lt>,
            <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
    </#list>
            <#lt>);
</#if>
<#if containsOffset>

    static BitSize initializeOffsets(const View<${fullName}>& view, BitSize bitPosition);
    <#if isPackable && usedInPackedArray>
    static BitSize initializeOffsets(PackingConext<${fullName}>& packingContext,
            const View<${fullName}>& view, , BitSize bitPosition);
    </#if>
</#if>
};
<#if withTypeInfoCode>

template <>
struct TypeInfo<${fullName}, ${types.allocator.default}>
{
    static const ${types.typeInfo.name}& get();
};

template <>
struct Reflectable<${fullName}, ${types.allocator.default}>
{
    static ${types.reflectableConstPtr.name} create(
            const ${fullName}& value, const ${types.allocator.default}& allocator);

    static ${types.reflectablePtr.name} create(
            ${fullName}& value, const ${types.allocator.default}& allocator);
};

template <>
struct Introspectable<${fullName}, ${types.allocator.default}>
{
    static ${types.introspectableConstPtr.name} create(
            const View<${fullName}>& view, const ${types.allocator.default}& allocator);
};
</#if>
<@namespace_end ["zserio", "detail"]/>
<@namespace_begin ["std"]/>

template <>
struct hash<${fullName}>
{
    size_t operator()(const ${fullName}& value) const;
};

template <>
struct hash<::zserio::View<${fullName}>>
{
    size_t operator()(const ::zserio::View<${fullName}>& view) const;
};
<@namespace_end ["std"]/>

<@include_guard_end package.path, name/>
