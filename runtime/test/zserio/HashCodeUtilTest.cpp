#include "gtest/gtest.h"
#include "zserio/BitBuffer.h"
#include "zserio/FloatUtil.h"
#include "zserio/HashCodeUtil.h"

namespace zserio
{

TEST(HashCodeUtilTest, simpleTypes)
{
    const uint32_t hashSeed = 1;

    const int intValue = 10;
    EXPECT_EQ(HASH_PRIME_NUMBER + 10, calcHashCode(hashSeed, intValue));

    const bool boolValue = true;
    EXPECT_EQ(HASH_PRIME_NUMBER + 1, calcHashCode(hashSeed, boolValue));

    const uint8_t uint8Value = 10;
    EXPECT_EQ(HASH_PRIME_NUMBER + 10, calcHashCode(hashSeed, uint8Value));

    const uint16_t uint16Value = 10;
    EXPECT_EQ(HASH_PRIME_NUMBER + 10, calcHashCode(hashSeed, uint16Value));

    const uint32_t uint32Value = 10;
    EXPECT_EQ(HASH_PRIME_NUMBER + 10, calcHashCode(hashSeed, uint32Value));

    const uint64_t uint64Value = 10;
    EXPECT_EQ(HASH_PRIME_NUMBER + 10, calcHashCode(hashSeed, uint64Value));

    const int8_t int8Value = 10;
    EXPECT_EQ(HASH_PRIME_NUMBER + 10, calcHashCode(hashSeed, int8Value));

    const int16_t int16Value = 10;
    EXPECT_EQ(HASH_PRIME_NUMBER + 10, calcHashCode(hashSeed, int16Value));

    const int32_t int32Value = 10;
    EXPECT_EQ(HASH_PRIME_NUMBER + 10, calcHashCode(hashSeed, int32Value));
    const int32_t int32Value2 = -1;
    EXPECT_EQ(HASH_PRIME_NUMBER - 1, calcHashCode(hashSeed, int32Value2));

    const int64_t int64Value = -1;
    EXPECT_EQ(HASH_PRIME_NUMBER, calcHashCode(hashSeed, int64Value));

    const float floatValue = 10.0F;
    EXPECT_EQ(HASH_PRIME_NUMBER + convertFloatToUInt32(floatValue), calcHashCode(hashSeed, floatValue));

    const double doubleValue = 10.0;
    const uint64_t uint64DoubleValue = convertDoubleToUInt64(doubleValue);
    const uint32_t expectedHashCode =
            HASH_PRIME_NUMBER + static_cast<uint32_t>(uint64DoubleValue ^ (uint64DoubleValue >> 32U));
    EXPECT_EQ(expectedHashCode, calcHashCode(hashSeed, doubleValue));
}

TEST(HashCodeUtilTest, bitBufferType)
{
    const uint32_t hashSeed = 1;
    const BitBuffer bitBufferValue;
    EXPECT_EQ(HASH_PRIME_NUMBER + HASH_SEED, calcHashCode(hashSeed, bitBufferValue));
}

} // namespace zserio
