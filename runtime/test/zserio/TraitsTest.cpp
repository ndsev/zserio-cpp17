#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "zserio/Traits.h"
#include "zserio/Variant.h"

namespace zserio
{

// wrapper functions to prevent macro confusion with ',' in template parameters
static void assertTrue(bool value)
{
    ASSERT_TRUE(value);
}

static void assertFalse(bool value)
{
    ASSERT_FALSE(value);
}

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

} // namespace zserio
