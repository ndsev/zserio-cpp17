#include "functions/structure_string/StringPool.h"
#include "functions/structure_string/TestStructure.h"
#include "gtest/gtest.h"

namespace functions
{
namespace structure_string
{

using namespace std::literals;

TEST(StructureStringTest, getPoolConst)
{
    TestStructure data;
    zserio::View view(data);
    ASSERT_EQ("POOL_CONST"sv, view.getPoolConst());
}

TEST(StructureStringTest, getPoolFiled)
{
    TestStructure data;
    zserio::View view(data);
    ASSERT_EQ("POOL_FIELD"sv, view.getPoolField());
}

TEST(StructureStringTest, getConst)
{
    TestStructure data;
    zserio::View view(data);
    ASSERT_EQ("CONST"sv, view.getConst());
}

TEST(StructureStringTest, getField)
{
    TestStructure data;
    zserio::View view(data);
    ASSERT_EQ("FIELD"sv, view.getField());
}

} // namespace structure_string
} // namespace functions
