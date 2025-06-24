#include "gtest/gtest.h"
#include "templates/instantiate_via_import/InstantiateViaImport.h"
#include "templates/instantiate_via_import/pkg/U32.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_via_import
{

using AllocatorType = InstantiateViaImport::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(InstantiateViaImportTest, writeRead)
{
    InstantiateViaImport data;
    data.test32 = pkg::U32{13};
    data.testStr = pkg::Test<StringType>{"test"};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_via_import
} // namespace templates
