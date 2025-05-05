#include "gtest/gtest.h"
#include "structure_types/empty_structure/EmptyStructure.h"
#include "test_utils/TestUtility.h"

namespace structure_types
{
namespace empty_structure
{

using AllocatorType = EmptyStructure::allocator_type;

TEST(EmptyStructureTest, emptyConstructor)
{
    {
        EmptyStructure data;
        (void)data;
    }
    {
        EmptyStructure data = {};
        (void)data;
    }
    {
        EmptyStructure data(AllocatorType{});
        (void)data;
    }
    {
        EmptyStructure data;
        zserio::View view(data);
        (void)view;
    }
}

TEST(EmptyStructureTest, comparisonOperators)
{
    EmptyStructure data;
    EmptyStructure equalData;
    test_utils::comparisonOperatorsTest(data, equalData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    test_utils::comparisonOperatorsTest(view, equalView);
}

TEST(EmptyStructureTest, validate)
{
    EmptyStructure data;
    zserio::View view(data);
    EXPECT_NO_THROW(zserio::detail::validate(view));
}

TEST(EmptyStructureTest, bitSizeOf)
{
    EmptyStructure data;
    zserio::View view(data);
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view));
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view, 1));
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view, 100));
}

TEST(EmptyStructureTest, read)
{
    EmptyStructure expectedReadData;
    test_utils::readTest(
            [](zserio::BitStreamWriter&) {
            },
            expectedReadData);
}

TEST(EmptyStructureTest, writeRead)
{
    EmptyStructure data;
    test_utils::writeReadTest(data);
}

TEST(EmptyStructureTest, stdHash)
{
    EmptyStructure data;
    const size_t dataHash = 23; // hardcoded value to check that the hash code is stable
    EmptyStructure equalData;
    test_utils::hashTest(data, dataHash, equalData);

    zserio::View view(data);
    const size_t viewHash = dataHash;
    zserio::View equalView(equalData);
    test_utils::hashTest(view, viewHash, equalView);
}

} // namespace empty_structure
} // namespace structure_types
