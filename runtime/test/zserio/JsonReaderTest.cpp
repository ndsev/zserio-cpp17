#include "gtest/gtest.h"
#include "test_object/std_allocator/CreatorBitmask.h"
#include "test_object/std_allocator/CreatorEnum.h"
#include "test_object/std_allocator/CreatorNested.h"
#include "test_object/std_allocator/CreatorObject.h"
#include "zserio/JsonReader.h"
#include "zserio/ReflectableUtil.h"

using test_object::std_allocator::CreatorBitmask;
using test_object::std_allocator::CreatorEnum;
using test_object::std_allocator::CreatorNested;
using test_object::std_allocator::CreatorObject;

namespace zserio
{

namespace
{

void checkReadStringifiedEnum(const char* stringValue, CreatorEnum expectedValue)
{
    std::stringstream str;
    str << "{\n"
           "    \"nested\": {\n"
           "        \"creatorEnum\": \""
        << stringValue
        << "\"\n"
           "    }\n"
           "}";

    JsonReader jsonReader(str);
    auto reflectable = jsonReader.read(typeInfo<CreatorObject>());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ(expectedValue, ReflectableUtil::getValue<CreatorEnum>(reflectable->find("nested.creatorEnum")));
}

void checkReadStringifiedEnumThrows(const char* stringValue, const char* expectedMessage)
{
    std::stringstream str;
    str << "{\n"
           "    \"nested\": {\n"
           "        \"creatorEnum\": \""
        << stringValue
        << "\"\n"
           "    }\n"
           "}";

    JsonReader jsonReader(str);
    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ(expectedMessage, e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

void checkReadStringifiedBitmask(const char* stringValue, CreatorBitmask expectedValue)
{
    std::stringstream str;
    str << "{\n"
           "    \"nested\": {\n"
           "        \"creatorBitmask\": \""
        << stringValue
        << "\"\n"
           "    }\n"
           "}";

    JsonReader jsonReader(str);
    auto reflectable = jsonReader.read(typeInfo<CreatorObject>());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ(expectedValue,
            ReflectableUtil::getValue<CreatorBitmask>(reflectable->find("nested.creatorBitmask")));
}

void checkReadStringifiedBitmaskThrows(const char* stringValue, const char* expectedMessage)
{
    std::stringstream str;
    str << "{\n"
           "    \"nested\": {\n"
           "        \"creatorBitmask\": \""
        << stringValue << "\"\n"
        << "    }\n"
           "}";

    JsonReader jsonReader(str);
    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ(expectedMessage, e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

} // namespace

TEST(JsonReaderTest, readObjectWithoutOptional)
{
    std::stringstream str(
            "{\n"
            "    \"value\": 13,\n"
            "    \"nested\": {\n"
            "        \"value\": 10,\n"
            "        \"text\": \"nested\",\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 203,\n"
            "                 240\n"
            "             ],\n"
            "             \"bitSize\": 12\n"
            "        },\n"
            "        \"bytesData\": {\n"
            "           \"buffer\": [\n"
            "               202,\n"
            "               254\n"
            "           ]\n"
            "        },\n"
            "        \"creatorEnum\": 0,\n"
            "        \"creatorBitmask\": 1\n"
            "    },\n"
            "    \"text\": \"test\",\n"
            "    \"nestedArray\": [\n"
            "        {\n"
            "            \"value\": 5,\n"
            "            \"text\": \"nestedArray\",\n"
            "            \"externData\": {\n"
            "                 \"buffer\": [\n"
            "                     202,\n"
            "                     254\n"
            "                 ],"
            "                 \"bitSize\": 15\n"
            "            },\n"
            "            \"bytesData\": {\n"
            "               \"buffer\": [\n"
            "                   203,\n"
            "                   240\n"
            "               ]\n"
            "            },\n"
            "            \"creatorEnum\": 1,\n"
            "            \"creatorBitmask\": 2\n"
            "        }\n"
            "    ],\n"
            "    \"textArray\": [\n"
            "        \"this\",\n"
            "        \"is\",\n"
            "        \"text\",\n"
            "        \"array\"\n"
            "    ],\n"
            "    \"externArray\": [\n"
            "        {\n"
            "            \"buffer\": [\n"
            "                222,\n"
            "                209\n"
            "            ],"
            "            \"bitSize\": 13\n"
            "        }\n"
            "    ],\n"
            "    \"bytesArray\": [\n"
            "        {\n"
            "           \"buffer\": [\n"
            "               0\n"
            "           ]\n"
            "        }\n"
            "    ],\n"
            "    \"optionalBool\": null\n"
            "}");

    JsonReader jsonReader(str);
    IReflectableDataPtr reflectable = jsonReader.read(typeInfo<CreatorObject>());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ(13, reflectable->getField("value")->getUInt32());
    ASSERT_EQ(10, reflectable->find("nested.value")->getUInt32());
    ASSERT_EQ("nested"sv, reflectable->find("nested.text")->getStringView());
    ASSERT_EQ(BitBuffer({0xCB, 0xF0}, 12), reflectable->find("nested.externData")->getBitBuffer());
    const Span<const uint8_t> bytesData = reflectable->find("nested.bytesData")->getBytes();
    ASSERT_EQ(2, bytesData.size());
    ASSERT_EQ(0xCA, bytesData[0]);
    ASSERT_EQ(0xFE, bytesData[1]);
    ASSERT_EQ(enumToValue(CreatorEnum::ONE), reflectable->find("nested.creatorEnum")->getInt8());
    ASSERT_EQ(CreatorBitmask(CreatorBitmask::Values::READ).getValue(),
            reflectable->find("nested.creatorBitmask")->getUInt8());
    ASSERT_EQ("test"sv, reflectable->getField("text")->getStringView());
    ASSERT_EQ(1, reflectable->getField("nestedArray")->size());
    ASSERT_EQ(5, reflectable->getField("nestedArray")->at(0)->getField("value")->getUInt32());
    ASSERT_EQ("nestedArray"sv, reflectable->getField("nestedArray")->at(0)->getField("text")->getStringView());
    ASSERT_EQ(BitBuffer({0xCA, 0xFE}, 15),
            reflectable->getField("nestedArray")->at(0)->getField("externData")->getBitBuffer());
    const Span<const uint8_t> nestedBytesData =
            reflectable->getField("nestedArray")->at(0)->getField("bytesData")->getBytes();
    ASSERT_EQ(2, nestedBytesData.size());
    ASSERT_EQ(0xCB, nestedBytesData[0]);
    ASSERT_EQ(0xF0, nestedBytesData[1]);
    ASSERT_EQ(enumToValue(CreatorEnum::TWO),
            reflectable->getField("nestedArray")->at(0)->getField("creatorEnum")->getInt8());
    ASSERT_EQ(CreatorBitmask(CreatorBitmask::Values::WRITE).getValue(),
            reflectable->getField("nestedArray")->at(0)->getField("creatorBitmask")->getUInt8());
    ASSERT_EQ(4, reflectable->getField("textArray")->size());
    ASSERT_EQ("this"sv, reflectable->getField("textArray")->at(0)->getStringView());
    ASSERT_EQ("is"sv, reflectable->getField("textArray")->at(1)->getStringView());
    ASSERT_EQ("text"sv, reflectable->getField("textArray")->at(2)->getStringView());
    ASSERT_EQ("array"sv, reflectable->getField("textArray")->at(3)->getStringView());
    ASSERT_EQ(1, reflectable->getField("externArray")->size());
    ASSERT_EQ(BitBuffer({0xDE, 0xD1}, 13), reflectable->getField("externArray")->at(0)->getBitBuffer());
    ASSERT_EQ(1, reflectable->getField("bytesArray")->size());
    Span<const uint8_t> bytesElement = reflectable->getField("bytesArray")->at(0)->getBytes();
    ASSERT_EQ(1, bytesElement.size());
    ASSERT_EQ(0, bytesElement[0]);
    ASSERT_EQ(nullptr, reflectable->getField("optionalBool"));
    ASSERT_EQ(nullptr, reflectable->getField("optionalNested")); // not present in json
}

TEST(JsonReaderTest, readObjectWithOptional)
{
    std::stringstream str(
            "{\n"
            "    \"value\": 13,\n"
            "    \"nested\": {\n"
            "        \"value\": 10,\n"
            "        \"text\": \"nested\",\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 203,\n"
            "                 240\n"
            "             ],\n"
            "             \"bitSize\": 12\n"
            "        },\n"
            "        \"bytesData\": {\n"
            "           \"buffer\": [\n"
            "               202,\n"
            "               254\n"
            "           ]\n"
            "        },\n"
            "        \"creatorEnum\": 0,\n"
            "        \"creatorBitmask\": 1\n"
            "    },\n"
            "    \"text\": \"test\",\n"
            "    \"nestedArray\": [\n"
            "        {\n"
            "            \"value\": 5,\n"
            "            \"text\": \"nestedArray\",\n"
            "            \"externData\": {\n"
            "                 \"buffer\": [\n"
            "                     202,\n"
            "                     254\n"
            "                 ],"
            "                 \"bitSize\": 15\n"
            "            },\n"
            "            \"bytesData\": {\n"
            "               \"buffer\": [\n"
            "                   203,\n"
            "                   240\n"
            "               ]\n"
            "            },\n"
            "            \"creatorEnum\": 1,\n"
            "            \"creatorBitmask\": 2\n"
            "        }\n"
            "    ],\n"
            "    \"textArray\": [\n"
            "        \"this\",\n"
            "        \"is\",\n"
            "        \"text\",\n"
            "        \"array\"\n"
            "    ],\n"
            "    \"externArray\": [\n"
            "        {\n"
            "            \"buffer\": [\n"
            "                222,\n"
            "                209\n"
            "            ],"
            "            \"bitSize\": 13\n"
            "        }\n"
            "    ],\n"
            "    \"bytesArray\": [\n"
            "        {\n"
            "           \"buffer\": [\n"
            "               0\n"
            "           ]\n"
            "        }\n"
            "    ],\n"
            "    \"optionalBool\": true\n"
            "}");

    JsonReader jsonReader(str);
    IReflectableDataPtr reflectable = jsonReader.read(typeInfo<CreatorObject>());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ(13, reflectable->getField("value")->getUInt32());
    ASSERT_EQ(10, reflectable->find("nested.value")->getUInt32());
    ASSERT_EQ("nested"sv, reflectable->find("nested.text")->getStringView());
    ASSERT_EQ(BitBuffer({0xCB, 0xF0}, 12), reflectable->find("nested.externData")->getBitBuffer());
    const Span<const uint8_t> bytesData = reflectable->find("nested.bytesData")->getBytes();
    ASSERT_EQ(2, bytesData.size());
    ASSERT_EQ(0xCA, bytesData[0]);
    ASSERT_EQ(0xFE, bytesData[1]);
    ASSERT_EQ(enumToValue(CreatorEnum::ONE), reflectable->find("nested.creatorEnum")->getInt8());
    ASSERT_EQ(CreatorBitmask(CreatorBitmask::Values::READ).getValue(),
            reflectable->find("nested.creatorBitmask")->getUInt8());
    ASSERT_EQ("test"sv, reflectable->getField("text")->getStringView());
    ASSERT_EQ(1, reflectable->getField("nestedArray")->size());
    ASSERT_EQ(5, reflectable->getField("nestedArray")->at(0)->getField("value")->getUInt32());
    ASSERT_EQ("nestedArray"sv, reflectable->getField("nestedArray")->at(0)->getField("text")->getStringView());
    ASSERT_EQ(BitBuffer({0xCA, 0xFE}, 15),
            reflectable->getField("nestedArray")->at(0)->getField("externData")->getBitBuffer());
    const Span<const uint8_t> nestedBytesData =
            reflectable->getField("nestedArray")->at(0)->getField("bytesData")->getBytes();
    ASSERT_EQ(2, nestedBytesData.size());
    ASSERT_EQ(0xCB, nestedBytesData[0]);
    ASSERT_EQ(0xF0, nestedBytesData[1]);
    ASSERT_EQ(enumToValue(CreatorEnum::TWO),
            reflectable->getField("nestedArray")->at(0)->getField("creatorEnum")->getInt8());
    ASSERT_EQ(CreatorBitmask(CreatorBitmask::Values::WRITE).getValue(),
            reflectable->getField("nestedArray")->at(0)->getField("creatorBitmask")->getUInt8());
    ASSERT_EQ(4, reflectable->getField("textArray")->size());
    ASSERT_EQ("this"sv, reflectable->getField("textArray")->at(0)->getStringView());
    ASSERT_EQ("is"sv, reflectable->getField("textArray")->at(1)->getStringView());
    ASSERT_EQ("text"sv, reflectable->getField("textArray")->at(2)->getStringView());
    ASSERT_EQ("array"sv, reflectable->getField("textArray")->at(3)->getStringView());
    ASSERT_EQ(1, reflectable->getField("externArray")->size());
    ASSERT_EQ(BitBuffer({0xDE, 0xD1}, 13), reflectable->getField("externArray")->at(0)->getBitBuffer());
    ASSERT_EQ(1, reflectable->getField("bytesArray")->size());
    Span<const uint8_t> bytesElement = reflectable->getField("bytesArray")->at(0)->getBytes();
    ASSERT_EQ(1, bytesElement.size());
    ASSERT_EQ(0, bytesElement[0]);
    ASSERT_EQ(true, reflectable->getField("optionalBool")->getBool());
    ASSERT_EQ(nullptr, reflectable->getField("optionalNested")); // not present in json
}

TEST(JsonReaderTest, readTwoObjects)
{
    std::stringstream str(
            "{\"value\": 13}\n"
            "{\"value\": 42, \"text\": \"test\"}\n");

    JsonReader jsonReader(str);

    auto obj1 = jsonReader.read(typeInfo<CreatorObject>());
    ASSERT_TRUE(obj1);
    ASSERT_EQ(13, obj1->getField("value")->getUInt32());
    ASSERT_EQ(""sv, obj1->getField("text")->getStringView());

    auto obj2 = jsonReader.read(typeInfo<CreatorObject>());
    ASSERT_TRUE(obj2);
    ASSERT_EQ(42, obj2->getField("value")->getUInt32());
    ASSERT_EQ("test"sv, obj2->getField("text")->getStringView());
}

TEST(JsonReaderTest, readParameterizedObject)
{
    std::stringstream str(
            "{\n"
            "    \"value\": 10,\n"
            "    \"text\": \"nested\",\n"
            "    \"externData\": {\n"
            "         \"bitSize\": 12,\n"
            "         \"buffer\": [\n"
            "             203,\n"
            "             240\n"
            "         ]\n"
            "    },\n"
            "    \"bytesData\": {\n"
            "       \"buffer\": [\n"
            "           202,\n"
            "           254\n"
            "       ]\n"
            "    },\n"
            "    \"creatorEnum\": 0,\n"
            "    \"creatorBitmask\": 1\n"
            "}\n");

    JsonReader jsonReader(str);
    auto reflectable = jsonReader.read(typeInfo<CreatorNested>());
    ASSERT_EQ(10, reflectable->getField("value")->getUInt32());
}

TEST(JsonReaderTest, readUnorderedBitBuffer)
{
    std::stringstream str(
            "{\n"
            "    \"value\": 13,\n"
            "    \"nested\": {\n"
            "        \"value\": 10,\n"
            "        \"text\": \"nested\",\n"
            "        \"externData\": {\n"
            "             \"bitSize\": 12,\n"
            "             \"buffer\": [\n"
            "                 203,\n"
            "                 240\n"
            "             ]\n"
            "        },\n"
            "        \"bytesData\": {\n"
            "           \"buffer\": [\n"
            "               202,\n"
            "               254\n"
            "           ]\n"
            "        },\n"
            "        \"creatorEnum\": -1,\n"
            "        \"creatorBitmask\": 1\n"
            "    }\n"
            "}");

    JsonReader jsonReader(str);
    auto reflectable = jsonReader.read(typeInfo<CreatorObject>());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ(13, reflectable->getField("value")->getUInt32());
    ASSERT_EQ(10, reflectable->find("nested.value")->getUInt32());
    ASSERT_EQ("nested"sv, reflectable->find("nested.text")->getStringView());
    ASSERT_EQ(BitBuffer({0xCB, 0xF0}, 12), reflectable->find("nested.externData")->getBitBuffer());
    const Span<const uint8_t> bytesData = reflectable->find("nested.bytesData")->getBytes();
    ASSERT_EQ(2, bytesData.size());
    ASSERT_EQ(0xCA, bytesData[0]);
    ASSERT_EQ(0xFE, bytesData[1]);
    ASSERT_EQ(enumToValue(CreatorEnum::MinusOne), reflectable->find("nested.creatorEnum")->getInt8());
    ASSERT_EQ(CreatorBitmask(CreatorBitmask::Values::READ).getValue(),
            reflectable->find("nested.creatorBitmask")->getUInt8());
}

TEST(JsonReaderTest, readEmptyBitBuffer)
{
    std::stringstream str(
            "{\n"
            "    \"value\": 13,\n"
            "    \"nested\": {\n"
            "        \"value\": 10,\n"
            "        \"text\": \"nested\",\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "             ],\n"
            "             \"bitSize\": 0\n"
            "        },\n"
            "        \"bytesData\": {\n"
            "           \"buffer\": [\n"
            "               202,\n"
            "               254\n"
            "           ]\n"
            "        },\n"
            "        \"creatorEnum\": -1,\n"
            "        \"creatorBitmask\": 1\n"
            "    }\n"
            "}");

    JsonReader jsonReader(str);
    auto reflectable = jsonReader.read(typeInfo<CreatorObject>());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ(13, reflectable->getField("value")->getUInt32());
    ASSERT_EQ(10, reflectable->find("nested.value")->getUInt32());
    ASSERT_EQ("nested"sv, reflectable->find("nested.text")->getStringView());
    ASSERT_EQ(BitBuffer(), reflectable->find("nested.externData")->getBitBuffer());
    const Span<const uint8_t> bytesData = reflectable->find("nested.bytesData")->getBytes();
    ASSERT_EQ(2, bytesData.size());
    ASSERT_EQ(0xCA, bytesData[0]);
    ASSERT_EQ(0xFE, bytesData[1]);
    ASSERT_EQ(enumToValue(CreatorEnum::MinusOne), reflectable->find("nested.creatorEnum")->getInt8());
    ASSERT_EQ(CreatorBitmask(CreatorBitmask::Values::READ).getValue(),
            reflectable->find("nested.creatorBitmask")->getUInt8());
}

TEST(JsonReaderTest, readEmptyBytes)
{
    std::stringstream str(
            "{\n"
            "    \"value\": 13,\n"
            "    \"nested\": {\n"
            "        \"value\": 10,\n"
            "        \"text\": \"nested\",\n"
            "        \"externData\": {\n"
            "             \"bitSize\": 0,\n"
            "             \"buffer\": [\n"
            "             ]\n"
            "        },\n"
            "        \"bytesData\": {\n"
            "           \"buffer\": [\n"
            "           ]\n"
            "        },\n"
            "        \"creatorEnum\": -1,\n"
            "        \"creatorBitmask\": 1\n"
            "    }\n"
            "}");

    JsonReader jsonReader(str);
    auto reflectable = jsonReader.read(typeInfo<CreatorObject>());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ(13, reflectable->getField("value")->getUInt32());
    ASSERT_EQ(10, reflectable->find("nested.value")->getUInt32());
    ASSERT_EQ("nested"sv, reflectable->find("nested.text")->getStringView());
    ASSERT_EQ(BitBuffer(), reflectable->find("nested.externData")->getBitBuffer());
    const Span<const uint8_t> bytesData = reflectable->find("nested.bytesData")->getBytes();
    ASSERT_EQ(0, bytesData.size());
    ASSERT_EQ(enumToValue(CreatorEnum::MinusOne), reflectable->find("nested.creatorEnum")->getInt8());
    ASSERT_EQ(CreatorBitmask(CreatorBitmask::Values::READ).getValue(),
            reflectable->find("nested.creatorBitmask")->getUInt8());
}

TEST(JsonReaderTest, readStringifiedEnum)
{
    checkReadStringifiedEnum("ONE", CreatorEnum::ONE);
    checkReadStringifiedEnum("MinusOne", CreatorEnum::MinusOne);
    checkReadStringifiedEnumThrows("NONEXISTING",
            "ZserioTreeCreator: Cannot create enum 'test_object.std_allocator.CreatorEnum' "
            "from string value 'NONEXISTING'! (JsonParser:3:24)");
    checkReadStringifiedEnumThrows("***",
            "ZserioTreeCreator: Cannot create enum 'test_object.std_allocator.CreatorEnum' "
            "from string value '***'! (JsonParser:3:24)");
    checkReadStringifiedEnumThrows("10 /* no match */",
            "ZserioTreeCreator: Cannot create enum 'test_object.std_allocator.CreatorEnum' "
            "from string value '10 /* no match */'! (JsonParser:3:24)");
    checkReadStringifiedEnumThrows("-10 /* no match */",
            "ZserioTreeCreator: Cannot create enum 'test_object.std_allocator.CreatorEnum' "
            "from string value '-10 /* no match */'! (JsonParser:3:24)");
    checkReadStringifiedEnumThrows("",
            "ZserioTreeCreator: Cannot create enum 'test_object.std_allocator.CreatorEnum' "
            "from string value ''! (JsonParser:3:24)");
}

TEST(JsonReaderTest, readStringifiedBitmask)
{
    checkReadStringifiedBitmask("READ", CreatorBitmask::Values::READ);
    checkReadStringifiedBitmask("READ | WRITE", CreatorBitmask::Values::READ | CreatorBitmask::Values::WRITE);
    checkReadStringifiedBitmaskThrows("NONEXISTING",
            "ZserioTreeCreator: Cannot create bitmask 'test_object.std_allocator.CreatorBitmask' "
            "from string value 'NONEXISTING'! (JsonParser:3:27)");
    checkReadStringifiedBitmaskThrows("READ | NONEXISTING",
            "ZserioTreeCreator: Cannot create bitmask 'test_object.std_allocator.CreatorBitmask' "
            "from string value 'READ | NONEXISTING'! (JsonParser:3:27)");
    checkReadStringifiedBitmaskThrows("READ * NONEXISTING",
            "ZserioTreeCreator: Cannot create bitmask 'test_object.std_allocator.CreatorBitmask' "
            "from string value 'READ * NONEXISTING'! (JsonParser:3:27)");
    checkReadStringifiedBitmask("7 /* READ | WRITE */", CreatorBitmask(7));
    checkReadStringifiedBitmask("15 /* READ | WRITE */", CreatorBitmask(15));
    checkReadStringifiedBitmask("4 /* no match */", CreatorBitmask(4));
    checkReadStringifiedBitmaskThrows("",
            "ZserioTreeCreator: Cannot create bitmask 'test_object.std_allocator.CreatorBitmask' "
            "from string value ''! (JsonParser:3:27)");
    checkReadStringifiedBitmaskThrows(" ",
            "ZserioTreeCreator: Cannot create bitmask 'test_object.std_allocator.CreatorBitmask' "
            "from string value ' '! (JsonParser:3:27)");
    checkReadStringifiedBitmaskThrows(" | ",
            "ZserioTreeCreator: Cannot create bitmask 'test_object.std_allocator.CreatorBitmask' "
            "from string value ' | '! (JsonParser:3:27)");
}

TEST(JsonReaderTest, jsonParserException)
{
    std::stringstream str("{\"value\"\n\"value\"");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const JsonParserException& e)
                {
                    ASSERT_STREQ("JsonParser:2:1: unexpected token: VALUE, expecting KEY_SEPARATOR!", e.what());
                    throw;
                }
            },
            JsonParserException);
}

TEST(JsonReaderTest, wrongKeyException)
{
    std::stringstream str("{\"value\": 13,\n\"nonexisting\": 10}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("ZserioTreeCreator: Member 'nonexisting' not found in "
                                 "'test_object.std_allocator.CreatorObject'! (JsonParser:2:16)",
                            e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, wrongValueTypeException)
{
    std::stringstream str("{\n  \"value\": \"13\"\n}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("ZserioTreeCreator: Value '13' cannot be converted to integral value! "
                                 "(JsonParser:2:12)",
                            e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, wrongBitBufferException)
{
    std::stringstream str(
            "{\n"
            "    \"value\": 13,\n"
            "    \"nested\": {\n"
            "        \"value\": 10,\n"
            "        \"text\": \"nested\",\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 203,\n"
            "                 240\n"
            "             ],\n"
            "             \"bitSize\": {\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ(
                            "JsonReader: Unexpected beginObject in BitBuffer! (JsonParser:12:14)", e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, partialBitBufferException)
{
    std::stringstream str(
            "{\n"
            "    \"value\": 13,\n"
            "    \"nested\": {\n"
            "        \"value\": 10,\n"
            "        \"text\": \"nested\",\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 203,\n"
            "                 240\n"
            "             ]\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("JsonReader: Unexpected end in BitBuffer! (JsonParser:12:5)", e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, bigBitBufferByteValueException)
{
    std::stringstream str(
            "{\n"
            "    \"nested\": {\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 256\n"
            "             ],\n"
            "             \"bitSize\": 7\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ(
                            "JsonReader: Cannot create byte for Bit Buffer from value '256'! (JsonParser:5:18)",
                            e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, negativeBitBufferByteValueException)
{
    std::stringstream str(
            "{\n"
            "    \"nested\": {\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 -1\n"
            "             ],\n"
            "             \"bitSize\": 7\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("JsonReader: Unexpected visitValue (int) in BitBuffer! (JsonParser:5:18)",
                            e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, wrongBitBufferSizeValueException)
{
    std::stringstream str(
            "{\n"
            "    \"nested\": {\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 255\n"
            "             ],\n"
            "             \"bitSize\": 18446744073709551616\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("JsonTokenizer:7:25: Value is outside of the 64-bit integer range!", e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, wrongBitBufferNullPtrValueException)
{
    std::stringstream str(
            "{\n"
            "    \"nested\": {\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 255\n"
            "             ],\n"
            "             \"bitSize\": null\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("JsonReader: Unexpected visitValue (null) in BitBuffer! (JsonParser:7:25)",
                            e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, wrongBitBufferBoolValueException)
{
    std::stringstream str(
            "{\n"
            "    \"nested\": {\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 255\n"
            "             ],\n"
            "             \"bitSize\": true\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("JsonReader: Unexpected visitValue (bool) in BitBuffer! (JsonParser:7:25)",
                            e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, wrongBitBufferDoubleValueException)
{
    std::stringstream str(
            "{\n"
            "    \"nested\": {\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 255\n"
            "             ],\n"
            "             \"bitSize\": 1.0\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("JsonReader: Unexpected visitValue (double) in BitBuffer! (JsonParser:7:25)",
                            e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, wrongBitBufferStringValueException)
{
    std::stringstream str(
            "{\n"
            "    \"nested\": {\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 255\n"
            "             ],\n"
            "             \"bitSize\": \"Text\"\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("JsonReader: Unexpected visitValue (string) in BitBuffer! (JsonParser:7:25)",
                            e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, wrongBytesException)
{
    std::stringstream str(
            "{\n"
            "    \"value\": 13,\n"
            "    \"nested\": {\n"
            "        \"value\": 10,\n"
            "        \"text\": \"nested\",\n"
            "        \"externData\": {\n"
            "             \"buffer\": [\n"
            "                 203,\n"
            "                 240\n"
            "             ],\n"
            "             \"bitSize\": 12\n"
            "        },\n"
            "        \"bytesData\" : {\n"
            "            \"buffer\" : {}\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("JsonReader: Unexpected beginObject in bytes! (JsonParser:14:25)", e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, negativeBytesByteValueException)
{
    std::stringstream str(
            "{\n"
            "    \"nested\": {\n"
            "        \"bytesData\": {\n"
            "             \"buffer\": [\n"
            "                 -1\n"
            "             ]\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ(
                            "JsonReader: Unexpected visitValue (int) in bytes! (JsonParser:5:18)", e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, jsonArrayException)
{
    std::stringstream str("[1, 2]");
    JsonReader jsonReader(str);

    ASSERT_THROW(jsonReader.read(typeInfo<CreatorObject>()), CppRuntimeException);
}

TEST(JsonReaderTest, jsonValueException)
{
    std::stringstream str("\"text\"");
    JsonReader jsonReader(str);

    ASSERT_THROW(jsonReader.read(typeInfo<CreatorObject>()), CppRuntimeException);
}

TEST(JsonReaderTest, bigLongValueException)
{
    std::stringstream str(
            "{\n"
            "    \"value\": 4294967296\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("ZserioTreeCreator: Integral value '4294967296' overflow (<0, 4294967295>)! "
                                 "(JsonParser:2:14)",
                            e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, floatLongValueException)
{
    std::stringstream str(
            "{\n"
            "    \"value\": 1.234\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("ZserioTreeCreator: Value '1.233' cannot be converted to integral value! "
                                 "(JsonParser:2:14)",
                            e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, bigBytesByteValueException)
{
    std::stringstream str(
            "{\n"
            "    \"nested\": {\n"
            "        \"bytesData\": {\n"
            "             \"buffer\": [\n"
            "                 256\n"
            "             ]\n"
            "        }\n"
            "    }\n"
            "}");
    JsonReader jsonReader(str);

    ASSERT_THROW(
            {
                try
                {
                    jsonReader.read(typeInfo<CreatorObject>());
                }
                catch (const CppRuntimeException& e)
                {
                    ASSERT_STREQ("JsonReader: Cannot create byte for bytes from value '256'! (JsonParser:5:18)",
                            e.what());
                    throw;
                }
            },
            CppRuntimeException);
}

TEST(JsonReaderTest, bytesAdapterUninitializedCalls)
{
    detail::BytesAdapter<std::allocator<uint8_t>> bytesAdapter{std::allocator<uint8_t>()};

    ASSERT_THROW(bytesAdapter.get(), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.beginObject(), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.endObject(), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.beginArray(), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.endArray(), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.visitKey("nonexisting"sv), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.visitValue(static_cast<uint64_t>(10)), CppRuntimeException);
    bytesAdapter.visitKey("buffer"sv);
    ASSERT_THROW(bytesAdapter.get(), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.visitKey("nonexisting"sv), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.visitValue(nullptr), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.visitValue(true), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.visitValue(static_cast<int64_t>(-1)), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.visitValue(static_cast<uint64_t>(1)), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.visitValue(0.0), CppRuntimeException);
    ASSERT_THROW(bytesAdapter.visitValue("BadValue"sv), CppRuntimeException);
}

TEST(JsonReaderTest, bitBufferAdapterUninitializedCalls)
{
    detail::BitBufferAdapter<std::allocator<uint8_t>> bitBufferAdapter{std::allocator<uint8_t>()};

    ASSERT_THROW(bitBufferAdapter.get(), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.beginObject(), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.endObject(), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.beginArray(), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.endArray(), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.visitKey("nonexisting"sv), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.visitValue(static_cast<uint64_t>(10)), CppRuntimeException);
    bitBufferAdapter.visitKey("buffer"sv);
    ASSERT_THROW(bitBufferAdapter.get(), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.visitKey("nonexisting"sv), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.visitValue(nullptr), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.visitValue(true), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.visitValue(static_cast<int64_t>(-1)), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.visitValue(static_cast<uint64_t>(1)), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.visitValue(0.0), CppRuntimeException);
    ASSERT_THROW(bitBufferAdapter.visitValue("BadValue"sv), CppRuntimeException);
}

TEST(JsonReaderTest, creatorAdapter)
{
    detail::CreatorAdapter<std::allocator<uint8_t>> creatorAdapter{std::allocator<uint8_t>()};

    ASSERT_THROW(creatorAdapter.get(), CppRuntimeException);
    ASSERT_THROW(creatorAdapter.beginObject(), CppRuntimeException);
    ASSERT_THROW(creatorAdapter.endObject(), CppRuntimeException);
    ASSERT_THROW(creatorAdapter.beginArray(), CppRuntimeException);
    ASSERT_THROW(creatorAdapter.endArray(), CppRuntimeException);
    ASSERT_THROW(creatorAdapter.visitKey("nonexistent"sv), CppRuntimeException);
    ASSERT_THROW(creatorAdapter.visitValue(nullptr), CppRuntimeException);
    ASSERT_THROW(creatorAdapter.visitValue(true), CppRuntimeException);
    ASSERT_THROW(creatorAdapter.visitValue(static_cast<int64_t>(-1)), CppRuntimeException);
    ASSERT_THROW(creatorAdapter.visitValue(static_cast<uint64_t>(1)), CppRuntimeException);
    ASSERT_THROW(creatorAdapter.visitValue(0.0), CppRuntimeException);
    ASSERT_THROW(creatorAdapter.visitValue("BadValue"sv), CppRuntimeException);
}

} // namespace zserio
