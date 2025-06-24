#include "gtest/gtest.h"
#include "templates/struct_templated_type_argument/StructTemplatedTypeArgument.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace struct_templated_type_argument
{

using AllocatorType = StructTemplatedTypeArgument::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(StructTemplatedTypeArgumentTest, writeRead)
{
    StructTemplatedTypeArgument data;
    data.paramHolder = ParamHolder<zserio::UInt32>{42};
    data.parameterized = Parameterized<zserio::UInt32>{StringType{"description"}, 13};

    test_utils::writeReadTest(data);
}

} // namespace struct_templated_type_argument
} // namespace templates
