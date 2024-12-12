#include "extended_members/multiple_extended_fields/Extended1.h"
#include "extended_members/multiple_extended_fields/Extended2.h"
#include "extended_members/multiple_extended_fields/Original.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/BitPositionUtil.h"
#include "zserio/ExtendedFieldException.h"

namespace extended_members
{
namespace multiple_extended_fields
{

using AllocatorType = Extended2::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

class MultipleExtendedFieldsTest : public ::testing::Test
{
protected:
    static constexpr std::string_view DEFAULT_EXTENDED_VALUE2 = "test";

    static constexpr size_t ORIGINAL_BIT_SIZE = 4 * 8;
    static constexpr size_t EXTENDED1_BIT_SIZE = ORIGINAL_BIT_SIZE + 4;
    static constexpr size_t EXTENDED2_BIT_SIZE =
            zserio::alignTo(8, EXTENDED1_BIT_SIZE) + 8 + DEFAULT_EXTENDED_VALUE2.size() * 8;
};

TEST_F(MultipleExtendedFieldsTest, defaultConstructor)
{
    Extended2 data;
    zserio::View view(data);

    // always present when not read from stream
    ASSERT_TRUE(data.extendedValue1.isPresent());
    ASSERT_TRUE(view.extendedValue1().isPresent());
    ASSERT_TRUE(data.extendedValue2.isPresent());
    ASSERT_TRUE(view.extendedValue2().isPresent());

    // default initialized
    ASSERT_EQ(0, data.value);
    ASSERT_EQ(0, view.value());
    ASSERT_EQ(0, *data.extendedValue1);
    ASSERT_EQ(0, *view.extendedValue1());
    ASSERT_EQ(DEFAULT_EXTENDED_VALUE2, *data.extendedValue2);
    ASSERT_EQ(DEFAULT_EXTENDED_VALUE2, *view.extendedValue2());
}

TEST_F(MultipleExtendedFieldsTest, fieldConstructor)
{
    Extended2 data(42, 2, StringType("other"));
    zserio::View view(data);

    ASSERT_TRUE(data.extendedValue1.isPresent());
    ASSERT_TRUE(view.extendedValue1().isPresent());
    ASSERT_TRUE(data.extendedValue2.isPresent());
    ASSERT_TRUE(view.extendedValue2().isPresent());

    ASSERT_EQ(42, data.value);
    ASSERT_EQ(42, view.value());
    ASSERT_EQ(2, *data.extendedValue1);
    ASSERT_EQ(2, *view.extendedValue1());
    ASSERT_EQ(StringType("other"), *data.extendedValue2);
    ASSERT_EQ(StringType("other"), *view.extendedValue2());
}

TEST_F(MultipleExtendedFieldsTest, comparisonOperators)
{
    Extended2 data(42, 2, StringType("other"));
    Extended2 equalData(42, 2, StringType("other"));
    Extended2 lessThanData(41, 2, StringType("other"));

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(MultipleExtendedFieldsTest, bitSizeOf)
{
    Extended2 data;
    zserio::View view(data);
    ASSERT_EQ(EXTENDED2_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(MultipleExtendedFieldsTest, writeReadExtended2)
{
    Extended2 data(42, 2, StringType(DEFAULT_EXTENDED_VALUE2));
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED2_BIT_SIZE, bitBuffer.getBitSize());

    Extended2 readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);
    ASSERT_TRUE(readData.extendedValue1.isPresent());
    ASSERT_TRUE(readData.extendedValue2.isPresent());
    ASSERT_TRUE(readView.extendedValue1().isPresent());
    ASSERT_TRUE(readView.extendedValue2().isPresent());
    ASSERT_EQ(data, readData);
    ASSERT_EQ(view, readView);
}

TEST_F(MultipleExtendedFieldsTest, writeOriginalReadExtended2)
{
    Original dataOriginal(42);
    zserio::View viewOriginal(dataOriginal);

    auto bitBuffer = zserio::serialize(dataOriginal);

    Extended2 readDataExtended;
    zserio::View readViewExtended = zserio::deserialize(bitBuffer, readDataExtended);
    ASSERT_FALSE(readDataExtended.extendedValue1.isPresent());
    ASSERT_FALSE(readDataExtended.extendedValue2.isPresent());
    ASSERT_FALSE(readViewExtended.extendedValue1().isPresent());
    ASSERT_FALSE(readViewExtended.extendedValue2().isPresent());

    // extended values are default constructed
    ASSERT_EQ(0, *readViewExtended.extendedValue1());
    ASSERT_EQ(DEFAULT_EXTENDED_VALUE2, *readViewExtended.extendedValue2());

    // bit size as original
    ASSERT_EQ(ORIGINAL_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended));

    // writes as original
    bitBuffer = zserio::serialize(readDataExtended);
    ASSERT_EQ(ORIGINAL_BIT_SIZE, bitBuffer.getBitSize());

    // read original again
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::deserialize(bitBuffer, readDataOriginal);
    ASSERT_EQ(dataOriginal, readDataOriginal);
    ASSERT_EQ(viewOriginal, readViewOriginal);

    // set and make all values present!
    *readDataExtended.extendedValue1 = 2;
    readDataExtended.extendedValue1.setPresent(true);
    readDataExtended.extendedValue2.setPresent(true);

    // bit size as extended2
    ASSERT_EQ(EXTENDED2_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended));

    // writes as extended2
    bitBuffer = zserio::serialize(readDataExtended);
    ASSERT_EQ(EXTENDED2_BIT_SIZE, bitBuffer.getBitSize());
}

TEST_F(MultipleExtendedFieldsTest, writeExtended1ReadExtended2)
{
    Extended1 data1(42, 2);
    zserio::View view1(data1);
    auto bitBuffer = zserio::serialize(data1);
    ASSERT_EQ(EXTENDED1_BIT_SIZE, bitBuffer.getBitSize());

    Extended2 readDataExtended;
    zserio::View readViewExtended = zserio::deserialize(bitBuffer, readDataExtended);
    ASSERT_TRUE(readViewExtended.extendedValue1().isPresent());
    ASSERT_FALSE(readViewExtended.extendedValue2().isPresent());

    ASSERT_EQ(2, *readViewExtended.extendedValue1());
    ASSERT_EQ(DEFAULT_EXTENDED_VALUE2, *readViewExtended.extendedValue2());

    // bit size as extended1
    ASSERT_EQ(EXTENDED1_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended));

    // writes as extended1
    bitBuffer = zserio::serialize(readDataExtended);
    ASSERT_EQ(EXTENDED1_BIT_SIZE, bitBuffer.getBitSize());

    // read extended1 again
    Extended1 readDataExtended1;
    zserio::View readViewExtended1 = zserio::deserialize(bitBuffer, readDataExtended1);
    ASSERT_EQ(data1, readDataExtended1);
    ASSERT_EQ(view1, readViewExtended1);

    // read original
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::deserialize(bitBuffer, readDataOriginal);
    ASSERT_EQ(42, readViewOriginal.value());

    // check actually present field
    ASSERT_TRUE(readDataExtended.extendedValue1.isPresent());

    // set non present field
    *readDataExtended.extendedValue2 = DEFAULT_EXTENDED_VALUE2;
    readDataExtended.extendedValue2.setPresent(true);

    // bit size as extended2
    ASSERT_EQ(EXTENDED2_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended));

    // writes as extended2
    bitBuffer = zserio::serialize(readViewExtended);
    ASSERT_EQ(EXTENDED2_BIT_SIZE, bitBuffer.getBitSize());

    test_utils::writeReadTest(readDataExtended);
}

TEST_F(MultipleExtendedFieldsTest, writeExtended2ReadOriginal)
{
    Extended2 data(42, 2, StringType(DEFAULT_EXTENDED_VALUE2));
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED2_BIT_SIZE, bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::detail::read(reader, readDataOriginal);
    ASSERT_EQ(view.value(), readViewOriginal.value());
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

TEST_F(MultipleExtendedFieldsTest, writeExtended2ReadExtended1)
{
    Extended2 data(42, 2, StringType(DEFAULT_EXTENDED_VALUE2));
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED2_BIT_SIZE, bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Extended1 readDataExtended1;
    zserio::View readViewExtended1 = zserio::detail::read(reader, readDataExtended1);
    ASSERT_EQ(view.value(), readViewExtended1.value());
    ASSERT_EQ(EXTENDED1_BIT_SIZE, reader.getBitPosition());
}

TEST_F(MultipleExtendedFieldsTest, stdHash)
{
    Extended2 data(42, 2, StringType("other"));
    const size_t dataHash = 2328856229;
    Extended2 equalData(42, 2, StringType("other"));
    Extended2 diffData(41, 2, StringType("other"));
    const size_t diffDataHash = 4058097116;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 2328856229;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 4058097116;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

TEST_F(MultipleExtendedFieldsTest, validationError)
{
    Original dataOriginal(42);
    auto bitBuffer = zserio::serialize(dataOriginal);

    Extended2 readDataExtended2;
    zserio::View readViewExtended2 = zserio::deserialize(bitBuffer, readDataExtended2);

    EXPECT_FALSE(readViewExtended2.extendedValue1().isPresent());
    EXPECT_FALSE(readViewExtended2.extendedValue2().isPresent());

    readDataExtended2.extendedValue2.setPresent(true);
    ASSERT_THROW(zserio::serialize(readDataExtended2), zserio::ExtendedFieldException);
}

} // namespace multiple_extended_fields
} // namespace extended_members
