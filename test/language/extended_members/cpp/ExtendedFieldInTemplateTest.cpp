#include "extended_members/extended_field_in_template/ExtendedCompound.h"
#include "extended_members/extended_field_in_template/ExtendedSimple.h"
#include "extended_members/extended_field_in_template/Original.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace extended_members
{
namespace extended_field_in_template
{

class ExtendedFieldInTemplateTest : public ::testing::Test
{
protected:
    static constexpr zserio::BitSize ORIGINAL_BIT_SIZE = 4 * 8;
    static constexpr zserio::BitSize EXTENDED_BIT_SIZE = ORIGINAL_BIT_SIZE + 4 * 8;
};

TEST_F(ExtendedFieldInTemplateTest, defaultConstructorSimple)
{
    ExtendedSimple data;
    zserio::View view(data);

    // always present when not read from stream
    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    // default initialized
    ASSERT_EQ(0, data.value);
    ASSERT_EQ(0, view.value());
    ASSERT_EQ(0, *data.extendedValue);
    ASSERT_EQ(0, *view.extendedValue());
}

TEST_F(ExtendedFieldInTemplateTest, defaultConstructorCompound)
{
    ExtendedCompound data;
    zserio::View view(data);

    // always present when not read from stream
    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    // default initialized
    ASSERT_EQ(0, data.value);
    ASSERT_EQ(0, view.value());
    ASSERT_EQ(0, data.extendedValue->field);
    ASSERT_EQ(0, view.extendedValue()->field());
}

TEST_F(ExtendedFieldInTemplateTest, fieldConstructorSimple)
{
    ExtendedSimple data(42, UINT32_MAX);
    zserio::View view(data);

    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    ASSERT_EQ(42, data.value);
    ASSERT_EQ(42, view.value());
    ASSERT_EQ(UINT32_MAX, *data.extendedValue);
    ASSERT_EQ(UINT32_MAX, *view.extendedValue());
}

TEST_F(ExtendedFieldInTemplateTest, fieldConstructorCompound)
{
    ExtendedCompound data(42, Compound(UINT32_MAX));
    zserio::View view(data);

    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    ASSERT_EQ(42, data.value);
    ASSERT_EQ(42, view.value());
    ASSERT_EQ(UINT32_MAX, data.extendedValue->field);
    ASSERT_EQ(UINT32_MAX, view.extendedValue()->field());
}

TEST_F(ExtendedFieldInTemplateTest, comparisonOperatorsSimple)
{
    ExtendedSimple data(42, UINT32_MAX);
    ExtendedSimple equalData(42, UINT32_MAX);
    ExtendedSimple lessThanData(41, UINT32_MAX);

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(ExtendedFieldInTemplateTest, comparisonOperatorsCompound)
{
    ExtendedCompound data(42, Compound(UINT32_MAX));
    ExtendedCompound equalData(42, Compound(UINT32_MAX));
    ExtendedCompound lessThanData(41, Compound(UINT32_MAX));

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(ExtendedFieldInTemplateTest, bitSizeOfSimple)
{
    ExtendedSimple data;
    zserio::View view(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(ExtendedFieldInTemplateTest, bitSizeOfCompound)
{
    ExtendedCompound data;
    zserio::View view(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(ExtendedFieldInTemplateTest, writeReadExtendedSimple)
{
    ExtendedSimple data(42, UINT32_MAX);
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE, bitBuffer.getBitSize());

    ExtendedSimple readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);
    ASSERT_TRUE(readView.extendedValue().isPresent());
    ASSERT_EQ(data, readData);
    ASSERT_EQ(view, readView);
}

TEST_F(ExtendedFieldInTemplateTest, writeReadExtendedCompound)
{
    ExtendedCompound data(42, Compound(UINT32_MAX));
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE, bitBuffer.getBitSize());

    ExtendedCompound readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);
    ASSERT_TRUE(readView.extendedValue().isPresent());
    ASSERT_EQ(data, readData);
    ASSERT_EQ(view, readView);
}

TEST_F(ExtendedFieldInTemplateTest, writeOriginalReadExtendedSimple)
{
    Original dataOriginal(42);
    zserio::View viewOriginal(dataOriginal);

    auto bitBuffer = zserio::serialize(dataOriginal);

    ExtendedSimple readDataExtended;
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
    bitBuffer = zserio::serialize(readDataExtended);
    ASSERT_EQ(EXTENDED_BIT_SIZE, bitBuffer.getBitSize());
}

TEST_F(ExtendedFieldInTemplateTest, writeOriginalReadExtendedCompound)
{
    Original dataOriginal(42);
    zserio::View viewOriginal(dataOriginal);

    auto bitBuffer = zserio::serialize(dataOriginal);

    ExtendedCompound readDataExtended;
    zserio::View readViewExtended = zserio::deserialize(bitBuffer, readDataExtended);
    ASSERT_FALSE(readViewExtended.extendedValue().isPresent());

    // extended value is default constructed
    ASSERT_EQ(0, readViewExtended.extendedValue()->field());

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
    readDataExtended.extendedValue->field = UINT32_MAX;
    readDataExtended.extendedValue.setPresent(true);
    ASSERT_TRUE(readViewExtended.extendedValue().isPresent());
    ASSERT_EQ(UINT32_MAX, readDataExtended.extendedValue->field);
    ASSERT_EQ(UINT32_MAX, readViewExtended.extendedValue()->field());

    // bit size as extended
    ASSERT_EQ(EXTENDED_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended));

    // writes as extended
    bitBuffer = zserio::serialize(readDataExtended);
    ASSERT_EQ(EXTENDED_BIT_SIZE, bitBuffer.getBitSize());

    test_utils::writeReadTest(readDataExtended);
}

TEST_F(ExtendedFieldInTemplateTest, writeExtendedSimpleReadOriginal)
{
    ExtendedSimple data(42, UINT32_MAX);
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE, bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::detail::read(reader, readDataOriginal);
    ASSERT_EQ(view.value(), readViewOriginal.value());
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

TEST_F(ExtendedFieldInTemplateTest, writeExtendedCompoundReadOriginal)
{
    ExtendedCompound data(42, Compound(UINT32_MAX));
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE, bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::detail::read(reader, readDataOriginal);
    ASSERT_EQ(view.value(), readViewOriginal.value());
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

TEST_F(ExtendedFieldInTemplateTest, stdHashSimple)
{
    ExtendedSimple data(42, UINT32_MAX);
    const size_t dataHash = 33040;
    ExtendedSimple equalData(42, UINT32_MAX);
    ExtendedSimple diffData(41, UINT32_MAX);
    const size_t diffDataHash = 33003;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 33040;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 33003;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

TEST_F(ExtendedFieldInTemplateTest, stdHashCompound)
{
    ExtendedCompound data(42, Compound(UINT32_MAX));
    const size_t dataHash = 33891;
    ExtendedCompound equalData(42, Compound(UINT32_MAX));
    ExtendedCompound diffData(41, Compound(UINT32_MAX));
    const size_t diffDataHash = 33854;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 33891;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 33854;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace extended_field_in_template
} // namespace extended_members
