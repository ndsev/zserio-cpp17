#include "gtest/gtest.h"
#include "parameterized_types/array_element_param_with_optional_in_template/Holder.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace array_element_param_with_optional_in_template
{

// note that in the data-view approach we cannot get exception due to inconsistent parameters for this schema
TEST(ArrayElementParamWithOptionalInTemplateTest, writeRead)
{
    Holder holder;
    holder.holder.param = {true, zserio::UInt7(0)};
    holder.holder.values.push_back({std::nullopt, ExtraValue<zserio::UInt7>(zserio::UInt64(13))});

    test_utils::writeReadTest(holder);
}

TEST(ArrayElementParamWithOptionalInTemplateTest, hash)
{
    Holder holder;
    holder.holder.param = {true, zserio::UInt7(0)};
    holder.holder.values.push_back({std::nullopt, ExtraValue<zserio::UInt7>(zserio::UInt64(13))});

    ASSERT_EQ(std::hash<Holder>()(holder), static_cast<size_t>(1200441));
}

} // namespace array_element_param_with_optional_in_template
} // namespace parameterized_types
