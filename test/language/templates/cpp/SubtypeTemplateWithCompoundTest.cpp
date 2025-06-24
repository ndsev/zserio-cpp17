#include "gtest/gtest.h"
#include "templates/subtype_template_with_compound/SubtypeTemplateWithCompound.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace subtype_template_with_compound
{

TEST(SubtypeTemplateWithCompoundTest, writeRead)
{
    SubtypeTemplateWithCompound data;
    data.value1 = Compound{13};
    data.value2 = TemplateCompound<Compound>{Compound{42}};

    test_utils::writeReadTest(data);
}

} // namespace subtype_template_with_compound
} // namespace templates
