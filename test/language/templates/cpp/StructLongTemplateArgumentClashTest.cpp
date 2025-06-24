#include "gtest/gtest.h"
#include "templates/struct_long_template_argument_clash/StructLongTemplateArgumentClash.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace struct_long_template_argument_clash
{

TEST(StructLongTemplateArgumentClashTest, writeRead)
{
    const TemplatedStruct<ThisIsVeryVeryVeryLongNamedStructure, ThisIsVeryVeryVeryLongNamedStructure,
            ThisIsVeryVeryVeryLongNamedStructure>
            struct1(ThisIsVeryVeryVeryLongNamedStructure("StringT"),
                    ThisIsVeryVeryVeryLongNamedStructure("StringU"),
                    ThisIsVeryVeryVeryLongNamedStructure("StringV"));
    const TemplatedStruct<ThisIsVeryVeryVeryLongNamedStructure, ThisIsVeryVeryVeryLongNamedStructure,
            ThisIsVeryVeryVeryLongNamedStructure_>
            struct2(ThisIsVeryVeryVeryLongNamedStructure("StringT"),
                    ThisIsVeryVeryVeryLongNamedStructure("StringU"), ThisIsVeryVeryVeryLongNamedStructure_(42));
    StructLongTemplateArgumentClash data(struct1, struct2);

    test_utils::writeReadTest(data);
}

} // namespace struct_long_template_argument_clash
} // namespace templates
