#include "enumeration_types/multiple_removed_enum_items/Dummy.h"
#include "enumeration_types/multiple_removed_enum_items/Traffic.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/CppRuntimeException.h"

namespace enumeration_types
{
namespace multiple_removed_enum_items
{

class MultipleRemovedEnumItemsTest : public ::testing::Test
{
protected:
    static constexpr uint8_t NONE_VALUE = 1;
    static constexpr uint8_t HEAVY_VALUE = 2;
    static constexpr uint8_t LIGHT_VALUE = 3;
    static constexpr uint8_t MID_VALUE = 4;

    static constexpr size_t TRAFFIC_BIT_SIZE = 8;
};

TEST_F(MultipleRemovedEnumItemsTest, EnumTraits)
{
    ASSERT_EQ(std::string("NONE"), zserio::EnumTraits<Traffic>::names[0]);
    ASSERT_EQ(std::string("ZSERIO_REMOVED_HEAVY"), zserio::EnumTraits<Traffic>::names[1]);
    ASSERT_EQ(std::string("ZSERIO_REMOVED_LIGHT"), zserio::EnumTraits<Traffic>::names[2]);
    ASSERT_EQ(std::string("ZSERIO_REMOVED_MID"), zserio::EnumTraits<Traffic>::names[3]);

    ASSERT_EQ(Traffic::NONE, zserio::EnumTraits<Traffic>::values[0]);
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_HEAVY, zserio::EnumTraits<Traffic>::values[1]);
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_LIGHT, zserio::EnumTraits<Traffic>::values[2]);
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_MID, zserio::EnumTraits<Traffic>::values[3]);
}

TEST_F(MultipleRemovedEnumItemsTest, enumToOrdinal)
{
    ASSERT_EQ(0, zserio::enumToOrdinal(Traffic::NONE));
    ASSERT_EQ(1, zserio::enumToOrdinal(Traffic::ZSERIO_REMOVED_HEAVY));
    ASSERT_EQ(2, zserio::enumToOrdinal(Traffic::ZSERIO_REMOVED_LIGHT));
    ASSERT_EQ(3, zserio::enumToOrdinal(Traffic::ZSERIO_REMOVED_MID));
}

TEST_F(MultipleRemovedEnumItemsTest, valueToEnum)
{
    ASSERT_EQ(Traffic::NONE, zserio::valueToEnum<Traffic>(NONE_VALUE));
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_HEAVY, zserio::valueToEnum<Traffic>(HEAVY_VALUE));
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_LIGHT, zserio::valueToEnum<Traffic>(LIGHT_VALUE));
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_MID, zserio::valueToEnum<Traffic>(MID_VALUE));
}

TEST_F(MultipleRemovedEnumItemsTest, stringToEnum)
{
    ASSERT_EQ(Traffic::NONE, zserio::stringToEnum<Traffic>("NONE"));
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_HEAVY, zserio::stringToEnum<Traffic>("ZSERIO_REMOVED_HEAVY"));
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_LIGHT, zserio::stringToEnum<Traffic>("ZSERIO_REMOVED_LIGHT"));
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_MID, zserio::stringToEnum<Traffic>("ZSERIO_REMOVED_MID"));
}

TEST_F(MultipleRemovedEnumItemsTest, valueToEnumFailure)
{
    ASSERT_THROW(zserio::valueToEnum<Traffic>(MID_VALUE + 1), zserio::CppRuntimeException);
}

TEST_F(MultipleRemovedEnumItemsTest, stringToEnumFailure)
{
    ASSERT_THROW(zserio::stringToEnum<Traffic>("NONEXISTING"), zserio::CppRuntimeException);
}

TEST_F(MultipleRemovedEnumItemsTest, bitSizeOf)
{
    ASSERT_EQ(TRAFFIC_BIT_SIZE, zserio::detail::bitSizeOf(Traffic::NONE));
    ASSERT_EQ(TRAFFIC_BIT_SIZE, zserio::detail::bitSizeOf(Traffic::ZSERIO_REMOVED_HEAVY));
    ASSERT_EQ(TRAFFIC_BIT_SIZE, zserio::detail::bitSizeOf(Traffic::ZSERIO_REMOVED_LIGHT));
    ASSERT_EQ(TRAFFIC_BIT_SIZE, zserio::detail::bitSizeOf(Traffic::ZSERIO_REMOVED_MID));
}

TEST_F(MultipleRemovedEnumItemsTest, read)
{
    zserio::BitBuffer bitBuffer(TRAFFIC_BIT_SIZE * 4);
    zserio::BitStreamWriter writer(bitBuffer);
    writer.writeUnsignedBits32(NONE_VALUE, 8);
    writer.writeUnsignedBits32(HEAVY_VALUE, 8);
    writer.writeUnsignedBits32(LIGHT_VALUE, 8);
    writer.writeUnsignedBits32(MID_VALUE, 8);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    Traffic traffic{};
    zserio::detail::read(reader, traffic);
    ASSERT_EQ(Traffic::NONE, traffic);
    zserio::detail::read(reader, traffic);
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_HEAVY, traffic);
    zserio::detail::read(reader, traffic);
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_LIGHT, traffic);
    zserio::detail::read(reader, traffic);
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_MID, traffic);
}

TEST_F(MultipleRemovedEnumItemsTest, write)
{
    zserio::BitBuffer bitBuffer(TRAFFIC_BIT_SIZE);
    zserio::BitStreamWriter writer(bitBuffer);
    ASSERT_NO_THROW(zserio::detail::write(writer, Traffic::NONE));

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    ASSERT_EQ(NONE_VALUE, reader.readUnsignedBits32(8));

    // TODO[Mi-L@]: Should be solved by issue https://github.com/ndsev/zserio-cpp17/issues/36.
    /*
    ASSERT_THROW(zserio::detail::write(writer, Traffic::ZSERIO_REMOVED_HEAVY), zserio::CppRuntimeException);
    ASSERT_THROW(zserio::detail::write(writer, Traffic::ZSERIO_REMOVED_LIGHT), zserio::CppRuntimeException);
    ASSERT_THROW(zserio::detail::write(writer, Traffic::ZSERIO_REMOVED_MID), zserio::CppRuntimeException);
    */
}

// TODO[Mi-L@]: Original test contains enumTypeInfo test, make either specialized type info test or place
//              it also here.

TEST_F(MultipleRemovedEnumItemsTest, stdHash)
{
    // use hardcoded values to check that the hash code is stable

    const Traffic traffic = Traffic::ZSERIO_REMOVED_HEAVY;
    const size_t trafficHash = 853;
    const Traffic equalTraffic = Traffic::ZSERIO_REMOVED_HEAVY;
    const Traffic diffTraffic = Traffic::ZSERIO_REMOVED_MID;
    const size_t diffHash = 855;

    test_utils::hashTest(traffic, trafficHash, equalTraffic, diffTraffic, diffHash);
}

} // namespace multiple_removed_enum_items
} // namespace enumeration_types
