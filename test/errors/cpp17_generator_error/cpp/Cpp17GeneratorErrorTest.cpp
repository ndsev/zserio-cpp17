#include "gtest/gtest.h"
#include "test_utils/ZserioErrorOutput.h"

class Cpp17GeneratorErrorTest : public ::testing::Test
{
protected:
    Cpp17GeneratorErrorTest() :
            choiceArrayClashOutput(
                    "errors/cpp17_generator_error", "zserio_log_template_parameter_choice_array_clash.txt"),
            structArrayClashOutput(
                    "errors/cpp17_generator_error", "zserio_log_template_parameter_struct_array_clash.txt"),
            structClashOutput("errors/cpp17_generator_error", "zserio_log_template_parameter_struct_clash.txt"),
            structOtherClashOutput(
                    "errors/cpp17_generator_error", "zserio_log_template_parameter_struct_other_clash.txt"),
            unionArrayClashOutput(
                    "errors/cpp17_generator_error", "zserio_log_template_parameter_union_array_clash.txt")
    {}

    const test_utils::ZserioErrorOutput choiceArrayClashOutput;
    const test_utils::ZserioErrorOutput structArrayClashOutput;
    const test_utils::ZserioErrorOutput structClashOutput;
    const test_utils::ZserioErrorOutput structOtherClashOutput;
    const test_utils::ZserioErrorOutput unionArrayClashOutput;
};

TEST_F(Cpp17GeneratorErrorTest, templateParameterArrayClash)
{
    ASSERT_TRUE(
            choiceArrayClashOutput.isPresent("[ERROR] C++17 Generator: Template parameter clash detected!"));
    ASSERT_TRUE(
            structArrayClashOutput.isPresent("[ERROR] C++17 Generator: Template parameter clash detected!"));
    ASSERT_TRUE(unionArrayClashOutput.isPresent("[ERROR] C++17 Generator: Template parameter clash detected!"));
}

TEST_F(Cpp17GeneratorErrorTest, templateParameterClash)
{
    ASSERT_TRUE(structClashOutput.isPresent("[ERROR] C++17 Generator: Template parameter clash detected!"));
    ASSERT_TRUE(
            structOtherClashOutput.isPresent("[ERROR] C++17 Generator: Template parameter clash detected!"));
}
