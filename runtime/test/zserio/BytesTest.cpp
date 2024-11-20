#include "gtest/gtest.h"
#include "zserio/Bytes.h"

namespace zserio
{

TEST(BytesTest, bitSizeOf)
{
    EXPECT_EQ((1 + 2) * 8, detail::bitSizeOf(Bytes({0xAB, 0xC0})));
    EXPECT_EQ(8, detail::bitSizeOf(Bytes({})));
}

} // namespace zserio
