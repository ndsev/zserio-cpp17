#include "gtest/gtest.h"
#include "parameterized_types/dynamic_bitfield_param/DynamicBitfieldParamHolder.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace dynamic_bitfield_param
{

class DynamicBitfieldParamTest : public ::testing::Test
{
protected:
    static void fillData(DynamicBitfieldParamHolder& data)
    {
        DynamicBitfieldParam& dynamicBitfieldParam = data.dynamicBitfieldParam;
        dynamicBitfieldParam.value = DYNAMIC_BITFIELD_PARAM_VALUE;
        dynamicBitfieldParam.extraValue = DYNAMIC_BITFIELD_PARAM_EXTRA_VALUE;
    }

    static void writeData(zserio::BitStreamWriter& writer)
    {
        zserio::detail::write(writer, LENGTH);
        zserio::detail::write(writer, zserio::DynInt16<LENGTH>(BITFIELD));
        zserio::detail::write(writer, DYNAMIC_BITFIELD_PARAM_VALUE);
        zserio::detail::write(writer, DYNAMIC_BITFIELD_PARAM_EXTRA_VALUE);
    }

private:
    static constexpr zserio::UInt4 LENGTH = 5;
    static constexpr zserio::DynInt16<> BITFIELD = 11;
    static constexpr zserio::UInt16 DYNAMIC_BITFIELD_PARAM_VALUE = 0x0BED;
    static constexpr zserio::UInt32 DYNAMIC_BITFIELD_PARAM_EXTRA_VALUE = 0x0BEDDEAD;
};

TEST_F(DynamicBitfieldParamTest, writeRead)
{
    DynamicBitfieldParamHolder data;
    fillData(data);

    test_utils::writeReadTest(data);
}

TEST_F(DynamicBitfieldParamTest, read)
{
    DynamicBitfieldParamHolder expectedDynamicBitfieldParamHolder;
    fillData(expectedDynamicBitfieldParamHolder);

    test_utils::readTest(writeData, expectedDynamicBitfieldParamHolder);
}

} // namespace dynamic_bitfield_param
} // namespace parameterized_types
