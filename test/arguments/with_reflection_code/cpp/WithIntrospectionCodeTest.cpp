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

using AllocatorType = Choice::AllocatorType;
using StringAllocatorType = zserio::RebindAlloc<AllocatorType, char>;
using StringType = zserio::BasicString<StringAllocatorType>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;
using IReflectableData = zserio::IBasicReflectableData<AllocatorType>;
using IReflectableDataPtr = zserio::IBasicReflectableDataPtr<AllocatorType>;
using IIntrospectableView = zserio::IBasicIntrospectableView<AllocatorType>;
using IIntrospectableViewConstPtr = zserio::IBasicIntrospectableViewConstPtr<AllocatorType>;
using ZserioTreeCreator = zserio::BasicZserioTreeCreator<AllocatorType>;
using Any = zserio::BasicAny<AllocatorType>;
using BitBuffer = zserio::BasicBitBuffer<AllocatorType>;
using Bytes = zserio::BasicBytes<AllocatorType>;

class WithIntrospectionCodeTest : public ::testing::Test
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

    void checkWriteThrows(const IIntrospectableView& introspectableView)
    {
        zserio::BitBuffer bitBuffer;
        zserio::BitStreamWriter writer(bitBuffer);
        ASSERT_THROW(introspectableView.write(writer), zserio::CppRuntimeException);
    }

    // for builtin types
    template <typename CHECKER>
    void checkWriteReadBuiltin(const IIntrospectableView& introspectableView, const CHECKER& checker)
    {
        zserio::BitBuffer bitBuffer(1024 * 8);
        zserio::BitStreamWriter writer(bitBuffer);
        introspectableView.write(writer);
        const size_t bitSizeOf = introspectableView.bitSizeOf();
        ASSERT_EQ(bitSizeOf, writer.getBitPosition());

        zserio::BitStreamReader reader(bitBuffer);
        checker(reader);
        ASSERT_EQ(bitSizeOf, reader.getBitPosition());
    }

    template <typename T, typename... ARGS>
    void checkWriteRead(
            const IIntrospectableView& introspectableView, const zserio::View<T>& originalView, ARGS... args)
    {
        const size_t bitSizeOf = introspectableView.bitSizeOf();
        zserio::BitBuffer bitBuffer(bitSizeOf);
        zserio::BitStreamWriter writer(bitBuffer);
        introspectableView.write(writer);
        ASSERT_EQ(bitSizeOf, writer.getBitPosition());

        zserio::BitStreamReader reader(bitBuffer);
        T readObject;
        zserio::View<T> readView = zserio::detail::read(reader, readObject, args...);
        ASSERT_EQ(originalView, readView);
        ASSERT_EQ(bitSizeOf, reader.getBitPosition());
    }

    template <typename T, std::enable_if_t<std::is_enum_v<T> || zserio::is_bitmask_v<T>, int> = 0>
    void checkWriteReadValue(const IIntrospectableView& introspectableView, T value)
    {
        const size_t bitSizeOf = introspectableView.bitSizeOf();
        zserio::BitBuffer bitBuffer(bitSizeOf);
        zserio::BitStreamWriter writer(bitBuffer);
        introspectableView.write(writer);
        ASSERT_EQ(bitSizeOf, writer.getBitPosition());

        zserio::BitStreamReader reader(bitBuffer);
        T readValue{};
        zserio::detail::read(reader, readValue);
        ASSERT_EQ(value, readValue);
        ASSERT_EQ(bitSizeOf, reader.getBitPosition());
    }

    void checkStructReflectable(
            const IIntrospectableView& introspectableView, const zserio::View<Struct>& structureView)
    {
        ASSERT_EQ("with_reflection_code.Struct"sv, introspectableView.getTypeInfo().getSchemaName());

        // Empty empty;
        ASSERT_EQ("with_reflection_code.Empty"sv,
                introspectableView.getField("empty")->getTypeInfo().getSchemaName());

        // optional Child child;
        ASSERT_EQ(nullptr, introspectableView.getField("child")); // non-present optional

        // Child childArray[];
        ASSERT_TRUE(introspectableView["childArray"]->isArray());
        ASSERT_EQ(3, introspectableView.getField("childArray")->size());
        ASSERT_FALSE(introspectableView["childArray"]->at(0)->find("hasNicknames")->getBool());
        ASSERT_FALSE(introspectableView["childArray"]->at(0)->find("nicknames"));
        ASSERT_TRUE(introspectableView["childArray"]->at(1)->find("hasNicknames")->getBool());
        ASSERT_EQ("true", introspectableView["childArray"]->at(1)->find("hasNicknames")->toString());
        ASSERT_TRUE(introspectableView["childArray"]->at(1)->find("nicknames")->isArray());
        ASSERT_EQ(2, introspectableView["childArray"]->at(1)->find("nicknames")->size());
        ASSERT_EQ("first", introspectableView["childArray"]->at(1)->find("nicknames")->at(1)->toString());
        checkWriteThrows(*(introspectableView["childArray"]));
        checkWriteRead(*(introspectableView["childArray"]->at(1)), structureView.childArray()[1]);
        checkWriteReadBuiltin(*(introspectableView["childArray"]->at(1)->getField("name")),
                [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.childArray()[1].name(), reader.readString<StringAllocatorType>());
                });
        checkWriteThrows(*(introspectableView["childArray"]->at(1)->getField("nicknames")));
        checkWriteReadBuiltin(*(introspectableView["childArray"]->at(1)->getField("nicknames")->at(1)),
                [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.childArray()[1].nicknames()->at(1),
                            reader.readString<StringAllocatorType>());
                });

        // uint8 param;
        ASSERT_EQ(5, introspectableView.getField("param")->getUInt8());
        ASSERT_EQ(5, introspectableView.getField("param")->toUInt());
        ASSERT_EQ("5", introspectableView.getField("param")->toString());
        ASSERT_THROW(introspectableView.getField("param")->toInt(), zserio::CppRuntimeException);
        ASSERT_THROW(introspectableView.getField("param")->getInt8(), zserio::CppRuntimeException);
        checkWriteReadBuiltin(
                *(introspectableView.getField("param")), [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.param(), reader.readUnsignedBits32(8));
                });

        // Parameterized(param) parameterized;
        ASSERT_EQ(5, introspectableView["parameterized.param"]->getUInt8());
        ASSERT_EQ("5", introspectableView.getField("parameterized")->getParameter("param")->toString());
        ASSERT_THROW(
                introspectableView.getField("parameterized")->getField("param"), zserio::CppRuntimeException);
        ASSERT_TRUE(introspectableView["parameterized.array"]->isArray());
        ASSERT_EQ(5, introspectableView["parameterized.array"]->size());
        ASSERT_EQ(5, introspectableView["parameterized.array"]->at(4)->getUInt8());
        ASSERT_EQ(4, (*(introspectableView["parameterized.array"]))[3]->toUInt());
        ASSERT_THROW((*(introspectableView["parameterized.array"]))[4]->toInt(), zserio::CppRuntimeException);
        checkWriteThrows(*(introspectableView["parameterized.array"]));
        checkWriteRead(
                *(introspectableView["parameterized"]), structureView.parameterized(), structureView.param());

        // varsize len : len > 0 && len < 1000;
        ASSERT_EQ(
                zserio::SchemaType::VARSIZE, introspectableView.getField("len")->getTypeInfo().getSchemaType());
        ASSERT_EQ(zserio::CppType::UINT32, introspectableView.getField("len")->getTypeInfo().getCppType());
        ASSERT_EQ(4, introspectableView.find("len")->getUInt32());
        ASSERT_EQ(4, introspectableView.find("len")->toUInt());
        ASSERT_EQ(4.0, introspectableView.find("len")->toDouble());
        ASSERT_THROW(introspectableView.find("len")->toInt(), zserio::CppRuntimeException);
        checkWriteReadBuiltin(
                *(introspectableView.find("len")), [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.len(), reader.readVarSize());
                });

        // uint32 offsets[len];
        ASSERT_TRUE(introspectableView.getField("offsets")->isArray());
        ASSERT_EQ(4, introspectableView.getField("offsets")->size());
        ASSERT_NE(0, introspectableView.getField("offsets")->at(0)->getUInt32()); // offset shall be initialized
        ASSERT_NE(0, introspectableView.getField("offsets")->at(0)->toDouble());
        checkWriteThrows(*(introspectableView.getField("offsets")));
        checkWriteReadBuiltin(*(introspectableView.getField("offsets")->at(0)),
                [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.offsets()[0], reader.readUnsignedBits32(32));
                });

        // Parameterized(param) parameterizedArray[len];
        ASSERT_TRUE(introspectableView.getField("parameterizedArray")->isArray());
        ASSERT_EQ(4, introspectableView.getField("parameterizedArray")->size());
        ASSERT_EQ("3", introspectableView["parameterizedArray"]->at(2)->find("array")->at(2)->toString());
        checkWriteThrows(*(introspectableView["parameterizedArray"]));
        checkWriteRead(*(introspectableView["parameterizedArray"]->at(2)),
                structureView.parameterizedArray()[2], structureView.param());

        // Bitmask bitmaskField;
        ASSERT_EQ(Bitmask(Bitmask::Values::FLAG1 | Bitmask::Values::FLAG2).getValue(),
                introspectableView.getField("bitmaskField")->getUInt8());
        ASSERT_EQ(Bitmask(Bitmask::Values::FLAG1 | Bitmask::Values::FLAG2).getValue(),
                introspectableView.getField("bitmaskField")->toDouble());
        ASSERT_EQ(Bitmask(Bitmask::Values::FLAG1 | Bitmask::Values::FLAG2).toString(),
                introspectableView.getField("bitmaskField")->toString());
        checkWriteReadValue(*(introspectableView.getField("bitmaskField")), structureView.bitmaskField());

        // Bitmask bitmaskArray[];
        ASSERT_TRUE(introspectableView.getField("bitmaskArray")->isArray());
        ASSERT_EQ(2, introspectableView.getField("bitmaskArray")->size());
        ASSERT_EQ(Bitmask(Bitmask::Values::FLAG1).getValue(),
                introspectableView["bitmaskArray"]->at(0)->toUInt());
        checkWriteThrows(*(introspectableView["bitmaskArray"]));
        checkWriteReadValue(*(introspectableView["bitmaskArray"]->at(0)), structureView.bitmaskArray()[0]);

        // SelectorEnum enumField;
        ASSERT_EQ(zserio::SchemaType::ENUM, introspectableView["enumField"]->getTypeInfo().getSchemaType());
        ASSERT_EQ(zserio::CppType::ENUM, introspectableView["enumField"]->getTypeInfo().getCppType());
        ASSERT_EQ(zserio::enumToValue(Selector::STRUCT), introspectableView.getField("enumField")->getInt8());
        ASSERT_THROW(introspectableView.getField("enumField")->toUInt(), zserio::CppRuntimeException);
        ASSERT_EQ(zserio::enumToString(Selector::STRUCT), introspectableView.getField("enumField")->toString());
        checkWriteReadValue(*(introspectableView.getField("enumField")), structureView.enumField());

        // Selector enumArray[];
        ASSERT_EQ(zserio::SchemaType::ENUM, introspectableView["enumArray"]->getTypeInfo().getSchemaType());
        auto enumArrayFieldInfo = introspectableView.getTypeInfo().getFields()[11];
        ASSERT_EQ("enumArray"sv, enumArrayFieldInfo.schemaName);
        ASSERT_TRUE(enumArrayFieldInfo.isArray);
        ASSERT_EQ(""sv, enumArrayFieldInfo.arrayLength);
        ASSERT_TRUE(enumArrayFieldInfo.isPacked);
        ASSERT_FALSE(enumArrayFieldInfo.isImplicit);
        ASSERT_TRUE(introspectableView["enumArray"]->isArray());
        ASSERT_EQ(zserio::enumToValue(Selector::STRUCT), introspectableView["enumArray"]->at(0)->getInt8());
        ASSERT_EQ(zserio::enumToValue(Selector::STRUCT), introspectableView["enumArray"]->at(0)->toDouble());
        checkWriteThrows(*(introspectableView.getField("enumArray")));
        checkWriteReadValue(*(introspectableView.getField("enumArray")->at(0)), structureView.enumArray()[0]);

        // bit<param> dynamicBitField if param < 64;
        ASSERT_EQ(31, introspectableView["dynamicBitField"]->toUInt());
        ASSERT_EQ(31, introspectableView["dynamicBitField"]->getUInt64());
        ASSERT_THROW(introspectableView["dynamicBitField"]->getUInt8(), zserio::CppRuntimeException);
        checkWriteReadBuiltin(
                *(introspectableView["dynamicBitField"]), [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(
                            structureView.dynamicBitField(), reader.readUnsignedBits64(structureView.param()));
                });

        // bit<param> dynamicBitFieldArray[];
        ASSERT_TRUE(introspectableView.getField("dynamicBitFieldArray")->isArray());
        ASSERT_EQ(3, introspectableView.getField("dynamicBitFieldArray")->size());
        ASSERT_EQ(10, introspectableView["dynamicBitFieldArray"]->at(0)->getUInt64());
        ASSERT_EQ(20, introspectableView["dynamicBitFieldArray"]->at(1)->toUInt());
        ASSERT_EQ(20.0, introspectableView["dynamicBitFieldArray"]->at(1)->toDouble());
        ASSERT_EQ("30", introspectableView["dynamicBitFieldArray"]->at(2)->toString());
        checkWriteThrows(*(introspectableView["dynamicBitFieldArray"]));
        checkWriteReadBuiltin(*(introspectableView["dynamicBitFieldArray"]->at(2)),
                [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.dynamicBitFieldArray()[2],
                            reader.readUnsignedBits64(structureView.param()));
                });

        // int<param> dynamicIntField if param < 4;
        ASSERT_EQ(nullptr, introspectableView.getField("dynamicIntField"));
        ASSERT_THROW(introspectableView.getParameter("dynamicIntField"), zserio::CppRuntimeException);

        // int<4> dynamicIntFieldArray[4] if param < 64;
        ASSERT_TRUE(introspectableView.getField("dynamicIntFieldArray")->isArray());
        ASSERT_EQ(4, introspectableView.getField("dynamicIntFieldArray")->size());
        ASSERT_THROW(introspectableView["dynamicIntFieldArray"]->at(0)->toUInt(), zserio::CppRuntimeException);
        ASSERT_EQ(-3, introspectableView["dynamicIntFieldArray"]->at(0)->toInt());
        ASSERT_EQ(-1, introspectableView["dynamicIntFieldArray"]->at(1)->getInt8());
        ASSERT_EQ(-1.0, introspectableView["dynamicIntFieldArray"]->at(1)->toDouble());
        ASSERT_EQ("-1", introspectableView["dynamicIntFieldArray"]->at(1)->toString());
        ASSERT_EQ(1, introspectableView["dynamicIntFieldArray"]->at(2)->getInt8());
        checkWriteThrows(*(introspectableView["dynamicIntFieldArray"]));
        checkWriteReadBuiltin(*(introspectableView["dynamicIntFieldArray"]->at(0)),
                [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.dynamicIntFieldArray()->at(0), reader.readSignedBits32(4));
                });

        // bool boolArray[];
        ASSERT_TRUE(introspectableView["boolArray"]->isArray());
        ASSERT_EQ(3, introspectableView["boolArray"]->size());
        ASSERT_FALSE(introspectableView["boolArray"]->at(1)->getBool());
        ASSERT_EQ(0.0, introspectableView["boolArray"]->at(1)->toDouble());
        ASSERT_EQ("false", introspectableView["boolArray"]->at(1)->toString());
        checkWriteThrows(*(introspectableView["boolArray"]));
        checkWriteReadBuiltin(
                *(introspectableView["boolArray"]->at(0)), [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.boolArray()[0], reader.readBool());
                });

        // extern externField;
        ASSERT_EQ(16, introspectableView.getField("externField")->getBitBuffer().getBitSize());
        ASSERT_EQ(0xAB, introspectableView.getField("externField")->getBitBuffer().getBytes().at(0));
        ASSERT_EQ(0xCD, introspectableView.getField("externField")->getBitBuffer().getBytes().at(1));
        checkWriteReadBuiltin(*(introspectableView.getField("externField")),
                [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.externField().get(), reader.readBitBuffer<AllocatorType>());
                });

        // extern externArray[];
        ASSERT_TRUE(introspectableView["externArray"]->isArray());
        ASSERT_EQ(2, introspectableView["externArray"]->size());
        ASSERT_EQ(2, introspectableView["externArray"]->at(0)->getBitBuffer().getBitSize());
        ASSERT_EQ(0x02, introspectableView["externArray"]->at(0)->getBitBuffer().getBytes().at(0));
        ASSERT_EQ(1, introspectableView["externArray"]->at(1)->getBitBuffer().getBitSize());
        ASSERT_EQ(0x01, introspectableView["externArray"]->at(1)->getBitBuffer().getBytes().at(0));
        checkWriteThrows(*(introspectableView["externArray"]));
        checkWriteReadBuiltin(*(introspectableView.getField("externArray")->at(0)),
                [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.externArray()[0].get(), reader.readBitBuffer<AllocatorType>());
                });

        // bytes bytesField;
        ASSERT_EQ(2, introspectableView.getField("bytesField")->getBytes().size());
        ASSERT_EQ(0xAB, introspectableView.getField("bytesField")->getBytes().data()[0]);
        ASSERT_EQ(0xCD, introspectableView.getField("bytesField")->getBytes().data()[1]);
        checkWriteReadBuiltin(*(introspectableView.getField("bytesField")),
                [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.bytesField(), reader.readBytes<AllocatorType>());
                });

        // bytes bytesArray[];
        ASSERT_TRUE(introspectableView["bytesArray"]->isArray());
        ASSERT_EQ(2, introspectableView["bytesArray"]->size());
        ASSERT_EQ(2, introspectableView["bytesArray"]->at(0)->getBytes().size());
        ASSERT_EQ(0xDE, introspectableView["bytesArray"]->at(0)->getBytes()[0]);
        ASSERT_EQ(0xAD, introspectableView["bytesArray"]->at(0)->getBytes()[1]);
        ASSERT_EQ(2, introspectableView["bytesArray"]->at(1)->getBytes().size());
        ASSERT_EQ(0xCA, introspectableView["bytesArray"]->at(1)->getBytes()[0]);
        ASSERT_EQ(0xFE, introspectableView["bytesArray"]->at(1)->getBytes()[1]);
        checkWriteThrows(*(introspectableView["bytesArray"]));
        checkWriteReadBuiltin(*(introspectableView.getField("bytesArray")->at(0)),
                [&structureView](zserio::BitStreamReader& reader) {
                    ASSERT_EQ(structureView.bytesArray()[0], reader.readBytes<AllocatorType>());
                });

        // function Selector getEnumField()
        ASSERT_EQ(zserio::enumToString(SelectorEnum::STRUCT),
                introspectableView.callFunction("getEnumField")->toString());
        ASSERT_EQ(zserio::enumToValue(SelectorEnum::STRUCT), introspectableView["getEnumField"]->toInt());

        // write read check on structure
        checkWriteRead(introspectableView, structureView);
    }
};

TEST_F(WithIntrospectionCodeTest, checkChoiceWithStructure)
{
    Choice choice;
    choice.emplace<Choice::ChoiceTag::CHOICE_structField>(createStruct());
    zserio::View<Choice> view(choice, Selector::STRUCT);
    zserio::detail::initializeOffsets(view, 0);

    auto introspectableView = zserio::introspectable(view);
    ASSERT_EQ("with_reflection_code.Choice"sv, introspectableView->getTypeInfo().getSchemaName());

    ASSERT_EQ(
            zserio::enumToValue(SelectorEnum::STRUCT), introspectableView->getParameter("selector")->getInt8());
    ASSERT_EQ(zserio::enumToValue(SelectorEnum::STRUCT), introspectableView->getParameter("selector")->toInt());
    ASSERT_EQ(zserio::enumToString(SelectorEnum::STRUCT), introspectableView->find("selector")->toString());

    ASSERT_TRUE(zserio::TypeInfoUtil::hasChoice(introspectableView->getTypeInfo().getCppType()));
    ASSERT_EQ("structField"sv, introspectableView->getChoice());

    auto structIntrospectableView = introspectableView->getField(introspectableView->getChoice());
    ASSERT_TRUE(structIntrospectableView);
    ASSERT_EQ("with_reflection_code.Struct"sv, structIntrospectableView->getTypeInfo().getSchemaName());

    // existing array element somewhere within the structField
    ASSERT_EQ(1, (*introspectableView)["structField.parameterized.array"]->at(0)->toUInt());
    ASSERT_EQ(3,
            introspectableView->find("structField.parameterizedArray")
                    ->at(1)
                    ->find("array")
                    ->at(2)
                    ->getUInt8());

    // function on structField
    ASSERT_EQ(zserio::enumToValue(Selector::STRUCT),
            introspectableView->find("structField.getEnumField")->getInt8());

    ASSERT_EQ("zero"sv, introspectableView->callFunction("getFirstChildName")->getStringView());
    ASSERT_EQ("zero", introspectableView->find("getFirstChildName")->toString());

    checkStructReflectable(*structIntrospectableView, view.structField());

    // write read check on choice
    checkWriteRead(*introspectableView, view, Selector::STRUCT);
}

// fully created via reflectable interface
TEST_F(WithIntrospectionCodeTest, checkReflectableChoiceWithStruct)
{
    ZserioTreeCreator creator(zserio::typeInfo<Choice>());
    creator.beginRoot();
    creator.beginCompound("structField");
    createReflectableStruct(creator);
    creator.endCompound();
    IReflectableDataPtr reflectable = creator.endRoot();
    ASSERT_TRUE(reflectable);

    // TODO[Mi-L@]: introspectable from reflectable?
    Choice& choiceData = reflectable->getAnyValue().get<std::reference_wrapper<Choice>>();
    IIntrospectableViewConstPtr introspectableView =
            zserio::introspectable(zserio::View(choiceData, Selector::STRUCT));
    introspectableView->initializeOffsets();

    Choice choice;
    choice.emplace<Choice::ChoiceTag::CHOICE_structField>(createStruct());
    zserio::View view(choice, Selector::STRUCT);
    zserio::detail::initializeOffsets(view, 0);

    // check that write-read of object created via reflections gets the same object as the one
    // created via generated classes
    checkWriteRead(*introspectableView, view, Selector::STRUCT);
}

TEST_F(WithIntrospectionCodeTest, checkChoiceWithUnion)
{
    Choice choice;
    Union unionField;
    unionField.emplace<Union::ChoiceTag::CHOICE_structField>(createStruct());
    choice.emplace<Choice::ChoiceTag::CHOICE_unionField>(unionField);
    zserio::View view(choice, Selector::UNION);

    IIntrospectableViewConstPtr introspectableView = zserio::introspectable(view);
    ASSERT_EQ("with_reflection_code.Choice"sv, introspectableView->getTypeInfo().getSchemaName());
    introspectableView->initializeOffsets();

    ASSERT_EQ(
            zserio::enumToValue(SelectorEnum::UNION), introspectableView->getParameter("selector")->getInt8());
    ASSERT_EQ(zserio::enumToValue(SelectorEnum::UNION), introspectableView->getParameter("selector")->toInt());
    ASSERT_EQ(zserio::enumToString(SelectorEnum::UNION), introspectableView->find("selector")->toString());

    ASSERT_TRUE(zserio::TypeInfoUtil::hasChoice(introspectableView->getTypeInfo().getCppType()));
    ASSERT_EQ("unionField"sv, introspectableView->getChoice());

    ASSERT_EQ("with_reflection_code.Union"sv,
            introspectableView->getField(introspectableView->getChoice())->getTypeInfo().getSchemaName());

    // non-present choices
    ASSERT_EQ(nullptr, introspectableView->find("unionField.childArray"));
    ASSERT_EQ(nullptr, introspectableView->find("unionField.parameterized"));
    ASSERT_EQ(nullptr, introspectableView->find("unionField.bitmaskField"));
    auto unionIntrospectableView = introspectableView->getField("unionField");
    ASSERT_TRUE(unionIntrospectableView);
    ASSERT_THROW(unionIntrospectableView->getField("childArray"), zserio::CppRuntimeException);

    ASSERT_EQ(5, introspectableView->find("unionField.getStructFieldParam")->toUInt());
    ASSERT_EQ(5, introspectableView->getField("unionField")->callFunction("getStructFieldParam")->getUInt8());

    auto structIntrospectableView = introspectableView->find("unionField.structField");
    ASSERT_TRUE(structIntrospectableView);
    checkStructReflectable(*structIntrospectableView, view.unionField().structField());

    // write read check on union
    checkWriteRead(*(introspectableView->getField("unionField")), view.unionField());

    // write read check on choice
    checkWriteRead(*introspectableView, view, Selector::UNION);
}

TEST_F(WithIntrospectionCodeTest, checkChoiceWithBitmask)
{
    Choice choice;
    choice.emplace<Choice::ChoiceTag::CHOICE_bitmaskField>(Bitmask::Values::FLAG3);
    zserio::View view(choice, Selector::BITMASK);

    auto introspectableView = zserio::introspectable(view);
    ASSERT_EQ("with_reflection_code.Choice"sv, introspectableView->getTypeInfo().getSchemaName());

    ASSERT_EQ(zserio::enumToValue(SelectorEnum::BITMASK),
            introspectableView->getParameter("selector")->getInt8());
    ASSERT_EQ(
            zserio::enumToValue(SelectorEnum::BITMASK), introspectableView->getParameter("selector")->toInt());
    ASSERT_EQ(zserio::enumToValue(SelectorEnum::BITMASK),
            introspectableView->getParameter("selector")->toDouble());
    ASSERT_EQ(zserio::enumToString(SelectorEnum::BITMASK), introspectableView->find("selector")->toString());

    ASSERT_TRUE(zserio::TypeInfoUtil::hasChoice(introspectableView->getTypeInfo().getCppType()));
    ASSERT_EQ("bitmaskField"sv, introspectableView->getChoice());

    // exact getter
    ASSERT_EQ(Bitmask(Bitmask::Values::FLAG3).getValue(), introspectableView->find("bitmaskField")->getUInt8());
    // conversion to unsigned int
    ASSERT_EQ(Bitmask(Bitmask::Values::FLAG3).getValue(),
            introspectableView->getField(introspectableView->getChoice())->toUInt());
    // conversion to string
    ASSERT_EQ(Bitmask(Bitmask::Values::FLAG3).toString(), (*introspectableView)["bitmaskField"]->toString());

    // non-present choices
    ASSERT_EQ(nullptr, introspectableView->find("structField"));
    ASSERT_EQ(nullptr, introspectableView->find("unionField"));

    // nonexistent
    ASSERT_EQ(nullptr, (*introspectableView)["nonexistent.something"]);
    ASSERT_THROW(introspectableView->getField("nonexistent"), zserio::CppRuntimeException);
    ASSERT_THROW(introspectableView->getParameter("nonexistent"), zserio::CppRuntimeException);
    ASSERT_THROW(introspectableView->callFunction("nonexistent"), zserio::CppRuntimeException);

    // invalid paths
    ASSERT_EQ(nullptr, introspectableView->find("invalid..something"));
    ASSERT_EQ(nullptr, introspectableView->find("."));
    ASSERT_EQ(nullptr, introspectableView->find(".."));
    ASSERT_EQ(nullptr, introspectableView->find(".something"));
    ASSERT_EQ(nullptr, introspectableView->find("..something"));
    ASSERT_EQ(nullptr, introspectableView->find("invalid."));
    ASSERT_EQ(nullptr, introspectableView->find("invalid.."));

    // call function which throws via find shall return nullptr
    ASSERT_EQ(nullptr, introspectableView->find("getBitmaskFromUnion"));
    // call function via callFunction method throws
    ASSERT_THROW(introspectableView->callFunction("getBitmaskFromUnion"), zserio::CppRuntimeException);

    // write read check on bitmask
    checkWriteReadValue(*((*introspectableView)["bitmaskField"]), view.bitmaskField());

    // write read check on choice
    checkWriteRead(*introspectableView, view, Selector::BITMASK);
}

TEST_F(WithIntrospectionCodeTest, childOptionalInconsistencies)
{
    // optional not present
    Child child{13, "name", false, std::nullopt};
    zserio::View view(child);
    auto introspectableView = introspectable(view);
    ASSERT_EQ(nullptr, introspectableView->getField("nicknames"));

    // optional present
    child.hasNicknames = true; // TODO[Mi-L@]: Still passes when not set to true!
    child.nicknames = VectorType<StringType>{{{"nick1"}, {"nick2"}}};
    ASSERT_NE(nullptr, introspectableView->getField("nicknames"));
    ASSERT_EQ(2, introspectableView->getField("nicknames")->size());
}

TEST_F(WithIntrospectionCodeTest, checkExtendedField)
{
    Original original(42);
    auto bitBuffer = zserio::serialize(original);
    Extended extended;
    auto extendedView = zserio::deserialize(bitBuffer, extended);

    auto introspectableView = zserio::introspectable(extendedView);

    ASSERT_EQ(42, (*introspectableView)["field"]->toUInt());

    ASSERT_EQ(nullptr, (*introspectableView)["extendedField"]); // not present
    ASSERT_EQ(nullptr, introspectableView->getField("extendedField"));

    extended.extendedField.setPresent(true);
    extended.extendedField.value() = "hello world";

    ASSERT_NE(nullptr, (*introspectableView)["extendedField"]); // present
    ASSERT_EQ("hello world", introspectableView->getField("extendedField")->toString());

    // create using reflectable
    ZserioTreeCreator creator(zserio::typeInfo<Extended>());
    creator.beginRoot();
    auto reflectable = creator.endRoot();
    Extended& extendedData = reflectable->getAnyValue().get<std::reference_wrapper<Extended>>();
    introspectableView = zserio::introspectable(zserio::View(extendedData));
    ASSERT_NE(nullptr, (*introspectableView)["extendedField"]); // is present
    ASSERT_EQ("", introspectableView->getField("extendedField")->toString()); // default constructed
}

} // namespace with_reflection_code
