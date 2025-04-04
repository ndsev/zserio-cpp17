#include "array_types/packed_arrays_mapping/PackedArraysMapping.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"

namespace array_types
{
namespace packed_arrays_mapping
{

using allocator_type = PackedArraysMapping::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class PackedArraysMappingTest : public ::testing::Test
{
protected:
    static const size_t fixedArrayLength = 5;
};

TEST_F(PackedArraysMappingTest, unsignedIntegerArrays)
{
    PackedArraysMapping packedArraysMapping;

    packedArraysMapping.uint8Array = VectorType<zserio::UInt8>(fixedArrayLength);
    packedArraysMapping.uint16Array = VectorType<zserio::UInt16>(fixedArrayLength);
    packedArraysMapping.uint32Array = VectorType<zserio::UInt32>(fixedArrayLength);
    packedArraysMapping.uint64Array = VectorType<zserio::UInt64>(fixedArrayLength);
}

TEST_F(PackedArraysMappingTest, signedIntegerArrays)
{
    PackedArraysMapping packedArraysMapping;

    packedArraysMapping.int8Array = VectorType<zserio::Int8>(fixedArrayLength);
    packedArraysMapping.int16Array = VectorType<zserio::Int16>(fixedArrayLength);
    packedArraysMapping.int32Array = VectorType<zserio::Int32>(fixedArrayLength);
    packedArraysMapping.int64Array = VectorType<zserio::Int64>(fixedArrayLength);
}

TEST_F(PackedArraysMappingTest, unsignedBitfieldArrays)
{
    PackedArraysMapping packedArraysMapping;

    packedArraysMapping.bitfield8Array = VectorType<zserio::UInt8>(fixedArrayLength);
    packedArraysMapping.bitfield16Array = VectorType<zserio::UInt16>(fixedArrayLength);
    packedArraysMapping.bitfield32Array = VectorType<zserio::UInt32>(fixedArrayLength);
    packedArraysMapping.bitfield63Array = VectorType<zserio::UInt63>(fixedArrayLength);
    packedArraysMapping.uint8Value = 8;
    packedArraysMapping.variableBitfieldLongArray = VectorType<zserio::DynUInt64<>>(fixedArrayLength);
}

TEST_F(PackedArraysMappingTest, signedBitfieldArrays)
{
    PackedArraysMapping packedArraysMapping;

    packedArraysMapping.intfield8Array = VectorType<zserio::Int8>(fixedArrayLength);
    packedArraysMapping.intfield16Array = VectorType<zserio::Int16>(fixedArrayLength);
    packedArraysMapping.intfield32Array = VectorType<zserio::Int32>(fixedArrayLength);
    packedArraysMapping.intfield64Array = VectorType<zserio::Int64>(fixedArrayLength);
    packedArraysMapping.uint8Value = 8;
    packedArraysMapping.variableIntfieldLongArray = VectorType<zserio::DynInt64<>>(fixedArrayLength);
}

TEST_F(PackedArraysMappingTest, variableUnsignedIntegerArrays)
{
    PackedArraysMapping packedArraysMapping;

    packedArraysMapping.varuint16Array = VectorType<zserio::VarUInt16>(fixedArrayLength);
    packedArraysMapping.varuint32Array = VectorType<zserio::VarUInt32>(fixedArrayLength);
    packedArraysMapping.varuint64Array = VectorType<zserio::VarUInt64>(fixedArrayLength);
    packedArraysMapping.varuintArray = VectorType<zserio::VarUInt>(fixedArrayLength);
    packedArraysMapping.varsizeArray = VectorType<zserio::VarSize>(fixedArrayLength);
}

TEST_F(PackedArraysMappingTest, variableSignedIntegerArrays)
{
    PackedArraysMapping packedArraysMapping;

    packedArraysMapping.varint16Array = VectorType<zserio::VarInt16>(fixedArrayLength);
    packedArraysMapping.varint32Array = VectorType<zserio::VarInt32>(fixedArrayLength);
    packedArraysMapping.varint64Array = VectorType<zserio::VarInt64>(fixedArrayLength);
    packedArraysMapping.varintArray = VectorType<zserio::VarInt>(fixedArrayLength);
}

TEST_F(PackedArraysMappingTest, compoundArray)
{
    PackedArraysMapping packedArraysMapping;
    packedArraysMapping.compoundArray = VectorType<TestStructure>(fixedArrayLength);
}

TEST_F(PackedArraysMappingTest, enumArray)
{
    PackedArraysMapping packedArraysMapping;
    packedArraysMapping.enumArray = VectorType<TestEnum>(fixedArrayLength);
}

TEST_F(PackedArraysMappingTest, bitmaskArray)
{
    PackedArraysMapping packedArraysMapping;
    packedArraysMapping.bitmaskArray = VectorType<TestBitmask>(fixedArrayLength);
}

} // namespace packed_arrays_mapping
} // namespace array_types
