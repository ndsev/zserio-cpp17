#include "gtest/gtest.h"
#include "parameterized_types/dynamic_bitfield_param/DynamicBitfieldParamHolder.h"
#include "test_utils/ReadTest.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"

namespace parameterized_types
{
namespace dynamic_bitfield_param
{

class DynamicBitfieldParamTest : public ::testing::Test
{
protected:
    void fillData(DynamicBitfieldParamHolder& dynamicBitfieldParamHolder)
    {
        DynamicBitfieldParam& dynamicBitfieldParam = dynamicBitfieldParamHolder.dynamicBitfieldParam;
        dynamicBitfieldParam.value = DYNAMIC_BITFIELD_PARAM_VALUE;
        dynamicBitfieldParam.extraValue = DYNAMIC_BITFIELD_PARAM_EXTRA_VALUE;
    }

    void writeToStream(zserio::BitStreamWriter& writer)
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
    DynamicBitfieldParamHolder dynamicBitfieldParamHolder;
    fillData(dynamicBitfieldParamHolder);

    test_utils::writeReadTest(dynamicBitfieldParamHolder);
}

TEST_F(DynamicBitfieldParamTest, read)
{
    zserio::BitBuffer bitBuffer(57);
    zserio::BitStreamWriter writer(bitBuffer);
    writeToStream(writer);

    DynamicBitfieldParamHolder expectedDynamicBitfieldParamHolder;
    fillData(expectedDynamicBitfieldParamHolder);
    test_utils::readTest(writer, expectedDynamicBitfieldParamHolder);
}

} // namespace dynamic_bitfield_param
} // namespace parameterized_types
