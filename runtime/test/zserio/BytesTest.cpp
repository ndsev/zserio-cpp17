#include "gtest/gtest.h"
#include "zserio/Bytes.h"

namespace zserio
{

TEST(BytesTest, bitSizeOf)
{
    EXPECT_EQ((1 + 2) * 8, detail::bitSizeOf(std::vector<uint8_t>({0xAB, 0xC0})));
    EXPECT_EQ(8, detail::bitSizeOf(std::vector<uint8_t>({})));
}

} // namespace zserio
