#include <cmath>

#include "gtest/gtest.h"
#include "test_object/std_allocator/ReflectableUtilBitmask.h"
#include "test_object/std_allocator/ReflectableUtilEnum.h"
#include "test_object/std_allocator/ReflectableUtilUnion.h"
#include "zserio/ReflectableData.h"
#include "zserio/ReflectableUtil.h"
#include "zserio/TypeInfo.h"

using test_object::std_allocator::ReflectableUtilBitmask;
using test_object::std_allocator::ReflectableUtilChoice;
using test_object::std_allocator::ReflectableUtilEnum;
using test_object::std_allocator::ReflectableUtilObject;
using test_object::std_allocator::ReflectableUtilUnion;

namespace zserio
{

TEST(ReflectableUtilTest, unequalTypeInfo)
{
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Bool(false)), reflectable(UInt8(13))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Int8(0)), reflectable(Int16(0))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Int8(0)), reflectable(UInt8(0))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(UInt64(0)), reflectable(VarSize(0))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Int8(0)), reflectable(Float32(0.0))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Float16(0.0F)), reflectable(Float32(0.0F))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(String("")), reflectable(Int8(0))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(BitBuffer(BitBuffer())), reflectable(String(""))));
}

TEST(ReflectableUtilTest, equalsNullValues)
{
    ASSERT_TRUE(ReflectableUtil::equal(nullptr, nullptr));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Bool(false)), nullptr));
    ASSERT_FALSE(ReflectableUtil::equal(nullptr, reflectable(Int8(0))));
}

TEST(ReflectableUtilTest, equalBools)
{
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Bool(true)), reflectable(Bool(true))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Bool(false)), reflectable(Bool(true))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Bool(true)), reflectable(Bool(false))));
}

TEST(ReflectableUtilTest, equalSingedIntegrals)
{
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Int8(INT8_MIN)), reflectable(Int8(INT8_MIN))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Int8(INT8_MAX)), reflectable(Int8(INT8_MAX))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Int8(-1)), reflectable(Int8(0))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Int8(INT8_MIN)), reflectable(Int8(INT8_MAX))));

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Int16(INT16_MIN)), reflectable(Int16(INT16_MIN))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Int16(INT16_MAX)), reflectable(Int16(INT16_MAX))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Int16(-1)), reflectable(Int16(0))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Int16(INT16_MIN)), reflectable(Int16(INT16_MAX))));

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Int32(INT32_MIN)), reflectable(Int32(INT32_MIN))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Int32(INT32_MAX)), reflectable(Int32(INT32_MAX))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Int32(-1)), reflectable(Int32(0))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Int32(INT32_MIN)), reflectable(Int32(INT32_MAX))));

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Int64(INT64_MIN)), reflectable(Int64(INT64_MIN))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Int64(INT64_MAX)), reflectable(Int64(INT64_MAX))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Int64(-1)), reflectable(Int64(0))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Int64(INT64_MIN)), reflectable(Int64(INT64_MAX))));
}

TEST(ReflectableUtilTest, equalUnsignedIntegrals)
{
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(UInt8(0)), reflectable(UInt8(0))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(UInt8(UINT8_MAX)), reflectable(UInt8(UINT8_MAX))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(UInt8(0)), reflectable(UInt8(1))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(UInt8(0)), reflectable(UInt8(UINT8_MAX))));

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(UInt16(0)), reflectable(UInt16(0))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(UInt16(UINT16_MAX)), reflectable(UInt16(UINT16_MAX))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(UInt16(0)), reflectable(UInt16(1))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(UInt16(0)), reflectable(UInt16(UINT16_MAX))));

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(UInt32(0)), reflectable(UInt32(0))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(UInt32(UINT32_MAX)), reflectable(UInt32(UINT32_MAX))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(UInt32(0)), reflectable(UInt32(1))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(UInt32(0)), reflectable(UInt32(UINT32_MAX))));

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(UInt64(0)), reflectable(UInt64(0))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(UInt64(UINT64_MAX)), reflectable(UInt64(UINT64_MAX))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(UInt64(0)), reflectable(UInt64(1))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(UInt64(0)), reflectable(UInt64(UINT64_MAX))));
}

TEST(ReflectableUtilTest, equalFloatingPoints)
{
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Float16(0.0F)), reflectable(Float16(0.0F))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Float16(-1.0F)), reflectable(Float16(1.0F))));

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Float32(0.0F)), reflectable(Float32(0.0F))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Float32(-1.0F)), reflectable(Float32(1.0F))));

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Float64(0.0)), reflectable(Float64(0.0))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Float64(-1.0)), reflectable(Float64(1.0))));

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Float64(std::numeric_limits<double>::epsilon())),
            reflectable(Float64(std::numeric_limits<double>::epsilon()))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Float64(std::numeric_limits<double>::min())),
            reflectable(Float64(std::numeric_limits<double>::min()))));

    ASSERT_TRUE(ReflectableUtil::equal(
            reflectable(Float64(1.0 + std::numeric_limits<double>::epsilon())), reflectable(Float64(1.0))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Float64(std::numeric_limits<double>::denorm_min() * 2)),
            reflectable(Float64(std::numeric_limits<double>::denorm_min()))));

    ASSERT_TRUE(ReflectableUtil::equal(
            reflectable(Float64(static_cast<double>(NAN))), reflectable(Float64(static_cast<double>(NAN)))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Float64(static_cast<double>(INFINITY))),
            reflectable(Float64(static_cast<double>(INFINITY)))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Float64(-static_cast<double>(INFINITY))),
            reflectable(Float64(-static_cast<double>(INFINITY)))));

    ASSERT_FALSE(ReflectableUtil::equal(
            reflectable(Float64(0.0)), reflectable(Float64(static_cast<double>(INFINITY)))));
    ASSERT_FALSE(ReflectableUtil::equal(
            reflectable(Float64(static_cast<double>(INFINITY))), reflectable(Float64(0.0))));

    ASSERT_FALSE(
            ReflectableUtil::equal(reflectable(Float64(0.0)), reflectable(Float64(static_cast<double>(NAN)))));
    ASSERT_FALSE(
            ReflectableUtil::equal(reflectable(Float64(static_cast<double>(NAN))), reflectable(Float64(0.0))));

    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Float64(static_cast<double>(NAN))),
            reflectable(Float64(static_cast<double>(INFINITY)))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Float64(static_cast<double>(INFINITY))),
            reflectable(Float64(static_cast<double>(NAN)))));

    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Float64(static_cast<double>(INFINITY))),
            reflectable(Float64(-static_cast<double>(INFINITY)))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Float64(-static_cast<double>(INFINITY))),
            reflectable(Float64(static_cast<double>(INFINITY)))));
}

TEST(ReflectableUtilTest, equalStrings)
{
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(String("")), reflectable(String(""))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(String("test")), reflectable(String("test"))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(String("")), reflectable(String("a"))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(String("1")), reflectable(String(""))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(String("test")), reflectable(String("test2"))));
}

TEST(ReflectableUtilTest, equalBitBuffers)
{
    ASSERT_TRUE(
            ReflectableUtil::equal(reflectable(BitBuffer(BitBuffer())), reflectable(BitBuffer(BitBuffer()))));
    ASSERT_TRUE(ReflectableUtil::equal(
            reflectable(BitBuffer(BitBuffer({0xAB}, 8))), reflectable(BitBuffer(BitBuffer({0xAB}, 8)))));
    ASSERT_FALSE(ReflectableUtil::equal(
            reflectable(BitBuffer(BitBuffer({0xAB}, 8))), reflectable(BitBuffer(BitBuffer()))));
}

TEST(ReflectableUtilTest, equalBytes)
{
    Vector<uint8_t> bytesData1{{0xCA, 0xFE}};
    Vector<uint8_t> bytesData2{{0xCA, 0xDE}};
    Vector<uint8_t> bytesEmpty;

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Bytes(bytesEmpty)), reflectable(Bytes(bytesEmpty))));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(Bytes(bytesData1)), reflectable(Bytes(bytesData1))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Bytes(bytesData1)), reflectable(Bytes(bytesEmpty))));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(Bytes(bytesData1)), reflectable(Bytes(bytesData2))));
}

TEST(ReflectableUtilTest, equalEnums)
{
    const ReflectableUtilEnum oneEnum = ReflectableUtilEnum::ONE;
    const ReflectableUtilEnum twoEnum = ReflectableUtilEnum::TWO;

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(oneEnum), reflectable(oneEnum)));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(twoEnum), reflectable(twoEnum)));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(oneEnum), reflectable(twoEnum)));
}

TEST(ReflectableUtilTest, equalBitmasks)
{
    const ReflectableUtilBitmask readBitmask = ReflectableUtilBitmask::Values::READ;
    const ReflectableUtilBitmask writeBitmask = ReflectableUtilBitmask::Values::WRITE;

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(readBitmask), reflectable(readBitmask)));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(writeBitmask), reflectable(writeBitmask)));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(writeBitmask), reflectable(readBitmask)));
}

TEST(ReflectableUtilTest, equalCompounds)
{
    ReflectableUtilUnion compound1 = ReflectableUtilUnion();
    compound1.emplace<ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum>(ReflectableUtilEnum::ONE);

    ReflectableUtilUnion compound2 = ReflectableUtilUnion();
    compound2.emplace<ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum>(ReflectableUtilEnum::TWO);

    ReflectableUtilUnion compound3 = ReflectableUtilUnion();
    compound3.emplace<ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>(
            ReflectableUtilBitmask::Values::READ);

    ReflectableUtilUnion compound4 = ReflectableUtilUnion();
    compound4.emplace<ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>(
            ReflectableUtilObject(0, ReflectableUtilChoice()));

    ReflectableUtilUnion compound5 = ReflectableUtilUnion();
    compound5.emplace<ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>(
            ReflectableUtilObject(1, ReflectableUtilChoice()));
    compound5.get<ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>()
            .reflectableUtilChoice.emplace<ReflectableUtilChoice::ChoiceTag::CHOICE_array>(
                    std::vector<UInt32>());

    ReflectableUtilUnion compound6 = ReflectableUtilUnion();
    compound6.emplace<ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>(
            ReflectableUtilObject(1, ReflectableUtilChoice()));
    compound6.get<ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>()
            .reflectableUtilChoice.emplace<ReflectableUtilChoice::ChoiceTag::CHOICE_array>(
                    std::vector<UInt32>{{1, 2, 3}});

    ASSERT_TRUE(ReflectableUtil::equal(reflectable(compound1), reflectable(compound1)));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(compound2), reflectable(compound2)));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(compound3), reflectable(compound3)));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(compound4), reflectable(compound4)));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(compound5), reflectable(compound5)));
    ASSERT_TRUE(ReflectableUtil::equal(reflectable(compound6), reflectable(compound6)));

    ASSERT_FALSE(ReflectableUtil::equal(reflectable(compound1), reflectable(compound2)));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(compound1), reflectable(compound3)));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(compound1), reflectable(compound4)));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(compound1), reflectable(compound5)));
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(compound1), reflectable(compound6)));

    // unequal fields in choice
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(compound5), reflectable(compound6)));

    // array and not array
    std::vector<ReflectableUtilUnion> compoundArray(1);
    ASSERT_FALSE(ReflectableUtil::equal(reflectable(compound1), reflectableArray(compoundArray)));
    ASSERT_FALSE(ReflectableUtil::equal(reflectableArray(compoundArray), reflectable(compound1)));
}

TEST(ReflectableUtilTest, equalArrays)
{
    std::vector<UInt32> array1 = {1, 2, 3};
    std::vector<UInt32> array2 = {1, 2, 4};
    std::vector<UInt32> array3 = {1, 2};

    auto array1Reflectable = reflectableArray(array1);
    auto array2Reflectable = reflectableArray(array2);
    auto array3Reflectable = reflectableArray(array3);

    ASSERT_TRUE(ReflectableUtil::equal(array1Reflectable, array1Reflectable));
    ASSERT_TRUE(ReflectableUtil::equal(array2Reflectable, array2Reflectable));
    ASSERT_TRUE(ReflectableUtil::equal(array3Reflectable, array3Reflectable));

    ASSERT_FALSE(ReflectableUtil::equal(array1Reflectable, array2Reflectable));
    ASSERT_FALSE(ReflectableUtil::equal(array1Reflectable, array3Reflectable));
}

TEST(ReflectableUtilTest, equalWrong)
{
    using allocator_type = ::std::allocator<uint8_t>;
    class WrongTypeInfo : public detail::TypeInfoBase<allocator_type>
    {
    public:
        WrongTypeInfo(std::string_view schemaName, SchemaType schemaType, CppType cppType) :
                TypeInfoBase(schemaName, schemaType, cppType)
        {}
    };

    const WrongTypeInfo wrongTypeInfo = {"int<>", SchemaType::DYNAMIC_SIGNED_BITFIELD, CppType::PUBSUB};
    const WrongTypeInfo wrongTypeInfoDiffName = {"diff", SchemaType::DYNAMIC_SIGNED_BITFIELD, CppType::PUBSUB};

    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<allocator_type>
    {
    public:
        explicit Reflectable(
                const detail::TypeInfoBase<allocator_type>& typeInfo, const allocator_type& allocator) :
                ::zserio::ReflectableDataAllocatorHolderBase<allocator_type>(typeInfo, allocator)
        {}
    };

    auto wrongReflectable =
            std::allocate_shared<Reflectable>(allocator_type(), wrongTypeInfo, allocator_type());
    auto wrongReflectableDiffName =
            std::allocate_shared<Reflectable>(allocator_type(), wrongTypeInfoDiffName, allocator_type());

    ASSERT_THROW(ReflectableUtil::equal(wrongReflectable, wrongReflectable), CppRuntimeException);
    ASSERT_FALSE(ReflectableUtil::equal(wrongReflectable, wrongReflectableDiffName));
}

TEST(ReflectableUtilTest, getValueArithmeticType)
{
    ASSERT_FALSE(ReflectableUtil::getValue<Bool>(reflectable(Bool(false))));

    ASSERT_EQ(1, ReflectableUtil::getValue<UInt8>(reflectable(UInt8(1))));
    ASSERT_EQ(12, ReflectableUtil::getValue<UInt16>(reflectable(UInt16(12))));
    ASSERT_EQ(123, ReflectableUtil::getValue<UInt32>(reflectable(UInt32(123))));
    ASSERT_EQ(1234, ReflectableUtil::getValue<UInt64>(reflectable(UInt64(1234))));

    ASSERT_EQ(-1, ReflectableUtil::getValue<Int8>(reflectable(Int8(-1))));
    ASSERT_EQ(-12, ReflectableUtil::getValue<Int16>(reflectable(Int16(-12))));
    ASSERT_EQ(-123, ReflectableUtil::getValue<Int32>(reflectable(Int32(-123))));
    ASSERT_EQ(-1234, ReflectableUtil::getValue<Int64>(reflectable(Int64(-1234))));

    ASSERT_EQ(12, ReflectableUtil::getValue<VarUInt16>(reflectable(VarUInt16(12))));
    ASSERT_EQ(123, ReflectableUtil::getValue<VarUInt32>(reflectable(VarUInt32(123))));
    ASSERT_EQ(1234, ReflectableUtil::getValue<VarUInt64>(reflectable(VarUInt64(1234))));
    ASSERT_EQ(UINT64_MAX, ReflectableUtil::getValue<VarUInt>(reflectable(VarUInt(UINT64_MAX))));

    ASSERT_EQ(-12, ReflectableUtil::getValue<VarInt16>(reflectable(VarInt16(-12))));
    ASSERT_EQ(-123, ReflectableUtil::getValue<VarInt32>(reflectable(VarInt32(-123))));
    ASSERT_EQ(-1234, ReflectableUtil::getValue<VarInt64>(reflectable(VarInt64(-1234))));
    ASSERT_EQ(INT64_MIN, ReflectableUtil::getValue<VarInt>(reflectable(VarInt(INT64_MIN))));
    ASSERT_EQ(INT64_MAX, ReflectableUtil::getValue<VarInt>(reflectable(VarInt(INT64_MAX))));

    ASSERT_EQ(0, ReflectableUtil::getValue<VarSize>(reflectable(VarSize(0))));

    ASSERT_EQ(1.0F, ReflectableUtil::getValue<Float16>(reflectable(Float16(1.0F))));
    ASSERT_EQ(3.5F, ReflectableUtil::getValue<Float32>(reflectable(Float32(3.5F))));
    ASSERT_EQ(9.875, ReflectableUtil::getValue<Float64>(reflectable(Float64(9.875))));
}

TEST(ReflectableUtilTest, getValueString)
{
    ASSERT_EQ("test", ReflectableUtil::getValue<std::string_view>(reflectable(String("test"))));
}

TEST(ReflectableUtilTest, getValueBitBuffer)
{
    const BitBuffer bitBuffer;
    auto reflectablePtr = reflectable(BitBuffer(bitBuffer));
    const BitBuffer& bitBufferRef = ReflectableUtil::getValue<BitBuffer>(reflectablePtr);
    ASSERT_EQ(bitBuffer, bitBufferRef);
}

TEST(ReflectableUtilTest, getValueEnum)
{
    ReflectableUtilEnum reflectableUtilEnum = ReflectableUtilEnum::ONE;
    auto reflectablePtr = reflectable(reflectableUtilEnum);
    ASSERT_EQ(reflectableUtilEnum, ReflectableUtil::getValue<ReflectableUtilEnum>(reflectablePtr));
}

TEST(ReflectableUtilTest, getValueBitmask)
{
    ReflectableUtilBitmask reflectableUtilBitmask = ReflectableUtilBitmask::Values::READ;
    auto reflectablePtr = reflectable(reflectableUtilBitmask);
    ASSERT_EQ(reflectableUtilBitmask, ReflectableUtil::getValue<ReflectableUtilBitmask>(reflectablePtr));
}

TEST(ReflectableUtilTest, getValueCompound)
{
    ReflectableUtilUnion reflectableUtilUnion;
    auto reflectablePtr = reflectable(reflectableUtilUnion);
    ASSERT_EQ(&reflectableUtilUnion, &ReflectableUtil::getValue<ReflectableUtilUnion>(reflectablePtr));

    ReflectableUtilUnion& reflectableUtilUnionRef =
            ReflectableUtil::getValue<ReflectableUtilUnion>(reflectablePtr);
    reflectableUtilUnionRef.emplace<ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>(
            ReflectableUtilBitmask::Values::WRITE);
    ASSERT_EQ(ReflectableUtilBitmask::Values::WRITE,
            reflectableUtilUnion.get<ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>());

    auto constReflectable = reflectable(static_cast<const ReflectableUtilUnion&>(reflectableUtilUnion));
    const ReflectableUtilUnion& reflectableUtilUnionConstRef =
            ReflectableUtil::getValue<ReflectableUtilUnion>(constReflectable);
    ASSERT_EQ(ReflectableUtilBitmask::Values::WRITE,
            reflectableUtilUnionConstRef.get<ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>());
    ASSERT_EQ(&reflectableUtilUnion, &reflectableUtilUnionConstRef);
}

TEST(ReflectableUtilTest, getValueBuiltinArray)
{
    std::vector<UInt8> uint8Array{{1, 2, 3}};
    auto reflectablePtr = reflectableArray(uint8Array);
    std::vector<UInt8>& uint8ArrayRef = ReflectableUtil::getValue<std::vector<UInt8>>(reflectablePtr);
    ASSERT_EQ(&uint8Array, &uint8ArrayRef);

    auto constReflectable = reflectableArray(static_cast<const std::vector<UInt8>&>(uint8Array));
    const std::vector<UInt8>& uint8ArrayConstRef =
            ReflectableUtil::getValue<std::vector<UInt8>>(constReflectable);
    ASSERT_EQ(&uint8Array, &uint8ArrayConstRef);
}

TEST(ReflectableUtilTest, getValueCompoundArray)
{
    std::vector<ReflectableUtilUnion> reflectableUtilUnionArray;
    auto reflectablePtr = reflectableArray(reflectableUtilUnionArray);
    std::vector<ReflectableUtilUnion>& reflectableUtilUnionArrayRef =
            ReflectableUtil::getValue<std::vector<ReflectableUtilUnion>>(reflectablePtr);
    ASSERT_EQ(&reflectableUtilUnionArray, &reflectableUtilUnionArrayRef);
}

} // namespace zserio
