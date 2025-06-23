#include "gtest/gtest.h"
#include "templates/instantiate_type_imported_as_struct_field/InstantiateTypeImportedAsStructField.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_type_imported_as_struct_field
{

TEST(InstantiateTypeImportedAsStructFieldTest, writeRead)
{
    InstantiateTypeImportedAsStructField data;
    data.test = pkg::Test32{13};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_type_imported_as_struct_field
} // namespace templates
