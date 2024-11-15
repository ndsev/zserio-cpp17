<#include "FileHeader.inc.ftl">
<#include "DocComment.inc.ftl">
<#include "Structure.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <memory>
<#if isPackable && usedInPackedArray>
#include <zserio/DeltaContext.h>
</#if>
<#if structure_has_optional_field(fieldList)>
#include <zserio/Optional.h>
</#if>
#include <zserio/View.h>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>

<#if docComments??>
<@doc_comments docComments/>
</#if>
struct ${name}
{
    using AllocatorType = ${types.allocator.default};
<#if structure_has_recursive_optional_field(fieldList)>
    using IS_RECURSIVE = void;
</#if>

    ${name}() noexcept;
    explicit ${name}(const AllocatorType& allocator) noexcept;
<#list fieldList>

    explicit ${name}(
    <#items as field>
        <@structure_field_data_type_name field/> <@field_data_arg_name field/><#if field?has_next>,<#else>) noexcept;</#if>
    </#items>
</#list>

<#list fieldList as field>
    <@structure_field_data_type_name field/> <@field_data_member_name field/>;
</#list>
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
<#list fieldList as field>
    <#if array_needs_custom_traits(field)>
    struct <@array_traits_name field/>
    {
        <#if array_needs_owner(field)>
        using OwnerType = View<${fullName}>;

        </#if>
        static View<${field.typeInfo.typeFullName}> at(<#rt>
                <#lt>const <#if array_needs_owner(field)>OwnerType& owner<#else>detail::DummyArrayOwner&</#if>,
                const ${field.typeInfo.typeFullName}& element, size_t<#if array_needs_index(field)> index</#if>)
        {
            return View<${field.typeInfo.typeFullName}>(element<@field_view_owner_indirect_parameters field/>);
        }

        static void read(BitStreamReader& reader,
                const <#if array_needs_owner(field)>OwnerType& owner<#else>detail::DummyArrayOwner&</#if>, <#rt>
                <#lt>${field.typeInfo.typeFullName}& element, size_t<#if array_needs_index(field)> index</#if>)
        {
            (void)detail::read(reader, element<@field_view_owner_indirect_parameters field/>);
        }
        <#if field.isPackable && (field.array.isPacked || usedInPackedArray)>
        static void read(<@packing_context_type_name field, true/>& packingContext, BitStreamReader& reader,
                const <#if array_needs_owner(field)>OwnerType& owner<#else>detail::DummyArrayOwner&</#if>, <#rt>
                <#lt>${field.typeInfo.typeFullName}& element, size_t<#if array_needs_index(field)> index</#if>)
        {
            detail::read(packingContext, reader, element<@field_view_owner_indirect_parameters field/>);
        }
        </#if>
    };

    </#if>
</#list>
    explicit View(const ${fullName}& data<#rt>
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
    <@structure_field_view_type_name field/> ${field.getterName}() const;
    </#items>
</#list>
<#list functionList>

    <#items as function>
    <@function_return_type_name function/> ${function.name}() const;
    </#items>
</#list>

private:
<#if fieldList?has_content>
    const ${fullName}& m_data;
</#if>
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
void validate(const View<${fullName}>& view);

template <>
BitSize bitSizeOf(const View<${fullName}>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<${fullName}>& view);

template <>
View<${fullName}> read(BitStreamReader& reader, ${fullName}& data<#rt>
<#list parameterList as parameter>
        <#lt>,
        <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
</#list>
        <#lt>);
<#if isPackable && usedInPackedArray>

template <>
struct PackingContext<${fullName}>
{
<#list fieldList as field>
    <#if field_needs_packing_context(field) && !(field.optional?? && field.optional.isRecursive)>
    <@packing_context_type_name field/> <@packing_context_member_name field/>;
    </#if>
</#list>
};

template <>
void initContext(PackingContext<${fullName}>& packingContext, const View<${fullName}>& view);

template <>
BitSize bitSizeOf(PackingContext<${fullName}>& packingContext, const View<${fullName}>& view,
        BitSize bitPosition);

template <>
void write(PackingContext<${fullName}>& packingContext, BitStreamWriter& writer, const View<${fullName}>& view);

template <>
void read(PackingContext<${fullName}>& packingContext, BitStreamReader& reader, ${fullName}& data<#rt>
    <#list parameterList as parameter>
        <#lt>,
        <@parameter_view_type_name parameter/> <@parameter_view_arg_name parameter/><#rt>
    </#list>
        <#lt>);
</#if>
<@namespace_end ["detail"]/>
<@namespace_end ["zserio"]/>
<@namespace_begin ["std"]/>

template <>
struct hash<${fullName}>
{
    size_t operator()(const ${fullName}& data) const;
};

template <>
struct hash<::zserio::View<${fullName}>>
{
    size_t operator()(const ::zserio::View<${fullName}>& view) const;
};
<@namespace_end ["std"]/>

<@include_guard_end package.path, name/>
