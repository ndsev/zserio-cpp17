#include <string>

#include "gtest/gtest.h"
#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{

namespace
{

class TestResource : public std::pmr::memory_resource
{
public:
    void* do_allocate(size_t bytes, size_t) override
    {
        ++m_numAllocations;
        return ::operator new(bytes);
    }

    void do_deallocate(void* memory, size_t, size_t) override
    {
        --m_numAllocations;
        ::operator delete(memory);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override
    {
        return this == &other;
    }

    size_t num_allocations() const
    {
        return m_numAllocations;
    }

private:
    size_t m_numAllocations = 0;
};

} // namespace

TEST(PolymorphicAllocatorTest, constructorAndResource)
{
    std::pmr::polymorphic_allocator<std::byte> allocator;
    ASSERT_EQ(std::pmr::get_default_resource(), allocator.resource());

    TestResource resource;

    std::pmr::polymorphic_allocator<std::byte> allocator2(&resource);
    ASSERT_EQ(&resource, allocator2.resource());

    std::pmr::polymorphic_allocator<std::byte> allocator2Copy(allocator2);
    ASSERT_EQ(&resource, allocator2Copy.resource());

    std::pmr::polymorphic_allocator<std::byte> allocator2Moved(std::move(allocator2));
    ASSERT_EQ(&resource, allocator2Moved.resource());
}

TEST(PropagatingPolymorphicAllocatorTest, constructorAndResource)
{
    ppmr::PropagatingPolymorphicAllocator<std::byte> allocator;
    ASSERT_EQ(std::pmr::get_default_resource(), allocator.resource());

    TestResource resource;

    ppmr::PropagatingPolymorphicAllocator<std::byte> allocator2(&resource);
    ASSERT_EQ(&resource, allocator2.resource());

    ppmr::PropagatingPolymorphicAllocator<std::byte> allocator2Copy(allocator2);
    ASSERT_EQ(&resource, allocator2Copy.resource());

    ppmr::PropagatingPolymorphicAllocator<std::byte> allocator2Moved(std::move(allocator2));
    ASSERT_EQ(&resource, allocator2Moved.resource());
}

TEST(PolymorphicAllocatorTest, constructorRebind)
{
    TestResource resource;
    std::pmr::polymorphic_allocator<std::byte> allocator(&resource);
    std::pmr::polymorphic_allocator<int> allocatorRebind(allocator);
    ASSERT_EQ(&resource, allocatorRebind.resource());
}

TEST(PropagatingPolymorphicAllocatorTest, constructorRebind)
{
    TestResource resource;
    ppmr::PropagatingPolymorphicAllocator<std::byte> allocator(&resource);
    ppmr::PropagatingPolymorphicAllocator<int> allocatorRebind(allocator);
    ASSERT_EQ(&resource, allocatorRebind.resource());
}

TEST(PolymorphicAllocatorTest, allocations)
{
    TestResource resource;
    std::pmr::polymorphic_allocator<std::byte> allocator(&resource);
    ASSERT_EQ(0, resource.num_allocations());

    const auto ptr = allocator.allocate(1);
    ASSERT_EQ(1, resource.num_allocations());

    const auto ptr2 = allocator.allocate(10);
    ASSERT_EQ(2, resource.num_allocations());

    allocator.deallocate(ptr, 1);
    ASSERT_EQ(1, resource.num_allocations());

    allocator.deallocate(ptr2, 10);
    ASSERT_EQ(0, resource.num_allocations());
}

TEST(PropagatingPolymorphicAllocatorTest, allocations)
{
    TestResource resource;
    ppmr::PropagatingPolymorphicAllocator<std::byte> allocator(&resource);
    ASSERT_EQ(0, resource.num_allocations());

    const auto ptr = allocator.allocate(1);
    ASSERT_EQ(1, resource.num_allocations());

    const auto ptr2 = allocator.allocate(10);
    ASSERT_EQ(2, resource.num_allocations());

    allocator.deallocate(ptr, 1);
    ASSERT_EQ(1, resource.num_allocations());

    allocator.deallocate(ptr2, 10);
    ASSERT_EQ(0, resource.num_allocations());
}

TEST(PolymorphicAllocatorTest, select_on_container_copy_construction)
{
    TestResource resource;
    std::pmr::polymorphic_allocator<std::byte> allocator(&resource);

    std::vector<int, std::pmr::polymorphic_allocator<int>> vector(allocator);
    vector.assign({0, 13, 42});
    std::vector<int, std::pmr::polymorphic_allocator<int>> vectorCopy(vector);
    ASSERT_NE(&resource, vectorCopy.get_allocator().resource());
}

TEST(PropagatingPolymorphicAllocatorTest, select_on_container_copy_construction)
{
    TestResource resource;
    ppmr::PropagatingPolymorphicAllocator<std::byte> allocator(&resource);

    std::vector<int, ppmr::PropagatingPolymorphicAllocator<int>> vector(allocator);
    vector.assign({0, 13, 42});
    std::vector<int, ppmr::PropagatingPolymorphicAllocator<int>> vectorCopy(vector);
    ASSERT_EQ(&resource, vectorCopy.get_allocator().resource());
}

TEST(PolymorphicAllocatorTest, compare)
{
    std::pmr::polymorphic_allocator<std::byte> allocator;
    std::pmr::polymorphic_allocator<std::byte> allocator2;
    ASSERT_TRUE(allocator == allocator2);
    ASSERT_FALSE(allocator != allocator2);

    TestResource resource;

    std::pmr::polymorphic_allocator<std::byte> allocatorRes(&resource);
    std::pmr::polymorphic_allocator<std::byte> allocatorRes2(&resource);
    ASSERT_TRUE(allocatorRes == allocatorRes2);
    ASSERT_FALSE(allocatorRes != allocatorRes2);
    ASSERT_FALSE(allocatorRes == allocator);
    ASSERT_TRUE(allocatorRes != allocator);
}

TEST(PropagatingPolymorphicAllocatorTest, compare)
{
    ppmr::PropagatingPolymorphicAllocator<std::byte> allocator;
    ppmr::PropagatingPolymorphicAllocator<std::byte> allocator2;
    ASSERT_TRUE(allocator == allocator2);
    ASSERT_FALSE(allocator != allocator2);

    TestResource resource;

    ppmr::PropagatingPolymorphicAllocator<std::byte> allocatorRes(&resource);
    ppmr::PropagatingPolymorphicAllocator<std::byte> allocatorRes2(&resource);
    ASSERT_TRUE(allocatorRes == allocatorRes2);
    ASSERT_FALSE(allocatorRes != allocatorRes2);
    ASSERT_FALSE(allocatorRes == allocator);
    ASSERT_TRUE(allocatorRes != allocator);
}

} // namespace zserio
