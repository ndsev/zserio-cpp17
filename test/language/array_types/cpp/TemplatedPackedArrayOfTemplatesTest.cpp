#include "array_types/templated_packed_array_of_templates/Holder.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace array_types
{
namespace templated_packed_array_of_templates
{

class TemplatedPackedArrayOfTemplatesTest : public ::testing::Test
{
protected:
    Holder makeData()
    {
        Holder data;
        data.list.enumeration = TestEnum::NAME;
        data.list.length = 1;
        data.list.array.emplace_back(TestChoice(zserio::in_place_index<TestChoice::Tag::text>, "dogfood"));
        return data;
    }
};

TEST_F(TemplatedPackedArrayOfTemplatesTest, writeReadTest)
{
    auto data = makeData();
    test_utils::writeReadTest(data);
}

TEST_F(TemplatedPackedArrayOfTemplatesTest, hashTest)
{
    auto data = makeData();
    ASSERT_EQ(std::hash<Holder>()(data), static_cast<size_t>(1612707247ULL));
}

} // namespace templated_packed_array_of_templates
} // namespace array_types
