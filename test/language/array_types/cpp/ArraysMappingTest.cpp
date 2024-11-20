#include "array_types/arrays_mapping/ArraysMapping.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"

namespace array_types
{
namespace arrays_mapping
{

using AllocatorType = ArraysMapping::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

using BitBuffer = zserio::BasicBitBuffer<AllocatorType>;

class ArraysMappingTest : public ::testing::Test
{
protected:
    static const size_t FIXED_ARRAY_LENGTH = 5;
};

TEST_F(ArraysMappingTest, unsignedIntegerArrays)
{
    ArraysMapping arraysMapping;

    arraysMapping.uint8Array = VectorType<zserio::UInt8>(FIXED_ARRAY_LENGTH);
    arraysMapping.uint16Array = VectorType<zserio::UInt16>(FIXED_ARRAY_LENGTH);
    arraysMapping.uint32Array = VectorType<zserio::UInt32>(FIXED_ARRAY_LENGTH);
    arraysMapping.uint64Array = VectorType<zserio::UInt64>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, signedIntegerArrays)
{
    ArraysMapping arraysMapping;

    arraysMapping.int8Array = VectorType<zserio::Int8>(FIXED_ARRAY_LENGTH);
    arraysMapping.int16Array = VectorType<zserio::Int16>(FIXED_ARRAY_LENGTH);
    arraysMapping.int32Array = VectorType<zserio::Int32>(FIXED_ARRAY_LENGTH);
    arraysMapping.int64Array = VectorType<zserio::Int64>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, unsignedBitfieldArrays)
{
    ArraysMapping arraysMapping;

    arraysMapping.bitfield8Array = VectorType<zserio::UInt8>(FIXED_ARRAY_LENGTH);
    arraysMapping.bitfield16Array = VectorType<zserio::UInt16>(FIXED_ARRAY_LENGTH);
    arraysMapping.bitfield32Array = VectorType<zserio::UInt32>(FIXED_ARRAY_LENGTH);
    arraysMapping.bitfield63Array = VectorType<zserio::UInt63>(FIXED_ARRAY_LENGTH);
    arraysMapping.uint8Value = 8;
    arraysMapping.variableBitfieldLongArray = VectorType<zserio::DynUInt64<>>(FIXED_ARRAY_LENGTH);
    arraysMapping.variableBitfieldIntArray = VectorType<zserio::DynUInt32<>>(FIXED_ARRAY_LENGTH);
    arraysMapping.variableBitfieldShortArray = VectorType<zserio::DynUInt16<>>(FIXED_ARRAY_LENGTH);
    arraysMapping.variableBitfieldByteArray = VectorType<zserio::DynUInt8<>>(FIXED_ARRAY_LENGTH);
    arraysMapping.length64 = 64;
    arraysMapping.variableBitfield64Array = VectorType<zserio::DynUInt64<>>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, signedBitfieldArrays)
{
    ArraysMapping arraysMapping;

    arraysMapping.intfield8Array = VectorType<zserio::Int8>(FIXED_ARRAY_LENGTH);
    arraysMapping.intfield16Array = VectorType<zserio::Int16>(FIXED_ARRAY_LENGTH);
    arraysMapping.intfield32Array = VectorType<zserio::Int32>(FIXED_ARRAY_LENGTH);
    arraysMapping.intfield64Array = VectorType<zserio::Int64>(FIXED_ARRAY_LENGTH);
    arraysMapping.uint8Value = 8;
    arraysMapping.variableIntfieldLongArray = VectorType<zserio::DynInt64<>>(FIXED_ARRAY_LENGTH);
    arraysMapping.variableIntfieldIntArray = VectorType<zserio::DynInt32<>>(FIXED_ARRAY_LENGTH);
    arraysMapping.variableIntfieldShortArray = VectorType<zserio::DynInt16<>>(FIXED_ARRAY_LENGTH);
    arraysMapping.variableIntfieldByteArray = VectorType<zserio::DynInt8<>>(FIXED_ARRAY_LENGTH);
    arraysMapping.length32 = 64;
    arraysMapping.variableIntfield64Array = VectorType<zserio::DynInt64<>>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, float16Array)
{
    ArraysMapping arraysMapping;
    arraysMapping.float16Array = VectorType<zserio::Float16>(FIXED_ARRAY_LENGTH);
    arraysMapping.float32Array = VectorType<zserio::Float32>(FIXED_ARRAY_LENGTH);
    arraysMapping.float64Array = VectorType<zserio::Float64>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, variableUnsignedIntegerArrays)
{
    ArraysMapping arraysMapping;

    arraysMapping.varuint16Array = VectorType<zserio::VarUInt16>(FIXED_ARRAY_LENGTH);
    arraysMapping.varuint32Array = VectorType<zserio::VarUInt32>(FIXED_ARRAY_LENGTH);
    arraysMapping.varuint64Array = VectorType<zserio::VarUInt64>(FIXED_ARRAY_LENGTH);
    arraysMapping.varuintArray = VectorType<zserio::VarUInt>(FIXED_ARRAY_LENGTH);
    arraysMapping.varsizeArray = VectorType<zserio::VarSize>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, variableSignedIntegerArrays)
{
    ArraysMapping arraysMapping;

    arraysMapping.varint16Array = VectorType<zserio::VarInt16>(FIXED_ARRAY_LENGTH);
    arraysMapping.varint32Array = VectorType<zserio::VarInt32>(FIXED_ARRAY_LENGTH);
    arraysMapping.varint64Array = VectorType<zserio::VarInt64>(FIXED_ARRAY_LENGTH);
    arraysMapping.varintArray = VectorType<zserio::VarInt>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, boolArray)
{
    ArraysMapping arraysMapping;
    arraysMapping.boolArray = VectorType<zserio::Bool>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, stringArray)
{
    ArraysMapping arraysMapping;
    arraysMapping.stringArray = VectorType<StringType>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, externArray)
{
    ArraysMapping arraysMapping;
    arraysMapping.externArray = VectorType<BitBuffer>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, bytesArray)
{
    ArraysMapping arraysMapping;
    arraysMapping.bytesArray = VectorType<VectorType<uint8_t>>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, compoundArray)
{
    ArraysMapping arraysMapping;
    arraysMapping.compoundArray = VectorType<TestStructure>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, enumArray)
{
    ArraysMapping arraysMapping;
    arraysMapping.enumArray = VectorType<TestEnum>(FIXED_ARRAY_LENGTH);
}

TEST_F(ArraysMappingTest, bitmaskArray)
{
    ArraysMapping arraysMapping;
    arraysMapping.bitmaskArray = VectorType<TestBitmask>(FIXED_ARRAY_LENGTH);
}

} // namespace arrays_mapping
} // namespace array_types
