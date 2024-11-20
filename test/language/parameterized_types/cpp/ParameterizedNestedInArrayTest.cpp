#include "gtest/gtest.h"
#include "parameterized_types/parameterized_nested_in_array/Holder.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace parameterized_nested_in_array
{

// note that in the data-view approach we cannot get exception due to inconsistent parameters for this schema
TEST(ParameterizedNestedInArrayTest, writeRead)
{
    Holder holder({{Element{Parameterized{6}}}}, {{Element{Parameterized{6}}}});

    test_utils::writeReadTest(holder);
}

} // namespace parameterized_nested_in_array
} // namespace parameterized_types
