#include "gtest/gtest.h"
#include "parameterized_types/extern_param/ExternParam.h"
#include "test_utils/WriteReadTest.h"

namespace parameterized_types
{
namespace extern_param
{

using AllocatorType = ExternParam::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;
using BitBufferType = zserio::BasicBitBuffer<AllocatorType>;

TEST(ExternParamTest, writeRead)
{
    ExternParam data(BitBufferType(VectorType<uint8_t>{{0xCA, 0xFE}}, 15), Parameterized(13));

    test_utils::writeReadTest(data);
}

} // namespace extern_param
} // namespace parameterized_types
