#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "union_types/templated_union/TemplatedUnion.h"
#include "zserio/RebindAlloc.h"

namespace union_types
{
namespace templated_union
{

using AllocatorType = TemplatedUnion::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;
using BytesType = zserio::BasicBytes<AllocatorType>;

using TestUnion1 = TestUnion<zserio::Float32, StringType, Compound, ParameterizedUnion<zserio::VarSize>>;

TEST(TemplatedUnionTest, writeRead)
{
    TemplatedUnion data;
    data.emplace<TemplatedUnion::Tag::testUnion1>().emplace<TestUnion1::Tag::t1Array>();
    test_utils::writeReadTest(data);
}

TEST(TemplatedUnionTest, hash)
{
    TemplatedUnion data;
    data.emplace<TemplatedUnion::Tag::testUnion1>().emplace<TestUnion1::Tag::t1Array>();
    ASSERT_EQ(std::hash<TemplatedUnion>()(data), static_cast<size_t>(32376));
}

} // namespace templated_union
} // namespace union_types
