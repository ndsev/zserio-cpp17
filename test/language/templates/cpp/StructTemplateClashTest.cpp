#include "gtest/gtest.h"
#include "templates/struct_template_clash/InstantiationNameClash.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace struct_template_clash
{

TEST(StructTemplateClashTest, writeRead)
{
    const TestStruct<zserio::UInt32> testStruct_uint32(
            42, Template<A_B, C>(A_B(1), C(true)), Template<A, B_C>(A(1), B_C("string")));
    InstantiationNameClash data(testStruct_uint32);

    test_utils::writeReadTest(data);
}

} // namespace struct_template_clash
} // namespace templates
