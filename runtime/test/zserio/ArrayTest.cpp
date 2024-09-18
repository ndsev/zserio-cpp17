#include "gtest/gtest.h"
#include "zserio/Array.h"
#include "zserio/Types.h"

namespace zserio
{

namespace
{

struct TestObject
{
    UInt32 field;

    constexpr bool operator==(const TestObject& other) const
    {
        return field == other.field;
    }

    constexpr bool operator<(const TestObject& other) const
    {
        return field < other.field;
    }
};

} // namespace

template <>
struct ArrayTraits<TestObject>
{
    static constexpr const TestObject& at(detail::DummyArrayOwner, const TestObject& element, size_t)
    {
        return element;
    }

    static constexpr bool equal(const TestObject& lhs, const TestObject& rhs)
    {
        return lhs == rhs;
    }

    static constexpr bool lessThan(const TestObject& lhs, const TestObject& rhs)
    {
        return lhs < rhs;
    }
};

TEST(ArrayTest, int8Array)
{
    std::vector<Int8> rawArray1{0, 13};
    std::vector<Int8> rawArray2{0, 42};

    Array<std::vector<Int8>, ArrayType::NORMAL> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    Array<std::vector<Int8>, ArrayType::NORMAL> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);
}

TEST(ArrayTest, testObjectArray)
{
    std::vector<TestObject> rawArray1{TestObject{0}, TestObject{13}};
    std::vector<TestObject> rawArray2{TestObject{0}, TestObject{42}};

    Array<std::vector<TestObject>, ArrayType::NORMAL> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    Array<std::vector<TestObject>, ArrayType::NORMAL> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);
}

} // namespace zserio
