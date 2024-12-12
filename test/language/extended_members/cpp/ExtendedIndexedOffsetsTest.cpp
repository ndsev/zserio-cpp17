#include <numeric>

#include "extended_members/extended_indexed_offsets/Extended.h"
#include "extended_members/extended_indexed_offsets/Original.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace extended_members
{
namespace extended_indexed_offsets
{

using AllocatorType = Extended::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class ExtendedIndexedOffsetsTest : public ::testing::Test
{
protected:
    static zserio::BitSize getExtendedBitSize()
    {
        zserio::BitSize bitSize = ORIGINAL_BIT_SIZE;
        bitSize += 8;
        bitSize += std::accumulate(ARRAY.begin(), ARRAY.end(), static_cast<zserio::BitSize>(0),
                [](zserio::BitSize size, const StringType& str) {
                    return size + zserio::detail::bitSizeOf(str);
                });
        return bitSize;
    }

    static const VectorType<zserio::UInt32> OFFSETS;
    static const VectorType<StringType> ARRAY;

    static constexpr zserio::BitSize ORIGINAL_BIT_SIZE = 8 + 5 * 4 * 8;
};

const VectorType<zserio::UInt32> ExtendedIndexedOffsetsTest::OFFSETS = {0, 0, 0, 0, 0};
const VectorType<StringType> ExtendedIndexedOffsetsTest::ARRAY = {
        "extended", "indexed", "offsets", "test", "!"};

TEST_F(ExtendedIndexedOffsetsTest, defaultConstructor)
{
    Extended data;
    zserio::View view(data);

    // always present when not read from stream
    ASSERT_TRUE(data.array.isPresent());
    ASSERT_TRUE(view.array().isPresent());

    // default initialized
    ASSERT_EQ(0, data.offsets.size());
    ASSERT_EQ(0, view.offsets().size());
    ASSERT_EQ(0, data.array->size());
    ASSERT_EQ(0, view.array()->size());
}

TEST_F(ExtendedIndexedOffsetsTest, fieldConstructor)
{
    Extended data(OFFSETS, ARRAY);
    zserio::View view(data);

    ASSERT_TRUE(data.array.isPresent());
    ASSERT_TRUE(view.array().isPresent());

    ASSERT_EQ(OFFSETS, data.offsets);
    ASSERT_EQ(OFFSETS.size(), view.offsets().size());
    ASSERT_EQ(ARRAY, *data.array);
    ASSERT_EQ(ARRAY.size(), view.array()->size());
}

TEST_F(ExtendedIndexedOffsetsTest, comparisonOperators)
{
    Extended data(OFFSETS, ARRAY);
    Extended equalData(OFFSETS, ARRAY);
    Extended lessThanData(OFFSETS, {});

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(ExtendedIndexedOffsetsTest, bitSizeOf)
{
    Extended data(OFFSETS, ARRAY);
    zserio::View view(data);
    ASSERT_EQ(getExtendedBitSize(), zserio::detail::bitSizeOf(view));
}

TEST_F(ExtendedIndexedOffsetsTest, writeReadExtended)
{
    Extended data(OFFSETS, ARRAY);
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(getExtendedBitSize(), bitBuffer.getBitSize());

    Extended readData;
    zserio::View readView = zserio::deserialize(bitBuffer, readData);
    ASSERT_TRUE(readView.array().isPresent());
    ASSERT_EQ(data, readData);
    ASSERT_EQ(view, readView);
}

TEST_F(ExtendedIndexedOffsetsTest, writeOriginalReadExtended)
{
    Original dataOriginal(OFFSETS);
    zserio::View viewOriginal(dataOriginal);

    auto bitBuffer = zserio::serialize(dataOriginal);

    Extended readDataExtended;
    zserio::View readViewExtended = zserio::deserialize(bitBuffer, readDataExtended);
    ASSERT_FALSE(readViewExtended.array().isPresent());

    // extended value is default constructed
    ASSERT_EQ(0, readViewExtended.array()->size());

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
    *readDataExtended.array = ARRAY;
    readDataExtended.array.setPresent(true);
    ASSERT_TRUE(readViewExtended.array().isPresent());
    ASSERT_EQ(ARRAY, *readDataExtended.array);

    // bit size as extended
    ASSERT_EQ(getExtendedBitSize(), zserio::detail::bitSizeOf(readViewExtended));

    // writes as extended
    bitBuffer = zserio::serialize(readDataExtended);
    ASSERT_EQ(getExtendedBitSize(), bitBuffer.getBitSize());

    test_utils::writeReadTest(readDataExtended);
}

TEST_F(ExtendedIndexedOffsetsTest, writeExtendedReadOriginal)
{
    Extended data(OFFSETS, ARRAY);
    zserio::View view(data);
    auto bitBuffer = zserio::serialize(data);
    ASSERT_EQ(getExtendedBitSize(), bitBuffer.getBitSize());

    zserio::BitStreamReader reader(bitBuffer);
    Original readDataOriginal;
    zserio::View readViewOriginal = zserio::detail::read(reader, readDataOriginal);
    ASSERT_EQ(view.offsets(), readViewOriginal.offsets());
    ASSERT_EQ(ORIGINAL_BIT_SIZE, reader.getBitPosition());
}

} // namespace extended_indexed_offsets
} // namespace extended_members
