#include <memory>
#include <tuple>
#include <vector>

#include "gtest/gtest.h"
#include "zserio/Any.h"
#include "zserio/pmr/PropagatingPolymorphicAllocator.h"

#include "TrackingAllocator.h"

namespace zserio
{

namespace
{

class SmallObject
{
public:
    SmallObject() :
            m_value(0)
    {}

    bool operator==(const SmallObject& other) const
    {
        return m_value == other.m_value;
    }

private:
    int m_value;
};

struct BigObject
{
    BigObject() = default;

    bool operator==(const BigObject& other) const
    {
        return std::tie(value1, value2, value3, value4) ==
                std::tie(other.value1, other.value2, other.value3, other.value4);
    }

    uint64_t value1 = 1;
    uint64_t value2 = 2;
    uint64_t value3 = 3;
    uint64_t value4 = 4;
};

} // namespace

class AnyTest : public ::testing::Test
{
protected:
    template <typename T, typename ALLOC>
    void testAny(const T& value, const ALLOC& allocator)
    {
        testEmptyConstructor<ALLOC>();
        testAllocatorConstructor(allocator);
        testLvalueConstructor(value, allocator);
        testRvalueConstructor(value, allocator);
        testCopyConstructor(value, allocator);
        testCopyConstructorWithAllocator(value, allocator);
        testCopyAssignmentOperator(value, allocator);
        testMoveConstructor(value, allocator);
        testMoveConstructorWithAllocator(value, allocator);
        testMoveAssignmentOperator(value, allocator);
        testMoveAssignmentValueOperator(value, allocator);
        testReset(value, allocator);
        testSetGet(value, allocator);
        testIsType(value, allocator);
        testHasValue(value, allocator);
    }

private:
    template <typename ALLOC>
    void testEmptyConstructor()
    {
        BasicAny<ALLOC> any;
        ASSERT_FALSE(any.hasValue());
    }

    template <typename ALLOC>
    void testAllocatorConstructor(const ALLOC& allocator)
    {
        BasicAny<ALLOC> any(allocator);
        ASSERT_FALSE(any.hasValue());
        ASSERT_EQ(allocator, any.get_allocator());
    }

    template <typename T, typename ALLOC>
    void testLvalueConstructor(const T& value, const ALLOC& allocator)
    {
        const void* origAddress = &value;
        BasicAny<ALLOC> any(value, allocator);

        const T& anyValue = any.template get<T>();
        ASSERT_NE(origAddress, &anyValue);
        ASSERT_EQ(value, anyValue);
    }

    template <typename ALLOC>
    void testRvalueConstructor(const std::vector<int>& value, const ALLOC& allocator)
    {
        std::vector<int> origValue(value);
        const void* origAddress = origValue.data();
        BasicAny<ALLOC> any(std::move(origValue), allocator);

        const std::vector<int>& anyValue = any.template get<std::vector<int>>();
        ASSERT_EQ(origAddress, anyValue.data());
        ASSERT_EQ(value, anyValue);
    }

    template <typename T, typename ALLOC>
    void testRvalueConstructor(const T& value, const ALLOC& allocator)
    {
        T origValue(value);
        BasicAny<ALLOC> any(std::move(origValue), allocator);

        const T& anyValue = any.template get<T>();
        ASSERT_EQ(value, anyValue);
    }

    template <typename T, typename ALLOC>
    void testCopyConstructor(const T& value, const ALLOC& allocator)
    {
        BasicAny<ALLOC> any(allocator);

        // copy empty holder
        BasicAny<ALLOC> anyEmptyCopy(any);
        ASSERT_FALSE(anyEmptyCopy.hasValue());

        // copy filled holder
        any.set(value);
        BasicAny<ALLOC> anyCopy(any);
        ASSERT_EQ(value, anyCopy.template get<T>());
    }

    template <typename T, typename ALLOC>
    void testCopyConstructorWithAllocator(const T& value, const ALLOC& allocator)
    {
        BasicAny<ALLOC> any(allocator);

        // copy empty holder
        BasicAny<ALLOC> anyEmptyCopy(any);
        ASSERT_FALSE(anyEmptyCopy.hasValue());

        // copy filled holder
        any.set(value);
        const size_t numAllocations = allocator.numAllocs();
        const ALLOC newAllocator;
        BasicAny<ALLOC> anyCopy(any, newAllocator);
        ASSERT_EQ(value, anyCopy.template get<T>());
        ASSERT_EQ(numAllocations, allocator.numAllocs()); // no allocation and deallocations in old allocator
    }

    template <typename T, typename ALLOC>
    void testCopyAssignmentOperator(const T& value, const ALLOC& allocator)
    {
        BasicAny<ALLOC> any(allocator);

        // assignment empty holder
        BasicAny<ALLOC> anyEmpty;
        anyEmpty = any;
        ASSERT_FALSE(anyEmpty.hasValue());

        // assignment filled holder to itself
        any.set(value);
        BasicAny<ALLOC>& anyRef(any);
        anyRef = any;
        ASSERT_EQ(value, anyRef.template get<T>());

        // assignment filled holder
        BasicAny<ALLOC> anyCopy;
        anyCopy = any;
        ASSERT_EQ(value, anyCopy.template get<T>());
    }

    template <typename T, typename ALLOC>
    void testMoveConstructor(const T& value, const ALLOC& allocator)
    {
        {
            // move empty holder
            BasicAny<ALLOC> any(allocator);
            BasicAny<ALLOC> anyMove(std::move(any));
            ASSERT_FALSE(anyMove.hasValue());
        }

        {
            // move filled holder
            BasicAny<ALLOC> any(allocator);
            any.set(value);
            const size_t numAllocations = allocator.numAllocs();
            BasicAny<ALLOC> anyMove(std::move(any));
            ASSERT_EQ(value, anyMove.template get<T>());
            ASSERT_EQ(numAllocations, allocator.numAllocs()); // no allocations and deallocations
        }
    }

    template <typename T, typename ALLOC>
    void testMoveConstructorWithAllocator(const T& value, const ALLOC& allocator)
    {
        {
            // empty holder with the same allocator
            BasicAny<ALLOC> any(allocator);
            BasicAny<ALLOC> anyMove(std::move(any));
            ASSERT_FALSE(anyMove.hasValue());
        }

        {
            // empty holder with the different allocator
            BasicAny<ALLOC> any(allocator);
            const ALLOC newAllocator;
            BasicAny<ALLOC> anyMove(std::move(any), newAllocator);
            ASSERT_FALSE(anyMove.hasValue());
        }

        {
            // filled holder with the same allocator
            BasicAny<ALLOC> any(allocator);
            any.set(value);
            const size_t numAllocations = allocator.numAllocs();
            BasicAny<ALLOC> anyMove(std::move(any), allocator);
            ASSERT_EQ(value, anyMove.template get<T>());
            ASSERT_EQ(numAllocations, allocator.numAllocs()); // no allocations and deallocations
        }

        {
            // filled holder with the different allocator
            BasicAny<ALLOC> any(allocator);
            any.set(value);
            const size_t numAllocations = allocator.numAllocs();
            const ALLOC newAllocator;
            BasicAny<ALLOC> anyMove(std::move(any), newAllocator);
            ASSERT_EQ(value, anyMove.template get<T>());
            ASSERT_TRUE(numAllocations >= allocator.numAllocs()); // no further allocations
        }
    }

    template <typename T, typename ALLOC>
    void testMoveAssignmentOperator(const T& value, const ALLOC& allocator)
    {
        {
            // assignment empty holder
            BasicAny<ALLOC> any(allocator);
            BasicAny<ALLOC> anyMove;
            anyMove = std::move(any);
            ASSERT_FALSE(anyMove.hasValue());
        }

        {
            // assignment filled holder to itself
            BasicAny<ALLOC> any(allocator);
            any.set(value);
            const size_t numAllocations = allocator.numAllocs();
            BasicAny<ALLOC>& anyRef = any;
            anyRef = std::move(any);
            ASSERT_EQ(value, anyRef.template get<T>());
            ASSERT_EQ(numAllocations, allocator.numAllocs()); // no allocations and deallocations
        }

        {
            // assignment filled holder
            BasicAny<ALLOC> any(allocator);
            any.set(value);
            const size_t numAllocations = allocator.numAllocs();
            BasicAny<ALLOC> anyMove;
            anyMove = std::move(any);
            ASSERT_EQ(value, anyMove.template get<T>());
            ASSERT_TRUE(numAllocations >= allocator.numAllocs()); // no further allocations
        }
    }

    template <typename T, typename ALLOC>
    void testMoveAssignmentValueOperator(const T& value, const ALLOC& allocator)
    {
        {
            // assignment empty holder
            const T valueCopy(value);
            BasicAny<ALLOC> any(allocator);
            const size_t numAllocations = allocator.numAllocs();
            BasicAny<ALLOC> anyMove;
            anyMove = std::move(valueCopy);
            ASSERT_EQ(value, anyMove.template get<T>());
            ASSERT_EQ(numAllocations, allocator.numAllocs()); // no allocations and deallocations
        }

        {
            // assignment filled holder
            const T valueCopy(value);
            BasicAny<ALLOC> any(allocator);
            any.set(value);
            const size_t numAllocations = allocator.numAllocs();
            BasicAny<ALLOC> anyMove;
            anyMove = std::move(valueCopy);
            ASSERT_EQ(value, anyMove.template get<T>());
            ASSERT_EQ(numAllocations, allocator.numAllocs()); // no allocations and deallocations
        }
    }

    template <typename T, typename ALLOC>
    void testReset(const T& value, const ALLOC& allocator)
    {
        BasicAny<ALLOC> any(value, allocator);
        ASSERT_TRUE(any.hasValue());

        any.reset();
        ASSERT_FALSE(any.hasValue());

        any = value;
        ASSERT_TRUE(any.hasValue());
        ASSERT_EQ(value, any.template get<T>());
    }

    template <typename T, typename ALLOC>
    void testSetGet(const T& value, const ALLOC& allocator)
    {
        BasicAny<ALLOC> any(allocator);
        ASSERT_THROW(any.template get<int>(), zserio::CppRuntimeException);

        any.set(value);
        ASSERT_EQ(value, any.template get<T>());

        const int intValue = 0xDEAD;
        any.set(intValue);
        ASSERT_EQ(intValue, any.template get<int>());
        ASSERT_THROW(any.template get<float>(), CppRuntimeException);

        const float floatValue = 3.14F;
        any.set(floatValue);
        ASSERT_THROW(any.template get<int>(), CppRuntimeException);
        ASSERT_EQ(floatValue, any.template get<float>());

        const BigObject bigObjectValue;
        any.set(bigObjectValue);
        ASSERT_THROW(any.template get<int>(), CppRuntimeException);
        ASSERT_EQ(bigObjectValue, any.template get<BigObject>());
    }

    template <typename T, typename ALLOC>
    void testIsType(const T& value, const ALLOC& allocator)
    {
        BasicAny<ALLOC> any(allocator);
        ASSERT_FALSE(any.template isType<int>());

        any.set(value);
        ASSERT_TRUE(any.template isType<T>());

        const int intValue = 0xDEAD;
        any.set(intValue);
        ASSERT_TRUE(any.template isType<int>());

        const float floatValue = 3.14F;
        any.set(floatValue);
        ASSERT_TRUE(any.template isType<float>());
        ASSERT_FALSE(any.template isType<int>());
    }

    template <typename T, typename ALLOC>
    void testHasValue(const T& value, const ALLOC& allocator)
    {
        BasicAny<ALLOC> any(allocator);
        ASSERT_FALSE(any.hasValue());

        any.set(value);
        ASSERT_TRUE(any.hasValue());

        const int intValue = 0xDEAD;
        any.set(intValue);
        ASSERT_TRUE(any.hasValue());

        any.reset();
        ASSERT_FALSE(any.hasValue());
    }
};

TEST_F(AnyTest, integerPropagatingAllocator)
{
    const int value = 0xDEAD;
    const TrackingAllocator<uint8_t> allocator;
    testAny(value, allocator);
}

TEST_F(AnyTest, integerNonPropagatingAllocator)
{
    const int value = 0xDEAD;
    const TrackingAllocatorNonProp<uint8_t> allocator;
    testAny(value, allocator);
}

TEST_F(AnyTest, vectorPropagatingAllocator)
{
    const std::vector<int> value{1, 2, 3};
    const TrackingAllocator<uint8_t> allocator;
    testAny(value, allocator);
}

TEST_F(AnyTest, vectorNonPropagatingAllocator)
{
    const std::vector<int> value{1, 2, 3};
    const TrackingAllocatorNonProp<uint8_t> allocator;
    testAny(value, allocator);
}

TEST_F(AnyTest, smallObjectPropagatingAllocator)
{
    const SmallObject value;
    const TrackingAllocator<uint8_t> allocator;
    testAny(value, allocator);
}

TEST_F(AnyTest, smallObjectNonPropagatingAllocator)
{
    const SmallObject value;
    const TrackingAllocatorNonProp<uint8_t> allocator;
    testAny(value, allocator);
}

TEST_F(AnyTest, bigObjectPropagatingAllocator)
{
    const BigObject value;
    const TrackingAllocator<uint8_t> allocator;
    testAny(value, allocator);
}

TEST_F(AnyTest, bigObjectNonPropagatingAllocator)
{
    const BigObject value;
    const TrackingAllocatorNonProp<uint8_t> allocator;
    testAny(value, allocator);
}

TEST_F(AnyTest, unexceptedCallsHeapHolder)
{
    const TrackingAllocator<uint8_t> allocator;
    detail::HeapHolder<BigObject, TrackingAllocator<uint8_t>>* holder =
            detail::HeapHolder<BigObject, TrackingAllocator<uint8_t>>::create(allocator);
    ASSERT_THROW(holder->clone(nullptr), CppRuntimeException);
    ASSERT_THROW(holder->move(nullptr), CppRuntimeException);
    holder->destroy(allocator);
}

TEST_F(AnyTest, unexceptedCallsNonHeapHolder)
{
    const TrackingAllocator<uint8_t> allocator;
    using MaxInPlaceType = std::aligned_storage<3 * sizeof(void*), alignof(void*)>::type;
    MaxInPlaceType inPlace = MaxInPlaceType();
    detail::NonHeapHolder<uint8_t, TrackingAllocator<uint8_t>>* holder =
            detail::NonHeapHolder<uint8_t, TrackingAllocator<uint8_t>>::create(&inPlace);
    ASSERT_THROW(holder->clone(allocator), CppRuntimeException);
    ASSERT_THROW(holder->move(allocator), CppRuntimeException);
    holder->destroy(allocator);
}

} // namespace zserio
