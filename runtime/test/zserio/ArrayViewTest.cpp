#include <algorithm>

#include "gtest/gtest.h"
#include "zserio/ArrayView.h"
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
    uint8_t numBits = 10;
};

} // namespace

template <>
class View<TestObject>
{
public:
    explicit View(const TestObject& data) :
            m_data(data)
    {}

    UInt32 field() const
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

namespace detail
{

template <>
struct PackingContext<TestObject>
{
    DeltaContext field;
};

template <>
BitSize bitSizeOf(const View<TestObject>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;
    endBitPosition += bitSizeOf(view.field(), endBitPosition);
    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<TestObject>& view)
{
    write(writer, view.field());
}

template <>
View<TestObject> read(BitStreamReader& reader, TestObject& data)
{
    View<TestObject> view(data);
    read(reader, data.field);
    return view;
}

template <>
void initContext(PackingContext<TestObject>& packingContext, const View<TestObject>& view)
{
    initContext(packingContext.field, view.field());
}

template <>
BitSize bitSizeOf(PackingContext<TestObject>& packingContext, const View<TestObject>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;
    endBitPosition += bitSizeOf(packingContext.field, view.field(), endBitPosition);
    return endBitPosition - bitPosition;
}

template <>
void write(PackingContext<TestObject>& packingContext, BitStreamWriter& writer, const View<TestObject>& view)
{
    write(packingContext.field, writer, view.field());
}

template <>
void read(PackingContext<TestObject>& packingContext, BitStreamReader& reader, TestObject& data)
{

    read(packingContext.field, reader, data.field);
}

} // namespace detail

struct VarDynInt16ArrayTraits
{
    using OwnerType = VarDynInt16Owner;

    static View<DynInt16> at(const VarDynInt16Owner& owner, const DynInt16& element, size_t)
    {
        return View<DynInt16>(element, owner.numBits);
    }

    static void read(BitStreamReader& reader, const VarDynInt16Owner& owner, DynInt16& element, size_t)
    {
        detail::read(reader, element, owner.numBits);
    }

    static void read(DeltaContext& context, BitStreamReader& reader, const VarDynInt16Owner& owner,
            DynInt16& element, size_t)
    {
        detail::read(context, reader, element, owner.numBits);
    }
};

TEST(ArrayTest, boolArray)
{
    Vector<Bool> rawArray1{true, false};
    Vector<Bool> rawArray2{true, true};

    ArrayView<const Bool> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    ArrayView<const Bool> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf<detail::ArrayType::AUTO>(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write<detail::ArrayType::AUTO>(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Bool> readRawArray;
    detail::read<detail::ArrayType::AUTO>(reader, readRawArray);
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, boolPackedArray)
{
    Vector<Bool> rawArray{true, false, true, false, true};
    ArrayView<const Bool> array(rawArray);

    const BitSize packedBitSize = detail::bitSizeOfPacked<detail::ArrayType::AUTO>(array);
    BitBuffer buffer(packedBitSize);
    BitStreamWriter writer(buffer);
    detail::writePacked<detail::ArrayType::AUTO>(writer, array);
    ASSERT_EQ(packedBitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Bool> readRawArray;
    detail::readPacked<detail::ArrayType::AUTO>(reader, readRawArray, rawArray.size());
    ASSERT_EQ(packedBitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray, readRawArray);
}

TEST(ArrayTest, int8Array)
{
    Vector<Int8> rawArray1{0, 13};
    Vector<Int8> rawArray2{0, 42};

    ArrayView<const Int8> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    ArrayView<const Int8> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf<detail::ArrayType::AUTO>(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write<detail::ArrayType::AUTO>(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Int8> readRawArray;
    detail::read<detail::ArrayType::AUTO>(reader, readRawArray);
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, int8PackedArray)
{
    Vector<Int8> rawArray = {-4, -3, -1, 0, 2, 4, 6, 8, 10, 10, 11};
    ArrayView<const Int8> array(rawArray);

    // maxBitNumber == 2
    // packingDescriptor 7 + firstElement 8 + 10 * (maxBitNumber 2 + 1)
    static constexpr BitSize packedBitSize = 45;
    ASSERT_EQ(packedBitSize, detail::bitSizeOfPacked<detail::ArrayType::NORMAL>(array));

    BitBuffer buffer(packedBitSize);
    BitStreamWriter writer(buffer);
    detail::writePacked<detail::ArrayType::NORMAL>(writer, array);
    ASSERT_EQ(packedBitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Int8> readRawArray;
    detail::readPacked<detail::ArrayType::NORMAL>(reader, readRawArray, rawArray.size());
    ASSERT_EQ(rawArray, readRawArray);

    ASSERT_EQ(rawArray.size(), array.size());
    ASSERT_FALSE(array.empty());

    Vector<Int8> emptyRawArray;
    ArrayView<const Int8> emptyArray(emptyRawArray);
    ASSERT_TRUE(emptyArray.empty());

    ASSERT_THROW(emptyArray.front(), CppRuntimeException);

    ASSERT_EQ(rawArray.front(), array.front());
    ASSERT_EQ(rawArray.back(), array.back());

    auto it = array.begin();
    ASSERT_EQ(-4, *it);
    ASSERT_EQ(-1, it[2]);
    it += 2;
    ASSERT_EQ(-1, *it);
    ASSERT_EQ(-3, it[-1]);
    it -= 1;
    ASSERT_EQ(-3, *it);
    auto otherIt = it + 2;
    ASSERT_EQ(0, *otherIt);
    ASSERT_EQ(2, *(++otherIt));
    ASSERT_EQ(2, *(otherIt++));
    ASSERT_EQ(4, *otherIt);
    ASSERT_EQ(2, *(--otherIt));
    ASSERT_EQ(2, *(otherIt--));
    ASSERT_EQ(0, *otherIt);
    otherIt = 2 + it;
    ASSERT_EQ(0, *otherIt);
    otherIt = it - 1;
    ASSERT_EQ(-4, *otherIt);
    ASSERT_EQ(1, it - otherIt);
    ASSERT_EQ(-1, otherIt - it);
    ASSERT_EQ(array.size(), array.end() - array.begin());

    ASSERT_TRUE(otherIt == array.begin());
    ASSERT_FALSE(otherIt == array.end());

    ASSERT_TRUE(otherIt != array.end());
    ASSERT_FALSE(otherIt != array.begin());

    ASSERT_TRUE(otherIt < array.end());
    ASSERT_FALSE(array.end() < otherIt);

    ASSERT_TRUE(array.end() > array.begin());
    ASSERT_FALSE(array.begin() > array.end());

    ASSERT_TRUE(array.begin() <= array.end());
    ASSERT_TRUE(array.begin() <= array.begin());
    ASSERT_FALSE(array.end() <= array.begin());

    ASSERT_TRUE(array.end() >= array.begin());
    ASSERT_TRUE(array.end() >= array.end());
    ASSERT_FALSE(array.begin() >= array.end());

    size_t i = 0;
    for (auto element : array)
    {
        ASSERT_EQ(rawArray[i++], element);
    }

    i = array.size() - 1;
    for (auto rit = array.rbegin(); rit < array.rend(); ++rit)
    {
        ASSERT_EQ(rawArray[i--], *rit);
    }

    auto foundIt = std::find_if(array.begin(), array.end(), [](auto value) {
        return value > 0;
    });
    ASSERT_NE(array.end(), foundIt);
    ASSERT_EQ(2, *foundIt);
}

TEST(ArrayTest, fixedDynInt16Array)
{
    Vector<Int<9>> rawArray1{-2, 13};
    Vector<Int<9>> rawArray2{-2, 42};

    ArrayView<const Int<9>> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    ArrayView<const Int<9>> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf<detail::ArrayType::AUTO>(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write<detail::ArrayType::AUTO>(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Int<9>> readRawArray;
    detail::read<detail::ArrayType::AUTO>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, variableDynInt16Array)
{
    Vector<DynInt16> rawArray1{-2, 13};
    Vector<DynInt16> rawArray2{-2, 42};

    VarDynInt16Owner owner;

    ArrayView<const DynInt16, VarDynInt16ArrayTraits> array1(rawArray1, owner);
    ASSERT_EQ(rawArray1.at(0), array1.at(0).value());
    ASSERT_EQ(rawArray1.at(1), array1.at(1).value());

    ArrayView<const DynInt16, VarDynInt16ArrayTraits> array2(rawArray2, owner);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf<detail::ArrayType::AUTO>(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write<detail::ArrayType::AUTO>(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<DynInt16> readRawArray;
    detail::readWithTraits<detail::ArrayType::AUTO, VarDynInt16ArrayTraits>(
            reader, readRawArray, owner, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, variableDynInt16PackedArray)
{
    Vector<DynInt16> rawArray{-2, 0, 2, 4, 6, 8, 10};

    VarDynInt16Owner owner;
    ArrayView<const DynInt16, VarDynInt16ArrayTraits> array(rawArray, owner);

    // maxBitNumber == 2
    // packingDescriptor 7 + firstElement 10 + 6 * (maxBitNumber 2 + 1)
    const BitSize packedBitSize = 35;
    ASSERT_EQ(packedBitSize, detail::bitSizeOfPacked<detail::ArrayType::NORMAL>(array));

    BitBuffer buffer(packedBitSize);
    BitStreamWriter writer(buffer);
    detail::writePacked<detail::ArrayType::NORMAL>(writer, array);
    ASSERT_EQ(packedBitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<DynInt16> readRawArray;
    detail::readPackedWithTraits<detail::ArrayType::NORMAL, VarDynInt16ArrayTraits>(
            reader, readRawArray, owner, rawArray.size());
    ASSERT_EQ(packedBitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray, readRawArray);
}

TEST(ArrayTest, varUInt16Array)
{
    Vector<VarUInt16> rawArray1{0, 13};
    Vector<VarUInt16> rawArray2{0, 42};

    ArrayView<const VarUInt16> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    ArrayView<const VarUInt16> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf<detail::ArrayType::AUTO>(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write<detail::ArrayType::AUTO>(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<VarUInt16> readRawArray;
    detail::read<detail::ArrayType::AUTO>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, varUInt16PackedArray)
{
    Vector<VarUInt16> rawArray{0, 2, 4, 6, 8, 10, 12};
    ArrayView<const VarUInt16> array(rawArray);

    const BitSize packedBitSize = detail::bitSizeOfPacked<detail::ArrayType::AUTO>(array);
    BitBuffer buffer(packedBitSize);
    BitStreamWriter writer(buffer);
    detail::writePacked<detail::ArrayType::AUTO>(writer, array);
    ASSERT_EQ(packedBitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<VarUInt16> readRawArray;
    detail::readPacked<detail::ArrayType::AUTO>(reader, readRawArray, rawArray.size());
    ASSERT_EQ(packedBitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray, readRawArray);
}

TEST(ArrayTest, float16Array)
{
    Vector<Float16> rawArray1{-9.0F, 0.0F, 10.0F};
    Vector<Float16> rawArray2{-9.0F, 0.0F, 10.1F};

    ArrayView<const Float16> array1(rawArray1);
    ArrayView<const Float16> array2(rawArray2);

    ASSERT_EQ(16, ArrayTraits<Float16>::bitSizeOf());

    ASSERT_LT(array1, array2);
    ASSERT_GT(array2, array1);

    const BitSize bitSize = detail::bitSizeOf<detail::ArrayType::AUTO>(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write<detail::ArrayType::AUTO>(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Float16> readRawArray;
    detail::read<detail::ArrayType::AUTO>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, bytesArray)
{
    Vector<Bytes> rawArray1{{{{1, 255}}, {{127, 128}}}};
    Vector<Bytes> rawArray2{{{{1, 255}}, {{127, 129}}}};

    ArrayView<const Bytes> array1(rawArray1);
    ArrayView<const Bytes> array2(rawArray2);

    ASSERT_TRUE(std::equal(
            rawArray1.begin(), rawArray1.end(), array1.zserioData().begin(), array1.zserioData().end()));

    constexpr bool isSpan = std::is_same_v<Span<const uint8_t>, decltype(array1.at(0))>;
    ASSERT_TRUE(isSpan);

    ASSERT_TRUE(std::equal(
            rawArray1.at(0).begin(), rawArray1.at(0).end(), array1.at(0).begin(), array1.at(0).end()));
    ASSERT_TRUE(std::equal(
            rawArray1.at(1).begin(), rawArray1.at(1).end(), array1.at(1).begin(), array1.at(1).end()));

    ASSERT_NE(array1, array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf<detail::ArrayType::AUTO>(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write<detail::ArrayType::AUTO>(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Bytes> readRawArray;
    detail::read<detail::ArrayType::AUTO>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, stringArray)
{
    Vector<std::string> rawArray1 = {"String0", "String1", "String2"};
    Vector<std::string> rawArray2 = {"String0", "String1", "String3"};

    ArrayView<const std::string> array1(rawArray1);
    ArrayView<const std::string> array2(rawArray2);

    constexpr bool isStringView = std::is_same_v<std::string_view, decltype(array1.at(0))>;
    ASSERT_TRUE(isStringView);

    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));
    ASSERT_EQ(rawArray1.at(2), array1.at(2));

    ASSERT_NE(array1, array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf<detail::ArrayType::AUTO>(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write<detail::ArrayType::AUTO>(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<std::string> readRawArray;
    detail::read<detail::ArrayType::AUTO>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, testObjectArray)
{
    Vector<TestObject> rawArray1{TestObject{0}, TestObject{13}};
    Vector<TestObject> rawArray2{TestObject{0}, TestObject{42}};

    ArrayView<const TestObject> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0).field, array1.at(0).field());
    ASSERT_EQ(rawArray1.at(1).field, array1.at(1).field());

    ArrayView<const TestObject> array2(rawArray2);

    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf<detail::ArrayType::AUTO>(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write<detail::ArrayType::AUTO>(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<TestObject> readRawArray;
    detail::read<detail::ArrayType::AUTO>(reader, readRawArray);
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);

    ASSERT_FALSE(array1.empty());
    ASSERT_EQ(0, array1.front().field());
    ASSERT_EQ(13, array1.back().field());

    size_t i = 0;
    for (auto object : array1)
    {
        ASSERT_EQ(i++ == 0 ? 0 : 13, object.field());
    }

    TestObject object{13};
    zserio::View view(object);
    ASSERT_EQ(view, *(array1.begin() + 1));
    ASSERT_EQ(0, array1.begin()->field());
    ASSERT_EQ(view, array1.begin()[1]);
    ASSERT_EQ(array1.begin(), array1.end() - 2);
    ASSERT_EQ(*array1.begin(), *(array1.end() - 2));
    ASSERT_EQ(array1.size(), array1.end() - array1.begin());

    auto foundIt = std::find_if(array1.begin(), array1.end(), [](const zserio::View<TestObject>& element) {
        return element.field() != 0;
    });
    ASSERT_NE(array1.end(), foundIt);
    ASSERT_EQ(array1.at(1), *foundIt);

    auto notFoundIt = std::find_if(array1.begin(), array1.end(), [](const zserio::View<TestObject>& element) {
        return element.field() > 13;
    });
    ASSERT_EQ(array1.end(), notFoundIt);
}

TEST(ArrayTest, testObjectPackedArray)
{
    Vector<TestObject> rawArray{TestObject{0}, TestObject{2}, TestObject{4}, TestObject{6}, TestObject{8}};
    ArrayView<const TestObject> array(rawArray);

    // maxBitNumber == 2
    // packingDescriptor 7 + firstElement 32 + 4 * (maxBitNumber 2 + 1)
    const BitSize packedBitSize = 51;
    ASSERT_EQ(packedBitSize, detail::bitSizeOfPacked<detail::ArrayType::NORMAL>(array));

    BitBuffer buffer(packedBitSize);
    BitStreamWriter writer(buffer);
    detail::writePacked<detail::ArrayType::NORMAL>(writer, array);
    ASSERT_EQ(packedBitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<TestObject> readRawArray;
    detail::readPacked<detail::ArrayType::NORMAL>(reader, readRawArray, rawArray.size());
    ASSERT_EQ(packedBitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray, readRawArray);
}

} // namespace zserio
