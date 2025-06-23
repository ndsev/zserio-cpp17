#include "gtest/gtest.h"
#include "templates/instantiate_imported/InstantiateImported.h"
#include "templates/instantiate_imported/pkg/U32.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace instantiate_imported
{

using AllocatorType = InstantiateImported::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(InstantiateImportedTest, writeRead)
{
    InstantiateImported data;
    data.test32 = pkg::U32{13};
    data.testStr = instantiate_imported::Test<StringType>{"test"};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_imported
} // namespace templates
