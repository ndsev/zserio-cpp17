#include "gtest/gtest.h"
#include "templates/union_templated_field/UnionTemplatedField.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace union_templated_field
{

TEST(UnionTemplatedFieldTest, writeRead)
{
    UnionTemplatedField data;
    {
        TemplatedUnion<zserio::UInt16, zserio::UInt32> uintUnion;
        uintUnion.emplace<TemplatedUnion<zserio::UInt16, zserio::UInt32>::Tag::field2>(42);
        data.uintUnion = uintUnion;
    }
    {
        TemplatedUnion<zserio::Float32, zserio::Float64> floatUnion;
        floatUnion.emplace<TemplatedUnion<zserio::Float32, zserio::Float64>::Tag::field2>(42.0);
        data.floatUnion = floatUnion;
    }
    {
        TemplatedUnion<Compound<zserio::UInt16>, Compound<zserio::UInt32>> compoundUnion;
        compoundUnion.emplace<TemplatedUnion<Compound<zserio::UInt16>, Compound<zserio::UInt32>>::Tag::field3>(
                Compound<Compound<zserio::UInt16>>{Compound<zserio::UInt16>{13}});
        data.compoundUnion = std::move(compoundUnion);
    }

    test_utils::writeReadTest(data);
}

} // namespace union_templated_field
} // namespace templates
