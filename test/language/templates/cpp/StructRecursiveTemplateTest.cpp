#include "gtest/gtest.h"
#include "templates/struct_recursive_template/StructRecursiveTemplate.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace struct_recursive_template
{

using AllocatorType = StructRecursiveTemplate::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(StructRecursiveTemplateTest, writeRead)
{
    StructRecursiveTemplate data;
    data.compound1 = Compound<Compound<zserio::UInt32>>{Compound<zserio::UInt32>{42}};
    data.compound2 = Compound<Compound<Compound<StringType>>>{
            Compound<Compound<StringType>>{Compound<StringType>{StringType{"string"}}}};

    test_utils::writeReadTest(data);
}

} // namespace struct_recursive_template
} // namespace templates
