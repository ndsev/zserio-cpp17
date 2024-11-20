#include <array>

#include "array_types/packed_auto_array_struct_with_unpacked_field/PackedAutoArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace array_types
{
namespace packed_auto_array_struct_with_unpacked_field
{

class PackedAutoArrayStructWithUnpackedFieldTest : public ::testing::Test
{
protected:
    static PackedAutoArray createData()
    {
        PackedAutoArray packedAutoArray;
        auto& array = packedAutoArray.array;
        for (size_t i = 0; i < UINT8_FIELD.size(); ++i)
        {
            array.emplace_back(UINT8_FIELD.at(i), UNPACKED_FIELD.at(i));
        }

        return packedAutoArray;
    }

    static const std::array<uint8_t, 10> UINT8_FIELD;
    static const std::array<uint64_t, 10> UNPACKED_FIELD;

    static constexpr uint8_t UINT8_MAX_BIT_NUMBER = 2;
    static constexpr size_t PACKED_AUTO_ARRAY_BIT_SIZE = //
            8 + // auto array size: varsize
            1 + // uint8Field packing descriptor: isPacked (true)
            6 + // uint8Field is packed: maxBitNumber
            1 + // unpackedField packing descriptor: isPacked (false)
            8 + // UINT8_FIELD[0]
            32 + // UNPACKED_FIELD[0] (4 bytes for the first value)
            9 * (UINT8_MAX_BIT_NUMBER + 1) + // deltas for uint8Field values
            9 * 8; // unpackedField varuint values (1 byte)

    static constexpr std::string_view BLOB_NAME =
            "language/array_types/packed_auto_array_struct_with_unpacked_field.blob";
};

const std::array<uint8_t, 10> PackedAutoArrayStructWithUnpackedFieldTest::UINT8_FIELD = {
        0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
const std::array<uint64_t, 10> PackedAutoArrayStructWithUnpackedFieldTest::UNPACKED_FIELD = {
        5000000, 0, 1, 0, 1, 0, 1, 0, 1, 0};

TEST_F(PackedAutoArrayStructWithUnpackedFieldTest, bitSizeOf)
{
    auto data = createData();
    const zserio::View<decltype(data)> view(data);
    ASSERT_EQ(PACKED_AUTO_ARRAY_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(PackedAutoArrayStructWithUnpackedFieldTest, writeReadFile)
{
    auto data = createData();
    test_utils::writeReadFileTest(BLOB_NAME, data);
}

} // namespace packed_auto_array_struct_with_unpacked_field
} // namespace array_types
