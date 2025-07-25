#include "functions/templated_choice_array/Inner.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace functions
{
namespace templated_choice_array
{

class TemplatedChoiceArrayTest : public ::testing::Test
{
protected:
    Inner makeData()
    {
        Inner data;
        data.outerArray.numElements = 1;
        data.outerArray.values = {{zserio::UInt8(10), zserio::UInt8(11)}};
        data.isExplicit = 0;
        data.ref.emplace<ItemRef<Item, OuterArray<Item>>::Tag::pos>(0);
        return data;
    }
};

TEST_F(TemplatedChoiceArrayTest, writeRead)
{
    auto data = makeData();
    test_utils::writeReadTest(data);
}

TEST_F(TemplatedChoiceArrayTest, hash)
{
    auto data = makeData();
    ASSERT_EQ(std::hash<Inner>()(data), static_cast<size_t>(87980228));
}

} // namespace templated_choice_array
} // namespace functions
