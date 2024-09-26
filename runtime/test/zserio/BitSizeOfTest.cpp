#include "gtest/gtest.h"
#include "zserio/BitBuffer.h"
#include "zserio/BitSizeOf.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/String.h"
#include "zserio/Types.h"

namespace zserio
{

TEST(BitSizeOfCalculatorTest, bitSizeOfBool)
{
    EXPECT_EQ(1, detail::bitSizeOf(Bool(true)));
    EXPECT_EQ(1, detail::bitSizeOf(Bool(false)));
}

TEST(BitSizeOfCalculatorTest, bitSizeOfIntWrapper)
{
    EXPECT_EQ(1, detail::bitSizeOf(Int1()));
    EXPECT_EQ(7, detail::bitSizeOf(Int7()));
    EXPECT_EQ(8, detail::bitSizeOf(Int8()));
    EXPECT_EQ(15, detail::bitSizeOf(Int15()));
    EXPECT_EQ(16, detail::bitSizeOf(Int16()));
    EXPECT_EQ(17, detail::bitSizeOf(Int17()));
    EXPECT_EQ(31, detail::bitSizeOf(Int31()));
    EXPECT_EQ(32, detail::bitSizeOf(Int32()));
    EXPECT_EQ(33, detail::bitSizeOf(Int33()));
    EXPECT_EQ(63, detail::bitSizeOf(Int63()));
    EXPECT_EQ(64, detail::bitSizeOf(Int64()));

    EXPECT_EQ(1, detail::bitSizeOf(UInt1()));
    EXPECT_EQ(7, detail::bitSizeOf(UInt7()));
    EXPECT_EQ(8, detail::bitSizeOf(UInt8()));
    EXPECT_EQ(15, detail::bitSizeOf(UInt15()));
    EXPECT_EQ(16, detail::bitSizeOf(UInt16()));
    EXPECT_EQ(17, detail::bitSizeOf(UInt17()));
    EXPECT_EQ(31, detail::bitSizeOf(UInt31()));
    EXPECT_EQ(32, detail::bitSizeOf(UInt32()));
    EXPECT_EQ(33, detail::bitSizeOf(UInt33()));
    EXPECT_EQ(63, detail::bitSizeOf(UInt63()));
    EXPECT_EQ(64, detail::bitSizeOf(UInt64()));
}

TEST(BitSizeOfCalculatorTest, bitSizeOfVarInt16)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarInt16(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarInt16(static_cast<int16_t>(1U << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt16(-static_cast<int16_t>(1U << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt16(static_cast<int16_t>((1U << (6U)) - 1))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt16(-static_cast<int16_t>((1U << (6U)) - 1))));

    EXPECT_EQ(16, detail::bitSizeOf(VarInt16(static_cast<int16_t>(1U << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt16(-static_cast<int16_t>(1U << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt16(static_cast<int16_t>((1U << (6U + 8)) - 1))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt16(-static_cast<int16_t>((1U << (6U + 8)) - 1))));

    const int16_t outOfRangeValue = static_cast<int16_t>(1U << (6U + 8));
    ASSERT_THROW(detail::bitSizeOf(VarInt16(outOfRangeValue)), CppRuntimeException);
}

TEST(BitSizeOfCalculatorTest, bitSizeOfVarInt32)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarInt32(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarInt32(static_cast<int32_t>(1U << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(1U << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt32(static_cast<int32_t>((1U << (6U)) - 1))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(((1U << (6U)) - 1)))));

    EXPECT_EQ(16, detail::bitSizeOf(VarInt32(static_cast<int32_t>(1U << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(1U << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt32(static_cast<int32_t>((1U << (6U + 7)) - 1))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(((1U << (6U + 7)) - 1)))));

    EXPECT_EQ(24, detail::bitSizeOf(VarInt32(static_cast<int32_t>(1U << (6U + 7)))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(1U << (6U + 7)))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt32(static_cast<int32_t>((1U << (6U + 7 + 7)) - 1))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt32(-static_cast<int32_t>((1U << (6U + 7 + 7)) - 1))));

    EXPECT_EQ(32, detail::bitSizeOf(VarInt32(static_cast<int32_t>(1U << (6U + 7 + 7)))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(1U << (6U + 7 + 7)))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt32(static_cast<int32_t>((1U << (6U + 7 + 7 + 8)) - 1))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt32(-static_cast<int32_t>((1U << (6U + 7 + 7 + 8)) - 1))));

    const int32_t outOfRangeValue = static_cast<int32_t>(1U << (6U + 7 + 7 + 8));
    ASSERT_THROW(detail::bitSizeOf(VarInt32(outOfRangeValue)), CppRuntimeException);
}

TEST(BitSizeOfCalculatorTest, bitSizeOfVarInt64)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarInt64(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U)) - 1))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U)) - 1))));

    EXPECT_EQ(16, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7)) - 1))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7)) - 1))));

    EXPECT_EQ(24, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7)))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7)))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7)) - 1))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7)) - 1))));

    EXPECT_EQ(32, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7)))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7)))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7)) - 1))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7)) - 1))));

    EXPECT_EQ(40, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7)))));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7)))));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7)) - 1))));
    EXPECT_EQ(
            40, detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7)) - 1))));

    EXPECT_EQ(48, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(48, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(48,
            detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7)) - 1))));
    EXPECT_EQ(48,
            detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7)) - 1))));

    EXPECT_EQ(56, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(56, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(56,
            detail::bitSizeOf(
                    VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7)) - 1))));
    EXPECT_EQ(56,
            detail::bitSizeOf(
                    VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7)) - 1))));

    EXPECT_EQ(
            64, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(64,
            detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(64,
            detail::bitSizeOf(
                    VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7 + 8)) - 1))));
    EXPECT_EQ(64,
            detail::bitSizeOf(
                    VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7 + 8)) - 1))));

    const int64_t outOfRangeValue = static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7 + 8));
    ASSERT_THROW(detail::bitSizeOf(VarInt64(outOfRangeValue)), CppRuntimeException);
}

TEST(BitSizeOfCalculatorTest, bitSizeOfVarUInt16)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt16(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarUInt16(1U << (0U))));
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt16((1U << (7U)) - 1)));

    EXPECT_EQ(16, detail::bitSizeOf(VarUInt16(1U << (7U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarUInt16((1U << (7U + 8)) - 1)));

    const uint16_t outOfRangeValue = 1U << (7U + 8);
    ASSERT_THROW(detail::bitSizeOf(VarUInt16(outOfRangeValue)), CppRuntimeException);
}

TEST(BitSizeOfCalculatorTest, bitSizeOfVarUInt32)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt32(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarUInt32(1U << (0U))));
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt32((1U << (7U)) - 1)));

    EXPECT_EQ(16, detail::bitSizeOf(VarUInt32(1U << (7U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarUInt32((1U << (7U + 7)) - 1)));

    EXPECT_EQ(24, detail::bitSizeOf(VarUInt32(1U << (7U + 7))));
    EXPECT_EQ(24, detail::bitSizeOf(VarUInt32((1U << (7U + 7 + 7)) - 1)));

    EXPECT_EQ(32, detail::bitSizeOf(VarUInt32(1U << (7U + 7 + 7))));
    EXPECT_EQ(32, detail::bitSizeOf(VarUInt32((1U << (7U + 7 + 7 + 8)) - 1)));

    const uint32_t outOfRangeValue = 1U << (7U + 7 + 7 + 8);
    ASSERT_THROW(detail::bitSizeOf(VarUInt32(outOfRangeValue)), CppRuntimeException);
}

TEST(BitSizeOfCalculatorTest, bitSizeOfVarUInt64)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt64(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (0U))));
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U)) - 1)));

    EXPECT_EQ(16, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7)) - 1)));

    EXPECT_EQ(24, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7))));
    EXPECT_EQ(24, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7)) - 1)));

    EXPECT_EQ(32, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7 + 7))));
    EXPECT_EQ(32, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7 + 7)) - 1)));

    EXPECT_EQ(40, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7 + 7 + 7))));
    EXPECT_EQ(40, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7 + 7 + 7)) - 1)));

    EXPECT_EQ(48, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7 + 7 + 7 + 7))));
    EXPECT_EQ(48, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7)) - 1)));

    EXPECT_EQ(56, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7))));
    EXPECT_EQ(56, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7 + 7)) - 1)));

    EXPECT_EQ(64, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7 + 7))));
    EXPECT_EQ(64, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7 + 7 + 8)) - 1)));

    const uint64_t outOfRangeValue = UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7 + 7 + 8);
    ASSERT_THROW(detail::bitSizeOf(VarUInt64(outOfRangeValue)), CppRuntimeException);
}

TEST(BitSizeOfCalculatorTest, bitSizeOfVarInt)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarInt(INT64_C(0))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 6U) + 1)));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 6U) - 1)));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 6U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 6U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 13U) + 1)));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 13U) - 1)));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 13U))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 13U))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 20U) + 1)));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 20U) - 1)));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 20U))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 20U))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 27U) + 1)));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 27U) - 1)));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 27U))));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 27U))));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 34U) + 1)));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 34U) - 1)));
    EXPECT_EQ(48, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 34U))));
    EXPECT_EQ(48, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 34U))));
    EXPECT_EQ(48, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 41U) + 1)));
    EXPECT_EQ(48, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 41U) - 1)));
    EXPECT_EQ(56, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 41U))));
    EXPECT_EQ(56, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 41U))));
    EXPECT_EQ(56, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 48U) + 1)));
    EXPECT_EQ(56, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 48U) - 1)));
    EXPECT_EQ(64, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 48U))));
    EXPECT_EQ(64, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 48U))));
    EXPECT_EQ(64, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 55U) + 1)));
    EXPECT_EQ(64, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 55U) - 1)));
    EXPECT_EQ(72, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 55U))));
    EXPECT_EQ(72, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 55U))));
    EXPECT_EQ(72, detail::bitSizeOf(VarInt(INT64_MIN + 1)));
    EXPECT_EQ(72, detail::bitSizeOf(VarInt(INT64_MAX)));

    // special case, INT64_MIN is stored as -0
    EXPECT_EQ(8, detail::bitSizeOf(VarInt(INT64_MIN)));
}

TEST(BitSizeOfCalculatorTest, bitSizeOfVarUInt)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt(UINT64_C(0))));
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt((UINT64_C(1) << 7U) - 1)));
    EXPECT_EQ(16, detail::bitSizeOf(VarUInt((UINT64_C(1) << 7U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarUInt((UINT64_C(1) << 14U) - 1)));
    EXPECT_EQ(24, detail::bitSizeOf(VarUInt((UINT64_C(1) << 14U))));
    EXPECT_EQ(24, detail::bitSizeOf(VarUInt((UINT64_C(1) << 21U) - 1)));
    EXPECT_EQ(32, detail::bitSizeOf(VarUInt((UINT64_C(1) << 21U))));
    EXPECT_EQ(32, detail::bitSizeOf(VarUInt((UINT64_C(1) << 28U) - 1)));
    EXPECT_EQ(40, detail::bitSizeOf(VarUInt((UINT64_C(1) << 28U))));
    EXPECT_EQ(40, detail::bitSizeOf(VarUInt((UINT64_C(1) << 35U) - 1)));
    EXPECT_EQ(48, detail::bitSizeOf(VarUInt((UINT64_C(1) << 35U))));
    EXPECT_EQ(48, detail::bitSizeOf(VarUInt((UINT64_C(1) << 42U) - 1)));
    EXPECT_EQ(56, detail::bitSizeOf(VarUInt((UINT64_C(1) << 42U))));
    EXPECT_EQ(56, detail::bitSizeOf(VarUInt((UINT64_C(1) << 49U) - 1)));
    EXPECT_EQ(64, detail::bitSizeOf(VarUInt((UINT64_C(1) << 49U))));
    EXPECT_EQ(64, detail::bitSizeOf(VarUInt((UINT64_C(1) << 56U) - 1)));
    EXPECT_EQ(72, detail::bitSizeOf(VarUInt((UINT64_C(1) << 56U))));
    EXPECT_EQ(72, detail::bitSizeOf(VarUInt(UINT64_MAX)));
}

TEST(BitSizeOfCalculatorTest, bitSizeOfVarSize)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarSize(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarSize(1U << (0U))));
    EXPECT_EQ(8, detail::bitSizeOf(VarSize((1U << (7U)) - 1)));

    EXPECT_EQ(16, detail::bitSizeOf(VarSize(1U << (7U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarSize((1U << (7U + 7)) - 1)));

    EXPECT_EQ(24, detail::bitSizeOf(VarSize(1U << (7U + 7))));
    EXPECT_EQ(24, detail::bitSizeOf(VarSize((1U << (7U + 7 + 7)) - 1)));

    EXPECT_EQ(32, detail::bitSizeOf(VarSize(1U << (7U + 7 + 7))));
    EXPECT_EQ(32, detail::bitSizeOf(VarSize((1U << (7U + 7 + 7 + 7)) - 1)));

    EXPECT_EQ(40, detail::bitSizeOf(VarSize(1U << (7U + 7 + 7 + 7))));
    EXPECT_EQ(40, detail::bitSizeOf(VarSize((1U << (2U + 7 + 7 + 7 + 8)) - 1)));

    const uint32_t outOfRangeValue = 1U << (2U + 7 + 7 + 7 + 8);
    ASSERT_THROW(detail::bitSizeOf(VarSize(outOfRangeValue)), CppRuntimeException);
}

TEST(BitSizeOfCalculatorTest, bitSizeOfFloat)
{
    EXPECT_EQ(16, detail::bitSizeOf(Float16()));
    EXPECT_EQ(32, detail::bitSizeOf(Float32()));
    EXPECT_EQ(64, detail::bitSizeOf(Float64()));
}

TEST(BitSizeOfCalculatorTest, bitSizeOfString)
{
    EXPECT_EQ((1 + 1) * 8, detail::bitSizeOf(std::string("T")));
    EXPECT_EQ((1 + 4) * 8, detail::bitSizeOf(std::string("Test")));

    const size_t testStringLength = static_cast<size_t>(1U << 7U);
    std::string testString(testStringLength, '\xAB');
    EXPECT_EQ((2 + testStringLength) * 8, detail::bitSizeOf(testString));
}

TEST(BitSizeOfCalculatorTest, bitSizeOfBitBuffer)
{
    EXPECT_EQ(8 + 8, detail::bitSizeOf(BitBuffer(std::vector<uint8_t>({0xAB, 0xC0}), 8)));
    EXPECT_EQ(8 + 11, detail::bitSizeOf(BitBuffer(std::vector<uint8_t>({0xAB, 0xC0}), 11)));
    EXPECT_EQ(8 + 16, detail::bitSizeOf(BitBuffer(std::vector<uint8_t>({0xAB, 0xCD}), 16)));
    EXPECT_EQ(8 + 16, detail::bitSizeOf(BitBuffer(std::vector<uint8_t>({0xAB, 0xCD}))));

    EXPECT_EQ(8 + 15 * 8 + 7, detail::bitSizeOf(BitBuffer(std::vector<uint8_t>(16), 127)));
    EXPECT_EQ(16 + 16 * 8, detail::bitSizeOf(BitBuffer(std::vector<uint8_t>(16), 128)));
}

} // namespace zserio
