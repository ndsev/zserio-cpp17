#include "gtest/gtest.h"
#include "parameterized_types/bytes_param/BytesParam.h"
#include "test_utils/WriteReadTest.h"

namespace parameterized_types
{
namespace bytes_param
{

using allocator_type = BytesParam::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

TEST(BytesParamTest, writeRead)
{
    BytesParam data(VectorType<uint8_t>{{0xCA, 0xFE}}, Parameterized(13));

    test_utils::writeReadTest(data);
}

} // namespace bytes_param
} // namespace parameterized_types
