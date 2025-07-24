#include "gtest/gtest.h"
#include "parameterized_types/parameterized_nested_in_array_in_template/Holder.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace parameterized_nested_in_array_in_template
{

using ParameterizedType = Parameterized<zserio::UInt8>;
using ElementType = Element<Parameterized<zserio::UInt8>>;

// note that in the data-view approach we cannot get exception due to inconsistent parameters for this schema
TEST(ParameterizedNestedInArrayInTemplateTest, writeRead)
{
    Holder holder({{ElementType{ParameterizedType{6}}}, {ElementType{ParameterizedType{6}}}});

    test_utils::writeReadTest(holder);
}

TEST(ParameterizedNestedInArrayInTemplateTest, hashTest)
{
    Holder holder({{ElementType{ParameterizedType{6}}}, {ElementType{ParameterizedType{6}}}});

    ASSERT_EQ(std::hash<Holder>()(holder), static_cast<size_t>(97242));
}

} // namespace parameterized_nested_in_array_in_template
} // namespace parameterized_types
