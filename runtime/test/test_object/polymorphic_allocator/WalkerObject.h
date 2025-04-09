/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#ifndef TEST_OBJECT_POLYMORPHIC_ALLOCATOR_WALKER_OBJECT_H
#define TEST_OBJECT_POLYMORPHIC_ALLOCATOR_WALKER_OBJECT_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 2000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.2.0.
#endif

#include <memory>
#include <zserio/pmr/Optional.h>
#include <zserio/pmr/ITypeInfo.h>
#include <zserio/pmr/IReflectableData.h>
#include <zserio/pmr/IIntrospectableView.h>
#include <zserio/View.h>
#include <zserio/pmr/PropagatingPolymorphicAllocator.h>
#include <zserio/ArrayView.h>
#include <zserio/Types.h>
#include <zserio/pmr/String.h>
#include <zserio/pmr/Vector.h>

#include <test_object/polymorphic_allocator/WalkerChoice.h>
#include <test_object/polymorphic_allocator/WalkerNested.h>
#include <test_object/polymorphic_allocator/WalkerUnion.h>

namespace test_object
{
namespace polymorphic_allocator
{

struct WalkerObject
{
    using allocator_type = ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

    WalkerObject() noexcept;
    explicit WalkerObject(const allocator_type& allocator) noexcept;

    WalkerObject(WalkerObject&&) = default;
    WalkerObject(WalkerObject&& other_, const allocator_type& allocator);

    WalkerObject(const WalkerObject&) = default;
    WalkerObject(const WalkerObject& other_, const allocator_type& allocator);

    WalkerObject& operator=(WalkerObject&&) = default;
    WalkerObject& operator=(const WalkerObject&) = default;

    ~WalkerObject() = default;

    explicit WalkerObject(
            ::zserio::UInt32 identifier_,
            ::zserio::pmr::Optional<::test_object::polymorphic_allocator::WalkerNested> nested_,
            ::zserio::pmr::String text_,
            ::zserio::pmr::Vector<::test_object::polymorphic_allocator::WalkerUnion> unionArray_,
            ::zserio::pmr::Optional<::zserio::pmr::Vector<::test_object::polymorphic_allocator::WalkerUnion>> optionalUnionArray_,
            ::zserio::UInt8 choiceSelector_,
            ::test_object::polymorphic_allocator::WalkerChoice choiceField_,
            const allocator_type& allocator = {});

    ::zserio::UInt32 identifier;
    ::zserio::pmr::Optional<::test_object::polymorphic_allocator::WalkerNested> nested;
    ::zserio::pmr::String text;
    ::zserio::pmr::Vector<::test_object::polymorphic_allocator::WalkerUnion> unionArray;
    ::zserio::pmr::Optional<::zserio::pmr::Vector<::test_object::polymorphic_allocator::WalkerUnion>> optionalUnionArray;
    ::zserio::UInt8 choiceSelector;
    ::test_object::polymorphic_allocator::WalkerChoice choiceField;
};

bool operator==(const ::test_object::polymorphic_allocator::WalkerObject& lhs, const ::test_object::polymorphic_allocator::WalkerObject& rhs);
bool operator<(const ::test_object::polymorphic_allocator::WalkerObject& lhs, const ::test_object::polymorphic_allocator::WalkerObject& rhs);
bool operator!=(const ::test_object::polymorphic_allocator::WalkerObject& lhs, const ::test_object::polymorphic_allocator::WalkerObject& rhs);
bool operator>(const ::test_object::polymorphic_allocator::WalkerObject& lhs, const ::test_object::polymorphic_allocator::WalkerObject& rhs);
bool operator<=(const ::test_object::polymorphic_allocator::WalkerObject& lhs, const ::test_object::polymorphic_allocator::WalkerObject& rhs);
bool operator>=(const ::test_object::polymorphic_allocator::WalkerObject& lhs, const ::test_object::polymorphic_allocator::WalkerObject& rhs);

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::polymorphic_allocator::WalkerObject>
{
public:
    explicit View(const ::test_object::polymorphic_allocator::WalkerObject& data) noexcept;

    ::zserio::UInt32 identifier() const;
    ::zserio::pmr::Optional<View<::test_object::polymorphic_allocator::WalkerNested>> nested() const;
    ::std::string_view text() const;
    ArrayView<const ::test_object::polymorphic_allocator::WalkerUnion> unionArray() const;
    ::zserio::pmr::Optional<ArrayView<const ::test_object::polymorphic_allocator::WalkerUnion>> optionalUnionArray() const;
    ::zserio::UInt8 choiceSelector() const;
    View<::test_object::polymorphic_allocator::WalkerChoice> choiceField() const;

    const ::test_object::polymorphic_allocator::WalkerObject& zserioData() const;

protected:
    View(const ::test_object::polymorphic_allocator::WalkerObject& data, const View& other) noexcept;

private:
    const ::test_object::polymorphic_allocator::WalkerObject* m_data;
};

bool operator==(const View<::test_object::polymorphic_allocator::WalkerObject>& lhs, const View<::test_object::polymorphic_allocator::WalkerObject>& rhs);
bool operator<(const View<::test_object::polymorphic_allocator::WalkerObject>& lhs, const View<::test_object::polymorphic_allocator::WalkerObject>& rhs);
bool operator!=(const View<::test_object::polymorphic_allocator::WalkerObject>& lhs, const View<::test_object::polymorphic_allocator::WalkerObject>& rhs);
bool operator>(const View<::test_object::polymorphic_allocator::WalkerObject>& lhs, const View<::test_object::polymorphic_allocator::WalkerObject>& rhs);
bool operator<=(const View<::test_object::polymorphic_allocator::WalkerObject>& lhs, const View<::test_object::polymorphic_allocator::WalkerObject>& rhs);
bool operator>=(const View<::test_object::polymorphic_allocator::WalkerObject>& lhs, const View<::test_object::polymorphic_allocator::WalkerObject>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::polymorphic_allocator::WalkerObject>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::polymorphic_allocator::WalkerObject>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::polymorphic_allocator::WalkerObject>& view);

template <>
View<::test_object::polymorphic_allocator::WalkerObject> read(BitStreamReader& reader, ::test_object::polymorphic_allocator::WalkerObject& data);

template <>
struct TypeInfo<::test_object::polymorphic_allocator::WalkerObject, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
{
    static const ::zserio::pmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::pmr::IReflectableDataConstPtr reflectable(const ::test_object::polymorphic_allocator::WalkerObject& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::pmr::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::WalkerObject& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::pmr::IIntrospectableViewConstPtr introspectable(const View<::test_object::polymorphic_allocator::WalkerObject>& view, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::polymorphic_allocator::WalkerObject>
{
    size_t operator()(const ::test_object::polymorphic_allocator::WalkerObject& data) const;
};

template <>
struct hash<::zserio::View<::test_object::polymorphic_allocator::WalkerObject>>
{
    size_t operator()(const ::zserio::View<::test_object::polymorphic_allocator::WalkerObject>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_POLYMORPHIC_ALLOCATOR_WALKER_OBJECT_H
