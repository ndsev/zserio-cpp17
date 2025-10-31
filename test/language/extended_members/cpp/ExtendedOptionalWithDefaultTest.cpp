#include "extended_members/extended_optional_with_default/Extended.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace extended_members
{
namespace extended_optional_with_default
{

class ExtendedWithDefaultTest : public ::testing::Test
{
protected:
    static void writeData(
            zserio::BitStreamWriter& writer, zserio::UInt32 value, bool hasExtValue, bool extValue)
    {
        // value
        writer.writeUnsignedBits32(value, 32);
        // extendedValue
        writer.alignTo(8);
        writer.writeBool(hasExtValue);
        if (hasExtValue)
        {
            writer.writeBool(extValue);
        }
    }
};

TEST_F(ExtendedWithDefaultTest, constructor)
{
    Extended ext;
    ASSERT_TRUE(ext.extendedValue.isPresent());
    ASSERT_TRUE(ext.extendedValue->has_value() && ext.extendedValue->value());
}

TEST_F(ExtendedWithDefaultTest, writeRead)
{
    Extended ext;
    ext.extendedValue.value() = {};
    test_utils::writeReadTest(ext);
    ext.extendedValue.value() = true;
    test_utils::writeReadTest(ext);
}

TEST_F(ExtendedWithDefaultTest, read)
{
    Extended ext;
    ext.extendedValue.value() = {};
    test_utils::readTest(
            [&](zserio::BitStreamWriter& writer) {
                writeData(
                        writer, ext.value, ext.extendedValue->has_value(), ext.extendedValue->value_or(false));
            },
            ext);
    ext.extendedValue.value() = false;
    test_utils::readTest(
            [&](zserio::BitStreamWriter& writer) {
                writeData(
                        writer, ext.value, ext.extendedValue->has_value(), ext.extendedValue->value_or(false));
            },
            ext);
}

TEST_F(ExtendedWithDefaultTest, stdHash)
{
    Extended ext;
    ext.extendedValue.value() = {};
    ASSERT_EQ(851, std::hash<Extended>()(ext));
    ext.extendedValue.value() = true;
    ASSERT_EQ(31488, std::hash<Extended>()(ext));
}

} // namespace extended_optional_with_default
} // namespace extended_members
