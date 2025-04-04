#include <array>
#include <vector>

#include "array_types/packed_auto_array_struct_with_extern/PackedAutoArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace array_types
{
namespace packed_auto_array_struct_with_extern
{

using allocator_type = PackedAutoArray::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;
using BitBufferType = zserio::BasicBitBuffer<allocator_type>;

class PackedAutoArrayStructWithExternTest : public ::testing::Test
{
protected:
    static PackedAutoArray createData()
    {
        PackedAutoArray packedAutoArray;
        auto& array = packedAutoArray.array;
        for (size_t i = 0; i < UINT8_FIELD.size(); ++i)
        {
            array.emplace_back(UINT32_FIELD.at(i), BitBufferType(EXTERN_FIELD.at(i)), UINT8_FIELD.at(i));
        }

        return packedAutoArray;
    }

    static const std::array<zserio::UInt32, 10> UINT32_FIELD;
    static const std::array<VectorType<uint8_t>, 10> EXTERN_FIELD;
    static const std::array<zserio::UInt8, 10> UINT8_FIELD;

    static constexpr std::string_view BLOB_NAME =
            "language/array_types/packed_auto_array_struct_with_extern.blob";
};

const std::array<zserio::UInt32, 10> PackedAutoArrayStructWithExternTest::UINT32_FIELD = {
        100000, 110000, 120000, 130000, 140000, 150000, 160000, 170000, 180000, 190000};
const std::array<VectorType<uint8_t>, 10> PackedAutoArrayStructWithExternTest::EXTERN_FIELD = {
        VectorType<uint8_t>{0xAB, 0xCD, 0xEF}, VectorType<uint8_t>{0x00}, VectorType<uint8_t>{0x01},
        VectorType<uint8_t>{0x00}, VectorType<uint8_t>{0x01}, VectorType<uint8_t>{0x00},
        VectorType<uint8_t>{0x01}, VectorType<uint8_t>{0x00}, VectorType<uint8_t>{0x01},
        VectorType<uint8_t>{0x00}};
const std::array<zserio::UInt8, 10> PackedAutoArrayStructWithExternTest::UINT8_FIELD = {
        0, 2, 4, 6, 8, 10, 12, 14, 16, 18};

TEST_F(PackedAutoArrayStructWithExternTest, writeRead)
{
    auto data = createData();
    test_utils::writeReadTest(data);
}

TEST_F(PackedAutoArrayStructWithExternTest, writeReadFile)
{
    auto data = createData();
    test_utils::writeReadFileTest(BLOB_NAME, data);
}

} // namespace packed_auto_array_struct_with_extern
} // namespace array_types
