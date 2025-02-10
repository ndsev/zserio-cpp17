#ifndef ZSERIO_DATA_VIEW_H_INC
#define ZSERIO_DATA_VIEW_H_INC

#include <zserio/BitStreamReader.h>
#include <zserio/View.h>

namespace zserio
{

/**
 * DataView utility class which has interface of View but includes data as well.
 * Data access is read only. This utility is useful to manage data lifetime while working with view.
 */
template <class T>
class DataView : public View<T>
{
public:
    /**
     * Read constructor used from deserialize (doesn't need to do validation).
     *
     * \param reader Bit stream reader.
     * \param data Data structure.
     * \param arguments Parameters in case of parameterized types.
     */
    template <typename... ARGS>
    DataView(BitStreamReader& reader, T&& data, ARGS&&... arguments) :
            View<T>(m_ownData, arguments...),
            m_ownData(std::move(data))
    {
        detail::read(reader, m_ownData, std::forward<ARGS>(arguments)...);
    }

    /**
     * Data constructor which performs validation.
     *
     * \param data Data structure l-value to be copied into the new DataView.
     * \param arguments Parameters in case of parameterized types.
     */
    template <typename... ARGS>
    explicit DataView(const T& data, ARGS&&... arguments) :
            View<T>(m_ownData, std::forward<ARGS>(arguments)...),
            m_ownData(data)
    {
        detail::validate(*this);
        (void)detail::initializeOffsets(*this, 0);
    }

    /**
     * Data constructor which performs validation.
     *
     * \param data Data structure r-value to be moved into the new DataView.
     * \param arguments Parameters in case of parameterized types.
     */
    template <typename... ARGS>
    explicit DataView(T&& data, ARGS&&... arguments) :
            View<T>(m_ownData, std::forward<ARGS>(arguments)...),
            m_ownData(std::move(data))
    {
        detail::validate(*this);
        (void)detail::initializeOffsets(*this, 0);
    }

    /**
     * Copy constructor.
     *
     * \param other Other data view to copy construct from.
     */
    DataView(const DataView& other) :
            View<T>(m_ownData, other),
            m_ownData(other.m_ownData)
    {}

    /**
     * Move constructor.
     *
     * \param other Other data view to move construct from.
     */
    DataView(DataView&& other) :
            View<T>(m_ownData, other),
            m_ownData(std::move(other.m_ownData))
    {}

    /**
     * Explicitly deleted assignment operator.
     */
    /** \{ */
    DataView& operator=(const DataView& other) = delete;
    DataView& operator=(DataView&& other) = delete;
    /** \} */

    ~DataView() = default;

private:
    T m_ownData;
};

// template argument deduction guide for DataView constructor
template <typename T, typename... ARGS>
DataView(T, ARGS&&...) -> DataView<T>;

} // namespace zserio

#endif // ZSERIO_DATA_VIEW_H_INC
