#ifndef ZSERIO_ISERVICE_H_INC
#define ZSERIO_ISERVICE_H_INC

#include <string_view>

#include "zserio/SerializeUtil.h"
#include "zserio/Span.h"
#include "zserio/Types.h"
#include "zserio/Vector.h"
#include "zserio/View.h"

namespace zserio
{

/**
 * Service data interface.
 *
 * When reflectable interface is available, holds the reflectable object, otherwise holds the serialized data.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class IBasicServiceData
{
public:
    /** Destructor. */
    virtual ~IBasicServiceData() = default;

    /**
     * Gets service data as bytes.
     *
     * \return Service data.
     */
    virtual Span<const uint8_t> getData() const = 0;
};

/** Typedef to service data smart pointer needed for convenience in generated code. */
template <typename ALLOC = std::allocator<uint8_t>>
using IBasicServiceDataPtr = std::shared_ptr<IBasicServiceData<ALLOC>>;

/**
 * Service data implementation based on objects generated by Zserio.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BasicObjectServiceData : public IBasicServiceData<ALLOC>
{
public:
    /**
     * Constructor from zserio-generated object.
     *
     * \param object Reference to zserio object.
     * \param allocator Allocator to use for data allocation
     */
    template <typename ZSERIO_OBJECT>
    explicit BasicObjectServiceData(ZSERIO_OBJECT& object, const ALLOC& allocator = ALLOC()) :
            m_data(zserio::serialize(object, allocator))
    {}

    Span<const uint8_t> getData() const override
    {
        return m_data.getData();
    }

private:
    BasicBitBuffer<ALLOC> m_data;
};

/**
 * Service data implementation which owns the data.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BasicRawServiceDataHolder : public IBasicServiceData<ALLOC>
{
public:
    /**
     * Constructor from vector const l-value reference.
     *
     * \param rawData Raw data to be copied inside the service data.
     */
    explicit BasicRawServiceDataHolder(const Vector<uint8_t, ALLOC>& rawData) :
            m_data(rawData)
    {}

    /**
     * Constructor from vector r-value reference.
     *
     * \param rawData Raw data to be moved inside the service data.
     */
    explicit BasicRawServiceDataHolder(Vector<uint8_t, ALLOC>&& rawData) :
            m_data(std::move(rawData))
    {}

    Span<const uint8_t> getData() const override
    {
        return m_data;
    }

private:
    Vector<uint8_t, ALLOC> m_data;
};

/**
 * Service data implementation which only keeps pointer to the raw data buffer.
 *
 * Must be used with caution only when the proper life-time of the data is ensured.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BasicRawServiceDataView : public IBasicServiceData<ALLOC>
{
public:
    /**
     * Constructor from span.
     *
     * \param rawData Span as a pointer to the raw data buffer.
     */
    explicit BasicRawServiceDataView(zserio::Span<const uint8_t> rawData) :
            m_data(rawData)
    {}

    Span<const uint8_t> getData() const override
    {
        return m_data;
    }

private:
    Span<const uint8_t> m_data;
};

/**
 * Generic interface for all Zserio services to be used on the server side.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class IBasicService
{
public:
    virtual ~IBasicService() = default;

    /**
     * Calls method with the given name synchronously.
     *
     * \param methodName Name of the service method to call.
     * \param requestData Request data to be passed to the method.
     * \param context Context specific for particular service or nullptr in case of no context.
     *
     * \return Created response data.
     *
     * \throw ServiceException if the call fails.
     */
    virtual IBasicServiceDataPtr<ALLOC> callMethod(
            StringView methodName, Span<const uint8_t> requestData, void* context) = 0;
};

/**
 * Generic interface for all Zserio services to be used on the client side.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class IBasicServiceClient
{
public:
    virtual ~IBasicServiceClient() = default;

    /**
     * Calls method with the given name synchronously.
     *
     * \param methodName Name of the service method to call.
     * \param requestData Request data to be passed to the method.
     * \param context Context specific for particular service or nullptr in case of no context.
     *
     * \return Created response data as bytes.
     *
     * \throw ServiceException if the call fails.
     */
    virtual Vector<uint8_t, ALLOC> callMethod(
            StringView methodName, const IBasicServiceData<ALLOC>& requestData, void* context) = 0;
};

/** Typedef to service interface provided for convenience - using default std::allocator<uint8_t>. */
/** \{ */
using IServiceData = IBasicServiceData<>;
using IServiceDataPtr = IBasicServiceDataPtr<>;
using IService = IBasicService<>;
using IServiceClient = IBasicServiceClient<>;
/** \} */

/** Typedef to service data implementation provided for convenience - using default std::allocator<uint8_t>. */
/** \{ */
using ObjectServiceData = BasicObjectServiceData<>;
using RawServiceDataHolder = BasicRawServiceDataHolder<>;
using RawServiceDataView = BasicRawServiceDataView<>;
/** \} */

} // namespace zserio

#endif // ifndef ZSERIO_ISERVICE_H_INC