#include <array>

#include "expressions/index_operator/Element.h"
#include "expressions/index_operator/ElementList.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/CppRuntimeException.h"

namespace expressions
{
namespace index_operator
{

class IndexOperatorTest : public ::testing::Test
{
protected:
    ElementList createData(size_t length, bool lastWrong = false)
    {
        ElementList data;
        auto& elements = data.elements;
        elements.reserve(length);
        for (size_t i = 0; i < length; ++i)
        {
            bool isEven = i % 2 + 1 == 2;
            const bool wrong = lastWrong && i + 1 == length;
            Element element;
            if (wrong ? !isEven : isEven)
            {
                element.emplace<Element::ChoiceTag::CHOICE_field8>(static_cast<uint8_t>(ELEMENTS[i]));
            }
            else
            {
                element.emplace<Element::ChoiceTag::CHOICE_field16>(static_cast<int16_t>(ELEMENTS[i]));
            }
            elements.push_back(element);
        }
        data.length = static_cast<uint16_t>(elements.size());
        return data;
    }

    static constexpr std::array<uint16_t, 4> ELEMENTS = {11, 33, 55, 77};
    static constexpr size_t LENGTH_SIZE = 16;
    static constexpr size_t FIELD8_SIZE = 8;
    static constexpr size_t FIELD16_SIZE = 16;
};

TEST_F(IndexOperatorTest, zeroLength)
{
    ElementList data = createData(0);
    zserio::View view(data);

    ASSERT_EQ(LENGTH_SIZE, zserio::detail::bitSizeOf(view));
    test_utils::writeReadTest(data);
}

TEST_F(IndexOperatorTest, oneElement)
{
    const size_t length = 1;
    ElementList data = createData(length);
    zserio::View view(data);

    ASSERT_EQ(LENGTH_SIZE + FIELD16_SIZE, zserio::detail::bitSizeOf(view));
    test_utils::writeReadTest(data);
}

TEST_F(IndexOperatorTest, oneElementWriteWrongField)
{
    const size_t length = 1;
    ElementList data = createData(length, true);
    zserio::View view(data);

    ASSERT_THROW(zserio::detail::validate(view), zserio::CppRuntimeException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::CppRuntimeException);
}

TEST_F(IndexOperatorTest, twoElements)
{
    const size_t length = 2;
    ElementList data = createData(length);
    zserio::View view(data);

    ASSERT_EQ(LENGTH_SIZE + FIELD16_SIZE + FIELD8_SIZE, zserio::detail::bitSizeOf(view));
    test_utils::writeReadTest(data);
}

TEST_F(IndexOperatorTest, twoElementsWriteWrongField)
{
    const size_t length = 2;
    ElementList data = createData(length, true);
    zserio::View view(data);

    ASSERT_THROW(zserio::detail::validate(view), zserio::CppRuntimeException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::CppRuntimeException);
}

TEST_F(IndexOperatorTest, threeElements)
{
    const size_t length = 3;
    ElementList data = createData(length);
    zserio::View view(data);

    ASSERT_EQ(LENGTH_SIZE + FIELD16_SIZE + FIELD8_SIZE + FIELD16_SIZE, zserio::detail::bitSizeOf(view));
    test_utils::writeReadTest(data);
}

TEST_F(IndexOperatorTest, threeElementsWriteWrongField)
{
    const size_t length = 3;
    ElementList data = createData(length, true);
    zserio::View view(data);

    ASSERT_THROW(zserio::detail::validate(view), zserio::CppRuntimeException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::CppRuntimeException);
}

TEST_F(IndexOperatorTest, fourElements)
{
    const size_t length = 4;
    ElementList data = createData(length);
    zserio::View view(data);

    ASSERT_EQ(LENGTH_SIZE + FIELD16_SIZE + FIELD8_SIZE + FIELD16_SIZE + FIELD8_SIZE,
            zserio::detail::bitSizeOf(view));
    test_utils::writeReadTest(data);
}

TEST_F(IndexOperatorTest, fourElementsWriteWrongField)
{
    const size_t length = 4;
    ElementList data = createData(length, true);
    zserio::View view(data);

    ASSERT_THROW(zserio::detail::validate(view), zserio::CppRuntimeException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::CppRuntimeException);
}

} // namespace index_operator
} // namespace expressions
