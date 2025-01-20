#include <string>

#include "bitmask_types/uint8_bitmask/Permission.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace bitmask_types
{
namespace uint8_bitmask
{

class Uint8BitmaskTest : public ::testing::Test
{
protected:
    static constexpr size_t PERMISSION_BITSIZEOF = 8;

    static constexpr Permission::ZserioType NONE_VALUE = 0;
    static constexpr Permission::ZserioType READ_VALUE = 2;
    static constexpr Permission::ZserioType WRITE_VALUE = 4;
};

TEST_F(Uint8BitmaskTest, emptyConstructor)
{
    {
        const Permission permission;
        ASSERT_EQ(0, permission.getValue());
    }
    {
        const Permission permission = {};
        ASSERT_EQ(0, permission.getValue());
    }
}

TEST_F(Uint8BitmaskTest, valuesConstructor)
{
    const Permission permission(Permission::Values::WRITE);
    ASSERT_EQ(WRITE_VALUE, permission.getValue());
}

TEST_F(Uint8BitmaskTest, zserioTypeConstructor)
{
    const Permission permission(READ_VALUE);
    ASSERT_TRUE((permission & Permission::Values::READ) == Permission::Values::READ);
}

TEST_F(Uint8BitmaskTest, copyConstructor)
{
    const Permission permission(READ_VALUE);
    const Permission copy(permission);
    ASSERT_EQ(READ_VALUE, copy.getValue());
}

TEST_F(Uint8BitmaskTest, assignmentOperator)
{
    const Permission permission(READ_VALUE);
    Permission copy;
    copy = permission;
    ASSERT_EQ(READ_VALUE, copy.getValue());
}

TEST_F(Uint8BitmaskTest, moveConstructor)
{
    Permission permission(READ_VALUE);
    const Permission moved(std::move(permission));
    ASSERT_EQ(READ_VALUE, moved.getValue());
}

TEST_F(Uint8BitmaskTest, moveAssignmentOperator)
{
    Permission permission(READ_VALUE);
    Permission moved;
    moved = std::move(permission);
    ASSERT_EQ(READ_VALUE, moved.getValue());
}

TEST_F(Uint8BitmaskTest, underlyingTypeCast)
{
    const Permission permission(WRITE_VALUE);
    ASSERT_EQ(WRITE_VALUE, static_cast<Permission::ZserioType>(permission));
}

TEST_F(Uint8BitmaskTest, getValue)
{
    ASSERT_EQ(NONE_VALUE, Permission(Permission::Values::NONE).getValue());
    ASSERT_EQ(READ_VALUE, Permission(Permission::Values::READ).getValue());
    ASSERT_EQ(WRITE_VALUE, Permission(Permission::Values::WRITE).getValue());
}

TEST_F(Uint8BitmaskTest, bitSizeOf)
{
    ASSERT_EQ(PERMISSION_BITSIZEOF, zserio::detail::bitSizeOf(Permission(Permission::Values::NONE)));
}

TEST_F(Uint8BitmaskTest, write)
{
    const Permission permission(Permission::Values::READ);
    zserio::BitBuffer bitBuffer(PERMISSION_BITSIZEOF);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, permission);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    ASSERT_EQ(READ_VALUE, reader.readUnsignedBits32(PERMISSION_BITSIZEOF));
}

TEST_F(Uint8BitmaskTest, read)
{
    zserio::BitBuffer bitBuffer(PERMISSION_BITSIZEOF);
    zserio::BitStreamWriter writer(bitBuffer);
    writer.writeUnsignedBits32(static_cast<uint32_t>(Permission::Values::WRITE), PERMISSION_BITSIZEOF);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    Permission permission;
    zserio::detail::read(reader, permission);
    ASSERT_EQ(WRITE_VALUE, permission.getValue());
}

TEST_F(Uint8BitmaskTest, toString)
{
    ASSERT_EQ(std::string("0[NONE]"), Permission(Permission::Values::NONE).toString().c_str());
    ASSERT_EQ(std::string("2[READ]"), Permission(Permission::Values::READ).toString().c_str());
    ASSERT_EQ(std::string("4[WRITE]"), Permission(Permission::Values::WRITE).toString().c_str());
    ASSERT_EQ(std::string("6[READ | WRITE]"),
            (Permission::Values::READ | Permission::Values::WRITE).toString().c_str());
    ASSERT_EQ(std::string("7[READ | WRITE | CREATE]"), Permission(7).toString().c_str());
    ASSERT_EQ(std::string("255[READ | WRITE | CREATE]"), Permission(255).toString().c_str());
}

TEST_F(Uint8BitmaskTest, comparisonOperators)
{
    Permission writePermission = Permission::Values::WRITE;
    Permission equalPermission = Permission::Values::WRITE;
    Permission lessThanPermission = Permission::Values::READ;
    test_utils::comparisonOperatorsTest(writePermission, equalPermission, lessThanPermission);
}

TEST_F(Uint8BitmaskTest, operatorBitwiseOr)
{
    const Permission read(Permission::Values::READ);
    const Permission write(Permission::Values::WRITE);

    ASSERT_EQ(read | write, Permission::Values::READ | Permission::Values::WRITE);
    ASSERT_EQ(read | Permission::Values::WRITE, Permission::Values::READ | write);
    ASSERT_EQ(read, read | Permission::Values::NONE);
    ASSERT_EQ(write, Permission::Values::NONE | write);

    ASSERT_EQ(READ_VALUE | WRITE_VALUE, (read | write).getValue());
}

TEST_F(Uint8BitmaskTest, operatorBitwiseAnd)
{
    const Permission read(Permission::Values::READ);
    const Permission write(Permission::Values::WRITE);
    const Permission readwrite(Permission::Values::READ | Permission::Values::WRITE);

    ASSERT_EQ(Permission::Values::READ, readwrite & read);
    ASSERT_EQ(Permission::Values::WRITE, readwrite & write);
    ASSERT_EQ(read, readwrite & Permission::Values::READ);
    ASSERT_EQ(read, Permission::Values::READ & readwrite);
    ASSERT_EQ(write, Permission::Values::WRITE & readwrite);
    ASSERT_EQ(readwrite & Permission::Values::NONE, Permission::Values::NONE);

    ASSERT_EQ(read, read & read & read & read);
}

TEST_F(Uint8BitmaskTest, operatorBitwiseXor)
{
    const Permission read(Permission::Values::READ);
    const Permission write(Permission::Values::WRITE);

    ASSERT_EQ(read ^ write, Permission::Values::READ ^ Permission::Values::WRITE);
    ASSERT_EQ(Permission::Values::READ ^ write, read ^ Permission::Values::WRITE);
    ASSERT_EQ((read ^ write).getValue(), READ_VALUE ^ WRITE_VALUE);
    ASSERT_EQ(read, (read ^ write) & read);
    ASSERT_EQ(write, (read ^ write) & write);
    ASSERT_EQ(Permission::Values::NONE, read ^ read);
    ASSERT_EQ(Permission::Values::NONE, read ^ Permission::Values::READ);
}

TEST_F(Uint8BitmaskTest, operatorBitwiseNot)
{
    const Permission read(Permission::Values::READ);

    ASSERT_EQ(Permission::Values::WRITE, ~read & Permission::Values::WRITE);
    ASSERT_EQ(Permission::Values::NONE, ~read & Permission::Values::READ);
    ASSERT_EQ(Permission::Values::WRITE, ~Permission::Values::NONE & Permission::Values::WRITE);
    ASSERT_EQ(Permission::Values::READ, ~Permission::Values::NONE & Permission::Values::READ);
    ASSERT_EQ(Permission::Values::READ | Permission::Values::WRITE,
            ~Permission::Values::NONE & (Permission::Values::READ | Permission::Values::WRITE));
}

TEST_F(Uint8BitmaskTest, operatorBitwiseOrAssignment)
{
    Permission permission;
    permission |= Permission::Values::READ;
    ASSERT_EQ(Permission::Values::READ, permission);

    permission |= Permission::Values::NONE;
    ASSERT_EQ(Permission::Values::READ, permission);

    const Permission write(Permission::Values::WRITE);
    permission |= write;
    ASSERT_EQ(Permission::Values::READ | Permission::Values::WRITE, permission);
}

TEST_F(Uint8BitmaskTest, operatorBitwiseAndAssignment)
{
    Permission permission(Permission::Values::READ | Permission::Values::WRITE);
    permission &= Permission::Values::READ;
    ASSERT_EQ(Permission::Values::READ, permission);

    permission |= Permission::Values::WRITE;
    permission &= Permission::Values::WRITE;
    ASSERT_EQ(Permission::Values::WRITE, permission);

    const Permission write(Permission::Values::WRITE);
    permission |= Permission::Values::READ;
    permission &= write;
    ASSERT_EQ(Permission::Values::WRITE, permission);

    permission &= Permission::Values::NONE;
    ASSERT_EQ(Permission::Values::NONE, permission);
}

TEST_F(Uint8BitmaskTest, operatorBitwiseXorAssignment)
{
    Permission permission;
    permission ^= Permission::Values::READ;
    ASSERT_EQ(Permission::Values::READ, permission);

    permission ^= Permission::Values::NONE;
    ASSERT_EQ(Permission::Values::READ, permission);

    permission ^= Permission::Values::WRITE;
    ASSERT_EQ(Permission::Values::READ | Permission::Values::WRITE, permission);

    permission ^= Permission::Values::WRITE;
    ASSERT_EQ(Permission::Values::READ, permission);

    permission ^= Permission::Values::READ;
    ASSERT_EQ(Permission::Values::NONE, permission);
}

TEST_F(Uint8BitmaskTest, stdHash)
{
    const Permission readPermission(Permission::Values::READ);
    const size_t readHash = 853; // use hardcoded values to check that the hash code is stable
    const Permission equalPermission(Permission::Values::READ);
    const Permission writePermission(Permission::Values::WRITE);
    const size_t writeHash = 855;

    test_utils::hashTest(readPermission, readHash, equalPermission, writePermission, writeHash);
}

} // namespace uint8_bitmask
} // namespace bitmask_types
