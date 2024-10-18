#include "gtest/gtest.h"
#include "structure_types/empty_structure/EmptyStructure.h"

namespace structure_types
{
namespace empty_structure
{

using allocator_type = EmptyStructure::allocator_type;

TEST(EmptyStructureDataTest, emptyConstructor)
{
    {
        EmptyStructure emptyStructure;
        (void)emptyStructure;
    }
    {
        EmptyStructure emptyStructure = {};
        (void)emptyStructure;
    }
    {
        EmptyStructure emptyStructure(allocator_type{});
        (void)emptyStructure;
    }
}

TEST(EmptyStructureDataTest, copyConstructor)
{
    EmptyStructure emptyStructure;
    EmptyStructure emptyStructureCopy(emptyStructure);
    ASSERT_EQ(emptyStructure, emptyStructureCopy);
}

TEST(EmptyStructureDataTest, assignmentOperator)
{
    EmptyStructure emptyStructure;
    EmptyStructure emptyStructureCopy;
    emptyStructureCopy = emptyStructure;
    ASSERT_EQ(emptyStructure, emptyStructureCopy);
}

TEST(EmptyStructureDataTest, moveConstructor)
{
    EmptyStructure emptyStructure;
    EmptyStructure emptyStructureMoved(std::move(emptyStructure));
    (void)emptyStructureMoved;
}

TEST(EmptyStructureDataTest, moveAssignmentOperator)
{
    EmptyStructure emptyStructure;
    EmptyStructure emptyStructureMoved;
    emptyStructureMoved = std::move(emptyStructure);
    (void)emptyStructureMoved;
}

TEST(EmptyStructureDataTest, operatorEquality)
{
    EmptyStructure emptyStructure1;
    EmptyStructure emptyStructure2;
    ASSERT_TRUE(emptyStructure1 == emptyStructure2);
}

TEST(EmptyStructureDataTest, operatorLessThan)
{
    EmptyStructure emptyStructure1;
    EmptyStructure emptyStructure2;
    ASSERT_FALSE(emptyStructure1 < emptyStructure2);
    ASSERT_FALSE(emptyStructure2 < emptyStructure1);
}

TEST(EmptyStructureDataTest, stdHash)
{
    std::hash<EmptyStructure> hasher;

    EmptyStructure emptyStructure1;
    EmptyStructure emptyStructure2;
    ASSERT_EQ(hasher(emptyStructure1), hasher(emptyStructure2));

    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(23, hasher(emptyStructure1));
}

TEST(EmptyStructureViewTest, operatorEquality)
{
    EmptyStructure emptyStructure1;
    EmptyStructure emptyStructure2;

    zserio::View<EmptyStructure> view1(emptyStructure1);
    zserio::View<EmptyStructure> view2(emptyStructure2);
    ASSERT_TRUE(view1 == view2);
}

TEST(EmptyStructureViewTest, operatorLessThan)
{
    EmptyStructure emptyStructure1;
    EmptyStructure emptyStructure2;

    zserio::View<EmptyStructure> view1(emptyStructure1);
    zserio::View<EmptyStructure> view2(emptyStructure2);
    ASSERT_FALSE(view1 < view2);
    ASSERT_FALSE(view2 < view1);
}

TEST(EmptyStructureViewTest, stdHash)
{
    std::hash<zserio::View<EmptyStructure>> hasher;

    EmptyStructure emptyStructure1;
    EmptyStructure emptyStructure2;

    zserio::View<EmptyStructure> view1(emptyStructure1);
    zserio::View<EmptyStructure> view2(emptyStructure2);
    ASSERT_EQ(hasher(view1), hasher(view2));

    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(23, hasher(view1));
}

TEST(EmptyStructureViewTest, read)
{
    zserio::BitStreamReader reader(zserio::Span<const uint8_t>{});
    EmptyStructure emptyStructure{allocator_type()};
    zserio::View<EmptyStructure> readView = zserio::detail::read(reader, emptyStructure);
    (void)readView;
}

TEST(EmptyStructureViewTest, writeRead)
{
    EmptyStructure emptyStructure{allocator_type()};
    zserio::View<EmptyStructure> view(emptyStructure);

    zserio::BitBuffer bitBuffer(0);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    EmptyStructure readEmptyStructure{allocator_type()};
    zserio::View<EmptyStructure> readView = zserio::detail::read(reader, readEmptyStructure);
    ASSERT_EQ(view, readView);
}

TEST(EmptyStructureViewTest, bitSizeOf)
{
    EmptyStructure emptyStructure;
    zserio::View<EmptyStructure> view(emptyStructure);
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view, 0));
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view, 1));
    ASSERT_EQ(0, zserio::detail::bitSizeOf(view, 100));
}

} // namespace empty_structure
} // namespace structure_types
