#include "extended_members/extended_packed_array/Extended1.h"
#include "extended_members/extended_packed_array/Extended2.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace extended_members
{
namespace extended_packed_array
{

using allocator_type = Extended2::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class ExtendedPackedArrayTest : public ::testing::Test
{
protected:
    template <typename T>
    T createData()
    {
        T data;
        data.array.resize(ARRAY_SIZE);
        data.packedArray->resize(PACKED_ARRAY_SIZE);
        return data;
    }

    static constexpr size_t ARRAY_SIZE = 1;
    static constexpr size_t PACKED_ARRAY_SIZE = 5;

    static constexpr zserio::BitSize ORIGINAL_BIT_SIZE = 8 + 32; // array of Elements of length 1
    static constexpr zserio::BitSize EXTENDED1_BIT_SIZE =
            zserio::alignTo(8, ORIGINAL_BIT_SIZE) + // align to 8 due to extended
            8 + // varsize
            1 + // is packed
            6 + // max bit number
            32; // first element
    static constexpr zserio::BitSize EXTENDED2_BIT_SIZE =
            zserio::alignTo(8, EXTENDED1_BIT_SIZE) + // align to 8 due to extended
            1; // auto optional not present
};

TEST_F(ExtendedPackedArrayTest, defaultConstructor)
{
    Extended2 data;
    zserio::View view(data);

    // always present when not read from stream
    ASSERT_TRUE(data.packedArray.isPresent());
    ASSERT_TRUE(data.optionalPackedArray.isPresent());
    ASSERT_TRUE(view.packedArray().isPresent());
    ASSERT_TRUE(view.optionalPackedArray().isPresent());

    // default initialized
    ASSERT_EQ(0, data.packedArray.value().size());
    ASSERT_FALSE(data.optionalPackedArray.value().has_value());
    ASSERT_EQ(0, view.packedArray().value().size());
    ASSERT_FALSE(view.optionalPackedArray().value().has_value());
}

TEST_F(ExtendedPackedArrayTest, fieldConstructor)
{
    Extended2 data(VectorType<Element>{}, VectorType<Element>{{Element{42}}}, std::nullopt);
    zserio::View view(data);

    ASSERT_TRUE(data.packedArray.isPresent());
    ASSERT_TRUE(view.packedArray().isPresent());

    ASSERT_EQ(42, data.packedArray->at(0).value);
    ASSERT_EQ(42, view.packedArray()->at(0).value());
}

TEST_F(ExtendedPackedArrayTest, operatorEquality)
{
    Extended2 data = createData<Extended2>();
    Extended2 equalData = createData<Extended2>();
    Extended2 lessThanData = createData<Extended2>();
    lessThanData.packedArray->back().value = 12;

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(ExtendedPackedArrayTest, bitSizeOfExtended1)
{
    Extended1 data = createData<Extended1>();
    zserio::View view(data);

    ASSERT_EQ(EXTENDED1_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(ExtendedPackedArrayTest, bitSizeOfExtended2)
{
    Extended2 data = createData<Extended2>();
    zserio::View view(data);

    ASSERT_EQ(EXTENDED2_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(ExtendedPackedArrayTest, writeReadExtended2)
{
    Extended2 data = createData<Extended2>();

    test_utils::writeReadTest(data);
}

TEST_F(ExtendedPackedArrayTest, writeExtended1ReadExtended2)
{
    Extended1 dataExtended1 = createData<Extended1>();
    zserio::View viewExtended1(dataExtended1);

    auto bitBuffer = zserio::serialize(dataExtended1);
    Extended2 readDataExtended2;
    zserio::View readViewExtended2 = zserio::deserialize<Extended2>(bitBuffer, readDataExtended2);
    ASSERT_FALSE(readViewExtended2.optionalPackedArray().isPresent());
    ASSERT_FALSE(readViewExtended2.optionalPackedArray());

    // bit size as extended1
    ASSERT_EQ(EXTENDED1_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended2));

    // write as extened1
    bitBuffer = zserio::serialize(readViewExtended2);
    ASSERT_EQ(EXTENDED1_BIT_SIZE, bitBuffer.getBitSize());

    // read extended1 again
    Extended1 readDataExtended1;
    zserio::View readViewExtended1 = zserio::deserialize<Extended1>(bitBuffer, readDataExtended1);
    ASSERT_EQ(viewExtended1, readViewExtended1);

    // make the extended value present
    readDataExtended2.optionalPackedArray.setPresent(true);
    ASSERT_TRUE(readViewExtended2.optionalPackedArray().isPresent());
    ASSERT_FALSE(readViewExtended2.optionalPackedArray()->has_value()); // optional not present

    // bit size as extended2
    ASSERT_EQ(EXTENDED2_BIT_SIZE, zserio::detail::bitSizeOf(readViewExtended2));

    // write as extended2
    bitBuffer = zserio::serialize(readViewExtended2);
    ASSERT_EQ(EXTENDED2_BIT_SIZE, bitBuffer.getBitSize());

    test_utils::writeReadTest(readDataExtended2);
}

TEST_F(ExtendedPackedArrayTest, stdHash)
{
    Extended2 data = createData<Extended2>();
    const size_t dataHash = 330056911;
    Extended2 equalData = createData<Extended2>();
    Extended2 diffData = createData<Extended2>();
    diffData.packedArray->back().value = 12;
    const size_t diffDataHash = 330056910;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 330056911;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 330056910;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace extended_packed_array
} // namespace extended_members
