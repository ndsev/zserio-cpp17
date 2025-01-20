#include <string>

#include "enumeration_types/bitfield_const_enum/Color.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/CppRuntimeException.h"

namespace enumeration_types
{
namespace bitfield_const_enum
{

class BitfieldConstEnumTest : public ::testing::Test
{
protected:
    static constexpr size_t COLOR_BITSIZEOF = 5;

    static constexpr uint8_t NONE_VALUE = 0;
    static constexpr uint8_t RED_VALUE = 2;
    static constexpr uint8_t BLUE_VALUE = 3;
    static constexpr uint8_t GREEN_VALUE = 7;
};

TEST_F(BitfieldConstEnumTest, EnumTraits)
{
    ASSERT_EQ(std::string("NONE"), zserio::EnumTraits<Color>::names[0]);
    ASSERT_EQ(std::string("GREEN"), zserio::EnumTraits<Color>::names[3]);
    ASSERT_EQ(4, zserio::EnumTraits<Color>::names.size());

    ASSERT_EQ(Color::RED, zserio::EnumTraits<Color>::values[1]);
    ASSERT_EQ(Color::BLUE, zserio::EnumTraits<Color>::values[2]);
    ASSERT_EQ(4, zserio::EnumTraits<Color>::values.size());
}

TEST_F(BitfieldConstEnumTest, enumToOrdinal)
{
    ASSERT_EQ(0, zserio::enumToOrdinal(Color::NONE));
    ASSERT_EQ(1, zserio::enumToOrdinal(Color::RED));
    ASSERT_EQ(2, zserio::enumToOrdinal(Color::BLUE));
    ASSERT_EQ(3, zserio::enumToOrdinal(Color::GREEN));
}

TEST_F(BitfieldConstEnumTest, valueToEnum)
{
    ASSERT_EQ(Color::NONE, zserio::valueToEnum<Color>(NONE_VALUE));
    ASSERT_EQ(Color::RED, zserio::valueToEnum<Color>(RED_VALUE));
    ASSERT_EQ(Color::BLUE, zserio::valueToEnum<Color>(BLUE_VALUE));
    ASSERT_EQ(Color::GREEN, zserio::valueToEnum<Color>(GREEN_VALUE));
}

TEST_F(BitfieldConstEnumTest, stringToEnum)
{
    ASSERT_EQ(Color::NONE, zserio::stringToEnum<Color>("NONE"));
    ASSERT_EQ(Color::RED, zserio::stringToEnum<Color>("RED"));
    ASSERT_EQ(Color::BLUE, zserio::stringToEnum<Color>("BLUE"));
    ASSERT_EQ(Color::GREEN, zserio::stringToEnum<Color>("GREEN"));
}

TEST_F(BitfieldConstEnumTest, valueToEnumFailure)
{
    ASSERT_THROW(zserio::valueToEnum<Color>(1), zserio::CppRuntimeException);
}

TEST_F(BitfieldConstEnumTest, stringToEnumFailure)
{
    ASSERT_THROW(zserio::stringToEnum<Color>("NONEXISTING"), zserio::CppRuntimeException);
}

TEST_F(BitfieldConstEnumTest, bitSizeOf)
{
    ASSERT_EQ(COLOR_BITSIZEOF, zserio::detail::bitSizeOf(Color::NONE));
}

TEST_F(BitfieldConstEnumTest, read)
{
    zserio::BitBuffer bitBuffer(COLOR_BITSIZEOF);
    zserio::BitStreamWriter writer(bitBuffer);
    writer.writeUnsignedBits32(static_cast<uint32_t>(Color::RED), COLOR_BITSIZEOF);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    Color color{};
    zserio::detail::read(reader, color);
    ASSERT_EQ(RED_VALUE, zserio::enumToValue(color));
}

TEST_F(BitfieldConstEnumTest, write)
{
    const Color color(Color::BLUE);
    zserio::BitBuffer bitBuffer(COLOR_BITSIZEOF);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, color);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    ASSERT_EQ(BLUE_VALUE, reader.readUnsignedBits32(COLOR_BITSIZEOF));
}

TEST_F(BitfieldConstEnumTest, calcHashCode)
{
    // use hardcoded values to check that the hash code is stable

    Color color = Color::RED;
    const size_t colorHash = 853;
    Color equalColor = Color::RED;
    Color diffColor = Color::NONE;
    const size_t diffColorHash = 851;

    test_utils::hashTest(color, colorHash, equalColor, diffColor, diffColorHash);
}

} // namespace bitfield_const_enum
} // namespace enumeration_types
