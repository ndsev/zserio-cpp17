#include <memory>

#include "gtest/gtest.h"
#include "parameterized_types/param_with_optional/Holder.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace param_with_optional
{

using AllocatorType = Holder::AllocatorType;

class ParamWithOptionalTest : public ::testing::Test
{
protected:
    static void fillData(Holder& data)
    {
        const Param param(HAS_EXTRA, EXTRA_PARAM);
        data.param = param;
        Value value;
        value.extraValue = ExtraValue(EXTRA_VALUE);
        data.value = value;
    }

    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeBool(HAS_EXTRA);
        writer.writeUnsignedBits32(EXTRA_PARAM, 7);
        writer.writeUnsignedBits64(EXTRA_VALUE, 64);
    }

    static constexpr std::string_view BLOB_NAME = "language/parameterized_types/param_with_optional.blob";

    static constexpr bool HAS_EXTRA = true;
    static constexpr zserio::UInt7 EXTRA_PARAM = 0x00;
    static constexpr zserio::UInt64 EXTRA_VALUE = 0xDEAD;
    static constexpr size_t HOLDER_BIT_SIZE = 1 + 7 + 64;
};

TEST_F(ParamWithOptionalTest, bitSizeOf)
{
    Holder data;
    fillData(data);

    zserio::View view(data);
    ASSERT_EQ(HOLDER_BIT_SIZE, zserio::detail::bitSizeOf(view, 0));
    ASSERT_EQ(HOLDER_BIT_SIZE, zserio::detail::bitSizeOf(view, 1));
    ASSERT_EQ(HOLDER_BIT_SIZE, zserio::detail::bitSizeOf(view, 100));
}

TEST_F(ParamWithOptionalTest, writeRead)
{
    Holder data;
    fillData(data);

    test_utils::writeReadTest(data);
}

TEST_F(ParamWithOptionalTest, read)
{
    Holder data;
    fillData(data);

    test_utils::readTest(writeData, data);
}

} // namespace param_with_optional
} // namespace parameterized_types
