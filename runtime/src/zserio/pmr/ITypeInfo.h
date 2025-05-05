#ifndef ZSERIO_PMR_I_TYPE_INFO_H_INC
#define ZSERIO_PMR_I_TYPE_INFO_H_INC

#include <memory_resource>

#include "zserio/ITypeInfo.h"

namespace zserio
{
namespace pmr
{

/**
 * Global function for type info of a generated type provided via specializations.
 *
 * \return Type info.
 */
template <typename T, typename ALLOC = std::pmr::polymorphic_allocator<uint8_t>>
const IBasicTypeInfo<ALLOC>& typeInfo()
{
    return detail::TypeInfo<T, ALLOC>::get();
}

/** Typedef provided for convenience - using default std::pmr::polymorphic_allocator<uint8_t>. */
/** \{ */
using ITypeInfo = IBasicTypeInfo<std::pmr::polymorphic_allocator<uint8_t>>;
using FieldInfo = BasicFieldInfo<std::pmr::polymorphic_allocator<uint8_t>>;
using ParameterInfo = BasicParameterInfo<std::pmr::polymorphic_allocator<uint8_t>>;
using FunctionInfo = BasicFunctionInfo<std::pmr::polymorphic_allocator<uint8_t>>;
using CaseInfo = BasicCaseInfo<std::pmr::polymorphic_allocator<uint8_t>>;
using ColumnInfo = BasicColumnInfo<std::pmr::polymorphic_allocator<uint8_t>>;
using TableInfo = BasicTableInfo<std::pmr::polymorphic_allocator<uint8_t>>;
using TemplateArgumentInfo = BasicTemplateArgumentInfo<std::pmr::polymorphic_allocator<uint8_t>>;
using MessageInfo = BasicMessageInfo<std::pmr::polymorphic_allocator<uint8_t>>;
using MethodInfo = BasicMethodInfo<std::pmr::polymorphic_allocator<uint8_t>>;
/** \} */

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_I_TYPE_INFO_H_INC
