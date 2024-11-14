#include <vector>

#include "functions/structure_param/MetresConverter.h"
#include "functions/structure_param/MetresConverterCaller.h"
#include "gtest/gtest.h"
#include "test_utils/ReadTest.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/BitStreamWriter.h"

namespace functions
{
namespace structure_param
{

class StructureParamTest : public ::testing::Test
{
protected:
    void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(VALUE_A, 16);
        writer.writeUnsignedBits32(CONVERTED_CM_VALUE, 16);
    }

    void fillData(MetresConverterCaller& data)
    {
        MetresConverter& metresConverter = data.metresConverter;
        metresConverter.valueA = VALUE_A;
        data.centimeters = CONVERTED_CM_VALUE;
    }

    static const uint16_t VALUE_A = 0xABCD;
    static const uint16_t M_VALUE_TO_CONVERT = 2;
    static const uint16_t CONVERTED_CM_VALUE = M_VALUE_TO_CONVERT * 100;
};

TEST_F(StructureParamTest, checkMetresConverterCaller)
{
    MetresConverterCaller data;
    fillData(data);
    zserio::View<MetresConverterCaller> view(data);

    const uint16_t expectedCm = CONVERTED_CM_VALUE;
    ASSERT_EQ(expectedCm, view.centimeters());

    zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
    zserio::BitBuffer expectedBitBuffer = zserio::BitBuffer(bitSize);
    zserio::BitStreamWriter expectedWriter(expectedBitBuffer);
    writeData(expectedWriter);

    test_utils::readTest(expectedWriter, data);
    test_utils::writeReadTest(data);
}

} // namespace structure_param
} // namespace functions
