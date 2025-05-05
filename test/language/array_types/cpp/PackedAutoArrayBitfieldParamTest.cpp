#include "array_types/packed_auto_array_bitfield_param/ParameterizedBitfieldLength.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace array_types
{
namespace packed_auto_array_bitfield_param
{

using AllocatorType = ParameterizedBitfieldLength::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class PackedAutoArrayBitfieldParamTest : public ::testing::Test
{
protected:
    static void fillData(ParameterizedBitfieldLength& parameterizedBitfieldLength)
    {
        VectorType<zserio::DynUInt16<>>& dynamicBitfieldArray =
                parameterizedBitfieldLength.dynamicBitfieldArray;
        for (uint16_t i = 0; i < DYNAMIC_BITFIELD_ARRAY_SIZE; ++i)
        {
            dynamicBitfieldArray.push_back(i);
        }
    }

    static constexpr std::string_view BLOB_NAME = "language/array_types/packed_auto_array_bitfield_param.blob";
    static constexpr zserio::UInt4 NUM_BITS_PARAM = 9;

private:
    static constexpr uint16_t DYNAMIC_BITFIELD_ARRAY_SIZE = (1U << 9U) - 1;
};

TEST_F(PackedAutoArrayBitfieldParamTest, writeRead)
{
    ParameterizedBitfieldLength parameterizedBitfieldLength;
    fillData(parameterizedBitfieldLength);

    test_utils::writeReadTest(parameterizedBitfieldLength, NUM_BITS_PARAM);
}

TEST_F(PackedAutoArrayBitfieldParamTest, writeReadFile)
{
    ParameterizedBitfieldLength parameterizedBitfieldLength;
    fillData(parameterizedBitfieldLength);

    test_utils::writeReadFileTest(BLOB_NAME, parameterizedBitfieldLength, NUM_BITS_PARAM);
}

} // namespace packed_auto_array_bitfield_param
} // namespace array_types
