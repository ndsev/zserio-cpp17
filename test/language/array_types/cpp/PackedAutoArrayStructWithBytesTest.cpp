#include <array>
#include <vector>

#include "array_types/packed_auto_array_struct_with_bytes/PackedAutoArray.h"
#include "gtest/gtest.h"
#include "test_utils/WriteReadFileTest.h"
#include "test_utils/WriteReadTest.h"

namespace array_types
{
namespace packed_auto_array_struct_with_bytes
{

using AllocatorType = PackedAutoArray::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, AllocatorType>;
using BytesType = zserio::BasicBytes<AllocatorType>;

class PackedAutoArrayStructWithBytesTest : public ::testing::Test
{
protected:
    PackedAutoArray createData()
    {
        PackedAutoArray data;
        auto& array = data.array;
        for (size_t i = 0; i < UINT8_FIELD.size(); ++i)
        {
            array.emplace_back(UINT32_FIELD.at(i), BYTES_FIELD.at(i), UINT8_FIELD.at(i));
        }

        return data;
    }

    static const std::array<zserio::UInt32, 10> UINT32_FIELD;
    static const std::array<BytesType, 10> BYTES_FIELD;
    static const std::array<zserio::UInt8, 10> UINT8_FIELD;

    static constexpr std::string_view BLOB_NAME =
            "language/array_types/packed_auto_array_struct_with_bytes.blob";
};

const std::array<zserio::UInt32, 10> PackedAutoArrayStructWithBytesTest::UINT32_FIELD = {
        100000, 110000, 120000, 130000, 140000, 150000, 160000, 170000, 180000, 190000};
const std::array<BytesType, 10> PackedAutoArrayStructWithBytesTest::BYTES_FIELD = {BytesType{0xAB, 0xCD, 0xEF},
        BytesType{0x00}, BytesType{0x01}, BytesType{0x00}, BytesType{0x01}, BytesType{0x00}, BytesType{0x01},
        BytesType{0x00}, BytesType{0x01}, BytesType{0x00}};
const std::array<zserio::UInt8, 10> PackedAutoArrayStructWithBytesTest::UINT8_FIELD = {
        0, 2, 4, 6, 8, 10, 12, 14, 16, 18};

TEST_F(PackedAutoArrayStructWithBytesTest, writeRead)
{
    auto data = createData();
    test_utils::writeReadTest(data);
}

TEST_F(PackedAutoArrayStructWithBytesTest, writeReadFile)
{
    auto data = createData();
    test_utils::writeReadFileTest(BLOB_NAME, data);
}

} // namespace packed_auto_array_struct_with_bytes
} // namespace array_types
