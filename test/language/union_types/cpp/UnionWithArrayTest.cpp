#include <limits>
#include <string>
#include <string_view>

#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "union_types/union_with_array/TestUnion.h"
#include "zserio/RebindAlloc.h"
#include "zserio/UnionCaseException.h"

namespace union_types
{
namespace union_with_array
{

using AllocatorType = TestUnion::AllocatorType;
using ChoiceTag = TestUnion::ChoiceTag;

template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class UnionWithArrayTest : public ::testing::Test
{
protected:
    VectorType<Data8> createArray8()
    {
        VectorType<Data8> array8(ARRAY8_SIZE);
        VectorType<zserio::Int8> source({-1, -2, -3, -4});
        for (size_t i = 0; i < ARRAY8_SIZE; ++i)
        {
            array8[i].data = source[i];
        }

        return array8;
    }

    VectorType<zserio::Int16> createArray16(zserio::Int16 firstElement = -10)
    {
        return VectorType<zserio::Int16>({firstElement, -20, -30, -40, -50});
    }

    static constexpr std::string_view BLOB_NAME_BASE = "language/union_types/union_with_array_";

    static constexpr size_t ARRAY8_SIZE = 4;
    static constexpr size_t ARRAY8_BITSIZE = 8 + 8 + ARRAY8_SIZE * 8;
    static constexpr size_t ARRAY16_SIZE = 5;
    static constexpr size_t ARRAY16_BITSIZE = 8 + 8 + ARRAY16_SIZE * 16;
};

TEST_F(UnionWithArrayTest, zserioChoiceTag)
{
    {
        TestUnion data;
        data.emplace<ChoiceTag::CHOICE_array8>(createArray8());
        zserio::View view(data);
        ASSERT_EQ(ChoiceTag::CHOICE_array8, view.zserioChoiceTag());
    }
    {
        TestUnion data;
        data.emplace<ChoiceTag::CHOICE_array16>(createArray16());
        zserio::View view(data);
        ASSERT_EQ(ChoiceTag::CHOICE_array16, view.zserioChoiceTag());
    }
}

TEST_F(UnionWithArrayTest, array8)
{
    TestUnion data;
    const VectorType<Data8> value = createArray8();
    data.emplace<ChoiceTag::CHOICE_array8>(value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_array16>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_array8>(data));

    zserio::View view(data);
    ASSERT_THROW(view.array16(), zserio::BadVariantAccess);
    zserio::Array<VectorType<Data8>, zserio::ArrayType::AUTO> expectedArray8View(value);
    ASSERT_EQ(expectedArray8View, view.array8());
}

TEST_F(UnionWithArrayTest, array16)
{
    TestUnion data;
    const VectorType<zserio::Int16> value = createArray16();
    data.emplace<ChoiceTag::CHOICE_array16>(value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_array8>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_array16>(data));

    zserio::View view(data);
    ASSERT_THROW(view.array8(), zserio::BadVariantAccess);
    zserio::Array<VectorType<zserio::Int16>, zserio::ArrayType::AUTO> expectedArray16View(value);
    ASSERT_EQ(expectedArray16View, view.array16());
}

TEST_F(UnionWithArrayTest, comparisonOperators)
{
    TestUnion data;
    data.emplace<ChoiceTag::CHOICE_array16>(createArray16(1));
    TestUnion equalData;
    equalData.emplace<ChoiceTag::CHOICE_array16>(createArray16(1));
    TestUnion lessThenData;
    lessThenData.emplace<ChoiceTag::CHOICE_array16>(createArray16(0));
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThenView(lessThenData);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);

    TestUnion anotherLessThenData;
    anotherLessThenData.emplace<ChoiceTag::CHOICE_array8>(createArray8());
    test_utils::comparisonOperatorsTest(data, equalData, anotherLessThenData);

    zserio::View anotherLessThenView(anotherLessThenData);
    test_utils::comparisonOperatorsTest(view, equalView, anotherLessThenView);
}

TEST_F(UnionWithArrayTest, validate)
{
    {
        // initialized union
        TestUnion data;
        zserio::View view(data);
        ASSERT_THROW(zserio::detail::validate(view), zserio::UnionCaseException);
    }
}

TEST_F(UnionWithArrayTest, bitSizeOf)
{
    {
        TestUnion data;
        data.emplace<ChoiceTag::CHOICE_array8>(createArray8());
        zserio::View view(data);
        ASSERT_EQ(ARRAY8_BITSIZE, zserio::detail::bitSizeOf(view));
    }
    {
        TestUnion data;
        data.emplace<ChoiceTag::CHOICE_array16>(createArray16());
        zserio::View view(data);
        ASSERT_EQ(ARRAY16_BITSIZE, zserio::detail::bitSizeOf(view));
    }
}

TEST_F(UnionWithArrayTest, writeRead)
{
    {
        TestUnion data;
        data.emplace<ChoiceTag::CHOICE_array8>(createArray8());
        test_utils::writeReadTest(data);
    }
    {
        TestUnion data;
        data.emplace<ChoiceTag::CHOICE_array16>(createArray16());
        test_utils::writeReadTest(data);
    }
}

TEST_F(UnionWithArrayTest, writeReadFile)
{
    {
        TestUnion data;
        data.emplace<ChoiceTag::CHOICE_array8>(createArray8());
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "array8.blob", data);
    }
    {
        TestUnion data;
        data.emplace<ChoiceTag::CHOICE_array16>(createArray16());
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "array16.blob", data);
    }
}

} // namespace union_with_array
} // namespace union_types
