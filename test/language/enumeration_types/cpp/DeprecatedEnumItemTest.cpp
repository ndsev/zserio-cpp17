#include <fstream>

#include "enumeration_types/deprecated_enum_item/Dummy.h"
#include "enumeration_types/deprecated_enum_item/Traffic.h"
#include "gtest/gtest.h"
#include "test_utils/Assertions.h"
#include "test_utils/TestUtility.h"
#include "zserio/CppRuntimeException.h"

namespace enumeration_types
{
namespace deprecated_enum_item
{

class DeprecatedEnumItemTest : public ::testing::Test
{
protected:
    static constexpr uint8_t NONE_VALUE = 1;
    static constexpr uint8_t HEAVY_VALUE = 2;
    static constexpr uint8_t LIGHT_VALUE = 3;
    static constexpr uint8_t MID_VALUE = 4;

    static constexpr size_t TRAFFIC_BIT_SIZE = 8;

    static constexpr std::string_view TRAFFIC_HEADER_PATH =
            "language/enumeration_types/gen/enumeration_types/deprecated_enum_item/Traffic.h";
};

TEST_F(DeprecatedEnumItemTest, deprecatedMacroPresent)
{
    ASSERT_STRING_IN_FILE_PRESENT(TRAFFIC_HEADER_PATH, "HEAVY [[deprecated]] =");
}

TEST_F(DeprecatedEnumItemTest, EnumTraits)
{
    ASSERT_EQ(std::string("NONE"), zserio::EnumTraits<Traffic>::names[0]);
    ASSERT_EQ(std::string("HEAVY"), zserio::EnumTraits<Traffic>::names[1]);
    ASSERT_EQ(std::string("LIGHT"), zserio::EnumTraits<Traffic>::names[2]);
    ASSERT_EQ(std::string("MID"), zserio::EnumTraits<Traffic>::names[3]);

    ASSERT_EQ(Traffic::NONE, zserio::EnumTraits<Traffic>::values[0]);
    ASSERT_EQ(Traffic::HEAVY, zserio::EnumTraits<Traffic>::values[1]);
    ASSERT_EQ(Traffic::LIGHT, zserio::EnumTraits<Traffic>::values[2]);
    ASSERT_EQ(Traffic::MID, zserio::EnumTraits<Traffic>::values[3]);
}

TEST_F(DeprecatedEnumItemTest, enumToOrdinal)
{
    ASSERT_EQ(0, zserio::enumToOrdinal(Traffic::NONE));
    ASSERT_EQ(1, zserio::enumToOrdinal(Traffic::HEAVY));
    ASSERT_EQ(2, zserio::enumToOrdinal(Traffic::LIGHT));
    ASSERT_EQ(3, zserio::enumToOrdinal(Traffic::MID));
}

TEST_F(DeprecatedEnumItemTest, valueToEnum)
{
    ASSERT_EQ(Traffic::NONE, zserio::valueToEnum<Traffic>(NONE_VALUE));
    ASSERT_EQ(Traffic::HEAVY, zserio::valueToEnum<Traffic>(HEAVY_VALUE));
    ASSERT_EQ(Traffic::LIGHT, zserio::valueToEnum<Traffic>(LIGHT_VALUE));
    ASSERT_EQ(Traffic::MID, zserio::valueToEnum<Traffic>(MID_VALUE));
}

TEST_F(DeprecatedEnumItemTest, stringToEnum)
{
    ASSERT_EQ(Traffic::NONE, zserio::stringToEnum<Traffic>("NONE"));
    ASSERT_EQ(Traffic::HEAVY, zserio::stringToEnum<Traffic>("HEAVY"));
    ASSERT_EQ(Traffic::LIGHT, zserio::stringToEnum<Traffic>("LIGHT"));
    ASSERT_EQ(Traffic::MID, zserio::stringToEnum<Traffic>("MID"));
}

TEST_F(DeprecatedEnumItemTest, valueToEnumFailure)
{
    ASSERT_THROW(zserio::valueToEnum<Traffic>(MID_VALUE + 1), zserio::CppRuntimeException);
}

TEST_F(DeprecatedEnumItemTest, stringToEnumFailure)
{
    ASSERT_THROW(zserio::stringToEnum<Traffic>("NONEXISTING"), zserio::CppRuntimeException);
}

TEST_F(DeprecatedEnumItemTest, bitSizeOf)
{
    ASSERT_EQ(TRAFFIC_BIT_SIZE, zserio::detail::bitSizeOf(Traffic::HEAVY));
}

TEST_F(DeprecatedEnumItemTest, read)
{
    zserio::BitBuffer bitBuffer(TRAFFIC_BIT_SIZE);
    zserio::BitStreamWriter writer(bitBuffer);
    writer.writeUnsignedBits32(HEAVY_VALUE, TRAFFIC_BIT_SIZE);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    Traffic traffic{};
    zserio::detail::read(reader, traffic);
    ASSERT_EQ(Traffic::HEAVY, traffic);
}

TEST_F(DeprecatedEnumItemTest, writeRead)
{
    zserio::BitBuffer bitBuffer(TRAFFIC_BIT_SIZE * 2);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, Traffic::NONE);
    zserio::detail::write(writer, Traffic::HEAVY);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    ASSERT_EQ(NONE_VALUE, reader.readUnsignedBits32(TRAFFIC_BIT_SIZE));
    ASSERT_EQ(HEAVY_VALUE, reader.readUnsignedBits32(TRAFFIC_BIT_SIZE));
}

// TODO[Mi-L@]: Original test contains enumTypeInfo test, make either specialized type info test or place
//              it also here.

TEST_F(DeprecatedEnumItemTest, stdHash)
{
    // use hardcoded values to check that the hash code is stable

    const Traffic traffic = Traffic::HEAVY;
    const size_t trafficHash = 853;
    const Traffic equalTraffic = Traffic::HEAVY;
    const Traffic diffTraffic = Traffic::MID;
    const size_t diffHash = 855;

    test_utils::hashTest(traffic, trafficHash, equalTraffic, diffTraffic, diffHash);
}

} // namespace deprecated_enum_item
} // namespace enumeration_types
