#include <cstring>
#include <string_view>

#include "gtest/gtest.h"
#include "structure_types/one_string_structure/OneStringStructure.h"

namespace structure_types
{
namespace one_string_structure
{

using allocator_type = OneStringStructure::allocator_type;
using string_type = zserio::basic_string<allocator_type>;

class OneStringStructureDataTest : public ::testing::Test
{
protected:
    static constexpr std::string_view ONE_STRING = "This is a string!";
};

class OneStringStructureViewTest : public OneStringStructureDataTest
{
protected:
    void writeOneStringStructure(zserio::BitStreamWriter& writer, std::string_view oneString)
    {
        writer.writeString(oneString);
    }

    static constexpr std::string_view BLOB_NAME = "language/structure_types/one_string_structure.blob";
    static constexpr std::string_view ONE_STRING = "This is a string!";
    static constexpr size_t ONE_STRING_STRUCTURE_BIT_SIZE = (1 + ONE_STRING.length()) * 8;

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

TEST_F(OneStringStructureDataTest, emptyConstructor)
{
    {
        OneStringStructure oneStringStructure;
        ASSERT_EQ("", oneStringStructure.oneString);
    }
    {
        OneStringStructure oneStringStructure = {};
        ASSERT_EQ("", oneStringStructure.oneString);
    }
    {
        OneStringStructure oneStringStructure(allocator_type{});
        ASSERT_EQ("", oneStringStructure.oneString);
    }
}

TEST_F(OneStringStructureDataTest, fieldConstructor)
{
    {
        const char* str = "test";
        OneStringStructure oneStringStructure(str);
        ASSERT_EQ(str, oneStringStructure.oneString);
    }
    {
        string_type movedString(1000, 'a'); // long enough to prevent small string optimization
        const void* ptr = movedString.data();
        OneStringStructure oneStringStructure(std::move(movedString));
        const void* movedPtr = oneStringStructure.oneString.data();
        ASSERT_EQ(ptr, movedPtr);
    }
    {
        // cannot use just '{}' since ctor would be ambiguous (ambiguity with move/copy ctors)
        OneStringStructure oneStringStructure(string_type{});
        ASSERT_TRUE(oneStringStructure.oneString.empty());
    }
}

// we shall test also generated 'default' methods!
TEST_F(OneStringStructureDataTest, copyConstructor)
{
    const char* str = "test";
    OneStringStructure oneStringStructure(str);
    OneStringStructure oneStringStructureCopy(oneStringStructure);
    ASSERT_EQ(str, oneStringStructure.oneString);
    ASSERT_EQ(str, oneStringStructureCopy.oneString);
}

// we shall test also generated 'default' methods!
TEST_F(OneStringStructureDataTest, copyAssignmentOperator)
{
    const char* str = "test";
    OneStringStructure oneStringStructure(str);
    OneStringStructure oneStringStructureCopy;
    oneStringStructureCopy = oneStringStructure;
    ASSERT_EQ(str, oneStringStructure.oneString);
    ASSERT_EQ(str, oneStringStructureCopy.oneString);
}

// we shall test also generated 'default' methods!
TEST_F(OneStringStructureDataTest, moveConstructor)
{
    OneStringStructure oneStringStructure(string_type(1000, 'a'));
    const void* ptr = oneStringStructure.oneString.data();
    OneStringStructure movedOneStringStructure(std::move(oneStringStructure));
    const void* movedPtr = movedOneStringStructure.oneString.data();
    ASSERT_EQ(ptr, movedPtr);
    ASSERT_EQ(string_type(1000, 'a'), movedOneStringStructure.oneString);
}

// we shall test also generated 'default' methods!
TEST_F(OneStringStructureDataTest, moveAssignmentOperator)
{
    OneStringStructure oneStringStructure(string_type(1000, 'a'));
    const void* ptr = oneStringStructure.oneString.data();
    OneStringStructure movedOneStringStructure;
    movedOneStringStructure = std::move(oneStringStructure);
    const void* movedPtr = movedOneStringStructure.oneString.data();
    ASSERT_EQ(ptr, movedPtr);
    ASSERT_EQ(string_type(1000, 'a'), movedOneStringStructure.oneString);
}

TEST_F(OneStringStructureDataTest, operatorEquality)
{
    OneStringStructure oneStringStructure1;
    OneStringStructure oneStringStructure2;
    ASSERT_TRUE(oneStringStructure1 == oneStringStructure2);

    oneStringStructure1.oneString = ONE_STRING;
    ASSERT_FALSE(oneStringStructure1 == oneStringStructure2);

    oneStringStructure2.oneString = ONE_STRING;
    ASSERT_TRUE(oneStringStructure1 == oneStringStructure2);
}

TEST_F(OneStringStructureDataTest, operatorLessThan)
{
    OneStringStructure oneStringStructure1;
    OneStringStructure oneStringStructure2;
    ASSERT_FALSE(oneStringStructure1 < oneStringStructure2);

    oneStringStructure1.oneString = ONE_STRING;
    ASSERT_FALSE(oneStringStructure1 < oneStringStructure2);
    ASSERT_TRUE(oneStringStructure2 < oneStringStructure1);

    oneStringStructure2.oneString = ONE_STRING;
    ASSERT_FALSE(oneStringStructure1 < oneStringStructure2);
    ASSERT_FALSE(oneStringStructure2 < oneStringStructure1);

    oneStringStructure1.oneString = "A string";
    ASSERT_TRUE(oneStringStructure1 < oneStringStructure2);
    ASSERT_FALSE(oneStringStructure2 < oneStringStructure1);
}

TEST_F(OneStringStructureDataTest, stdHash)
{
    OneStringStructure oneStringStructure1;
    OneStringStructure oneStringStructure2;
    std::hash<OneStringStructure> hasher;

    ASSERT_EQ(hasher(oneStringStructure1), hasher(oneStringStructure2));

    oneStringStructure1.oneString = ONE_STRING;
    ASSERT_NE(hasher(oneStringStructure1), hasher(oneStringStructure2));

    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(1773897624, hasher(oneStringStructure1));
    ASSERT_EQ(23, hasher(oneStringStructure2));

    oneStringStructure2.oneString = ONE_STRING;
    ASSERT_EQ(hasher(oneStringStructure1), hasher(oneStringStructure2));
}

TEST_F(OneStringStructureViewTest, operatorEquality)
{
    OneStringStructure oneStringStructure1;
    OneStringStructure oneStringStructure2;

    zserio::View<OneStringStructure> view1(oneStringStructure1);
    zserio::View<OneStringStructure> view2(oneStringStructure2);

    ASSERT_TRUE(view1 == view2);

    oneStringStructure1.oneString = ONE_STRING;
    ASSERT_FALSE(view1 == view2);

    oneStringStructure2.oneString = ONE_STRING;
    ASSERT_TRUE(view1 == view2);
}

TEST_F(OneStringStructureViewTest, operatorLessThan)
{
    OneStringStructure oneStringStructure1;
    OneStringStructure oneStringStructure2;

    zserio::View<OneStringStructure> view1(oneStringStructure1);
    zserio::View<OneStringStructure> view2(oneStringStructure2);

    ASSERT_FALSE(view1 < view2);

    oneStringStructure1.oneString = ONE_STRING;
    ASSERT_FALSE(view1 < view2);
    ASSERT_TRUE(view2 < view1);

    oneStringStructure2.oneString = ONE_STRING;
    ASSERT_FALSE(view1 < view2);
    ASSERT_FALSE(view2 < view1);

    oneStringStructure1.oneString = "A string";
    ASSERT_TRUE(view1 < view2);
    ASSERT_FALSE(view2 < view1);
}

TEST_F(OneStringStructureViewTest, stdHash)
{
    OneStringStructure oneStringStructure1;
    OneStringStructure oneStringStructure2;

    zserio::View<OneStringStructure> view1(oneStringStructure1);
    zserio::View<OneStringStructure> view2(oneStringStructure2);

    std::hash<zserio::View<OneStringStructure>> hasher;

    ASSERT_EQ(hasher(view1), hasher(view2));

    oneStringStructure1.oneString = ONE_STRING;
    ASSERT_NE(hasher(view1), hasher(view2));

    // TODO[Mi-L@]: View returns string_view which goes through different calcHashCode calls then string
    // use hardcoded values to check that the hash code is stable
    // ASSERT_EQ(1773897624, hasher(view1));
    // ASSERT_EQ(23, hasher(view2));

    oneStringStructure2.oneString = ONE_STRING;
    ASSERT_EQ(hasher(view1), hasher(view2));
}

TEST_F(OneStringStructureViewTest, read)
{
    zserio::BitStreamWriter writer(bitBuffer);
    writeOneStringStructure(writer, ONE_STRING);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    OneStringStructure oneStringStructure{allocator_type()};
    zserio::View<OneStringStructure> readView = zserio::detail::read(reader, oneStringStructure);
    ASSERT_EQ(ONE_STRING, readView.oneString());
}

TEST_F(OneStringStructureViewTest, writeRead)
{
    OneStringStructure oneStringStructure{string_type(ONE_STRING)};
    zserio::View<OneStringStructure> view(oneStringStructure);

    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());

    OneStringStructure readOneStringStructure{allocator_type()};
    zserio::View<OneStringStructure> readView = zserio::detail::read(reader, readOneStringStructure);
    ASSERT_EQ(ONE_STRING, readView.oneString());
    ASSERT_EQ(view, readView);
}

TEST_F(OneStringStructureViewTest, bitSizeOf)
{
    using namespace std::literals;

    OneStringStructure oneStringStructure;
    zserio::View<OneStringStructure> view(oneStringStructure);

    ASSERT_EQ(zserio::detail::bitSizeOf(""sv), zserio::detail::bitSizeOf(view, 0));

    oneStringStructure.oneString = ONE_STRING;

    ASSERT_EQ(zserio::detail::bitSizeOf(ONE_STRING), zserio::detail::bitSizeOf(view, 0));
    ASSERT_EQ(zserio::detail::bitSizeOf(ONE_STRING), zserio::detail::bitSizeOf(view, 1));
    ASSERT_EQ(zserio::detail::bitSizeOf(ONE_STRING), zserio::detail::bitSizeOf(view, 100));
}

} // namespace one_string_structure
} // namespace structure_types
