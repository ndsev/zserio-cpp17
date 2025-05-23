#include <utility>

#include "array_types/auto_array_bitfield_param/ParameterizedBitfieldLength.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace array_types
{
namespace auto_array_bitfield_param
{

using AllocatorType = ParameterizedBitfieldLength::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class AutoArrayBitfieldParamTest : public ::testing::Test
{
protected:
    static void fillData(ParameterizedBitfieldLength& data)
    {
        VectorType<zserio::DynUInt16>& dynamicBitfieldArray = data.dynamicBitfieldArray;
        for (uint16_t i = 0; i < DYNAMIC_BITFIELD_ARRAY_SIZE; ++i)
        {
            dynamicBitfieldArray.push_back(i);
        }
    }

    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeVarSize(static_cast<uint32_t>(DYNAMIC_BITFIELD_ARRAY_SIZE));

        for (uint16_t i = 0; i < DYNAMIC_BITFIELD_ARRAY_SIZE; ++i)
        {
            writer.writeUnsignedBits32(i, NUM_BITS_PARAM);
        }
    }

    static constexpr std::string_view BLOB_NAME = "language/array_types/auto_array_bitfield_param.blob";
    static constexpr zserio::UInt4 NUM_BITS_PARAM = 9;

private:
    static constexpr size_t DYNAMIC_BITFIELD_ARRAY_SIZE = (1U << 9U) - 1;
};

TEST_F(AutoArrayBitfieldParamTest, writeRead)
{
    ParameterizedBitfieldLength data;
    fillData(data);

    test_utils::writeReadTest(data, NUM_BITS_PARAM);
}

TEST_F(AutoArrayBitfieldParamTest, writeReadFile)
{
    ParameterizedBitfieldLength data;
    fillData(data);

    test_utils::writeReadFileTest(BLOB_NAME, data, NUM_BITS_PARAM);
}

TEST_F(AutoArrayBitfieldParamTest, read)
{
    ParameterizedBitfieldLength data;
    fillData(data);

    test_utils::readTest(writeData, data, NUM_BITS_PARAM);
}

} // namespace auto_array_bitfield_param
} // namespace array_types
