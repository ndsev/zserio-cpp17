#include "gtest/gtest.h"
#include "parameterized_types/grand_child_param/GrandChildParam.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace grand_child_param
{

using AllocatorType = GrandChildParam::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class GrandChildParamTest : public ::testing::Test
{
protected:
    static void fillItemChoiceHolder(ItemChoiceHolder& itemChoiceHolder)
    {
        itemChoiceHolder.hasItem = ITEM_CHOICE_HOLDER_HAS_ITEM;
        ItemChoice& itemChoice = itemChoiceHolder.itemChoice;
        Item item;
        item.param = ITEM_PARAM;
        item.extraParam = ITEM_EXTRA_PARAM;
        itemChoice.emplace<ItemChoice::ChoiceTag::CHOICE_item>(item);
    }

    static void fillData(GrandChildParam& data)
    {
        ItemChoiceHolder& itemChoiceHolder = data.itemChoiceHolder;
        fillItemChoiceHolder(itemChoiceHolder);

        auto& itemChoiceHolderArray = data.itemChoiceHolderArray;
        itemChoiceHolderArray.push_back(itemChoiceHolder);

        VectorType<zserio::UInt32> dummyArray(1);
        data.dummyArray = dummyArray;
    }

    static void writeItemChoiceHolder(zserio::BitStreamWriter& writer, const ItemChoiceHolder& itemChoiceHolder)
    {
        writer.writeBool(itemChoiceHolder.hasItem);

        const Item& item = itemChoiceHolder.itemChoice.get<ItemChoice::ChoiceTag::CHOICE_item>();
        writer.writeUnsignedBits32(item.param, 16);
        writer.writeUnsignedBits32(*item.extraParam, 32);
    }

    static void writeData(zserio::BitStreamWriter& writer, const GrandChildParam& data)
    {
        const ItemChoiceHolder& itemChoiceHolder = data.itemChoiceHolder;
        writeItemChoiceHolder(writer, itemChoiceHolder);

        const auto& itemChoiceHolderArray = data.itemChoiceHolderArray;
        writer.writeVarSize(static_cast<uint32_t>(itemChoiceHolderArray.size()));
        writeItemChoiceHolder(writer, itemChoiceHolderArray[0]);

        writer.writeBool(data.dummyArray.has_value());
        if (data.dummyArray)
        {
            writer.writeVarSize(static_cast<uint32_t>(data.dummyArray->size()));
            writer.writeUnsignedBits32(data.dummyArray->at(0), 32);
        }
    }

    static constexpr std::string_view BLOB_NAME = "language/parameterized_types/grand_child_param.blob";

private:
    static constexpr bool ITEM_CHOICE_HOLDER_HAS_ITEM = true;
    static constexpr uint16_t ITEM_PARAM = 0xAABB;
    static constexpr uint32_t ITEM_EXTRA_PARAM = 0x11223344;
};

TEST_F(GrandChildParamTest, writeRead)
{
    GrandChildParam data;
    fillData(data);

    test_utils::writeReadTest(data);
}

TEST_F(GrandChildParamTest, writeReadFile)
{
    GrandChildParam data;
    fillData(data);

    test_utils::writeReadFileTest(BLOB_NAME, data);
}

TEST_F(GrandChildParamTest, read)
{
    GrandChildParam data;
    fillData(data);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, data), data);
}

} // namespace grand_child_param
} // namespace parameterized_types
