#include <limits>
#include <string>

#include "bitmask_types/varuint_bitmask/Permission.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace bitmask_types
{
namespace varuint_bitmask
{

class VarUIntBitmaskTest : public ::testing::Test
{
protected:
    static constexpr std::string_view BLOB_NAME = "language/bitmask_types/varuint_bitmask.blob";

    static constexpr Permission::ZserioType NONE_VALUE = 0;
    static constexpr Permission::ZserioType READ_VALUE = 2;
    static constexpr Permission::ZserioType WRITE_VALUE = 4;

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(8);
};

TEST_F(VarUIntBitmaskTest, emptyConstructor)
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

TEST_F(VarUIntBitmaskTest, valuesConstructor)
{
    const Permission permission(Permission::Values::WRITE);
    ASSERT_EQ(WRITE_VALUE, permission.getValue());
}

TEST_F(VarUIntBitmaskTest, zserioTypeConstructor)
{
    const Permission permission(READ_VALUE);
    ASSERT_TRUE((permission & Permission::Values::READ) == Permission::Values::READ);
}

TEST_F(VarUIntBitmaskTest, copyConstructor)
{
    const Permission permission(READ_VALUE);
    const Permission copy(permission);
    ASSERT_EQ(READ_VALUE, copy.getValue());
}

TEST_F(VarUIntBitmaskTest, assignmentOperator)
{
    const Permission permission(READ_VALUE);
    Permission copy;
    copy = permission;
    ASSERT_EQ(READ_VALUE, copy.getValue());
}

TEST_F(VarUIntBitmaskTest, moveConstructor)
{
    Permission permission(READ_VALUE);
    const Permission moved(std::move(permission));
    ASSERT_EQ(READ_VALUE, moved.getValue());
}

TEST_F(VarUIntBitmaskTest, moveAssignmentOperator)
{
    Permission permission(READ_VALUE);
    Permission moved;
    moved = std::move(permission);
    ASSERT_EQ(READ_VALUE, moved.getValue());
}

TEST_F(VarUIntBitmaskTest, zserioTypeCast)
{
    const Permission permission(WRITE_VALUE);
    ASSERT_EQ(WRITE_VALUE, static_cast<Permission::ZserioType>(permission));
}

TEST_F(VarUIntBitmaskTest, getValue)
{
    ASSERT_EQ(NONE_VALUE, Permission(Permission::Values::NONE).getValue());
    ASSERT_EQ(READ_VALUE, Permission(Permission::Values::READ).getValue());
    ASSERT_EQ(WRITE_VALUE, Permission(Permission::Values::WRITE).getValue());
}

TEST_F(VarUIntBitmaskTest, bitSizeOf)
{
    ASSERT_EQ(zserio::detail::bitSizeOf(NONE_VALUE),
            zserio::detail::bitSizeOf(Permission(Permission::Values::NONE)));
    ASSERT_EQ(zserio::detail::bitSizeOf(WRITE_VALUE),
            zserio::detail::bitSizeOf(Permission(Permission::Values::WRITE)));
    const Permission permission(~Permission::Values::NONE);
    ASSERT_EQ(zserio::detail::bitSizeOf(zserio::NumericLimits<Permission::ZserioType>::max()),
            zserio::detail::bitSizeOf(permission));
}

TEST_F(VarUIntBitmaskTest, writeRead)
{
    const Permission permission(Permission::Values::READ);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, permission);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    Permission readPermission;
    zserio::detail::read(reader, readPermission);
    ASSERT_EQ(READ_VALUE, readPermission.getValue());
}

TEST_F(VarUIntBitmaskTest, writeReadFile)
{
    const Permission permission(Permission::Values::READ);

    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, permission);
    zserio::writeBufferToFile(bitBuffer, BLOB_NAME);

    auto readBitBuffer = zserio::readBufferFromFile(BLOB_NAME);
    zserio::BitStreamReader reader(readBitBuffer);
    Permission readPermission;
    zserio::detail::read(reader, readPermission);
    ASSERT_EQ(permission, readPermission);
}

TEST_F(VarUIntBitmaskTest, read)
{
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, WRITE_VALUE);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    Permission permission;
    zserio::detail::read(reader, permission);
    ASSERT_EQ(WRITE_VALUE, permission.getValue());
}

TEST_F(VarUIntBitmaskTest, toString)
{
    ASSERT_EQ(std::string("0[NONE]"), Permission(Permission::Values::NONE).toString().c_str());
    ASSERT_EQ(std::string("2[READ]"), Permission(Permission::Values::READ).toString().c_str());
    ASSERT_EQ(std::string("4[WRITE]"), Permission(Permission::Values::WRITE).toString().c_str());
    ASSERT_EQ(std::string("6[READ | WRITE]"),
            (Permission::Values::READ | Permission::Values::WRITE).toString().c_str());
    ASSERT_EQ(std::string("7[READ | WRITE]"), Permission(7).toString().c_str());
    ASSERT_EQ(std::string("255[READ | WRITE]"), Permission(255).toString().c_str());
}

TEST_F(VarUIntBitmaskTest, comparisonOperator)
{
    Permission writePermission = Permission::Values::WRITE;
    Permission equalPermission = Permission::Values::WRITE;
    Permission lessThanPermission = Permission::Values::READ;
    test_utils::comparisonOperatorsTest(writePermission, equalPermission, lessThanPermission);
}

TEST_F(VarUIntBitmaskTest, operatorBitwiseOr)
{
    const Permission read(Permission::Values::READ);
    const Permission write(Permission::Values::WRITE);

    ASSERT_EQ(read | write, Permission::Values::READ | Permission::Values::WRITE);
    ASSERT_EQ(read | Permission::Values::WRITE, Permission::Values::READ | write);
    ASSERT_EQ(read, read | Permission::Values::NONE);
    ASSERT_EQ(write, Permission::Values::NONE | write);

    ASSERT_EQ(READ_VALUE | WRITE_VALUE, (read | write).getValue());
}

TEST_F(VarUIntBitmaskTest, operatorBitwiseAnd)
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

TEST_F(VarUIntBitmaskTest, operatorBitwiseXor)
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

TEST_F(VarUIntBitmaskTest, operatorBitwiseNot)
{
    const Permission read(Permission::Values::READ);

    ASSERT_EQ(Permission::Values::WRITE, ~read & Permission::Values::WRITE);
    ASSERT_EQ(Permission::Values::NONE, ~read & Permission::Values::READ);
    ASSERT_EQ(Permission::Values::WRITE, ~Permission::Values::NONE & Permission::Values::WRITE);
    ASSERT_EQ(Permission::Values::READ, ~Permission::Values::NONE & Permission::Values::READ);
    ASSERT_EQ(Permission::Values::READ | Permission::Values::WRITE,
            ~Permission::Values::NONE & (Permission::Values::READ | Permission::Values::WRITE));
}

TEST_F(VarUIntBitmaskTest, operatorBitwiseOrAssignment)
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

TEST_F(VarUIntBitmaskTest, operatorBitwiseAndAssignment)
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

TEST_F(VarUIntBitmaskTest, operatorBitwiseXorAssignment)
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

TEST_F(VarUIntBitmaskTest, stdHash)
{
    const Permission readPermission(Permission::Values::READ);
    const size_t readHash = 853; // use hardcoded values to check that the hash code is stable
    const Permission equalPermission(Permission::Values::READ);
    const Permission writePermission(Permission::Values::WRITE);
    const size_t writeHash = 855;

    test_utils::hashTest(readPermission, readHash, equalPermission, writePermission, writeHash);
}

} // namespace varuint_bitmask
} // namespace bitmask_types
