#include "extended_members/extended_simple_field/Extended.h"
#include "extended_members/extended_simple_field/Original.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace extended_members
{
namespace extended_simple_field
{

using AllocatorType = Extended::allocator_type;

class ExtendedSimpleFieldTest : public ::testing::Test
{
protected:
    static constexpr size_t ORIGINAL_BIT_SIZE = 4 * 8;
    static constexpr size_t EXTENDED_BIT_SIZE = ORIGINAL_BIT_SIZE + 8 * 8;
};

TEST_F(ExtendedSimpleFieldTest, defaultConstructor)
{
    Extended data;
    zserio::View view(data);

    // always present when not read from stream
    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    // default initialized
    ASSERT_EQ(0, data.value);
    ASSERT_EQ(0, data.extendedValue.value());
    ASSERT_EQ(0, view.value());
    ASSERT_EQ(0, view.extendedValue().value());
}

TEST_F(ExtendedSimpleFieldTest, fieldConstructor)
{
    Extended data(zserio::UInt32(42), zserio::NumericLimits<zserio::UInt64>::max());
    zserio::View view(data);

    ASSERT_TRUE(data.extendedValue.isPresent());

    ASSERT_EQ(42, data.value);
    ASSERT_EQ(UINT64_MAX, data.extendedValue.value());
    ASSERT_EQ(42, view.value());
    ASSERT_EQ(UINT64_MAX, view.extendedValue().value());
}

TEST_F(ExtendedSimpleFieldTest, comparisonOperators)
{
    Extended data(zserio::UInt32(42), zserio::NumericLimits<zserio::UInt64>::max());
    Extended equalData(zserio::UInt32(42), zserio::NumericLimits<zserio::UInt64>::max());
    Extended lessThanData(zserio::UInt32(42), zserio::UInt64(zserio::NumericLimits<zserio::UInt64>::max() - 1));

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(ExtendedSimpleFieldTest, bitSizeOf)
{
    Extended data;
    zserio::View view(data);

    ASSERT_EQ(EXTENDED_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(ExtendedSimpleFieldTest, writeReadExtended)
{
    Extended data(zserio::UInt32(42), zserio::NumericLimits<zserio::UInt64>::max());

    test_utils::writeReadTest(data);
}

TEST_F(ExtendedSimpleFieldTest, writeOriginalReadExtended)
{
    Original dataOriginal(zserio::UInt32(42));
    zserio::View viewOriginal(dataOriginal);

    auto bitBuffer = zserio::serialize(dataOriginal);
    Extended readDataExtended;
    zserio::View readViewExtended = zserio::deserialize<Extended>(bitBuffer, readDataExtended);
    ASSERT_FALSE(readViewExtended.extendedValue().isPresent());
    ASSERT_FALSE(readViewExtended.extendedValue());

    // bit size as original
    ASSERT_EQ(ORIGINAL_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended));

    // write as original
    bitBuffer = zserio::serialize(readViewExtended);
    ASSERT_EQ(ORIGINAL_BIT_SIZE, bitBuffer.getBitSize());

    // read original again
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::deserialize<Original>(bitBuffer, readDataOriginal);
    ASSERT_EQ(viewOriginal, readViewOriginal);

    // set and make the value present!
    readDataExtended.extendedValue.value() = UINT64_MAX;
    readDataExtended.extendedValue.setPresent(true);
    ASSERT_TRUE(readViewExtended.extendedValue().isPresent());
    ASSERT_EQ(UINT64_MAX, readViewExtended.extendedValue().value());

    // bit size as extended
    ASSERT_EQ(EXTENDED_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended));

    // writes as extended
    bitBuffer = zserio::serialize(readViewExtended);
    ASSERT_EQ(EXTENDED_BIT_SIZE, bitBuffer.getBitSize());

    test_utils::writeReadTest(readDataExtended);
}

TEST_F(ExtendedSimpleFieldTest, writeExtendedReadOriginal)
{
    Extended dataExtended(42, UINT64_MAX);
    auto bitBuffer = zserio::serialize(dataExtended);
    ASSERT_EQ(EXTENDED_BIT_SIZE, bitBuffer.getBitSize());

    Original readDataOriginal;
    zserio::BitStreamReader reader(bitBuffer);
    zserio::View readViewOriginal = zserio::detail::read(reader, readDataOriginal);
    ASSERT_EQ(dataExtended.value, readViewOriginal.value());
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

TEST_F(ExtendedSimpleFieldTest, stdHash)
{
    Extended data(zserio::UInt32(42), zserio::NumericLimits<zserio::UInt64>::max());
    const size_t dataHash = 33041;
    Extended equalData(zserio::UInt32(42), zserio::NumericLimits<zserio::UInt64>::max());
    Extended diffData(zserio::UInt32(13), zserio::NumericLimits<zserio::UInt64>::max());
    const size_t diffDataHash = 31968;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 33041;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 31968;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace extended_simple_field
} // namespace extended_members
