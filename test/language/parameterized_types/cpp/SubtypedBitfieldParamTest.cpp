#include "gtest/gtest.h"
#include "parameterized_types/subtyped_bitfield_param/SubtypedBitfieldParamHolder.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace subtyped_bitfield_param
{

class SubtypedBitfieldParamTest : public ::testing::Test
{
protected:
    static void fillData(SubtypedBitfieldParamHolder& data)
    {
        SubtypedBitfieldParam& subtypedBitfieldParam = data.subtypedBitfieldParam;
        subtypedBitfieldParam.value = VALUE;
        subtypedBitfieldParam.extraValue = EXTRA_VALUE;
    }

    static void writeData(zserio::BitStreamWriter& writer, zserio::UInt16 value, zserio::UInt32 extraValue)
    {
        zserio::detail::write(writer, value);
        zserio::detail::write(writer, extraValue);
    }

    static constexpr ParamType PARAM = 11;
    static constexpr zserio::UInt16 VALUE = 0x0BED;
    static constexpr zserio::UInt32 EXTRA_VALUE = 0x0BEDDEAD;
};

TEST_F(SubtypedBitfieldParamTest, writeRead)
{
    SubtypedBitfieldParamHolder data;
    fillData(data);

    test_utils::writeReadTest(data);
}

TEST_F(SubtypedBitfieldParamTest, read)
{
    SubtypedBitfieldParamHolder data;
    fillData(data);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, VALUE, EXTRA_VALUE), data);
}

} // namespace subtyped_bitfield_param
} // namespace parameterized_types
