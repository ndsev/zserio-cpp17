#include <string>

#include "bitmask_types/bitfield_bitmask/Permission.h"
#include "gtest/gtest.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/CppRuntimeException.h"

namespace bitmask_types
{
namespace bitfield_bitmask
{

class BitfieldBitmaskTest : public ::testing::Test
{
protected:
    static const size_t PERMISSION_BITSIZEOF;

    static const Permission::ZserioType NONE_VALUE;
    static const Permission::ZserioType READ_VALUE;
    static const Permission::ZserioType WRITE_VALUE;

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

const size_t BitfieldBitmaskTest::PERMISSION_BITSIZEOF = 3;

const Permission::ZserioType BitfieldBitmaskTest::NONE_VALUE = 0;
const Permission::ZserioType BitfieldBitmaskTest::READ_VALUE = 2;
const Permission::ZserioType BitfieldBitmaskTest::WRITE_VALUE = 4;

TEST_F(BitfieldBitmaskTest, emptyConstructor)
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

TEST_F(BitfieldBitmaskTest, valuesConstructor)
{
    const Permission permission(Permission::Values::WRITE);
    ASSERT_EQ(WRITE_VALUE, permission.getValue());
}

TEST_F(BitfieldBitmaskTest, zserioTypeConstructor)
{
    const Permission permission(READ_VALUE);
    ASSERT_TRUE((permission & Permission::Values::READ) == Permission::Values::READ);
}

TEST_F(BitfieldBitmaskTest, copyConstructor)
{
    const Permission permission(READ_VALUE);
    const Permission copy(permission);
    ASSERT_EQ(READ_VALUE, copy.getValue());
}

TEST_F(BitfieldBitmaskTest, assignmentOperator)
{
    const Permission permission(READ_VALUE);
    Permission copy;
    copy = permission;
    ASSERT_EQ(READ_VALUE, copy.getValue());
}

TEST_F(BitfieldBitmaskTest, moveConstructor)
{
    Permission permission(READ_VALUE);
    const Permission moved(std::move(permission));
    ASSERT_EQ(READ_VALUE, moved.getValue());
}

TEST_F(BitfieldBitmaskTest, moveAssignmentOperator)
{
    Permission permission(READ_VALUE);
    Permission moved;
    moved = std::move(permission);
    ASSERT_EQ(READ_VALUE, moved.getValue());
}

TEST_F(BitfieldBitmaskTest, zserioTypeCast)
{
    const Permission permission(WRITE_VALUE);
    ASSERT_EQ(WRITE_VALUE, static_cast<Permission::ZserioType>(permission));
}

TEST_F(BitfieldBitmaskTest, getValue)
{
    ASSERT_EQ(NONE_VALUE, Permission(Permission::Values::NONE).getValue());
    ASSERT_EQ(READ_VALUE, Permission(Permission::Values::READ).getValue());
    ASSERT_EQ(WRITE_VALUE, Permission(Permission::Values::WRITE).getValue());
}

TEST_F(BitfieldBitmaskTest, bitSizeOf)
{
    ASSERT_EQ(PERMISSION_BITSIZEOF, zserio::detail::bitSizeOf(Permission(Permission::Values::NONE)));
}

TEST_F(BitfieldBitmaskTest, stdHashCode)
{
    std::hash<Permission> hasher;
    const Permission readPermission(Permission::Values::READ);
    const Permission writePermission(Permission::Values::WRITE);
    const Permission copyRead(readPermission);
    ASSERT_EQ(hasher(readPermission), hasher(copyRead));
    ASSERT_NE(hasher(readPermission), hasher(writePermission));
    ASSERT_NE(hasher(readPermission), hasher(Permission(Permission::Values::NONE)));
    ASSERT_NE(hasher(writePermission), hasher(Permission(Permission::Values::NONE)));

    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(851, hasher(Permission(Permission::Values::NONE)));
    ASSERT_EQ(853, hasher(Permission(Permission::Values::READ)));
    ASSERT_EQ(855, hasher(Permission(Permission::Values::WRITE)));
}

TEST_F(BitfieldBitmaskTest, write)
{
    const Permission permission(Permission::Values::READ);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, permission);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    ASSERT_EQ(READ_VALUE, reader.readUnsignedBits32(PERMISSION_BITSIZEOF));
}

TEST_F(BitfieldBitmaskTest, read)
{
    zserio::BitStreamWriter writer(bitBuffer);
    writer.writeUnsignedBits32(static_cast<uint32_t>(Permission::Values::WRITE), PERMISSION_BITSIZEOF);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    Permission permission;
    zserio::detail::read(reader, permission);
    ASSERT_EQ(WRITE_VALUE, permission.getValue());
}

TEST_F(BitfieldBitmaskTest, toString)
{
    ASSERT_EQ(std::string("0[NONE]"), Permission(Permission::Values::NONE).toString().c_str());
    ASSERT_EQ(std::string("2[READ]"), Permission(Permission::Values::READ).toString().c_str());
    ASSERT_EQ(std::string("4[WRITE]"), Permission(Permission::Values::WRITE).toString().c_str());
    ASSERT_EQ(std::string("6[READ | WRITE]"),
            (Permission::Values::READ | Permission::Values::WRITE).toString().c_str());
    ASSERT_EQ(std::string("7[READ | WRITE]"), Permission(7).toString().c_str());
}

TEST_F(BitfieldBitmaskTest, operatorEquality)
{
    ASSERT_TRUE(Permission::Values::READ == Permission::Values::READ);
    ASSERT_FALSE(Permission::Values::READ == Permission::Values::WRITE);
    ASSERT_TRUE(Permission::Values::WRITE == Permission::Values::WRITE);

    const Permission read(Permission::Values::READ);
    ASSERT_TRUE(read == Permission::Values::READ);
    ASSERT_TRUE(Permission::Values::READ == read);
    ASSERT_FALSE(read == Permission::Values::WRITE);
    ASSERT_FALSE(Permission::Values::WRITE == read);

    const Permission write(Permission::Values::WRITE);
    ASSERT_TRUE(write == Permission::Values::WRITE);
    ASSERT_TRUE(Permission::Values::WRITE == write);
    ASSERT_FALSE(write == Permission::Values::READ);
    ASSERT_FALSE(Permission::Values::READ == write);

    ASSERT_TRUE(read == read);
    ASSERT_TRUE(read == Permission(read)); // copy
    ASSERT_TRUE(write == write);
    ASSERT_TRUE(write == Permission(write)); // copy

    ASSERT_FALSE(read == write);
}

TEST_F(BitfieldBitmaskTest, operatorInequality)
{
    ASSERT_FALSE(Permission::Values::READ != Permission::Values::READ);
    ASSERT_TRUE(Permission::Values::READ != Permission::Values::WRITE);
    ASSERT_FALSE(Permission::Values::WRITE != Permission::Values::WRITE);

    const Permission read(Permission::Values::READ);
    ASSERT_FALSE(read != Permission::Values::READ);
    ASSERT_FALSE(Permission::Values::READ != read);
    ASSERT_TRUE(read != Permission::Values::WRITE);
    ASSERT_TRUE(Permission::Values::WRITE != read);

    const Permission write(Permission::Values::WRITE);
    ASSERT_FALSE(write != Permission::Values::WRITE);
    ASSERT_FALSE(Permission::Values::WRITE != write);
    ASSERT_TRUE(write != Permission::Values::READ);
    ASSERT_TRUE(Permission::Values::READ != write);

    ASSERT_FALSE(read != read);
    ASSERT_FALSE(read != Permission(read)); // copy
    ASSERT_FALSE(write != write);
    ASSERT_FALSE(write != Permission(write)); // copy

    ASSERT_TRUE(read != write);
}

TEST_F(BitfieldBitmaskTest, operatorLessThan)
{
    ASSERT_TRUE(Permission::Values::NONE < Permission::Values::READ);
    ASSERT_FALSE(Permission::Values::READ < Permission::Values::NONE);

    ASSERT_TRUE(Permission::Values::READ < Permission::Values::WRITE);
    ASSERT_FALSE(Permission::Values::WRITE < Permission::Values::READ);

    ASSERT_FALSE(Permission::Values::NONE < Permission::Values::NONE);
    ASSERT_FALSE(Permission::Values::READ < Permission::Values::READ);
    ASSERT_FALSE(Permission::Values::WRITE < Permission::Values::WRITE);

    ASSERT_TRUE(Permission::Values::READ < (Permission::Values::READ | Permission::Values::WRITE));
    ASSERT_FALSE((Permission::Values::READ | Permission::Values::WRITE) < Permission::Values::READ);

    const Permission read(Permission::Values::READ);
    const Permission write(Permission::Values::WRITE);
    ASSERT_TRUE(read < write);
    ASSERT_FALSE(write < read);
}

TEST_F(BitfieldBitmaskTest, operatorBitwiseOr)
{
    const Permission read(Permission::Values::READ);
    const Permission write(Permission::Values::WRITE);

    ASSERT_EQ(read | write, Permission::Values::READ | Permission::Values::WRITE);
    ASSERT_EQ(read | Permission::Values::WRITE, Permission::Values::READ | write);
    ASSERT_EQ(read, read | Permission::Values::NONE);
    ASSERT_EQ(write, Permission::Values::NONE | write);

    ASSERT_EQ(READ_VALUE | WRITE_VALUE, (read | write).getValue());
}

TEST_F(BitfieldBitmaskTest, operatorBitwiseAnd)
{
    const Permission read(Permission::Values::READ);
    const Permission write(Permission::Values::WRITE);
    const Permission readwrite(Permission::Values::READ | Permission::Values::WRITE);

    ASSERT_EQ(Permission::Values::READ, readwrite & read);
    ASSERT_EQ(Permission::Values::WRITE, readwrite & write);
    ASSERT_EQ(read, readwrite & Permission::Values::READ);
    ASSERT_EQ(read, Permission::Values::READ & readwrite);
    ASSERT_EQ(write, Permission::Values::WRITE & readwrite);
    ASSERT_EQ(Permission::Values::NONE, readwrite & Permission::Values::NONE);

    ASSERT_EQ(read, read & read & read & read);
}

TEST_F(BitfieldBitmaskTest, operatorBitwiseXor)
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

TEST_F(BitfieldBitmaskTest, operatorBitwiseNot)
{
    const Permission read(Permission::Values::READ);

    ASSERT_EQ(Permission::Values::WRITE, ~read & Permission::Values::WRITE);
    ASSERT_EQ(Permission::Values::NONE, ~read & Permission::Values::READ);
    ASSERT_EQ(Permission::Values::WRITE, ~Permission::Values::NONE & Permission::Values::WRITE);
    ASSERT_EQ(Permission::Values::READ, ~Permission::Values::NONE & Permission::Values::READ);
    ASSERT_EQ(Permission::Values::READ | Permission::Values::WRITE,
            ~Permission::Values::NONE & (Permission::Values::READ | Permission::Values::WRITE));
}

TEST_F(BitfieldBitmaskTest, operatorBitwiseOrAssignment)
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

TEST_F(BitfieldBitmaskTest, operatorBitwiseAndAssignment)
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

TEST_F(BitfieldBitmaskTest, operatorBitwiseXorAssignment)
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

} // namespace bitfield_bitmask
} // namespace bitmask_types
