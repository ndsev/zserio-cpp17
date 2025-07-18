#include <utility>

#include "default_values/default_values_in_templates/DefaultValuesInTemplate.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace default_values
{
namespace default_values_in_templates
{

using AllocatorType = DefaultValuesInTemplate::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

class DefaultValuesInTemplatesTest : public ::testing::Test
{
protected:
    DefaultValuesInTemplate makeData()
    {
        DefaultValuesInTemplate dv;
        // #384: Choose round numbers so otherwise float comparison fails
        dv.f16Default = FloatingDefault<zserio::Float16>(3.125f);
        dv.f64Default = FloatingDefault<zserio::Float64>(3.125);
        return dv;
    }
};

TEST_F(DefaultValuesInTemplatesTest, writeReadTest)
{
    auto data = makeData();
    test_utils::writeReadTest(data);
}

TEST_F(DefaultValuesInTemplatesTest, hashTest)
{
    auto data = makeData();
    EXPECT_EQ(std::hash<DefaultValuesInTemplate>()(data), static_cast<size_t>(2986456084ULL));
}

} // namespace default_values_in_templates
} // namespace default_values
