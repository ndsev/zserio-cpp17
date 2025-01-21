#include "gtest/gtest.h"
#include "package_name_conflict/PackageNameConflictImported.h"
#include "package_name_conflict/PackageNameConflictLocal.h"
#include "test_utils/WriteReadTest.h"

namespace package_name_conflict
{

TEST(PackageNameConflictTest, packageNameConflictLocal)
{
    // just test that PackageNameConflictLocal includes correct Blob
    PackageNameConflictLocal data{Blob{13}};

    test_utils::writeReadTest(data);
}

TEST(PackageNameConflictTest, packageNameConflictImported)
{
    // just test that PackageNameConflictImported includes correct Blob
    PackageNameConflictImported data{::package_name_conflict::package_name_conflict::Blob{"test"}};

    test_utils::writeReadTest(data);
}

} // namespace package_name_conflict
