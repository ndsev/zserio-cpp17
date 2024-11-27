#include "gtest/gtest.h"
#include "parameterized_types/simple_param/Item.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace simple_param
{

using AllocatorType = Item::AllocatorType;

class SimpleParamDataTest : public ::testing::Test
{
protected:
    static constexpr zserio::UInt32 LOWER_VERSION = 9;
    static constexpr zserio::UInt32 HIGHER_VERSION = 10;

    static constexpr zserio::UInt16 ITEM_PARAM = 0xAA;
    static constexpr zserio::UInt32 ITEM_EXTRA_PARAM = 0xBB;
};

class SimpleParamViewTest : public SimpleParamDataTest
{
protected:
    void writeItemToByteArray(
            zserio::BitStreamWriter& writer, uint32_t version, uint16_t param, uint32_t extraParam)
    {
        writer.writeUnsignedBits32(param, 16);
        if (version >= HIGHER_VERSION)
        {
            writer.writeUnsignedBits32(extraParam, 32);
        }
    }

    void checkItemInBitStream(zserio::BitStreamReader& reader, uint32_t version, const Item& item)
    {
        ASSERT_EQ(item.param, reader.readUnsignedBits32(16));
        if (version >= HIGHER_VERSION)
        {
            ASSERT_EQ(item.extraParam, reader.readUnsignedBits32(32));
        }
        reader.setBitPosition(0);
    }

    static constexpr size_t ITEM_BIT_SIZE_WITHOUT_OPTIONAL = 16;
    static constexpr size_t ITEM_BIT_SIZE_WITH_OPTIONAL = 16 + 32;

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

TEST_F(SimpleParamDataTest, emptyConstructor)
{
    {
        Item item;
        ASSERT_EQ(0, item.param);
        ASSERT_FALSE(item.extraParam);
    }

    {
        Item item = {};
        ASSERT_EQ(0, item.param);
        ASSERT_FALSE(item.extraParam);
    }

    {
        Item item{AllocatorType()};
        ASSERT_EQ(0, item.param);
        ASSERT_FALSE(item.extraParam);
    }
}

TEST_F(SimpleParamDataTest, fieldConstructor)
{
    {
        Item item(ITEM_PARAM, std::nullopt);
        ASSERT_FALSE(item.extraParam);
    }
    {
        Item item(ITEM_PARAM, ITEM_EXTRA_PARAM);
        ASSERT_TRUE(item.extraParam);
        ASSERT_EQ(ITEM_EXTRA_PARAM, *item.extraParam);
    }

    {
        Item item({}, {});
        ASSERT_EQ(0, item.param);
        ASSERT_FALSE(item.extraParam);
    }
}

TEST_F(SimpleParamDataTest, copyConstructor)
{
    Item item;
    item.param = ITEM_PARAM;
    item.extraParam = ITEM_EXTRA_PARAM;

    const Item itemCopy(item);
    ASSERT_EQ(item.param, itemCopy.param);
    ASSERT_TRUE(itemCopy.extraParam);
    ASSERT_EQ(*item.extraParam, *itemCopy.extraParam);
}

TEST_F(SimpleParamDataTest, operatorAssignment)
{
    Item item;
    item.param = ITEM_PARAM;
    item.extraParam = ITEM_EXTRA_PARAM;

    const Item itemCopy = item;
    ASSERT_EQ(item.param, itemCopy.param);
    ASSERT_TRUE(itemCopy.extraParam);
    ASSERT_EQ(*item.extraParam, *itemCopy.extraParam);
}

TEST_F(SimpleParamDataTest, moveConstructor)
{
    Item item(ITEM_PARAM, {});

    Item movedItem(std::move(item));
    ASSERT_EQ(ITEM_PARAM, movedItem.param);
    ASSERT_FALSE(movedItem.extraParam);
}

TEST_F(SimpleParamDataTest, moveAssignmentOperator)
{
    Item item(ITEM_PARAM, ITEM_EXTRA_PARAM);

    Item movedItem;
    movedItem = std::move(item);
    ASSERT_EQ(ITEM_PARAM, movedItem.param);
    ASSERT_TRUE(movedItem.extraParam);
    ASSERT_EQ(ITEM_EXTRA_PARAM, movedItem.extraParam);
}

TEST_F(SimpleParamDataTest, operatorEquality)
{
    Item item1(ITEM_PARAM, ITEM_EXTRA_PARAM);
    Item item2(ITEM_PARAM, ITEM_EXTRA_PARAM);
    ASSERT_TRUE(item1 == item2);

    Item item3(ITEM_PARAM, {ITEM_EXTRA_PARAM + 1});
    ASSERT_FALSE(item2 == item3);
}

TEST_F(SimpleParamDataTest, operatorLessThan)
{
    Item item1(ITEM_PARAM, ITEM_EXTRA_PARAM);
    Item item2(ITEM_PARAM, ITEM_EXTRA_PARAM);
    ASSERT_FALSE(item1 < item2);
    ASSERT_FALSE(item2 < item1);

    Item item3(ITEM_PARAM, {ITEM_EXTRA_PARAM + 1});
    ASSERT_TRUE(item2 < item3);
    ASSERT_FALSE(item3 < item2);
}

TEST_F(SimpleParamDataTest, stdHash)
{
    std::hash<Item> hasher;
    Item item1(ITEM_PARAM, ITEM_EXTRA_PARAM);
    Item item2(ITEM_PARAM, ITEM_EXTRA_PARAM);
    ASSERT_EQ(hasher(item1), hasher(item2));

    Item item3(ITEM_PARAM, {ITEM_EXTRA_PARAM + 1});
    ASSERT_TRUE(hasher(item2) != hasher(item3));
}

TEST_F(SimpleParamViewTest, operatorEquality)
{
    Item item1(ITEM_PARAM, ITEM_EXTRA_PARAM);
    Item item2(ITEM_PARAM, ITEM_EXTRA_PARAM);

    zserio::View view1(item1, HIGHER_VERSION);
    zserio::View view21(item2, HIGHER_VERSION);
    ASSERT_TRUE(view1 == view21);

    zserio::View view22(item2, LOWER_VERSION);
    ASSERT_FALSE(view21 == view22);

    Item item3(ITEM_PARAM, {});
    zserio::View view3(item3, LOWER_VERSION);
    ASSERT_TRUE(view22 == view3);

    // TODO[Mi-L@]: Check exceptions?
}

TEST_F(SimpleParamViewTest, operatorLessThan)
{
    Item item1(ITEM_PARAM, ITEM_EXTRA_PARAM);
    Item item2(ITEM_PARAM, ITEM_EXTRA_PARAM);

    zserio::View view1(item1, HIGHER_VERSION);
    zserio::View view21(item2, HIGHER_VERSION);
    ASSERT_FALSE(view1 < view21);
    ASSERT_FALSE(view21 < view1);

    zserio::View view22(item2, LOWER_VERSION);
    ASSERT_TRUE(view22 < view21);
    ASSERT_FALSE(view21 < view22);

    Item item3(ITEM_PARAM, {});
    zserio::View view3(item3, LOWER_VERSION);
    ASSERT_FALSE(view22 < view3);
    ASSERT_FALSE(view3 < view22);
}

TEST_F(SimpleParamViewTest, stdHash)
{
    std::hash<zserio::View<Item>> hasher;
    Item item1(ITEM_PARAM, ITEM_EXTRA_PARAM);
    Item item2(ITEM_PARAM, ITEM_EXTRA_PARAM);

    zserio::View view1(item1, HIGHER_VERSION);
    zserio::View view21(item2, HIGHER_VERSION);
    ASSERT_EQ(hasher(view1), hasher(view21));

    zserio::View view22(item2, LOWER_VERSION);
    ASSERT_NE(hasher(view1), hasher(view22));

    Item item3(ITEM_PARAM, {});
    zserio::View view3(item3, LOWER_VERSION);
    ASSERT_EQ(hasher(view22), hasher(view3));
}

TEST_F(SimpleParamViewTest, bitSizeOf)
{
    Item item1;
    item1.param = ITEM_PARAM;
    zserio::View view1(item1, LOWER_VERSION);
    ASSERT_EQ(ITEM_BIT_SIZE_WITHOUT_OPTIONAL, zserio::detail::bitSizeOf(view1, 0));

    Item item2{ITEM_PARAM, ITEM_EXTRA_PARAM};
    ::zserio::View view2(item2, HIGHER_VERSION);
    ASSERT_EQ(ITEM_BIT_SIZE_WITH_OPTIONAL, zserio::detail::bitSizeOf(view2, 0));
}

TEST_F(SimpleParamViewTest, writeRead)
{
    const zserio::UInt32 version = HIGHER_VERSION;

    Item item;
    item.param = ITEM_PARAM;
    item.extraParam = ITEM_EXTRA_PARAM;

    zserio::View view(item, version);

    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    checkItemInBitStream(reader, version, item);

    Item readItem;
    zserio::View readView = zserio::detail::read(reader, readItem, version);
    ASSERT_EQ(item, readItem);
    ASSERT_EQ(view, readView);
}

} // namespace simple_param
} // namespace parameterized_types
