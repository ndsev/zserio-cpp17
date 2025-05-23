/**
 * Automatically generated by Zserio C++17 generator version 0.4.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, ppmrAllocator.
 */

#ifndef TEST_OBJECT_PPMR_ALLOCATOR_REFLECTABLE_UTIL_CHOICE_H
#define TEST_OBJECT_PPMR_ALLOCATOR_REFLECTABLE_UTIL_CHOICE_H

#include <memory>

#include <zserio/ppmr/Variant.h>
#include <zserio/ChoiceTag.h>
#include <zserio/ppmr/ITypeInfo.h>
#include <zserio/ppmr/IReflectableData.h>
#include <zserio/ppmr/IIntrospectableView.h>
#include <zserio/View.h>
#include <zserio/ArrayView.h>
#include <zserio/Types.h>
#include <zserio/ppmr/Vector.h>

namespace test_object
{
namespace ppmr_allocator
{

// forward declaration
struct ReflectableUtilChoice;

} // namespace ppmr_allocator
} // namespace test_object

namespace zserio
{
namespace detail
{

template <>
struct ChoiceTag<::test_object::ppmr_allocator::ReflectableUtilChoice>
{
    enum Tag : size_t
    {
        ZSERIO_UNDEFINED,
        array
    };
};

} // namespace detail
} // namespace zserio

namespace test_object
{
namespace ppmr_allocator
{

struct ReflectableUtilChoice : ::zserio::ppmr::Variant<::zserio::detail::ChoiceTag<ReflectableUtilChoice>::Tag,
        ::std::monostate,
        ::zserio::ppmr::Vector<::zserio::UInt32>>
{
    using Tag = ::zserio::detail::ChoiceTag<ReflectableUtilChoice>::Tag;
    using Base = ::zserio::ppmr::Variant<Tag,
            ::std::monostate,
            ::zserio::ppmr::Vector<::zserio::UInt32>>;
    using Base::BasicVariant;
};

bool operator==(const ::test_object::ppmr_allocator::ReflectableUtilChoice& lhs, const ::test_object::ppmr_allocator::ReflectableUtilChoice& rhs);
bool operator<(const ::test_object::ppmr_allocator::ReflectableUtilChoice& lhs, const ::test_object::ppmr_allocator::ReflectableUtilChoice& rhs);
bool operator!=(const ::test_object::ppmr_allocator::ReflectableUtilChoice& lhs, const ::test_object::ppmr_allocator::ReflectableUtilChoice& rhs);
bool operator>(const ::test_object::ppmr_allocator::ReflectableUtilChoice& lhs, const ::test_object::ppmr_allocator::ReflectableUtilChoice& rhs);
bool operator<=(const ::test_object::ppmr_allocator::ReflectableUtilChoice& lhs, const ::test_object::ppmr_allocator::ReflectableUtilChoice& rhs);
bool operator>=(const ::test_object::ppmr_allocator::ReflectableUtilChoice& lhs, const ::test_object::ppmr_allocator::ReflectableUtilChoice& rhs);

} // namespace ppmr_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::ppmr_allocator::ReflectableUtilChoice>
{
public:
    explicit View(const ::test_object::ppmr_allocator::ReflectableUtilChoice& data,
            ::zserio::UInt8 param_) noexcept;

    ::zserio::UInt8 param() const;

    ArrayView<const ::zserio::UInt32> array() const;

    ::test_object::ppmr_allocator::ReflectableUtilChoice::Tag zserioChoiceTag() const;
    const ::test_object::ppmr_allocator::ReflectableUtilChoice& zserioData() const;

protected:
    View(const ::test_object::ppmr_allocator::ReflectableUtilChoice& data, const View& other) noexcept;

private:
    const ::test_object::ppmr_allocator::ReflectableUtilChoice* m_data;
    ::zserio::UInt8 m_param_;
};

bool operator==(const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& rhs);
bool operator<(const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& rhs);
bool operator!=(const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& rhs);
bool operator>(const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& rhs);
bool operator<=(const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& rhs);
bool operator>=(const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& view);

template <>
View<::test_object::ppmr_allocator::ReflectableUtilChoice> read(BitStreamReader& reader, ::test_object::ppmr_allocator::ReflectableUtilChoice& data,
        ::zserio::UInt8 param_);

template <>
struct TypeInfo<::test_object::ppmr_allocator::ReflectableUtilChoice, ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>
{
    static const ::zserio::ppmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::ppmr::IReflectableDataConstPtr reflectable(const ::test_object::ppmr_allocator::ReflectableUtilChoice& value, const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::ppmr::IReflectableDataPtr reflectable(::test_object::ppmr_allocator::ReflectableUtilChoice& value, const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::ppmr::IIntrospectableViewConstPtr introspectable(const View<::test_object::ppmr_allocator::ReflectableUtilChoice>& view, const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::ppmr_allocator::ReflectableUtilChoice>
{
    size_t operator()(const ::test_object::ppmr_allocator::ReflectableUtilChoice& value) const;
};

template <>
struct hash<::zserio::View<::test_object::ppmr_allocator::ReflectableUtilChoice>>
{
    size_t operator()(const ::zserio::View<::test_object::ppmr_allocator::ReflectableUtilChoice>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_PPMR_ALLOCATOR_REFLECTABLE_UTIL_CHOICE_H
