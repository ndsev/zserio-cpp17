#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "zserio/Bytes.h"
#include "zserio/Extended.h"
#include "zserio/Optional.h"
#include "zserio/String.h"
#include "zserio/Traits.h"
#include "zserio/Variant.h"
#include "zserio/View.h"

namespace zserio
{

namespace
{

// wrapper functions to prevent macro confusion with ',' in template parameters
void assertTrue(bool value)
{
    ASSERT_TRUE(value);
}

void assertFalse(bool value)
{
    ASSERT_FALSE(value);
}

class DummyBitmask
{
public:
    using ZserioType = Int8;

    int getValue()
    {
        return 0;
    }
};

enum class DummyEnum : int8_t
{
    ONE,
    TWO
};

class DummyObjectWithAllocatorType
{
public:
    using allocator_type = std::allocator<uint8_t>;
};

template <typename T>
class TmpTest;

template <>
class TmpTest<std::string>
{};

} // namespace

template <>
class View<DummyObjectWithAllocatorType>
{};

TEST(TraitsTest, isAllocator)
{
    assertTrue(is_allocator<std::allocator<uint8_t>>::value);
    assertTrue(is_allocator<std::allocator<std::vector<uint8_t>>>::value);

    assertFalse(is_allocator<int>::value);
    assertFalse(is_allocator<uint64_t>::value);
    assertFalse(is_allocator<std::vector<uint8_t>>::value);
    assertFalse(is_allocator<Variant<size_t, uint32_t, std::string>>::value);
}

TEST(TraitsTest, isFirstAllocator)
{
    assertTrue(is_first_allocator<std::allocator<uint8_t>, int>::value);
    assertTrue(is_first_allocator<std::allocator<uint8_t>, std::allocator<uint8_t>>::value);
    assertTrue(is_first_allocator<std::allocator<uint8_t>, std::allocator<uint8_t>, char>::value);
    assertTrue(is_first_allocator<std::allocator<uint8_t>, int, std::allocator<uint8_t>>::value);

    assertFalse(is_first_allocator<int, std::allocator<uint8_t>>::value);
    assertFalse(is_first_allocator<std::vector<uint8_t>, std::allocator<uint8_t>>::value);
    assertFalse(is_first_allocator<char, std::allocator<uint8_t>, std::allocator<uint8_t>>::value);
    assertFalse(is_first_allocator<int, std::allocator<uint8_t>, std::allocator<uint8_t>>::value);
}

TEST(TraitsTest, hasAllocator)
{
    assertTrue(has_allocator<std::string>::value);
    assertTrue(has_allocator<DummyObjectWithAllocatorType>::value);
}

TEST(TraitsTest, isBitmask)
{
    ASSERT_TRUE(is_bitmask<DummyBitmask>::value);
    ASSERT_EQ(0, DummyBitmask().getValue()); // prevent warning
    ASSERT_FALSE(is_bitmask<DummyEnum>::value);
    ASSERT_FALSE(is_bitmask<std::string>::value);
    ASSERT_FALSE(is_bitmask<std::vector<uint8_t>>::value);
}

TEST(TraitsTest, isComplete)
{
    ASSERT_TRUE(is_complete<std::string>::value);
    ASSERT_FALSE(is_complete<TmpTest<int>>::value);
    ASSERT_TRUE(is_complete<TmpTest<std::string>>::value);
}

TEST(TraitsTest, viewType)
{
    assertTrue(std::is_same_v<std::string_view, view_type_t<std::string>>);
    assertTrue(std::is_same_v<BytesView, view_type_t<Bytes>>);
}

TEST(TraitsTest, needsOffsetReference)
{
    assertTrue(detail::needs_offset_reference_v<int>);
    assertTrue(detail::needs_offset_reference_v<Optional<int>>);
    assertTrue(detail::needs_offset_reference_v<Extended<int>>);
    assertTrue(detail::needs_offset_reference_v<Extended<Optional<int>>>);
    assertFalse(detail::needs_offset_reference_v<View<DummyObjectWithAllocatorType>>);
    assertFalse(detail::needs_offset_reference_v<Optional<View<DummyObjectWithAllocatorType>>>);
    assertFalse(detail::needs_offset_reference_v<Extended<View<DummyObjectWithAllocatorType>>>);
    assertFalse(detail::needs_offset_reference_v<Extended<Optional<View<DummyObjectWithAllocatorType>>>>);
}

} // namespace zserio
