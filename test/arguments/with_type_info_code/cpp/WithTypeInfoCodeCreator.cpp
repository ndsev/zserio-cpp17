#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"

#include "WithTypeInfoCodeCreator.h"

namespace with_type_info_code
{

using AllocatorType = WithTypeInfoCode::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

using BitBufferType = zserio::BasicBitBuffer<zserio::RebindAlloc<AllocatorType, uint8_t>>;
using BytesType = zserio::BasicBytes<AllocatorType>;

static void fillSimpleStruct(SimpleStruct& simpleStruct)
{
    simpleStruct.fieldOffset = 0;
    simpleStruct.fieldFloat32 = 4.0;
}

static void fillParameterizedStruct(ParameterizedStruct& parameterizedStruct, const SimpleStruct& simpleStruct)
{
    VectorType<zserio::UInt8> array;
    for (zserio::UInt32 i = 0; i < simpleStruct.fieldU32; i++)
    {
        array.push_back(static_cast<zserio::UInt8::ValueType>(i));
    }
    parameterizedStruct.array = array;
}

static void fillComplexStruct(ComplexStruct& complexStruct, bool createOptionals)
{
    fillSimpleStruct(complexStruct.simpleStruct);

    fillSimpleStruct(complexStruct.anotherSimpleStruct);

    if (createOptionals)
    {
        SimpleStruct simpleStruct;
        fillSimpleStruct(simpleStruct);
        complexStruct.optionalSimpleStruct = simpleStruct;
    }

    const VectorType<zserio::UInt32> array = {3, 0xABCD2, 0xABCD3, 0xABCD4, 0xABCD5};
    complexStruct.array = array;

    const VectorType<zserio::Int5> arrayWithLen = {3, 2, 1};
    complexStruct.arrayWithLen = arrayWithLen;

    if (createOptionals)
    {
        ParameterizedStruct parameterizedStruct0;
        fillParameterizedStruct(parameterizedStruct0, complexStruct.simpleStruct);
        ParameterizedStruct parameterizedStruct1;
        fillParameterizedStruct(parameterizedStruct1, complexStruct.anotherSimpleStruct);
        const VectorType<ParameterizedStruct> paramStructArray = {parameterizedStruct0, parameterizedStruct1};
        complexStruct.paramStructArray = paramStructArray;
    }

    complexStruct.dynamicBitField = 8;

    VectorType<zserio::DynUInt64<>> dynamicBitFieldArray;
    for (size_t i = 1; i < 65536; i += 2)
    {
        dynamicBitFieldArray.push_back(i);
    }
    complexStruct.dynamicBitFieldArray = dynamicBitFieldArray;

    if (createOptionals)
    {
        complexStruct.optionalEnum = TestEnum::ItemThree;
        complexStruct.optionalBitmask =
                TestBitmask::Values::RED | TestBitmask::Values::_Green | TestBitmask::Values::ColorBlue;
        const VectorType<uint8_t> buffer = {0xCB, 0xF0};
        complexStruct.optionalExtern = BitBufferType(buffer, 12);
        complexStruct.optionalBytes = BytesType{{0xAB, 0xCD}};
    }

    VectorType<TestEnum> enumArray;
    enumArray.push_back(TestEnum::_TWO);
    enumArray.push_back(TestEnum::ItemThree);
    complexStruct.enumArray = enumArray;

    VectorType<TestBitmask> bitmaskArray;
    for (size_t i = 0; i < static_cast<size_t>(TestEnum::_TWO); ++i)
    {
        bitmaskArray.push_back(TestBitmask::Values::_Green);
    }
    complexStruct.bitmaskArray = bitmaskArray;
}

static void fillRecursiveStruct(RecursiveStruct& recursiveStruct)
{
    recursiveStruct.fieldU32 = 0xDEAD1;
    recursiveStruct.fieldRecursion = RecursiveStruct(0xDEAD2, std::nullopt, VectorType<RecursiveStruct>());
    const VectorType<RecursiveStruct> arrayRecursion = {
            RecursiveStruct(0xDEAD3, std::nullopt, VectorType<RecursiveStruct>()),
            RecursiveStruct(0xDEAD4, std::nullopt, VectorType<RecursiveStruct>())};
    recursiveStruct.arrayRecursion = arrayRecursion;
}

static void fillRecursiveUnion(RecursiveUnion& recursiveUnion)
{
    RecursiveUnion recursiveUnionFieldU32;
    recursiveUnionFieldU32.emplace<RecursiveUnion::Tag::fieldU32>(0xDEAD);
    const VectorType<RecursiveUnion> recursive = {recursiveUnionFieldU32};
    recursiveUnion.emplace<RecursiveUnion::Tag::recursive>(recursive);
}

static void fillRecursiveChoice(RecursiveChoice& recursiveChoice, bool param1, bool param2)
{
    if (param1)
    {
        RecursiveChoice recursiveChoiceFalse;
        fillRecursiveChoice(recursiveChoiceFalse, param2, false);
        const VectorType<RecursiveChoice> recursive = {recursiveChoiceFalse};
        recursiveChoice.emplace<RecursiveChoice::Tag::recursive>(recursive);
    }
    else
    {
        recursiveChoice.emplace<RecursiveChoice::Tag::fieldU32>(0xDEAD);
    }
}

static void fillSimpleUnion(SimpleUnion& simpleUnion)
{
    simpleUnion.emplace<SimpleUnion::Tag::testBitmask>(TestBitmask::Values::_Green);
}

static void fillSimpleChoice(SimpleChoice& simpleChoice, const TestEnum& testEnum)
{
    if (testEnum == TestEnum::_TWO)
    {
        SimpleUnion simpleUnion;
        fillSimpleUnion(simpleUnion);
        simpleChoice.emplace<SimpleChoice::Tag::fieldTwo>(simpleUnion);
    }
    else
    {
        simpleChoice.emplace<SimpleChoice::Tag::fieldDefault>("text");
    }
}

static void fillTS32(TS32& ts32)
{
    ts32.field = 0xDEAD;
}

static void fillTemplatedParameterizedStruct_TS32(
        TemplatedParameterizedStruct_TS32& templatedParameterizedStruct_TS32, const TS32& ts32)
{
    VectorType<zserio::UInt32> array;
    for (zserio::UInt32 i = ts32.field; i > 0; --i)
    {
        array.push_back(i);
    }
    templatedParameterizedStruct_TS32.array = array;
}

void fillWithTypeInfoCode(WithTypeInfoCode& withTypeInfoCode, bool createOptionals)
{
    fillSimpleStruct(withTypeInfoCode.simpleStruct);
    fillComplexStruct(withTypeInfoCode.complexStruct, createOptionals);
    fillParameterizedStruct(withTypeInfoCode.parameterizedStruct, withTypeInfoCode.simpleStruct);
    fillRecursiveStruct(withTypeInfoCode.recursiveStruct);
    fillRecursiveUnion(withTypeInfoCode.recursiveUnion);
    fillRecursiveChoice(withTypeInfoCode.recursiveChoice, true, false);
    withTypeInfoCode.selector = TestEnum::_TWO;
    fillSimpleChoice(withTypeInfoCode.simpleChoice, withTypeInfoCode.selector);
    fillTS32(withTypeInfoCode.templatedStruct);
    fillTemplatedParameterizedStruct_TS32(
            withTypeInfoCode.templatedParameterizedStruct, withTypeInfoCode.templatedStruct);
    const BitBufferType externData({0xCA, 0xFE}, 15);
    withTypeInfoCode.externData = externData;
    const VectorType<BitBufferType> externArray = {externData, externData};
    withTypeInfoCode.externArray = externArray;
    withTypeInfoCode.bytesData = BytesType{{0xAB, 0xCD}};
    const VectorType<BytesType> bytesArray{{
            BytesType{{0xAB, 0xCD}},
            BytesType{{0xAB, 0xCD}},
    }};
    withTypeInfoCode.bytesArray = zserio::Extended<VectorType<BytesType>>(bytesArray);
    const VectorType<zserio::UInt32> implicitArray = {1, 4, 6, 4, 6, 1};
    withTypeInfoCode.implicitArray = zserio::Extended<VectorType<zserio::UInt32>>(implicitArray);
}

} // namespace with_type_info_code
