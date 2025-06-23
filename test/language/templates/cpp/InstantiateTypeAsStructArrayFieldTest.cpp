#include "gtest/gtest.h"
#include "templates/instantiate_type_as_struct_array_field/InstantiateTypeAsStructArrayField.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_type_as_struct_array_field
{

TEST(InstantiateTypeAsStructArrayFieldTest, writeRead)
{
    InstantiateTypeAsStructArrayField data;
    data.test = {Test32{13}, Test32{17}, Test32{23}};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_type_as_struct_array_field
} // namespace templates
