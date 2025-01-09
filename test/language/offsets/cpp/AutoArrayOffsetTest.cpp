#include "gtest/gtest.h"
#include "offsets/auto_array_offset/AutoArrayHolder.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace offsets
{
namespace auto_array_offset
{

using AllocatorType = AutoArrayHolder::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;
using BitBuffer = zserio::BasicBitBuffer<AllocatorType>;

class AutoArrayOffsetTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(AUTO_ARRAY_OFFSET, 32);

        writer.writeUnsignedBits32(FORCED_ALIGNMENT_VALUE, 8);

        zserio::detail::write(writer, zserio::VarSize(AUTO_ARRAY_LENGTH));
        for (size_t i = 0; i < AUTO_ARRAY_LENGTH; ++i)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(i), 7);
        }
    }

    static void fillData(AutoArrayHolder& data, bool setOffset = false)
    {
        if (setOffset)
        {
            data.autoArrayOffset = AUTO_ARRAY_OFFSET;
        }

        data.forceAlignment = FORCED_ALIGNMENT_VALUE;

        VectorType<zserio::Int7> autoArray;
        autoArray.reserve(AUTO_ARRAY_LENGTH);
        for (size_t i = 0; i < AUTO_ARRAY_LENGTH; ++i)
        {
            autoArray.push_back(static_cast<int8_t>(i));
        }
        data.autoArray = autoArray;
    }

    static constexpr size_t AUTO_ARRAY_LENGTH = 5;
    static constexpr uint8_t FORCED_ALIGNMENT_VALUE = 0;

    static constexpr uint32_t AUTO_ARRAY_OFFSET = 5;

    static constexpr size_t AUTO_ARRAY_HOLDER_BIT_SIZE = 32 + 1 + 7 + 8 + AUTO_ARRAY_LENGTH * 7;
};

TEST_F(AutoArrayOffsetTest, read)
{
    AutoArrayHolder data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(AutoArrayOffsetTest, writeRead)
{
    AutoArrayHolder data;
    fillData(data);
    zserio::View view(data);

    EXPECT_EQ(0, data.autoArrayOffset);

    test_utils::writeReadTest(data);
    ASSERT_EQ(AUTO_ARRAY_OFFSET, data.autoArrayOffset);
    ASSERT_EQ(AUTO_ARRAY_OFFSET, view.autoArrayOffset());
}

} // namespace auto_array_offset
} // namespace offsets
