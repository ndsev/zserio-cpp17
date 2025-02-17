#include "functions/structure_extern/TestStructure.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace functions
{
namespace structure_extern
{

using AllocatorType = TestStructure::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;
using BitBufferType = zserio::BasicBitBuffer<AllocatorType>;

class StructureExternTest : public ::testing::Test
{
protected:
    static const BitBufferType FIELD;
    static const BitBufferType CHILD_FIELD;
};

const BitBufferType StructureExternTest::FIELD = BitBufferType{VectorType<uint8_t>{{0xAB, 0xE0}}, 11};
const BitBufferType StructureExternTest::CHILD_FIELD = BitBufferType{VectorType<uint8_t>{{0xCA, 0xFE}}, 15};

TEST_F(StructureExternTest, getField)
{
    TestStructure data = TestStructure{FIELD, Child{CHILD_FIELD}};
    zserio::View view(data);

    ASSERT_EQ(FIELD, view.getField());
}

TEST_F(StructureExternTest, getChildField)
{
    TestStructure data = TestStructure{FIELD, Child{CHILD_FIELD}};
    zserio::View view(data);

    ASSERT_EQ(CHILD_FIELD, view.getChildField());
}

} // namespace structure_extern
} // namespace functions
