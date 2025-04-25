#include <numeric>

#include "extended_members/extended_optional_parameterized_field/Extended.h"
#include "extended_members/extended_optional_parameterized_field/Original.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace extended_members
{
namespace extended_optional_parameterized_field
{

using AllocatorType = Extended::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class ExtendedOptionalParameterizedFieldTest : public ::testing::Test
{
protected:
    zserio::BitSize getExtendedBitSizeWithOptional()
    {
        return zserio::alignTo(8, ORIGINAL_BIT_SIZE) + 1 +
                std::accumulate(ARRAY.begin(), ARRAY.end(), static_cast<zserio::BitSize>(0),
                        [](zserio::BitSize size, const StringType& str) {
                            return size + zserio::detail::bitSizeOf(str);
                        });
    }

    static const VectorType<StringType> ARRAY;

    static constexpr zserio::BitSize ORIGINAL_BIT_SIZE = 11;
    static constexpr zserio::BitSize EXTENDED_BIT_SIZE_WITHOUT_OPTIONAL =
            zserio::alignTo(8, ORIGINAL_BIT_SIZE) + 1;
};

const VectorType<StringType> ExtendedOptionalParameterizedFieldTest::ARRAY = {"this", "is", "test"};

TEST_F(ExtendedOptionalParameterizedFieldTest, defaultConstructor)
{
    Extended data;
    zserio::View view(data);

    // always present when not read from stream
    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    // default initialized
    ASSERT_EQ(0, data.value);
    ASSERT_EQ(0, view.value());
    ASSERT_FALSE(data.extendedValue->has_value());
    ASSERT_FALSE(view.extendedValue()->has_value());
}

TEST_F(ExtendedOptionalParameterizedFieldTest, fieldConstructor)
{
    Extended data(static_cast<uint16_t>(ARRAY.size()), Parameterized());
    data.extendedValue->value().array = ARRAY;
    zserio::View view(data);

    ASSERT_TRUE(data.extendedValue.isPresent());
    ASSERT_TRUE(view.extendedValue().isPresent());

    ASSERT_EQ(ARRAY.size(), data.value);
    ASSERT_EQ(ARRAY.size(), view.value());
    ASSERT_TRUE(data.extendedValue->has_value());
    ASSERT_TRUE(view.extendedValue()->has_value());
    ASSERT_EQ(ARRAY, data.extendedValue->value().array);
    ASSERT_EQ(ARRAY.size(), view.extendedValue()->value().array().size());
}

TEST_F(ExtendedOptionalParameterizedFieldTest, comparisonOperators)
{
    Extended data(static_cast<uint16_t>(ARRAY.size()), Parameterized());
    data.extendedValue->value().array = ARRAY;
    Extended equalData(static_cast<uint16_t>(ARRAY.size()), Parameterized());
    equalData.extendedValue->value().array = ARRAY;
    Extended lessThanData(static_cast<uint16_t>(ARRAY.size() - 1), Parameterized());
    lessThanData.extendedValue->value().array = ARRAY;

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(ExtendedOptionalParameterizedFieldTest, bitSizeOf)
{
    Extended data;
    zserio::View view(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_WITHOUT_OPTIONAL, zserio::detail::bitSizeOf(view));

    data.extendedValue->emplace(Parameterized(ARRAY));
    ASSERT_EQ(getExtendedBitSizeWithOptional(), zserio::detail::bitSizeOf(view));
}

TEST_F(ExtendedOptionalParameterizedFieldTest, writeReadExtendedWithoutOptional)
{
    Extended data;
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_WITHOUT_OPTIONAL, bitBuffer.getBitSize());

    Extended readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);
    ASSERT_TRUE(readView.extendedValue().isPresent());
    ASSERT_FALSE(readView.extendedValue()->has_value());
    ASSERT_EQ(data, readData);
    ASSERT_EQ(view, readView);
}

TEST_F(ExtendedOptionalParameterizedFieldTest, writeReadExtendedWithOptional)
{
    Extended data(static_cast<uint16_t>(ARRAY.size()), Parameterized());
    data.extendedValue->value().array = ARRAY;
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(getExtendedBitSizeWithOptional(), bitBuffer.getBitSize());

    Extended readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);
    ASSERT_TRUE(readView.extendedValue().isPresent());
    ASSERT_TRUE(readView.extendedValue()->has_value());
    ASSERT_EQ(data, readData);
    ASSERT_EQ(view, readView);
}

TEST_F(ExtendedOptionalParameterizedFieldTest, writeOriginalReadExtended)
{
    Original dataOriginal(static_cast<uint16_t>(ARRAY.size()));
    zserio::View viewOriginal(dataOriginal);

    auto bitBuffer = zserio::serialize(dataOriginal);

    Extended readDataExtended;
    zserio::View readViewExtended = zserio::deserialize(bitBuffer, readDataExtended);
    ASSERT_FALSE(readViewExtended.extendedValue().isPresent());
    ASSERT_FALSE(readViewExtended.extendedValue()->has_value());

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

    // set and make the value present!
    readDataExtended.extendedValue->emplace(Parameterized(ARRAY));
    readDataExtended.extendedValue.setPresent(true);
    ASSERT_TRUE(readViewExtended.extendedValue().isPresent());
    ASSERT_EQ(ARRAY.size(), readViewExtended.extendedValue()->value().array().size());

    // bit size as extended
    ASSERT_EQ(getExtendedBitSizeWithOptional(), zserio::detail::bitSizeOf(readViewExtended));

    // writes as extended
    bitBuffer = zserio::serialize(readViewExtended);
    ASSERT_EQ(getExtendedBitSizeWithOptional(), bitBuffer.getBitSize());

    test_utils::writeReadTest(readDataExtended);
}

TEST_F(ExtendedOptionalParameterizedFieldTest, writeExtendedWithoutOptionalReadOriginal)
{
    Extended data;
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(EXTENDED_BIT_SIZE_WITHOUT_OPTIONAL, bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::detail::read(reader, readDataOriginal);
    ASSERT_EQ(view.value(), readViewOriginal.value());
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

TEST_F(ExtendedOptionalParameterizedFieldTest, writeExtendedWithOptionalReadOriginal)
{
    Extended data(static_cast<uint16_t>(ARRAY.size()), Parameterized());
    data.extendedValue->value().array = ARRAY;
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(getExtendedBitSizeWithOptional(), bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::detail::read(reader, readDataOriginal);
    ASSERT_EQ(view.value(), readViewOriginal.value());
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

TEST_F(ExtendedOptionalParameterizedFieldTest, stdHash)
{
    Extended data(static_cast<uint16_t>(ARRAY.size()), Parameterized());
    data.extendedValue->value().array = ARRAY;
    const size_t dataHash = 3594355405;
    Extended equalData(static_cast<uint16_t>(ARRAY.size()), Parameterized());
    equalData.extendedValue->value().array = ARRAY;
    Extended diffData(static_cast<uint16_t>(ARRAY.size() - 1), Parameterized());
    diffData.extendedValue->value().array = ARRAY;
    const size_t diffDataHash = 3594355368;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 263620308;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 1146261878;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace extended_optional_parameterized_field
} // namespace extended_members
