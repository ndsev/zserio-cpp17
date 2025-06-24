#include "company/appl/SimpleStructure.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace company
{
namespace appl
{
namespace simple_structure
{

TEST(SetTopLevelPackageTest, bitSizeOf)
{
    SimpleStructure data;
    zserio::View view(data);
    ASSERT_EQ(3 + 8 + 7 + 8 + 0 + 1 + 0, zserio::detail::bitSizeOf(view));

    data.simpleTemplate =
            SimpleTemplate<set_top_level_package::enumeration::Enumeration>(true, zserio::UInt5(0x1F));
    ASSERT_EQ(3 + 8 + 7 + 8 + 0 + 1 + 5, zserio::detail::bitSizeOf(view));
}

TEST(SetTopLevelPackageTest, writeRead)
{
    SimpleStructure data;
    data.numberA = 0x07;
    data.numberB = 43;
    data.numberC = 0x7F;
    data.simpleTemplate =
            SimpleTemplate<set_top_level_package::enumeration::Enumeration>(true, zserio::UInt5(0x1F));

    test_utils::writeReadTest(data);
}

} // namespace simple_structure
} // namespace appl
} // namespace company
