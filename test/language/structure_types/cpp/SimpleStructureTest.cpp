#include "gtest/gtest.h"
#include "structure_types/simple_structure/SimpleStructure.h"
#include "test_utils/TestUtility.h"

namespace structure_types
{
namespace simple_structure
{

using AllocatorType = SimpleStructure::allocator_type;

class SimpleStructureTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        zserio::detail::write(writer, zserio::UInt3(0x04));
        zserio::detail::write(writer, zserio::UInt8(0xCD));
        zserio::detail::write(writer, zserio::UInt7(0x57));
    }

    static constexpr size_t SIMPLE_STRUCTURE_BIT_SIZE = 18;
};

TEST_F(SimpleStructureTest, emptyConstructor)
{
    {
        SimpleStructure data;
        ASSERT_EQ(0, data.numberA);
        ASSERT_EQ(0, data.numberB);
        ASSERT_EQ(0, data.numberC);
    }
    {
        SimpleStructure data = {};
        ASSERT_EQ(0, data.numberA);
        ASSERT_EQ(0, data.numberB);
        ASSERT_EQ(0, data.numberC);
    }
    {
        SimpleStructure data(AllocatorType{});
        ASSERT_EQ(0, data.numberA);
        ASSERT_EQ(0, data.numberB);
        ASSERT_EQ(0, data.numberC);
    }
    {
        SimpleStructure data;
        zserio::View view(data);
        ASSERT_EQ(0, view.numberA());
        ASSERT_EQ(0, view.numberB());
        ASSERT_EQ(0, view.numberC());
    }
}

TEST_F(SimpleStructureTest, fieldConstructor)
{
    {
        // normal arguments
        const uint8_t numberA = 0x07;
        const uint8_t numberB = 0xFF;
        const uint8_t numberC = 0x7F;
        SimpleStructure data(numberA, numberB, numberC);
        ASSERT_EQ(numberA, data.numberA);
        ASSERT_EQ(numberB, data.numberB);
        ASSERT_EQ(numberC, data.numberC);

        zserio::View view(data);
        ASSERT_EQ(numberA, view.numberA());
        ASSERT_EQ(numberB, view.numberB());
        ASSERT_EQ(numberC, view.numberC());
    }

    {
        // initializer list
        const uint8_t numberA = 0x07;
        const uint8_t numberB = 0xFF;
        const uint8_t numberC = 0x7F;
        SimpleStructure data({numberA, numberB, numberC});
        ASSERT_EQ(numberA, data.numberA);
        ASSERT_EQ(numberB, data.numberB);
        ASSERT_EQ(numberC, data.numberC);

        zserio::View view(data);
        ASSERT_EQ(numberA, view.numberA());
        ASSERT_EQ(numberB, view.numberB());
        ASSERT_EQ(numberC, view.numberC());
    }
}

TEST_F(SimpleStructureTest, copyConstructor)
{
    const uint8_t numberA = 0x07;
    const uint8_t numberB = 0xFF;
    const uint8_t numberC = 0x7F;
    SimpleStructure data(numberA, numberB, numberC);
    SimpleStructure dataCopy(data);
    ASSERT_EQ(dataCopy, data);

    zserio::View view(data);
    zserio::View viewCopy(view);
    ASSERT_EQ(viewCopy, view);
}

TEST_F(SimpleStructureTest, assignmentOperator)
{
    const uint8_t numberA = 0x07;
    const uint8_t numberB = 0xFF;
    const uint8_t numberC = 0x7F;
    SimpleStructure data(numberA, numberB, numberC);
    SimpleStructure dataCopy;
    dataCopy = data;
    ASSERT_EQ(dataCopy, data);

    zserio::View view(data);
    SimpleStructure emptyData{};
    zserio::View viewCopy(emptyData);
    viewCopy = view;
    ASSERT_EQ(viewCopy, view);
}

TEST_F(SimpleStructureTest, moveConstructor)
{
    const uint8_t numberA = 0x07;
    const uint8_t numberB = 0xFF;
    const uint8_t numberC = 0x7F;
    SimpleStructure data(numberA, numberB, numberC);
    SimpleStructure dataMoved(std::move(data));
    ASSERT_EQ(numberA, dataMoved.numberA);
    ASSERT_EQ(numberB, dataMoved.numberB);
    ASSERT_EQ(numberC, dataMoved.numberC);

    zserio::View view(dataMoved);
    zserio::View viewMoved(std::move(view));
    ASSERT_EQ(numberA, viewMoved.numberA());
    ASSERT_EQ(numberB, viewMoved.numberB());
    ASSERT_EQ(numberC, viewMoved.numberC());
}

TEST_F(SimpleStructureTest, moveAssignmentOperator)
{
    const uint8_t numberA = 0x07;
    const uint8_t numberB = 0xFF;
    const uint8_t numberC = 0x7F;
    SimpleStructure data(numberA, numberB, numberC);
    SimpleStructure dataMoved;
    dataMoved = std::move(data);
    ASSERT_EQ(numberA, dataMoved.numberA);
    ASSERT_EQ(numberB, dataMoved.numberB);
    ASSERT_EQ(numberC, dataMoved.numberC);

    zserio::View view(dataMoved);
    SimpleStructure emptyData{};
    zserio::View viewMoved(emptyData);
    viewMoved = std::move(view);
    ASSERT_EQ(numberA, viewMoved.numberA());
    ASSERT_EQ(numberB, viewMoved.numberB());
    ASSERT_EQ(numberC, viewMoved.numberC());
}

TEST_F(SimpleStructureTest, numberA)
{
    SimpleStructure data;
    const uint8_t numberA = 0x02;
    data.numberA = numberA;
    ASSERT_EQ(numberA, data.numberA);

    zserio::View view(data);
    ASSERT_EQ(numberA, view.numberA());
}

TEST_F(SimpleStructureTest, numberB)
{
    SimpleStructure data;
    const uint8_t numberB = 0x23;
    data.numberB = numberB;
    ASSERT_EQ(numberB, data.numberB);

    zserio::View view(data);
    ASSERT_EQ(numberB, view.numberB());
}

TEST_F(SimpleStructureTest, numberC)
{
    SimpleStructure data;
    const uint8_t numberC = 0x11;
    data.numberC = numberC;
    ASSERT_EQ(numberC, data.numberC);

    zserio::View view(data);
    ASSERT_EQ(numberC, view.numberC());
}

TEST_F(SimpleStructureTest, comparisonOperators)
{
    const uint8_t numberA = 0x03;
    const uint8_t numberB = 0xDE;
    const uint8_t numberC = 0x55;
    SimpleStructure data{numberA, numberB, numberC};
    SimpleStructure equalData{numberA, numberB, numberC};
    SimpleStructure lessThenData{numberA - 1, numberB, numberC};
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThenView(lessThenData);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);

    SimpleStructure anotherLessThenData{numberA, numberB, numberC - 1};
    test_utils::comparisonOperatorsTest(data, equalData, anotherLessThenData);

    zserio::View anotherLessThenView(anotherLessThenData);
    test_utils::comparisonOperatorsTest(view, equalView, anotherLessThenView);
}

TEST_F(SimpleStructureTest, validate)
{
    {
        // numberA
        SimpleStructure data{0x08, 0xDE, 0x55};
        zserio::View view(data);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // numberC
        SimpleStructure data{0x03, 0xDE, 0xF0};
        zserio::View view(data);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
}

TEST_F(SimpleStructureTest, bitSizeOf)
{
    const uint8_t numberA = 0x04;
    const uint8_t numberB = 0xCD;
    const uint8_t numberC = 0x57;
    SimpleStructure data(numberA, numberB, numberC);
    zserio::View view(data);
    ASSERT_EQ(SIMPLE_STRUCTURE_BIT_SIZE, zserio::detail::bitSizeOf(view, 0));
    ASSERT_EQ(SIMPLE_STRUCTURE_BIT_SIZE, zserio::detail::bitSizeOf(view, 1));
    ASSERT_EQ(SIMPLE_STRUCTURE_BIT_SIZE, zserio::detail::bitSizeOf(view, 100));
}

TEST_F(SimpleStructureTest, read)
{
    const uint8_t numberA = 0x04;
    const uint8_t numberB = 0xCD;
    const uint8_t numberC = 0x57;
    SimpleStructure data(numberA, numberB, numberC);

    test_utils::readTest(writeData, data);
}

TEST_F(SimpleStructureTest, writeRead)
{
    const uint8_t numberA = 0x04;
    const uint8_t numberB = 0xCD;
    const uint8_t numberC = 0x57;
    SimpleStructure data(numberA, numberB, numberC);

    test_utils::writeReadTest(data);
}

TEST_F(SimpleStructureTest, stdHash)
{
    const uint8_t numberA = 0x04;
    const uint8_t numberB = 0xCD;
    const uint8_t numberC = 0x57;
    SimpleStructure data{numberA, numberB, numberC};
    const size_t dataHash = 1178167; // hardcoded value to check that the hash code is stable

    SimpleStructure equalData{numberA, numberB, numberC};

    SimpleStructure diffData{numberA - 1, numberB + 1, numberC};
    const size_t diffDataHash = 1176835; // hardcoded value to check that the hash code is stable

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 1178167; // hardcoded value to check that the hash code is stable
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffHash = 1176835; // hardcoded value to check that the hash code is stable
    test_utils::hashTest(view, viewHash, equalView, diffView, diffHash);
}

} // namespace simple_structure
} // namespace structure_types
