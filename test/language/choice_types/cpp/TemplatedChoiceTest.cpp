#include <utility>

#include "choice_types/templated_choice/TemplatedChoice.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace choice_types
{
namespace templated_choice
{

using AllocatorType = TemplatedChoice::allocator_type;
using BytesType = zserio::BasicBytes<AllocatorType>;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

class TemplatedChoiceTypeTest : public ::testing::Test
{
protected:
    TemplatedChoice makeData()
    {
        TemplatedChoice data;
        data.bytesSelector.field = 0;
        data.bytesChoice.emplace<TestChoice<BytesType, TestUnion, Selector>::Tag::field>(BytesType{0x10, 0x20});
        data.structSelector.field = 1;
        data.structChoice.emplace<TestChoice<TestStruct, TestUnion, Selector>::Tag::paramField>(
                TestUnion(zserio::in_place_index<TestUnion::Tag::strField>, "salvation"));
        return data;
    }
};

TEST_F(TemplatedChoiceTypeTest, writeReadTest)
{
    auto data = makeData();
    test_utils::writeReadTest(data);
}

TEST_F(TemplatedChoiceTypeTest, hashTest)
{
    auto data = makeData();
    EXPECT_EQ(std::hash<TemplatedChoice>()(data), static_cast<size_t>(2923572428ULL));
}

} // namespace templated_choice
} // namespace choice_types
