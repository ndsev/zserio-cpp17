#include <limits>

#include "builtin_types/all_builtin_types/AllBuiltInTypes.h"
#include "builtin_types/all_builtin_types/ExternalStructure.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/OutOfRangeException.h"
#include "zserio/RebindAlloc.h"

namespace builtin_types
{
namespace all_builtin_types
{

using AllocatorType = AllBuiltInTypes::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

using BitBuffer = zserio::BasicBitBuffer<zserio::RebindAlloc<AllocatorType, uint8_t>>;

class AllBuiltInTypesTest : public ::testing::Test
{
public:
    AllBuiltInTypesTest() :
            m_data(),
            m_view(m_data)
    {
        m_data.uint8Type = 1; // set to have valid data
    }

protected:
    static BitBuffer getExternalBitBuffer()
    {
        ExternalStructure externalStructure(0xCD, 0x03);
        return zserio::serialize(externalStructure);
    }

    AllBuiltInTypes m_data;
    zserio::View<AllBuiltInTypes> m_view;

    static constexpr std::string_view BLOB_NAME = "language/builtin_types/all_builtin_types.blob";
};

TEST_F(AllBuiltInTypesTest, uint8Type)
{
    const zserio::UInt8 maxUint8Type = std::numeric_limits<uint8_t>::max();
    m_data.uint8Type = maxUint8Type;
    ASSERT_EQ(maxUint8Type, m_data.uint8Type);
    ASSERT_EQ(maxUint8Type, m_view.uint8Type());
}

TEST_F(AllBuiltInTypesTest, uint16Type)
{
    const zserio::UInt16 maxUint16Type = std::numeric_limits<uint16_t>::max();
    m_data.uint16Type = maxUint16Type;
    ASSERT_EQ(maxUint16Type, m_data.uint16Type);
    ASSERT_EQ(maxUint16Type, m_view.uint16Type());
}

TEST_F(AllBuiltInTypesTest, uint32Type)
{
    const zserio::UInt32 maxUint32Type = std::numeric_limits<uint32_t>::max();
    m_data.uint32Type = maxUint32Type;
    ASSERT_EQ(maxUint32Type, m_data.uint32Type);
    ASSERT_EQ(maxUint32Type, m_view.uint32Type());
}

TEST_F(AllBuiltInTypesTest, uint64Type)
{
    const uint64_t maxUint64Type = std::numeric_limits<uint64_t>::max();
    m_data.uint64Type = maxUint64Type;
    ASSERT_EQ(maxUint64Type, m_data.uint64Type);
    ASSERT_EQ(maxUint64Type, m_view.uint64Type());
}

TEST_F(AllBuiltInTypesTest, int8Type)
{
    const zserio::Int8 maxInt8Type = std::numeric_limits<int8_t>::max();
    m_data.int8Type = maxInt8Type;
    ASSERT_EQ(maxInt8Type, m_data.int8Type);
    ASSERT_EQ(maxInt8Type, m_view.int8Type());
}

TEST_F(AllBuiltInTypesTest, int16Type)
{
    const zserio::Int16 maxInt16Type = std::numeric_limits<int16_t>::max();
    m_data.int16Type = maxInt16Type;
    ASSERT_EQ(maxInt16Type, m_data.int16Type);
    ASSERT_EQ(maxInt16Type, m_view.int16Type());
}

TEST_F(AllBuiltInTypesTest, int32Type)
{
    const zserio::Int32 maxInt32Type = std::numeric_limits<int32_t>::max();
    m_data.int32Type = maxInt32Type;
    ASSERT_EQ(maxInt32Type, m_data.int32Type);
    ASSERT_EQ(maxInt32Type, m_view.int32Type());
}

TEST_F(AllBuiltInTypesTest, int64Type)
{
    const zserio::Int64 maxInt64Type = std::numeric_limits<int64_t>::max();
    m_data.int64Type = maxInt64Type;
    ASSERT_EQ(maxInt64Type, m_data.int64Type);
    ASSERT_EQ(maxInt64Type, m_view.int64Type());
}

TEST_F(AllBuiltInTypesTest, bitfield7Type)
{
    const zserio::UInt7 maxBitfield7Type = zserio::NumericLimits<zserio::UInt7>::max();
    m_data.bitfield7Type = maxBitfield7Type + 1;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.bitfield7Type = maxBitfield7Type;
    ASSERT_EQ(maxBitfield7Type, m_data.bitfield7Type);
    ASSERT_EQ(maxBitfield7Type, m_view.bitfield7Type());
}

TEST_F(AllBuiltInTypesTest, bitfield8Type)
{
    const uint8_t maxBitfield8Type = std::numeric_limits<uint8_t>::max();
    m_data.bitfield8Type = maxBitfield8Type;
    ASSERT_EQ(maxBitfield8Type, m_data.bitfield8Type);
    ASSERT_EQ(maxBitfield8Type, m_view.bitfield8Type());
}

TEST_F(AllBuiltInTypesTest, bitfield15Type)
{
    const uint16_t maxBitfield15Type = zserio::NumericLimits<zserio::UInt15>::max();
    m_data.bitfield15Type = maxBitfield15Type + 1;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.bitfield15Type = maxBitfield15Type;
    ASSERT_EQ(maxBitfield15Type, m_data.bitfield15Type);
    ASSERT_EQ(maxBitfield15Type, m_view.bitfield15Type());
}

TEST_F(AllBuiltInTypesTest, bitfield16Type)
{
    const uint16_t maxBitfield16Type = std::numeric_limits<uint16_t>::max();
    m_data.bitfield16Type = maxBitfield16Type;
    ASSERT_EQ(maxBitfield16Type, m_data.bitfield16Type);
    ASSERT_EQ(maxBitfield16Type, m_view.bitfield16Type());
}

TEST_F(AllBuiltInTypesTest, bitfield31Type)
{
    const uint32_t maxBitfield31Type = zserio::NumericLimits<zserio::UInt31>::max();
    m_data.bitfield31Type = maxBitfield31Type + 1;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.bitfield31Type = maxBitfield31Type;
    ASSERT_EQ(maxBitfield31Type, m_data.bitfield31Type);
    ASSERT_EQ(maxBitfield31Type, m_view.bitfield31Type());
}

TEST_F(AllBuiltInTypesTest, bitfield32Type)
{
    const uint32_t maxBitfield32Type = std::numeric_limits<uint32_t>::max();
    m_data.bitfield32Type = maxBitfield32Type;
    ASSERT_EQ(maxBitfield32Type, m_data.bitfield32Type);
    ASSERT_EQ(maxBitfield32Type, m_view.bitfield32Type());
}

TEST_F(AllBuiltInTypesTest, bitfield63Type)
{
    const uint64_t maxBitfield63Type = zserio::NumericLimits<zserio::UInt63>::max();
    m_data.bitfield63Type = maxBitfield63Type + 1;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.bitfield63Type = maxBitfield63Type;
    ASSERT_EQ(maxBitfield63Type, m_data.bitfield63Type);
    ASSERT_EQ(maxBitfield63Type, m_view.bitfield63Type());
}

TEST_F(AllBuiltInTypesTest, variableBitfieldType)
{
    m_data.uint8Type = 1;
    m_data.variableBitfieldType = 2;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.uint8Type = std::numeric_limits<uint8_t>::max();
    const zserio::DynUInt64<> maxVariableBitfieldType = std::numeric_limits<uint64_t>::max();
    m_data.variableBitfieldType = maxVariableBitfieldType;
    ASSERT_EQ(maxVariableBitfieldType, m_data.variableBitfieldType);
    ASSERT_EQ(maxVariableBitfieldType, m_view.variableBitfieldType());
}

TEST_F(AllBuiltInTypesTest, variableBitfield8Type)
{
    const zserio::DynUInt8<8> maxVariableBitfield8Type = std::numeric_limits<uint8_t>::max();
    m_data.variableBitfield8Type = maxVariableBitfield8Type;
    ASSERT_EQ(maxVariableBitfield8Type, m_data.variableBitfield8Type);
    ASSERT_EQ(maxVariableBitfield8Type, m_view.variableBitfield8Type());
}

TEST_F(AllBuiltInTypesTest, intfield8Type)
{
    const int8_t maxIntfield8Type = std::numeric_limits<int8_t>::max();
    m_data.intfield8Type = maxIntfield8Type;
    ASSERT_EQ(maxIntfield8Type, m_data.intfield8Type);
    ASSERT_EQ(maxIntfield8Type, m_view.intfield8Type());
}

TEST_F(AllBuiltInTypesTest, intfield16Type)
{
    const int16_t maxIntfield16Type = std::numeric_limits<int16_t>::max();
    m_data.intfield16Type = maxIntfield16Type;
    ASSERT_EQ(maxIntfield16Type, m_data.intfield16Type);
    ASSERT_EQ(maxIntfield16Type, m_view.intfield16Type());
}

TEST_F(AllBuiltInTypesTest, intfield32Type)
{
    const int32_t maxIntfield32Type = std::numeric_limits<int32_t>::max();
    m_data.intfield32Type = maxIntfield32Type;
    ASSERT_EQ(maxIntfield32Type, m_data.intfield32Type);
    ASSERT_EQ(maxIntfield32Type, m_view.intfield32Type());
}

TEST_F(AllBuiltInTypesTest, intfield64Type)
{
    const int64_t maxIntfield64Type = std::numeric_limits<int64_t>::max();
    m_data.intfield64Type = maxIntfield64Type;
    ASSERT_EQ(maxIntfield64Type, m_data.intfield64Type);
    ASSERT_EQ(maxIntfield64Type, m_view.intfield64Type());
}

TEST_F(AllBuiltInTypesTest, variableIntfieldType)
{
    // 14 is result of the length expression
    const zserio::DynInt16<> variableIntfieldTypeMax = zserio::NumericLimits<zserio::DynInt16<>>::max(14);
    m_data.variableIntfieldType = variableIntfieldTypeMax + 1;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.variableIntfieldType = variableIntfieldTypeMax;
    ASSERT_EQ(variableIntfieldTypeMax, m_data.variableIntfieldType);
    ASSERT_EQ(variableIntfieldTypeMax, m_view.variableIntfieldType());
}

TEST_F(AllBuiltInTypesTest, variableIntfield8Type)
{
    const zserio::DynInt8<8> maxVariableIntfield8Type = std::numeric_limits<int8_t>::max();
    m_data.variableIntfield8Type = maxVariableIntfield8Type;
    ASSERT_EQ(maxVariableIntfield8Type, m_data.variableIntfield8Type);
    ASSERT_EQ(maxVariableIntfield8Type, m_view.variableIntfield8Type());
}

TEST_F(AllBuiltInTypesTest, float16Type)
{
    const zserio::Float16 maxFloat16Type = zserio::NumericLimits<zserio::Float16>::max();
    m_data.float16Type = maxFloat16Type;
    ASSERT_TRUE(maxFloat16Type - m_data.float16Type <= std::numeric_limits<float>::epsilon());
    ASSERT_TRUE(maxFloat16Type - m_view.float16Type() <= std::numeric_limits<float>::epsilon());
}

TEST_F(AllBuiltInTypesTest, float32Type)
{
    const zserio::Float32 maxFloat32Type = std::numeric_limits<float>::max();
    m_data.float32Type = maxFloat32Type;
    ASSERT_TRUE(maxFloat32Type - m_data.float32Type <= std::numeric_limits<float>::epsilon());
    ASSERT_TRUE(maxFloat32Type - m_view.float32Type() <= std::numeric_limits<float>::epsilon());
}

TEST_F(AllBuiltInTypesTest, float64Type)
{
    const zserio::Float64 maxFloat64Type = std::numeric_limits<double>::max();
    m_data.float64Type = maxFloat64Type;
    ASSERT_TRUE(maxFloat64Type - m_data.float64Type <= std::numeric_limits<double>::epsilon());
    ASSERT_TRUE(maxFloat64Type - m_view.float64Type() <= std::numeric_limits<double>::epsilon());
}

TEST_F(AllBuiltInTypesTest, varuint16Type)
{
    const zserio::VarUInt16 maxVaruint16Type = zserio::NumericLimits<zserio::VarUInt16>::max();
    m_data.varuint16Type = maxVaruint16Type + 1;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.varuint16Type = maxVaruint16Type;
    ASSERT_EQ(maxVaruint16Type, m_data.varuint16Type);
    ASSERT_EQ(maxVaruint16Type, m_view.varuint16Type());
}

TEST_F(AllBuiltInTypesTest, varuint32Type)
{
    const zserio::VarUInt32 maxVaruint32Type = zserio::NumericLimits<zserio::VarUInt32>::max();
    m_data.varuint32Type = maxVaruint32Type + 1;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.varuint32Type = maxVaruint32Type;
    ASSERT_EQ(maxVaruint32Type, m_data.varuint32Type);
    ASSERT_EQ(maxVaruint32Type, m_view.varuint32Type());
}

TEST_F(AllBuiltInTypesTest, varuint64Type)
{
    const zserio::VarUInt64 maxVaruint64Type = zserio::NumericLimits<zserio::VarUInt64>::max();
    m_data.varuint64Type = maxVaruint64Type + 1;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.varuint64Type = maxVaruint64Type;
    ASSERT_EQ(maxVaruint64Type, m_data.varuint64Type);
    ASSERT_EQ(maxVaruint64Type, m_view.varuint64Type());
}

TEST_F(AllBuiltInTypesTest, varuintType)
{
    const zserio::VarUInt minVaruintType = zserio::NumericLimits<zserio::VarUInt>::min();
    m_data.varuintType = minVaruintType;
    ASSERT_EQ(minVaruintType, m_data.varuintType);
    ASSERT_EQ(minVaruintType, m_view.varuintType());

    const zserio::VarUInt maxVaruintType = zserio::NumericLimits<zserio::VarUInt>::max();
    m_data.varuintType = maxVaruintType;
    ASSERT_EQ(maxVaruintType, m_data.varuintType);
    ASSERT_EQ(maxVaruintType, m_view.varuintType());
}

TEST_F(AllBuiltInTypesTest, varsizeType)
{
    const zserio::VarSize maxVarSizeType = zserio::NumericLimits<zserio::VarSize>::max();
    m_data.varsizeType = maxVarSizeType + 1;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.varsizeType = maxVarSizeType;
    const uint32_t varsizeType = m_data.varsizeType;
    ASSERT_EQ(maxVarSizeType, varsizeType);
}

TEST_F(AllBuiltInTypesTest, varint16Type)
{
    const zserio::VarInt16 maxVarint16Type = zserio::NumericLimits<zserio::VarInt16>::max();
    m_data.varint16Type = static_cast<zserio::VarInt16::ValueType>(maxVarint16Type + 1);
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.varint16Type = maxVarint16Type;
    ASSERT_EQ(maxVarint16Type, m_data.varint16Type);
    ASSERT_EQ(maxVarint16Type, m_view.varint16Type());
}

TEST_F(AllBuiltInTypesTest, varint32Type)
{
    const zserio::VarInt32 maxVarint32Type = zserio::NumericLimits<zserio::VarInt32>::max();
    m_data.varint32Type = maxVarint32Type + 1;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.varint32Type = maxVarint32Type;
    ASSERT_EQ(maxVarint32Type, m_data.varint32Type);
    ASSERT_EQ(maxVarint32Type, m_view.varint32Type());
}

TEST_F(AllBuiltInTypesTest, varint64Type)
{
    const zserio::VarInt64 maxVarint64Type = zserio::NumericLimits<zserio::VarInt64>::max();
    m_data.varint64Type = maxVarint64Type + 1;
    ASSERT_THROW(zserio::detail::validate(m_view), zserio::OutOfRangeException);

    m_data.varint64Type = maxVarint64Type;
    ASSERT_EQ(maxVarint64Type, m_data.varint64Type);
    ASSERT_EQ(maxVarint64Type, m_view.varint64Type());
}

TEST_F(AllBuiltInTypesTest, varintType)
{
    const zserio::VarInt minVarintType = zserio::NumericLimits<zserio::VarInt>::min();
    m_data.varintType = minVarintType;
    ASSERT_EQ(minVarintType, m_data.varintType);
    ASSERT_EQ(minVarintType, m_view.varintType());

    const zserio::VarInt maxVarintType = zserio::NumericLimits<zserio::VarInt>::max();
    m_data.varintType = maxVarintType;
    ASSERT_EQ(maxVarintType, m_data.varintType);
    ASSERT_EQ(maxVarintType, m_view.varintType());
}

TEST_F(AllBuiltInTypesTest, boolType)
{
    m_data.boolType = true;
    ASSERT_EQ(true, m_data.boolType);
    ASSERT_EQ(true, m_view.boolType());
}

TEST_F(AllBuiltInTypesTest, stringType)
{
    const StringType testString("TEST");
    m_data.stringType = testString;
    ASSERT_EQ(testString, m_data.stringType);
    ASSERT_EQ(testString, m_view.stringType());
}

TEST_F(AllBuiltInTypesTest, externType)
{
    const BitBuffer testExtern = getExternalBitBuffer();
    m_data.externType = testExtern;
    ASSERT_EQ(testExtern, m_data.externType);
    ASSERT_EQ(testExtern, m_view.externType());
}

TEST_F(AllBuiltInTypesTest, bytesType)
{
    const VectorType<uint8_t> testBytes{{1, 255}};
    m_data.bytesType = testBytes;
    ASSERT_EQ(testBytes, m_data.bytesType);
    ASSERT_EQ(testBytes, m_view.bytesType());
}

TEST_F(AllBuiltInTypesTest, bitSizeOf)
{
    m_data.boolType = true;
    m_data.uint8Type = 1;
    m_data.uint16Type = std::numeric_limits<uint16_t>::max();
    m_data.uint32Type = std::numeric_limits<uint32_t>::max();
    m_data.uint64Type = 10;
    m_data.int8Type = std::numeric_limits<int8_t>::max();
    m_data.int16Type = std::numeric_limits<int16_t>::max();
    m_data.int32Type = std::numeric_limits<int32_t>::max();
    m_data.int64Type = std::numeric_limits<int64_t>::max();
    m_data.bitfield7Type = zserio::NumericLimits<zserio::UInt7>::max();
    m_data.bitfield8Type = std::numeric_limits<uint8_t>::max();
    m_data.bitfield15Type = zserio::NumericLimits<zserio::UInt15>::max();
    m_data.bitfield16Type = std::numeric_limits<uint16_t>::max();
    m_data.bitfield31Type = zserio::NumericLimits<zserio::UInt31>::max();
    m_data.bitfield32Type = std::numeric_limits<uint32_t>::max();
    m_data.bitfield63Type = zserio::NumericLimits<zserio::UInt63>::max();
    m_data.variableBitfieldType = 1;
    m_data.variableBitfield8Type = std::numeric_limits<uint8_t>::max();
    m_data.intfield8Type = std::numeric_limits<int8_t>::max();
    m_data.intfield16Type = std::numeric_limits<int16_t>::max();
    m_data.intfield32Type = std::numeric_limits<int32_t>::max();
    m_data.intfield64Type = std::numeric_limits<int64_t>::max();
    m_data.variableIntfieldType = zserio::NumericLimits<zserio::DynInt16<>>::max(14);
    m_data.variableIntfield8Type = std::numeric_limits<int8_t>::max();
    m_data.float16Type = zserio::NumericLimits<zserio::Float16>::max();
    m_data.float32Type = std::numeric_limits<float>::max();
    m_data.float64Type = std::numeric_limits<double>::max();
    m_data.varuint16Type = zserio::NumericLimits<zserio::VarUInt16>::max();
    m_data.varuint32Type = zserio::NumericLimits<zserio::VarUInt32>::max();
    m_data.varuint64Type = zserio::NumericLimits<zserio::VarUInt64>::max();
    m_data.varuintType = std::numeric_limits<uint64_t>::max();
    m_data.varsizeType = zserio::NumericLimits<zserio::VarSize>::max();
    m_data.varint16Type = zserio::NumericLimits<zserio::VarInt16>::max();
    m_data.varint32Type = zserio::NumericLimits<zserio::VarInt32>::max();
    m_data.varint64Type = zserio::NumericLimits<zserio::VarInt64>::max();
    m_data.varintType = zserio::NumericLimits<zserio::VarInt>::max();
    m_data.stringType = "TEST";
    m_data.externType = getExternalBitBuffer();
    m_data.bytesType = VectorType<uint8_t>{{1, 255}};
    const size_t expectedBitSizeOf = 1166;
    ASSERT_EQ(expectedBitSizeOf, zserio::detail::bitSizeOf(m_view));
}

TEST_F(AllBuiltInTypesTest, writeReadFile)
{
    m_data.boolType = true;
    m_data.uint8Type = 8;
    m_data.uint16Type = std::numeric_limits<uint16_t>::max();
    m_data.uint32Type = std::numeric_limits<uint32_t>::max();
    m_data.uint64Type = std::numeric_limits<uint64_t>::max();
    m_data.int8Type = std::numeric_limits<int8_t>::max();
    m_data.int16Type = std::numeric_limits<int16_t>::max();
    m_data.int32Type = std::numeric_limits<int32_t>::max();
    m_data.int64Type = std::numeric_limits<int64_t>::max();
    m_data.bitfield7Type = zserio::NumericLimits<zserio::UInt7>::max();
    m_data.bitfield8Type = std::numeric_limits<uint8_t>::max();
    m_data.bitfield15Type = zserio::NumericLimits<zserio::UInt15>::max();
    m_data.bitfield16Type = std::numeric_limits<uint16_t>::max();
    m_data.bitfield31Type = zserio::NumericLimits<zserio::UInt31>::max();
    m_data.bitfield32Type = std::numeric_limits<uint32_t>::max();
    m_data.bitfield63Type = zserio::NumericLimits<zserio::UInt63>::max();
    m_data.variableBitfieldType = std::numeric_limits<uint8_t>::max();
    m_data.variableBitfield8Type = std::numeric_limits<uint8_t>::max();
    m_data.intfield8Type = std::numeric_limits<int8_t>::max();
    m_data.intfield16Type = std::numeric_limits<int16_t>::max();
    m_data.intfield32Type = std::numeric_limits<int32_t>::max();
    m_data.intfield64Type = std::numeric_limits<int64_t>::max();
    m_data.variableIntfieldType = zserio::NumericLimits<zserio::DynInt16<>>::max(14);
    m_data.variableIntfield8Type = std::numeric_limits<int8_t>::max();
    m_data.float16Type = 1.0F;
    m_data.float32Type = std::numeric_limits<float>::max();
    m_data.float64Type = std::numeric_limits<double>::max();
    m_data.varuint16Type = zserio::NumericLimits<zserio::VarUInt16>::max();
    m_data.varuint32Type = zserio::NumericLimits<zserio::VarUInt32>::max();
    m_data.varuint64Type = zserio::NumericLimits<zserio::VarUInt64>::max();
    m_data.varuintType = zserio::NumericLimits<zserio::VarUInt>::max();
    m_data.varsizeType = zserio::NumericLimits<zserio::VarSize>::max();
    m_data.varint16Type = zserio::NumericLimits<zserio::VarInt16>::max();
    m_data.varint32Type = zserio::NumericLimits<zserio::VarInt32>::max();
    m_data.varint64Type = zserio::NumericLimits<zserio::VarInt64>::max();
    m_data.varintType = std::numeric_limits<int64_t>::max();
    m_data.stringType = "TEST";
    m_data.externType = getExternalBitBuffer();
    m_data.bytesType = VectorType<uint8_t>{{1, 255}};

    test_utils::writeReadFileTest(BLOB_NAME, m_data);
}

} // namespace all_builtin_types
} // namespace builtin_types
