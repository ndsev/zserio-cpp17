#include "gtest/gtest.h"
#include "templates/struct_long_template_argument/StructLongTemplateArgument.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace struct_long_template_argument
{

TEST(StructLongTemplateArgumentTest, writeRead)
{
    const TemplatedStruct<ThisIsVeryVeryVeryLongNamedStructure, ThisIsVeryVeryVeryLongNamedStructure,
            ThisIsVeryVeryVeryLongNamedStructure>
            templ(ThisIsVeryVeryVeryLongNamedStructure("StringT"),
                    ThisIsVeryVeryVeryLongNamedStructure("StringU"),
                    ThisIsVeryVeryVeryLongNamedStructure("StringV"));
    StructLongTemplateArgument data(templ);

    test_utils::writeReadTest(data);
}

} // namespace struct_long_template_argument
} // namespace templates
