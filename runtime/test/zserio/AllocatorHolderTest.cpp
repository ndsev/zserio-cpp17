#include <memory>
#include <memory_resource>

#include "gtest/gtest.h"
#include "zserio/AllocatorHolder.h"

namespace zserio
{

template <typename ALLOC>
struct MyContainer : public AllocatorHolder<ALLOC>
{
    int data;
};

struct Empty
{};

TEST(AllocatorHolderTest, size)
{
    ASSERT_EQ(sizeof(MyContainer<std::allocator<uint8_t>>), sizeof(int));
    ASSERT_EQ(sizeof(MyContainer<Empty>), sizeof(int));
    ASSERT_GT(sizeof(MyContainer<std::pmr::polymorphic_allocator<uint8_t>>), sizeof(int));
}

} // namespace zserio
