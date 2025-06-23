#include "gtest/gtest.h"
#include "templates/function_templated_return_type/FunctionTemplatedReturnType.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

using namespace std::literals;

namespace templates
{
namespace function_templated_return_type
{

using AllocatorType = FunctionTemplatedReturnType::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(FunctionTemplatedReturnTypeTest, writeRead)
{
    FunctionTemplatedReturnType data;
    data.hasHolder = true;
    data.uint32Test = TestStructure<zserio::UInt32>{std::nullopt, Holder<zserio::UInt32>{42}};
    data.stringTest = TestStructure<StringType>{std::nullopt, Holder<StringType>{StringType{"string"}}};
    data.floatTest = TestStructure<zserio::Float32>{zserio::Float32{4.2F}, std::nullopt};

    zserio::View view(data);
    ASSERT_EQ(42, view.uint32Test().get());
    ASSERT_EQ("string"sv, view.stringTest().get());
    ASSERT_EQ(4.2F, view.floatTest().get());

    test_utils::writeReadTest(data);
}

} // namespace function_templated_return_type
} // namespace templates
