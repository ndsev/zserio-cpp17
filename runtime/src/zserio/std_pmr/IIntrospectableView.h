#ifndef ZSERIO_STD_PMR_I_INTROSPECTABLE_VIEW_H_INC
#define ZSERIO_STD_PMR_I_INTROSPECTABLE_VIEW_H_INC

#include <memory_resource>

#include "zserio/IIntrospectableView.h"

// needed to have proper typedefs
#include "zserio/std_pmr/Any.h"
#include "zserio/std_pmr/BitBuffer.h"
#include "zserio/std_pmr/String.h"

namespace zserio
{
namespace std_pmr
{

/**
 * Typedef to reflectable interface provided for convenience - using std::pmr::polymorphic_allocator<uint8_t>.
 */
/** \{ */
using IIntrospectableView = IBasicIntrospectableView<std::pmr::polymorphic_allocator<uint8_t>>;
using IIntrospectableViewConstPtr = IBasicIntrospectableViewConstPtr<std::pmr::polymorphic_allocator<uint8_t>>;
/** \} */

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_I_INTROSPECTABLE_VIEW_H_INC
