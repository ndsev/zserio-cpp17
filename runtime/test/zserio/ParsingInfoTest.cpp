#include "gtest/gtest.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/ParsingInfo.h"

namespace zserio
{

TEST(ParsingInfoTest, bitPosition)
{
    ParsingInfo pinfo;
    ASSERT_THROW(pinfo.getBitPosition(), CppRuntimeException);
    pinfo.setBitPosition(111);
    ASSERT_EQ(pinfo.getBitPosition(), 111);
}

TEST(ParsingInfoTest, bitSize)
{
    ParsingInfo pinfo;
    ASSERT_THROW(pinfo.getBitSize(), CppRuntimeException);
    ASSERT_THROW(pinfo.setEndBitPosition(11), CppRuntimeException);
    pinfo.setBitPosition(111);
    ASSERT_THROW(pinfo.setEndBitPosition(11), CppRuntimeException);
    pinfo.setEndBitPosition(333);
    ASSERT_EQ(pinfo.getBitSize(), 222);
}

} // namespace zserio
