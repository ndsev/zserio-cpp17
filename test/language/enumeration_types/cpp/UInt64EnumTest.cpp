#include "enumeration_types/uint64_enum/DarkColor.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/CppRuntimeException.h"

namespace enumeration_types
{
namespace uint64_enum
{

class UInt64EnumTest : public ::testing::Test
{
protected:
    static constexpr size_t DARK_COLOR_BITSIZEOF = 64;

    static constexpr uint64_t NONE_COLOR_VALUE = 0;
    static constexpr uint64_t DARK_RED_VALUE = 1;
    static constexpr uint64_t DARK_BLUE_VALUE = 2;
    static constexpr uint64_t DARK_GREEN_VALUE = 7;
};

TEST_F(UInt64EnumTest, EnumTraits)
{
    ASSERT_EQ(std::string("noneColor"), zserio::EnumTraits<DarkColor>::names[0]);
    ASSERT_EQ(std::string("DarkGreen"), zserio::EnumTraits<DarkColor>::names[3]);
    ASSERT_EQ(4, zserio::EnumTraits<DarkColor>::names.size());

    ASSERT_EQ(DarkColor::DARK_RED, zserio::EnumTraits<DarkColor>::values[1]);
    ASSERT_EQ(DarkColor::dark_blue, zserio::EnumTraits<DarkColor>::values[2]);
    ASSERT_EQ(4, zserio::EnumTraits<DarkColor>::values.size());
}

TEST_F(UInt64EnumTest, enumToOrdinal)
{
    ASSERT_EQ(0, zserio::enumToOrdinal(DarkColor::noneColor));
    ASSERT_EQ(1, zserio::enumToOrdinal(DarkColor::DARK_RED));
    ASSERT_EQ(2, zserio::enumToOrdinal(DarkColor::dark_blue));
    ASSERT_EQ(3, zserio::enumToOrdinal(DarkColor::DarkGreen));
}

TEST_F(UInt64EnumTest, valueToEnum)
{
    ASSERT_EQ(DarkColor::noneColor, zserio::valueToEnum<DarkColor>(NONE_COLOR_VALUE));
    ASSERT_EQ(DarkColor::DARK_RED, zserio::valueToEnum<DarkColor>(DARK_RED_VALUE));
    ASSERT_EQ(DarkColor::dark_blue, zserio::valueToEnum<DarkColor>(DARK_BLUE_VALUE));
    ASSERT_EQ(DarkColor::DarkGreen, zserio::valueToEnum<DarkColor>(DARK_GREEN_VALUE));
}

TEST_F(UInt64EnumTest, stringToEnum)
{
    ASSERT_EQ(DarkColor::noneColor, zserio::stringToEnum<DarkColor>("noneColor"));
    ASSERT_EQ(DarkColor::DARK_RED, zserio::stringToEnum<DarkColor>("DARK_RED"));
    ASSERT_EQ(DarkColor::dark_blue, zserio::stringToEnum<DarkColor>("dark_blue"));
    ASSERT_EQ(DarkColor::DarkGreen, zserio::stringToEnum<DarkColor>("DarkGreen"));
}

TEST_F(UInt64EnumTest, valueToEnumFailure)
{
    ASSERT_THROW(zserio::valueToEnum<DarkColor>(3), zserio::CppRuntimeException);
}

TEST_F(UInt64EnumTest, stringToEnumFailure)
{
    ASSERT_THROW(zserio::stringToEnum<DarkColor>("NONEXISTING"), zserio::CppRuntimeException);
}

TEST_F(UInt64EnumTest, bitSizeOf)
{
    ASSERT_TRUE(zserio::detail::bitSizeOf(DarkColor::noneColor) == DARK_COLOR_BITSIZEOF);
}

TEST_F(UInt64EnumTest, read)
{
    zserio::BitBuffer bitBuffer(DARK_COLOR_BITSIZEOF);
    zserio::BitStreamWriter writer(bitBuffer);
    writer.writeUnsignedBits64(static_cast<uint64_t>(DarkColor::DARK_RED), DARK_COLOR_BITSIZEOF);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    DarkColor darkColor{};
    zserio::detail::read(reader, darkColor);
    ASSERT_EQ(DARK_RED_VALUE, zserio::enumToValue(darkColor));
}

TEST_F(UInt64EnumTest, write)
{
    const DarkColor darkColor(DarkColor::dark_blue);
    zserio::BitBuffer bitBuffer(DARK_COLOR_BITSIZEOF);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, darkColor);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    ASSERT_EQ(DARK_BLUE_VALUE, reader.readUnsignedBits64(DARK_COLOR_BITSIZEOF));
}

TEST_F(UInt64EnumTest, stdHash)
{
    // use hardcoded values to check that the hash code is stable

    DarkColor color = DarkColor::DARK_RED;
    const size_t colorHash = 852;
    DarkColor equalColor = DarkColor::DARK_RED;
    DarkColor diffColor = DarkColor::noneColor;
    const size_t diffColorHash = 851;

    test_utils::hashTest(color, colorHash, equalColor, diffColor, diffColorHash);
}

} // namespace uint64_enum
} // namespace enumeration_types
