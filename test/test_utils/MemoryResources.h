#ifndef TEST_UTILS_MEMORY_RESOURCES_H_INC
#define TEST_UTILS_MEMORY_RESOURCES_H_INC

#include <array>
#include <memory_resource>

#include "zserio/CppRuntimeException.h"
#include "zserio/Types.h"

namespace test_utils
{

class InvalidMemoryResource : public std::pmr::memory_resource
{
private:
    void* do_allocate(size_t bytes, size_t align) override
    {
        throw zserio::CppRuntimeException("Trying to allocate using default memory resource (")
                << bytes << ", " << align << ")!";
    }

    void do_deallocate(void*, size_t bytes, size_t align) override
    {
        throw zserio::CppRuntimeException("Trying to deallocate using default memory resource (")
                << bytes << ", " << align << ")!";
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        return this == &other;
    }
};

class HeapMemoryResource : public std::pmr::memory_resource
{
public:
    void* do_allocate(size_t bytes, size_t) override
    {
        ++m_numAllocations;
        m_allocatedSize += bytes;

        return ::operator new(bytes);
    }

    void do_deallocate(void* p, size_t bytes, size_t) override
    {
        ++m_numDeallocations;
        m_allocatedSize -= bytes;

        ::operator delete(p);
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        return this == &other;
    }

    size_t getNumAllocations() const noexcept
    {
        return m_numAllocations;
    }

    size_t getNumDeallocations() const noexcept
    {
        return m_numDeallocations;
    }

    size_t getAllocatedSize() const noexcept
    {
        return m_allocatedSize;
    }

private:
    size_t m_numAllocations = 0;
    size_t m_numDeallocations = 0;
    size_t m_allocatedSize = 0;
};

template <size_t BUFFER_SIZE = 1024 * 2>
class TestMemoryResource : public std::pmr::memory_resource
{
public:
    explicit TestMemoryResource(const char* name) :
            m_name(name)
    {
        m_buffer.fill(0);
        m_nextPtr = m_buffer.begin();
    }

    void* do_allocate(size_t bytes, size_t align) override
    {
        const size_t alignMod = static_cast<size_t>(m_nextPtr - m_buffer.begin()) % align;
        if (alignMod != 0)
            m_nextPtr += align - alignMod;

        void* const ptr = &(*m_nextPtr);
        m_nextPtr += bytes;

        const size_t usedBytes = static_cast<size_t>(m_nextPtr - m_buffer.begin());
        if (usedBytes > BUFFER_SIZE)
            throw zserio::CppRuntimeException(m_name) << ": Buffer overflow (" << usedBytes << ")!";

        ++m_numAllocations;
        m_allocatedSize += bytes;

        return ptr;
    }

    void do_deallocate(void*, size_t bytes, size_t) override
    {
        ++m_numDeallocations;
        m_allocatedSize -= bytes;
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        return this == &other;
    }

    size_t getNumAllocations() const noexcept
    {
        return m_numAllocations;
    }

    size_t getNumDeallocations() const noexcept
    {
        return m_numDeallocations;
    }

    size_t getAllocatedSize() const noexcept
    {
        return m_allocatedSize;
    }

private:
    const char* m_name;

    std::array<uint8_t, BUFFER_SIZE> m_buffer;
    typename std::array<uint8_t, BUFFER_SIZE>::iterator m_nextPtr;
    size_t m_numAllocations = 0;
    size_t m_numDeallocations = 0;
    size_t m_allocatedSize = 0;
};

class MemoryResourceScopedSetter
{
public:
    explicit MemoryResourceScopedSetter(std::pmr::memory_resource& memoryResource) :
            m_origMemoryResource(std::pmr::set_default_resource(&memoryResource))
    {}

    ~MemoryResourceScopedSetter()
    {
        std::pmr::set_default_resource(m_origMemoryResource);
    }

    MemoryResourceScopedSetter(const MemoryResourceScopedSetter&) = default;
    MemoryResourceScopedSetter& operator=(const MemoryResourceScopedSetter&) = default;

    MemoryResourceScopedSetter(MemoryResourceScopedSetter&&) = default;
    MemoryResourceScopedSetter& operator=(MemoryResourceScopedSetter&&) = default;

private:
    std::pmr::memory_resource* m_origMemoryResource;
};

} // namespace test_utils

#endif // TEST_UTILS_MEMORY_RESOURCES_H_INC
