#include <array>
#include <utility>

#include "functions/choice_array/Inner.h"
#include "functions/choice_array/Item.h"
#include "functions/choice_array/ItemRef.h"
#include "functions/choice_array/OuterArray.h"
#include "gtest/gtest.h"
#include "test_utils/ReadTest.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/BitStreamWriter.h"

namespace functions
{
namespace choice_array
{

class ChoiceArrayTest : public ::testing::Test
{
public:
    ChoiceArrayTest()
    {
        const std::array<uint8_t, 3> elementsA = {12, ELEMENT_A_FOR_EXTRA_VALUE, 17};
        const std::array<uint8_t, 3> elementsB = {13, 18, 14};
        for (size_t i = 0; i < m_items.size(); ++i)
        {
            m_items.at(i).valueA = elementsA.at(i);
            m_items.at(i).valueB = elementsB.at(i);
        }

        m_explicitItem.valueA = 27;
        m_explicitItem.valueB = 29;
    }

protected:
    void writeData(zserio::BitStreamWriter& writer, zserio::UInt16 pos)
    {
        writer.writeUnsignedBits32(static_cast<uint32_t>(m_items.size()), 16);

        for (Item item : m_items)
        {
            zserio::detail::write(writer, item.valueA);
            zserio::detail::write(writer, item.valueB);
        }

        const zserio::UInt8 isExplicit = (pos >= m_items.size()) ? 1 : 0;
        zserio::detail::write(writer, isExplicit);
        uint8_t elementA = 0;
        if (isExplicit != 0)
        {
            zserio::detail::write(writer, m_explicitItem.valueA);
            zserio::detail::write(writer, m_explicitItem.valueB);
            elementA = m_explicitItem.valueA;
        }
        else
        {
            zserio::detail::write(writer, pos);
            elementA = m_items.at(pos).valueA;
        }

        if (elementA == ELEMENT_A_FOR_EXTRA_VALUE)
        {
            zserio::detail::write(writer, EXTRA_VALUE);
        }
    }

    void fillData(Inner& inner, zserio::UInt16 pos)
    {
        OuterArray outerArray;
        outerArray.numElements = static_cast<uint16_t>(m_items.size());
        auto& values = outerArray.values;
        values.assign(m_items.begin(), m_items.end());
        inner.outerArray = outerArray;

        const zserio::UInt8 isExplicit = (pos >= m_items.size()) ? 1 : 0;
        inner.isExplicit = isExplicit;
        ItemRef& itemRef = inner.ref;
        zserio::UInt8 elementA = 0;
        if (isExplicit != 0)
        {
            itemRef.emplace<ItemRef::ChoiceTag::CHOICE_item>(m_explicitItem);
            elementA = m_explicitItem.valueA;
        }
        else
        {
            itemRef.emplace<ItemRef::ChoiceTag::CHOICE_pos>(pos);
            elementA = m_items.at(pos).valueA;
        }
        if (elementA == ELEMENT_A_FOR_EXTRA_VALUE)
        {
            inner.extra = EXTRA_VALUE;
        }
    }

    void checkChoiceArrayFunction(uint16_t pos)
    {
        Inner data;
        fillData(data, pos);
        zserio::View<Inner> view(data);

        const auto& readElement = view.ref().getElement();
        if (pos >= m_items.size())
        {
            ASSERT_EQ(zserio::View<Item>(m_explicitItem), readElement);
        }
        else
        {
            ASSERT_EQ(zserio::View<Item>(m_items.at(pos)), readElement);
        }

        test_utils::readTest(std::bind(&ChoiceArrayTest::writeData, this, std::placeholders::_1, pos), data);
        test_utils::writeReadTest(data);
    }

    std::array<Item, 3> m_items;

private:
    Item m_explicitItem;

    static constexpr zserio::UInt8 ELEMENT_A_FOR_EXTRA_VALUE = 20;
    static constexpr zserio::Int32 EXTRA_VALUE = 4711;
};

TEST_F(ChoiceArrayTest, checkChoiceArrayFunctionElement0)
{
    checkChoiceArrayFunction(0);
}

TEST_F(ChoiceArrayTest, checkChoiceArrayFunctionElement1)
{
    checkChoiceArrayFunction(1);
}

TEST_F(ChoiceArrayTest, checkChoiceArrayFunctionElement2)
{
    checkChoiceArrayFunction(2);
}

TEST_F(ChoiceArrayTest, checkChoiceArrayFunctionExplicitElement)
{
    checkChoiceArrayFunction(static_cast<uint16_t>(m_items.size()));
}

} // namespace choice_array
} // namespace functions
