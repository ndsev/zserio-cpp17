#include <numeric>

#include "extended_members/multiple_extended_fields_various_types/Extended1.h"
#include "extended_members/multiple_extended_fields_various_types/Extended2.h"
#include "extended_members/multiple_extended_fields_various_types/Original.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ExtendedFieldException.h"

namespace extended_members
{
namespace multiple_extended_fields_various_types
{

using allocator_type = Extended2::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;
using BitBufferType = zserio::BasicBitBuffer<zserio::RebindAlloc<allocator_type, uint8_t>>;
using BytesType = zserio::BasicBytes<zserio::RebindAlloc<allocator_type, uint8_t>>;

class MultipleExtendedFieldsVariousTypesTest : public ::testing::Test
{
protected:
    void checkExtended1FieldsPresent(
            const Extended2& data, const zserio::View<Extended2>& view, bool expectedExtended1FieldsPresent)
    {
        ASSERT_EQ(expectedExtended1FieldsPresent, data.extendedValue1.isPresent());
        ASSERT_EQ(expectedExtended1FieldsPresent, data.extendedValue2.isPresent());
        ASSERT_EQ(expectedExtended1FieldsPresent, data.extendedValue3.isPresent());

        ASSERT_EQ(expectedExtended1FieldsPresent, view.extendedValue1().isPresent());
        ASSERT_EQ(expectedExtended1FieldsPresent, view.extendedValue2().isPresent());
        ASSERT_EQ(expectedExtended1FieldsPresent, view.extendedValue3().isPresent());
    }

    void checkExtended2FieldsPresent(
            const Extended2& data, const zserio::View<Extended2>& view, bool expectedExtended2FieldsPresent)
    {
        ASSERT_EQ(expectedExtended2FieldsPresent, data.extendedValue4.isPresent());
        ASSERT_EQ(expectedExtended2FieldsPresent, data.extendedValue5.isPresent());
        ASSERT_EQ(expectedExtended2FieldsPresent, data.extendedValue6.isPresent());
        ASSERT_EQ(expectedExtended2FieldsPresent, data.extendedValue7.isPresent());
        ASSERT_EQ(expectedExtended2FieldsPresent, data.extendedValue8.isPresent());
        ASSERT_EQ(expectedExtended2FieldsPresent, data.extendedValue9.isPresent());

        ASSERT_EQ(expectedExtended2FieldsPresent, view.extendedValue4().isPresent());
        ASSERT_EQ(expectedExtended2FieldsPresent, view.extendedValue5().isPresent());
        ASSERT_EQ(expectedExtended2FieldsPresent, view.extendedValue6().isPresent());
        ASSERT_EQ(expectedExtended2FieldsPresent, view.extendedValue7().isPresent());
        ASSERT_EQ(expectedExtended2FieldsPresent, view.extendedValue8().isPresent());
        ASSERT_EQ(expectedExtended2FieldsPresent, view.extendedValue9().isPresent());
    }

    void checkAllExtendedFieldsPresent(
            const Extended2& data, const zserio::View<Extended2>& view, bool expectedPresent)
    {
        checkExtended1FieldsPresent(data, view, expectedPresent);
        checkExtended2FieldsPresent(data, view, expectedPresent);
    }

    template <typename T = Extended1>
    T createExtended1()
    {
        T extended1;
        extended1.value = VALUE;
        *extended1.extendedValue1 = EXTENDED_VALUE1;
        *extended1.extendedValue2 = EXTENDED_VALUE2;
        *extended1.extendedValue3 = EXTENDED_VALUE3;
        return extended1;
    }

    Extended2 createExtended2()
    {
        Extended2 extended2;
        extended2.value = VALUE;
        *extended2.extendedValue1 = EXTENDED_VALUE1;
        *extended2.extendedValue2 = EXTENDED_VALUE2;
        *extended2.extendedValue3 = EXTENDED_VALUE3;
        *extended2.extendedValue4 = std::nullopt;
        *extended2.extendedValue5 = EXTENDED_VALUE5;
        *extended2.extendedValue6 = EXTENDED_VALUE6;
        extended2.extendedValue7->emplace<Union::Tag::valueU32>(UINT32_MAX);
        *extended2.extendedValue8 = std::nullopt;
        *extended2.extendedValue9 = EXTENDED_VALUE9;
        return extended2;
    }

    static zserio::BitSize calcExtended1BitSize()
    {
        zserio::BitSize bitSize = ORIGINAL_BIT_SIZE;
        bitSize = zserio::alignTo(8, bitSize);
        bitSize += 1 + 4 * 8; // optional extendedValue1
        bitSize = zserio::alignTo(8, bitSize);
        bitSize += zserio::detail::bitSizeOf(std::cref(EXTENDED_VALUE2));
        bitSize = zserio::alignTo(8, bitSize);
        bitSize += zserio::detail::bitSizeOf(EXTENDED_VALUE3);
        return bitSize;
    }

    static zserio::BitSize calcExtended2BitSize()
    {
        zserio::BitSize bitSize = calcExtended1BitSize();
        bitSize = zserio::alignTo(8, bitSize);
        bitSize += 1; // unset optional extendedValue4
        bitSize = zserio::alignTo(8, bitSize);
        bitSize += zserio::detail::bitSizeOf(EXTENDED_VALUE5);
        bitSize = zserio::alignTo(8, bitSize);
        bitSize += std::accumulate(EXTENDED_VALUE6.begin(), EXTENDED_VALUE6.end(),
                static_cast<zserio::BitSize>(0), [](zserio::BitSize size, const StringType& str) {
                    return size + zserio::detail::bitSizeOf(str);
                });
        bitSize = zserio::alignTo(8, bitSize);
        bitSize += 8 + 4 * 8; // extendedValue7 (choiceTag + valueU32)
        bitSize = zserio::alignTo(8, bitSize);
        bitSize += 1; // unset optional extendedValue8
        bitSize = zserio::alignTo(8, bitSize);
        bitSize += EXTENDED_VALUE5; // used non-auto optional dynamic bit field extendedValue9
        return bitSize;
    }

    static constexpr zserio::Int7 VALUE = -13;
    static constexpr zserio::UInt32 EXTENDED_VALUE1 = 42;
    static const BitBufferType EXTENDED_VALUE2;
    static const BytesType EXTENDED_VALUE3;
    static constexpr zserio::VarSize EXTENDED_VALUE5 = 3;
    static const VectorType<StringType> EXTENDED_VALUE6;
    static constexpr zserio::DynUInt64<> EXTENDED_VALUE9 = 7; // bit<EXTENDED_VALUE5>

    static constexpr size_t ORIGINAL_BIT_SIZE = 7;
};

const BitBufferType MultipleExtendedFieldsVariousTypesTest::EXTENDED_VALUE2 = BitBufferType({0xCA, 0xFE}, 16);
const BytesType MultipleExtendedFieldsVariousTypesTest::EXTENDED_VALUE3 = {0xDE, 0xAD};
const VectorType<StringType> MultipleExtendedFieldsVariousTypesTest::EXTENDED_VALUE6 = {"this", "is", "test"};

TEST_F(MultipleExtendedFieldsVariousTypesTest, defaultConstructor)
{
    Extended2 data;
    zserio::View view(data);

    // always present when not read from stream
    checkAllExtendedFieldsPresent(data, view, true);

    // default initialized
    ASSERT_FALSE(data.extendedValue1->has_value());
    ASSERT_EQ(BitBufferType(), *data.extendedValue2);
    ASSERT_EQ(VectorType<uint8_t>(), *data.extendedValue3);
    ASSERT_FALSE(data.extendedValue4->has_value());
    ASSERT_EQ(0, *data.extendedValue5);
    ASSERT_EQ(VectorType<StringType>(), *data.extendedValue6);
    ASSERT_EQ(Union::Tag::ZSERIO_UNDEFINED, data.extendedValue7->index());
    ASSERT_FALSE(data.extendedValue8->has_value());
    ASSERT_FALSE(data.extendedValue9->has_value());

    ASSERT_FALSE(view.extendedValue1()->has_value());
    ASSERT_EQ(BitBufferType(), *view.extendedValue2());
    ASSERT_EQ(VectorType<uint8_t>(), *view.extendedValue3());
    ASSERT_FALSE(view.extendedValue4()->has_value());
    ASSERT_EQ(0, *view.extendedValue5());
    ASSERT_EQ(0, view.extendedValue6()->size());
    ASSERT_EQ(Union::Tag::ZSERIO_UNDEFINED, view.extendedValue7()->zserioChoiceTag());
    ASSERT_FALSE(view.extendedValue8()->has_value());
    ASSERT_FALSE(view.extendedValue9()->has_value());
}

TEST_F(MultipleExtendedFieldsVariousTypesTest, fieldConstructor)
{
    Union extendedValue7;
    Extended2 data(VALUE, EXTENDED_VALUE1, EXTENDED_VALUE2, EXTENDED_VALUE3, std::nullopt, EXTENDED_VALUE5,
            EXTENDED_VALUE6, extendedValue7, std::nullopt, EXTENDED_VALUE9);

    zserio::View view(data);

    checkAllExtendedFieldsPresent(data, view, true);

    ASSERT_TRUE(data.extendedValue1->has_value());
    ASSERT_EQ(EXTENDED_VALUE1, *data.extendedValue1);
    ASSERT_EQ(EXTENDED_VALUE2, *data.extendedValue2);
    ASSERT_EQ(EXTENDED_VALUE3, *data.extendedValue3);
    ASSERT_FALSE(data.extendedValue4->has_value());
    ASSERT_EQ(EXTENDED_VALUE5, *data.extendedValue5);
    ASSERT_EQ(EXTENDED_VALUE6, *data.extendedValue6);
    ASSERT_EQ(Union::Tag::ZSERIO_UNDEFINED, data.extendedValue7->index());
    ASSERT_FALSE(data.extendedValue8->has_value());
    ASSERT_EQ(EXTENDED_VALUE9, *data.extendedValue9);

    ASSERT_TRUE(view.extendedValue1()->has_value());
    ASSERT_EQ(EXTENDED_VALUE1, *view.extendedValue1());
    ASSERT_EQ(EXTENDED_VALUE2, *view.extendedValue2());
    ASSERT_EQ(EXTENDED_VALUE3, *view.extendedValue3());
    ASSERT_FALSE(view.extendedValue4()->has_value());
    ASSERT_EQ(EXTENDED_VALUE5, *view.extendedValue5());
    ASSERT_EQ(EXTENDED_VALUE6.size(), view.extendedValue6()->size());
    ASSERT_EQ(Union::Tag::ZSERIO_UNDEFINED, view.extendedValue7()->zserioChoiceTag());
    ASSERT_FALSE(view.extendedValue8()->has_value());
    ASSERT_EQ(EXTENDED_VALUE9, *view.extendedValue9());
}

TEST_F(MultipleExtendedFieldsVariousTypesTest, comparisionOperators)
{
    Extended2 data = createExtended2();
    Extended2 equalData = createExtended2();
    Extended2 lessThanData = createExtended2();
    **lessThanData.extendedValue9 -= 1;

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);
}

TEST_F(MultipleExtendedFieldsVariousTypesTest, bitSizeOf)
{
    Extended1 dataExtended1 = createExtended1();
    zserio::View viewExtended1(dataExtended1);
    ASSERT_EQ(calcExtended1BitSize(), zserio::detail::bitSizeOf(viewExtended1));

    Extended2 dataExtended2 = createExtended2();
    zserio::View viewExtended2(dataExtended2);
    ASSERT_EQ(calcExtended2BitSize(), zserio::detail::bitSizeOf(viewExtended2));
}

TEST_F(MultipleExtendedFieldsVariousTypesTest, writeReadExtended2)
{
    Extended2 data = createExtended2();
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(view);
    ASSERT_EQ(calcExtended2BitSize(), bitBuffer.getBitSize());

    Extended2 readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);
    checkAllExtendedFieldsPresent(readData, readView, true);
    ASSERT_EQ(data, readData);
    ASSERT_EQ(view, readView);
}

TEST_F(MultipleExtendedFieldsVariousTypesTest, writeOriginalReadExtended2)
{
    Original dataOriginal(VALUE);
    zserio::View viewOriginal(dataOriginal);
    auto bitBuffer = zserio::serialize(dataOriginal);
    Extended2 readDataExtended2;
    zserio::View readViewExtended2 = zserio::deserialize(bitBuffer, readDataExtended2);
    checkAllExtendedFieldsPresent(readDataExtended2, readViewExtended2, false);

    // extended fields are default constructed
    ASSERT_FALSE(readViewExtended2.extendedValue1()->has_value());
    ASSERT_EQ(BitBufferType(), *readViewExtended2.extendedValue2());
    ASSERT_EQ(VectorType<uint8_t>(), *readViewExtended2.extendedValue3());
    ASSERT_FALSE(readViewExtended2.extendedValue4()->has_value());
    ASSERT_EQ(0, *readViewExtended2.extendedValue5());
    ASSERT_EQ(0, readViewExtended2.extendedValue6()->size());
    ASSERT_EQ(Union::Tag::ZSERIO_UNDEFINED, readViewExtended2.extendedValue7()->zserioChoiceTag());
    ASSERT_FALSE(readViewExtended2.extendedValue8()->has_value());
    ASSERT_FALSE(readViewExtended2.extendedValue9()->has_value());

    // bit size as original
    ASSERT_EQ(ORIGINAL_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended2));

    // writes as original
    bitBuffer = zserio::serialize(readViewExtended2);
    ASSERT_EQ(ORIGINAL_BIT_SIZE, bitBuffer.getBitSize());

    // read original again
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::deserialize(bitBuffer, readDataOriginal);
    ASSERT_EQ(dataOriginal, readDataOriginal);
    ASSERT_EQ(viewOriginal, readViewOriginal);
}

TEST_F(MultipleExtendedFieldsVariousTypesTest, writeExtended1ReadExtended2)
{
    Extended1 dataExtended1 = createExtended1();
    zserio::View viewExtended1(dataExtended1);
    auto bitBuffer = zserio::serialize(viewExtended1);

    Extended2 readDataExtended2;
    zserio::View readViewExtended2 = zserio::deserialize(bitBuffer, readDataExtended2);
    checkExtended1FieldsPresent(readDataExtended2, readViewExtended2, true);
    checkExtended2FieldsPresent(readDataExtended2, readViewExtended2, false);

    // extended1 fields are read from the stream
    ASSERT_TRUE(readViewExtended2.extendedValue1()->has_value());
    ASSERT_EQ(EXTENDED_VALUE1, *readViewExtended2.extendedValue1());
    ASSERT_EQ(EXTENDED_VALUE2, *readViewExtended2.extendedValue2());
    ASSERT_EQ(EXTENDED_VALUE3, *readViewExtended2.extendedValue3());

    // extended2 fields are default constructed
    ASSERT_FALSE(readViewExtended2.extendedValue4()->has_value());
    ASSERT_EQ(0, *readViewExtended2.extendedValue5());
    ASSERT_EQ(0, readViewExtended2.extendedValue6()->size());
    ASSERT_EQ(Union::Tag::ZSERIO_UNDEFINED, readViewExtended2.extendedValue7()->zserioChoiceTag());
    ASSERT_FALSE(readViewExtended2.extendedValue8()->has_value());
    ASSERT_FALSE(readViewExtended2.extendedValue9()->has_value());

    // bit size as extended1
    ASSERT_EQ(calcExtended1BitSize(), zserio::detail::bitSizeOf(readViewExtended2));

    // writes as extended1
    bitBuffer = zserio::serialize(readViewExtended2);
    ASSERT_EQ(calcExtended1BitSize(), bitBuffer.getBitSize());

    // read extended1 again
    Extended1 readDataExtended1;
    zserio::View readViewExtended1 = zserio::deserialize(bitBuffer, readDataExtended1);
    ASSERT_EQ(dataExtended1, readDataExtended1);
    ASSERT_EQ(viewExtended1, readViewExtended1);

    // read original
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::deserialize(bitBuffer, readDataOriginal);
    ASSERT_EQ(VALUE, readDataOriginal.value);
    ASSERT_EQ(VALUE, readViewOriginal.value());

    // setter Extended2 fields present
    readDataExtended2.extendedValue4.setPresent(true);
    *readDataExtended2.extendedValue5 = EXTENDED_VALUE5;
    readDataExtended2.extendedValue5.setPresent(true);
    *readDataExtended2.extendedValue6 = EXTENDED_VALUE6;
    readDataExtended2.extendedValue6.setPresent(true);
    readDataExtended2.extendedValue7->emplace<Union::Tag::valueU32>(UINT32_MAX);
    readDataExtended2.extendedValue7.setPresent(true);
    readDataExtended2.extendedValue8.setPresent(true);
    *readDataExtended2.extendedValue9 = EXTENDED_VALUE9;
    readDataExtended2.extendedValue9.setPresent(true);
    checkAllExtendedFieldsPresent(readDataExtended2, readViewExtended2, true);

    test_utils::writeReadTest(readDataExtended2);
}

TEST_F(MultipleExtendedFieldsVariousTypesTest, validationError)
{
    Original dataOriginal;
    auto bitBuffer = zserio::serialize(dataOriginal);

    Extended2 dataExtended2;
    zserio::deserialize(bitBuffer, dataExtended2);

    EXPECT_FALSE(dataExtended2.extendedValue1.isPresent());
    EXPECT_FALSE(dataExtended2.extendedValue2.isPresent());
    EXPECT_FALSE(dataExtended2.extendedValue3.isPresent());
    EXPECT_FALSE(dataExtended2.extendedValue4.isPresent());
    EXPECT_FALSE(dataExtended2.extendedValue5.isPresent());
    EXPECT_FALSE(dataExtended2.extendedValue6.isPresent());
    EXPECT_FALSE(dataExtended2.extendedValue7.isPresent());
    EXPECT_FALSE(dataExtended2.extendedValue8.isPresent());
    EXPECT_FALSE(dataExtended2.extendedValue9.isPresent());

    dataExtended2.extendedValue2.setPresent(true);
    ASSERT_THROW((void)zserio::serialize(dataExtended2), zserio::ExtendedFieldException);

    dataExtended2.extendedValue1.setPresent(true);
    ASSERT_NO_THROW((void)zserio::serialize(dataExtended2));

    *dataExtended2.extendedValue5 = EXTENDED_VALUE5;
    dataExtended2.extendedValue5.setPresent(true);
    ASSERT_THROW((void)zserio::serialize(dataExtended2), zserio::ExtendedFieldException);

    dataExtended2.extendedValue3.setPresent(true);
    dataExtended2.extendedValue4.setPresent(true);
    ASSERT_NO_THROW((void)zserio::serialize(dataExtended2));

    *dataExtended2.extendedValue9 = EXTENDED_VALUE9;
    dataExtended2.extendedValue9.setPresent(true);
    ASSERT_THROW((void)zserio::serialize(dataExtended2), zserio::ExtendedFieldException);

    *dataExtended2.extendedValue6 = EXTENDED_VALUE6;
    dataExtended2.extendedValue6.setPresent(true);
    ASSERT_THROW((void)zserio::serialize(dataExtended2), zserio::ExtendedFieldException);

    dataExtended2.extendedValue7->emplace<Union::Tag::valueU32>(UINT32_MAX);
    dataExtended2.extendedValue7.setPresent(true);
    ASSERT_THROW((void)zserio::serialize(dataExtended2), zserio::ExtendedFieldException);

    dataExtended2.extendedValue8.setPresent(true);
    ASSERT_NO_THROW((void)zserio::serialize(dataExtended2));
}

TEST_F(MultipleExtendedFieldsVariousTypesTest, stdHash)
{
    Extended2 data = createExtended2();
    const size_t dataHash = 2740934259;
    Extended2 equalData = createExtended2();
    Extended2 diffData = createExtended2();
    **diffData.extendedValue9 -= 1;
    const size_t diffDataHash = 2740934258;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 2740968595;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 2740968594;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace multiple_extended_fields_various_types
} // namespace extended_members
