#include "gtest/gtest.h"
#include "offsets/parameter_offset/School.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace parameter_offset
{

class ParameterOffsetTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(SCHOOL_ID, 16);
        writer.writeUnsignedBits32(ROOM_OFFSET, 32);
        writer.writeUnsignedBits32(ROOM_ID, 16);
    }

    void fillData(School& data, bool setOffset = false)
    {
        data.schoolId = SCHOOL_ID;
        if (setOffset)
        {
            data.offsetHolder.roomOffset = ROOM_OFFSET;
        }
        data.room.roomId = ROOM_ID;
    }

    static constexpr uint16_t SCHOOL_ID = 0x01;
    static constexpr uint16_t ROOM_ID = 0x11;

    static constexpr uint32_t WRONG_ROOM_OFFSET = 0;
    static constexpr uint32_t ROOM_OFFSET = 6;
};

TEST_F(ParameterOffsetTest, read)
{
    School data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(ParameterOffsetTest, writeRead)
{
    School data;
    fillData(data);
    zserio::View view(data);

    EXPECT_EQ(0, data.offsetHolder.roomOffset);
    EXPECT_EQ(0, view.offsetHolder().roomOffset());

    test_utils::writeReadTest(data);

    ASSERT_EQ(ROOM_OFFSET, data.offsetHolder.roomOffset);
    ASSERT_EQ(ROOM_OFFSET, view.offsetHolder().roomOffset());
}

} // namespace parameter_offset
} // namespace offsets
