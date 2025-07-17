#include <utility>

#include "array_types/templated_variable_array/TemplatedVariableArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace array_types
{
namespace templated_variable_array
{

using AllocatorType = TemplatedVariableArray::allocator_type;
using BytesType = zserio::BasicBytes<AllocatorType>;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

class TemplatedVariableArrayTest : public ::testing::Test
{
protected:
    TemplatedVariableArray makeData()
    {
        TemplatedVariableArray taa;
        taa.len = ARRAY_LENGTH;
        auto& arrays = taa.arrays;

        fillData(arrays.uint8Array, 12);
        fillData(arrays.varsizeArray, 1024);
        fillData(arrays.stringArray, "kuku");
        fillData(arrays.bytesArray, BytesType{0x20, 0x40, 0x80});
        fillData(arrays.enumArray, TestEnum::MINUS_ONE);
        fillData(arrays.bitmaskArray, TestBitmask::Values::READ | TestBitmask::Values::WRITE);
        fillData(arrays.structArray, TestStruct<StringType>("hello"));

        fillData(arrays.unionArray,
                TestUnion<TestBitmask>(zserio::in_place_index<TestUnion<TestBitmask>::Tag::bitmaskField>,
                        TestBitmask::Values::READ));

        fillData(arrays.choiceArray,
                TestChoice<TestEnum>(zserio::in_place_index<TestChoice<TestEnum>::Tag::uint32Field>, 25));

        return taa;
    }

    template <typename T, typename U>
    static void fillData(Templated<T>& tmp, const U& value)
    {
        tmp.array.reserve(ARRAY_LENGTH);
        std::fill_n(std::back_inserter(tmp.array), ARRAY_LENGTH, value);
    }
    template <typename T, typename P, typename U, typename S>
    static void fillData(TemplatedParam<T, P, S>& tmp, const U& value)
    {
        tmp.array.reserve(ARRAY_LENGTH);
        std::fill_n(std::back_inserter(tmp.array), ARRAY_LENGTH, value);
    }

    static constexpr size_t ARRAY_LENGTH = 5;
};

TEST_F(TemplatedVariableArrayTest, writeReadTest)
{
    auto data = makeData();
    test_utils::writeReadTest(data);
}

TEST_F(TemplatedVariableArrayTest, hashTest)
{
    auto data = makeData();
    EXPECT_EQ(std::hash<TemplatedVariableArray>()(data), static_cast<size_t>(912666778));
}

} // namespace templated_variable_array
} // namespace array_types
