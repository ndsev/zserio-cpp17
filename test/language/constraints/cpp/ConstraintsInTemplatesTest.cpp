#include <utility>

#include "constraints/constraints_in_templates/ConstraintsInTemplates.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace constraints
{
namespace constraints_in_templates
{

using AllocatorType = ConstraintsInTemplates::allocator_type;
using BytesType = zserio::BasicBytes<AllocatorType>;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
using BitBufferType = zserio::BasicBitBuffer<AllocatorType>;

class ConstraintsInTemplatesTypeTest : public ::testing::Test
{
protected:
    ConstraintsInTemplates makeData()
    {
        ConstraintsInTemplates data;
        data.structField.tField.child = 1;
        data.structField.sField = "parrot";
        data.structField.eField = TestEnum::ONE;
        data.choiceField.emplace<TemplatedChoice<TStruct, StringType, TestBitmask>::Tag::bField>(
                TestBitmask::Values::FOUR);
        data.unionField.emplace<TemplatedUnion<zserio::VarInt64, BitBufferType, TestBitmask>::Tag::tField>(25);
        return data;
    }
};

TEST_F(ConstraintsInTemplatesTypeTest, writeReadTest)
{
    auto data = makeData();
    test_utils::writeReadTest(data);
}

TEST_F(ConstraintsInTemplatesTypeTest, hashTest)
{
    auto data = makeData();
    EXPECT_EQ(std::hash<ConstraintsInTemplates>()(data), static_cast<size_t>(2791886731ULL));
}

} // namespace constraints_in_templates
} // namespace constraints
