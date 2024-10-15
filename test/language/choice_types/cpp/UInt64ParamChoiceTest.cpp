#include "choice_types/uint64_param_choice/UInt64ParamChoice.h"
#include "gtest/gtest.h"

namespace choice_types
{
namespace uint64_param_choice
{

using allocator_type = UInt64ParamChoice::allocator_type;
using ChoiceTag = UInt64ParamChoice::ChoiceTag;

class UInt64ParamChoiceDataTest : public ::testing::Test
{
protected:
    static constexpr uint64_t VARIANT_A_SELECTOR = 1;
    static constexpr uint64_t VARIANT_B_SELECTOR = 2;
    static constexpr uint64_t VARIANT_C_SELECTOR = 7;
    static constexpr uint64_t EMPTY_SELECTOR = 5;
};

TEST_F(UInt64ParamChoiceDataTest, emptyConstructor)
{
    {
        UInt64ParamChoice uint64ParamChoice;
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, uint64ParamChoice.index());
    }
    {
        UInt64ParamChoice uint64ParamChoice = {};
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, uint64ParamChoice.index());
    }
    {
        UInt64ParamChoice uint64ParamChoice(allocator_type{});
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, uint64ParamChoice.index());
    }
}

TEST_F(UInt64ParamChoiceDataTest, copyConstructor)
{
    UInt64ParamChoice uint64ParamChoice;
    const int8_t value = 99;
    uint64ParamChoice.emplace<ChoiceTag::CHOICE_valueA>(value);

    const UInt64ParamChoice uint64ParamChoiceCopy(uint64ParamChoice);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueA>(uint64ParamChoiceCopy));
}

TEST_F(UInt64ParamChoiceDataTest, assignmentOperator)
{
    const int16_t value = 234;
    UInt64ParamChoice uint64ParamChoice(zserio::in_place_index<ChoiceTag::CHOICE_valueB>, value);

    UInt64ParamChoice uint64ParamChoiceCopy;
    uint64ParamChoiceCopy = uint64ParamChoice;
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueB>(uint64ParamChoiceCopy));
}

TEST_F(UInt64ParamChoiceDataTest, moveConstructor)
{
    UInt64ParamChoice uint64ParamChoice;
    const int8_t value = 99;
    uint64ParamChoice.emplace<ChoiceTag::CHOICE_valueA>(value);

    const UInt64ParamChoice uint64ParamChoiceMoved(std::move(uint64ParamChoice));
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueA>(uint64ParamChoiceMoved));
}

TEST_F(UInt64ParamChoiceDataTest, moveAssignmentOperator)
{
    UInt64ParamChoice uint64ParamChoice;
    const int16_t value = 234;
    uint64ParamChoice.emplace<ChoiceTag::CHOICE_valueB>(value);

    UInt64ParamChoice uint64ParamChoiceMoved;
    uint64ParamChoiceMoved = std::move(uint64ParamChoice);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueB>(uint64ParamChoiceMoved));
}

TEST_F(UInt64ParamChoiceDataTest, getValueA)
{
    UInt64ParamChoice uint64ParamChoice;
    const int8_t value = 99;
    uint64ParamChoice.emplace<ChoiceTag::CHOICE_valueA>(value);

    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueB>(uint64ParamChoice), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueC>(uint64ParamChoice), zserio::BadVariantAccess);

    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueA>(uint64ParamChoice));
}

TEST_F(UInt64ParamChoiceDataTest, getValueB)
{
    const int16_t value = 234;
    UInt64ParamChoice uint64ParamChoice(zserio::in_place_index<ChoiceTag::CHOICE_valueB>, value);

    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueA>(uint64ParamChoice), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueC>(uint64ParamChoice), zserio::BadVariantAccess);

    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueB>(uint64ParamChoice));
}

TEST_F(UInt64ParamChoiceDataTest, getValueC)
{
    UInt64ParamChoice uint64ParamChoice;
    const int32_t value = 23456;
    uint64ParamChoice.emplace<ChoiceTag::CHOICE_valueC>(value);

    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueA>(uint64ParamChoice), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_valueB>(uint64ParamChoice), zserio::BadVariantAccess);

    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_valueC>(uint64ParamChoice));
}

TEST_F(UInt64ParamChoiceDataTest, index)
{
    UInt64ParamChoice uint64ParamChoice;
    uint64ParamChoice.emplace<ChoiceTag::CHOICE_valueA>(0);
    ASSERT_EQ(ChoiceTag::CHOICE_valueA, uint64ParamChoice.index());

    uint64ParamChoice.emplace<ChoiceTag::CHOICE_valueB>(0);
    ASSERT_EQ(ChoiceTag::CHOICE_valueB, uint64ParamChoice.index());

    uint64ParamChoice.emplace<ChoiceTag::CHOICE_valueC>(0);
    ASSERT_EQ(ChoiceTag::CHOICE_valueC, uint64ParamChoice.index());

    uint64ParamChoice.emplace<ChoiceTag::UNDEFINED_CHOICE>();
    ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, uint64ParamChoice.index());
}

TEST_F(UInt64ParamChoiceDataTest, operatorEquality)
{
    UInt64ParamChoice uint64ParamChoice1;
    uint64ParamChoice1.emplace<ChoiceTag::CHOICE_valueA>(0);
    UInt64ParamChoice uint64ParamChoice2;
    uint64ParamChoice2.emplace<ChoiceTag::CHOICE_valueA>(0);
    ASSERT_TRUE(uint64ParamChoice1 == uint64ParamChoice2);

    const int8_t value = 99;
    uint64ParamChoice1.emplace<ChoiceTag::CHOICE_valueA>(value);
    ASSERT_FALSE(uint64ParamChoice1 == uint64ParamChoice2);

    uint64ParamChoice2.emplace<ChoiceTag::CHOICE_valueA>(value);
    ASSERT_TRUE(uint64ParamChoice1 == uint64ParamChoice2);

    uint64ParamChoice2.emplace<ChoiceTag::CHOICE_valueB>(value);
    ASSERT_FALSE(uint64ParamChoice1 == uint64ParamChoice2);
}

TEST_F(UInt64ParamChoiceDataTest, operatorLessThan)
{
    UInt64ParamChoice uint64ParamChoice1;
    uint64ParamChoice1.emplace<ChoiceTag::CHOICE_valueA>(0);
    UInt64ParamChoice uint64ParamChoice2;
    uint64ParamChoice2.emplace<ChoiceTag::CHOICE_valueB>(0);
    ASSERT_TRUE(uint64ParamChoice1 < uint64ParamChoice2);
    ASSERT_FALSE(uint64ParamChoice2 < uint64ParamChoice1);

    uint64ParamChoice2.emplace<ChoiceTag::CHOICE_valueA>(0);
    ASSERT_FALSE(uint64ParamChoice1 < uint64ParamChoice2);
    ASSERT_FALSE(uint64ParamChoice2 < uint64ParamChoice1);

    const int8_t value = 99;
    uint64ParamChoice1.emplace<ChoiceTag::CHOICE_valueA>(value);
    ASSERT_FALSE(uint64ParamChoice1 < uint64ParamChoice2);
    ASSERT_TRUE(uint64ParamChoice2 < uint64ParamChoice1);

    uint64ParamChoice2.emplace<ChoiceTag::CHOICE_valueA>(value);
    ASSERT_FALSE(uint64ParamChoice1 < uint64ParamChoice2);
    ASSERT_FALSE(uint64ParamChoice2 < uint64ParamChoice1);
}

TEST_F(UInt64ParamChoiceDataTest, stdHash)
{
    std::hash<UInt64ParamChoice> hasher;

    UInt64ParamChoice uint64ParamChoice1;
    uint64ParamChoice1.emplace<ChoiceTag::CHOICE_valueA>(0);
    UInt64ParamChoice uint64ParamChoice2;
    uint64ParamChoice2.emplace<ChoiceTag::CHOICE_valueA>(0);
    ASSERT_EQ(hasher(uint64ParamChoice1), hasher(uint64ParamChoice2));

    const int8_t value = 99;
    uint64ParamChoice1.emplace<ChoiceTag::CHOICE_valueA>(value);
    ASSERT_NE(hasher(uint64ParamChoice1), hasher(uint64ParamChoice2));

    uint64ParamChoice2.emplace<ChoiceTag::CHOICE_valueA>(value);
    ASSERT_EQ(hasher(uint64ParamChoice1), hasher(uint64ParamChoice2));

    uint64ParamChoice2.emplace<ChoiceTag::CHOICE_valueB>(value);
    ASSERT_NE(hasher(uint64ParamChoice1), hasher(uint64ParamChoice2));

    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(31623, hasher(uint64ParamChoice1));
    ASSERT_EQ(31660, hasher(uint64ParamChoice2));
}

} // namespace uint64_param_choice
} // namespace choice_types
