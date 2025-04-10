#include "enumeration_types/uint8_enum/DarkColor.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/CppRuntimeException.h"

namespace enumeration_types
{
namespace uint8_enum
{

class UInt8EnumTest : public ::testing::Test
{
protected:
    static constexpr uint8_t DARK_COLOR_BITSIZEOF = 8;

    static constexpr zserio::UInt8 NONE_VALUE = 0;
    static constexpr zserio::UInt8 DARK_RED_VALUE = 1;
    static constexpr zserio::UInt8 DARK_BLUE_VALUE = 2;
    static constexpr zserio::UInt8 DARK_GREEN_VALUE = 7;
};

TEST_F(UInt8EnumTest, EnumTraits)
{
    ASSERT_EQ(std::string("NONE"), zserio::EnumTraits<DarkColor>::names[0]);
    ASSERT_EQ(std::string("DARK_GREEN"), zserio::EnumTraits<DarkColor>::names[3]);
    ASSERT_EQ(4, zserio::EnumTraits<DarkColor>::names.size());

    ASSERT_EQ(DarkColor::DARK_RED, zserio::EnumTraits<DarkColor>::values[1]);
    ASSERT_EQ(DarkColor::DARK_BLUE, zserio::EnumTraits<DarkColor>::values[2]);
    ASSERT_EQ(4, zserio::EnumTraits<DarkColor>::values.size());
}

TEST_F(UInt8EnumTest, enumToOrdinal)
{
    ASSERT_EQ(0, zserio::enumToOrdinal(DarkColor::NONE));
    ASSERT_EQ(1, zserio::enumToOrdinal(DarkColor::DARK_RED));
    ASSERT_EQ(2, zserio::enumToOrdinal(DarkColor::DARK_BLUE));
    ASSERT_EQ(3, zserio::enumToOrdinal(DarkColor::DARK_GREEN));
}

TEST_F(UInt8EnumTest, valueToEnum)
{
    ASSERT_EQ(DarkColor::NONE, zserio::valueToEnum<DarkColor>(NONE_VALUE));
    ASSERT_EQ(DarkColor::DARK_RED, zserio::valueToEnum<DarkColor>(DARK_RED_VALUE));
    ASSERT_EQ(DarkColor::DARK_BLUE, zserio::valueToEnum<DarkColor>(DARK_BLUE_VALUE));
    ASSERT_EQ(DarkColor::DARK_GREEN, zserio::valueToEnum<DarkColor>(DARK_GREEN_VALUE));
}

TEST_F(UInt8EnumTest, stringToEnum)
{
    ASSERT_EQ(DarkColor::NONE, zserio::stringToEnum<DarkColor>("NONE"));
    ASSERT_EQ(DarkColor::DARK_RED, zserio::stringToEnum<DarkColor>("DARK_RED"));
    ASSERT_EQ(DarkColor::DARK_BLUE, zserio::stringToEnum<DarkColor>("DARK_BLUE"));
    ASSERT_EQ(DarkColor::DARK_GREEN, zserio::stringToEnum<DarkColor>("DARK_GREEN"));
}

TEST_F(UInt8EnumTest, valueToEnumFailure)
{
    ASSERT_THROW(zserio::valueToEnum<DarkColor>(3), zserio::CppRuntimeException);
}

TEST_F(UInt8EnumTest, stringToEnumFailure)
{
    ASSERT_THROW(zserio::stringToEnum<DarkColor>("NONEXISTING"), zserio::CppRuntimeException);
}
TEST_F(UInt8EnumTest, bitSizeOf)
{
    ASSERT_TRUE(zserio::detail::bitSizeOf(DarkColor::NONE) == DARK_COLOR_BITSIZEOF);
}

TEST_F(UInt8EnumTest, read)
{
    zserio::BitBuffer bitBuffer(DARK_COLOR_BITSIZEOF);
    zserio::BitStreamWriter writer(bitBuffer);
    writer.writeUnsignedBits32(zserio::enumToValue(DarkColor::DARK_RED), DARK_COLOR_BITSIZEOF);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    DarkColor darkColor{};
    zserio::detail::read(reader, darkColor);
    ASSERT_EQ(DARK_RED_VALUE, zserio::enumToValue(darkColor));
}

TEST_F(UInt8EnumTest, write)
{
    zserio::BitBuffer bitBuffer(DARK_COLOR_BITSIZEOF);
    const DarkColor darkColor(DarkColor::DARK_BLUE);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, darkColor);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    ASSERT_EQ(DARK_BLUE_VALUE, reader.readUnsignedBits32(DARK_COLOR_BITSIZEOF));
}

TEST_F(UInt8EnumTest, stdHash)
{
    // use hardcoded values to check that the hash code is stable

    DarkColor color = DarkColor::DARK_RED;
    const size_t colorHash = 852;
    DarkColor equalColor = DarkColor::DARK_RED;
    DarkColor diffColor = DarkColor::NONE;
    const size_t diffColorHash = 851;

    test_utils::hashTest(color, colorHash, equalColor, diffColor, diffColorHash);
}

} // namespace uint8_enum
} // namespace enumeration_types
