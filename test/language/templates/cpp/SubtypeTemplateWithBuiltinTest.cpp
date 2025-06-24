#include "gtest/gtest.h"
#include "templates/subtype_template_with_builtin/SubtypeTemplateWithBuiltin.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace subtype_template_with_builtin
{

TEST(SubtypeTemplateWithBuiltinTest, writeRead)
{
    SubtypeTemplateWithBuiltin data;
    data.test = TestStructure<zserio::UInt32>{13};

    test_utils::writeReadTest(data);
}

} // namespace subtype_template_with_builtin
} // namespace templates
