#include "gtest/gtest.h"
#include "test_utils/Assertions.h"
#include "with_parsing_info_code/Main.h"
#include "zserio/BitBuffer.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/SerializeUtil.h"
#include "zserio/Vector.h"

namespace with_parsing_info_code
{

using AllocatorType = Main::allocator_type;
using BitBufferType = zserio::BasicBitBuffer<zserio::RebindAlloc<AllocatorType, uint8_t>>;

class WithParsingInfoCode : public ::testing::Test
{
protected:
    void writeMain(zserio::BitStreamWriter& writer)
    {
        // itemChoice
        writer.writeUnsignedBits32(ITEM_VALUE, 32);
        writer.writeSignedBits32(EXTRA_VALUE, 8);

        // valueUnion
        writer.writeVarSize(1);
        writer.writeUnsignedBits32(UINT32_VALUE, 32);

        // simpleStruct
        writer.writeString(STRING_VALUE);
        writer.writeBool(true);
        writer.writeBool(OPTIONAL_VALUE);

        // item
        writer.writeUnsignedBits32(ITEM_VALUE, 32);

        // items, 3-elements packed array {1000, 10}, {1001, 12}, {1002, 14}
        writer.writeVarSize(3);
        writer.writeBool(true);
        writer.writeUnsignedBits32(1, 6);
        writer.writeUnsignedBits32(ITEMS_VALUE0, 32);
        writer.writeBool(true);
        writer.writeUnsignedBits32(2, 6);
        writer.writeSignedBits32(ITEMS_EXTRA_VALUE0, 8);

        writer.writeSignedBits32(1, 2);
        writer.writeSignedBits32(2, 3);

        writer.writeSignedBits32(1, 2);
        writer.writeSignedBits32(2, 3);
    }

    void checkMain(const Main& main)
    {
        // itemChoice
        auto* pitem = main.itemChoice.get_if<ItemChoice::Tag::item>();
        ASSERT_NE(nullptr, pitem);
        ASSERT_EQ(true, pitem->extraValue.has_value());
        ASSERT_EQ(ITEM_VALUE, pitem->value);
        ASSERT_EQ(EXTRA_VALUE, *pitem->extraValue);

        // valueUnion
        ASSERT_EQ(UINT32_VALUE, main.valueUnion.get<ValueUnion::Tag::value32>());

        // simpleStruct
        ASSERT_EQ(STRING_VALUE, main.simpleStruct.stringValue);
        ASSERT_EQ(OPTIONAL_VALUE, main.simpleStruct.optionalValue);

        // item
        ASSERT_EQ(false, main.item.extraValue.has_value());
        ASSERT_EQ(ITEM_VALUE, main.item.value);

        // items
        ASSERT_EQ(3, main.items.size());
        ASSERT_EQ(ITEMS_VALUE0, main.items.at(0).value);
        ASSERT_EQ(ITEMS_EXTRA_VALUE0, main.items.at(0).extraValue);
        ASSERT_EQ(ITEMS_VALUE1, main.items.at(1).value);
        ASSERT_EQ(ITEMS_EXTRA_VALUE1, main.items.at(1).extraValue);
        ASSERT_EQ(ITEMS_VALUE2, main.items.at(2).value);
        ASSERT_EQ(ITEMS_EXTRA_VALUE2, main.items.at(2).extraValue);
    }

    void checkParsingInfo(const Main& mainData)
    {
        zserio::View<Main> main(mainData);

        // main
        ASSERT_EQ(BIT_POSITION_MAIN, zserio::parsingInfo(main).getBitPosition());
        ASSERT_EQ(BIT_SIZE_MAIN, zserio::parsingInfo(main).getBitSize());

        // itemChoice
        ASSERT_EQ(BIT_POSITION_ITEM_CHOICE, zserio::parsingInfo(main.itemChoice()).getBitPosition());
        ASSERT_EQ(BIT_SIZE_ITEM_CHOICE, zserio::parsingInfo(main.itemChoice()).getBitSize());

        // item
        ASSERT_EQ(BIT_POSITION_ITEM, zserio::parsingInfo(main.itemChoice().item()).getBitPosition());
        ASSERT_EQ(BIT_SIZE_ITEM, zserio::parsingInfo(main.itemChoice().item()).getBitSize());

        // valueUnion
        ASSERT_EQ(BIT_POSITION_VALUE_UNION, zserio::parsingInfo(main.valueUnion()).getBitPosition());
        ASSERT_EQ(BIT_SIZE_VALUE_UNION, zserio::parsingInfo(main.valueUnion()).getBitSize());

        // simpleStruct
        ASSERT_EQ(BIT_POSITION_SIMPLE_STRUCT, zserio::parsingInfo(main.simpleStruct()).getBitPosition());
        ASSERT_EQ(BIT_SIZE_SIMPLE_STRUCT, zserio::parsingInfo(main.simpleStruct()).getBitSize());

        // item
        ASSERT_EQ(BIT_POSITION_MAIN_ITEM, zserio::parsingInfo(main.item()).getBitPosition());
        ASSERT_EQ(BIT_SIZE_MAIN_ITEM, zserio::parsingInfo(main.item()).getBitSize());

        // items
        ASSERT_EQ(3, main.items().size());
        ASSERT_EQ(BIT_POSITION_ITEMS0, zserio::parsingInfo(main.items().at(0)).getBitPosition());
        ASSERT_EQ(BIT_SIZE_ITEMS0, zserio::parsingInfo(main.items().at(0)).getBitSize());
        ASSERT_EQ(BIT_POSITION_ITEMS1, zserio::parsingInfo(main.items().at(1)).getBitPosition());
        ASSERT_EQ(BIT_SIZE_ITEMS1, zserio::parsingInfo(main.items().at(1)).getBitSize());
        ASSERT_EQ(BIT_POSITION_ITEMS2, zserio::parsingInfo(main.items().at(2)).getBitPosition());
        ASSERT_EQ(BIT_SIZE_ITEMS2, zserio::parsingInfo(main.items().at(2)).getBitSize());
    }

    static const uint32_t ITEM_VALUE;
    static const int8_t EXTRA_VALUE;
    static const uint32_t UINT32_VALUE;
    static const char* const STRING_VALUE;
    static const bool OPTIONAL_VALUE;
    static const uint32_t ITEMS_VALUE0;
    static const int8_t ITEMS_EXTRA_VALUE0;
    static const uint32_t ITEMS_VALUE1;
    static const int8_t ITEMS_EXTRA_VALUE1;
    static const uint32_t ITEMS_VALUE2;
    static const int8_t ITEMS_EXTRA_VALUE2;

    static const size_t BIT_SIZE_ITEM_CHOICE;
    static const size_t BIT_SIZE_ITEM;
    static const size_t BIT_SIZE_VALUE_UNION;
    static const size_t BIT_SIZE_SIMPLE_STRUCT;
    static const size_t BIT_SIZE_MAIN_ITEM;
    static const size_t BIT_SIZE_MAIN;
    static const size_t BIT_SIZE_ITEMS0;
    static const size_t BIT_SIZE_ITEMS1;
    static const size_t BIT_SIZE_ITEMS2;

    static const size_t BIT_POSITION_MAIN;
    static const size_t BIT_POSITION_ITEM_CHOICE;
    static const size_t BIT_POSITION_ITEM;
    static const size_t BIT_POSITION_VALUE_UNION;
    static const size_t BIT_POSITION_SIMPLE_STRUCT;
    static const size_t BIT_POSITION_MAIN_ITEM;
    static const size_t BIT_POSITION_ITEMS0;
    static const size_t BIT_POSITION_ITEMS1;
    static const size_t BIT_POSITION_ITEMS2;

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

const uint32_t WithParsingInfoCode::ITEM_VALUE = 0xDEADBEEF;
const int8_t WithParsingInfoCode::EXTRA_VALUE = -34;
const uint32_t WithParsingInfoCode::UINT32_VALUE = 0xCAFECAFE;
const char* const WithParsingInfoCode::STRING_VALUE = "Supercalifragilisticexpialidocious";
const bool WithParsingInfoCode::OPTIONAL_VALUE = true;
const uint32_t WithParsingInfoCode::ITEMS_VALUE0 = 1000;
const int8_t WithParsingInfoCode::ITEMS_EXTRA_VALUE0 = 10;
const uint32_t WithParsingInfoCode::ITEMS_VALUE1 = 1001;
const int8_t WithParsingInfoCode::ITEMS_EXTRA_VALUE1 = 12;
const uint32_t WithParsingInfoCode::ITEMS_VALUE2 = 1002;
const int8_t WithParsingInfoCode::ITEMS_EXTRA_VALUE2 = 14;

const size_t WithParsingInfoCode::BIT_SIZE_ITEM_CHOICE = 32 + 8;
const size_t WithParsingInfoCode::BIT_SIZE_ITEM = 32 + 8;
const size_t WithParsingInfoCode::BIT_SIZE_VALUE_UNION = 8 + 32;
const size_t WithParsingInfoCode::BIT_SIZE_SIMPLE_STRUCT =
        // string length in varsize + string + optional flag in bool + bool
        8 + 8 * sizeof("Supercalifragilisticexpialidocious") - 8 + 1 + 1;
const size_t WithParsingInfoCode::BIT_SIZE_MAIN_ITEM = 32;
const size_t WithParsingInfoCode::BIT_SIZE_ITEMS0 =
        1 + 6 + 32 + 1 + 6 + 8; // packing descriptor + value 0 + packing descriptor + extra value 0
const size_t WithParsingInfoCode::BIT_SIZE_ITEMS1 = 2 + 3; // value delta + extra value delta
const size_t WithParsingInfoCode::BIT_SIZE_ITEMS2 = 2 + 3; // value delta + extra value delta
const size_t WithParsingInfoCode::BIT_SIZE_MAIN = BIT_SIZE_ITEM_CHOICE + BIT_SIZE_VALUE_UNION +
        BIT_SIZE_SIMPLE_STRUCT + BIT_SIZE_MAIN_ITEM + 8 + BIT_SIZE_ITEMS0 + BIT_SIZE_ITEMS1 + BIT_SIZE_ITEMS2;

const size_t WithParsingInfoCode::BIT_POSITION_MAIN = 0;
const size_t WithParsingInfoCode::BIT_POSITION_ITEM_CHOICE = 0;
const size_t WithParsingInfoCode::BIT_POSITION_ITEM = 0;
const size_t WithParsingInfoCode::BIT_POSITION_VALUE_UNION = BIT_SIZE_ITEM_CHOICE;
const size_t WithParsingInfoCode::BIT_POSITION_SIMPLE_STRUCT = BIT_POSITION_VALUE_UNION + BIT_SIZE_VALUE_UNION;
const size_t WithParsingInfoCode::BIT_POSITION_MAIN_ITEM = BIT_POSITION_SIMPLE_STRUCT + BIT_SIZE_SIMPLE_STRUCT;
const size_t WithParsingInfoCode::BIT_POSITION_ITEMS0 = BIT_POSITION_MAIN_ITEM + BIT_SIZE_MAIN_ITEM + 8;
const size_t WithParsingInfoCode::BIT_POSITION_ITEMS1 = BIT_POSITION_ITEMS0 + BIT_SIZE_ITEMS0;
const size_t WithParsingInfoCode::BIT_POSITION_ITEMS2 = BIT_POSITION_ITEMS1 + BIT_SIZE_ITEMS1;

TEST_F(WithParsingInfoCode, checkParsingInfoMethods)
{
    Item item;
    zserio::parsingInfo(zserio::View<Item>(item, true));
    ItemChoice itemChoice;
    zserio::parsingInfo(zserio::View(itemChoice, true));
    ValueUnion valueUnion;
    zserio::parsingInfo(zserio::View(valueUnion));
    SimpleStruct simpleStruct;
    zserio::parsingInfo(zserio::View(simpleStruct));
    Main main;
    zserio::parsingInfo(zserio::View(main));
}

TEST_F(WithParsingInfoCode, deserialize)
{
    zserio::BitStreamWriter writer(bitBuffer);
    writeMain(writer);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    BitBufferType buffer(
            zserio::Span<const uint8_t>(writer.getWriteBuffer(), (writer.getBitPosition() + 7) / 8));
    Main main;
    zserio::deserialize<Main>(buffer, main);
    checkMain(main);
    checkParsingInfo(main);
}

TEST_F(WithParsingInfoCode, copyConstructor)
{
    zserio::BitStreamWriter writer(bitBuffer);
    writeMain(writer);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    BitBufferType buffer(
            zserio::Span<const uint8_t>(writer.getWriteBuffer(), (writer.getBitPosition() + 7) / 8));
    Main main;
    zserio::deserialize<Main>(buffer, main);
    const Main mainCopy = main;
    checkMain(mainCopy);
    checkParsingInfo(mainCopy);
}

TEST_F(WithParsingInfoCode, moveConstructor)
{
    zserio::BitStreamWriter writer(bitBuffer);
    writeMain(writer);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    BitBufferType buffer(
            zserio::Span<const uint8_t>(writer.getWriteBuffer(), (writer.getBitPosition() + 7) / 8));
    Main main;
    zserio::deserialize<Main>(buffer, main);
    const Main mainMove = std::move(main);
    checkMain(mainMove);
    checkParsingInfo(mainMove);
}

TEST_F(WithParsingInfoCode, assignmentOperator)
{
    zserio::BitStreamWriter writer(bitBuffer);
    writeMain(writer);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    BitBufferType buffer(
            zserio::Span<const uint8_t>(writer.getWriteBuffer(), (writer.getBitPosition() + 7) / 8));
    Main main;
    zserio::deserialize<Main>(buffer, main);
    Main mainAssign;
    mainAssign = main;
    checkMain(mainAssign);
    checkParsingInfo(mainAssign);
}

TEST_F(WithParsingInfoCode, moveAssignmentOperator)
{
    zserio::BitStreamWriter writer(bitBuffer);
    writeMain(writer);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    BitBufferType buffer(
            zserio::Span<const uint8_t>(writer.getWriteBuffer(), (writer.getBitPosition() + 7) / 8));
    Main main;
    zserio::deserialize<Main>(buffer, main);
    Main mainMove;
    mainMove = std::move(main);
    checkMain(mainMove);
    checkParsingInfo(mainMove);
}

} // namespace with_parsing_info_code
