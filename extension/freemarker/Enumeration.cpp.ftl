<#include "FileHeader.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/StringConvertUtil.h>
<@system_includes cppSystemIncludes/>

<@user_include package.path, "${name}.h"/>
<@user_includes cppUserIncludes, false/>
<@namespace_begin ["zserio"]/>

template <>
size_t enumToOrdinal(${fullName} value)
{
    switch (value)
    {
<#list items as item>
    case ${item.fullName}:
        return ${item?index};
</#list>
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration ${name}: ") <<
                static_cast<typename ::std::underlying_type_t<${fullName}>>(value) << "!";
    }
}

template <>
${fullName} valueToEnum(${underlyingTypeInfo.typeFullName} rawValue)
{
    switch (rawValue)
    {
<#list items as item>
    case ${item.value}:
</#list>
        return static_cast<${fullName}>(static_cast<::std::underlying_type_t<${fullName}>>(rawValue));
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration ${name}: ") << rawValue << "!";
    }
}
<@namespace_end ["zserio"]/>
<@namespace_begin ["std"]/>

size_t hash<${fullName}>::operator()(const ${fullName}& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result,
            static_cast<::std::underlying_type_t<${fullName}>>(::zserio::enumToValue(value)));
    return static_cast<size_t>(result);
}
<@namespace_end ["std"]/>
