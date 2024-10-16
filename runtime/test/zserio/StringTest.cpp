#include "gtest/gtest.h"
#include "zserio/String.h"

namespace zserio
{

TEST(StringTest, bitSizeOf)
{
    EXPECT_EQ((1 + 1) * 8, detail::bitSizeOf(std::string("T")));
    EXPECT_EQ((1 + 4) * 8, detail::bitSizeOf(std::string("Test")));

    const size_t testStringLength = static_cast<size_t>(1U << 7U);
    std::string testString(testStringLength, '\xAB');
    EXPECT_EQ((2 + testStringLength) * 8, detail::bitSizeOf(testString));
}

} // namespace zserio
