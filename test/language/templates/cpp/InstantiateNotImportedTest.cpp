#include "gtest/gtest.h"
#include "templates/instantiate_not_imported/InstantiateNotImported.h"
#include "templates/instantiate_not_imported/pkg/U32.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace instantiate_not_imported
{

using AllocatorType = InstantiateNotImported::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(InstantiateNotImportedTest, writeRead)
{
    InstantiateNotImported data;
    data.test32 = pkg::Test<zserio::UInt32>{13};
    data.testStr = pkg::Test<StringType>{"test"};

    test_utils::writeReadTest(data);
}

TEST(InstantiateNotImportedTest, instantiationInPkg)
{
    pkg::U32 u32{13};
    ASSERT_EQ(13, u32.value); // just check that U32 exists in pkg
}

} // namespace instantiate_not_imported
} // namespace templates
