#include "array_types/packed_auto_array_removed_enum_item/PackedAutoArrayRemovedEnumItem.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace array_types
{
namespace packed_auto_array_removed_enum_item
{

class PackedAutoArrayRemovedEnumItemTest : public ::testing::Test
{
protected:
    PackedAutoArrayRemovedEnumItem createData()
    {
        PackedAutoArrayRemovedEnumItem packedAutoArrayRemovedEnumItem;
        auto& packedArray = packedAutoArrayRemovedEnumItem.packedArray;
        packedArray.push_back(Traffic::NONE);
        packedArray.push_back(Traffic::LIGHT);
        packedArray.push_back(Traffic::MID);

        return packedAutoArrayRemovedEnumItem;
    }

    static constexpr std::string_view BLOB_NAME =
            "language/array_types/packed_auto_array_removed_enum_item.blob";
};

TEST_F(PackedAutoArrayRemovedEnumItemTest, writeReadFile)
{
    auto data = createData();
    test_utils::writeReadFileTest(BLOB_NAME, data);
}

// TODO[Mi-L@]: Enable when implemented!
/*
TEST_F(PackedAutoArrayRemovedEnumItemTest, writeRemovedException)
{
    PackedAutoArrayRemovedEnumItem packedAutoArrayRemovedEnumItem;
    auto& packedArray = packedAutoArrayRemovedEnumItem.packedArray;
    packedArray.push_back(Traffic::NONE);
    packedArray.push_back(Traffic::LIGHT);
    packedArray.push_back(Traffic::MID);
    packedArray.push_back(Traffic::ZSERIO_REMOVED_HEAVY);

    ASSERT_THROW(zserio::serialize(packedAutoArrayRemovedEnumItem), zserio::ArrayLengthException);
}
*/

} // namespace packed_auto_array_removed_enum_item
} // namespace array_types
