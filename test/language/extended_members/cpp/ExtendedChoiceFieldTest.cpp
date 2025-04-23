#include "extended_members/extended_choice_field/Extended.h"
#include "extended_members/extended_choice_field/Original.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace extended_members
{
namespace extended_choice_field
{

using AllocatorType = Extended::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class ExtendedChoiceFieldTest : public ::testing::Test
{
protected:
    static constexpr zserio::BitSize ORIGINAL_BIT_SIZE = 4 * 8;
    static constexpr zserio::BitSize EXTENDED_BIT_SIZE_EMPTY = ORIGINAL_BIT_SIZE;
    static constexpr zserio::BitSize EXTENDED_BIT_SIZE_VALUE = ORIGINAL_BIT_SIZE + 4 * 8;

    static const VectorType<zserio::UInt32> VALUES;
    static constexpr zserio::BitSize EXTENDED_BIT_SIZE_VALUES = ORIGINAL_BIT_SIZE + 5 * 4 * 8;
};

const VectorType<zserio::UInt32> ExtendedChoiceFieldTest::VALUES = {0, 1, 2, 3, 4};

TEST_F(ExtendedChoiceFieldTest, defaultConstructor)
{
    Extended data;
    zserio::View view(data);

    // always present when not read from stream
    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    // default initialized
    ASSERT_EQ(0, data.numElements);
    ASSERT_EQ(0, view.numElements());

    ASSERT_EQ(Choice::Tag::ZSERIO_UNDEFINED, data.extendedValue->index());
    ASSERT_EQ(Choice::Tag::ZSERIO_UNDEFINED, view.extendedValue()->zserioChoiceTag());
    ASSERT_EQ(0, view.extendedValue()->numElements());
}

TEST_F(ExtendedChoiceFieldTest, fieldConstructor)
{
    Extended data(1, Choice());
    zserio::View view(data);

    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    data.extendedValue->emplace<Choice::Tag::value>(42);

    ASSERT_EQ(1, data.numElements);
    ASSERT_EQ(1, view.numElements());

    ASSERT_EQ(Choice::Tag::value, data.extendedValue->index());
    ASSERT_EQ(Choice::Tag::value, view.extendedValue()->zserioChoiceTag());
    ASSERT_EQ(42, zserio::get<Choice::Tag::value>(*data.extendedValue));
    ASSERT_EQ(1, view.extendedValue()->numElements());
    ASSERT_EQ(42, view.extendedValue()->value());
}

TEST_F(ExtendedChoiceFieldTest, comparisonOperators)
{
    Extended data(1, Choice(zserio::in_place_index<Choice::Tag::value>, 42));
    Extended equalData(1, Choice(zserio::in_place_index<Choice::Tag::value>, 42));
    Extended lessThanData(1, Choice(zserio::in_place_index<Choice::Tag::value>, 41));

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(ExtendedChoiceFieldTest, bitSizeOf)
{
    Extended dataEmpty(0, Choice());
    zserio::View viewEmpty(dataEmpty);
    ASSERT_EQ(EXTENDED_BIT_SIZE_EMPTY, zserio::detail::bitSizeOf(viewEmpty));

    Extended dataValue(1, Choice());
    zserio::View viewValue(dataValue);
    dataValue.extendedValue->emplace<Choice::Tag::value>(42);
    ASSERT_EQ(EXTENDED_BIT_SIZE_VALUE, zserio::detail::bitSizeOf(viewValue));

    Extended dataValues(static_cast<uint32_t>(VALUES.size()), Choice());
    zserio::View viewValues(dataValues);
    dataValues.extendedValue->emplace<Choice::Tag::values>(VALUES);
    ASSERT_EQ(EXTENDED_BIT_SIZE_VALUES, zserio::detail::bitSizeOf(viewValues));
}

TEST_F(ExtendedChoiceFieldTest, writeReadExtendedEmpty)
{
    Extended data(0, Choice());
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_EMPTY, bitBuffer.getBitSize());

    Extended readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);
    ASSERT_FALSE(readView.extendedValue().isPresent());
    ASSERT_FALSE(data == readData);
    ASSERT_FALSE(view == readView);
}

TEST_F(ExtendedChoiceFieldTest, writeReadExtendedValue)
{
    Extended data(1, Choice());
    data.extendedValue->emplace<Choice::Tag::value>(42);
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_VALUE, bitBuffer.getBitSize());

    Extended readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);
    ASSERT_TRUE(readView.extendedValue().isPresent());
    ASSERT_EQ(data, readData);
    ASSERT_EQ(view, readView);
}

TEST_F(ExtendedChoiceFieldTest, writeReadExtendedValues)
{
    Extended data(static_cast<uint32_t>(VALUES.size()), Choice());
    data.extendedValue->emplace<Choice::Tag::values>(VALUES);
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_VALUES, bitBuffer.getBitSize());

    Extended readData;
    zserio::View readView = zserio::deserialize<Extended>(bitBuffer, readData);
    ASSERT_TRUE(readView.extendedValue().isPresent());
    ASSERT_EQ(data, readData);
    ASSERT_EQ(view, readView);
}

TEST_F(ExtendedChoiceFieldTest, writeOriginalReadExtended)
{
    Original dataOriginal(static_cast<uint32_t>(VALUES.size()));
    zserio::View viewOriginal(dataOriginal);

    auto bitBuffer = zserio::serialize(dataOriginal);

    Extended readDataExtended;
    zserio::View readViewExtended = zserio::deserialize(bitBuffer, readDataExtended);
    ASSERT_FALSE(readViewExtended.extendedValue().isPresent());

    // extended value is default constructed
    ASSERT_EQ(Choice::Tag::ZSERIO_UNDEFINED, readViewExtended.extendedValue()->zserioChoiceTag());

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
    readDataExtended.extendedValue->emplace<Choice::Tag::values>(VALUES);
    readDataExtended.extendedValue.setPresent(true);
    ASSERT_TRUE(readViewExtended.extendedValue().isPresent());
    ASSERT_EQ(VALUES.at(0), readViewExtended.extendedValue()->values().at(0));

    // bit size as extended
    ASSERT_EQ(EXTENDED_BIT_SIZE_VALUES, zserio::detail::bitSizeOf(readViewExtended));

    // write as extended
    bitBuffer = zserio::serialize(readViewExtended);
    ASSERT_EQ(EXTENDED_BIT_SIZE_VALUES, bitBuffer.getBitSize());

    test_utils::writeReadTest(readDataExtended);
}

TEST_F(ExtendedChoiceFieldTest, writeExtendedEmptyReadOriginal)
{
    Extended data(0, Choice());
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_EMPTY, bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Original dataOriginal;
    zserio::View readViewOriginal = zserio::detail::read(reader, dataOriginal);
    ASSERT_EQ(view.numElements(), readViewOriginal.numElements());
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

TEST_F(ExtendedChoiceFieldTest, writeExtendedValueReadOriginal)
{
    Extended data(1, Choice());
    data.extendedValue->emplace<Choice::Tag::value>(42);
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_VALUE, bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::detail::read(reader, readDataOriginal);
    ASSERT_EQ(view.numElements(), readViewOriginal.numElements());
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

TEST_F(ExtendedChoiceFieldTest, writeExtendedValuesReadOriginal)
{
    Extended data(static_cast<uint32_t>(VALUES.size()), Choice());
    data.extendedValue->emplace<Choice::Tag::values>(VALUES);
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_VALUES, bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::detail::read(reader, readDataOriginal);
    ASSERT_EQ(view.numElements(), readViewOriginal.numElements());
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

TEST_F(ExtendedChoiceFieldTest, stdHash)
{
    Extended data(1, Choice(zserio::in_place_index<Choice::Tag::value>, 42));
    const size_t dataHash = 63090;
    Extended equalData(1, Choice(zserio::in_place_index<Choice::Tag::value>, 42));
    Extended diffData(1, Choice(zserio::in_place_index<Choice::Tag::value>, 41));
    const size_t diffDataHash = 63089;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 63090;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 63089;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace extended_choice_field
} // namespace extended_members
