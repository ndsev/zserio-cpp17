#include "gtest/gtest.h"
#include "templates/instantiate_nested_template/InstantiateNestedTemplate.h"
#include "templates/instantiate_nested_template/NStr.h"
#include "templates/instantiate_nested_template/TStr.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace instantiate_nested_template
{

using AllocatorType = InstantiateNestedTemplate::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(InstantiateNestedTemplateTest, writeRead)
{
    InstantiateNestedTemplate data;
    data.test = TStr{NStr{"test"}};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_nested_template
} // namespace templates
