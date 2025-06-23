#include "gtest/gtest.h"
#include "templates/instantiate_only_nested/InstantiateOnlyNested.h"
#include "templates/instantiate_only_nested/N32.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_only_nested
{

TEST(InstantiateOnlyNestedTest, writeRead)
{
    InstantiateOnlyNested data;
    data.test32 = pkg::Test<zserio::UInt32>{N32{13}};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_only_nested
} // namespace templates
