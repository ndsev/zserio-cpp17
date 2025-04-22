#ifndef ZSERIO_PPMR_I_TYPE_INFO_H_INC
#define ZSERIO_PPMR_I_TYPE_INFO_H_INC

#include "zserio/ITypeInfo.h"
#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace ppmr
{

/**
 * Global function for type info of a generated type provided via specializations.
 *
 * \return Type info.
 */
template <typename T, typename ALLOC = PropagatingPolymorphicAllocator<uint8_t>>
const IBasicTypeInfo<ALLOC>& typeInfo()
{
    return detail::TypeInfo<T, ALLOC>::get();
}

/** Typedef provided for convenience - using default PropagatingPolymorphicAllocator<uint8_t>. */
/** \{ */
using ITypeInfo = IBasicTypeInfo<PropagatingPolymorphicAllocator<uint8_t>>;
using FieldInfo = BasicFieldInfo<PropagatingPolymorphicAllocator<uint8_t>>;
using ParameterInfo = BasicParameterInfo<PropagatingPolymorphicAllocator<uint8_t>>;
using FunctionInfo = BasicFunctionInfo<PropagatingPolymorphicAllocator<uint8_t>>;
using CaseInfo = BasicCaseInfo<PropagatingPolymorphicAllocator<uint8_t>>;
using ColumnInfo = BasicColumnInfo<PropagatingPolymorphicAllocator<uint8_t>>;
using TableInfo = BasicTableInfo<PropagatingPolymorphicAllocator<uint8_t>>;
using TemplateArgumentInfo = BasicTemplateArgumentInfo<PropagatingPolymorphicAllocator<uint8_t>>;
using MessageInfo = BasicMessageInfo<PropagatingPolymorphicAllocator<uint8_t>>;
using MethodInfo = BasicMethodInfo<PropagatingPolymorphicAllocator<uint8_t>>;
/** \} */

} // namespace ppmr
} // namespace zserio

#endif // ZSERIO_PPMR_I_TYPE_INFO_H_INC
