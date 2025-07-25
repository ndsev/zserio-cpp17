#include "functions/templated_structure_array_param/TemplatedStructureArrayParam.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace functions
{
namespace templated_structure_array_param
{

class TemplatedStructureArrayParamTest : public ::testing::Test
{
protected:
    TemplatedStructureArrayParam makeData()
    {
        TemplatedStructureArrayParam data;
        data.parentStructure.numChildren = 1;
        data.parentStructure.children.emplace_back(zserio::DynUInt64(3));
        return data;
    }
};

TEST_F(TemplatedStructureArrayParamTest, writeRead)
{
    auto data = makeData();
    test_utils::writeReadTest(data);
}

TEST_F(TemplatedStructureArrayParamTest, hash)
{
    auto data = makeData();
    ASSERT_EQ(std::hash<TemplatedStructureArrayParam>()(data), static_cast<size_t>(44304540));
}

} // namespace templated_structure_array_param
} // namespace functions
