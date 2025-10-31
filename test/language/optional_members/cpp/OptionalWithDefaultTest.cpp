#include "gtest/gtest.h"
#include "optional_members/optional_with_default/Field.h"
#include "optional_members/optional_with_default/TemplatedField.h"
#include "test_utils/TestUtility.h"

namespace optional_members
{
namespace optional_with_default
{

class OptionalWithDefaultTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, bool hasInfo, bool alien)
    {
        writer.writeBool(hasInfo);
        if (hasInfo)
        {
            writer.writeBool(alien);
        }
    }
};

TEST_F(OptionalWithDefaultTest, constructor)
{
    Field field1;
    ASSERT_FALSE(field1.hasInfo);
    ASSERT_TRUE(field1.alien.has_value() && field1.alien.value());

    Field field2{Field::allocator_type{}};
    ASSERT_FALSE(field2.hasInfo);
    ASSERT_TRUE(field2.alien.has_value() && field2.alien.value());
}

TEST_F(OptionalWithDefaultTest, writeRead)
{
    Field field;
    field.hasInfo = false;
    field.alien = {};
    test_utils::writeReadTest(field);
    field.hasInfo = true;
    field.alien = true;
    test_utils::writeReadTest(field);

    TemplatedField<zserio::Int8> tmp;
    tmp.hasInfo = false;
    tmp.alien = {};
    test_utils::writeReadTest(tmp);
    tmp.hasInfo = true;
    tmp.alien = true;
    test_utils::writeReadTest(tmp);
}

TEST_F(OptionalWithDefaultTest, read)
{
    Field field;
    field.hasInfo = false;
    field.alien = {};
    test_utils::readTest(
            [&](zserio::BitStreamWriter& writer) {
                writeData(writer, field.hasInfo, field.alien.value_or(false));
            },
            field);
    field.hasInfo = true;
    field.alien = false;
    test_utils::readTest(
            [&](zserio::BitStreamWriter& writer) {
                writeData(writer, field.hasInfo, field.alien.value_or(false));
            },
            field);

    TemplatedField<zserio::Int8> tmp;
    tmp.hasInfo = false;
    tmp.alien = {};
    test_utils::readTest(
            [&](zserio::BitStreamWriter& writer) {
                writeData(writer, tmp.hasInfo, tmp.alien.value_or(false));
            },
            tmp);
    tmp.hasInfo = true;
    tmp.alien = false;
    test_utils::readTest(
            [&](zserio::BitStreamWriter& writer) {
                writeData(writer, tmp.hasInfo, tmp.alien.value_or(false));
            },
            tmp);
}

TEST_F(OptionalWithDefaultTest, stdHash)
{
    Field field;
    field.hasInfo = false;
    ASSERT_EQ(31488, std::hash<Field>()(field));
    field.hasInfo = true;
    field.alien = true;
    ASSERT_EQ(31525, std::hash<Field>()(field));
}

} // namespace optional_with_default
} // namespace optional_members
