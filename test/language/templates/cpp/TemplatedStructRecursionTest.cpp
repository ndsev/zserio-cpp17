#include "gtest/gtest.h"
#include "templates/templated_struct_recursion/TemplatedStructRecursion.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace templated_struct_recursion
{

using AllocatorType = TemplatedStructRecursion::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

TEST(TemplatedStructRecursionTest, writeRead)
{
    TemplatedStructRecursion data;
    data.recursiveTemplate.data = VectorType<zserio::UInt32>{{1, 2, 3}};
    data.recursiveTemplate.recursiveTemplate = RecursiveTemplate<zserio::UInt32>{
            VectorType<zserio::UInt32>{2, 3, 4},
            // lengthof(data) == 0 -> end of recursion
            RecursiveTemplate<zserio::UInt32>{VectorType<zserio::UInt32>{}, std::nullopt}};

    test_utils::writeReadTest(data);
}

} // namespace templated_struct_recursion
} // namespace templates
