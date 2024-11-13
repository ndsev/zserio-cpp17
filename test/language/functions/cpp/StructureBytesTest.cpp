#include "functions/structure_bytes/TestStructure.h"
#include "gtest/gtest.h"
#include "zserio/Bytes.h"

namespace functions
{
namespace structure_bytes
{

using AllocatorType = TestStructure::AllocatorType;
using BytesType = zserio::BasicBytes<AllocatorType>;

class StructureBytesTest : public ::testing::Test
{
protected:
    static const BytesType FIELD;
    static const BytesType CHILD_FIELD;
};

const BytesType StructureBytesTest::FIELD = BytesType{{0xAB, 0xE0}};
const BytesType StructureBytesTest::CHILD_FIELD = BytesType{{0xCA, 0xFE}};

TEST_F(StructureBytesTest, getField)
{
    TestStructure data = TestStructure{FIELD, Child{CHILD_FIELD}};
    zserio::View<TestStructure> view(data);
    ASSERT_EQ(FIELD, view.getField());
}

TEST_F(StructureBytesTest, getChildField)
{
    TestStructure data = TestStructure{FIELD, Child{CHILD_FIELD}};
    zserio::View<TestStructure> view(data);
    ASSERT_EQ(CHILD_FIELD, view.getChildField());
}

} // namespace structure_bytes
} // namespace functions
