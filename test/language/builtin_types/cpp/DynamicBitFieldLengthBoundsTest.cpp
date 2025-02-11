#include "builtin_types/dynamic_bitfield_length_bounds/Container.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace builtin_types
{
namespace dynamic_bitfield_length_bounds
{

using AllocatorType = Container::AllocatorType;
using BitBufferType = zserio::BasicBitBuffer<zserio::RebindAlloc<AllocatorType, uint8_t>>;

class DynamicBitFieldLengthBoundsTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, Container data)
    {
        writer.writeUnsignedBits32(data.unsignedBitLength, 4);
        if (data.unsignedBitLength == 0)
        {
            return;
        }
        writer.writeUnsignedBits32(data.unsignedValue, data.unsignedBitLength);
        writer.writeUnsignedBits32(data.unsignedBigBitLength, 8);
        if (data.unsignedBigBitLength == 0 || data.unsignedBigBitLength > 64)
        {
            return;
        }
        writer.writeUnsignedBits64(data.unsignedBigValue, data.unsignedBigBitLength);
        writer.writeUnsignedBits64(data.signedBitLength, 64);
        if (data.signedBitLength == 0 || data.signedBitLength > 64)
        {
            return;
        }
        writer.writeSignedBits64(data.signedValue, static_cast<uint8_t>(data.signedBitLength));
    }

    static constexpr zserio::UInt4 UNSIGNED_BIT_LENGTH = 15;
    static constexpr zserio::DynUInt16<> UNSIGNED_VALUE = (1U << UNSIGNED_BIT_LENGTH) - 1;
    static constexpr zserio::UInt8 UNSIGNED_BIG_BIT_LENGTH = 13;
    static constexpr zserio::DynUInt64<> UNSIGNED_BIG_VALUE = (1U << UNSIGNED_BIG_BIT_LENGTH) - 1;
    static constexpr zserio::UInt64 SIGNED_BIT_LENGTH = 7;
    static constexpr zserio::DynInt64<> SIGNED_VALUE = -static_cast<int64_t>(1U << (SIGNED_BIT_LENGTH - 1));
};

TEST_F(DynamicBitFieldLengthBoundsTest, read)
{
    Container data{UNSIGNED_BIT_LENGTH, UNSIGNED_VALUE, UNSIGNED_BIG_BIT_LENGTH, UNSIGNED_BIG_VALUE,
            SIGNED_BIT_LENGTH, SIGNED_VALUE};

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, data), data);
}

TEST_F(DynamicBitFieldLengthBoundsTest, writeRead)
{
    Container data{UNSIGNED_BIT_LENGTH, UNSIGNED_VALUE, UNSIGNED_BIG_BIT_LENGTH, UNSIGNED_BIG_VALUE,
            SIGNED_BIT_LENGTH, SIGNED_VALUE};

    test_utils::writeReadTest(data);
}

TEST_F(DynamicBitFieldLengthBoundsTest, unsignedBitLengthZero)
{
    Container data{
            0, UNSIGNED_VALUE, UNSIGNED_BIG_BIT_LENGTH, UNSIGNED_BIG_VALUE, SIGNED_BIT_LENGTH, SIGNED_VALUE};

    ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);

    zserio::View view(data);
    zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
    BitBufferType bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    writeData(writer, data);

    zserio::BitStreamReader reader(bitBuffer);
    Container readData;
    ASSERT_THROW(zserio::detail::read(reader, readData), zserio::CppRuntimeException);
}

TEST_F(DynamicBitFieldLengthBoundsTest, unsignedBitLengthZeroValueZero)
{
    Container data{0, 0, UNSIGNED_BIG_BIT_LENGTH, UNSIGNED_BIG_VALUE, SIGNED_BIT_LENGTH, SIGNED_VALUE};

    ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);

    zserio::View view(data);
    zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
    BitBufferType bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    writeData(writer, data);

    zserio::BitStreamReader reader(bitBuffer);
    Container readData;
    ASSERT_THROW(zserio::detail::read(reader, readData), zserio::CppRuntimeException);
}

TEST_F(DynamicBitFieldLengthBoundsTest, unsignedBigBitLengthZero)
{
    Container data{UNSIGNED_BIT_LENGTH, UNSIGNED_VALUE, 0, UNSIGNED_BIG_VALUE, SIGNED_BIT_LENGTH, SIGNED_VALUE};

    ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);

    zserio::View view(data);
    zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
    BitBufferType bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    writeData(writer, data);

    zserio::BitStreamReader reader(bitBuffer);
    Container readData;
    ASSERT_THROW(zserio::detail::read(reader, readData), zserio::CppRuntimeException);
}

TEST_F(DynamicBitFieldLengthBoundsTest, unsignedBigBitLengthZeroValueZero)
{
    Container data{UNSIGNED_BIT_LENGTH, UNSIGNED_VALUE, 0, 0, SIGNED_BIT_LENGTH, SIGNED_VALUE};

    zserio::View view(data);
    zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
    BitBufferType bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    writeData(writer, data);

    zserio::BitStreamReader reader(bitBuffer);
    Container readData;
    ASSERT_THROW(zserio::detail::read(reader, readData), zserio::CppRuntimeException);
}

TEST_F(DynamicBitFieldLengthBoundsTest, unsignedBigBitLengthOverMax)
{
    Container data{
            UNSIGNED_BIT_LENGTH, UNSIGNED_VALUE, 65, UNSIGNED_BIG_VALUE, SIGNED_BIT_LENGTH, SIGNED_VALUE};

    ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);

    zserio::View view(data);
    zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
    BitBufferType bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    writeData(writer, data);

    zserio::BitStreamReader reader(bitBuffer);
    Container readData;
    ASSERT_THROW(zserio::detail::read(reader, readData), zserio::CppRuntimeException);
}

TEST_F(DynamicBitFieldLengthBoundsTest, signedBitLengthZero)
{
    Container data{
            UNSIGNED_BIT_LENGTH, UNSIGNED_VALUE, UNSIGNED_BIG_BIT_LENGTH, UNSIGNED_BIG_VALUE, 0, SIGNED_VALUE};

    ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);

    zserio::View view(data);
    zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
    BitBufferType bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    writeData(writer, data);

    zserio::BitStreamReader reader(bitBuffer);
    Container readData;
    ASSERT_THROW(zserio::detail::read(reader, readData), zserio::CppRuntimeException);
}

TEST_F(DynamicBitFieldLengthBoundsTest, signedBitLengthZeroValueZero)
{
    Container data{UNSIGNED_BIT_LENGTH, UNSIGNED_VALUE, UNSIGNED_BIG_BIT_LENGTH, UNSIGNED_BIG_VALUE, 0, 0};

    ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);

    zserio::View view(data);
    zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
    BitBufferType bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    writeData(writer, data);

    zserio::BitStreamReader reader(bitBuffer);
    Container readData;
    ASSERT_THROW(zserio::detail::read(reader, readData), zserio::CppRuntimeException);
}

TEST_F(DynamicBitFieldLengthBoundsTest, signedBitLengthOverMax)
{
    Container data{
            UNSIGNED_BIT_LENGTH, UNSIGNED_VALUE, UNSIGNED_BIG_BIT_LENGTH, UNSIGNED_BIG_VALUE, 65, SIGNED_VALUE};

    ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);

    zserio::View view(data);
    zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
    BitBufferType bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    writeData(writer, data);

    zserio::BitStreamReader reader(bitBuffer);
    Container readData;
    ASSERT_THROW(zserio::detail::read(reader, readData), zserio::CppRuntimeException);
}

} // namespace dynamic_bitfield_length_bounds
} // namespace builtin_types
