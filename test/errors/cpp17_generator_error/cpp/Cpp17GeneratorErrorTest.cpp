#include "gtest/gtest.h"
#include "test_utils/ZserioErrorOutput.h"

class Cpp17GeneratorErrorTest : public ::testing::Test
{
protected:
    Cpp17GeneratorErrorTest() :
            choiceTmplParamFieldClashOutput("errors/cpp17_generator_error",
                    "zserio_log_type_alias_choice_tmpl_param_field_clash_error.txt"),
            structFieldClashOutput(
                    "errors/cpp17_generator_error", "zserio_log_type_alias_struct_field_clash_error.txt"),
            structNameFieldClashOutput(
                    "errors/cpp17_generator_error", "zserio_log_type_alias_struct_name_field_clash_error.txt"),
            structNameTmplParamClashOutput("errors/cpp17_generator_error",
                    "zserio_log_type_alias_struct_name_tmpl_param_clash_error.txt"),
            structTmplParamClashOutput(
                    "errors/cpp17_generator_error", "zserio_log_type_alias_struct_tmpl_param_clash_error.txt"),
            structTmplParamFieldClashOutput("errors/cpp17_generator_error",
                    "zserio_log_type_alias_struct_tmpl_param_field_clash_error.txt"),
            tableTmplParamClashOutput(
                    "errors/cpp17_generator_error", "zserio_log_type_alias_table_tmpl_param_clash_error.txt"),
            unionTmplParamFieldClashOutput("errors/cpp17_generator_error",
                    "zserio_log_type_alias_union_tmpl_param_field_clash_error.txt"),
    {}

    const test_utils::ZserioErrorOutput choiceTmplParamFieldClashOutput;
    const test_utils::ZserioErrorOutput structFieldClashOutput;
    const test_utils::ZserioErrorOutput structNameFieldClashOutput;
    const test_utils::ZserioErrorOutput structNameTmplParamClashOutput;
    const test_utils::ZserioErrorOutput structTmplParamClashOutput;
    const test_utils::ZserioErrorOutput structTmplParamFieldClashOutput;
    const test_utils::ZserioErrorOutput tableTmplParamClashOutput;
    const test_utils::ZserioErrorOutput unionTmplParamFieldClashOutput;
};

TEST_F(Cpp17GeneratorErrorTest, typeAliasClash)
{
    ASSERT_TRUE(choiceTmplParamFieldClashOutput.isPresent(
            "choice_tmpl_param_field_clash_error.zs:3:15: Template parameter 'Uint8ArrayType' clashes with "
            "type "
            "alias for field 'uint8Array'."));
    ASSERT_TRUE(structFieldClashOutput.isPresent(
            "struct_field_clash_error.zs:5:11: Type alias for field 'myArray' "
            "clashes with type alias for another field 'MY_ARRAY'."));
    ASSERT_TRUE(structNameFieldClashOutput.isPresent(
            "struct_name_field_clash_error.zs:5:11: Type alias for a field 'param1' clashes with the compound "
            "name 'Param1Type'."));
    ASSERT_TRUE(structNameTmplParamClashOutput.isPresent(
            "struct_name_tmpl_param_clash_error.zs:3:19: Type alias for a template parameter 'PARAM2' clashes "
            "with the compound name 'Param2Type'."));
    ASSERT_TRUE(structTmplParamClashOutput.isPresent(
            "struct_tmpl_param_clash_error.zs:3:15: Type alias for a template parameter 'PARAM' clashes with "
            "type alias for another template parameter 'Param'."));
    ASSERT_TRUE(structTmplParamFieldClashOutput.isPresent(
            "struct_tmpl_param_field_clash_error.zs:3:15: Template parameter 'Uint8ArrayType' clashes with "
            "type "
            "alias for field 'uint8Array'."));
    ASSERT_TRUE(tableTmplParamClashOutput.isPresent(
            "table_tmpl_param_clash_error.zs:3:22: Type alias for a template parameter 'PARAM' clashes with "
            "type alias for another template parameter 'Param'."));
    ASSERT_TRUE(unionTmplParamFieldClashOutput.isPresent(
            "union_tmpl_param_field_clash_error.zs:3:14: Template parameter 'Uint8ArrayType' clashes with type "
            "alias for field 'uint8Array'."));
}
