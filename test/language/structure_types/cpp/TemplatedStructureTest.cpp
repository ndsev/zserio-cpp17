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

TEST(TemplatedStructureTest, writeRead)
{
    TemplatedStructure data;
    data.testStruct2.field4.emplace<TestUnion::Tag::bField>(BytesType{0x10, 0x20});
    data.testStruct3.field1.field4.emplace<TestUnion::Tag::bField>(BytesType{0x30});
    test_utils::writeReadTest(data);
}

TEST(TemplatedStructureTest, hash)
{
    TemplatedStructure data;

    // #21: std::hash<string> is not stable don't compare it here
    ASSERT_GT(std::hash<TemplatedStructure>()(data), 0);
}

} // namespace templated_structure
} // namespace structure_types
