#include <utility>

#include "array_types/templated_auto_array/TemplatedAutoArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace array_types
{
namespace templated_auto_array
{

using AllocatorType = TemplatedAutoArray::allocator_type;
using BytesType = zserio::BasicBytes<AllocatorType>;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

class TemplatedAutoArrayTest : public ::testing::Test
{
protected:
    TemplatedAutoArray makeData()
    {
        TemplatedAutoArray taa;
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
    template <typename T, typename P, typename U>
    static void fillData(TemplatedParam<T, P>& tmp, const U& value)
    {
        tmp.array.reserve(ARRAY_LENGTH);
        std::fill_n(std::back_inserter(tmp.array), ARRAY_LENGTH, value);
    }

    static constexpr size_t ARRAY_LENGTH = 5;
};

TEST_F(TemplatedAutoArrayTest, writeReadTest)
{
    auto data = makeData();
    test_utils::writeReadTest(data);
}

TEST_F(TemplatedAutoArrayTest, hashTest)
{
    auto data = makeData();
    EXPECT_EQ(std::hash<TemplatedAutoArray>()(data), static_cast<size_t>(912635957));
}

} // namespace templated_auto_array
} // namespace array_types
