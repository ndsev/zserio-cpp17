#include "choice_types/uint64_param_choice/UInt64ParamChoice.h"
#include "gtest/gtest.h"
#include "test_utils/ComparisonOperatorsTest.h"
#include "test_utils/HashTest.h"
#include "test_utils/ReadTest.h"
#include "test_utils/WriteReadFileTest.h"
#include "test_utils/WriteReadTest.h"

namespace choice_types
{
namespace uint64_param_choice
{

using allocator_type = UInt64ParamChoice::allocator_type;
using ChoiceTag = UInt64ParamChoice::ChoiceTag;

class UInt64ParamChoiceTest : public ::testing::Test
{
protected:
    static constexpr zserio::UInt64 VARIANT_A_SELECTOR = 1;
    static constexpr zserio::UInt64 VARIANT_B_SELECTOR = 2;
    static constexpr zserio::UInt64 VARIANT_C_SELECTOR = 7;
    static constexpr zserio::UInt64 EMPTY_SELECTOR = 5;
};

TEST_F(UInt64ParamChoiceTest, constructors)
{
    {
        UInt64ParamChoice data;
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        UInt64ParamChoice data = {};
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        UInt64ParamChoice data(allocator_type{});
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        UInt64ParamChoice data;
        const VariantA value = 99;
        data.emplace<ChoiceTag::CHOICE_valueA>(value);
        zserio::View<UInt64ParamChoice> view(data, VARIANT_A_SELECTOR);
        ASSERT_EQ(ChoiceTag::CHOICE_valueA, view.zserioChoiceTag());
    }
}

TEST_F(UInt64ParamChoiceTest, selector)
{
    UInt64ParamChoice data;
    data.emplace<ChoiceTag::CHOICE_valueA>(0);
    zserio::View<UInt64ParamChoice> viewA(data, VARIANT_A_SELECTOR);
    ASSERT_EQ(VARIANT_A_SELECTOR, viewA.selector());

    zserio::View<UInt64ParamChoice> viewB(data, VARIANT_B_SELECTOR);
    ASSERT_EQ(VARIANT_B_SELECTOR, viewB.selector());

    zserio::View<UInt64ParamChoice> viewE(data, EMPTY_SELECTOR);
    ASSERT_EQ(EMPTY_SELECTOR, viewE.selector());
}

TEST_F(UInt64ParamChoiceTest, zserioChoiceTag)
{
    UInt64ParamChoice data;
    data.emplace<ChoiceTag::CHOICE_valueA>(0);
    zserio::View<UInt64ParamChoice> viewA(data, VARIANT_A_SELECTOR);
    ASSERT_EQ(ChoiceTag::CHOICE_valueA, viewA.zserioChoiceTag());

    zserio::View<UInt64ParamChoice> viewB(data, VARIANT_B_SELECTOR);
    ASSERT_EQ(ChoiceTag::CHOICE_valueB, viewB.zserioChoiceTag());

    zserio::View<UInt64ParamChoice> viewC(data, EMPTY_SELECTOR);
    ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, viewC.zserioChoiceTag());
}

TEST_F(UInt64ParamChoiceTest, valueA)
{
    UInt64ParamChoice data;
    const VariantA value = 99;
    data.emplace<ChoiceTag::CHOICE_valueA>(value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueB>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueC>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueA>(data));

    zserio::View<UInt64ParamChoice> view(data, VARIANT_A_SELECTOR);
    ASSERT_THROW(view.valueB(), zserio::BadVariantAccess);
    ASSERT_THROW(view.valueC(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.valueA());
}

TEST_F(UInt64ParamChoiceTest, valueB)
{
    const VariantB value = 234;
    UInt64ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueB>, value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueA>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueC>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueB>(data));

    zserio::View<UInt64ParamChoice> view(data, VARIANT_B_SELECTOR);
    ASSERT_THROW(view.valueA(), zserio::BadVariantAccess);
    ASSERT_THROW(view.valueC(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.valueB());
}

TEST_F(UInt64ParamChoiceTest, valueC)
{
    UInt64ParamChoice data;
    const VariantC value = 23456;
    data.emplace<ChoiceTag::CHOICE_valueC>(value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueA>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueB>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueC>(data));

    zserio::View<UInt64ParamChoice> view(data, VARIANT_C_SELECTOR);
    ASSERT_THROW(view.valueA(), zserio::BadVariantAccess);
    ASSERT_THROW(view.valueB(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.valueC());
}

TEST_F(UInt64ParamChoiceTest, comparisonOperators)
{
    UInt64ParamChoice data;
    data.emplace<ChoiceTag::CHOICE_valueB>(2);

    UInt64ParamChoice equalData;
    equalData.emplace<ChoiceTag::CHOICE_valueB>(2);

    UInt64ParamChoice lessThenData;
    lessThenData.emplace<ChoiceTag::CHOICE_valueA>(2);

    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View<UInt64ParamChoice> view(data, VARIANT_B_SELECTOR);
    zserio::View<UInt64ParamChoice> equalView(equalData, VARIANT_B_SELECTOR);
    zserio::View<UInt64ParamChoice> lessThenView(lessThenData, VARIANT_A_SELECTOR);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST_F(UInt64ParamChoiceTest, validate)
{
    // TODO
}

TEST_F(UInt64ParamChoiceTest, writeRead)
{
    {
        const VariantA value = 99;
        UInt64ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueA>, value);
        test_utils::writeReadTest(data, VARIANT_A_SELECTOR);
    }
    {
        const VariantB value = 234;
        UInt64ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueB>, value);
        test_utils::writeReadTest(data, VARIANT_B_SELECTOR);
    }
}

TEST_F(UInt64ParamChoiceTest, writeReadFile)
{
    const std::string blobNameBase = "language/choice_types/uint64_param_choice_";
    {
        const VariantA value = 99;
        UInt64ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueA>, value);
        test_utils::writeReadFileTest(blobNameBase + "a.blob", data, VARIANT_A_SELECTOR);
    }
    {
        const VariantB value = 234;
        UInt64ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueB>, value);
        test_utils::writeReadFileTest(blobNameBase + "b.blob", data, VARIANT_B_SELECTOR);
    }
}

TEST_F(UInt64ParamChoiceTest, read)
{
    zserio::BitBuffer bitBuffer = zserio::BitBuffer(32);
    zserio::BitStreamWriter writer(bitBuffer);
    const VariantC valueC = 0x0DEADBAD;
    zserio::detail::write(writer, valueC);

    UInt64ParamChoice expectedReadData(zserio::in_place_index<ChoiceTag::CHOICE_valueC>, valueC);
    test_utils::readTest(writer, expectedReadData, VARIANT_C_SELECTOR);
}

TEST_F(UInt64ParamChoiceTest, bitSizeOf)
{
    {
        UInt64ParamChoice data;
        const VariantA value = 99;
        data.emplace<ChoiceTag::CHOICE_valueA>(value);
        zserio::View<UInt64ParamChoice> view(data, VARIANT_A_SELECTOR);
        ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
    }
    {
        const VariantB value = 234;
        UInt64ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueB>, value);
        zserio::View<UInt64ParamChoice> view(data, VARIANT_B_SELECTOR);
        ASSERT_EQ(16, zserio::detail::bitSizeOf(view));
    }
    {
        UInt64ParamChoice data;
        const VariantC value = 23456;
        data.emplace<ChoiceTag::CHOICE_valueC>(value);
        zserio::View<UInt64ParamChoice> view(data, VARIANT_C_SELECTOR);
        ASSERT_EQ(32, zserio::detail::bitSizeOf(view));
    }
}

TEST_F(UInt64ParamChoiceTest, stdHash)
{
    UInt64ParamChoice data;
    data.emplace<ChoiceTag::CHOICE_valueA>(0);
    const size_t dataHash = 31524; // hardcoded value to check that the hash code is stable

    UInt64ParamChoice equalData;
    equalData.emplace<ChoiceTag::CHOICE_valueA>(0);

    UInt64ParamChoice diffData;
    const VariantA value = 99;
    diffData.emplace<ChoiceTag::CHOICE_valueA>(value);
    const size_t diffDataHash = 31623; // hardcoded value to check that the hash code is stable

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View<UInt64ParamChoice> view(data, VARIANT_A_SELECTOR);
    const size_t viewHash = 851; // hardcoded value to check that the hash code is stable
    zserio::View<UInt64ParamChoice> equalView(equalData, VARIANT_A_SELECTOR);
    zserio::View<UInt64ParamChoice> diffView(diffData, VARIANT_A_SELECTOR);
    const size_t diffHash = 950; // hardcoded value to check that the hash code is stable
    test_utils::hashTest(view, viewHash, equalView, diffView, diffHash);
}

} // namespace uint64_param_choice
} // namespace choice_types
