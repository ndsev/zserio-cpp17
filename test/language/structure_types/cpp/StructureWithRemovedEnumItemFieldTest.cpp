#include "gtest/gtest.h"
#include "structure_types/structure_with_removed_enum_item_field/StructureWithRemovedEnumItemField.h"
#include "test_utils/TestUtility.h"

namespace structure_types
{
namespace structure_with_removed_enum_item_field
{

TEST(StructureWithRemovedEnumItemField, writeRead)
{
    StructureWithRemovedEnumItemField data(Enumeration::VALID);

    test_utils::writeReadTest(data);
}

TEST(StructureWithRemovedEnumItemFieldTest, setter)
{
    StructureWithRemovedEnumItemField data(Enumeration::ZSERIO_REMOVED_REMOVED);
    ASSERT_EQ(Enumeration::ZSERIO_REMOVED_REMOVED, data.enumeration);

    zserio::View view(data);
    ASSERT_EQ(Enumeration::ZSERIO_REMOVED_REMOVED, view.enumeration());
}

TEST(StructureWithRemovedEnumItemFieldTest, writeValid)
{
    StructureWithRemovedEnumItemField data(Enumeration::VALID);
    ASSERT_NO_THROW(zserio::serialize(data));
}

TEST(StructureWithRemovedEnumItemFieldTest, writeRemovedException)
{
    StructureWithRemovedEnumItemField data(Enumeration::ZSERIO_REMOVED_REMOVED);
    // TODO[mikir]: Should be solved by issue https://github.com/ndsev/zserio-cpp17/issues/36.
    // ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);
}

// TODO[mikir]: Original test contains enumTypeInfo test, make either specialized type info test or place
//              it also here.
// TEST(StructureWithRemovedEnumItemFieldTest, toJsonString)
//{
//    StructureWithRemovedEnumItemField data(Enumeration::ZSERIO_REMOVED_REMOVED);
//    string_type json = zserio::toJsonString(data);
//    ASSERT_EQ("{\n    \"enumeration\": \"REMOVED\"\n}", json);
//}

// TEST(StructureWithRemovedEnumItemFieldTest, fromJsonString)
//{
//     auto data =
//             zserio::fromJsonString<StructureWithRemovedEnumItemField>("{\n    \"enumeration\":
//             \"REMOVED\"\n}");
//     ASSERT_EQ(Enumeration::ZSERIO_REMOVED_REMOVED, data.getEnumeration());
// }

} // namespace structure_with_removed_enum_item_field
} // namespace structure_types
