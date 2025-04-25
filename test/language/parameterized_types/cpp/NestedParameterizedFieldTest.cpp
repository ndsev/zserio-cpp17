#include <memory>

#include "gtest/gtest.h"
#include "parameterized_types/nested_parameterized_field/TopLevel.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace nested_parameterized_field
{

using AllocatorType = TopLevel::allocator_type;

class NestedParameterizedFieldTest : public ::testing::Test
{
protected:
    static void fillData(TopLevel& data)
    {
        const Param param(VALUE, EXTRA_VALUE);
        ParamHolder paramHolder;
        // paramHolder.parameter has default
        paramHolder.param = param;
        data.paramHolder = paramHolder;
    }

    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(PARAMETER, 16);
        writer.writeUnsignedBits32(VALUE, 16);
        writer.writeUnsignedBits32(EXTRA_VALUE, 32);
    }

    static constexpr std::string_view BLOB_NAME =
            "language/parameterized_types/nested_parameterized_field.blob";

    static constexpr zserio::UInt16 PARAMETER = 11;
    static constexpr zserio::UInt16 VALUE = 0xAB;
    static constexpr zserio::UInt32 EXTRA_VALUE = 0xDEAD;
    static constexpr zserio::BitSize HOLDER_BIT_SIZE = 16 + 16 + 32;
};

TEST_F(NestedParameterizedFieldTest, bitSizeOf)
{
    TopLevel data;
    fillData(data);
    zserio::View view(data);

    const size_t bitPosition = 2;
    ASSERT_EQ(HOLDER_BIT_SIZE, zserio::detail::bitSizeOf(view, bitPosition));
}

TEST_F(NestedParameterizedFieldTest, writeRead)
{
    TopLevel data;
    fillData(data);

    test_utils::writeReadTest(data);
}

TEST_F(NestedParameterizedFieldTest, writeReadFile)
{
    TopLevel data;
    fillData(data);

    test_utils::writeReadFileTest(BLOB_NAME, data);
}

TEST_F(NestedParameterizedFieldTest, read)
{
    TopLevel data;
    fillData(data);

    test_utils::readTest(writeData, data);
}

} // namespace nested_parameterized_field
} // namespace parameterized_types
