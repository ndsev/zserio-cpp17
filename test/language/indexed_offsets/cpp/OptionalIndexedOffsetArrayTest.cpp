#include <array>

#include "gtest/gtest.h"
#include "indexed_offsets/optional_indexed_offset_array/OptionalIndexedOffsetArray.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace indexed_offsets
{
namespace optional_indexed_offset_array
{

using AllocatorType = OptionalIndexedOffsetArray::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class OptionalIndexedOffsetArrayTest : public ::testing::Test
{
public:
    OptionalIndexedOffsetArrayTest()
    {
        const std::array<const char*, NUM_ELEMENTS> data = {"Green", "Red", "Pink", "Blue", "Black"};
        m_data.assign(data.begin(), data.end());
    }

protected:
    void writeData(zserio::BitStreamWriter& writer, bool hasOptional)
    {
        uint32_t currentOffset = ELEMENT0_OFFSET;
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            if (hasOptional)
            {
                writer.writeUnsignedBits32(currentOffset, 32);
            }
            else
            {
                writer.writeUnsignedBits32(0, 32);
            }
            currentOffset += static_cast<uint32_t>(zserio::detail::bitSizeOf(m_data[i]) / 8);
        }

        writer.writeBool(hasOptional);

        if (hasOptional)
        {
            writer.writeUnsignedBits32(0, 7);
            for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
            {
                writer.writeString(m_data[i]);
            }
        }

        writer.writeUnsignedBits32(FIELD_VALUE, 6);
    }

    void checkOffsets(
            const OptionalIndexedOffsetArray& data, const zserio::View<OptionalIndexedOffsetArray>& view)
    {
        uint32_t expectedOffset = ELEMENT0_OFFSET;
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            ASSERT_EQ(expectedOffset, data.offsets.at(i));
            ASSERT_EQ(expectedOffset, view.offsets().at(i));
            expectedOffset += static_cast<uint32_t>(zserio::detail::bitSizeOf(m_data[i]) / 8);
        }
    }

    void fillData(OptionalIndexedOffsetArray& data, bool hasOptional, bool setOffsets = false)
    {
        data.offsets.resize(NUM_ELEMENTS);
        if (hasOptional && setOffsets)
        {
            uint32_t currentOffset = ELEMENT0_OFFSET;
            for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
            {
                data.offsets[i] = currentOffset;
                currentOffset += static_cast<uint32_t>(zserio::detail::bitSizeOf(m_data[i]) / 8);
            }
        }

        data.hasOptional = hasOptional;

        if (hasOptional)
        {
            data.data = m_data;
        }

        data.field = FIELD_VALUE;
    }

    void readTest(const OptionalIndexedOffsetArray& expectedData, bool hasOptional)
    {
        test_utils::readTest(
                std::bind(&OptionalIndexedOffsetArrayTest::writeData, this, std::placeholders::_1, hasOptional),
                expectedData);
    }

    static constexpr uint8_t NUM_ELEMENTS = 5;

    static constexpr uint32_t ELEMENT0_OFFSET = NUM_ELEMENTS * sizeof(uint32_t) + sizeof(uint8_t);

    static constexpr uint8_t FIELD_VALUE = 63;

private:
    VectorType<StringType> m_data;
};

TEST_F(OptionalIndexedOffsetArrayTest, readWithOptional)
{
    OptionalIndexedOffsetArray data;
    const bool hasOptional = true;
    fillData(data, hasOptional, true);

    readTest(data, hasOptional);
}

TEST_F(OptionalIndexedOffsetArrayTest, readWithoutOptional)
{
    OptionalIndexedOffsetArray data;
    const bool hasOptional = false;
    fillData(data, hasOptional, true);

    readTest(data, hasOptional);
}

TEST_F(OptionalIndexedOffsetArrayTest, writeReadWithOptional)
{
    OptionalIndexedOffsetArray data;
    const bool hasOptional = true;
    fillData(data, hasOptional);
    zserio::View view(data);

    for (auto offset : data.offsets)
    {
        EXPECT_EQ(0, offset);
    }

    test_utils::writeReadTest(data);
    checkOffsets(data, view);
}

TEST_F(OptionalIndexedOffsetArrayTest, writeReadWithoutOptional)
{
    OptionalIndexedOffsetArray data;
    const bool hasOptional = false;
    fillData(data, hasOptional);
    zserio::View view(data);

    for (auto offset : data.offsets)
    {
        EXPECT_EQ(0, offset);
    }

    test_utils::writeReadTest(data);

    for (auto offset : data.offsets)
    {
        ASSERT_EQ(0, offset);
    }
}

} // namespace optional_indexed_offset_array
} // namespace indexed_offsets
