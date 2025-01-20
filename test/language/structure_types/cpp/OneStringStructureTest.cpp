#include <string_view>

#include "gtest/gtest.h"
#include "structure_types/one_string_structure/OneStringStructure.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace structure_types
{
namespace one_string_structure
{

using AllocatorType = OneStringStructure::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

class OneStringStructureTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeString(ONE_STRING);
    }

    static constexpr std::string_view BLOB_NAME = "language/structure_types/one_string_structure.blob";
    static constexpr std::string_view ONE_STRING = "This is a string!";
    static constexpr std::string_view ONE_DIFF_STRING = "This is a different string!";
    static constexpr size_t ONE_STRING_STRUCTURE_BIT_SIZE = (1 + ONE_STRING.length()) * 8;
};

TEST_F(OneStringStructureTest, emptyConstructor)
{
    {
        OneStringStructure data;
        ASSERT_EQ("", data.oneString);
    }
    {
        OneStringStructure data = {};
        ASSERT_EQ("", data.oneString);
    }
    {
        OneStringStructure data(AllocatorType{});
        ASSERT_EQ("", data.oneString);
    }
    {
        OneStringStructure data;
        zserio::View view(data);
        ASSERT_EQ("", view.oneString());
    }
}

TEST_F(OneStringStructureTest, fieldConstructor)
{
    {
        const StringType oneString = "test";
        OneStringStructure data(oneString);
        ASSERT_EQ(oneString, data.oneString);
    }
    {
        StringType movedString(1000, 'a'); // long enough to prevent small string optimization
        const void* ptr = movedString.data();
        OneStringStructure data(std::move(movedString));
        const void* movedPtr = data.oneString.data();
        ASSERT_EQ(ptr, movedPtr);
    }
    {
        // cannot use just '{}' since ctor would be ambiguous (ambiguity with move/copy ctors)
        OneStringStructure data(StringType{});
        ASSERT_TRUE(data.oneString.empty());
    }
    {
        const StringType oneString = "yet another test";
        OneStringStructure data(oneString);
        zserio::View view(data);
        ASSERT_EQ(oneString, view.oneString());
    }
}

TEST_F(OneStringStructureTest, copyConstructor)
{
    const StringType oneString = "test";
    OneStringStructure data(oneString);
    OneStringStructure dataCopy(data);
    ASSERT_EQ(dataCopy, data);

    zserio::View view(data);
    OneStringStructure emptyData{};
    zserio::View viewCopy(emptyData);
    viewCopy = view;
    ASSERT_EQ(viewCopy, view);
}

TEST_F(OneStringStructureTest, assignmentOperator)
{
    const StringType oneString = "test";
    OneStringStructure data(oneString);
    OneStringStructure dataCopy;
    dataCopy = data;
    ASSERT_EQ(dataCopy, data);

    zserio::View view(data);
    OneStringStructure emptyData{};
    zserio::View viewCopy(emptyData);
    viewCopy = view;
    ASSERT_EQ(viewCopy, view);
}

TEST_F(OneStringStructureTest, moveConstructor)
{
    OneStringStructure data(StringType(1000, 'a'));
    const void* ptr = data.oneString.data();
    OneStringStructure dataMoved(std::move(data));
    const void* movedPtr = dataMoved.oneString.data();
    ASSERT_EQ(ptr, movedPtr);
    ASSERT_EQ(StringType(1000, 'a'), dataMoved.oneString);

    zserio::View view(dataMoved);
    zserio::View viewMoved(std::move(view));
    ASSERT_EQ(StringType(1000, 'a'), viewMoved.oneString());
}

TEST_F(OneStringStructureTest, moveAssignmentOperator)
{
    OneStringStructure data(StringType(1000, 'a'));
    const void* ptr = data.oneString.data();
    OneStringStructure dataMoved;
    dataMoved = std::move(data);
    const void* movedPtr = dataMoved.oneString.data();
    ASSERT_EQ(ptr, movedPtr);
    ASSERT_EQ(StringType(1000, 'a'), dataMoved.oneString);

    zserio::View view(dataMoved);
    OneStringStructure emptyData{};
    zserio::View viewMoved(emptyData);
    viewMoved = std::move(view);
    ASSERT_EQ(StringType(1000, 'a'), dataMoved.oneString);
}

TEST_F(OneStringStructureTest, oneString)
{
    const StringType oneString = "test";
    OneStringStructure data(oneString);
    ASSERT_EQ(oneString, data.oneString);

    zserio::View view(data);
    ASSERT_EQ(oneString, view.oneString());
}

TEST_F(OneStringStructureTest, comparisonOperators)
{
    const StringType oneString(ONE_STRING);
    OneStringStructure data(oneString);
    OneStringStructure equalData(oneString);
    OneStringStructure lessThenData("This is a bbb string!");
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThenView(lessThenData);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);

    OneStringStructure anotherLessThenData("This is a aaa string!");
    test_utils::comparisonOperatorsTest(data, equalData, anotherLessThenData);

    zserio::View anotherLessThenView(anotherLessThenData);
    test_utils::comparisonOperatorsTest(view, equalView, anotherLessThenView);
}

TEST_F(OneStringStructureTest, validate)
{
    OneStringStructure data;
    data.oneString = ONE_STRING;
    zserio::View view(data);
    ASSERT_NO_THROW(zserio::detail::validate(view)); // always valid
}

TEST_F(OneStringStructureTest, bitSizeOf)
{
    OneStringStructure data;
    data.oneString = ONE_STRING;
    zserio::View view(data);
    ASSERT_EQ(ONE_STRING_STRUCTURE_BIT_SIZE, zserio::detail::bitSizeOf(view, 0));
    ASSERT_EQ(ONE_STRING_STRUCTURE_BIT_SIZE, zserio::detail::bitSizeOf(view, 1));
    ASSERT_EQ(ONE_STRING_STRUCTURE_BIT_SIZE, zserio::detail::bitSizeOf(view, 100));
}

TEST_F(OneStringStructureTest, read)
{
    OneStringStructure data;
    data.oneString = ONE_STRING;
    test_utils::readTest(writeData, data);
}

TEST_F(OneStringStructureTest, writeRead)
{
    OneStringStructure data;
    data.oneString = ONE_STRING;
    test_utils::writeReadTest(data);
}

TEST_F(OneStringStructureTest, writeReadFile)
{
    OneStringStructure data;
    data.oneString = ONE_STRING;
    test_utils::writeReadFileTest(BLOB_NAME, data);
}

TEST_F(OneStringStructureTest, stdHash)
{
    const StringType oneString(ONE_STRING);
    OneStringStructure data(oneString);
    const size_t dataHash = 1773897624; // hardcoded value to check that the hash code is stable
    OneStringStructure equalData(oneString);
    const StringType oneDiffString(ONE_DIFF_STRING);
    OneStringStructure diffData(oneDiffString);
    const size_t diffDataHash = 505292923; // hardcoded value to check that the hash code is stable
    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 1773897624; // hardcoded value to check that the hash code is stable
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffHash = 505292923; // hardcoded value to check that the hash code is stable
    test_utils::hashTest(view, viewHash, equalView, diffView, diffHash);
}

} // namespace one_string_structure
} // namespace structure_types
