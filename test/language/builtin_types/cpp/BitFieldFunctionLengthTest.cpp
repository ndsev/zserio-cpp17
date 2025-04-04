#include "builtin_types/bitfield_function_length/Container.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace builtin_types
{
namespace bitfield_function_length
{

using allocator_type = Container::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class BitFieldFunctionLengthTest : public ::testing::Test
{
protected:
    Container createContainer()
    {
        return Container(0xDEAD, // id
                VectorType<zserio::UInt64>(
                        {0xDEAD1, 0xDEAD2, 0xDEAD3, 0xDEAD4, 0xDEAD5, 0xDEAD6, 0xDEAD7}), // array[7]
                0x3F, // bitField3 (7 bits)
                0x1FFF, // bitField4 (0xDEAD & 0x0F = 0xD = 13 bits)
                0x1FFF // bitField5 (0xDEAD % 32 = 13 bits)
        );
    }

    static constexpr std::string_view BLOB_NAME = "language/builtin_types/bit_field_function_length.blob";
};

TEST_F(BitFieldFunctionLengthTest, bitSizeOf)
{
    const Container data = createContainer();
    zserio::View view(data);
    const size_t expectedBitSizeOfContainer = 64 + 7 * 64 + 7 + 13 + 13;
    ASSERT_EQ(expectedBitSizeOfContainer, zserio::detail::bitSizeOf(view));
}

TEST_F(BitFieldFunctionLengthTest, writeRead)
{
    Container data = createContainer();

    test_utils::writeReadTest(data);
}

TEST_F(BitFieldFunctionLengthTest, writeReadFile)
{
    Container data = createContainer();

    test_utils::writeReadFileTest(BLOB_NAME, data);
}

} // namespace bitfield_function_length
} // namespace builtin_types
