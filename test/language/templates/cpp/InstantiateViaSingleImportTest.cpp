#include "gtest/gtest.h"
#include "templates/instantiate_via_single_import/InstantiateViaSingleImport.h"
#include "templates/instantiate_via_single_import/pkg/U32.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_via_single_import
{

using AllocatorType = InstantiateViaSingleImport::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(InstantiateViaSingleImportTest, writeRead)
{
    InstantiateViaSingleImport data;
    data.test32 = pkg::U32{13};
    data.testStr = pkg::Test<StringType>{"test"};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_via_single_import
} // namespace templates
