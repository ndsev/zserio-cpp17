#include "enumeration_types/removed_enum_item/Dummy.h"
#include "enumeration_types/removed_enum_item/Traffic.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/CppRuntimeException.h"

using namespace std::literals;

namespace enumeration_types
{
namespace removed_enum_item
{

class RemovedEnumItemTest : public ::testing::Test
{
protected:
    static constexpr uint8_t NONE_VALUE = 1;
    static constexpr uint8_t HEAVY_VALUE = 2;
    static constexpr uint8_t LIGHT_VALUE = 3;
    static constexpr uint8_t MID_VALUE = 4;

    static constexpr size_t TRAFFIC_BIT_SIZE = 8;
};

TEST_F(RemovedEnumItemTest, EnumTraits)
{
    ASSERT_EQ(std::string("NONE"), zserio::EnumTraits<Traffic>::names[0]);
    ASSERT_EQ(std::string("ZSERIO_REMOVED_HEAVY"), zserio::EnumTraits<Traffic>::names[1]);
    ASSERT_EQ(std::string("LIGHT"), zserio::EnumTraits<Traffic>::names[2]);
    ASSERT_EQ(std::string("MID"), zserio::EnumTraits<Traffic>::names[3]);

    ASSERT_EQ(Traffic::NONE, zserio::EnumTraits<Traffic>::values[0]);
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_HEAVY, zserio::EnumTraits<Traffic>::values[1]);
    ASSERT_EQ(Traffic::LIGHT, zserio::EnumTraits<Traffic>::values[2]);
    ASSERT_EQ(Traffic::MID, zserio::EnumTraits<Traffic>::values[3]);
}

TEST_F(RemovedEnumItemTest, enumToOrdinal)
{
    ASSERT_EQ(0, zserio::enumToOrdinal(Traffic::NONE));
    ASSERT_EQ(1, zserio::enumToOrdinal(Traffic::ZSERIO_REMOVED_HEAVY));
    ASSERT_EQ(2, zserio::enumToOrdinal(Traffic::LIGHT));
    ASSERT_EQ(3, zserio::enumToOrdinal(Traffic::MID));
}

TEST_F(RemovedEnumItemTest, valueToEnum)
{
    ASSERT_EQ(Traffic::NONE, zserio::valueToEnum<Traffic>(NONE_VALUE));
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_HEAVY, zserio::valueToEnum<Traffic>(HEAVY_VALUE));
    ASSERT_EQ(Traffic::LIGHT, zserio::valueToEnum<Traffic>(LIGHT_VALUE));
    ASSERT_EQ(Traffic::MID, zserio::valueToEnum<Traffic>(MID_VALUE));
}

TEST_F(RemovedEnumItemTest, stringToEnum)
{
    ASSERT_EQ(Traffic::NONE, zserio::stringToEnum<Traffic>("NONE"));
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_HEAVY, zserio::stringToEnum<Traffic>("ZSERIO_REMOVED_HEAVY"));
    ASSERT_EQ(Traffic::LIGHT, zserio::stringToEnum<Traffic>("LIGHT"));
    ASSERT_EQ(Traffic::MID, zserio::stringToEnum<Traffic>("MID"));
}

TEST_F(RemovedEnumItemTest, valueToEnumFailure)
{
    ASSERT_THROW(zserio::valueToEnum<Traffic>(MID_VALUE + 1), zserio::CppRuntimeException);
}

TEST_F(RemovedEnumItemTest, stringToEnumFailure)
{
    ASSERT_THROW(zserio::stringToEnum<Traffic>("NONEXISTING"), zserio::CppRuntimeException);
}

TEST_F(RemovedEnumItemTest, bitSizeOf)
{
    ASSERT_EQ(TRAFFIC_BIT_SIZE, zserio::detail::bitSizeOf(Traffic::ZSERIO_REMOVED_HEAVY));
}

TEST_F(RemovedEnumItemTest, read)
{
    zserio::BitBuffer bitBuffer(TRAFFIC_BIT_SIZE);
    zserio::BitStreamWriter writer(bitBuffer);
    writer.writeUnsignedBits32(HEAVY_VALUE, 8);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    Traffic traffic{};
    zserio::detail::read(reader, traffic);
    ASSERT_EQ(Traffic::ZSERIO_REMOVED_HEAVY, traffic);
}

TEST_F(RemovedEnumItemTest, write)
{
    zserio::BitBuffer bitBuffer(TRAFFIC_BIT_SIZE * 3);
    zserio::BitStreamWriter writer(bitBuffer);
    ASSERT_NO_THROW(zserio::detail::write(writer, Traffic::NONE));
    ASSERT_NO_THROW(zserio::detail::write(writer, Traffic::LIGHT));
    ASSERT_NO_THROW(zserio::detail::write(writer, Traffic::MID));

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    ASSERT_EQ(NONE_VALUE, reader.readUnsignedBits32(8));
    ASSERT_EQ(LIGHT_VALUE, reader.readUnsignedBits32(8));
    ASSERT_EQ(MID_VALUE, reader.readUnsignedBits32(8));

    // TODO[Mi-L@]: Should be solved by issue https://github.com/ndsev/zserio-cpp17/issues/36.
    /*
    ASSERT_THROW(
            {
                try
                {
                    zserio::detail::write(writer, Traffic::ZSERIO_REMOVED_HEAVY);
                }
                catch (const zserio::CppRuntimeException& e)
                {
                    ASSERT_STREQ("Trying to write removed enumeration item 'ZSERIO_REMOVED_HEAVY'!", e.what());
                    throw;
                }
            },
            zserio::CppRuntimeException);
    */
}

// TODO[Mi-L@]: Original test contains enumTypeInfo test, make either specialized type info test or place
//              it also here.

TEST_F(RemovedEnumItemTest, stdHash)
{
    // use hardcoded values to check that the hash code is stable

    const Traffic traffic = Traffic::NONE;
    const size_t trafficHash = 852;
    const Traffic equalTraffic = Traffic::NONE;
    const Traffic diffTraffic = Traffic::ZSERIO_REMOVED_HEAVY;
    const size_t diffTrafficHash = 853;

    test_utils::hashTest(traffic, trafficHash, equalTraffic, diffTraffic, diffTrafficHash);
}

} // namespace removed_enum_item
} // namespace enumeration_types
