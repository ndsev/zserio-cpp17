#include <string_view>
#include <utility>

#include "choice_types/uint64_param_choice/UInt64ParamChoice.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ChoiceCaseException.h"

namespace choice_types
{
namespace uint64_param_choice
{

using AllocatorType = UInt64ParamChoice::allocator_type;

class UInt64ParamChoiceTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, VariantC valueC)
    {
        zserio::detail::write(writer, valueC);
    }

    static constexpr std::string_view BLOB_NAME_BASE = "language/choice_types/uint64_param_choice_";
    static constexpr zserio::UInt64 VARIANT_A_SELECTOR = 1;
    static constexpr zserio::UInt64 VARIANT_B_SELECTOR = 2;
    static constexpr zserio::UInt64 VARIANT_C_SELECTOR = 7;
    static constexpr zserio::UInt64 EMPTY_SELECTOR = 5;
};

TEST_F(UInt64ParamChoiceTest, constructors)
{
    {
        UInt64ParamChoice data;
        ASSERT_EQ(UInt64ParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        UInt64ParamChoice data = {};
        ASSERT_EQ(UInt64ParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        UInt64ParamChoice data(AllocatorType{});
        ASSERT_EQ(UInt64ParamChoice::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        UInt64ParamChoice data;
        const VariantA value = 99;
        data.emplace<UInt64ParamChoice::Tag::valueA>(value);
        zserio::View view(data, VARIANT_A_SELECTOR);
        ASSERT_EQ(UInt64ParamChoice::Tag::valueA, view.zserioChoiceTag());
    }
}

TEST_F(UInt64ParamChoiceTest, copyConstructor)
{
    UInt64ParamChoice data;
    const VariantA value = 99;
    data.emplace<UInt64ParamChoice::Tag::valueA>(value);
    UInt64ParamChoice dataCopy(data);
    ASSERT_EQ(dataCopy, data);

    zserio::View view(data, VARIANT_A_SELECTOR);
    zserio::View viewCopy(view);
    ASSERT_EQ(viewCopy, view);
}

TEST_F(UInt64ParamChoiceTest, assignmentOperator)
{
    UInt64ParamChoice data;
    const VariantA value = 99;
    data.emplace<UInt64ParamChoice::Tag::valueA>(value);
    UInt64ParamChoice dataCopy;
    dataCopy = data;
    ASSERT_EQ(dataCopy, data);

    zserio::View view(data, VARIANT_A_SELECTOR);
    UInt64ParamChoice dummyData;
    zserio::View viewCopy(dummyData, VARIANT_B_SELECTOR);
    viewCopy = view;
    ASSERT_EQ(viewCopy, view);
}

TEST_F(UInt64ParamChoiceTest, moveConstructor)
{
    UInt64ParamChoice data;
    const VariantA value = 99;
    data.emplace<UInt64ParamChoice::Tag::valueA>(value);
    UInt64ParamChoice dataMoved(std::move(data));
    ASSERT_EQ(value, zserio::get<UInt64ParamChoice::Tag::valueA>(dataMoved));

    zserio::View view(dataMoved, VARIANT_A_SELECTOR);
    zserio::View viewMoved(std::move(view));
    ASSERT_EQ(value, viewMoved.valueA());
}

TEST_F(UInt64ParamChoiceTest, moveAssignmentOperator)
{
    UInt64ParamChoice data;
    const VariantA value = 99;
    data.emplace<UInt64ParamChoice::Tag::valueA>(value);
    UInt64ParamChoice dataMoved;
    dataMoved = std::move(data);
    ASSERT_EQ(value, zserio::get<UInt64ParamChoice::Tag::valueA>(dataMoved));

    zserio::View view(dataMoved, VARIANT_A_SELECTOR);
    UInt64ParamChoice dummyData;
    zserio::View viewMoved(dummyData, VARIANT_B_SELECTOR);
    viewMoved = std::move(view);
    ASSERT_EQ(value, viewMoved.valueA());
}

TEST_F(UInt64ParamChoiceTest, selector)
{
    UInt64ParamChoice data;
    data.emplace<UInt64ParamChoice::Tag::valueA>(0);
    zserio::View viewA(data, VARIANT_A_SELECTOR);
    ASSERT_EQ(VARIANT_A_SELECTOR, viewA.selector());

    zserio::View viewB(data, VARIANT_B_SELECTOR);
    ASSERT_EQ(VARIANT_B_SELECTOR, viewB.selector());

    zserio::View viewE(data, EMPTY_SELECTOR);
    ASSERT_EQ(EMPTY_SELECTOR, viewE.selector());
}

TEST_F(UInt64ParamChoiceTest, zserioChoiceTag)
{
    {
        UInt64ParamChoice data;
        data.emplace<UInt64ParamChoice::Tag::valueA>(0);
        zserio::View view(data, VARIANT_A_SELECTOR);
        ASSERT_EQ(UInt64ParamChoice::Tag::valueA, view.zserioChoiceTag());
    }
    {
        UInt64ParamChoice data;
        data.emplace<UInt64ParamChoice::Tag::valueB>(0);
        zserio::View viewB(data, VARIANT_B_SELECTOR);
        ASSERT_EQ(UInt64ParamChoice::Tag::valueB, viewB.zserioChoiceTag());
    }
    {
        UInt64ParamChoice data;
        data.emplace<UInt64ParamChoice::Tag::valueC>(0);
        zserio::View viewC(data, EMPTY_SELECTOR);
        ASSERT_EQ(UInt64ParamChoice::Tag::valueC, viewC.zserioChoiceTag());
    }
}

TEST_F(UInt64ParamChoiceTest, valueA)
{
    UInt64ParamChoice data;
    const VariantA value = 99;
    data.emplace<UInt64ParamChoice::Tag::valueA>(value);
    ASSERT_THROW(zserio::get<UInt64ParamChoice::Tag::valueB>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<UInt64ParamChoice::Tag::valueC>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<UInt64ParamChoice::Tag::valueA>(data));

    zserio::View view(data, VARIANT_A_SELECTOR);
    ASSERT_THROW(view.valueB(), zserio::BadVariantAccess);
    ASSERT_THROW(view.valueC(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.valueA());
}

TEST_F(UInt64ParamChoiceTest, valueB)
{
    const VariantB value = 234;
    UInt64ParamChoice data(zserio::in_place_index<UInt64ParamChoice::Tag::valueB>, value);
    ASSERT_THROW(zserio::get<UInt64ParamChoice::Tag::valueA>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<UInt64ParamChoice::Tag::valueC>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<UInt64ParamChoice::Tag::valueB>(data));

    zserio::View view(data, VARIANT_B_SELECTOR);
    ASSERT_THROW(view.valueA(), zserio::BadVariantAccess);
    ASSERT_THROW(view.valueC(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.valueB());
}

TEST_F(UInt64ParamChoiceTest, valueC)
{
    UInt64ParamChoice data;
    const VariantC value = 23456;
    data.emplace<UInt64ParamChoice::Tag::valueC>(value);
    ASSERT_THROW(zserio::get<UInt64ParamChoice::Tag::valueA>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<UInt64ParamChoice::Tag::valueB>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<UInt64ParamChoice::Tag::valueC>(data));

    zserio::View view(data, VARIANT_C_SELECTOR);
    ASSERT_THROW(view.valueA(), zserio::BadVariantAccess);
    ASSERT_THROW(view.valueB(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.valueC());
}

TEST_F(UInt64ParamChoiceTest, comparisonOperators)
{
    UInt64ParamChoice data;
    data.emplace<UInt64ParamChoice::Tag::valueB>(2);
    UInt64ParamChoice equalData;
    equalData.emplace<UInt64ParamChoice::Tag::valueB>(2);
    UInt64ParamChoice lessThenData;
    lessThenData.emplace<UInt64ParamChoice::Tag::valueA>(2);
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data, VARIANT_B_SELECTOR);
    zserio::View equalView(equalData, VARIANT_B_SELECTOR);
    zserio::View lessThenView(lessThenData, VARIANT_A_SELECTOR);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);

    UInt64ParamChoice anotherLessThenData;
    anotherLessThenData.emplace<UInt64ParamChoice::Tag::valueB>(1);
    test_utils::comparisonOperatorsTest(data, equalData, anotherLessThenData);

    zserio::View anotherLessThenView(anotherLessThenData, VARIANT_A_SELECTOR);
    test_utils::comparisonOperatorsTest(view, equalView, anotherLessThenView);
}

TEST_F(UInt64ParamChoiceTest, validate)
{
    {
        UInt64ParamChoice data;
        const VariantA value = 99;
        data.emplace<UInt64ParamChoice::Tag::valueA>(value);
        zserio::View viewB(data, VARIANT_B_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
        zserio::View viewC(data, VARIANT_C_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewC), zserio::ChoiceCaseException);
    }
    {
        const VariantB value = 234;
        UInt64ParamChoice data(zserio::in_place_index<UInt64ParamChoice::Tag::valueB>, value);
        zserio::View viewA(data, VARIANT_A_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewA), zserio::ChoiceCaseException);
        zserio::View viewC(data, VARIANT_C_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewC), zserio::ChoiceCaseException);
    }
    {
        UInt64ParamChoice data;
        const VariantC value = 23456;
        data.emplace<UInt64ParamChoice::Tag::valueC>(value);
        zserio::View viewA(data, VARIANT_A_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewA), zserio::ChoiceCaseException);
        zserio::View viewB(data, VARIANT_B_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
    }
    {
        UInt64ParamChoice data;
        const VariantA value = 99;
        data.emplace<UInt64ParamChoice::Tag::valueA>(value);
        zserio::View view(data, EMPTY_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(view), zserio::ChoiceCaseException);
    }
    {
        UInt64ParamChoice data;
        zserio::View viewA(data, VARIANT_A_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewA), zserio::ChoiceCaseException);
        zserio::View viewB(data, VARIANT_B_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewB), zserio::ChoiceCaseException);
        zserio::View viewC(data, VARIANT_C_SELECTOR);
        ASSERT_THROW(zserio::detail::validate(viewC), zserio::ChoiceCaseException);
    }
}

TEST_F(UInt64ParamChoiceTest, bitSizeOf)
{
    {
        UInt64ParamChoice data;
        const VariantA value = 99;
        data.emplace<UInt64ParamChoice::Tag::valueA>(value);
        zserio::View view(data, VARIANT_A_SELECTOR);
        ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
    }
    {
        const VariantB value = 234;
        UInt64ParamChoice data(zserio::in_place_index<UInt64ParamChoice::Tag::valueB>, value);
        zserio::View view(data, VARIANT_B_SELECTOR);
        ASSERT_EQ(16, zserio::detail::bitSizeOf(view));
    }
    {
        UInt64ParamChoice data;
        const VariantC value = 23456;
        data.emplace<UInt64ParamChoice::Tag::valueC>(value);
        zserio::View view(data, VARIANT_C_SELECTOR);
        ASSERT_EQ(32, zserio::detail::bitSizeOf(view));
    }
}

TEST_F(UInt64ParamChoiceTest, writeRead)
{
    {
        const VariantA value = 99;
        UInt64ParamChoice data(zserio::in_place_index<UInt64ParamChoice::Tag::valueA>, value);
        test_utils::writeReadTest(data, VARIANT_A_SELECTOR);
    }
    {
        const VariantB value = 234;
        UInt64ParamChoice data(zserio::in_place_index<UInt64ParamChoice::Tag::valueB>, value);
        test_utils::writeReadTest(data, VARIANT_B_SELECTOR);
    }
}

TEST_F(UInt64ParamChoiceTest, writeReadFile)
{
    {
        const VariantA value = 99;
        UInt64ParamChoice data(zserio::in_place_index<UInt64ParamChoice::Tag::valueA>, value);
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "a.blob", data, VARIANT_A_SELECTOR);
    }
    {
        const VariantB value = 234;
        UInt64ParamChoice data(zserio::in_place_index<UInt64ParamChoice::Tag::valueB>, value);
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "b.blob", data, VARIANT_B_SELECTOR);
    }
}

TEST_F(UInt64ParamChoiceTest, read)
{
    const VariantC valueC = 0x0DEADBAD;
    UInt64ParamChoice expectedReadData(zserio::in_place_index<UInt64ParamChoice::Tag::valueC>, valueC);

    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, valueC), expectedReadData, VARIANT_C_SELECTOR);
}

TEST_F(UInt64ParamChoiceTest, stdHash)
{
    UInt64ParamChoice data;
    data.emplace<UInt64ParamChoice::Tag::valueA>(0);
    const size_t dataHash = 31524; // hardcoded value to check that the hash code is stable

    UInt64ParamChoice equalData;
    equalData.emplace<UInt64ParamChoice::Tag::valueA>(0);

    UInt64ParamChoice diffData;
    const VariantA value = 99;
    diffData.emplace<UInt64ParamChoice::Tag::valueA>(value);
    const size_t diffDataHash = 31623; // hardcoded value to check that the hash code is stable

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data, VARIANT_A_SELECTOR);
    const size_t viewHash = 31524; // hardcoded value to check that the hash code is stable
    zserio::View equalView(equalData, VARIANT_A_SELECTOR);
    zserio::View diffView(diffData, VARIANT_A_SELECTOR);
    const size_t diffHash = 31623; // hardcoded value to check that the hash code is stable
    test_utils::hashTest(view, viewHash, equalView, diffView, diffHash);
}

} // namespace uint64_param_choice
} // namespace choice_types
