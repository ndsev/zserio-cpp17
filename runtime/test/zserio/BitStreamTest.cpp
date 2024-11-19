#include <array>
#include <cstring>
#include <functional>
#include <string>

#include "gtest/gtest.h"
#include "zserio/BitBuffer.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/Types.h"
#include "zserio/Vector.h"

namespace zserio
{

class BitStreamTest : public ::testing::Test
{
public:
    BitStreamTest() :
            m_byteBuffer(),
            m_externalWriter(m_byteBuffer.data(), m_byteBuffer.size()),
            m_dummyWriter(nullptr, 0)
    {
        m_byteBuffer.fill(0);
    }

protected:
    template <typename T, size_t N, typename U>
    void testImpl(const std::array<T, N>& values, std::function<void(BitStreamWriter&, U)> writerFunc,
            std::function<void(BitStreamReader&, T&)> readerFunc, uint8_t maxStartBitPos)
    {
        testBitStreamValues(values, m_externalWriter, writerFunc, readerFunc, maxStartBitPos);
        testBitStreamValues(values, m_dummyWriter, writerFunc, readerFunc, maxStartBitPos);
    }

    template <typename T, size_t N, typename U>
    void testBitStreamValues(const std::array<T, N>& values, BitStreamWriter& writer,
            std::function<void(BitStreamWriter&, U)> writerFunc,
            std::function<void(BitStreamReader&, T&)> readerFunc, uint8_t maxStartBitPos)
    {
        for (uint8_t bitPos = 0; bitPos < maxStartBitPos; ++bitPos)
        {
            if (bitPos > 0)
            {
                writer.writeUnsignedBits64(0, bitPos);
            }
            for (size_t i = 0; i < N; ++i)
            {
                writerFunc(writer, values.at(i));
            }

            if (!writer.hasWriteBuffer())
            {
                continue;
            }

            BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), BitsTag());
            if (bitPos > 0)
            {
                reader.readUnsignedBits64(bitPos);
            }
            for (size_t i = 0; i < N; ++i)
            {
                T readValue = T();
                readerFunc(reader, readValue);
                ASSERT_EQ(readValue, values.at(i)) << "[bitPos=" << bitPos << "]";
            }

            writer.setBitPosition(0);
            m_byteBuffer.fill(0);
        }
    }

    void testReadUnsignedBits32(BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(1, 1);
        writer.writeUnsignedBits32(2, 2);
        writer.writeUnsignedBits32(42, 12);
        writer.writeUnsignedBits32(15999999, 24);
        writer.writeUnsignedBits32(7, 3);

        if (!writer.hasWriteBuffer())
        {
            return;
        }

        BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), BitsTag());
        ASSERT_EQ(1, reader.readUnsignedBits32(1));
        ASSERT_EQ(2, reader.readUnsignedBits32(2));
        ASSERT_EQ(42, reader.readUnsignedBits32(12));
        ASSERT_EQ(15999999, reader.readUnsignedBits32(24));
        ASSERT_EQ(7, reader.readUnsignedBits32(3));
    }

    void testReadUnsignedBits64(BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(1, 1);
        writer.writeUnsignedBits64(UINT64_C(42424242424242), 48);
        writer.writeUnsignedBits64(UINT64_C(0xFFFFFFFFFFFFFFFE), 64);

        if (!writer.hasWriteBuffer())
        {
            return;
        }

        BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), BitsTag());
        ASSERT_EQ(1, reader.readUnsignedBits32(1));
        ASSERT_EQ(UINT64_C(42424242424242), reader.readUnsignedBits64(48));
        ASSERT_EQ(UINT64_C(0xFFFFFFFFFFFFFFFE), reader.readUnsignedBits64(64));
    }

    void testReadSignedBits32(BitStreamWriter& writer)
    {
        writer.writeSignedBits32(-1, 5);
        writer.writeSignedBits32(3, 12);
        writer.writeSignedBits32(-142, 9);

        if (!writer.hasWriteBuffer())
        {
            return;
        }

        BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), BitsTag());
        ASSERT_EQ(-1, reader.readSignedBits32(5));
        ASSERT_EQ(3, reader.readSignedBits32(12));
        ASSERT_EQ(-142, reader.readSignedBits32(9));
    }

    void testReadSignedBits64(BitStreamWriter& writer)
    {
        writer.writeSignedBits64(INT64_C(1), 4);
        writer.writeSignedBits64(INT64_C(-1), 48);
        writer.writeSignedBits64(INT64_C(-42424242), 61);
        writer.writeSignedBits64(INT64_C(-820816), 32);

        if (!writer.hasWriteBuffer())
        {
            return;
        }

        BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), BitsTag());
        ASSERT_EQ(INT64_C(1), reader.readSignedBits32(4));
        ASSERT_EQ(INT64_C(-1), reader.readSignedBits64(48));
        ASSERT_EQ(INT64_C(-42424242), reader.readSignedBits64(61));
        ASSERT_EQ(INT64_C(-820816), reader.readSignedBits64(32));
    }

    void testAlignedBytes(BitStreamWriter& writer)
    {
        // reads aligned data directly from buffer, bit cache should remain empty
        writer.writeUnsignedBits32(UINT8_C(0xCA), 8);
        writer.writeUnsignedBits32(UINT16_C(0xCAFE), 16);
        writer.writeUnsignedBits32(UINT32_C(0xCAFEC0), 24);
        writer.writeUnsignedBits32(UINT32_C(0xCAFEC0DE), 32);
        writer.writeUnsignedBits64(UINT64_C(0xCAFEC0DEDE), 40);
        writer.writeUnsignedBits64(UINT64_C(0xCAFEC0DEDEAD), 48);
        writer.writeUnsignedBits64(UINT64_C(0xCAFEC0DEDEADFA), 56);
        writer.writeUnsignedBits64(UINT64_C(0xCAFEC0DEDEADFACE), 64);

        if (!writer.hasWriteBuffer())
        {
            return;
        }

        BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), BitsTag());
        ASSERT_EQ(UINT8_C(0xCA), reader.readUnsignedBits32(8));
        ASSERT_EQ(UINT16_C(0xCAFE), reader.readUnsignedBits32(16));
        ASSERT_EQ(UINT32_C(0xCAFEC0), reader.readUnsignedBits32(24));
        ASSERT_EQ(UINT32_C(0xCAFEC0DE), reader.readUnsignedBits32(32));
        ASSERT_EQ(UINT64_C(0xCAFEC0DEDE), reader.readUnsignedBits64(40));
        ASSERT_EQ(UINT64_C(0xCAFEC0DEDEAD), reader.readUnsignedBits64(48));
        ASSERT_EQ(UINT64_C(0xCAFEC0DEDEADFA), reader.readUnsignedBits64(56));
        ASSERT_EQ(UINT64_C(0xCAFEC0DEDEADFACE), reader.readUnsignedBits64(64));
    }

    void testSetBitPosition(BitStreamWriter& writer)
    {
        ASSERT_EQ(0, writer.getBitPosition());
        writer.writeUnsignedBits32(1, 1);
        ASSERT_EQ(1, writer.getBitPosition());
        writer.alignTo(4);
        ASSERT_EQ(4, writer.getBitPosition());
        writer.writeUnsignedBits32(5, 5);
        ASSERT_EQ(9, writer.getBitPosition());
        if (writer.hasWriteBuffer())
        {
            ASSERT_THROW(writer.setBitPosition(m_byteBuffer.size() * 8 + 1), CppRuntimeException);
        }
        else
        {
            // dummy buffer
            writer.setBitPosition(m_byteBuffer.size() * 8 + 1);
            ASSERT_EQ(m_byteBuffer.size() * 8 + 1, writer.getBitPosition());
        }
        writer.setBitPosition(4);
        ASSERT_EQ(4, writer.getBitPosition());
        writer.writeUnsignedBits32(3, 3);
        ASSERT_EQ(7, writer.getBitPosition());

        if (!writer.hasWriteBuffer())
        {
            return;
        }

        BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), BitsTag());
        ASSERT_EQ(0, reader.getBitPosition());
        ASSERT_EQ(1, reader.readUnsignedBits32(1));
        ASSERT_EQ(1, reader.getBitPosition());
        reader.alignTo(4);
        ASSERT_EQ(4, reader.getBitPosition());
        ASSERT_EQ(3, reader.readUnsignedBits32(3));
        ASSERT_EQ(7, reader.getBitPosition());
        ASSERT_THROW(reader.setBitPosition(writer.getBitPosition() + 1), CppRuntimeException);

        reader.setBitPosition(4);
        ASSERT_EQ(4, reader.getBitPosition());
        ASSERT_EQ(3, reader.readUnsignedBits32(3));
        ASSERT_EQ(7, reader.getBitPosition());
    }

    void testAlignTo(BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(1, 1);
        writer.alignTo(4);
        ASSERT_EQ(4, writer.getBitPosition());
        writer.writeUnsignedBits32(1, 1);
        writer.alignTo(4);
        ASSERT_EQ(8, writer.getBitPosition());
        writer.writeUnsignedBits32(37, 11);
        writer.alignTo(8);
        ASSERT_EQ(24, writer.getBitPosition());
        writer.writeUnsignedBits32(1, 1);
        writer.alignTo(16);
        ASSERT_EQ(32, writer.getBitPosition());
        writer.writeUnsignedBits32(13, 13);
        writer.alignTo(32);
        ASSERT_EQ(64, writer.getBitPosition());
        writer.writeUnsignedBits32(42, 15);
        writer.alignTo(64);
        ASSERT_EQ(128, writer.getBitPosition());
        writer.writeUnsignedBits32(99, 9);
        ASSERT_EQ(137, writer.getBitPosition());

        if (!writer.hasWriteBuffer())
        {
            return;
        }

        BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), BitsTag());
        ASSERT_EQ(1, reader.readUnsignedBits32(1));
        reader.alignTo(4);
        ASSERT_EQ(1, reader.readUnsignedBits32(1));
        reader.alignTo(4);
        ASSERT_EQ(37, reader.readUnsignedBits32(11));
        reader.alignTo(8);
        ASSERT_EQ(1, reader.readUnsignedBits32(1));
        reader.alignTo(16);
        ASSERT_EQ(13, reader.readUnsignedBits32(13));
        reader.alignTo(32);
        ASSERT_EQ(42, reader.readUnsignedBits32(15));
        reader.alignTo(64);
        ASSERT_EQ(99, reader.readUnsignedBits32(9));
        ASSERT_EQ(137, reader.getBitPosition());
    }

    std::array<uint8_t, 256> m_byteBuffer;
    BitStreamWriter m_externalWriter;
    BitStreamWriter m_dummyWriter;
};

TEST_F(BitStreamTest, readUnsignedBits32)
{
    testReadUnsignedBits32(m_externalWriter);
    testReadUnsignedBits32(m_dummyWriter);
}

TEST_F(BitStreamTest, readUnsignedBits64)
{
    testReadUnsignedBits64(m_externalWriter);
    testReadUnsignedBits64(m_dummyWriter);
}

TEST_F(BitStreamTest, readSignedBits32)
{
    testReadSignedBits32(m_externalWriter);
    testReadSignedBits32(m_dummyWriter);
}

TEST_F(BitStreamTest, readSignedBits64)
{
    testReadSignedBits64(m_externalWriter);
    testReadSignedBits64(m_dummyWriter);
}

TEST_F(BitStreamTest, alignedBytes)
{
    testAlignedBytes(m_externalWriter);
    testAlignedBytes(m_dummyWriter);
}

TEST_F(BitStreamTest, readBool)
{
    const std::array<Bool, 2> values = {true, false};

    std::function<void(BitStreamWriter&, Bool)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, Bool)>(&detail::write);
    std::function<void(BitStreamReader&, Bool&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, Bool&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 1);
}

TEST_F(BitStreamTest, readInt1)
{
    const std::array<Int1, 2> values = {NumericLimits<Int1>::min(), NumericLimits<Int1>::max()};

    std::function<void(BitStreamWriter&, Int1)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, Int1)>(&detail::write);
    std::function<void(BitStreamReader&, Int1&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, Int1&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 1);
}

TEST_F(BitStreamTest, readInt7)
{
    const std::array<Int7, 3> values = {NumericLimits<Int7>::min(), 0, NumericLimits<Int7>::max()};

    std::function<void(BitStreamWriter&, Int7)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, Int7)>(&detail::write);
    std::function<void(BitStreamReader&, Int7&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, Int7&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 6);
}

TEST_F(BitStreamTest, readInt32)
{
    const std::array<Int32, 3> values = {NumericLimits<Int32>::min(), 0, NumericLimits<Int32>::max()};

    std::function<void(BitStreamWriter&, Int32)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, Int32)>(&detail::write);
    std::function<void(BitStreamReader&, Int32&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, Int32&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 31);
}

TEST_F(BitStreamTest, readInt33)
{
    const std::array<Int33, 3> values = {NumericLimits<Int33>::min(), 0, NumericLimits<Int33>::max()};

    std::function<void(BitStreamWriter&, Int33)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, Int33)>(&detail::write);
    std::function<void(BitStreamReader&, Int33&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, Int33&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 32);
}

TEST_F(BitStreamTest, readInt64)
{
    const std::array<Int64, 3> values = {NumericLimits<Int64>::min(), 0, NumericLimits<Int64>::max()};

    std::function<void(BitStreamWriter&, Int64)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, Int64)>(&detail::write);
    std::function<void(BitStreamReader&, Int64&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, Int64&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 32);
}

TEST_F(BitStreamTest, readUInt1)
{
    const std::array<UInt1, 2> values = {NumericLimits<UInt1>::min(), NumericLimits<UInt1>::max()};

    std::function<void(BitStreamWriter&, UInt1)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, UInt1)>(&detail::write);
    std::function<void(BitStreamReader&, UInt1&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, UInt1&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 1);
}

TEST_F(BitStreamTest, readUInt7)
{
    const std::array<UInt7, 2> values = {NumericLimits<UInt7>::min(), NumericLimits<UInt7>::max()};

    std::function<void(BitStreamWriter&, UInt7)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, UInt7)>(&detail::write);
    std::function<void(BitStreamReader&, UInt7&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, UInt7&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 6);
}

TEST_F(BitStreamTest, readUInt32)
{
    const std::array<UInt32, 2> values = {NumericLimits<UInt32>::min(), NumericLimits<UInt32>::max()};

    std::function<void(BitStreamWriter&, UInt32)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, UInt32)>(&detail::write);
    std::function<void(BitStreamReader&, UInt32&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, UInt32&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 31);
}

TEST_F(BitStreamTest, readUInt33)
{
    const std::array<UInt33, 2> values = {NumericLimits<UInt33>::min(), NumericLimits<UInt33>::max()};

    std::function<void(BitStreamWriter&, UInt33)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, UInt33)>(&detail::write);
    std::function<void(BitStreamReader&, UInt33&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, UInt33&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 32);
}

TEST_F(BitStreamTest, readUInt64)
{
    const std::array<UInt64, 2> values = {NumericLimits<UInt64>::min(), NumericLimits<UInt64>::max()};

    std::function<void(BitStreamWriter&, UInt64)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, UInt64)>(&detail::write);
    std::function<void(BitStreamReader&, UInt64&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, UInt64&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 32);
}

TEST_F(BitStreamTest, readVarInt16)
{
    const std::array<VarInt16, 9> values = {
            static_cast<int16_t>(0),
            static_cast<int16_t>(-32),
            static_cast<int16_t>(32),
            static_cast<int16_t>(-4096),
            static_cast<int16_t>(4096),

            static_cast<int16_t>(1U << (0U)),
            static_cast<int16_t>(1U << (6U)) - 1,

            static_cast<int16_t>(1U << (6U)),
            static_cast<int16_t>(1U << (6 + 8U)) - 1,
    };

    std::function<void(BitStreamWriter&, VarInt16)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, VarInt16)>(&detail::write);
    std::function<void(BitStreamReader&, VarInt16&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, VarInt16&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 15);
}

TEST_F(BitStreamTest, readVarInt32)
{
    const std::array<VarInt32, 17> values = {
            static_cast<int32_t>(0),
            static_cast<int32_t>(-32),
            static_cast<int32_t>(32),
            static_cast<int32_t>(-4096),
            static_cast<int32_t>(4096),
            static_cast<int32_t>(-524288),
            static_cast<int32_t>(524288),
            static_cast<int32_t>(-67108864),
            static_cast<int32_t>(67108864),

            static_cast<int32_t>(1U << (0U)),
            static_cast<int32_t>(1U << (6U)) - 1,

            static_cast<int32_t>(1U << (6U)),
            static_cast<int32_t>(1U << (6U + 7)) - 1,

            static_cast<int32_t>(1U << (6U + 7)),
            static_cast<int32_t>(1U << (6U + 7 + 7)) - 1,

            static_cast<int32_t>(1U << (6U + 7 + 7)),
            static_cast<int32_t>(1U << (6U + 7 + 7 + 8)) - 1,
    };

    std::function<void(BitStreamWriter&, VarInt32)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, VarInt32)>(&detail::write);
    std::function<void(BitStreamReader&, VarInt32&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, VarInt32&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 31);
}

TEST_F(BitStreamTest, readVarInt64)
{
    const std::array<VarInt64, 33> values = {
            INT64_C(0),
            INT64_C(-32),
            INT64_C(32),
            INT64_C(-4096),
            INT64_C(4096),
            INT64_C(-524288),
            INT64_C(524288),
            INT64_C(-67108864),
            INT64_C(67108864),
            INT64_C(-8589934592),
            INT64_C(8589934592),
            INT64_C(-1099511627776),
            INT64_C(1099511627776),
            INT64_C(-140737488355328),
            INT64_C(140737488355328),
            INT64_C(-18014398509481984),
            INT64_C(18014398509481984),

            (INT64_C(1) << (0)),
            (INT64_C(1) << (6)) - 1,

            (INT64_C(1) << (6)),
            (INT64_C(1) << (6 + 7)) - 1,

            (INT64_C(1) << (6 + 7)),
            (INT64_C(1) << (6 + 7 + 7)) - 1,

            (INT64_C(1) << (6 + 7 + 7)),
            (INT64_C(1) << (6 + 7 + 7 + 7)) - 1,

            (INT64_C(1) << (6 + 7 + 7 + 7)),
            (INT64_C(1) << (6 + 7 + 7 + 7 + 7)) - 1,

            (INT64_C(1) << (6 + 7 + 7 + 7 + 7)),
            (INT64_C(1) << (6 + 7 + 7 + 7 + 7 + 7)) - 1,

            (INT64_C(1) << (6 + 7 + 7 + 7 + 7 + 7)),
            (INT64_C(1) << (6 + 7 + 7 + 7 + 7 + 7 + 7)) - 1,

            (INT64_C(1) << (6 + 7 + 7 + 7 + 7 + 7 + 7)),
            (INT64_C(1) << (6 + 7 + 7 + 7 + 7 + 7 + 7 + 8)) - 1,
    };

    std::function<void(BitStreamWriter&, VarInt64)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, VarInt64)>(&detail::write);
    std::function<void(BitStreamReader&, VarInt64&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, VarInt64&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 63);
}

TEST_F(BitStreamTest, readVarInt)
{
    const std::array<VarInt, 38> values = {
            // 1 byte
            0,
            -1,
            1,
            -static_cast<int64_t>(UINT64_C(1) << 6U) + 1,
            static_cast<int64_t>(UINT64_C(1) << 6U) - 1,
            // 2 bytes
            -static_cast<int64_t>(UINT64_C(1) << 6U),
            static_cast<int64_t>(UINT64_C(1) << 6U),
            -static_cast<int64_t>(UINT64_C(1) << 13U) + 1,
            static_cast<int64_t>(UINT64_C(1) << 13U) - 1,
            // 3 bytes
            -static_cast<int64_t>(UINT64_C(1) << 13U),
            static_cast<int64_t>(UINT64_C(1) << 13U),
            -static_cast<int64_t>(UINT64_C(1) << 20U) + 1,
            static_cast<int64_t>(UINT64_C(1) << 20U) - 1,
            // 4 bytes
            -static_cast<int64_t>(UINT64_C(1) << 20U),
            static_cast<int64_t>(UINT64_C(1) << 20U),
            -static_cast<int64_t>(UINT64_C(1) << 27U) + 1,
            static_cast<int64_t>(UINT64_C(1) << 27U) - 1,
            // 5 bytes
            -static_cast<int64_t>(UINT64_C(1) << 27U),
            static_cast<int64_t>(UINT64_C(1) << 27U),
            -static_cast<int64_t>(UINT64_C(1) << 34U) + 1,
            static_cast<int64_t>(UINT64_C(1) << 34U) - 1,
            // 6 bytes
            -static_cast<int64_t>(UINT64_C(1) << 34U),
            static_cast<int64_t>(UINT64_C(1) << 34U),
            -static_cast<int64_t>(UINT64_C(1) << 41U) + 1,
            static_cast<int64_t>(UINT64_C(1) << 41U) - 1,
            // 7 bytes
            -static_cast<int64_t>(UINT64_C(1) << 41U),
            static_cast<int64_t>(UINT64_C(1) << 41U),
            -static_cast<int64_t>(UINT64_C(1) << 48U) + 1,
            static_cast<int64_t>(UINT64_C(1) << 48U) - 1,
            // 8 bytes
            -static_cast<int64_t>(UINT64_C(1) << 48U),
            static_cast<int64_t>(UINT64_C(1) << 48U),
            -static_cast<int64_t>(UINT64_C(1) << 55U) + 1,
            static_cast<int64_t>(UINT64_C(1) << 55U) - 1,
            // 9 bytes
            -static_cast<int64_t>(UINT64_C(1) << 55U),
            static_cast<int64_t>(UINT64_C(1) << 55U),
            INT64_MIN + 1,
            INT64_MAX,

            // special case - stored as -0 (1 byte)
            INT64_MIN,
    };

    std::function<void(BitStreamWriter&, VarInt)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, VarInt)>(&detail::write);
    std::function<void(BitStreamReader&, VarInt&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, VarInt&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 63);
}

TEST_F(BitStreamTest, readVarUInt16)
{
    const std::array<VarUInt16, 7> values = {
            0,
            8192,
            16384,

            (1U << (0U)),
            (1U << (6U)) - 1,

            (1U << (6U)),
            (1U << (6U + 8)) - 1,
    };

    std::function<void(BitStreamWriter&, VarUInt16)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, VarUInt16)>(&detail::write);
    std::function<void(BitStreamReader&, VarUInt16&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, VarUInt16&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 15);
}

TEST_F(BitStreamTest, readVarUInt32)
{
    const std::array<VarUInt32, 11> values = {
            0,
            65536,
            131072,

            (1U << (0U)),
            (1U << (7U)) - 1,

            (1U << (7U)),
            (1U << (7U + 7)) - 1,

            (1U << (7U + 7)),
            (1U << (7U + 7 + 7)) - 1,

            (1U << (7U + 7 + 7)),
            (1U << (7U + 7 + 7 + 8)) - 1,
    };

    std::function<void(BitStreamWriter&, VarUInt32)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, VarUInt32)>(&detail::write);
    std::function<void(BitStreamReader&, VarUInt32&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, VarUInt32&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 31);
}

TEST_F(BitStreamTest, readVarUInt64)
{
    const std::array<VarUInt64, 19> values = {
            0,
            262144,
            524288,

            (UINT64_C(1) << (0U)),
            (UINT64_C(1) << (7U)) - 1,

            (UINT64_C(1) << (7U)),
            (UINT64_C(1) << (7U + 7)) - 1,

            (UINT64_C(1) << (7U + 7)),
            (UINT64_C(1) << (7U + 7 + 7)) - 1,

            (UINT64_C(1) << (7U + 7 + 7)),
            (UINT64_C(1) << (7U + 7 + 7 + 7)) - 1,

            (UINT64_C(1) << (7U + 7 + 7 + 7)),
            (UINT64_C(1) << (7U + 7 + 7 + 7 + 7)) - 1,

            (UINT64_C(1) << (7U + 7 + 7 + 7 + 7)),
            (UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7)) - 1,

            (UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7)),
            (UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7 + 7)) - 1,

            (UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7 + 7)),
            (UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7 + 7 + 8)) - 1,
    };

    std::function<void(BitStreamWriter&, VarUInt64)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, VarUInt64)>(&detail::write);
    std::function<void(BitStreamReader&, VarUInt64&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, VarUInt64&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 63);
}

TEST_F(BitStreamTest, readVarUInt)
{
    const std::array<VarUInt, 19> values = {
            // 1 byte
            0,
            1,
            (UINT64_C(1) << 7U) - 1,
            // 2 bytes
            (UINT64_C(1) << 7U),
            (UINT64_C(1) << 14U) - 1,
            // 3 bytes
            (UINT64_C(1) << 14U),
            (UINT64_C(1) << 21U) - 1,
            // 4 bytes
            (UINT64_C(1) << 21U),
            (UINT64_C(1) << 28U) - 1,
            // 5 bytes
            (UINT64_C(1) << 28U),
            (UINT64_C(1) << 35U) - 1,
            // 6 bytes
            (UINT64_C(1) << 35U),
            (UINT64_C(1) << 42U) - 1,
            // 7 bytes
            (UINT64_C(1) << 42U),
            (UINT64_C(1) << 49U) - 1,
            // 8 bytes
            (UINT64_C(1) << 49U),
            (UINT64_C(1) << 56U) - 1,
            // 9 bytes
            (UINT64_C(1) << 56U),
            UINT64_MAX,
    };

    std::function<void(BitStreamWriter&, VarUInt)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, VarUInt)>(&detail::write);
    std::function<void(BitStreamReader&, VarUInt&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, VarUInt&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 63);
}

TEST_F(BitStreamTest, readVarSize)
{
    const std::array<VarSize, 13> values = {
            0,
            65536,
            131072,

            (1U << (0U)),
            (1U << (7U)) - 1,

            (1U << (7U)),
            (1U << (7U + 7)) - 1,

            (1U << (7U + 7)),
            (1U << (7U + 7 + 7)) - 1,

            (1U << (7U + 7 + 7)),
            (1U << (7U + 7 + 7 + 7)) - 1,

            (1U << (7U + 7 + 7 + 7)),
            (1U << (7U + 7 + 7 + 7 + 3)) - 1,
    };

    std::function<void(BitStreamWriter&, VarSize)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, VarSize)>(&detail::write);
    std::function<void(BitStreamReader&, VarSize&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, VarSize&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 31);
}

TEST_F(BitStreamTest, readFloat16)
{
    const std::array<Float16, 6> values = {2.0, -2.0, 0.6171875, 0.875, 9.875, 42.5};

    std::function<void(BitStreamWriter&, Float16)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, Float16)>(&detail::write);
    std::function<void(BitStreamReader&, Float16&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, Float16&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 15);
}

TEST_F(BitStreamTest, readFloat32)
{
    const std::array<Float32, 6> values = {2.0, -2.0, 0.6171875, 0.875, 9.875, 42.5};

    std::function<void(BitStreamWriter&, Float32)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, Float32)>(&detail::write);
    std::function<void(BitStreamReader&, Float32&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, Float32&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 31);
}

TEST_F(BitStreamTest, readFloat64)
{
    const std::array<Float64, 6> values = {2.0, -2.0, 0.6171875, 0.875, 9.875, 42.5};

    std::function<void(BitStreamWriter&, Float64)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, Float64)>(&detail::write);
    std::function<void(BitStreamReader&, Float64&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, Float64&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 61);
}

TEST_F(BitStreamTest, readString)
{
    const std::array<std::string, 3> values = {
            "Hello World", "\n\t%^@(*aAzZ01234569$%^!?<>[]](){}-=+~:;/|\\\"\'Hello World2\0nonWrittenPart",
            "Price: \xE2\x82\xAC 3 what's this? -> \xC2\xA2" /* '€' '¢' */
    };

    std::function<void(BitStreamWriter&, StringView)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, StringView)>(&detail::write);
    std::function<void(BitStreamReader&, std::string&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, std::string&)>(&detail::read);
    testImpl(values, writerFunc, readerFunc, 7);
}

TEST_F(BitStreamTest, readBitBuffer)
{
    const std::array<BitBuffer, 2> values = {BitBuffer(std::vector<uint8_t>({0xAB, 0xE0}), 11),
            BitBuffer(std::vector<uint8_t>({0xAB, 0xCD, 0xFE}), 23)};

    std::function<void(BitStreamWriter&, const BitBuffer&)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, const BitBufferView&)>(&detail::write);
    std::function<void(BitStreamReader&, BitBuffer&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, BitBuffer&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 7);
}

TEST_F(BitStreamTest, readBytes)
{
    const std::array<Vector<uint8_t>, 2> values = {
            Vector<uint8_t>{{0, 255}},
            Vector<uint8_t>{{1, 127, 128, 254}},
    };

    std::function<void(BitStreamWriter&, BytesView)> writerFunc =
            static_cast<void (*)(BitStreamWriter&, BytesView)>(&detail::write);
    std::function<void(BitStreamReader&, Vector<uint8_t>&)> readerFunc =
            static_cast<void (*)(BitStreamReader&, Bytes&)>(&detail::read);

    testImpl(values, writerFunc, readerFunc, 7);
}

TEST_F(BitStreamTest, setBitPosition)
{
    testSetBitPosition(m_externalWriter);
    testSetBitPosition(m_dummyWriter);
}

TEST_F(BitStreamTest, alignTo)
{
    testAlignTo(m_externalWriter);
    testAlignTo(m_dummyWriter);
}

} // namespace zserio
