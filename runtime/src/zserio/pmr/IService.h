#ifndef ZSERIO_PMR_I_SERVICE_H_INC
#define ZSERIO_PMR_I_SERVICE_H_INC

#include <memory_resource>

#include "zserio/IService.h"

namespace zserio
{
namespace pmr
{

/** Typedef to service interface provided for convenience - using std::pmr::polymorphic_allocator<uint8_t>. */
/** \{ */
using IServiceData = IBasicServiceData<std::pmr::polymorphic_allocator<uint8_t>>;
using IServiceDataPtr = IBasicServiceDataPtr<std::pmr::polymorphic_allocator<uint8_t>>;
using IService = IBasicService<std::pmr::polymorphic_allocator<uint8_t>>;
using IServiceClient = IBasicServiceClient<std::pmr::polymorphic_allocator<uint8_t>>;
/** \} */

/**
 * Typedef to service data implementation provided for convenience - using
 * std::pmr::polymorphic_allocator<uint8_t>.
 */
template <typename ZSERIO_OBJECT>
using IntrospectableServiceData =
        BasicIntrospectableServiceData<ZSERIO_OBJECT, std::pmr::polymorphic_allocator<uint8_t>>;
using ObjectServiceData = BasicObjectServiceData<std::pmr::polymorphic_allocator<uint8_t>>;
using RawServiceDataHolder = BasicRawServiceDataHolder<std::pmr::polymorphic_allocator<uint8_t>>;
using RawServiceDataView = BasicRawServiceDataView<std::pmr::polymorphic_allocator<uint8_t>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_I_SERVICE_H_INC
