#include "gtest/gtest.h"
#include "indexed_offsets/optional_nested_indexed_offset_array_in_template/OptionalNestedIndexedOffsetArrayInTemplate.h"
#include "test_utils/TestUtility.h"

namespace indexed_offsets
{
namespace optional_nested_indexed_offset_array_in_template
{

class OptionalNestedIndexedOffsetArrayInTemplateTest : public ::testing::Test
{
protected:
    OptionalNestedIndexedOffsetArrayInTemplate makeData()
    {
        OptionalNestedIndexedOffsetArrayInTemplate data;
        data.field.header.length = LEN;
        data.field.header.offsets.resize(LEN);
        data.field.data = {"first", "second"};
        assert(data.field.data->at(0).size() + 1 == OFFSET2 - OFFSET1);
        data.field.field = 61;
        return data;
    }

    static constexpr int LEN = 2;
    static constexpr int OFFSET1 = 2 + LEN * 4; // Header(length + offsets)
    static constexpr int OFFSET2 = OFFSET1 + 6; // += data[0].size() + 1
};

TEST_F(OptionalNestedIndexedOffsetArrayInTemplateTest, writeReadOffset)
{
    auto data = makeData();
    test_utils::writeReadTest(data);
    ASSERT_EQ(data.field.header.offsets[0], OFFSET1);
    ASSERT_EQ(data.field.header.offsets[1], OFFSET2);
}

TEST_F(OptionalNestedIndexedOffsetArrayInTemplateTest, hash)
{
    auto data = makeData();
    ASSERT_EQ(
            std::hash<OptionalNestedIndexedOffsetArrayInTemplate>()(data), static_cast<size_t>(1806430300ULL));
}

} // namespace optional_nested_indexed_offset_array_in_template
} // namespace indexed_offsets
