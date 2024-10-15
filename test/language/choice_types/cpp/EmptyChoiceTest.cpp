#include "choice_types/empty_choice/EmptyChoice.h"
#include "gtest/gtest.h"
#include "zserio/CppRuntimeException.h"

namespace choice_types
{
namespace empty_choice
{

using allocator_type = EmptyChoice::allocator_type;

TEST(EmptyChoiceDataTest, emptyConstructor)
{
    {
        EmptyChoice emptyChoice;
        ASSERT_EQ(EmptyChoice::ChoiceTag::UNDEFINED_CHOICE, emptyChoice.index());
    }
    {
        EmptyChoice emptyChoice = {};
        ASSERT_EQ(EmptyChoice::ChoiceTag::UNDEFINED_CHOICE, emptyChoice.index());
    }

    {
        EmptyChoice emptyChoice(allocator_type{});
        ASSERT_EQ(EmptyChoice::ChoiceTag::UNDEFINED_CHOICE, emptyChoice.index());
    }
}

TEST(EmptyChoiceDataTest, copyConstructor)
{
    EmptyChoice emptyChoice;
    const EmptyChoice emptyChoiceCopy(emptyChoice);
    ASSERT_EQ(emptyChoice, emptyChoiceCopy);
}

TEST(EmptyChoiceDataTest, assignmentOperator)
{
    EmptyChoice emptyChoice;
    EmptyChoice emptyChoiceCopy;
    emptyChoiceCopy = emptyChoice;
    ASSERT_EQ(emptyChoice, emptyChoiceCopy);
}

TEST(EmptyChoiceDataTest, moveConstructor)
{
    EmptyChoice emptyChoice;
    const EmptyChoice emptyChoiceMoved(std::move(emptyChoice));
    (void)emptyChoiceMoved;
}

TEST(EmptyChoiceDataTest, moveAssignmentOperator)
{
    EmptyChoice emptyChoice;
    EmptyChoice emptyChoiceMoved;
    emptyChoiceMoved = std::move(emptyChoice);
    (void)emptyChoiceMoved;
}

TEST(EmptyChoiceDataTest, index)
{
    EmptyChoice emptyChoice;
    ASSERT_EQ(EmptyChoice::ChoiceTag::UNDEFINED_CHOICE, emptyChoice.index());
}

TEST(EmptyChoiceDataTest, operatorEquality)
{
    EmptyChoice emptyChoice1;
    EmptyChoice emptyChoice2;
    ASSERT_TRUE(emptyChoice1 == emptyChoice2);
}

TEST(EmptyChoiceDataTest, operatorLessThan)
{
    EmptyChoice emptyChoice1;
    EmptyChoice emptyChoice2;
    ASSERT_FALSE(emptyChoice1 < emptyChoice2);
    ASSERT_FALSE(emptyChoice2 < emptyChoice1);
}

TEST(EmptyChoiceDataTest, stdHash)
{
    std::hash<EmptyChoice> hasher;

    EmptyChoice emptyChoice1;
    EmptyChoice emptyChoice2;
    ASSERT_EQ(hasher(emptyChoice1), hasher(emptyChoice2));

    // TODO[Mi-L@]: hashCode not stable across extensions!
    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(23, hasher(emptyChoice1));
}

} // namespace empty_choice
} // namespace choice_types
