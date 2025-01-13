#include <limits>

#include "builtin_types/bitfield_uint64_length/Container.h"
#include "gtest/gtest.h"
#include "zserio/SerializeUtil.h"

namespace builtin_types
{
namespace bitfield_uint64_length
{

TEST(BitFieldUIn64LengthTest, bitSizeOf)
{
    Container container;
    const uint64_t bitFieldLength = 33;
    container.length = bitFieldLength;
    container.unsignedBitField = std::numeric_limits<uint32_t>::max() + static_cast<uint64_t>(1);
    container.signedBitField = std::numeric_limits<int32_t>::max() + static_cast<int64_t>(1);
    const size_t expectedBitSizeOfContainer = 64 + bitFieldLength + bitFieldLength;

    zserio::View view(container);
    ASSERT_EQ(expectedBitSizeOfContainer, zserio::detail::bitSizeOf(view));
}

TEST(BitFieldUIn64LengthTest, writeRead)
{
    Container container;
    const uint64_t bitFieldLength = 33;
    container.length = bitFieldLength;
    container.unsignedBitField = std::numeric_limits<uint32_t>::max() + static_cast<uint64_t>(1);
    container.signedBitField = std::numeric_limits<int32_t>::max() + static_cast<int64_t>(1);

    zserio::View view(container);

    const std::string fileName = "language/builtin_types/bit_field_uint64_length.blob";
    zserio::serializeToFile(view, fileName);

    Container readContainer;
    const zserio::View readView = zserio::deserializeFromFile(fileName, readContainer);
    ASSERT_EQ(view, readView);
}

} // namespace bitfield_uint64_length
} // namespace builtin_types
