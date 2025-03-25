#include "gtest/gtest.h"
#include "test_object/std_allocator/CreatorBitmask.h"
#include "test_object/std_allocator/CreatorEnum.h"
#include "test_object/std_allocator/CreatorNested.h"
#include "test_object/std_allocator/CreatorObject.h"
#include "test_object/std_allocator/CreatorUnsignedEnum.h"
#include "zserio/StringConvertUtil.h"
#include "zserio/TypeInfo.h"
#include "zserio/ZserioTreeCreator.h"

using test_object::std_allocator::CreatorBitmask;
using test_object::std_allocator::CreatorEnum;
using test_object::std_allocator::CreatorNested;
using test_object::std_allocator::CreatorObject;
using test_object::std_allocator::CreatorUnsignedEnum;

namespace zserio
{

using namespace std::literals::string_view_literals;

TEST(ZserioTreeCreator, makeAnyValue)
{
    const std::allocator<uint8_t> allocator;

    // bool
    auto any = detail::makeAnyValue(typeInfo<Bool>(), true, allocator);
    ASSERT_EQ(true, any.get<bool>());
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Bool>(), 1, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Bool>(), 1.0F, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Bool>(), "1", allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Bool>(), BitBuffer(), allocator), CppRuntimeException);

    // uint8
    any = detail::makeAnyValue(typeInfo<UInt8>(), 8, allocator);
    ASSERT_EQ(8, any.get<uint8_t>());
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt8>(), -1, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt8>(), 256, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt8>(), 1.0F, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt8>(), "1", allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt8>(), true, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt8>(), BitBuffer(), allocator), CppRuntimeException);

    // uint16
    any = detail::makeAnyValue(typeInfo<UInt16>(), 16, allocator);
    ASSERT_EQ(16, any.get<uint16_t>());
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt16>(), -1, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt16>(), 65536, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt16>(), 1.0F, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt16>(), "1", allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt16>(), true, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt16>(), BitBuffer(), allocator), CppRuntimeException);

    // uint32
    any = detail::makeAnyValue(typeInfo<UInt32>(), 32, allocator);
    ASSERT_EQ(32, any.get<uint32_t>());
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt32>(), -1, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt32>(), 4294967296, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt32>(), 1.0F, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt32>(), "1", allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt32>(), true, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt32>(), BitBuffer(), allocator), CppRuntimeException);

    // uint64
    any = detail::makeAnyValue(typeInfo<UInt64>(), 64, allocator);
    ASSERT_EQ(64, any.get<uint64_t>());
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt64>(), -1, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt64>(), 1.0F, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt64>(), "1", allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt64>(), true, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<UInt64>(), BitBuffer(), allocator), CppRuntimeException);

    // int8
    any = detail::makeAnyValue(typeInfo<Int8>(), 8, allocator);
    ASSERT_EQ(8, any.get<int8_t>());
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int8>(), -129, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int8>(), 128, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int8>(), 1.0F, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int8>(), "1", allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int8>(), true, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int8>(), BitBuffer(), allocator), CppRuntimeException);

    // int16
    any = detail::makeAnyValue(typeInfo<Int16>(), 16, allocator);
    ASSERT_EQ(16, any.get<int16_t>());
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int16>(), -32769, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int16>(), 32768, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int16>(), 1.0F, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int16>(), "1", allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int16>(), true, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int16>(), BitBuffer(), allocator), CppRuntimeException);

    // int32
    any = detail::makeAnyValue(typeInfo<Int32>(), 32, allocator);
    ASSERT_EQ(32, any.get<int32_t>());
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int32>(), -2147483649LL, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int32>(), 2147483648LL, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int32>(), 1.0F, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int32>(), "1", allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int32>(), true, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int32>(), BitBuffer(), allocator), CppRuntimeException);

    // int64
    any = detail::makeAnyValue(typeInfo<Int64>(), 64, allocator);
    ASSERT_EQ(64, any.get<int64_t>());
    ASSERT_THROW(
            detail::makeAnyValue(typeInfo<Int64>(), 9223372036854775808ULL, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int64>(), 1.0F, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int64>(), "1", allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int64>(), true, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Int64>(), BitBuffer(), allocator), CppRuntimeException);

    // float
    any = detail::makeAnyValue(typeInfo<Float32>(), 3.5F, allocator);
    ASSERT_EQ(3.5F, any.get<float>());
    any = detail::makeAnyValue(typeInfo<Float32>(), 1, allocator);
    ASSERT_EQ(1.0F, any.get<float>());
    any = detail::makeAnyValue(typeInfo<Float32>(), 1.0, allocator);
    ASSERT_EQ(1.0F, any.get<float>());
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Float32>(), "1", allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Float32>(), true, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Float32>(), BitBuffer(), allocator), CppRuntimeException);

    // double
    any = detail::makeAnyValue(typeInfo<Float64>(), 3.14, allocator);
    ASSERT_EQ(3.14, any.get<double>());
    any = detail::makeAnyValue(typeInfo<Float64>(), 1, allocator);
    ASSERT_EQ(1.0, any.get<double>());
    any = detail::makeAnyValue(typeInfo<Float64>(), 1.0F, allocator);
    ASSERT_EQ(1.0, any.get<double>());
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Float64>(), "1", allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Float64>(), true, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<Float64>(), BitBuffer(), allocator), CppRuntimeException);

    // string
    any = detail::makeAnyValue(typeInfo<String>(), "text", allocator);
    ASSERT_EQ("text", any.get<String>());
    any = detail::makeAnyValue(typeInfo<String>(), "text"sv, allocator);
    ASSERT_EQ("text", any.get<String>());
    any = detail::makeAnyValue(typeInfo<String>(), String("text"), allocator);
    ASSERT_EQ("text", any.get<String>());
    const String stringString = "text";
    any = detail::makeAnyValue(typeInfo<String>(), stringString, allocator);
    ASSERT_EQ("text", any.get<String>());
    ASSERT_THROW(detail::makeAnyValue(typeInfo<String>(), 1, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<String>(), 1.0F, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<String>(), true, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<String>(), BitBuffer(), allocator), CppRuntimeException);

    // enum
    any = detail::makeAnyValue(typeInfo<CreatorEnum>(), CreatorEnum::ONE, allocator);
    ASSERT_EQ(CreatorEnum::ONE, any.get<CreatorEnum>());
    any = detail::makeAnyValue(typeInfo<CreatorEnum>(), 0, allocator);
    ASSERT_EQ(enumToValue(CreatorEnum::ONE), any.get<int8_t>());
    any = detail::makeAnyValue(typeInfo<CreatorEnum>(), "ONE"sv, allocator);
    ASSERT_EQ(enumToValue(CreatorEnum::ONE), any.get<int8_t>());
    any = detail::makeAnyValue(typeInfo<CreatorEnum>(), "MinusOne"sv, allocator);
    ASSERT_EQ(enumToValue(CreatorEnum::MinusOne), any.get<int8_t>());
    ASSERT_THROW(
            detail::makeAnyValue(typeInfo<CreatorEnum>(), "NONEXISTING"sv, allocator), CppRuntimeException);
    ASSERT_THROW(
            detail::makeAnyValue(typeInfo<CreatorEnum>(), "{nonexisting}"sv, allocator), CppRuntimeException);
    ASSERT_THROW(
            detail::makeAnyValue(typeInfo<CreatorEnum>(), "nonexisting"sv, allocator), CppRuntimeException);
    ASSERT_THROW(
            detail::makeAnyValue(typeInfo<CreatorEnum>(), "_nonexisting"sv, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorEnum>(), "***"sv, allocator), CppRuntimeException);
    // check all string overloads!
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorEnum>(), "10 /* no match */"sv, allocator),
            CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorEnum>(), "-10 /* no match */", allocator),
            CppRuntimeException);
    ASSERT_THROW(
            detail::makeAnyValue(typeInfo<CreatorEnum>(), String("10 /* no match */", allocator), allocator),
            CppRuntimeException);
    const String enumString("10 /* no match */", allocator);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorEnum>(), enumString, allocator), CppRuntimeException);
    // out-of-range int64_t
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorEnum>(), "99999999999999999999", allocator),
            CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorEnum>(), "-99999999999999999999"sv, allocator),
            CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorEnum>(), ""sv, allocator), CppRuntimeException);

    // unsigned enum
    any = detail::makeAnyValue(typeInfo<CreatorUnsignedEnum>(), CreatorUnsignedEnum::ONE, allocator);
    ASSERT_EQ(CreatorUnsignedEnum::ONE, any.get<CreatorUnsignedEnum>());
    any = detail::makeAnyValue(typeInfo<CreatorUnsignedEnum>(), 0, allocator);
    ASSERT_EQ(enumToValue(CreatorUnsignedEnum::ONE), any.get<uint8_t>());
    any = detail::makeAnyValue(typeInfo<CreatorUnsignedEnum>(), "ONE"sv, allocator);
    ASSERT_EQ(enumToValue(CreatorUnsignedEnum::ONE), any.get<uint8_t>());
    any = detail::makeAnyValue(typeInfo<CreatorUnsignedEnum>(), "TWO"sv, allocator);
    ASSERT_EQ(enumToValue(CreatorUnsignedEnum::TWO), any.get<uint8_t>());

    // bitmask
    any = detail::makeAnyValue(
            typeInfo<CreatorBitmask>(), CreatorBitmask(CreatorBitmask::Values::READ), allocator);
    ASSERT_EQ(CreatorBitmask::Values::READ, any.get<CreatorBitmask>());
    any = detail::makeAnyValue(typeInfo<CreatorBitmask>(), 1, allocator);
    ASSERT_EQ(CreatorBitmask(CreatorBitmask::Values::READ).getValue(), any.get<uint8_t>());
    any = detail::makeAnyValue(typeInfo<CreatorBitmask>(), "READ"sv, allocator);
    ASSERT_EQ(CreatorBitmask(CreatorBitmask::Values::READ).getValue(), any.get<uint8_t>());
    any = detail::makeAnyValue(typeInfo<CreatorBitmask>(), "READ | WRITE"sv, allocator);
    ASSERT_EQ(CreatorBitmask(CreatorBitmask::Values::READ | CreatorBitmask::Values::WRITE).getValue(),
            any.get<uint8_t>());
    any = detail::makeAnyValue(typeInfo<CreatorBitmask>(), "READ|WRITE"sv, allocator);
    ASSERT_EQ(CreatorBitmask(CreatorBitmask::Values::READ | CreatorBitmask::Values::WRITE).getValue(),
            any.get<uint8_t>());
    ASSERT_THROW(
            detail::makeAnyValue(typeInfo<CreatorBitmask>(), "NONEXISTING"sv, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorBitmask>(), "READ "sv, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorBitmask>(), "READ |"sv, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorBitmask>(), "READ | NONEXISTING"sv, allocator),
            CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorBitmask>(), "READ * NONEXISTING"sv, allocator),
            CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorBitmask>(), "***"sv, allocator), CppRuntimeException);
    any = detail::makeAnyValue(typeInfo<CreatorBitmask>(), "7 /* READ | WRITE */"sv, allocator);
    ASSERT_EQ(7, any.get<uint8_t>());
    any = detail::makeAnyValue(typeInfo<CreatorBitmask>(), "7"sv, allocator);
    ASSERT_EQ(7, any.get<uint8_t>());
    // check all string overloads!
    any = detail::makeAnyValue(typeInfo<CreatorBitmask>(), "4 /* no match */"sv, allocator);
    ASSERT_EQ(4, any.get<uint8_t>());
    any = detail::makeAnyValue(typeInfo<CreatorBitmask>(), "4 /* no match */", allocator);
    ASSERT_EQ(4, any.get<uint8_t>());
    any = detail::makeAnyValue(typeInfo<CreatorBitmask>(), String("4 /* no match */", allocator), allocator);
    ASSERT_EQ(4, any.get<uint8_t>());
    const String bitmaskString("4 /* no match */", allocator);
    any = detail::makeAnyValue(typeInfo<CreatorBitmask>(), bitmaskString, allocator);
    ASSERT_EQ(4, any.get<uint8_t>());
    // out-of-range uint64_t
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorBitmask>(), "99999999999999999999"sv, allocator),
            CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorBitmask>(), "@WRONG"sv, allocator), CppRuntimeException);
    ASSERT_THROW(
            detail::makeAnyValue(typeInfo<CreatorBitmask>(), "_UNKNOWN"sv, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorBitmask>(), "unknown"sv, allocator), CppRuntimeException);
    ASSERT_THROW(
            detail::makeAnyValue(typeInfo<CreatorBitmask>(), "{unknown}"sv, allocator), CppRuntimeException);
    ASSERT_THROW(detail::makeAnyValue(typeInfo<CreatorBitmask>(), ""sv, allocator), CppRuntimeException);
}

TEST(ZserioTreeCreatorTest, parseBitmaskStringValue)
{
    static const ::std::array<ItemInfo, 3> values = {ItemInfo{"READ"sv, static_cast<uint64_t>(1), false, false},
            ItemInfo{"READ_EXT"sv, static_cast<uint64_t>(2), false, false},
            ItemInfo{"READ_EXT_EXT"sv, static_cast<uint64_t>(4), false, false}};

    static const detail::BitmaskTypeInfo<std::allocator<uint8_t>> bitmaskTypeInfo = {
            "Bitmask"sv, typeInfo<UInt8>(), values};

    const std::allocator<uint8_t> allocator;
    ASSERT_EQ(1, detail::parseBitmaskStringValue("READ"sv, bitmaskTypeInfo, allocator).get<uint8_t>());
    ASSERT_EQ(2, detail::parseBitmaskStringValue("READ_EXT"sv, bitmaskTypeInfo, allocator).get<uint8_t>());
    ASSERT_EQ(
            3, detail::parseBitmaskStringValue("READ_EXT | READ"sv, bitmaskTypeInfo, allocator).get<uint8_t>());
    ASSERT_EQ(6,
            detail::parseBitmaskStringValue("READ_EXT_EXT | READ_EXT"sv, bitmaskTypeInfo, allocator)
                    .get<uint8_t>());
    ASSERT_EQ(4,
            detail::parseBitmaskStringValue("READ_EXT_EXT | READ_EXT_EXT"sv, bitmaskTypeInfo, allocator)
                    .get<uint8_t>());
    ASSERT_EQ(7,
            detail::parseBitmaskStringValue("READ | READ_EXT | READ_EXT_EXT"sv, bitmaskTypeInfo, allocator)
                    .get<uint8_t>());
    ASSERT_FALSE(detail::parseBitmaskStringValue("READ|"sv, bitmaskTypeInfo, allocator).hasValue());
    ASSERT_FALSE(detail::parseBitmaskStringValue("READ | "sv, bitmaskTypeInfo, allocator).hasValue());
    ASSERT_FALSE(detail::parseBitmaskStringValue("READ|"sv, bitmaskTypeInfo, allocator).hasValue());
    ASSERT_FALSE(detail::parseBitmaskStringValue("READ_EXT | "sv, bitmaskTypeInfo, allocator).hasValue());
    ASSERT_FALSE(detail::parseBitmaskStringValue("READ_EXTABC"sv, bitmaskTypeInfo, allocator).hasValue());
}

TEST(ZserioTreeCreatorTest, createObject)
{
    ZserioTreeCreator creator(typeInfo<CreatorObject>());
    creator.beginRoot();
    IReflectableDataPtr reflectable = creator.endRoot();
    ASSERT_TRUE(reflectable);
    ASSERT_EQ(CppType::STRUCT, reflectable->getTypeInfo().getCppType());
}

TEST(ZserioTreeCreatorTest, createObjectSetFields)
{
    ZserioTreeCreator creator(typeInfo<CreatorObject>());
    creator.beginRoot();
    creator.setValue("value", 13);
    creator.setValue("text", "test");
    IReflectableDataPtr reflectable = creator.endRoot();
    ASSERT_TRUE(reflectable);

    ASSERT_EQ(13, reflectable->getField("value")->getUInt32());
    ASSERT_EQ("test"sv, reflectable->getField("text")->getStringView());
}

TEST(ZserioTreeCreatorTest, createObjectResetFields)
{
    ZserioTreeCreator creator(typeInfo<CreatorObject>());
    creator.beginRoot();
    creator.setValue("value", 13);
    creator.setValue("text", nullptr);
    creator.setValue("text", "test");
    IReflectableDataPtr reflectable = creator.endRoot();
    ASSERT_TRUE(reflectable);

    ASSERT_EQ(13, reflectable->getField("value")->getUInt32());
    ASSERT_EQ("test"sv, reflectable->getField("text")->getStringView());
}

TEST(ZserioTreeCreatorTest, createObjectFull)
{
    ZserioTreeCreator creator(typeInfo<CreatorObject>());
    creator.beginRoot();
    creator.setValue("value", 13);
    creator.setValue("text", String("test"));
    creator.beginCompound("nested");
    creator.setValue("value", 10);
    creator.setValue("text", "nested"sv);
    creator.setValue("externData", BitBuffer({0x3C}, 6));
    creator.setValue("bytesData", Vector<uint8_t>({0xFF}));
    creator.setValue("creatorEnum", CreatorEnum::ONE);
    creator.setValue("creatorBitmask", CreatorBitmask(CreatorBitmask::Values::WRITE));
    creator.endCompound();
    creator.beginArray("nestedArray");
    creator.beginCompoundElement();
    creator.setValue("value", 5);
    const String nestedArrayText = "nestedArray";
    creator.setValue("text", nestedArrayText);
    creator.setValue("creatorEnum", "MinusOne");
    creator.setValue("creatorBitmask", CreatorBitmask(CreatorBitmask::Values::READ));
    creator.endCompoundElement();
    creator.endArray();
    creator.beginArray("textArray");
    creator.addValueElement("this");
    creator.addValueElement("is");
    creator.addValueElement("text"sv);
    creator.addValueElement("array");
    creator.endArray();
    creator.beginArray("externArray");
    creator.addValueElement(BitBuffer({0x0F}, 4));
    creator.endArray();
    creator.beginArray("bytesArray");
    creator.addValueElement(Vector<uint8_t>({0xCA, 0xFE}));
    creator.endArray();
    creator.setValue("optionalBool", false);
    creator.beginCompound("optionalNested");
    creator.setValue("text", "optionalNested");
    creator.endCompound();
    IReflectableDataPtr reflectable = creator.endRoot();
    ASSERT_TRUE(reflectable);

    ASSERT_EQ(13, reflectable->getField("value")->getUInt32());
    ASSERT_EQ("test"sv, reflectable->getField("text")->getStringView());
    ASSERT_EQ(10, reflectable->find("nested.value")->getUInt32());
    ASSERT_EQ("nested"sv, reflectable->find("nested.text")->getStringView());
    ASSERT_EQ(0x3C, reflectable->find("nested.externData")->getBitBuffer().getData()[0]);
    ASSERT_EQ(6, reflectable->find("nested.externData")->getBitBuffer().getBitSize());
    ASSERT_EQ(1, reflectable->find("nested.bytesData")->getBytes().size());
    ASSERT_EQ(0xFF, reflectable->find("nested.bytesData")->getBytes()[0]);
    ASSERT_EQ(enumToValue(CreatorEnum::ONE), reflectable->find("nested.creatorEnum")->getInt8());
    ASSERT_EQ(CreatorBitmask::Values::WRITE,
            CreatorBitmask(reflectable->find("nested.creatorBitmask")->getUInt8()));
    ASSERT_EQ(1, reflectable->getField("nestedArray")->size());
    ASSERT_EQ(5, reflectable->getField("nestedArray")->at(0)->getField("value")->getUInt32());
    ASSERT_EQ("nestedArray"sv, reflectable->getField("nestedArray")->at(0)->getField("text")->getStringView());
    ASSERT_EQ(enumToValue(CreatorEnum::MinusOne),
            reflectable->getField("nestedArray")->at(0)->getField("creatorEnum")->getInt8());
    ASSERT_EQ(CreatorBitmask::Values::READ,
            CreatorBitmask(
                    reflectable->getField("nestedArray")->at(0)->getField("creatorBitmask")->getUInt8()));
    ASSERT_EQ(4, reflectable->getField("textArray")->size());
    ASSERT_EQ("this"sv, reflectable->getField("textArray")->at(0)->getStringView());
    ASSERT_EQ("is"sv, reflectable->getField("textArray")->at(1)->getStringView());
    ASSERT_EQ("text"sv, reflectable->getField("textArray")->at(2)->getStringView());
    ASSERT_EQ("array"sv, reflectable->getField("textArray")->at(3)->getStringView());
    ASSERT_EQ(1, reflectable->getField("externArray")->size());
    ASSERT_EQ(0x0f, reflectable->getField("externArray")->at(0)->getBitBuffer().getData()[0]);
    ASSERT_EQ(4, reflectable->getField("externArray")->at(0)->getBitBuffer().getBitSize());
    ASSERT_EQ(1, reflectable->getField("bytesArray")->size());
    ASSERT_EQ(2, reflectable->getField("bytesArray")->at(0)->getBytes().size());
    ASSERT_EQ(0xCA, reflectable->getField("bytesArray")->at(0)->getBytes()[0]);
    ASSERT_EQ(0xFE, reflectable->getField("bytesArray")->at(0)->getBytes()[1]);
    ASSERT_EQ(false, reflectable->getField("optionalBool")->getBool());
    ASSERT_TRUE(reflectable->getField("optionalNested"));
    ASSERT_EQ("optionalNested"sv, reflectable->find("optionalNested.text")->getStringView());
}

TEST(ZserioTreeCreator, exceptionsBeforeRoot)
{
    ZserioTreeCreator creator(typeInfo<CreatorObject>());

    ASSERT_THROW(creator.endRoot(), CppRuntimeException);
    ASSERT_THROW(creator.beginArray("nestedArray"), CppRuntimeException);
    ASSERT_THROW(creator.endArray(), CppRuntimeException);
    ASSERT_THROW(creator.getFieldType("nested"), CppRuntimeException);
    ASSERT_THROW(creator.beginCompound("nested"), CppRuntimeException);
    ASSERT_THROW(creator.endCompound(), CppRuntimeException);
    ASSERT_THROW(creator.setValue("value", 13), CppRuntimeException);
    ASSERT_THROW(creator.setValue("nonexistent", nullptr), CppRuntimeException);
    ASSERT_THROW(creator.getElementType(), CppRuntimeException);
    ASSERT_THROW(creator.beginCompoundElement(), CppRuntimeException);
    ASSERT_THROW(creator.endCompoundElement(), CppRuntimeException);
    ASSERT_THROW(creator.addValueElement(13), CppRuntimeException);
}

TEST(ZserioTreeCreator, exceptionsInRoot)
{
    ZserioTreeCreator creator(typeInfo<CreatorObject>());
    creator.beginRoot();

    ASSERT_THROW(creator.beginRoot(), CppRuntimeException);
    ASSERT_THROW(creator.beginArray("nonexistent"), CppRuntimeException);
    ASSERT_THROW(creator.beginArray("nested"), CppRuntimeException); // not an array
    ASSERT_THROW(creator.endArray(), CppRuntimeException);
    ASSERT_THROW(creator.beginCompound("nonexistent"), CppRuntimeException);
    ASSERT_THROW(creator.beginCompound("nestedArray"), CppRuntimeException); // is array
    ASSERT_THROW(creator.endCompound(), CppRuntimeException);
    ASSERT_THROW(creator.setValue("nonexistent", 13), CppRuntimeException);
    ASSERT_THROW(creator.setValue("nestedArray", 13), CppRuntimeException); // is array
    ASSERT_THROW(creator.beginCompoundElement(), CppRuntimeException);
    ASSERT_THROW(creator.endCompoundElement(), CppRuntimeException);
    ASSERT_THROW(creator.addValueElement(13), CppRuntimeException);
}

TEST(ZserioTreeCreator, exceptionsInCompound)
{
    ZserioTreeCreator creator(typeInfo<CreatorObject>());
    creator.beginRoot();
    creator.beginCompound("nested");

    ASSERT_THROW(creator.beginRoot(), CppRuntimeException);
    ASSERT_THROW(creator.endRoot(), CppRuntimeException);
    ASSERT_THROW(creator.beginArray("nonexistent"), CppRuntimeException);
    ASSERT_THROW(creator.beginArray("value"), CppRuntimeException); // not an array
    ASSERT_THROW(creator.endArray(), CppRuntimeException);
    ASSERT_THROW(creator.beginCompound("nonexistent"), CppRuntimeException);
    ASSERT_THROW(creator.beginCompound("text"), CppRuntimeException); // not a compound
    ASSERT_THROW(creator.setValue("nonexistent", "test"), CppRuntimeException);
    ASSERT_THROW(creator.setValue("value", "test"), CppRuntimeException); // wrong type
    ASSERT_THROW(creator.beginCompoundElement(), CppRuntimeException);
    ASSERT_THROW(creator.endCompoundElement(), CppRuntimeException);
    ASSERT_THROW(creator.addValueElement(13), CppRuntimeException);
}

TEST(ZserioTreeCreator, exceptionsInCompoundArray)
{
    ZserioTreeCreator creator(typeInfo<CreatorObject>());
    creator.beginRoot();
    creator.beginArray("nestedArray");

    ASSERT_THROW(creator.beginRoot(), CppRuntimeException);
    ASSERT_THROW(creator.endRoot(), CppRuntimeException);
    ASSERT_THROW(creator.beginArray("nonexistent"), CppRuntimeException);
    ASSERT_THROW(creator.beginCompound("nonexistent"), CppRuntimeException);
    ASSERT_THROW(creator.endCompound(), CppRuntimeException);
    ASSERT_THROW(creator.setValue("nonexistent", 13), CppRuntimeException);
    ASSERT_THROW(creator.endCompoundElement(), CppRuntimeException);
    ASSERT_THROW(creator.addValueElement(13), CppRuntimeException);
}

TEST(ZserioTreeCreator, exceptionsInSimpleArray)
{
    ZserioTreeCreator creator(typeInfo<CreatorObject>());
    creator.beginRoot();
    creator.beginArray("textArray");

    ASSERT_THROW(creator.beginRoot(), CppRuntimeException);
    ASSERT_THROW(creator.endRoot(), CppRuntimeException);
    ASSERT_THROW(creator.beginArray("nonexistent"), CppRuntimeException);
    ASSERT_THROW(creator.beginCompound("nonexistent"), CppRuntimeException);
    ASSERT_THROW(creator.endCompound(), CppRuntimeException);
    ASSERT_THROW(creator.setValue("nonexistent", 13), CppRuntimeException);
    ASSERT_THROW(creator.beginCompoundElement(), CppRuntimeException); // not a compound array
    ASSERT_THROW(creator.endCompoundElement(), CppRuntimeException);
    ASSERT_THROW(creator.addValueElement(13), CppRuntimeException); // wrong type
}

TEST(ZserioTreeCreator, exceptionsInCompoundElement)
{
    ZserioTreeCreator creator(typeInfo<CreatorObject>());
    creator.beginRoot();
    creator.beginArray("nestedArray");
    creator.beginCompoundElement();

    ASSERT_THROW(creator.beginRoot(), CppRuntimeException);
    ASSERT_THROW(creator.endRoot(), CppRuntimeException);
    ASSERT_THROW(creator.beginArray("nonexistent"), CppRuntimeException);
    ASSERT_THROW(creator.endArray(), CppRuntimeException);
    ASSERT_THROW(creator.beginCompound("nonexistent"), CppRuntimeException);
    ASSERT_THROW(creator.endCompound(), CppRuntimeException);
    ASSERT_THROW(creator.setValue("nonexistent", 13), CppRuntimeException);
    ASSERT_THROW(creator.beginCompoundElement(), CppRuntimeException);
    ASSERT_THROW(creator.addValueElement(13), CppRuntimeException);
}

} // namespace zserio
