#include <string>

#include "bitmask_types/uint64_bitmask/Permission.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace bitmask_types
{
namespace uint64_bitmask
{

class Uint64BitmaskTest : public ::testing::Test
{
protected:
    static constexpr size_t PERMISSION_BITSIZEOF = 64;

    static constexpr Permission::ZserioType NONE_PERMISSION_VALUE = 0;
    static constexpr Permission::ZserioType READ_PERMISSION_VALUE = 2;
    static constexpr Permission::ZserioType WRITE_PERMISSION_VALUE = 4;

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

TEST_F(Uint64BitmaskTest, emptyConstructor)
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

TEST_F(Uint64BitmaskTest, valuesConstructor)
{
    const Permission permission(Permission::Values::write_permission);
    ASSERT_EQ(WRITE_PERMISSION_VALUE, permission.getValue());
}

TEST_F(Uint64BitmaskTest, underlyingTypeConstructor)
{
    const Permission permission(READ_PERMISSION_VALUE);
    ASSERT_TRUE((permission & Permission::Values::READ_PERMISSION) == Permission::Values::READ_PERMISSION);
}

TEST_F(Uint64BitmaskTest, copyConstructor)
{
    const Permission permission(READ_PERMISSION_VALUE);
    const Permission copy(permission);
    ASSERT_EQ(READ_PERMISSION_VALUE, copy.getValue());
}

TEST_F(Uint64BitmaskTest, assignmentOperator)
{
    const Permission permission(READ_PERMISSION_VALUE);
    Permission copy;
    copy = permission;
    ASSERT_EQ(READ_PERMISSION_VALUE, copy.getValue());
}

TEST_F(Uint64BitmaskTest, moveConstructor)
{
    Permission permission(READ_PERMISSION_VALUE);
    const Permission moved(std::move(permission));
    ASSERT_EQ(READ_PERMISSION_VALUE, moved.getValue());
}

TEST_F(Uint64BitmaskTest, moveAssignmentOperator)
{
    Permission permission(READ_PERMISSION_VALUE);
    Permission moved;
    moved = std::move(permission);
    ASSERT_EQ(READ_PERMISSION_VALUE, moved.getValue());
}

TEST_F(Uint64BitmaskTest, zserioTypeCast)
{
    const Permission permission(WRITE_PERMISSION_VALUE);
    ASSERT_EQ(WRITE_PERMISSION_VALUE, static_cast<Permission::ZserioType>(permission));
}

TEST_F(Uint64BitmaskTest, getValue)
{
    ASSERT_EQ(NONE_PERMISSION_VALUE, Permission(Permission::Values::nonePermission).getValue());
    ASSERT_EQ(READ_PERMISSION_VALUE, Permission(Permission::Values::READ_PERMISSION).getValue());
    ASSERT_EQ(WRITE_PERMISSION_VALUE, Permission(Permission::Values::write_permission).getValue());
}

TEST_F(Uint64BitmaskTest, bitSizeOf)
{
    ASSERT_EQ(PERMISSION_BITSIZEOF, zserio::detail::bitSizeOf(Permission(Permission::Values::nonePermission)));
}

TEST_F(Uint64BitmaskTest, write)
{
    const Permission permission(Permission::Values::READ_PERMISSION);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, permission);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    ASSERT_EQ(READ_PERMISSION_VALUE, reader.readUnsignedBits64(PERMISSION_BITSIZEOF));
}

TEST_F(Uint64BitmaskTest, read)
{
    zserio::BitStreamWriter writer(bitBuffer);
    writer.writeUnsignedBits64(
            static_cast<uint64_t>(Permission::Values::write_permission), PERMISSION_BITSIZEOF);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    Permission permission;
    zserio::detail::read(reader, permission);
    ASSERT_EQ(WRITE_PERMISSION_VALUE, permission.getValue());
}

TEST_F(Uint64BitmaskTest, toString)
{
    ASSERT_EQ(std::string("0[nonePermission]"),
            Permission(Permission::Values::nonePermission).toString().c_str());
    ASSERT_EQ(std::string("2[READ_PERMISSION]"),
            Permission(Permission::Values::READ_PERMISSION).toString().c_str());
    ASSERT_EQ(std::string("4[write_permission]"),
            Permission(Permission::Values::write_permission).toString().c_str());
    ASSERT_EQ(std::string("6[READ_PERMISSION | write_permission]"),
            (Permission::Values::READ_PERMISSION | Permission::Values::write_permission).toString().c_str());
    ASSERT_EQ(std::string("7[READ_PERMISSION | write_permission]"), Permission(7).toString().c_str());
    ASSERT_EQ(std::string("255[READ_PERMISSION | write_permission | CreatePermission]"),
            Permission(255).toString().c_str());
}

TEST_F(Uint64BitmaskTest, comparisionOperator)
{
    Permission writePermission = Permission::Values::write_permission;
    Permission equalPermission = Permission::Values::write_permission;
    Permission lessThanPermission = Permission::Values::READ_PERMISSION;
    test_utils::comparisonOperatorsTest(writePermission, equalPermission, lessThanPermission);
}

TEST_F(Uint64BitmaskTest, operatorBitwiseOr)
{
    const Permission read(Permission::Values::READ_PERMISSION);
    const Permission write(Permission::Values::write_permission);

    ASSERT_EQ(read | write, Permission::Values::READ_PERMISSION | Permission::Values::write_permission);
    ASSERT_EQ(read | Permission::Values::write_permission, Permission::Values::READ_PERMISSION | write);
    ASSERT_EQ(read, read | Permission::Values::nonePermission);
    ASSERT_EQ(write, Permission::Values::nonePermission | write);

    ASSERT_EQ(READ_PERMISSION_VALUE | WRITE_PERMISSION_VALUE, (read | write).getValue());
}

TEST_F(Uint64BitmaskTest, operatorBitwiseAnd)
{
    const Permission read(Permission::Values::READ_PERMISSION);
    const Permission write(Permission::Values::write_permission);
    const Permission readwrite(Permission::Values::READ_PERMISSION | Permission::Values::write_permission);

    ASSERT_EQ(Permission::Values::READ_PERMISSION, readwrite & read);
    ASSERT_EQ(Permission::Values::write_permission, readwrite & write);
    ASSERT_EQ(read, readwrite & Permission::Values::READ_PERMISSION);
    ASSERT_EQ(read, Permission::Values::READ_PERMISSION & readwrite);
    ASSERT_EQ(write, Permission::Values::write_permission & readwrite);
    ASSERT_EQ(readwrite & Permission::Values::nonePermission, Permission::Values::nonePermission);

    ASSERT_EQ(read, read & read & read & read);
}

TEST_F(Uint64BitmaskTest, operatorBitwiseXor)
{
    const Permission read(Permission::Values::READ_PERMISSION);
    const Permission write(Permission::Values::write_permission);

    ASSERT_EQ(read ^ write, Permission::Values::READ_PERMISSION ^ Permission::Values::write_permission);
    ASSERT_EQ(Permission::Values::READ_PERMISSION ^ write, read ^ Permission::Values::write_permission);
    ASSERT_EQ((read ^ write).getValue(), READ_PERMISSION_VALUE ^ WRITE_PERMISSION_VALUE);
    ASSERT_EQ(read, (read ^ write) & read);
    ASSERT_EQ(write, (read ^ write) & write);
    ASSERT_EQ(Permission::Values::nonePermission, read ^ read);
    ASSERT_EQ(Permission::Values::nonePermission, read ^ Permission::Values::READ_PERMISSION);
}

TEST_F(Uint64BitmaskTest, operatorBitwiseNot)
{
    const Permission read(Permission::Values::READ_PERMISSION);

    ASSERT_EQ(Permission::Values::write_permission, ~read & Permission::Values::write_permission);
    ASSERT_EQ(Permission::Values::nonePermission, ~read & Permission::Values::READ_PERMISSION);
    ASSERT_EQ(Permission::Values::write_permission,
            ~Permission::Values::nonePermission & Permission::Values::write_permission);
    ASSERT_EQ(Permission::Values::READ_PERMISSION,
            ~Permission::Values::nonePermission & Permission::Values::READ_PERMISSION);
    ASSERT_EQ(Permission::Values::READ_PERMISSION | Permission::Values::write_permission,
            ~Permission::Values::nonePermission &
                    (Permission::Values::READ_PERMISSION | Permission::Values::write_permission));
}

TEST_F(Uint64BitmaskTest, operatorBitwiseOrAssignment)
{
    Permission permission;
    permission |= Permission::Values::READ_PERMISSION;
    ASSERT_EQ(Permission::Values::READ_PERMISSION, permission);

    permission |= Permission::Values::nonePermission;
    ASSERT_EQ(Permission::Values::READ_PERMISSION, permission);

    const Permission write(Permission::Values::write_permission);
    permission |= write;
    ASSERT_EQ(Permission::Values::READ_PERMISSION | Permission::Values::write_permission, permission);
}

TEST_F(Uint64BitmaskTest, operatorBitwiseAndAssignment)
{
    Permission permission(Permission::Values::READ_PERMISSION | Permission::Values::write_permission);
    permission &= Permission::Values::READ_PERMISSION;
    ASSERT_EQ(Permission::Values::READ_PERMISSION, permission);

    permission |= Permission::Values::write_permission;
    permission &= Permission::Values::write_permission;
    ASSERT_EQ(Permission::Values::write_permission, permission);

    const Permission write(Permission::Values::write_permission);
    permission |= Permission::Values::READ_PERMISSION;
    permission &= write;
    ASSERT_EQ(Permission::Values::write_permission, permission);

    permission &= Permission::Values::nonePermission;
    ASSERT_EQ(Permission::Values::nonePermission, permission);
}

TEST_F(Uint64BitmaskTest, operatorBitwiseXorAssignment)
{
    Permission permission;
    permission ^= Permission::Values::READ_PERMISSION;
    ASSERT_EQ(Permission::Values::READ_PERMISSION, permission);

    permission ^= Permission::Values::nonePermission;
    ASSERT_EQ(Permission::Values::READ_PERMISSION, permission);

    permission ^= Permission::Values::write_permission;
    ASSERT_EQ(Permission::Values::READ_PERMISSION | Permission::Values::write_permission, permission);

    permission ^= Permission::Values::write_permission;
    ASSERT_EQ(Permission::Values::READ_PERMISSION, permission);

    permission ^= Permission::Values::READ_PERMISSION;
    ASSERT_EQ(Permission::Values::nonePermission, permission);
}

TEST_F(Uint64BitmaskTest, stdHash)
{
    const Permission readPermission(Permission::Values::READ_PERMISSION);
    const size_t readHash = 853; // use hardcoded values to check that the hash code is stable
    const Permission equalPermission(Permission::Values::READ_PERMISSION);
    const Permission writePermission(Permission::Values::write_permission);
    const size_t writeHash = 855;

    test_utils::hashTest(readPermission, readHash, equalPermission, writePermission, writeHash);
}

} // namespace uint64_bitmask
} // namespace bitmask_types
