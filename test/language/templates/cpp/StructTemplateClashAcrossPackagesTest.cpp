#include "gtest/gtest.h"
#include "templates/struct_template_clash_across_packages/pkg1/InstantiationInPkg1.h"
#include "templates/struct_template_clash_across_packages/pkg2/InstantiationInPkg2.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace struct_template_clash_across_packages
{

namespace pkg1
{

TEST(StructTemplateClashAcrossPackagesTest, writeReadInPkg1)
{
    InstantiationInPkg1 data;
    data.test = test_struct::TestStruct<pkg1::Test>(pkg1::Test(42));

    test_utils::writeReadTest(data);
}

} // namespace pkg1

namespace pkg2
{

TEST(StructTemplateClashAcrossPackagesTest, writeReadInPkg2)
{
    InstantiationInPkg2 data;
    data.test = test_struct::TestStruct<pkg2::Test>(pkg2::Test("string"));

    test_utils::writeReadTest(data);
}

} // namespace pkg2

} // namespace struct_template_clash_across_packages
} // namespace templates
