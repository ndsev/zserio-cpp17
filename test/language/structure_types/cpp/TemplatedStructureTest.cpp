#include "gtest/gtest.h"
#include "structure_types/templated_structure/TemplatedStructure.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace structure_types
{
namespace templated_structure
{

using AllocatorType = TemplatedStructure::allocator_type;
using BytesType = zserio::BasicBytes<AllocatorType>;

class TemplatedStructureTest : public ::testing::Test
{
protected:
    TemplatedStructure makeData()
    {
        TemplatedStructure data;
        data.testStruct2.field4.emplace<TestUnion::Tag::bField>(BytesType{0x10, 0x20});
        data.testStruct3.field1.field4.emplace<TestUnion::Tag::bField>(BytesType{0x30});
        return data;
    }
};

TEST_F(TemplatedStructureTest, writeRead)
{
    auto data = makeData();
    test_utils::writeReadTest(data);
}

TEST_F(TemplatedStructureTest, hash)
{
    auto data = makeData();
    ASSERT_EQ(std::hash<TemplatedStructure>()(data), static_cast<size_t>(1230427740ULL));
}

} // namespace templated_structure
} // namespace structure_types
