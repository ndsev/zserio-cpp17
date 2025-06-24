#include "gtest/gtest.h"
#include "subtypes/subtype_imported/SubtypeImported.h"
#include "test_utils/TestUtility.h"

namespace subtypes
{
namespace subtype_imported
{

TEST(SubtypeImportedTest, writeRead)
{
    SubtypeImported data{pkg::SubTest{42}};
    test_utils::writeReadTest(data);
}

} // namespace subtype_imported
} // namespace subtypes
