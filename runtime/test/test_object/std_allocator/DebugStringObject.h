/**
 * Automatically generated by Zserio C++17 generator version 0.5.0 using Zserio core 2.17.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_DEBUG_STRING_OBJECT_H
#define TEST_OBJECT_STD_ALLOCATOR_DEBUG_STRING_OBJECT_H

#include <memory>
#include <zserio/ITypeInfo.h>
#include <zserio/IReflectableData.h>
#include <zserio/IIntrospectableView.h>
#include <zserio/View.h>
#include <memory>
#include <zserio/String.h>

namespace test_object
{
namespace std_allocator
{

struct DebugStringObject
{
    using allocator_type = ::std::allocator<uint8_t>;

    DebugStringObject() noexcept;
    explicit DebugStringObject(const allocator_type& allocator) noexcept;

    DebugStringObject(DebugStringObject&&) = default;
    DebugStringObject(DebugStringObject&& other, const allocator_type& allocator);

    DebugStringObject(const DebugStringObject&) = default;
    DebugStringObject(const DebugStringObject& other, const allocator_type& allocator);

    DebugStringObject& operator=(DebugStringObject&&) = default;
    DebugStringObject& operator=(const DebugStringObject&) = default;

    ~DebugStringObject() = default;

    explicit DebugStringObject(
            ::zserio::String text_,
            const allocator_type& allocator = {});

    ::zserio::String text;
};

bool operator==(const ::test_object::std_allocator::DebugStringObject& lhs, const ::test_object::std_allocator::DebugStringObject& rhs);
bool operator<(const ::test_object::std_allocator::DebugStringObject& lhs, const ::test_object::std_allocator::DebugStringObject& rhs);
bool operator!=(const ::test_object::std_allocator::DebugStringObject& lhs, const ::test_object::std_allocator::DebugStringObject& rhs);
bool operator>(const ::test_object::std_allocator::DebugStringObject& lhs, const ::test_object::std_allocator::DebugStringObject& rhs);
bool operator<=(const ::test_object::std_allocator::DebugStringObject& lhs, const ::test_object::std_allocator::DebugStringObject& rhs);
bool operator>=(const ::test_object::std_allocator::DebugStringObject& lhs, const ::test_object::std_allocator::DebugStringObject& rhs);

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::std_allocator::DebugStringObject>
{
public:
    explicit View(const ::test_object::std_allocator::DebugStringObject& data) noexcept;

    ::std::string_view text() const;

    const ::test_object::std_allocator::DebugStringObject& zserioData() const;

protected:
    View(const ::test_object::std_allocator::DebugStringObject& data, const View& other) noexcept;

private:
    const ::test_object::std_allocator::DebugStringObject* m_data;
};

bool operator==(const View<::test_object::std_allocator::DebugStringObject>& lhs, const View<::test_object::std_allocator::DebugStringObject>& rhs);
bool operator<(const View<::test_object::std_allocator::DebugStringObject>& lhs, const View<::test_object::std_allocator::DebugStringObject>& rhs);
bool operator!=(const View<::test_object::std_allocator::DebugStringObject>& lhs, const View<::test_object::std_allocator::DebugStringObject>& rhs);
bool operator>(const View<::test_object::std_allocator::DebugStringObject>& lhs, const View<::test_object::std_allocator::DebugStringObject>& rhs);
bool operator<=(const View<::test_object::std_allocator::DebugStringObject>& lhs, const View<::test_object::std_allocator::DebugStringObject>& rhs);
bool operator>=(const View<::test_object::std_allocator::DebugStringObject>& lhs, const View<::test_object::std_allocator::DebugStringObject>& rhs);

namespace detail
{

template <>
struct ObjectTraits<::test_object::std_allocator::DebugStringObject>
{
    static void validate(const View<::test_object::std_allocator::DebugStringObject>& view, ::std::string_view fieldName);

    static BitSize bitSizeOf(const View<::test_object::std_allocator::DebugStringObject>& view, BitSize bitPosition);

    static void write(BitStreamWriter& writer, const View<::test_object::std_allocator::DebugStringObject>& view);

    static View<::test_object::std_allocator::DebugStringObject> read(BitStreamReader& reader, ::test_object::std_allocator::DebugStringObject& data);
};

template <>
struct TypeInfo<::test_object::std_allocator::DebugStringObject, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

template <>
struct Reflectable<::test_object::std_allocator::DebugStringObject, ::std::allocator<uint8_t>>
{
    static ::zserio::IReflectableDataConstPtr create(const ::test_object::std_allocator::DebugStringObject& value,
            const ::std::allocator<uint8_t>& allocator);

    static ::zserio::IReflectableDataPtr create(::test_object::std_allocator::DebugStringObject& value,
            const ::std::allocator<uint8_t>& allocator);
};

template <>
struct Introspectable<::test_object::std_allocator::DebugStringObject, ::std::allocator<uint8_t>>
{
    static ::zserio::IIntrospectableViewConstPtr create(const View<::test_object::std_allocator::DebugStringObject>& view,
            const ::std::allocator<uint8_t>& allocator);
};

} // namespace detail
} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_allocator::DebugStringObject>
{
    size_t operator()(const ::test_object::std_allocator::DebugStringObject& data) const;
};

template <>
struct hash<::zserio::View<::test_object::std_allocator::DebugStringObject>>
{
    size_t operator()(const ::zserio::View<::test_object::std_allocator::DebugStringObject>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_ALLOCATOR_DEBUG_STRING_OBJECT_H
