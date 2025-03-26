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
<@type_includes types.optional/>
</#if>
<#if withTypeInfoCode>
<@type_includes types.typeInfo/>
<@type_includes types.reflectablePtr/>
</#if>
#include <zserio/View.h>
<@type_includes types.allocator/>
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
            <@structure_field_ctor_type_name field/> <@field_data_arg_name field/><#if field?has_next>,<#else>);</#if>
    </#items>
</#list>
<#list fieldList>

    <#items as field>
        <#if field.usedAsOffset>mutable </#if><@structure_field_data_type_name field/> <@field_data_member_name field/>;
    </#items>
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
    <@array_traits_declaration fullName, fieldList/>
    <@structure_offset_setters_declaration fullName, fieldList/>
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

    const ${fullName}& zserioData() const;

protected:
    View(const ${fullName}& data, const View& other) noexcept;

private:
    const ${fullName}* m_data;
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
void validate(const View<${fullName}>& view, ::std::string_view fieldName);

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
<#if containsOffset>

template <>
struct OffsetsInitializer<${fullName}>
{
    static BitSize initialize(const View<${fullName}>& view, BitSize bitPosition);
    <#if isPackable && usedInPackedArray>
    static BitSize initialize(PackingContext<${fullName}>& packingContext,
            const View<${fullName}>& view, BitSize bitPosition);
    </#if>
};
</#if>
<#if withTypeInfoCode>

template <>
struct TypeInfo<${fullName}, ${types.allocator.default}>
{
    static const ${types.typeInfo.name}& get();
};
<@namespace_end ["detail"]/>

template <>
${types.reflectableConstPtr.name} reflectable(const ${fullName}& value, const ${types.allocator.default}& allocator);

template <>
${types.reflectablePtr.name} reflectable(${fullName}& value, const ${types.allocator.default}& allocator);
<@namespace_end ["zserio"]/>
<#else>
<@namespace_end ["zserio", "detail"]/>
</#if>
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
