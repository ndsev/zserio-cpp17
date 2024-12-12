#include "extended_members/extended_empty_structure/Extended.h"
#include "extended_members/extended_empty_structure/Original.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace extended_members
{
namespace extended_empty_structure
{

class ExtendedEmptyStructureTest : public ::testing::Test
{
protected:
    static constexpr zserio::BitSize ORIGINAL_BIT_SIZE = 0;
    static constexpr zserio::BitSize EXTENDED_BIT_SIZE = ORIGINAL_BIT_SIZE + 4 * 8; // size of uint32_t
};

TEST_F(ExtendedEmptyStructureTest, defaultConstructor)
{
    Extended data;
    zserio::View view(data);

    // always present when not read from stream
    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    // default initialized
    ASSERT_EQ(0, *data.extendedValue);
    ASSERT_EQ(0, *view.extendedValue());
}

TEST_F(ExtendedEmptyStructureTest, fieldConstructor)
{
    Extended data(UINT32_MAX);
    zserio::View view(data);

    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    ASSERT_EQ(UINT32_MAX, *data.extendedValue);
    ASSERT_EQ(UINT32_MAX, *view.extendedValue());
}

TEST_F(ExtendedEmptyStructureTest, comparisonOperators)
{
    Extended data(UINT32_MAX);
    Extended equalData(UINT32_MAX);
    Extended lessThanData(UINT32_MAX - 1);

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(ExtendedEmptyStructureTest, bitSizeOf)
{
    Extended data;
    zserio::View view(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(ExtendedEmptyStructureTest, writeReadExtended)
{
    Extended data(UINT32_MAX);
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE, bitBuffer.getBitSize());

    Extended readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);
    ASSERT_TRUE(readView.extendedValue().isPresent());
    ASSERT_EQ(data, readData);
    ASSERT_EQ(view, readView);
}

TEST_F(ExtendedEmptyStructureTest, writeOriginalReadExtended)
{
    Original dataOriginal;
    zserio::View viewOriginal(dataOriginal);

    auto bitBuffer = zserio::serialize(dataOriginal);

    Extended readDataExtended;
    zserio::View readViewExtended = zserio::deserialize(bitBuffer, readDataExtended);
    ASSERT_FALSE(readViewExtended.extendedValue().isPresent());

    // extended value is default constructed
    ASSERT_EQ(0, *readViewExtended.extendedValue());

    // bit size as original
    ASSERT_EQ(ORIGINAL_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended));

    // writes as original
    bitBuffer = zserio::serialize(readViewExtended);
    ASSERT_EQ(ORIGINAL_BIT_SIZE, bitBuffer.getBitSize());

    // read original again
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::deserialize(bitBuffer, readDataOriginal);
    ASSERT_EQ(dataOriginal, readDataOriginal);
    ASSERT_EQ(viewOriginal, readViewOriginal);

    // set and make the value present!
    *readDataExtended.extendedValue = UINT32_MAX;
    readDataExtended.extendedValue.setPresent(true);
    ASSERT_TRUE(readViewExtended.extendedValue().isPresent());
    ASSERT_EQ(UINT32_MAX, *readDataExtended.extendedValue);
    ASSERT_EQ(UINT32_MAX, *readViewExtended.extendedValue());

    // bit size as extended
    ASSERT_EQ(EXTENDED_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended));

    // writes as extended
    bitBuffer = zserio::serialize(readViewExtended);
    ASSERT_EQ(EXTENDED_BIT_SIZE, bitBuffer.getBitSize());

    test_utils::writeReadTest(readDataExtended);
}

TEST_F(ExtendedEmptyStructureTest, writeExtendedReadOriginal)
{
    Extended data(UINT32_MAX);
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE, bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Original readDataOriginal;
    (void)zserio::detail::read(reader, readDataOriginal);
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

TEST_F(ExtendedEmptyStructureTest, stdHash)
{
    Extended data(UINT32_MAX);
    const size_t dataHash = 850;
    Extended equalData(UINT32_MAX);
    Extended diffData(UINT32_MAX - 1);
    const size_t diffDataHash = 849;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 850;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 849;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace extended_empty_structure
} // namespace extended_members
