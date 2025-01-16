#include "choice_types/constant_in_choice_case/ConstantInChoiceCase.h"
#include "choice_types/constant_in_choice_case/UINT8_CONST.h"
#include "gtest/gtest.h"
#include "test_utils/WriteReadTest.h"

namespace choice_types
{
namespace constant_in_choice_case
{

TEST(ConstantInChoiceCaseTest, writeRead)
{
    ConstantInChoiceCase data(zserio::in_place_index<ConstantInChoiceCase::ChoiceTag::CHOICE_constCase>, 42);
    test_utils::writeReadTest(data, UINT8_CONST);
}

} // namespace constant_in_choice_case
} // namespace choice_types
