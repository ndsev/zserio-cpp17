#ifndef ZSERIO_DATA_VIEW_H_INC
#define ZSERIO_DATA_VIEW_H_INC

#include <zserio/View.h>

namespace zserio
{

namespace detail
{

struct SkipValidationT
{
    constexpr explicit SkipValidationT() = default;
};

constexpr SkipValidationT SkipValidation{};

} // namespace detail

// DataView has interface of View but includes data as well.
// data access is read only
template <class T>
class DataView : public View<T>
{
public:
    // tagged data constructor for reader without validation
    template <typename... ARGS>
    DataView(const T& data, detail::SkipValidationT, ARGS&&... arguments) :
            View<T>(m_data, ::std::forward<ARGS>(arguments)...),
            m_data(data)
    {}

    // data constructor with validation
    template <typename... ARGS>
    DataView(const T& data, ARGS&&... arguments) :
            View<T>(m_data, ::std::forward<ARGS>(arguments)...),
            m_data(data)
    {
        detail::initializeOffsets(m_data, 0, ::std::forward<ARGS>(arguments)...);
        detail::validate(this->view());
    }

    // moved data ctor with validation
    template <typename... ARGS, typename = ::std::enable_if_t<!::std::is_lvalue_reference_v<T>>>
    DataView(T&& data, ARGS&&... arguments) :
            View<T>(m_data, ::std::forward<ARGS>(arguments)...),
            m_data(::std::move(data))
    {
        detail::validate(this->view());
    }

    // copy ctor
    DataView(const DataView& sv) :
            View<T>(m_data),
            m_data(sv.m_data)
    {}

    // move ctor
    DataView(DataView&& sv) :
            View<T>(m_data),
            m_data(::std::move(sv.m_data))
    {}

    // assignment
    DataView& operator=(const DataView&& sv)
    {
        m_data = sv.m_data;
        return *this;
    }

    // move assignment
    DataView& operator=(DataView&& sv)
    {
        m_data = ::std::move(sv.m_data);
        return *this;
    }

    // to be used in templated call overload resolution
    View<T>& view()
    {
        return static_cast<View<T>&>(*this);
    }
    const View<T>& view() const
    {
        return static_cast<const View<T>&>(*this);
    }

    // data accessors
    const T& data() const
    {
        return m_data;
    }

    // this may be more convenient than data()
    const T* operator->() const
    {
        return &m_data;
    }
    const T& operator*() const
    {
        return m_data;
    }

private:
    T m_data;
};

} // namespace zserio

#endif // ZSERIO_DATA_VIEW_H_INC
