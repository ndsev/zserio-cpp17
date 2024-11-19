#include <utility>

#include "functions/structure_value/CustomVarInt.h"
#include "functions/structure_value/CustomVarList.h"
#include "gtest/gtest.h"
#include "test_utils/ReadTest.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/BitStreamWriter.h"

namespace functions
{
namespace structure_value
{

class StructureValueTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, uint32_t value)
    {
        if (value <= MAX_ONE_BYTE_VALUE)
        {
            writer.writeUnsignedBits32(value, 8);
        }
        else if (value <= 0xFFFF)
        {
            writer.writeUnsignedBits32(TWO_BYTES_INDICATOR, 8);
            writer.writeUnsignedBits32(value, 16);
        }
        else
        {
            writer.writeUnsignedBits32(FOUR_BYTES_INDICATOR, 8);
            writer.writeUnsignedBits32(value, 32);
        }
    }

    static void fillData(CustomVarInt& data, uint32_t value)
    {
        if (value <= MAX_ONE_BYTE_VALUE)
        {
            data.val1 = static_cast<uint8_t>(value);
        }
        else if (value <= 0xFFFF)
        {
            data.val1 = TWO_BYTES_INDICATOR;
            data.val2 = static_cast<uint16_t>(value);
        }
        else
        {
            data.val1 = FOUR_BYTES_INDICATOR;
            data.val3 = value;
        }
    }

    static void checkCustomVarInt(uint32_t value)
    {
        CustomVarInt data;
        fillData(data, value);
        zserio::View<CustomVarInt> view(data);
        zserio::UInt32 readValue = view.getValue();
        ASSERT_EQ(value, readValue);

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, value), data);
        test_utils::writeReadTest(data);
    }

    static constexpr uint32_t MAX_ONE_BYTE_VALUE = 253;
    static constexpr uint8_t TWO_BYTES_INDICATOR = 255;
    static constexpr uint8_t FOUR_BYTES_INDICATOR = 254;
};

TEST_F(StructureValueTest, checkCustomVarIntValue42)
{
    checkCustomVarInt(42);
}

TEST_F(StructureValueTest, checkCustomVarIntValue253)
{
    checkCustomVarInt(MAX_ONE_BYTE_VALUE);
}

TEST_F(StructureValueTest, checkCustomVarIntValue255)
{
    checkCustomVarInt(TWO_BYTES_INDICATOR);
}

TEST_F(StructureValueTest, checkCustomVarIntValue254)
{
    checkCustomVarInt(FOUR_BYTES_INDICATOR);
}

TEST_F(StructureValueTest, checkCustomVarIntValue1000)
{
    checkCustomVarInt(1000);
}

TEST_F(StructureValueTest, checkCustomVarIntValue87654)
{
    checkCustomVarInt(87654);
}

} // namespace structure_value
} // namespace functions
