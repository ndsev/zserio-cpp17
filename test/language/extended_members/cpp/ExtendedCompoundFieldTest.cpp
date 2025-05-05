#include "extended_members/extended_compound_field/Extended.h"
#include "extended_members/extended_compound_field/Original.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace extended_members
{
namespace extended_compound_field
{

using AllocatorType = Extended::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class ExtendedCompoundFieldTest : public ::testing::Test
{
protected:
    static const VectorType<zserio::UInt32> COMPOUND_ARRAY;

    static constexpr zserio::BitSize ORIGINAL_BIT_SIZE = 4 * 8;
    static constexpr zserio::BitSize EXTENDED_BIT_SIZE_DEFAULT = ORIGINAL_BIT_SIZE + 8 /* varsize */;
    static constexpr zserio::BitSize EXTENDED_BIT_SIZE_WITH_ARRAY = ORIGINAL_BIT_SIZE + 8 + /* varsize */
            5 /* compound array size */ * 4 * 8;
};

const VectorType<zserio::UInt32> ExtendedCompoundFieldTest::COMPOUND_ARRAY = {0, 1, 2, 3, 4};

TEST_F(ExtendedCompoundFieldTest, defaultConstructor)
{
    Extended data;
    zserio::View view(data);

    // always present when not read from stream
    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    // default initialized
    ASSERT_EQ(0, data.value);
    ASSERT_EQ(0, data.extendedValue->array.size());
    ASSERT_EQ(0, view.value());
    ASSERT_EQ(0, view.extendedValue()->array().size());
}

TEST_F(ExtendedCompoundFieldTest, fieldConstructor)
{
    Extended data(42, Compound(COMPOUND_ARRAY));
    zserio::View view(data);

    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    ASSERT_EQ(42, data.value);
    ASSERT_EQ(42, view.value());
    ASSERT_EQ(COMPOUND_ARRAY, data.extendedValue->array);
    ASSERT_EQ(COMPOUND_ARRAY.size(), view.extendedValue()->array().size());
}

TEST_F(ExtendedCompoundFieldTest, comparisionOperators)
{
    Extended data(42, Compound(COMPOUND_ARRAY));
    Extended equalData(42, Compound(COMPOUND_ARRAY));
    Extended lessThanData(41, Compound(COMPOUND_ARRAY));

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(ExtendedCompoundFieldTest, bitSizeOf)
{
    Extended data;
    zserio::View view(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_DEFAULT, zserio::detail::bitSizeOf(view));
}

TEST_F(ExtendedCompoundFieldTest, writeReadExtended)
{
    Extended data(42, Compound(COMPOUND_ARRAY));
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_WITH_ARRAY, bitBuffer.getBitSize());

    Extended readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);
    ASSERT_TRUE(readView.extendedValue().isPresent());
    ASSERT_EQ(data, readData);
    ASSERT_EQ(view, readView);
}

TEST_F(ExtendedCompoundFieldTest, writeOriginalReadExtended)
{
    Original dataOriginal(42);
    zserio::View viewOriginal(dataOriginal);

    auto bitBuffer = zserio::serialize(dataOriginal);

    Extended readDataExtended;
    zserio::View readViewExtended = zserio::deserialize(bitBuffer, readDataExtended);
    ASSERT_FALSE(readViewExtended.extendedValue().isPresent());

    // extended value is default constructed
    ASSERT_EQ(0, readViewExtended.extendedValue()->array().size());

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
    readDataExtended.extendedValue->array = COMPOUND_ARRAY;
    readDataExtended.extendedValue.setPresent(true);
    ASSERT_TRUE(readViewExtended.extendedValue().isPresent());
    ASSERT_EQ(COMPOUND_ARRAY, readDataExtended.extendedValue->array);

    // bit size as extended
    ASSERT_EQ(EXTENDED_BIT_SIZE_WITH_ARRAY, zserio::detail::bitSizeOf(readViewExtended));

    // write as extended
    bitBuffer = zserio::serialize(readViewExtended);
    ASSERT_EQ(EXTENDED_BIT_SIZE_WITH_ARRAY, bitBuffer.getBitSize());

    test_utils::writeReadTest(readDataExtended);
}

TEST_F(ExtendedCompoundFieldTest, writeExtendedReadOriginal)
{
    Extended data(42, Compound(COMPOUND_ARRAY));
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_WITH_ARRAY, bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::detail::read(reader, readDataOriginal);
    ASSERT_EQ(view.value(), readViewOriginal.value());
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

TEST_F(ExtendedCompoundFieldTest, stdHash)
{
    Extended data(42, Compound(COMPOUND_ARRAY));
    const size_t dataHash = 1594997558;
    Extended equalData(42, Compound(COMPOUND_ARRAY));
    Extended diffData(41, Compound(COMPOUND_ARRAY));
    const size_t diffDataHash = 1594997521;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 1594997558;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 1594997521;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace extended_compound_field
} // namespace extended_members
