#include "gtest/gtest.h"
#include "test_utils/ZserioErrorOutput.h"

class Cpp17GeneratorErrorTest : public ::testing::Test
{
protected:
    Cpp17GeneratorErrorTest() :
            choiceClashOutput("errors/cpp17_generator_error", "zserio_log_template_parameter_choice_clash.txt"),
            structClashOutput("errors/cpp17_generator_error", "zserio_log_template_parameter_struct_clash.txt"),
            unionClashOutput("errors/cpp17_generator_error", "zserio_log_template_parameter_union_clash.txt")
    {}

    const test_utils::ZserioErrorOutput choiceClashOutput;
    const test_utils::ZserioErrorOutput structClashOutput;
    const test_utils::ZserioErrorOutput unionClashOutput;
};

TEST_F(Cpp17GeneratorErrorTest, templateParameterClash)
{
    ASSERT_TRUE(choiceClashOutput.isPresent("[ERROR] C++17 Generator: Template parameter clash detected!"));
    ASSERT_TRUE(structClashOutput.isPresent("[ERROR] C++17 Generator: Template parameter clash detected!"));
    ASSERT_TRUE(unionClashOutput.isPresent("[ERROR] C++17 Generator: Template parameter clash detected!"));
}
