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
class View<TestObject>
{
public:
    explicit View(const TestObject& data) :
            m_data(data)
    {}

    UInt32 field()
    {
        return m_data.field;
    }

    constexpr bool operator==(const View& other) const
    {
        return m_data == other.m_data;
    }

    constexpr bool operator!=(const View& other) const
    {
        return !operator==(other);
    }

    constexpr bool operator<(const View& other) const
    {
        return m_data < other.m_data;
    }

private:
    const TestObject& m_data;
};

template <>
struct ArrayTraits<TestObject>
{
    static zserio::View<TestObject> at(detail::DummyArrayOwner, const TestObject& element, size_t)
    {
        return zserio::View<TestObject>{element};
    }
};

TEST(ArrayTest, boolArray)
{
    std::vector<Bool> rawArray1{true, false};
    std::vector<Bool> rawArray2{true, true};

    Array<std::vector<Bool>, ArrayType::NORMAL> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    Array<std::vector<Bool>, ArrayType::NORMAL> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);
}

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

TEST(ArrayTest, fixedDynInt16Array)
{
    std::vector<DynInt16<9>> rawArray1{-2, 13};
    std::vector<DynInt16<9>> rawArray2{-2, 42};

    Array<std::vector<DynInt16<9>>, ArrayType::NORMAL> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    Array<std::vector<DynInt16<9>>, ArrayType::NORMAL> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);
}

TEST(ArrayTest, varUInt16Array)
{
    std::vector<VarUInt16> rawArray1{0, 13};
    std::vector<VarUInt16> rawArray2{0, 42};

    Array<std::vector<VarUInt16>, ArrayType::NORMAL> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    Array<std::vector<VarUInt16>, ArrayType::NORMAL> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);
}

TEST(ArrayTest, float16Array)
{
    std::vector<Float16> rawArray1{-9.0F, 0.0F, 10.0F};
    std::vector<Float16> rawArray2{-9.0F, 0.0F, 10.1F};

    Array<std::vector<Float16>, ArrayType::NORMAL> array1(rawArray1);
    Array<std::vector<Float16>, ArrayType::NORMAL> array2(rawArray2);

    ASSERT_EQ(16, ArrayTraits<Float16>::bitSizeOf());

    ASSERT_LT(array1, array2);
    ASSERT_GT(array2, array1);
}

TEST(ArrayTest, testObjectArray)
{
    std::vector<TestObject> rawArray1{TestObject{0}, TestObject{13}};
    std::vector<TestObject> rawArray2{TestObject{0}, TestObject{42}};

    Array<std::vector<TestObject>, ArrayType::NORMAL> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0).field, array1.at(0).field());
    ASSERT_EQ(rawArray1.at(1).field, array1.at(1).field());

    Array<std::vector<TestObject>, ArrayType::NORMAL> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);
}

} // namespace zserio
