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

    static View<DynInt16<>> at(const VarDynInt16Owner& owner, const DynInt16<>& element, size_t)
    {
        return View<DynInt16<>>(element, owner.numBits);
    }

    static void read(BitStreamReader& reader, const VarDynInt16Owner& owner, DynInt16<>& element, size_t)
    {
        detail::read(reader, element, owner.numBits);
    }

    static void read(DeltaContext& context, BitStreamReader& reader, const VarDynInt16Owner& owner,
            DynInt16<>& element, size_t)
    {
        detail::read(context, reader, element, owner.numBits);
    }
};

TEST(ArrayTest, boolArray)
{
    Vector<Bool> rawArray1{true, false};
    Vector<Bool> rawArray2{true, true};

    Array<Vector<Bool>, ArrayType::AUTO> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    Array<Vector<Bool>, ArrayType::AUTO> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Bool> readRawArray;
    detail::read<Array<Vector<Bool>, ArrayType::AUTO>>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, boolPackedArray)
{
    Vector<Bool> rawArray{true, false, true, false, true};
    Array<Vector<Bool>, ArrayType::AUTO> array(rawArray);

    const BitSize packedBitSize = detail::bitSizeOfPacked(array);
    BitBuffer buffer(packedBitSize);
    BitStreamWriter writer(buffer);
    detail::writePacked(writer, array);
    ASSERT_EQ(packedBitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Bool> readRawArray;
    detail::readPacked<Array<Vector<Bool>, ArrayType::AUTO>>(reader, readRawArray, rawArray.size());
    ASSERT_EQ(packedBitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray, readRawArray);
}

TEST(ArrayTest, int8Array)
{
    Vector<Int8> rawArray1{0, 13};
    Vector<Int8> rawArray2{0, 42};

    Array<Vector<Int8>, ArrayType::AUTO> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    Array<Vector<Int8>, ArrayType::AUTO> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Int8> readRawArray;
    detail::read<Array<Vector<Int8>, ArrayType::AUTO>>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, int8PackedArray)
{
    Vector<Int8> rawArray = {-4, -3, -1, 0, 2, 4, 6, 8, 10, 10, 11};
    Array<Vector<Int8>, ArrayType::NORMAL> array(rawArray, rawArray.size());

    // maxBitNumber == 2
    // packingDescriptor 7 + firstElement 8 + 10 * (maxBitNumber 2 + 1)
    static constexpr BitSize packedBitSize = 45;
    ASSERT_EQ(packedBitSize, detail::bitSizeOfPacked(array));

    BitBuffer buffer(packedBitSize);
    BitStreamWriter writer(buffer);
    detail::writePacked(writer, array);
    ASSERT_EQ(packedBitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Int8> readRawArray;
    detail::readPacked<Array<Vector<Int8>, ArrayType::NORMAL>>(reader, readRawArray, rawArray.size());
    ASSERT_EQ(rawArray, readRawArray);
}

TEST(ArrayTest, fixedDynInt16Array)
{
    Vector<DynInt16<9>> rawArray1{-2, 13};
    Vector<DynInt16<9>> rawArray2{-2, 42};

    Array<Vector<DynInt16<9>>, ArrayType::AUTO> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    Array<Vector<DynInt16<9>>, ArrayType::AUTO> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<DynInt16<9>> readRawArray;
    detail::read<Array<Vector<DynInt16<9>>, ArrayType::AUTO>>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, variableDynInt16Array)
{
    Vector<DynInt16<>> rawArray1{-2, 13};
    Vector<DynInt16<>> rawArray2{-2, 42};

    VarDynInt16Owner owner;

    Array<Vector<DynInt16<>>, ArrayType::AUTO, VarDynInt16ArrayTraits> array1(rawArray1, owner);
    ASSERT_EQ(rawArray1.at(0), array1.at(0).value());
    ASSERT_EQ(rawArray1.at(1), array1.at(1).value());

    Array<Vector<DynInt16<>>, ArrayType::AUTO, VarDynInt16ArrayTraits> array2(rawArray2, owner);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<DynInt16<>> readRawArray;
    detail::read<Array<Vector<DynInt16<>>, ArrayType::AUTO, VarDynInt16ArrayTraits>>(
            reader, readRawArray, owner, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, variableDynInt16PackedArray)
{
    Vector<DynInt16<>> rawArray{-2, 0, 2, 4, 6, 8, 10};

    VarDynInt16Owner owner;
    Array<Vector<DynInt16<>>, ArrayType::NORMAL, VarDynInt16ArrayTraits> array(
            rawArray, owner, rawArray.size());

    // maxBitNumber == 2
    // packingDescriptor 7 + firstElement 10 + 6 * (maxBitNumber 2 + 1)
    const BitSize packedBitSize = 35;
    ASSERT_EQ(packedBitSize, detail::bitSizeOfPacked(array));

    BitBuffer buffer(packedBitSize);
    BitStreamWriter writer(buffer);
    detail::writePacked(writer, array);
    ASSERT_EQ(packedBitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<DynInt16<>> readRawArray;
    detail::readPacked<Array<Vector<DynInt16<>>, ArrayType::NORMAL, VarDynInt16ArrayTraits>>(
            reader, readRawArray, owner, rawArray.size());
    ASSERT_EQ(packedBitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray, readRawArray);
}

TEST(ArrayTest, varUInt16Array)
{
    Vector<VarUInt16> rawArray1{0, 13};
    Vector<VarUInt16> rawArray2{0, 42};

    Array<Vector<VarUInt16>, ArrayType::AUTO> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));

    Array<Vector<VarUInt16>, ArrayType::AUTO> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<VarUInt16> readRawArray;
    detail::read<Array<Vector<VarUInt16>, ArrayType::AUTO>>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, varUInt16PackedArray)
{
    Vector<VarUInt16> rawArray{0, 2, 4, 6, 8, 10, 12};
    Array<Vector<VarUInt16>, ArrayType::AUTO> array(rawArray);

    const BitSize packedBitSize = detail::bitSizeOfPacked(array);
    BitBuffer buffer(packedBitSize);
    BitStreamWriter writer(buffer);
    detail::writePacked(writer, array);
    ASSERT_EQ(packedBitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<VarUInt16> readRawArray;
    detail::readPacked<Array<Vector<VarUInt16>, ArrayType::AUTO>>(reader, readRawArray, rawArray.size());
    ASSERT_EQ(packedBitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray, readRawArray);
}

TEST(ArrayTest, float16Array)
{
    Vector<Float16> rawArray1{-9.0F, 0.0F, 10.0F};
    Vector<Float16> rawArray2{-9.0F, 0.0F, 10.1F};

    Array<Vector<Float16>, ArrayType::AUTO> array1(rawArray1);
    Array<Vector<Float16>, ArrayType::AUTO> array2(rawArray2);

    ASSERT_EQ(16, ArrayTraits<Float16>::bitSizeOf());

    ASSERT_LT(array1, array2);
    ASSERT_GT(array2, array1);

    const BitSize bitSize = detail::bitSizeOf(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Float16> readRawArray;
    detail::read<Array<Vector<Float16>, ArrayType::AUTO>>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, bytesArray)
{
    Vector<Bytes> rawArray1{{{{1, 255}}, {{127, 128}}}};
    Vector<Bytes> rawArray2{{{{1, 255}}, {{127, 129}}}};

    Array<Vector<Bytes>, ArrayType::AUTO> array1(rawArray1);
    Array<Vector<Bytes>, ArrayType::AUTO> array2(rawArray2);

    constexpr bool isSpan = std::is_same_v<Span<const uint8_t>, decltype(array1.at(0))>;
    ASSERT_TRUE(isSpan);

    ASSERT_TRUE(std::equal(
            rawArray1.at(0).begin(), rawArray1.at(0).end(), array1.at(0).begin(), array1.at(0).end()));
    ASSERT_TRUE(std::equal(
            rawArray1.at(1).begin(), rawArray1.at(1).end(), array1.at(1).begin(), array1.at(1).end()));

    ASSERT_NE(array1, array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<Bytes> readRawArray;
    detail::read<Array<Vector<Bytes>, ArrayType::AUTO>>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, stringArray)
{
    Vector<std::string> rawArray1 = {"String0", "String1", "String2"};
    Vector<std::string> rawArray2 = {"String0", "String1", "String3"};

    Array<Vector<std::string>, ArrayType::AUTO> array1(rawArray1);
    Array<Vector<std::string>, ArrayType::AUTO> array2(rawArray2);

    constexpr bool isStringView = std::is_same_v<StringView, decltype(array1.at(0))>;
    ASSERT_TRUE(isStringView);

    ASSERT_EQ(rawArray1.at(0), array1.at(0));
    ASSERT_EQ(rawArray1.at(1), array1.at(1));
    ASSERT_EQ(rawArray1.at(2), array1.at(2));

    ASSERT_NE(array1, array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<std::string> readRawArray;
    detail::read<Array<Vector<std::string>, ArrayType::AUTO>>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, testObjectArray)
{
    Vector<TestObject> rawArray1{TestObject{0}, TestObject{13}};
    Vector<TestObject> rawArray2{TestObject{0}, TestObject{42}};

    Array<Vector<TestObject>, ArrayType::AUTO> array1(rawArray1);
    ASSERT_EQ(rawArray1.at(0).field, array1.at(0).field());
    ASSERT_EQ(rawArray1.at(1).field, array1.at(1).field());

    Array<Vector<TestObject>, ArrayType::AUTO> array2(rawArray2);
    ASSERT_FALSE(array1 == array2);
    ASSERT_TRUE(array1 != array2);
    ASSERT_LT(array1, array2);

    const BitSize bitSize = detail::bitSizeOf(array1);
    BitBuffer buffer(bitSize);
    BitStreamWriter writer(buffer);
    detail::write(writer, array1);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<TestObject> readRawArray;
    detail::read<Array<Vector<TestObject>, ArrayType::AUTO>>(reader, readRawArray, rawArray1.size());
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray1, readRawArray);
}

TEST(ArrayTest, testObjectPackedArray)
{
    Vector<TestObject> rawArray{TestObject{0}, TestObject{2}, TestObject{4}, TestObject{6}, TestObject{8}};
    Array<Vector<TestObject>, ArrayType::NORMAL> array(rawArray, rawArray.size());

    // maxBitNumber == 2
    // packingDescriptor 7 + firstElement 32 + 4 * (maxBitNumber 2 + 1)
    const BitSize packedBitSize = 51;
    ASSERT_EQ(packedBitSize, detail::bitSizeOfPacked(array));

    BitBuffer buffer(packedBitSize);
    BitStreamWriter writer(buffer);
    detail::writePacked(writer, array);
    ASSERT_EQ(packedBitSize, writer.getBitPosition());

    BitStreamReader reader(buffer);
    Vector<TestObject> readRawArray;
    detail::readPacked<Array<Vector<TestObject>, ArrayType::NORMAL>>(reader, readRawArray, rawArray.size());
    ASSERT_EQ(packedBitSize, reader.getBitPosition());
    ASSERT_EQ(rawArray, readRawArray);
}

} // namespace zserio
