#include <array>

#include "gtest/gtest.h"
#include "indexed_offsets/optional_nested_indexed_offset_array/Header.h"
#include "indexed_offsets/optional_nested_indexed_offset_array/OptionalNestedIndexedOffsetArray.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace indexed_offsets
{
namespace optional_nested_indexed_offset_array
{

using AllocatorType = OptionalNestedIndexedOffsetArray::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class OptionalNestedIndexedOffsetArrayTest : public ::testing::Test
{
public:
    OptionalNestedIndexedOffsetArrayTest()
    {
        const std::array<const char*, NUM_ELEMENTS> data = {"Green", "Red", "Pink", "Blue", "Black"};
        m_data.assign(data.begin(), data.end());
    }

protected:
    void writeData(zserio::BitStreamWriter& writer, uint16_t length)
    {
        writer.writeUnsignedBits32(length, 16);

        if (length > 0)
        {
            uint32_t currentOffset = ELEMENT0_OFFSET;
            for (uint16_t i = 0; i < length; ++i)
            {
                writer.writeUnsignedBits32(currentOffset, 32);
                currentOffset += static_cast<uint32_t>(zserio::detail::bitSizeOf(m_data[i]) / 8);
            }

            // already aligned
            for (uint16_t i = 0; i < length; ++i)
            {
                writer.writeString(m_data[i]);
            }
        }

        writer.writeUnsignedBits32(FIELD_VALUE, 6);
    }

    void checkOffsets(const OptionalNestedIndexedOffsetArray& data,
            const zserio::View<OptionalNestedIndexedOffsetArray>& view)
    {
        const uint16_t length = view.header().length();
        uint32_t expectedOffset = ELEMENT0_OFFSET;
        for (uint16_t i = 0; i < length; ++i)
        {
            ASSERT_EQ(expectedOffset, data.header.offsets.at(i));
            ASSERT_EQ(expectedOffset, view.header().offsets().at(i));
            expectedOffset += static_cast<uint32_t>(zserio::detail::bitSizeOf(m_data[i]) / 8);
        }
    }

    void fillData(OptionalNestedIndexedOffsetArray& data, uint16_t length, bool setOffsets = false)
    {
        data.header.length = length;
        data.header.offsets.resize(length);
        if (setOffsets)
        {
            uint32_t currentOffset = ELEMENT0_OFFSET;
            for (uint16_t i = 0; i < length; ++i)
            {
                data.header.offsets[i] = currentOffset;
                currentOffset += static_cast<uint32_t>(zserio::detail::bitSizeOf(m_data[i]) / 8);
            }
        }

        if (length > 0)
        {
            data.data = m_data;
        }

        data.field = FIELD_VALUE;
    }

    void readTest(const OptionalNestedIndexedOffsetArray& expectedData, uint16_t length)
    {
        test_utils::readTest(std::bind(&OptionalNestedIndexedOffsetArrayTest::writeData, this,
                                     std::placeholders::_1, length),
                expectedData);
    }

    static constexpr uint8_t NUM_ELEMENTS = 5;

    static constexpr uint32_t ELEMENT0_OFFSET = sizeof(int16_t) + NUM_ELEMENTS * sizeof(uint32_t);

    static constexpr uint8_t FIELD_VALUE = 63;

private:
    VectorType<StringType> m_data;
};

TEST_F(OptionalNestedIndexedOffsetArrayTest, readWithOptional)
{
    OptionalNestedIndexedOffsetArray data;
    const uint16_t length = NUM_ELEMENTS;
    fillData(data, length, true);

    readTest(data, length);
}

TEST_F(OptionalNestedIndexedOffsetArrayTest, readWithoutOptional)
{
    OptionalNestedIndexedOffsetArray data;
    const uint16_t length = 0;
    fillData(data, length, true);

    readTest(data, length);
}

TEST_F(OptionalNestedIndexedOffsetArrayTest, writeReadWithOptional)
{
    OptionalNestedIndexedOffsetArray data;
    const uint16_t length = NUM_ELEMENTS;
    fillData(data, length);
    zserio::View view(data);

    for (auto offset : data.header.offsets)
    {
        EXPECT_EQ(0, offset);
    }

    test_utils::writeReadTest(data);
    checkOffsets(data, view);
}

TEST_F(OptionalNestedIndexedOffsetArrayTest, writeReadWithoutOptional)
{
    OptionalNestedIndexedOffsetArray data;
    const uint16_t length = 0;
    fillData(data, length);
    zserio::View view(data);

    EXPECT_EQ(0, data.header.offsets.size());

    test_utils::writeReadTest(data);

    ASSERT_EQ(0, data.header.offsets.size());
}

} // namespace optional_nested_indexed_offset_array
} // namespace indexed_offsets
