#include <array>
#include <utility>

#include "functions/union_array/Inner.h"
#include "functions/union_array/Item.h"
#include "functions/union_array/ItemRef.h"
#include "functions/union_array/OuterArray.h"
#include "gtest/gtest.h"
#include "test_utils/ReadTest.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/BitStreamWriter.h"

namespace functions
{
namespace union_array
{

class UnionArrayTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, zserio::UInt16 pos)
    {
        writer.writeUnsignedBits32(static_cast<uint32_t>(ITEMS.size()), 16);

        for (Item item : ITEMS)
        {
            zserio::detail::write(writer, item.valueA);
            zserio::detail::write(writer, item.valueB);
        }

        const zserio::UInt8 isExplicit = (pos >= ITEMS.size()) ? 1 : 0;
        writer.writeVarSize(isExplicit != 0 ? 0 : 1); // choice tag
        if (isExplicit != 0)
        {
            zserio::detail::write(writer, EXPLICIT_ITEM.valueA);
            zserio::detail::write(writer, EXPLICIT_ITEM.valueB);
        }
        else
        {
            zserio::detail::write(writer, pos);
        }
    }

    static void fillData(Inner& inner, zserio::UInt16 pos)
    {
        OuterArray outerArray;
        outerArray.numElements = static_cast<uint16_t>(ITEMS.size());
        auto& values = outerArray.values;
        values.assign(ITEMS.begin(), ITEMS.end());
        inner.outerArray = outerArray;

        const zserio::UInt8 isExplicit = (pos >= ITEMS.size()) ? 1 : 0;
        ItemRef& itemRef = inner.ref;
        if (isExplicit != 0)
        {
            itemRef.emplace<ItemRef::ChoiceTag::CHOICE_item>(EXPLICIT_ITEM);
        }
        else
        {
            itemRef.emplace<ItemRef::ChoiceTag::CHOICE_position>(pos);
        }
    }

    static void checkUnionArrayFunction(zserio::UInt16 pos)
    {
        Inner data;
        fillData(data, pos);
        zserio::View view(data);

        const zserio::UInt8 isExplicit = (pos >= ITEMS.size()) ? 1 : 0;
        if (isExplicit != 0)
        {
            const auto& readElement = view.ref().getItem();
            ASSERT_EQ(zserio::View(EXPLICIT_ITEM), readElement);
        }
        else
        {
            const auto& readElement = view.ref().getElement();
            ASSERT_EQ(zserio::View(ITEMS.at(pos)), readElement);
        }

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, pos), data);
        test_utils::writeReadTest(data);
    }

    static const std::array<Item, 3> ITEMS;

private:
    static const Item EXPLICIT_ITEM;
};

const std::array<Item, 3> UnionArrayTest::ITEMS = {Item{12, 13}, Item{42, 18}, Item{17, 14}};

const Item UnionArrayTest::EXPLICIT_ITEM = Item{27, 29};

TEST_F(UnionArrayTest, checkUnionArrayFunctionElement0)
{
    checkUnionArrayFunction(0);
}

TEST_F(UnionArrayTest, checkUnionArrayFunctionElement1)
{
    checkUnionArrayFunction(1);
}

TEST_F(UnionArrayTest, checkUnionArrayFunctionElement2)
{
    checkUnionArrayFunction(2);
}

TEST_F(UnionArrayTest, checkUnionArrayFunctionExplicitElement)
{
    checkUnionArrayFunction(static_cast<uint16_t>(ITEMS.size()));
}

} // namespace union_array
} // namespace functions
