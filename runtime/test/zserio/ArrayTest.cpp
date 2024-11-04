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

struct VarDynInt16Owner
{
    const uint8_t numBits = 10;
};

struct VarDynInt16Tag
{};

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

struct DynInt16ArrayTraits
{
    using OwnerType = VarDynInt16Owner;

    static View<DynInt16<>> at(const VarDynInt16Owner& owner, const DynInt16<>& element, size_t)
    {
        return View<DynInt16<>>(element, owner.numBits);
    }

    // TODO[Mi-L@]: Will be needed once reading test is implemented!
    // static void read(
    //         zserio::BitStreamReader& reader, const VarDynInt16Owner& owner, DynInt16<>& element, size_t)
    // {
    //     detail::read(reader, element, owner.numBits);
    // }
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

TEST(ArrayTest, variableDynInt16Array)
{
    std::vector<DynInt16<>> rawArray1{-2, 13};
    std::vector<DynInt16<>> rawArray2{-2, 42};

    VarDynInt16Owner owner;

    Array<std::vector<DynInt16<>>, ArrayType::NORMAL, DynInt16ArrayTraits> array1(rawArray1, owner);
    ASSERT_EQ(rawArray1.at(0), array1.at(0).value());
    ASSERT_EQ(rawArray1.at(1), array1.at(1).value());

    Array<std::vector<DynInt16<>>, ArrayType::NORMAL, DynInt16ArrayTraits> array2(rawArray2, owner);
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

TEST(ArrayTest, bytesArray)
{
    std::vector<Bytes> rawArray1{{{{1, 255}}, {{127, 128}}}};
    std::vector<Bytes> rawArray2{{{{1, 255}}, {{127, 129}}}};

    Array<std::vector<Bytes>, ArrayType::NORMAL> array1(rawArray1);
    Array<std::vector<Bytes>, ArrayType::NORMAL> array2(rawArray2);

    constexpr bool isSpan = std::is_same_v<Span<const uint8_t>, decltype(array1.at(0))>;
    ASSERT_TRUE(isSpan);

    ASSERT_TRUE(std::equal(
            rawArray1.at(0).begin(), rawArray1.at(0).end(), array1.at(0).begin(), array1.at(0).end()));
    ASSERT_TRUE(std::equal(
            rawArray1.at(1).begin(), rawArray1.at(1).end(), array1.at(1).begin(), array1.at(1).end()));

    ASSERT_NE(array1, array2);
    ASSERT_LT(array1, array2);
}

TEST(ArrayTest, stringArray)
{
    std::vector<std::string> rawArray1 = {"String0", "String1", "String2"};
    std::vector<std::string> rawArray2 = {"String0", "String1", "String3"};

    Array<std::vector<std::string>, ArrayType::NORMAL> array1(rawArray1);
    Array<std::vector<std::string>, ArrayType::NORMAL> array2(rawArray2);

    constexpr bool isStringView = std::is_same_v<std::string_view, decltype(array1.at(0))>;
    ASSERT_TRUE(isStringView);

    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));
    ASSERT_EQ(rawArray1.at(2), array1.at(2));

    ASSERT_NE(array1, array2);
    ASSERT_LT(array1, array2);
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
