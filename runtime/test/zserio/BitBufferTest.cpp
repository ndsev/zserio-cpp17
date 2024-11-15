#include <array>
#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "zserio/BitBuffer.h"
#include "zserio/CppRuntimeException.h"

#include "TrackingAllocator.h"

namespace zserio
{

TEST(BitBufferTest, emptyConstructor)
{
    const BitBuffer bitBuffer;
    ASSERT_EQ(0, bitBuffer.getBitSize());
    ASSERT_EQ(0, bitBuffer.getByteSize());
}

TEST(BitBufferTest, allocatorConstructor)
{
    const std::allocator<uint8_t> allocator;
    const BitBuffer bitBuffer(allocator);
    ASSERT_EQ(0, bitBuffer.getBitSize());
    ASSERT_EQ(0, bitBuffer.getByteSize());
}

TEST(BitBufferTest, bitSizeConstructor)
{
    const size_t bitSize = 11;
    const BitBuffer bitBuffer(bitSize);
    ASSERT_EQ(bitSize, bitBuffer.getBitSize());
}

TEST(BitBufferTest, spanConstructor)
{
    // vector
    std::vector<uint8_t> buffer1(2);
    const BitBuffer bitBuffer1(buffer1);
    ASSERT_EQ(8 * buffer1.size(), bitBuffer1.getBitSize());

    const size_t emptyBitSize = 0;
    const std::vector<uint8_t> emptyBuffer;
    const BitBuffer emptyBitBuffer(emptyBuffer);
    ASSERT_EQ(emptyBitSize, emptyBitBuffer.getBitSize());

    // const vector
    const std::vector<uint8_t> buffer2(2);
    const BitBuffer bitBuffer2(buffer2);
    ASSERT_EQ(8 * buffer2.size(), bitBuffer2.getBitSize());

    // array
    std::array<uint8_t, 2> buffer3{};
    const BitBuffer bitBuffer3(buffer3);
    ASSERT_EQ(8 * buffer3.size(), bitBuffer3.getBitSize());

    // const array
    const std::array<uint8_t, 2> buffer4{};
    const BitBuffer bitBuffer4(buffer4);
    ASSERT_EQ(8 * buffer4.size(), bitBuffer4.getBitSize());

    // span
    std::array<uint8_t, 2> arrayBuffer5{};
    Span<uint8_t> buffer5(arrayBuffer5);
    const BitBuffer bitBuffer5(buffer5);
    ASSERT_EQ(8 * buffer5.size(), bitBuffer5.getBitSize());

    // const span
    const std::array<uint8_t, 2> arrayBuffer6{};
    Span<const uint8_t> buffer6(arrayBuffer6);
    const BitBuffer bitBuffer6(buffer6);
    ASSERT_EQ(8 * buffer6.size(), bitBuffer6.getBitSize());
}

TEST(BitBufferTest, spanBitSizeConstructor)
{
    // vector
    const size_t bitSize = 11;
    std::vector<uint8_t> buffer1((bitSize + 7) / 8);
    const BitBuffer bitBuffer1(buffer1, bitSize);
    ASSERT_EQ(bitSize, bitBuffer1.getBitSize());

    const size_t emptyBitSize = 0;
    const std::vector<uint8_t> emptyBuffer;
    const BitBuffer emptyBitBuffer(emptyBuffer, emptyBitSize);
    ASSERT_EQ(emptyBitSize, emptyBitBuffer.getBitSize());

    const size_t outOfRangeBitSize = 9;
    const std::vector<uint8_t> outOfRangeBuffer(1);
    ASSERT_THROW(BitBuffer(outOfRangeBuffer, outOfRangeBitSize), CppRuntimeException);

    // const vector
    const std::vector<uint8_t> buffer2((bitSize + 7) / 8);
    const BitBuffer bitBuffer2(buffer2, bitSize);
    ASSERT_EQ(bitSize, bitBuffer2.getBitSize());

    // array
    std::array<uint8_t, 2> buffer3{};
    const BitBuffer bitBuffer3(buffer3, bitSize);
    ASSERT_EQ(bitSize, bitBuffer3.getBitSize());

    // const array
    const std::array<uint8_t, 2> buffer4{};
    const BitBuffer bitBuffer4(buffer4, bitSize);
    ASSERT_EQ(bitSize, bitBuffer4.getBitSize());

    // span
    std::array<uint8_t, 2> arrayBuffer5{};
    Span<uint8_t> buffer5(arrayBuffer5);
    const BitBuffer bitBuffer5(buffer5, bitSize);
    ASSERT_EQ(bitSize, bitBuffer5.getBitSize());

    // const span
    const std::array<uint8_t, 2> arrayBuffer6{};
    Span<const uint8_t> buffer6(arrayBuffer6);
    const BitBuffer bitBuffer6(buffer6, bitSize);
    ASSERT_EQ(bitSize, bitBuffer6.getBitSize());
}

TEST(BitBufferTest, vectorMoveConstructor)
{
    const size_t byteSize = 2;
    std::vector<uint8_t> buffer(byteSize);
    const uint8_t* bufferStart = buffer.data();
    const BitBuffer bitBuffer(std::move(buffer));
    ASSERT_EQ(8 * byteSize, bitBuffer.getBitSize());
    ASSERT_EQ(bufferStart, bitBuffer.getBuffer());
}

TEST(BitBufferTest, vectorBitSizeMoveConstructor)
{
    const size_t bitSize = 11;
    std::vector<uint8_t> buffer((bitSize + 7) / 8);
    const uint8_t* bufferStart = buffer.data();
    const BitBuffer bitBuffer(std::move(buffer), bitSize);
    ASSERT_EQ(bitSize, bitBuffer.getBitSize());
    ASSERT_EQ(bufferStart, bitBuffer.getBuffer());

    const size_t outOfRangeBitSize = 9;
    ASSERT_THROW(BitBuffer(std::vector<uint8_t>(1), outOfRangeBitSize), CppRuntimeException);
}

TEST(BitBufferTest, copyConstructor)
{
    const size_t bitSize = 11;
    const std::vector<uint8_t> buffer = {0xAB, 0xE0};
    const BitBuffer bitBuffer(buffer, bitSize);

    const BitBuffer copiedBitBuffer(bitBuffer);
    ASSERT_EQ(bitBuffer.getBitSize(), copiedBitBuffer.getBitSize());
    ASSERT_EQ(bitBuffer.getByteSize(), copiedBitBuffer.getByteSize());
    const Span<const uint8_t> copiedBuffer = copiedBitBuffer.getData();
    size_t index = 0;
    for (uint8_t element : buffer)
    {
        ASSERT_EQ(element, copiedBuffer[index]);
        ++index;
    }
}

TEST(BitBufferTest, copyConstructorWithAllocator)
{
    const TrackingAllocator<uint8_t> allocator;
    const size_t bitSize = 11;
    const std::array<uint8_t, 2> data = {0xAB, 0xE0};
    Span<const uint8_t> buffer(data);
    const BasicBitBuffer<TrackingAllocator<uint8_t>> bitBuffer(buffer, bitSize, allocator);
    const size_t numAllocations = allocator.numAllocs();

    const TrackingAllocator<uint8_t> newAllocator;
    const BasicBitBuffer<TrackingAllocator<uint8_t>> copiedBitBuffer(bitBuffer, newAllocator);
    ASSERT_EQ(bitBuffer.getBitSize(), copiedBitBuffer.getBitSize());
    ASSERT_EQ(bitBuffer.getByteSize(), copiedBitBuffer.getByteSize());
    const Span<const uint8_t> copiedBuffer = copiedBitBuffer.getData();
    size_t index = 0;
    for (uint8_t element : buffer)
    {
        ASSERT_EQ(element, copiedBuffer[index]);
        ++index;
    }
    ASSERT_EQ(numAllocations, allocator.numAllocs());
    ASSERT_NE(0, newAllocator.numAllocs());
}

TEST(BitBufferTest, assignmentOperator)
{
    const size_t bitSize = 11;
    const std::vector<uint8_t> buffer = {0xAB, 0xE0};
    const BitBuffer bitBuffer(buffer, bitSize);

    BitBuffer copiedBitBuffer;
    copiedBitBuffer = bitBuffer;
    ASSERT_EQ(bitBuffer.getBitSize(), copiedBitBuffer.getBitSize());
    ASSERT_EQ(bitBuffer.getByteSize(), copiedBitBuffer.getByteSize());
    const Span<const uint8_t> copiedBuffer = copiedBitBuffer.getData();
    size_t index = 0;
    for (uint8_t element : buffer)
    {
        ASSERT_EQ(element, copiedBuffer[index]);
        ++index;
    }

    BitBuffer anotherBitBuffer;
    const void* origDataAddress = &copiedBitBuffer.getData()[0];
    anotherBitBuffer = std::move(copiedBitBuffer);
    ASSERT_EQ(bitBuffer.getBitSize(), anotherBitBuffer.getBitSize());
    ASSERT_EQ(bitBuffer.getByteSize(), anotherBitBuffer.getByteSize());
    ASSERT_EQ(origDataAddress, &anotherBitBuffer.getData()[0]);
}

TEST(BitBufferTest, moveConstructor)
{
    const size_t bitSize = 11;
    const std::vector<uint8_t> buffer = {0xAB, 0xE0};
    const size_t byteSize = buffer.size();
    BitBuffer bitBuffer(buffer, bitSize);
    const uint8_t* bufferStart = &bitBuffer.getData()[0];

    const BitBuffer movedBitBuffer(std::move(bitBuffer));
    ASSERT_EQ(bitSize, movedBitBuffer.getBitSize());
    ASSERT_EQ(byteSize, movedBitBuffer.getByteSize());
    ASSERT_EQ(bufferStart, movedBitBuffer.getBuffer());
}

TEST(BitBufferTest, moveConstructorWithAllocator)
{
    const TrackingAllocator<uint8_t> allocator;
    const size_t bitSize = 11;
    const std::array<uint8_t, 2> data = {0xAB, 0xE0};
    Span<const uint8_t> buffer(data);
    const size_t byteSize = buffer.size();
    BasicBitBuffer<TrackingAllocator<uint8_t>> bitBuffer(buffer, bitSize, allocator);
    const size_t numAllocations = allocator.numAllocs();

    const TrackingAllocator<uint8_t> newAllocator;
    const BasicBitBuffer<TrackingAllocator<uint8_t>> movedBitBuffer(std::move(bitBuffer), newAllocator);
    ASSERT_EQ(bitSize, movedBitBuffer.getBitSize());
    ASSERT_EQ(byteSize, movedBitBuffer.getByteSize());
    ASSERT_EQ(numAllocations, allocator.numAllocs());
    ASSERT_NE(0, newAllocator.numAllocs());
}

TEST(BitBufferTest, moveAssignmentOperator)
{
    const size_t bitSize = 11;
    const std::vector<uint8_t> buffer = {0xAB, 0xE0};
    const size_t byteSize = buffer.size();
    BitBuffer bitBuffer(buffer, bitSize);
    const uint8_t* bufferStart = bitBuffer.getBuffer();

    const BitBuffer movedBitBuffer = std::move(bitBuffer);
    ASSERT_EQ(bitSize, movedBitBuffer.getBitSize());
    ASSERT_EQ(byteSize, movedBitBuffer.getByteSize());
    ASSERT_EQ(bufferStart, movedBitBuffer.getBuffer());
}

TEST(BitBufferTest, operatorEquality)
{
    const size_t bitSize = 11;
    const BitBuffer bitBuffer1(std::vector<uint8_t>({0xAB, 0xE0}), bitSize);
    const BitBuffer bitBuffer2(std::vector<uint8_t>({0xAB, 0xF0}), bitSize);
    ASSERT_TRUE(bitBuffer1 == bitBuffer2);

    const BitBuffer bitBuffer3(std::vector<uint8_t>({0xAB, 0xFF}), bitSize);
    ASSERT_TRUE(bitBuffer1 == bitBuffer3);

    const BitBuffer bitBuffer4(std::vector<uint8_t>({0xAB, 0xC0}), bitSize);
    ASSERT_FALSE(bitBuffer1 == bitBuffer4);

    const BitBuffer bitBuffer5(std::vector<uint8_t>({0xBA, 0xE0}), bitSize);
    ASSERT_FALSE(bitBuffer1 == bitBuffer5);

    const BitBuffer bitBuffer6(std::vector<uint8_t>({0xAB}));
    ASSERT_FALSE(bitBuffer1 == bitBuffer6);

    const BitBuffer bitBuffer7;
    ASSERT_FALSE(bitBuffer1 == bitBuffer7);
}

TEST(BitBufferTest, operatorLessThan)
{
    const BitBuffer bitBufferEmpty1;
    const BitBuffer bitBufferEmpty2;
    ASSERT_FALSE(bitBufferEmpty1 < bitBufferEmpty2);
    ASSERT_FALSE(bitBufferEmpty2 < bitBufferEmpty1);

    const BitBuffer bitBufferByte1(std::vector<uint8_t>({0xAB}), 8);
    const BitBuffer bitBufferByte2(std::vector<uint8_t>({0xAC}), 8);
    ASSERT_TRUE(bitBufferByte1 < bitBufferByte2);
    ASSERT_FALSE(bitBufferByte2 < bitBufferByte1);

    const BitBuffer bitBuffer1(std::vector<uint8_t>({0xAB, 0xE0}), 11);
    ASSERT_TRUE(bitBufferEmpty1 < bitBuffer1);
    ASSERT_TRUE(bitBufferByte1 < bitBuffer1);
    ASSERT_FALSE(bitBuffer1 < bitBufferEmpty1);
    ASSERT_FALSE(bitBuffer1 < bitBufferByte1);

    const BitBuffer bitBuffer1Copy(bitBuffer1);
    ASSERT_FALSE(bitBuffer1 < bitBuffer1Copy);
    ASSERT_FALSE(bitBuffer1Copy < bitBuffer1);

    const BitBuffer bitBuffer2(std::vector<uint8_t>({0xAB, 0xF0}), 11);
    ASSERT_FALSE(bitBuffer1 < bitBuffer2);
    ASSERT_FALSE(bitBuffer2 < bitBuffer1);

    const BitBuffer bitBuffer3(std::vector<uint8_t>({0xAB, 0x00}), 11);
    ASSERT_TRUE(bitBuffer3 < bitBuffer1);
    ASSERT_FALSE(bitBuffer1 < bitBuffer3);

    const BitBuffer bitBuffer4(std::vector<uint8_t>({0x00, 0x00}), 11);
    ASSERT_TRUE(bitBuffer4 < bitBuffer1);
    ASSERT_FALSE(bitBuffer1 < bitBuffer4);

    const BitBuffer bitBuffer5(std::vector<uint8_t>({0xAB, 0xE0, 0x00}), 20);
    ASSERT_TRUE(bitBuffer1 < bitBuffer5);
    ASSERT_FALSE(bitBuffer5 < bitBuffer1);
}

TEST(BitBufferTest, hashCode)
{
    const size_t bitSize = 11;
    const BitBuffer bitBuffer1(std::vector<uint8_t>({0xAB, 0xE0}), bitSize);
    const BitBuffer bitBuffer2(std::vector<uint8_t>({0xAB, 0xF0}), bitSize);
    ASSERT_EQ(bitBuffer1.hashCode(), bitBuffer2.hashCode());

    const BitBuffer bitBuffer3(std::vector<uint8_t>({0xAB, 0xFF}), bitSize);
    ASSERT_EQ(bitBuffer1.hashCode(), bitBuffer3.hashCode());

    const BitBuffer bitBuffer4(std::vector<uint8_t>({0xAB, 0xC0}), bitSize);
    ASSERT_NE(bitBuffer1.hashCode(), bitBuffer4.hashCode());

    const BitBuffer bitBuffer5(std::vector<uint8_t>({0xBA, 0xE0}), bitSize);
    ASSERT_NE(bitBuffer1.hashCode(), bitBuffer5.hashCode());

    const BitBuffer bitBuffer6(std::vector<uint8_t>({0xAB}));
    ASSERT_NE(bitBuffer1.hashCode(), bitBuffer6.hashCode());

    const BitBuffer bitBuffer7;
    ASSERT_NE(bitBuffer1.hashCode(), bitBuffer7.hashCode());
}

TEST(BitBufferTest, constGetBuffer)
{
    const size_t bitSize = 11;
    const std::vector<uint8_t> buffer = {0xAB, 0xC0};
    const BitBuffer bitBuffer(buffer, bitSize);

    const uint8_t* readBuffer = bitBuffer.getBuffer();
    for (uint8_t element : buffer)
    {
        ASSERT_EQ(element, *readBuffer);
        readBuffer++;
    }
}

TEST(BitBufferTest, getBuffer)
{
    const size_t bitSize = 11;
    const std::vector<uint8_t> buffer = {0xAB, 0xC0};
    BitBuffer bitBuffer(buffer, bitSize);

    uint8_t* readBuffer = bitBuffer.getBuffer();
    for (uint8_t element : buffer)
    {
        ASSERT_EQ(element, *readBuffer);
        readBuffer++;
    }
}

TEST(BitBufferTest, getBitSize)
{
    const size_t bitSize = 11;
    const std::vector<uint8_t> buffer = {0xAB, 0xC0};
    const BitBuffer bitBuffer(buffer, bitSize);
    ASSERT_EQ(bitSize, bitBuffer.getBitSize());
}

TEST(BitBufferTest, getByteSize)
{
    const size_t bitSize = 11;
    const std::vector<uint8_t> buffer = {0xAB, 0xC0};
    const size_t byteSize = buffer.size();
    const BitBuffer bitBuffer(buffer, bitSize);
    ASSERT_EQ(byteSize, bitBuffer.getByteSize());
}

TEST(BitBufferTest, getBytes)
{
    const size_t bitSize = 11;
    const std::vector<uint8_t> buffer = {0xAB, 0xC0};
    const BitBuffer bitBuffer(buffer, bitSize);

    const std::vector<uint8_t>& bytes = bitBuffer.getBytes();
    ASSERT_EQ(buffer.size(), bytes.size());
    for (size_t i = 0; i < buffer.size(); ++i)
    {
        ASSERT_EQ(buffer[i], bytes[i]);
    }
}

TEST(BitBufferTest, getData)
{
    const size_t bitSize = 11;
    const std::vector<uint8_t> buffer = {0xAB, 0xC0};
    const BitBuffer bitBuffer(buffer, bitSize);

    const Span<const uint8_t> bytes = bitBuffer.getData();
    ASSERT_EQ(buffer.size(), bytes.size());
    for (size_t i = 0; i < buffer.size(); ++i)
    {
        ASSERT_EQ(buffer[i], bytes[i]);
    }
}

TEST(BitBufferTest, cppRuntimeExceptionOperator)
{
    CppRuntimeException exception = CppRuntimeException() << BitBuffer();
    ASSERT_STREQ("BitBuffer([...], 0)", exception.what());
}

TEST(BitBufferTest, stdHash)
{
    const std::hash<BitBuffer> hasher;

    const size_t bitSize = 11;
    const BitBuffer bitBuffer1(std::vector<uint8_t>({0xAB, 0xE0}), bitSize);
    const BitBuffer bitBuffer2(std::vector<uint8_t>({0xAB, 0xF0}), bitSize);
    ASSERT_EQ(hasher(bitBuffer1), hasher(bitBuffer2));

    const BitBuffer bitBuffer3(std::vector<uint8_t>({0xAB, 0xFF}), bitSize);
    ASSERT_EQ(hasher(bitBuffer1), hasher(bitBuffer3));

    const BitBuffer bitBuffer4(std::vector<uint8_t>({0xAB, 0xC0}), bitSize);
    ASSERT_NE(hasher(bitBuffer1), hasher(bitBuffer4));

    const BitBuffer bitBuffer5(std::vector<uint8_t>({0xBA, 0xE0}), bitSize);
    ASSERT_NE(hasher(bitBuffer1), hasher(bitBuffer5));

    const BitBuffer bitBuffer6(std::vector<uint8_t>({0xAB}));
    ASSERT_NE(hasher(bitBuffer1), hasher(bitBuffer6));

    const BitBuffer bitBuffer7;
    ASSERT_NE(hasher(bitBuffer1), hasher(bitBuffer7));
}

TEST(BitBufferTest, bitSizeOf)
{
    BitBuffer buffer1(std::vector<uint8_t>({0xAB, 0xC0}), 8);
    BitBufferView view1 = buffer1;
    EXPECT_EQ(8 + 8, detail::bitSizeOf(view1));

    BitBuffer buffer2(std::vector<uint8_t>({0xAB, 0xC0}), 11);
    BitBufferView view2 = buffer2;
    EXPECT_EQ(8 + 11, detail::bitSizeOf(view2));

    BitBuffer buffer3(std::vector<uint8_t>({0xAB, 0xCD}), 16);
    BitBufferView view3 = buffer3;
    EXPECT_EQ(8 + 16, detail::bitSizeOf(view3));

    BitBuffer buffer4(std::vector<uint8_t>({0xAB, 0xCD}));
    BitBufferView view4 = buffer4;
    EXPECT_EQ(8 + 16, detail::bitSizeOf(view4));

    BitBuffer buffer5(std::vector<uint8_t>(16), 127);
    BitBufferView view5 = buffer5;
    EXPECT_EQ(8 + 15 * 8 + 7, detail::bitSizeOf(view5));

    BitBuffer buffer6(std::vector<uint8_t>(16), 128);
    BitBufferView view6 = buffer6;
    EXPECT_EQ(16 + 16 * 8, detail::bitSizeOf(view6));
}

} // namespace zserio
