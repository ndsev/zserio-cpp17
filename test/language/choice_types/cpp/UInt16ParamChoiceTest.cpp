#include "choice_types/uint16_param_choice/UInt16ParamChoice.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ChoiceCaseException.h"

namespace choice_types
{
namespace uint16_param_choice
{

using AllocatorType = UInt16ParamChoice::AllocatorType;
using ChoiceTag = UInt16ParamChoice::ChoiceTag;

class UInt16ParamChoiceTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, uint16_t selector, int32_t value)
    {
        switch (selector)
        {
        case 1:
            writer.writeSignedBits32(value, 8);
            break;

        case 2:
        case 3:
        case 4:
            writer.writeSignedBits32(value, 16);
            break;

        case 5:
        case 6:
            break;

        default:
            writer.writeSignedBits32(value, 32);
            break;
        }
    }

    static constexpr zserio::UInt16 VARIANT_A_SELECTOR = 1;
    static constexpr zserio::UInt16 VARIANT_B_SELECTOR1 = 2;
    static constexpr zserio::UInt16 VARIANT_B_SELECTOR2 = 3;
    static constexpr zserio::UInt16 VARIANT_B_SELECTOR3 = 4;
    static constexpr zserio::UInt16 EMPTY_SELECTOR1 = 5;
    static constexpr zserio::UInt16 EMPTY_SELECTOR2 = 6;
    static constexpr zserio::UInt16 VARIANT_C_SELECTOR = 7;
};

TEST_F(UInt16ParamChoiceTest, constructors)
{
    {
        UInt16ParamChoice data;
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        UInt16ParamChoice data = {};
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        UInt16ParamChoice data(AllocatorType{});
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        UInt16ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueA>, 99);
        zserio::View view(data, VARIANT_A_SELECTOR);
        ASSERT_EQ(ChoiceTag::CHOICE_valueA, view.zserioChoiceTag());
    }
}

TEST_F(UInt16ParamChoiceTest, copyConstructor)
{
    UInt16ParamChoice data;
    const VariantA value = 99;
    data.emplace<ChoiceTag::CHOICE_valueA>(value);
    UInt16ParamChoice dataCopy(data);
    ASSERT_EQ(dataCopy, data);

    zserio::View view(data, VARIANT_A_SELECTOR);
    zserio::View viewCopy(view);
    ASSERT_EQ(viewCopy, view);
}

TEST_F(UInt16ParamChoiceTest, assignmentOperator)
{
    UInt16ParamChoice data;
    const VariantA value = 99;
    data.emplace<ChoiceTag::CHOICE_valueA>(value);
    UInt16ParamChoice dataCopy;
    dataCopy = data;
    ASSERT_EQ(dataCopy, data);

    zserio::View view(data, VARIANT_A_SELECTOR);
    UInt16ParamChoice dummyData;
    zserio::View viewCopy(dummyData, VARIANT_B_SELECTOR1);
    viewCopy = view;
    ASSERT_EQ(viewCopy, view);
    ASSERT_EQ(&data, &viewCopy.zserioData());
}

TEST_F(UInt16ParamChoiceTest, moveConstructor)
{
    UInt16ParamChoice data;
    const VariantA value = 99;
    data.emplace<ChoiceTag::CHOICE_valueA>(value);
    UInt16ParamChoice dataMoved(std::move(data));
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueA>(dataMoved));

    zserio::View view(dataMoved, VARIANT_A_SELECTOR);
    zserio::View viewMoved(std::move(view));
    ASSERT_EQ(value, viewMoved.valueA());
}

TEST_F(UInt16ParamChoiceTest, moveAssignmentOperator)
{
    UInt16ParamChoice data;
    const VariantA value = 99;
    data.emplace<ChoiceTag::CHOICE_valueA>(value);
    UInt16ParamChoice dataMoved;
    dataMoved = std::move(data);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueA>(dataMoved));

    zserio::View view(dataMoved, VARIANT_A_SELECTOR);
    UInt16ParamChoice dummyData;
    zserio::View viewMoved(dummyData, VARIANT_B_SELECTOR2);
    viewMoved = std::move(view);
    ASSERT_EQ(value, viewMoved.valueA());
    ASSERT_EQ(&dataMoved, &viewMoved.zserioData());
}

TEST_F(UInt16ParamChoiceTest, selector)
{
    UInt16ParamChoice data;
    data.emplace<ChoiceTag::CHOICE_valueA>(0);
    zserio::View viewA(data, VARIANT_A_SELECTOR);
    ASSERT_EQ(VARIANT_A_SELECTOR, viewA.selector());

    zserio::View viewB(data, VARIANT_B_SELECTOR3);
    ASSERT_EQ(VARIANT_B_SELECTOR3, viewB.selector());

    zserio::View viewE(data, EMPTY_SELECTOR1);
    ASSERT_EQ(EMPTY_SELECTOR1, viewE.selector());
}

TEST_F(UInt16ParamChoiceTest, zserioChoiceTag)
{
    {
        UInt16ParamChoice data;
        data.emplace<ChoiceTag::CHOICE_valueA>(0);
        zserio::View view(data, VARIANT_A_SELECTOR);
        ASSERT_EQ(ChoiceTag::CHOICE_valueA, view.zserioChoiceTag());
    }
    {
        UInt16ParamChoice data;
        data.emplace<ChoiceTag::CHOICE_valueB>(0);
        zserio::View viewB(data, VARIANT_B_SELECTOR1);
        ASSERT_EQ(ChoiceTag::CHOICE_valueB, viewB.zserioChoiceTag());
    }
    {
        UInt16ParamChoice data;
        data.emplace<ChoiceTag::CHOICE_valueC>(0);
        zserio::View viewC(data, EMPTY_SELECTOR2);
        ASSERT_EQ(ChoiceTag::CHOICE_valueC, viewC.zserioChoiceTag());
    }
}

TEST_F(UInt16ParamChoiceTest, valueA)
{
    UInt16ParamChoice data;
    const VariantA value = 99;
    data.emplace<ChoiceTag::CHOICE_valueA>(value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueB>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueC>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueA>(data));

    zserio::View view(data, VARIANT_A_SELECTOR);
    ASSERT_THROW(view.valueB(), zserio::BadVariantAccess);
    ASSERT_THROW(view.valueC(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.valueA());
}

TEST_F(UInt16ParamChoiceTest, valueB)
{
    const VariantB value = 234;
    UInt16ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueB>, value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueA>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueC>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueB>(data));

    zserio::View view(data, VARIANT_B_SELECTOR3);
    ASSERT_THROW(view.valueA(), zserio::BadVariantAccess);
    ASSERT_THROW(view.valueC(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.valueB());
}

TEST_F(UInt16ParamChoiceTest, valueC)
{
    UInt16ParamChoice data;
    const VariantC value = 23456;
    data.emplace<ChoiceTag::CHOICE_valueC>(value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueA>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueB>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueC>(data));

    zserio::View view(data, VARIANT_C_SELECTOR);
    ASSERT_THROW(view.valueA(), zserio::BadVariantAccess);
    ASSERT_THROW(view.valueB(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.valueC());
}

TEST_F(UInt16ParamChoiceTest, comparisonOperators)
{
    UInt16ParamChoice data;
    data.emplace<ChoiceTag::CHOICE_valueB>(2);
    UInt16ParamChoice equalData;
    equalData.emplace<ChoiceTag::CHOICE_valueB>(2);
    UInt16ParamChoice lessThenData;
    lessThenData.emplace<ChoiceTag::CHOICE_valueA>(2);
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data, VARIANT_B_SELECTOR2);
    zserio::View equalView(equalData, VARIANT_B_SELECTOR2);
    zserio::View lessThenView(lessThenData, VARIANT_A_SELECTOR);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);

    UInt16ParamChoice lessThenDataViaSelector;
    lessThenDataViaSelector.emplace<ChoiceTag::CHOICE_valueB>(2);
    zserio::View lessThenViewViaSelector(lessThenDataViaSelector, VARIANT_B_SELECTOR1);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenViewViaSelector);
}

TEST_F(UInt16ParamChoiceTest, validate)
{
    {
        UInt16ParamChoice data;
        const VariantA value = 99;
        data.emplace<ChoiceTag::CHOICE_valueA>(value);
        zserio::View viewB(data, VARIANT_B_SELECTOR3);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
        zserio::View viewC(data, VARIANT_C_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewC), zserio::ChoiceCaseException);
    }
    {
        const VariantB value = 234;
        UInt16ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueB>, value);
        zserio::View viewA(data, VARIANT_A_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewA), zserio::ChoiceCaseException);
        zserio::View viewC(data, VARIANT_C_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewC), zserio::ChoiceCaseException);
    }
    {
        UInt16ParamChoice data;
        const VariantC value = 23456;
        data.emplace<ChoiceTag::CHOICE_valueC>(value);
        zserio::View viewA(data, VARIANT_A_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewA), zserio::ChoiceCaseException);
        zserio::View viewB(data, VARIANT_B_SELECTOR1);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
    }
}

TEST_F(UInt16ParamChoiceTest, bitSizeOf)
{
    {
        UInt16ParamChoice data;
        const VariantA value = 99;
        data.emplace<ChoiceTag::CHOICE_valueA>(value);
        zserio::View view(data, VARIANT_A_SELECTOR);
        ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
    }
    {
        const VariantB value = 234;
        UInt16ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueB>, value);
        zserio::View view(data, VARIANT_B_SELECTOR3);
        ASSERT_EQ(16, zserio::detail::bitSizeOf(view));
    }
    {
        UInt16ParamChoice data;
        const VariantC value = 23456;
        data.emplace<ChoiceTag::CHOICE_valueC>(value);
        zserio::View view(data, VARIANT_C_SELECTOR);
        ASSERT_EQ(32, zserio::detail::bitSizeOf(view));
    }
}

TEST_F(UInt16ParamChoiceTest, writeRead)
{
    {
        const VariantA value = 99;
        UInt16ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueA>, value);
        test_utils::writeReadTest(data, VARIANT_A_SELECTOR);
    }
    {
        const VariantB value = 234;
        UInt16ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueB>, value);
        test_utils::writeReadTest(data, VARIANT_B_SELECTOR2);
    }
    {
        const VariantC value = 23456;
        UInt16ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueC>, value);
        test_utils::writeReadTest(data, VARIANT_C_SELECTOR);
    }
}

TEST_F(UInt16ParamChoiceTest, read)
{
    {
        const zserio::UInt16 selector = VARIANT_A_SELECTOR;
        const int8_t value = 99;
        UInt16ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueA>, value);

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
    {
        const zserio::UInt16 selector = VARIANT_B_SELECTOR1;
        const int16_t value = 234;
        UInt16ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueB>, value);

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
    {
        const zserio::UInt16 selector = VARIANT_C_SELECTOR;
        const int32_t value = 23456;
        UInt16ParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_valueC>, value);

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, selector, value), data, selector);
    }
}

TEST_F(UInt16ParamChoiceTest, stdHash)
{
    UInt16ParamChoice data;
    data.emplace<ChoiceTag::CHOICE_valueA>(0);
    const size_t dataHash = 31524; // hardcoded value to check that the hash code is stable

    UInt16ParamChoice equalData;
    equalData.emplace<ChoiceTag::CHOICE_valueA>(0);

    UInt16ParamChoice diffData;
    const VariantA value = 99;
    diffData.emplace<ChoiceTag::CHOICE_valueA>(value);
    const size_t diffDataHash = 31623; // hardcoded value to check that the hash code is stable

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data, VARIANT_A_SELECTOR);
    const size_t viewHash = 31524; // hardcoded value to check that the hash code is stable
    zserio::View equalView(equalData, VARIANT_A_SELECTOR);
    zserio::View diffView(diffData, VARIANT_A_SELECTOR);
    const size_t diffHash = 31623; // hardcoded value to check that the hash code is stable
    test_utils::hashTest(view, viewHash, equalView, diffView, diffHash);
}

} // namespace uint16_param_choice
} // namespace choice_types
