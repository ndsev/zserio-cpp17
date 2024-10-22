#include "gtest/gtest.h"
#include "structure_types/simple_structure/SimpleStructure.h"

namespace structure_types
{
namespace simple_structure
{

using allocator_type = SimpleStructure::allocator_type;

class SimpleStructureDataTest : public ::testing::Test
{
protected:
};

class SimpleStructureViewTest : public SimpleStructureDataTest
{
protected:
    void writeSimpleStructure(
            zserio::BitStreamWriter& writer, uint8_t numberA, uint8_t numberB, uint8_t numberC)
    {
        zserio::detail::write(writer, zserio::UInt3(numberA));
        zserio::detail::write(writer, zserio::UInt8(numberB));
        zserio::detail::write(writer, zserio::UInt7(numberC));
    }

    static constexpr size_t SIMPLE_STRUCTURE_BIT_SIZE = 18;
    zserio::BitBuffer bitBuffer = zserio::BitBuffer(SIMPLE_STRUCTURE_BIT_SIZE);
};

TEST_F(SimpleStructureDataTest, emptyConstructor)
{
    {
        SimpleStructure simpleStructure;
        ASSERT_EQ(0, simpleStructure.numberA);
        ASSERT_EQ(0, simpleStructure.numberB);
        ASSERT_EQ(0, simpleStructure.numberC);
    }
    {
        SimpleStructure simpleStructure = {};
        ASSERT_EQ(0, simpleStructure.numberA);
        ASSERT_EQ(0, simpleStructure.numberB);
        ASSERT_EQ(0, simpleStructure.numberC);
    }
    {
        SimpleStructure simpleStructure(allocator_type{});
        ASSERT_EQ(0, simpleStructure.numberA);
        ASSERT_EQ(0, simpleStructure.numberB);
        ASSERT_EQ(0, simpleStructure.numberC);
    }
}

TEST_F(SimpleStructureDataTest, fieldConstructor)
{
    {
        const uint8_t numberA = 0x07;
        const uint8_t numberB = 0xFF;
        const uint8_t numberC = 0x7F;
        SimpleStructure simpleStructure(numberA, numberB, numberC);
        ASSERT_EQ(numberA, simpleStructure.numberA);
        ASSERT_EQ(numberB, simpleStructure.numberB);
        ASSERT_EQ(numberC, simpleStructure.numberC);
    }
    {
        SimpleStructure simpleStructure({}, {}, {});
        ASSERT_EQ(0, simpleStructure.numberA);
        ASSERT_EQ(0, simpleStructure.numberB);
        ASSERT_EQ(0, simpleStructure.numberC);
    }
}

TEST_F(SimpleStructureDataTest, copyConstructor)
{
    const uint8_t numberA = 0x07;
    const uint8_t numberB = 0xFF;
    const uint8_t numberC = 0x7F;
    SimpleStructure simpleStructure(numberA, numberB, numberC);
    SimpleStructure simpleStructureCopy(simpleStructure);
    ASSERT_EQ(numberA, simpleStructureCopy.numberA);
    ASSERT_EQ(numberB, simpleStructureCopy.numberB);
    ASSERT_EQ(numberC, simpleStructureCopy.numberC);
}

TEST_F(SimpleStructureDataTest, assignmentOperator)
{
    const uint8_t numberA = 0x07;
    const uint8_t numberB = 0xFF;
    const uint8_t numberC = 0x7F;
    SimpleStructure simpleStructure(numberA, numberB, numberC);
    SimpleStructure simpleStructureCopy;
    simpleStructureCopy = simpleStructure;
    ASSERT_EQ(numberA, simpleStructureCopy.numberA);
    ASSERT_EQ(numberB, simpleStructureCopy.numberB);
    ASSERT_EQ(numberC, simpleStructureCopy.numberC);
}

TEST_F(SimpleStructureDataTest, moveConstructor)
{
    const uint8_t numberA = 0x07;
    const uint8_t numberB = 0xFF;
    const uint8_t numberC = 0x7F;
    SimpleStructure simpleStructure(numberA, numberB, numberC);
    SimpleStructure simpleStructureMoved(std::move(simpleStructure));
    ASSERT_EQ(numberA, simpleStructureMoved.numberA);
    ASSERT_EQ(numberB, simpleStructureMoved.numberB);
    ASSERT_EQ(numberC, simpleStructureMoved.numberC);
}

TEST_F(SimpleStructureDataTest, moveAssignmentOperator)
{
    const uint8_t numberA = 0x07;
    const uint8_t numberB = 0xFF;
    const uint8_t numberC = 0x7F;
    SimpleStructure simpleStructure(numberA, numberB, numberC);
    SimpleStructure simpleStructureMoved;
    simpleStructureMoved = std::move(simpleStructure);
    ASSERT_EQ(numberA, simpleStructureMoved.numberA);
    ASSERT_EQ(numberB, simpleStructureMoved.numberB);
    ASSERT_EQ(numberC, simpleStructureMoved.numberC);
}

TEST_F(SimpleStructureDataTest, getSetNumberA)
{
    SimpleStructure simpleStructure;
    const uint8_t numberA = 0x02;
    simpleStructure.numberA = numberA;
    ASSERT_EQ(numberA, simpleStructure.numberA);
}

TEST_F(SimpleStructureDataTest, getSetNumberB)
{
    SimpleStructure simpleStructure;
    const uint8_t numberB = 0x23;
    simpleStructure.numberB = numberB;
    ASSERT_EQ(numberB, simpleStructure.numberB);
}

TEST_F(SimpleStructureDataTest, getSetNumberC)
{
    SimpleStructure simpleStructure;
    const uint8_t numberC = 0x11;
    simpleStructure.numberC = numberC;
    ASSERT_EQ(numberC, simpleStructure.numberC);
}

TEST_F(SimpleStructureDataTest, operatorEquality)
{
    SimpleStructure simpleStructure1;
    SimpleStructure simpleStructure2;

    const uint8_t numberA = 0x03;
    const uint8_t numberB = 0xDE;
    const uint8_t numberC = 0x55;
    simpleStructure1.numberA = numberA;
    simpleStructure1.numberB = numberB;
    simpleStructure1.numberC = numberC;
    simpleStructure2.numberA = numberA;
    simpleStructure2.numberB = numberB + 1;
    simpleStructure2.numberC = numberC;
    ASSERT_FALSE(simpleStructure1 == simpleStructure2);

    simpleStructure2.numberB = numberB;
    ASSERT_TRUE(simpleStructure1 == simpleStructure2);
}

TEST_F(SimpleStructureDataTest, operatorLessThan)
{
    SimpleStructure simpleStructure1;
    SimpleStructure simpleStructure2;

    ASSERT_FALSE(simpleStructure1 < simpleStructure2);
    ASSERT_FALSE(simpleStructure2 < simpleStructure1);

    simpleStructure1.numberA = 1;
    simpleStructure2.numberA = 1;
    simpleStructure1.numberB = 1;
    simpleStructure2.numberB = 1;
    simpleStructure1.numberC = 1;
    simpleStructure2.numberC = 2;
    ASSERT_TRUE(simpleStructure1 < simpleStructure2);
    ASSERT_FALSE(simpleStructure2 < simpleStructure1);

    simpleStructure1.numberB = 2;
    ASSERT_FALSE(simpleStructure1 < simpleStructure2);
    ASSERT_TRUE(simpleStructure2 < simpleStructure1);
}

TEST_F(SimpleStructureDataTest, stdHash)
{
    SimpleStructure simpleStructure1;
    SimpleStructure simpleStructure2;

    std::hash<SimpleStructure> hasher;

    const uint8_t numberA = 0x04;
    const uint8_t numberB = 0xCD;
    const uint8_t numberC = 0x57;
    simpleStructure1.numberA = numberA;
    simpleStructure1.numberB = numberB;
    simpleStructure1.numberC = numberC;
    simpleStructure2.numberA = numberA;
    simpleStructure2.numberB = numberB + 1;
    simpleStructure2.numberC = numberC;
    ASSERT_NE(hasher(simpleStructure1), hasher(simpleStructure2));

    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(1178167, hasher(simpleStructure1));
    ASSERT_EQ(1178204, hasher(simpleStructure2));

    simpleStructure2.numberB = numberB;
    ASSERT_EQ(hasher(simpleStructure1), hasher(simpleStructure2));
}

TEST_F(SimpleStructureViewTest, operatorEquality)
{
    SimpleStructure simpleStructure1;
    SimpleStructure simpleStructure2;
    zserio::View<SimpleStructure> view1(simpleStructure1);
    zserio::View<SimpleStructure> view2(simpleStructure2);
    ASSERT_TRUE(view1 == view2);

    const uint8_t numberA = 0x03;
    const uint8_t numberB = 0xDE;
    const uint8_t numberC = 0x55;
    simpleStructure1.numberA = numberA;
    simpleStructure1.numberB = numberB;
    simpleStructure1.numberC = numberC;
    simpleStructure2.numberA = numberA;
    simpleStructure2.numberB = numberB + 1;
    simpleStructure2.numberC = numberC;
    ASSERT_FALSE(view1 == view2);

    simpleStructure2.numberB = numberB;
    ASSERT_TRUE(view1 == view2);
}

TEST_F(SimpleStructureViewTest, operatorLessThan)
{
    SimpleStructure simpleStructure1;
    SimpleStructure simpleStructure2;
    zserio::View<SimpleStructure> view1(simpleStructure1);
    zserio::View<SimpleStructure> view2(simpleStructure2);

    ASSERT_FALSE(view1 < view2);
    ASSERT_FALSE(view2 < view1);

    simpleStructure1.numberA = 1;
    simpleStructure2.numberA = 1;
    simpleStructure1.numberB = 1;
    simpleStructure2.numberB = 1;
    simpleStructure1.numberC = 1;
    simpleStructure2.numberC = 2;
    ASSERT_TRUE(view1 < view2);
    ASSERT_FALSE(view2 < view1);

    simpleStructure1.numberB = 2;
    ASSERT_FALSE(view1 < view2);
    ASSERT_TRUE(view2 < view1);
}

TEST_F(SimpleStructureViewTest, stdHash)
{
    SimpleStructure simpleStructure1;
    SimpleStructure simpleStructure2;
    zserio::View<SimpleStructure> view1(simpleStructure1);
    zserio::View<SimpleStructure> view2(simpleStructure2);

    std::hash<zserio::View<SimpleStructure>> hasher;

    const uint8_t numberA = 0x04;
    const uint8_t numberB = 0xCD;
    const uint8_t numberC = 0x57;
    simpleStructure1.numberA = numberA;
    simpleStructure1.numberB = numberB;
    simpleStructure1.numberC = numberC;
    simpleStructure2.numberA = numberA;
    simpleStructure2.numberB = numberB + 1;
    simpleStructure2.numberC = numberC;
    ASSERT_NE(hasher(view1), hasher(view2));

    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(1178167, hasher(view1));
    ASSERT_EQ(1178204, hasher(view2));

    simpleStructure2.numberB = numberB;
    ASSERT_EQ(hasher(view1), hasher(view2));
}

TEST_F(SimpleStructureViewTest, read)
{
    zserio::BitStreamWriter writer(bitBuffer);
    const uint8_t numberA = 0x04;
    const uint8_t numberB = 0xCD;
    const uint8_t numberC = 0x57;
    writeSimpleStructure(writer, numberA, numberB, numberC);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    SimpleStructure simpleStructure{allocator_type()};
    zserio::View<SimpleStructure> readView = zserio::detail::read(reader, simpleStructure);
    ASSERT_EQ(numberA, readView.numberA());
    ASSERT_EQ(numberB, readView.numberB());
    ASSERT_EQ(numberC, readView.numberC());
}

TEST_F(SimpleStructureViewTest, writeRead)
{
    SimpleStructure simpleStructure(0x04, 0xCD, 0x57);
    zserio::View<SimpleStructure> view(simpleStructure);

    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    SimpleStructure readSimpleStructure;
    zserio::View<SimpleStructure> readView = zserio::detail::read(reader, readSimpleStructure);
    ASSERT_EQ(readView, view);
}

TEST_F(SimpleStructureViewTest, bitSizeOf)
{
    const uint8_t numberA = 0x04;
    const uint8_t numberB = 0xCD;
    const uint8_t numberC = 0x57;

    SimpleStructure simpleStructure(numberA, numberB, numberC);
    zserio::View<SimpleStructure> view(simpleStructure);
    ASSERT_EQ(SIMPLE_STRUCTURE_BIT_SIZE, zserio::detail::bitSizeOf(view, 0));
    ASSERT_EQ(SIMPLE_STRUCTURE_BIT_SIZE, zserio::detail::bitSizeOf(view, 1));
    ASSERT_EQ(SIMPLE_STRUCTURE_BIT_SIZE, zserio::detail::bitSizeOf(view, 100));
}

} // namespace simple_structure
} // namespace structure_types
