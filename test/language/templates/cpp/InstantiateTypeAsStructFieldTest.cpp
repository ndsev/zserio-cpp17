#include "gtest/gtest.h"
#include "templates/instantiate_type_as_struct_field/InstantiateTypeAsStructField.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_type_as_struct_field
{

TEST(InstantiateTypeAsStructFieldTest, writeRead)
{
    InstantiateTypeAsStructField data;
    data.test = Test32{13};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_type_as_struct_field
} // namespace templates
