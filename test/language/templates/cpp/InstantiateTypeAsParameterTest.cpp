#include "gtest/gtest.h"
#include "templates/instantiate_type_as_parameter/InstantiateTypeAsParameter.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace instantiate_type_as_parameter
{

using AllocatorType = InstantiateTypeAsParameter::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

TEST(InstantiateTypeAsParameterTest, writeRead)
{
    InstantiateTypeAsParameter data{P32{2}, Parameterized<P32>{VectorType<zserio::UInt32>{13, 42}}};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_type_as_parameter
} // namespace templates
