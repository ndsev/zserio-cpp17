#include "gtest/gtest.h"
#include "zserio/Extended.h"
#include "zserio/Types.h"

namespace zserio
{

namespace
{

struct TestData
{
    // empty ctor
    TestData() :
            valueA(0),
            valueB(0.)
    {}

    // field ctor
    TestData(UInt32 valueA_, Float64 valueB_) :
            valueA(valueA_),
            valueB(valueB_)
    {}

    UInt32 valueA;
    Float64 valueB;
};

bool operator==(const TestData& lhs, const TestData& rhs)
{
    return std::tie(lhs.valueA, lhs.valueB) == std::tie(rhs.valueA, rhs.valueB);
}

} // namespace

TEST(ExtendedTest, emptyConstructor)
{
    const Extended<UInt32> extendedU32;
    ASSERT_TRUE(extendedU32.isPresent());
    ASSERT_EQ(0, extendedU32.value());

    const Extended<TestData> extendedData;
    ASSERT_TRUE(extendedData.isPresent());
    ASSERT_EQ(0, extendedData.value().valueA);
    ASSERT_DOUBLE_EQ(0., extendedData.value().valueB);
}

TEST(ExtendedTest, fieldConstructor)
{
    const Extended<UInt32> extendedU32(UInt32(13));
    ASSERT_TRUE(extendedU32.isPresent());
    ASSERT_EQ(13, extendedU32.value());
    ASSERT_EQ(13, *extendedU32);

    const Extended<TestData> extendedData(UInt32(42), Float64(1.234));
    ASSERT_TRUE(extendedData.isPresent());
    ASSERT_EQ(42, extendedData.value().valueA);
    ASSERT_DOUBLE_EQ(1.234, extendedData->valueB);
}

TEST(ExtendedTest, copyConstructor)
{
    const Extended<UInt32> extendedU32(42U);
    const Extended<UInt32> extendedU32Copied(extendedU32);
    ASSERT_EQ(*extendedU32, *extendedU32Copied);

    const Extended<TestData> extendedData(UInt32(42), Float64(1.234));
    const Extended<TestData> extendedDataCopied(extendedData);
    ASSERT_EQ(*extendedData, *extendedDataCopied);
}

TEST(ExtendedTest, copyAssignmentOperator)
{
    const Extended<TestData> extendedData(UInt32(42), Float64(1.234));
    Extended<TestData> extendedDataCopied;
    extendedDataCopied = extendedData;
    ASSERT_EQ(*extendedData, *extendedDataCopied);
}

TEST(ExtendedTest, moveConstructor)
{
    Extended<TestData> extendedData(UInt32(42), Float64(1.234));
    const Extended<TestData> extendedDataMoved(std::move(extendedData));
    ASSERT_EQ(42, extendedDataMoved->valueA);
}

TEST(ExtendedTest, moveAssignmentOperator)
{
    Extended<TestData> extendedData(UInt32(42), Float64(1.234));
    Extended<TestData> extendedDataMoved;
    extendedDataMoved = std::move(extendedData);
    ASSERT_EQ(42, extendedDataMoved->valueA);
}

TEST(ExtendedTest, setPresent)
{
    Extended<UInt32> extendedU32(UInt32(42));
    extendedU32.setPresent(false);
    ASSERT_FALSE(extendedU32.isPresent());
    // value not destructed by design
    ASSERT_EQ(42, extendedU32.value());
    ASSERT_EQ(42, *extendedU32);

    Extended<TestData> extendedData(UInt32(42), Float64(1.234));
    extendedData.setPresent(false);
    ASSERT_FALSE(extendedData.isPresent());
    // value not destructed by design
    ASSERT_EQ(42, extendedData.value().valueA);
    ASSERT_DOUBLE_EQ(1.234, extendedData->valueB);
}

TEST(ExtendedTest, nonConstGetValue)
{
    Extended<TestData> extendedData(UInt32(42), Float64(1.234));
    extendedData->valueA = UInt32(13);
    ASSERT_EQ(13, extendedData.value().valueA);
    ASSERT_DOUBLE_EQ(1.234, extendedData->valueB);
}

TEST(ExtendedTest, comparisionOperators)
{
    Extended<UInt32> extended(UInt32(42));
    Extended<UInt32> extendedEqual(UInt32(42));
    Extended<UInt32> extendedLessThan(UInt32(13));

    ASSERT_EQ(extended, extendedEqual);
    ASSERT_NE(extended, extendedLessThan);
    ASSERT_LT(extendedLessThan, extended);
    ASSERT_GT(extended, extendedLessThan);
    ASSERT_LE(extended, extendedEqual);
    ASSERT_LE(extendedLessThan, extended);
    ASSERT_GE(extended, extendedEqual);
    ASSERT_GE(extended, extendedLessThan);
}

TEST(ExtendedTest, stdHash)
{
    std::hash<Extended<UInt32>> hasher;

    Extended<UInt32> extended(1U);
    Extended<UInt32> extendedMissing;
    extendedMissing.setPresent(false);

    ASSERT_NE(hasher(extended), hasher(extendedMissing));
}

} // namespace zserio
