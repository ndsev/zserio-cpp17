#include "gtest/gtest.h"
#include "with_reflection_code/Choice.h"
#include "with_reflection_code/Extended.h"
#include "with_reflection_code/Original.h"
#include "zserio/SerializeUtil.h"
#include "zserio/TypeInfoUtil.h"
#include "zserio/ZserioTreeCreator.h"

using namespace std::literals;

namespace with_reflection_code
{

using AllocatorType = Choice::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;
using IReflectableData = zserio::IBasicReflectableData<AllocatorType>;
using IReflectableDataPtr = zserio::IBasicReflectableDataPtr<AllocatorType>;
using ZserioTreeCreator = zserio::BasicZserioTreeCreator<AllocatorType>;
using Any = zserio::BasicAny<AllocatorType>;
using BitBuffer = zserio::BasicBitBuffer<AllocatorType>;
using Bytes = zserio::BasicBytes<AllocatorType>;

class WithReflectionCodeTest : public ::testing::Test
{
protected:
    static Parameterized createParameterized(uint8_t param)
    {
        auto parameterized = Parameterized{VectorType<zserio::UInt8>(5)};
        for (uint8_t i = 0; i < param; ++i)
        {
            parameterized.array[i] = zserio::UInt8(i + 1);
        }

        return parameterized;
    }

    static Struct createStruct()
    {
        return Struct{
                Empty{},
                std::nullopt, // child (auto optional)
                VectorType<Child>{Child{0, "zero", false, std::nullopt},
                        Child{1, "one", true, VectorType<StringType>{{{"best"}, {"first"}}}},
                        Child{2, "two", false, std::nullopt}},
                5, // param
                createParameterized(5),
                4, // len
                VectorType<zserio::UInt32>(4), // offsets
                VectorType<Parameterized>{{createParameterized(5), createParameterized(5),
                        createParameterized(5), createParameterized(5)}},
                Bitmask::Values::FLAG1 | Bitmask::Values::FLAG2,
                VectorType<Bitmask>{{Bitmask::Values::FLAG1, Bitmask::Values::FLAG2 | Bitmask::Values::FLAG3}},
                Selector::STRUCT,
                VectorType<Selector>{{Selector::STRUCT, SelectorEnum::UNION, Selector::BITMASK}},
                zserio::DynUInt64<>(31), // dynamicBitField (bit<param>)
                VectorType<zserio::DynUInt64<>>{{10, 20, 30}},
                std::nullopt, // dynamicIntField (param > 4)
                VectorType<zserio::DynInt8<4>>{{-3, -1, 1, 3}},
                VectorType<zserio::Bool>{{true, false, true}},
                BitBuffer{{0xAB, 0xCD}, 16},
                VectorType<BitBuffer>{BitBuffer{{0x02}, 2}, BitBuffer{{0x01}, 1}},
                Bytes{{0xAB, 0xCD}},
                VectorType<Bytes>{{{{0xDE, 0xAD}}, {{0xCA, 0xFE}}}},
        };
    }

    static void createReflectableParameterized(ZserioTreeCreator& creator, uint8_t param)
    {
        creator.beginArray("array");
        for (uint8_t i = 0; i < param; ++i)
        {
            creator.addValueElement(i + 1);
        }
        creator.endArray();
    }

    // the created object shall match to the struct created via createStruct() above
    static void createReflectableStruct(ZserioTreeCreator& creator)
    {
        creator.beginArray("childArray");
        creator.beginCompoundElement();
        creator.setValue("id", 0);
        creator.setValue("name", "zero");
        creator.setValue("hasNicknames", false);
        creator.endCompoundElement();
        creator.beginCompoundElement();
        creator.setValue("id", 1);
        creator.setValue("name", "one");
        creator.setValue("hasNicknames", true);
        creator.beginArray("nicknames");
        creator.addValueElement("best");
        creator.addValueElement("first");
        creator.endArray();
        creator.endCompoundElement();
        creator.beginCompoundElement();
        creator.setValue("id", 2);
        creator.setValue("name", "two");
        creator.setValue("hasNicknames", false);
        creator.endCompoundElement();
        creator.endArray();
        const uint8_t param = 5;
        creator.setValue("param", param);
        creator.beginCompound("parameterized");
        createReflectableParameterized(creator, 5);
        creator.endCompound();
        const size_t len = 4;
        creator.setValue("len", len);
        creator.beginArray("offsets");
        for (size_t i = 0; i < len; ++i)
        {
            creator.addValueElement(0);
        }
        creator.endArray();
        creator.beginArray("parameterizedArray");
        for (size_t i = 0; i < len; ++i)
        {
            creator.beginCompoundElement();
            createReflectableParameterized(creator, param);
            creator.endCompoundElement();
        }
        creator.endArray();
        creator.setValue("bitmaskField", (Bitmask::Values::FLAG1 | Bitmask::Values::FLAG2).getValue());
        creator.beginArray("bitmaskArray");
        creator.addValueElement(Bitmask(Bitmask::Values::FLAG1).getValue());
        creator.addValueElement((Bitmask::Values::FLAG2 | Bitmask::Values::FLAG3).getValue());
        creator.endArray();
        creator.setValue("enumField", zserio::enumToValue(Selector::STRUCT));
        creator.beginArray("enumArray");
        creator.addValueElement(zserio::enumToValue(Selector::STRUCT));
        creator.addValueElement(zserio::enumToValue(SelectorEnum::UNION));
        creator.addValueElement(zserio::enumToValue(Selector::BITMASK));
        creator.endArray();
        creator.setValue("dynamicBitField", 31);
        creator.beginArray("dynamicBitFieldArray");
        creator.addValueElement(10);
        creator.addValueElement(20);
        creator.addValueElement(30);
        creator.endArray();
        // dynamicIntField is optional - omitted
        creator.beginArray("dynamicIntFieldArray");
        creator.addValueElement(-3);
        creator.addValueElement(-1);
        creator.addValueElement(1);
        creator.addValueElement(3);
        creator.endArray();
        creator.beginArray("boolArray");
        creator.addValueElement(true);
        creator.addValueElement(false);
        creator.addValueElement(true);
        creator.endArray();
        creator.setValue("externField", BitBuffer({0xAB, 0xCD}, 16));
        creator.beginArray("externArray");
        creator.addValueElement(BitBuffer{{0x02}, 2});
        creator.addValueElement(BitBuffer{{0x01}, 1});
        creator.endArray();
        creator.setValue("bytesField", VectorType<uint8_t>{{0xAB, 0xCD}});
        creator.beginArray("bytesArray");
        creator.addValueElement(VectorType<uint8_t>{{0xDE, 0xAD}});
        creator.addValueElement(VectorType<uint8_t>{{0xCA, 0xFE}});
        creator.endArray();
    }

    void checkStructReflectable(IReflectableData& reflectable)
    {
        ASSERT_EQ("with_reflection_code.Struct"sv, reflectable.getTypeInfo().getSchemaName());

        // Empty empty;
        ASSERT_EQ("with_reflection_code.Empty"sv, reflectable.getField("empty")->getTypeInfo().getSchemaName());

        // optional Child child;
        ASSERT_EQ(nullptr, reflectable.getField("child")); // non-present optional

        // Child childArray[];
        ASSERT_TRUE(reflectable["childArray"]->isArray());
        ASSERT_EQ(3, reflectable.getField("childArray")->size());
        ASSERT_FALSE(reflectable["childArray"]->at(0)->find("hasNicknames")->getBool());
        ASSERT_FALSE(reflectable["childArray"]->at(0)->find("nicknames"));
        ASSERT_TRUE(reflectable["childArray"]->at(1)->find("hasNicknames")->getBool());
        ASSERT_EQ("true", reflectable["childArray"]->at(1)->find("hasNicknames")->toString());
        ASSERT_TRUE(reflectable["childArray"]->at(1)->find("nicknames")->isArray());
        ASSERT_EQ(2, reflectable["childArray"]->at(1)->find("nicknames")->size());
        ASSERT_EQ("first", reflectable["childArray"]->at(1)->find("nicknames")->at(1)->toString());

        // uint8 param;
        ASSERT_EQ(5, reflectable.getField("param")->getUInt8());
        ASSERT_EQ(5, reflectable.getField("param")->toUInt());
        ASSERT_EQ("5", reflectable.getField("param")->toString());
        ASSERT_THROW(reflectable.getField("param")->toInt(), zserio::CppRuntimeException);
        ASSERT_THROW(reflectable.getField("param")->getInt8(), zserio::CppRuntimeException);

        // Parameterized(param) parameterized;
        ASSERT_TRUE(reflectable["parameterized.array"]->isArray());
        ASSERT_EQ(5, reflectable["parameterized.array"]->size());
        ASSERT_EQ(5, reflectable["parameterized.array"]->at(4)->getUInt8());
        ASSERT_EQ(4, (*(reflectable["parameterized.array"]))[3]->toUInt());
        ASSERT_THROW((*(reflectable["parameterized.array"]))[4]->toInt(), zserio::CppRuntimeException);

        // varsize len : len > 0 && len < 1000;
        ASSERT_EQ(zserio::SchemaType::VARSIZE, reflectable.getField("len")->getTypeInfo().getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, reflectable.getField("len")->getTypeInfo().getCppType());
        ASSERT_EQ(4, reflectable.find("len")->getUInt32());
        ASSERT_EQ(4, reflectable.find("len")->toUInt());
        ASSERT_EQ(4.0, reflectable.find("len")->toDouble());
        ASSERT_THROW(reflectable.find("len")->toInt(), zserio::CppRuntimeException);

        // uint32 offsets[len];
        ASSERT_TRUE(reflectable.getField("offsets")->isArray());
        ASSERT_EQ(4, reflectable.getField("offsets")->size());
        ASSERT_EQ(
                0, reflectable.getField("offsets")->at(0)->getUInt32()); // offset shall be default-initialized
        ASSERT_EQ(0, reflectable.getField("offsets")->at(0)->toDouble());

        // Parameterized(param) parameterizedArray[len];
        ASSERT_TRUE(reflectable.getField("parameterizedArray")->isArray());
        ASSERT_EQ(4, reflectable.getField("parameterizedArray")->size());
        ASSERT_EQ("3", reflectable["parameterizedArray"]->at(2)->find("array")->at(2)->toString());

        // Bitmask bitmaskField;
        ASSERT_EQ(Bitmask(Bitmask::Values::FLAG1 | Bitmask::Values::FLAG2).getValue(),
                reflectable.getField("bitmaskField")->getUInt8());
        ASSERT_EQ(Bitmask(Bitmask::Values::FLAG1 | Bitmask::Values::FLAG2).getValue(),
                reflectable.getField("bitmaskField")->toDouble());
        ASSERT_EQ(Bitmask(Bitmask::Values::FLAG1 | Bitmask::Values::FLAG2).toString(),
                reflectable.getField("bitmaskField")->toString());

        // Bitmask bitmaskArray[];
        ASSERT_TRUE(reflectable.getField("bitmaskArray")->isArray());
        ASSERT_EQ(2, reflectable.getField("bitmaskArray")->size());
        ASSERT_EQ(Bitmask(Bitmask::Values::FLAG1).getValue(), reflectable["bitmaskArray"]->at(0)->toUInt());

        // SelectorEnum enumField;
        ASSERT_EQ(zserio::SchemaType::ENUM, reflectable["enumField"]->getTypeInfo().getSchemaType());
        ASSERT_EQ(zserio::CppType::ENUM, reflectable["enumField"]->getTypeInfo().getCppType());
        ASSERT_EQ(zserio::enumToValue(Selector::STRUCT), reflectable.getField("enumField")->getInt8());
        ASSERT_THROW(reflectable.getField("enumField")->toUInt(), zserio::CppRuntimeException);
        ASSERT_EQ(zserio::enumToString(Selector::STRUCT), reflectable.getField("enumField")->toString());

        // Selector enumArray[];
        ASSERT_EQ(zserio::SchemaType::ENUM, reflectable["enumArray"]->getTypeInfo().getSchemaType());
        auto enumArrayFieldInfo = reflectable.getTypeInfo().getFields()[11];
        ASSERT_EQ("enumArray"sv, enumArrayFieldInfo.schemaName);
        ASSERT_TRUE(enumArrayFieldInfo.isArray);
        ASSERT_EQ(""sv, enumArrayFieldInfo.arrayLength);
        ASSERT_TRUE(enumArrayFieldInfo.isPacked);
        ASSERT_FALSE(enumArrayFieldInfo.isImplicit);
        ASSERT_TRUE(reflectable["enumArray"]->isArray());
        ASSERT_EQ(zserio::enumToValue(Selector::STRUCT), reflectable["enumArray"]->at(0)->getInt8());
        ASSERT_EQ(zserio::enumToValue(Selector::STRUCT), reflectable["enumArray"]->at(0)->toDouble());

        // bit<param> dynamicBitField if param < 64;
        ASSERT_EQ(31, reflectable["dynamicBitField"]->toUInt());
        ASSERT_EQ(31, reflectable["dynamicBitField"]->getUInt64());
        ASSERT_THROW(reflectable["dynamicBitField"]->getUInt8(), zserio::CppRuntimeException);

        // bit<param> dynamicBitFieldArray[];
        ASSERT_TRUE(reflectable.getField("dynamicBitFieldArray")->isArray());
        ASSERT_EQ(3, reflectable.getField("dynamicBitFieldArray")->size());
        ASSERT_EQ(10, reflectable["dynamicBitFieldArray"]->at(0)->getUInt64());
        ASSERT_EQ(20, reflectable["dynamicBitFieldArray"]->at(1)->toUInt());
        ASSERT_EQ(20.0, reflectable["dynamicBitFieldArray"]->at(1)->toDouble());
        ASSERT_EQ("30", reflectable["dynamicBitFieldArray"]->at(2)->toString());

        // int<param> dynamicIntField if param < 4;
        ASSERT_EQ(nullptr, reflectable.getField("dynamicIntField"));

        // int<4> dynamicIntFieldArray[4] if param < 64;
        ASSERT_TRUE(reflectable.getField("dynamicIntFieldArray")->isArray());
        ASSERT_EQ(4, reflectable.getField("dynamicIntFieldArray")->size());
        ASSERT_THROW(reflectable["dynamicIntFieldArray"]->at(0)->toUInt(), zserio::CppRuntimeException);
        ASSERT_EQ(-3, reflectable["dynamicIntFieldArray"]->at(0)->toInt());
        ASSERT_EQ(-1, reflectable["dynamicIntFieldArray"]->at(1)->getInt8());
        ASSERT_EQ(-1.0, reflectable["dynamicIntFieldArray"]->at(1)->toDouble());
        ASSERT_EQ("-1", reflectable["dynamicIntFieldArray"]->at(1)->toString());
        ASSERT_EQ(1, reflectable["dynamicIntFieldArray"]->at(2)->getInt8());

        // bool boolArray[];
        ASSERT_TRUE(reflectable["boolArray"]->isArray());
        ASSERT_EQ(3, reflectable["boolArray"]->size());
        ASSERT_FALSE(reflectable["boolArray"]->at(1)->getBool());
        ASSERT_EQ(0.0, reflectable["boolArray"]->at(1)->toDouble());
        ASSERT_EQ("false", reflectable["boolArray"]->at(1)->toString());

        // extern externField;
        ASSERT_EQ(16, reflectable.getField("externField")->getBitBuffer().getBitSize());
        ASSERT_EQ(0xAB, reflectable.getField("externField")->getBitBuffer().getBuffer()[0]);
        ASSERT_EQ(0xCD, reflectable.getField("externField")->getBitBuffer().getBuffer()[1]);

        // extern externArray[];
        ASSERT_TRUE(reflectable["externArray"]->isArray());
        ASSERT_EQ(2, reflectable["externArray"]->size());
        ASSERT_EQ(2, reflectable["externArray"]->at(0)->getBitBuffer().getBitSize());
        ASSERT_EQ(0x02, reflectable["externArray"]->at(0)->getBitBuffer().getBuffer()[0]);
        ASSERT_EQ(1, reflectable["externArray"]->at(1)->getBitBuffer().getBitSize());
        ASSERT_EQ(0x01, reflectable["externArray"]->at(1)->getBitBuffer().getBuffer()[0]);

        // bytes bytesField;
        ASSERT_EQ(2, reflectable.getField("bytesField")->getBytes().size());
        ASSERT_EQ(0xAB, reflectable.getField("bytesField")->getBytes().data()[0]);
        ASSERT_EQ(0xCD, reflectable.getField("bytesField")->getBytes().data()[1]);

        // bytes bytesArray[];
        ASSERT_TRUE(reflectable["bytesArray"]->isArray());
        ASSERT_EQ(2, reflectable["bytesArray"]->size());
        ASSERT_EQ(2, reflectable["bytesArray"]->at(0)->getBytes().size());
        ASSERT_EQ(0xDE, reflectable["bytesArray"]->at(0)->getBytes()[0]);
        ASSERT_EQ(0xAD, reflectable["bytesArray"]->at(0)->getBytes()[1]);
        ASSERT_EQ(2, reflectable["bytesArray"]->at(1)->getBytes().size());
        ASSERT_EQ(0xCA, reflectable["bytesArray"]->at(1)->getBytes()[0]);
        ASSERT_EQ(0xFE, reflectable["bytesArray"]->at(1)->getBytes()[1]);

        // setField tests
        reflectable.setField("enumField", Any(Selector::UNION));
        ASSERT_EQ(zserio::enumToString(Selector::UNION), reflectable["enumField"]->toString());
        // - wrong type in any holder
        ASSERT_THROW(reflectable.setField("bitmaskField", Any(Selector::UNION)), zserio::CppRuntimeException);
        // - field does not exist
        ASSERT_THROW(reflectable.setField("nonexistent", Any(0)), zserio::CppRuntimeException);
        // - optional child
        reflectable.setField("child", Any(Child{13, "thirteen", false, std::nullopt}));
        ASSERT_TRUE(reflectable.getField("child"));
        ASSERT_EQ("thirteen"sv, reflectable["child.name"]->getStringView());
        ASSERT_FALSE(reflectable["child.hasNicknames"]->getBool());
    }
};

TEST_F(WithReflectionCodeTest, checkChoiceWithStructure)
{
    Choice choice;
    choice.emplace<Choice::Tag::structField>(createStruct());

    auto reflectablePtr = reflectable(choice, AllocatorType{});
    ASSERT_EQ("with_reflection_code.Choice"sv, reflectablePtr->getTypeInfo().getSchemaName());

    ASSERT_TRUE(zserio::TypeInfoUtil::hasChoice(reflectablePtr->getTypeInfo().getCppType()));
    ASSERT_EQ("structField"sv, reflectablePtr->getChoice());

    auto structReflectable = reflectablePtr->getField(reflectablePtr->getChoice());
    ASSERT_TRUE(structReflectable);
    ASSERT_EQ("with_reflection_code.Struct"sv, structReflectable->getTypeInfo().getSchemaName());

    // existing array element somewhere within the structField
    ASSERT_EQ(1, (*reflectablePtr)["structField.parameterized.array"]->at(0)->toUInt());
    ASSERT_EQ(
            3, reflectablePtr->find("structField.parameterizedArray")->at(1)->find("array")->at(2)->getUInt8());

    checkStructReflectable(*structReflectable);

    // check child set by checkStructReflectable
    ASSERT_EQ("thirteen"sv, (*reflectablePtr)["structField.child.name"]->getStringView());

    // set field within child
    structReflectable->setField("dynamicBitField", Any(static_cast<uint64_t>(13)));
    ASSERT_EQ(13, reflectablePtr->find("structField.dynamicBitField")->toUInt());
}

// fully created via reflectable interface
TEST_F(WithReflectionCodeTest, checkReflectableChoiceWithStruct)
{
    ZserioTreeCreator creator(zserio::typeInfo<Choice>());
    creator.beginRoot();
    creator.beginCompound("structField");
    createReflectableStruct(creator);
    creator.endCompound();
    IReflectableDataPtr reflectablePtr = creator.endRoot();
    ASSERT_TRUE(reflectablePtr);

    auto structReflectable = reflectablePtr->find("structField");
    ASSERT_TRUE(structReflectable);
    checkStructReflectable(*structReflectable);
}

TEST_F(WithReflectionCodeTest, checkChoiceWithUnion)
{
    Choice choice;
    Union unionField;
    unionField.emplace<Union::Tag::structField>(createStruct());
    choice.emplace<Choice::Tag::unionField>(unionField);

    auto reflectablePtr = reflectable(choice, AllocatorType{});
    ASSERT_EQ("with_reflection_code.Choice"sv, reflectablePtr->getTypeInfo().getSchemaName());

    ASSERT_TRUE(zserio::TypeInfoUtil::hasChoice(reflectablePtr->getTypeInfo().getCppType()));
    ASSERT_EQ("unionField"sv, reflectablePtr->getChoice());

    ASSERT_EQ("with_reflection_code.Union"sv,
            reflectablePtr->getField(reflectablePtr->getChoice())->getTypeInfo().getSchemaName());

    // non-present choices
    ASSERT_EQ(nullptr, reflectablePtr->find("unionField.childArray"));
    ASSERT_EQ(nullptr, reflectablePtr->find("unionField.parameterized"));
    ASSERT_EQ(nullptr, reflectablePtr->find("unionField.bitmaskField"));
    auto unionReflectable = reflectablePtr->getField("unionField");
    ASSERT_TRUE(unionReflectable);
    ASSERT_THROW(unionReflectable->getField("childArray"), zserio::CppRuntimeException);

    auto structReflectable = reflectablePtr->find("unionField.structField");
    ASSERT_TRUE(structReflectable);
    checkStructReflectable(*structReflectable);
}

TEST_F(WithReflectionCodeTest, checkChoiceWithBitmask)
{
    Choice choice;
    choice.emplace<Choice::Tag::bitmaskField>(Bitmask::Values::FLAG3);

    auto reflectablePtr = zserio::reflectable(choice, AllocatorType{});
    ASSERT_EQ("with_reflection_code.Choice"sv, reflectablePtr->getTypeInfo().getSchemaName());

    ASSERT_TRUE(zserio::TypeInfoUtil::hasChoice(reflectablePtr->getTypeInfo().getCppType()));
    ASSERT_EQ("bitmaskField"sv, reflectablePtr->getChoice());

    // exact getter
    ASSERT_EQ(Bitmask(Bitmask::Values::FLAG3).getValue(), reflectablePtr->find("bitmaskField")->getUInt8());
    // conversion to unsigned int
    ASSERT_EQ(Bitmask(Bitmask::Values::FLAG3).getValue(),
            reflectablePtr->getField(reflectablePtr->getChoice())->toUInt());
    // conversion to string
    ASSERT_EQ(Bitmask(Bitmask::Values::FLAG3).toString(), (*reflectablePtr)["bitmaskField"]->toString());

    // non-present choices
    ASSERT_EQ(nullptr, reflectablePtr->find("structField"));
    ASSERT_EQ(nullptr, reflectablePtr->find("unionField"));

    // nonexistent
    ASSERT_EQ(nullptr, (*reflectablePtr)["nonexistent.something"]);
    ASSERT_THROW(reflectablePtr->getField("nonexistent"), zserio::CppRuntimeException);

    // invalid paths
    ASSERT_EQ(nullptr, reflectablePtr->find("invalid..something"));
    ASSERT_EQ(nullptr, reflectablePtr->find("."));
    ASSERT_EQ(nullptr, reflectablePtr->find(".."));
    ASSERT_EQ(nullptr, reflectablePtr->find(".something"));
    ASSERT_EQ(nullptr, reflectablePtr->find("..something"));
    ASSERT_EQ(nullptr, reflectablePtr->find("invalid."));
    ASSERT_EQ(nullptr, reflectablePtr->find("invalid.."));
}

TEST_F(WithReflectionCodeTest, childOptional)
{
    // optional not present
    Child child{13, "name", false, std::nullopt};
    auto reflectablePtr = zserio::reflectable(child, AllocatorType{});
    ASSERT_EQ(nullptr, reflectablePtr->getField("nicknames"));

    // optional present
    child.hasNicknames = true;
    child.nicknames = VectorType<StringType>{{{"nick1"}, {"nick2"}}};
    ASSERT_NE(nullptr, reflectablePtr->getField("nicknames"));
    ASSERT_EQ(2, reflectablePtr->getField("nicknames")->size());
}

TEST_F(WithReflectionCodeTest, checkExtendedField)
{
    Original originalData(42);
    auto bitBuffer = zserio::serialize(originalData);
    Extended extendedData;
    (void)zserio::deserialize<Extended>(bitBuffer, extendedData);
    auto reflectablePtr = zserio::reflectable(extendedData, AllocatorType{});

    ASSERT_EQ(42, (*reflectablePtr)["field"]->toUInt());

    ASSERT_EQ(nullptr, (*reflectablePtr)["extendedField"]); // not present
    ASSERT_EQ(nullptr, reflectablePtr->getField("extendedField"));

    *extendedData.extendedField = "hello world";
    extendedData.extendedField.setPresent(true);

    ASSERT_NE(nullptr, (*reflectablePtr)["extendedField"]); // present
    ASSERT_EQ("hello world", reflectablePtr->getField("extendedField")->toString());

    // create using reflectable
    ZserioTreeCreator creator(zserio::typeInfo<Extended>());
    creator.beginRoot();
    reflectablePtr = creator.endRoot();
    ASSERT_NE(nullptr, (*reflectablePtr)["extendedField"]); // is present
    ASSERT_EQ("", reflectablePtr->getField("extendedField")->toString()); // default constructed

    reflectablePtr->setField("extendedField", Any(StringType("hello world")));
    ASSERT_EQ("hello world", reflectablePtr->getField("extendedField")->toString());
}

} // namespace with_reflection_code
