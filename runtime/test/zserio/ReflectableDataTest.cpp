#include <string>
#include <type_traits>

#include "gtest/gtest.h"
#include "test_object/std_allocator/ReflectableBitmask.h"
#include "test_object/std_allocator/ReflectableChoice.h"
#include "test_object/std_allocator/ReflectableEnum.h"
#include "test_object/std_allocator/ReflectableObject.h"
#include "test_object/std_allocator/ReflectableUnion.h"
#include "zserio/ArrayTraits.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/ReflectableData.h"
#include "zserio/Vector.h"

using namespace std::literals;

using test_object::std_allocator::ReflectableBitmask;
using test_object::std_allocator::ReflectableChoice;
using test_object::std_allocator::ReflectableEnum;
using test_object::std_allocator::ReflectableNested;
using test_object::std_allocator::ReflectableObject;
using test_object::std_allocator::ReflectableUnion;

namespace zserio
{

namespace detail
{

template <typename FUNC>
struct return_type;

template <typename R, typename CLASS, typename... ARGS>
struct return_type<R (CLASS::*)(ARGS...) const>
{
    using type = R;
};

template <typename FUNC>
using return_type_t = typename return_type<FUNC>::type;

} // namespace detail

class ReflectableDataTest : public ::testing::Test
{
protected:
    template <typename ARRAY>
    void checkArrayAnyValue(const ARRAY& array, const IReflectableDataConstPtr& reflectable)
    {
        ASSERT_EQ(array, reflectable->getAnyValue().template get<std::reference_wrapper<const ARRAY>>().get());
    }

    template <typename ARRAY>
    void checkArrayAnyValue(const ARRAY& array, const IReflectableDataPtr& reflectable)
    {
        ASSERT_EQ(array, reflectable->getAnyValue().template get<std::reference_wrapper<ARRAY>>().get());
    }

    template <typename ARRAY, typename REFLECTABLE_PTR, typename ELEMENT_CHECKER>
    void checkArray(
            const ARRAY& array, const REFLECTABLE_PTR& reflectable, const ELEMENT_CHECKER& elementChecker)
    {
        ASSERT_TRUE(reflectable->isArray());
        ASSERT_EQ(array.size(), reflectable->size());
        for (size_t i = 0; i < array.size(); ++i)
        {
            if (i % 2 == 0)
            {
                elementChecker(array[i], reflectable->at(i));
            }
            else
            {
                elementChecker(array[i], (*reflectable)[i]);
            }
        }

        ASSERT_THROW(reflectable->at(array.size()), CppRuntimeException);
        ASSERT_THROW((*reflectable)[array.size()], CppRuntimeException);

        checkArrayAnyValue(array, reflectable);

        ASSERT_THROW(reflectable->getBool(), CppRuntimeException);
        ASSERT_THROW(reflectable->getInt8(), CppRuntimeException);
        ASSERT_THROW(reflectable->getInt16(), CppRuntimeException);
        ASSERT_THROW(reflectable->getInt32(), CppRuntimeException);
        ASSERT_THROW(reflectable->getInt64(), CppRuntimeException);
        ASSERT_THROW(reflectable->getUInt8(), CppRuntimeException);
        ASSERT_THROW(reflectable->getUInt16(), CppRuntimeException);
        ASSERT_THROW(reflectable->getUInt32(), CppRuntimeException);
        ASSERT_THROW(reflectable->getUInt64(), CppRuntimeException);
        ASSERT_THROW(reflectable->getFloat(), CppRuntimeException);
        ASSERT_THROW(reflectable->getDouble(), CppRuntimeException);
        ASSERT_THROW(reflectable->getBytes(), CppRuntimeException);
        ASSERT_THROW(reflectable->getStringView(), CppRuntimeException);
        ASSERT_THROW(reflectable->getBitBuffer(), CppRuntimeException);

        ASSERT_THROW(reflectable->toInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toUInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toDouble(), CppRuntimeException);
        ASSERT_THROW(reflectable->toString(), CppRuntimeException);

        checkNonCompound(reflectable);
    }

    void checkNonArray(const IReflectableDataConstPtr& reflectable)
    {
        ASSERT_FALSE(reflectable->isArray());
        ASSERT_THROW(reflectable->size(), CppRuntimeException);
        ASSERT_THROW(reflectable->at(0), CppRuntimeException);
        ASSERT_THROW((*reflectable)[0], CppRuntimeException);
    }

    void checkNonArray(const IReflectableDataPtr& reflectable)
    {
        checkNonArray(static_cast<IReflectableDataConstPtr>(reflectable));

        ASSERT_THROW(reflectable->at(0), CppRuntimeException);
        ASSERT_THROW((*reflectable)[0], CppRuntimeException);
        ASSERT_THROW(reflectable->resize(1), CppRuntimeException);
        ASSERT_THROW(reflectable->setAt(Any{}, 0), CppRuntimeException);
    }

    template <typename REFLECTABLE_PTR>
    void checkNonCompoundConstMethods(const REFLECTABLE_PTR& reflectable)
    {
        ASSERT_THROW(reflectable->getField("field"), CppRuntimeException);

        ASSERT_EQ(nullptr, reflectable->find("some.field"));
        ASSERT_EQ(nullptr, (*reflectable)["some.field"]);
    }

    void checkNonCompound(const IReflectableDataPtr& reflectable)
    {
        ASSERT_THROW(reflectable->setField("field", Any{}), CppRuntimeException);
        ASSERT_THROW(reflectable->createField("field"), CppRuntimeException);

        checkNonCompoundConstMethods(reflectable);
    }

    void checkNonCompound(const IReflectableDataConstPtr& reflectable)
    {
        checkNonCompoundConstMethods(reflectable);
    }

    template <typename T, typename REFLECTABLE_PTR, typename GETTER,
            std::enable_if_t<std::is_same_v<typename T::ValueType, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(T value, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_EQ(value, ((*reflectablePtr).*getter)()) << testName << " : " << getterName;
    }

    template <typename T, typename REFLECTABLE_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<typename T::ValueType, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(T, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*reflectablePtr).*getter)(), CppRuntimeException) << testName << " : " << getterName;
    }

    template <typename REFLECTABLE_PTR, typename GETTER,
            std::enable_if_t<std::is_same_v<BytesView, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(BytesView value, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_EQ(value.size(), ((*reflectablePtr).*getter)().size()) << testName << " : " << getterName;
        ASSERT_EQ(value.data(), ((*reflectablePtr).*getter)().data()) << testName << " : " << getterName;
    }

    template <typename REFLECTABLE_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<BytesView, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(BytesView, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*reflectablePtr).*getter)(), CppRuntimeException) << testName << " : " << getterName;
    }

    template <typename REFLECTABLE_PTR, typename GETTER,
            std::enable_if_t<std::is_same_v<std::string_view, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(std::string_view value, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_EQ(value, ((*reflectablePtr).*getter)()) << testName << " : " << getterName;
    }

    template <typename REFLECTABLE_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<std::string_view, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(std::string_view, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*reflectablePtr).*getter)(), CppRuntimeException) << testName << " : " << getterName;
    }

    template <typename REFLECTABLE_PTR, typename GETTER,
            std::enable_if_t<std::is_same_v<const BitBuffer&, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(const BitBuffer& value, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_EQ(value, ((*reflectablePtr).*getter)()) << testName << " : " << getterName;
    }

    template <typename REFLECTABLE_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<const BitBuffer&, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(const BitBuffer&, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*reflectablePtr).*getter)(), CppRuntimeException) << testName << " : " << getterName;
    }

    template <typename T, typename REFLECTABLE_PTR>
    void checkCppTypeGetters(T value, const REFLECTABLE_PTR& reflectablePtr, const char* testName)
    {
        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getBool, testName, "getBool");

        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getInt8, testName, "getInt8");
        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getInt16, testName, "getInt16");
        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getInt32, testName, "getInt32");
        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getInt64, testName, "getInt64");

        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getUInt8, testName, "getUInt8");
        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getUInt16, testName, "getUInt16");
        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getUInt32, testName, "getUInt32");
        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getUInt64, testName, "getUInt64");

        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getFloat, testName, "getFloat");
        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getDouble, testName, "getDouble");

        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getBytes, testName, "getBytes");
        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getStringView, testName, "getStringView");
        checkCppTypeGetter(value, reflectablePtr, &IReflectableData::getBitBuffer, testName, "getBitBuffer");
    }

    template <typename T, typename REFLECTABLE_PTR, typename GETTER>
    void checkFloatingPoint(T value, const REFLECTABLE_PTR& reflectable, const GETTER& getter)
    {
        ASSERT_DOUBLE_EQ(value, ((*reflectable).*getter)());

        ASSERT_DOUBLE_EQ(value, reflectable->toDouble());
        ASSERT_THROW(reflectable->toInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toUInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toString(), CppRuntimeException); // NOT IMPLEMENTED!

        checkCppTypeGetters(value, reflectable, "checkFloatingPoint");

        checkNonCompound(reflectable);
        checkNonArray(reflectable);
    }

    template <typename T, typename REFLECTABLE_PTR, typename GETTER>
    void checkIntegral(
            T value, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter, const char* testName)
    {
        ASSERT_EQ(value, ((*reflectablePtr).*getter)());

        ASSERT_EQ(value, reflectablePtr->getAnyValue().template get<T>());

        ASSERT_DOUBLE_EQ(static_cast<double>(value), reflectablePtr->toDouble());
        ASSERT_EQ(zserio::toString(value), reflectablePtr->toString());

        checkCppTypeGetters(value, reflectablePtr, testName);

        checkNonCompound(reflectablePtr);

        checkNonArray(reflectablePtr);
    }

    template <typename T, typename REFLECTABLE_PTR, typename GETTER>
    void checkSignedIntegral(T value, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter)
    {
        ASSERT_EQ(value, reflectablePtr->toInt());
        ASSERT_THROW(reflectablePtr->toUInt(), CppRuntimeException);

        checkIntegral(value, reflectablePtr, getter, "checkSignedIntegral");
    }

    template <typename T, typename REFLECTABLE_PTR, typename GETTER>
    void checkUnsignedIntegral(T value, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter)
    {
        ASSERT_EQ(value, reflectablePtr->toUInt());
        ASSERT_THROW(reflectablePtr->toInt(), CppRuntimeException);

        checkIntegral(value, reflectablePtr, getter, "checkUnsignedIntegral");
    }

    template <typename REFLECTABLE_PTR>
    void checkString(std::string_view value, const REFLECTABLE_PTR& reflectable)
    {
        ASSERT_EQ(value, reflectable->getAnyValue().template get<std::string_view>());

        ASSERT_EQ(toString(value), reflectable->toString());
        ASSERT_THROW(reflectable->toInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toUInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toDouble(), CppRuntimeException);

        checkCppTypeGetters(value, reflectable, "checkString");

        checkNonCompound(reflectable);
        checkNonArray(reflectable);
    }

    template <typename REFLECTABLE_PTR>
    void checkBitBuffer(const BitBuffer& value, const REFLECTABLE_PTR& reflectable)
    {
        ASSERT_EQ(value,
                reflectable->getAnyValue().template get<std::reference_wrapper<const BitBuffer>>().get());

        ASSERT_THROW(reflectable->toString(), CppRuntimeException);
        ASSERT_THROW(reflectable->toDouble(), CppRuntimeException);
        ASSERT_THROW(reflectable->toInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toUInt(), CppRuntimeException);

        checkCppTypeGetters(value, reflectable, "checkBitBuffer");

        checkNonCompound(reflectable);
        checkNonArray(reflectable);
    }

    template <typename REFLECTABLE_PTR>
    void checkBytes(BytesView value, const REFLECTABLE_PTR& reflectable)
    {
        ASSERT_EQ(value.data(), reflectable->getBytes().data());
        ASSERT_EQ(value.size(), reflectable->getBytes().size());
        auto anyValue = reflectable->getAnyValue().template get<BytesView>();
        ASSERT_EQ(value.data(), anyValue.data());
        ASSERT_EQ(value.size(), anyValue.size());

        ASSERT_THROW(reflectable->toString(), CppRuntimeException);
        ASSERT_THROW(reflectable->toDouble(), CppRuntimeException);
        ASSERT_THROW(reflectable->toInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toUInt(), CppRuntimeException);

        checkCppTypeGetters(value, reflectable, "checkBytes");

        checkNonCompound(reflectable);
        checkNonArray(reflectable);
    }

    template <typename REFLECTABLE_PTR>
    void checkBitmask(ReflectableBitmask bitmask, const REFLECTABLE_PTR& reflectable)
    {
        ASSERT_EQ(bitmask.getValue(), reflectable->getUInt8());

        ASSERT_EQ(bitmask, reflectable->getAnyValue().template get<ReflectableBitmask>());

        ASSERT_EQ(bitmask.getValue(), reflectable->toUInt());
        ASSERT_EQ(bitmask.getValue(), reflectable->toDouble());
        ASSERT_EQ(bitmask.toString(), reflectable->toString());
        ASSERT_THROW(reflectable->toInt(), CppRuntimeException);

        checkCppTypeGetters(bitmask.getValue(), reflectable, "checkBitmask");

        checkNonCompound(reflectable);
        checkNonArray(reflectable);
    }

    template <typename REFLECTABLE_PTR>
    void checkEnum(ReflectableEnum enumeration, const REFLECTABLE_PTR& reflectable)
    {
        ASSERT_EQ(enumToValue(enumeration), reflectable->getInt8());

        ASSERT_EQ(enumeration, reflectable->getAnyValue().template get<ReflectableEnum>());

        ASSERT_EQ(enumToValue(enumeration), reflectable->toInt());
        ASSERT_EQ(enumToValue(enumeration), reflectable->toDouble());
        ASSERT_EQ(enumToString(enumeration), reflectable->toString());
        ASSERT_THROW(reflectable->toUInt(), CppRuntimeException);

        checkCppTypeGetters(enumToValue(enumeration), reflectable, "checkEnum");

        checkNonCompound(reflectable);
        checkNonArray(reflectable);
    }

    void checkCompoundAnyValue(
            const ReflectableObject& reflectableObject, const IReflectableDataConstPtr& reflectable)
    {
        ASSERT_EQ(reflectableObject,
                reflectable->getAnyValue()
                        .template get<std::reference_wrapper<const ReflectableObject>>()
                        .get());
        ASSERT_EQ(reflectableObject.reflectableNested,
                reflectable->getField("reflectableNested")
                        ->getAnyValue()
                        .template get<std::reference_wrapper<const ReflectableNested>>()
                        .get());
    }

    void checkCompoundAnyValue(
            const ReflectableObject& reflectableObject, const IReflectableDataPtr& reflectable)
    {
        ASSERT_EQ(reflectableObject,
                reflectable->getAnyValue().template get<std::reference_wrapper<ReflectableObject>>().get());
        ASSERT_EQ(reflectableObject.reflectableNested,
                reflectable->getField("reflectableNested")
                        ->getAnyValue()
                        .template get<std::reference_wrapper<ReflectableNested>>()
                        .get());
    }

    template <typename REFLECTABLE_PTR>
    void checkCompoundConstMethods(
            const ReflectableObject& reflectableObject, const REFLECTABLE_PTR& reflectable)
    {
        ASSERT_TRUE(TypeInfoUtil::isCompound(reflectable->getTypeInfo().getSchemaType()));

        // field getter
        ASSERT_EQ(reflectableObject.reflectableNested.value,
                reflectable->getField("reflectableNested")->getField("value")->getUInt32());
        ASSERT_THROW(reflectable->getField("nonexistent"), CppRuntimeException);
        ASSERT_THROW(reflectable->getField("reflectableNested")->getField("nonexistent"), CppRuntimeException);

        ASSERT_EQ("valueStr", reflectable->getField("reflectableChoice")->getChoice());
        ASSERT_EQ("value32", reflectable->getField("reflectableUnion")->getChoice());

        // find field
        ASSERT_EQ(reflectableObject.reflectableNested.value,
                reflectable->find("reflectableNested.value")->toUInt());
        ASSERT_EQ(reflectableObject.reflectableNested.value,
                (*reflectable)["reflectableNested.value"]->toDouble());

        // find failed
        ASSERT_EQ(nullptr, reflectable->find("reflectableNested.nonexistent"));
        ASSERT_EQ(nullptr, reflectable->find("nonexistent"));
        ASSERT_EQ(nullptr, reflectable->find("reflectableNested.value.nonexistent"));

        checkCompoundAnyValue(reflectableObject, reflectable);

        ASSERT_THROW(reflectable->getBool(), CppRuntimeException);
        ASSERT_THROW(reflectable->getInt8(), CppRuntimeException);
        ASSERT_THROW(reflectable->getInt16(), CppRuntimeException);
        ASSERT_THROW(reflectable->getInt32(), CppRuntimeException);
        ASSERT_THROW(reflectable->getInt64(), CppRuntimeException);
        ASSERT_THROW(reflectable->getUInt8(), CppRuntimeException);
        ASSERT_THROW(reflectable->getUInt16(), CppRuntimeException);
        ASSERT_THROW(reflectable->getUInt32(), CppRuntimeException);
        ASSERT_THROW(reflectable->getUInt64(), CppRuntimeException);
        ASSERT_THROW(reflectable->getFloat(), CppRuntimeException);
        ASSERT_THROW(reflectable->getDouble(), CppRuntimeException);
        ASSERT_THROW(reflectable->getBytes(), CppRuntimeException);
        ASSERT_THROW(reflectable->getStringView(), CppRuntimeException);
        ASSERT_THROW(reflectable->getBitBuffer(), CppRuntimeException);

        ASSERT_THROW(reflectable->toInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toUInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toDouble(), CppRuntimeException);
        ASSERT_THROW(reflectable->toString(), CppRuntimeException);

        checkNonArray(reflectable);
    }

    void checkCompound(const ReflectableObject& reflectableObject, const IReflectableDataConstPtr& reflectable)
    {
        checkCompoundConstMethods(reflectableObject, reflectable);
    }

    void checkCompound(const ReflectableObject& reflectableObject, const IReflectableDataPtr& reflectable)
    {
        checkCompoundConstMethods(reflectableObject, reflectable);
        checkCompoundConstMethods(reflectableObject, static_cast<IReflectableDataConstPtr>(reflectable));

        // setter
        reflectable->getField("reflectableNested")->setField("value", Any(UInt31(11)));
        ASSERT_EQ(11, reflectableObject.reflectableNested.value);
        reflectable->find("reflectableNested")->setField("value", Any(UInt31(11)));
        ASSERT_THROW(reflectable->setField("nonexistent", Any()), CppRuntimeException);
        ASSERT_THROW(
                reflectable->find("reflectableNested")->setField("nonexistent", Any()), CppRuntimeException);
        reflectable->find("reflectableChoice")->setField("value32", Any(UInt32(42)));
        ASSERT_EQ(reflectableObject.reflectableChoice.index(), ReflectableChoice::Tag::value32);
        reflectable->getField("reflectableUnion")->setField("valueStr", Any(std::string("test")));
        ASSERT_EQ(reflectableObject.reflectableUnion.index(), ReflectableUnion::Tag::valueStr);
        // return back to let checkCompoundConstMethods pass
        reflectable->find("reflectableChoice")->setField("valueStr", Any(std::string("test")));
        reflectable->find("reflectableUnion")->setField("value32", Any(UInt32(13)));

        // any value
        ASSERT_EQ(reflectableObject.reflectableNested,
                reflectable->find("reflectableNested")
                        ->getAnyValue()
                        .template get<std::reference_wrapper<ReflectableNested>>()
                        .get());

        reflectable->createField("reflectableNested");
        ASSERT_EQ(uint32_t(), reflectableObject.reflectableNested.value);

        reflectable->setField("reflectableNested", Any(ReflectableNested{42}));
        ASSERT_EQ(42, reflectableObject.reflectableNested.value);
    }

    static ReflectableObject createReflectableObject(std::string_view stringField, UInt31 value = 13,
            ReflectableEnum reflectableEnum = ReflectableEnum::VALUE1,
            ReflectableUnion::Tag unionTag = ReflectableUnion::Tag::value32)
    {
        ReflectableObject reflectableObject;
        reflectableObject.stringField = stringField;
        reflectableObject.reflectableNested.value = value;
        reflectableObject.reflectableEnum = reflectableEnum;

        switch (reflectableEnum)
        {
        case ReflectableEnum::VALUE1:
            reflectableObject.reflectableChoice.emplace<ReflectableChoice::Tag::valueStr>("test");
            break;
        case ReflectableEnum::VALUE2:
            reflectableObject.reflectableChoice.emplace<ReflectableChoice::Tag::value32>(13);
            break;
        default:
            // empty
            break;
        }

        switch (unionTag)
        {
        case ReflectableUnion::Tag::value32:
            reflectableObject.reflectableUnion.emplace<ReflectableUnion::Tag::value32>(13);
            break;
        case ReflectableUnion::Tag::valueStr:
            reflectableObject.reflectableUnion.emplace<ReflectableUnion::Tag::valueStr>("test");
            break;
        default:
            // shouldn't occur
            break;
        }

        return reflectableObject;
    }
};

TEST_F(ReflectableDataTest, boolReflectable)
{
    const Bool value = true;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getBool);
}

TEST_F(ReflectableDataTest, int8Reflectable)
{
    const Int8 value = -12;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt8);
}

TEST_F(ReflectableDataTest, int16Reflectable)
{
    const Int16 value = -1234;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt16);
}

TEST_F(ReflectableDataTest, int32Reflectable)
{
    const Int32 value = -123456;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt32);
}

TEST_F(ReflectableDataTest, int64Reflectable)
{
    const Int64 value = -1234567890;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt64);
}

TEST_F(ReflectableDataTest, uint8Reflectable)
{
    const UInt8 value = 0xFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt8);
}

TEST_F(ReflectableDataTest, uint16Reflectable)
{
    const UInt16 value = 0xFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt16);
}

TEST_F(ReflectableDataTest, uint32Reflectable)
{
    const UInt32 value = 0xFFFFFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt32);
}

TEST_F(ReflectableDataTest, uint64Reflectable)
{
    const UInt64 value = 0xFFFFFFFFFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt64);
}

TEST_F(ReflectableDataTest, fixedSignedBitField5) // mapped to int8_t
{
    const Int5 value = 15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt8);
}

TEST_F(ReflectableDataTest, fixedSignedBitField15) // mapped to int16_t
{
    const Int15 value = -15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt16);
}

TEST_F(ReflectableDataTest, fixedSignedBitField31) // mapped to int32_t
{
    const Int31 value = -12345678;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt32);
}

TEST_F(ReflectableDataTest, fixedSignedBitField60) // mapped to int64_t
{
    const Int60 value = 1234567890;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt64);
}

TEST_F(ReflectableDataTest, fixedUnsignedBitField7) // mapped to uint8_t
{
    const UInt7 value = 0x2F;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt8);
}

TEST_F(ReflectableDataTest, fixedUnsignedBitField9) // mapped to uint16_t
{
    const UInt9 value = 0x1FF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt16);
}

TEST_F(ReflectableDataTest, fixedUnsignedBitField31) // mapped to uint32_t
{
    const UInt31 value = UINT32_MAX >> 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt32);
}

TEST_F(ReflectableDataTest, fixedUnsignedBitField33) // mapped to uint64_t
{
    const UInt33 value = static_cast<uint64_t>(UINT32_MAX) << 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt64);
}

TEST_F(ReflectableDataTest, dynamicSignedBitField5) // mapped to int8_t
{
    const DynInt8<> value = 15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt8);
}

TEST_F(ReflectableDataTest, dynamicSignedBitField15) // mapped to int16_t
{
    const DynInt16<> value = -15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt16);
}

TEST_F(ReflectableDataTest, dynamicSignedBitField31) // mapped to int32_t
{
    const uint8_t numBits = 31;
    const DynInt32<> value = -12345678;
    const View view(value, numBits);
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt32);
}

TEST_F(ReflectableDataTest, dynamicSignedBitField60) // mapped to int64_t
{
    const uint8_t numBits = 60;
    const DynInt64<> value = 1234567890;
    const View view(value, numBits);
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt64);
}

TEST_F(ReflectableDataTest, dynamicUnsignedBitField7) // mapped to uint8_t
{
    const DynUInt8<> value = 0x2F;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt8);
}

TEST_F(ReflectableDataTest, dynamicUnsignedBitField9) // mapped to uint16_t
{
    const DynUInt16<> value = 0x1FF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt16);
}

TEST_F(ReflectableDataTest, dynamicUnsignedBitField31) // mapped to uint32_t
{
    const DynUInt32<> value = UINT32_MAX >> 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt32);
}

TEST_F(ReflectableDataTest, dynamicUnsignedBitField33) // mapped to uint64_t
{
    const DynUInt64<> value = static_cast<uint64_t>(UINT32_MAX) << 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt64);
}

TEST_F(ReflectableDataTest, varint16Reflectable)
{
    const VarInt16 value = -1234;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt16);
}

TEST_F(ReflectableDataTest, varint32Reflectable)
{
    const VarInt32 value = 54321;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt32);
}

TEST_F(ReflectableDataTest, varint64Reflectable)
{
    const VarInt64 value = -87654321;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt64);
}

TEST_F(ReflectableDataTest, varintReflectable)
{
    const VarInt value = INT64_MAX;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt64);
}

TEST_F(ReflectableDataTest, varuint16Reflectable)
{
    const VarUInt16 value = 1234;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt16);
}

TEST_F(ReflectableDataTest, varuint32Reflectable)
{
    const VarUInt32 value = 0x1FFFFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt32);
}

TEST_F(ReflectableDataTest, varuint64Reflectable)
{
    const VarUInt64 value = 4242424242;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt64);
}

TEST_F(ReflectableDataTest, varuintReflectable)
{
    const VarUInt value = UINT64_MAX;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt64);
}

TEST_F(ReflectableDataTest, varsizeReflectable)
{
    const VarSize value = (UINT32_C(1) << (7U + 7 + 7 + 7 + 3)) - 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt32);
}

TEST_F(ReflectableDataTest, float16Reflectable)
{
    const Float16 value = 2.0F;
    auto reflectablePtr = reflectable(value);
    checkFloatingPoint(value, reflectablePtr, &IReflectableData::getFloat);
}

TEST_F(ReflectableDataTest, float32Reflectable)
{
    const Float32 value = 1.2F;
    auto reflectablePtr = reflectable(value);
    checkFloatingPoint(value, reflectablePtr, &IReflectableData::getFloat);
}

TEST_F(ReflectableDataTest, float64Reflectable)
{
    const Float64 value = 1.2;
    auto reflectablePtr = reflectable(value);
    checkFloatingPoint(value, reflectablePtr, &IReflectableData::getDouble);
}

TEST_F(ReflectableDataTest, bytesReflectable)
{
    const Bytes value{{0, 127, 128, 255}};
    auto reflectablePtr = reflectable(value);
    checkBytes(value, reflectablePtr);
}

TEST_F(ReflectableDataTest, stringReflectable)
{
    const std::string value = "some longer string value to have a chance that some allocation hopefully occurs";
    auto reflectablePtr = reflectable(value);
    checkString(value, reflectablePtr);
}

TEST_F(ReflectableDataTest, stringViewReflectable)
{
    auto view = std::string_view("some text as a string view");
    auto reflectablePtr = reflectable(view);
    checkString(view, reflectablePtr);
}

TEST_F(ReflectableDataTest, bitBufferReflectable)
{
    const BitBuffer value = BitBuffer{std::vector<uint8_t>({0xAB, 0xF0}), 12};
    auto reflectablePtr = reflectable(value);
    checkBitBuffer(value, reflectablePtr);
}

TEST_F(ReflectableDataTest, boolConstArray)
{
    const auto rawArray = std::vector<Bool>({true, false, true, false});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Bool value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getBool);
    });
}

TEST_F(ReflectableDataTest, boolArray)
{
    auto rawArray = std::vector<Bool>({true, false, true, false});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Bool value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getBool);
    });
}

TEST_F(ReflectableDataTest, int8ConstArray)
{
    const auto rawArray = std::vector<Int8>({-10, -20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int8 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8);
    });

    auto nonConstReflectable = std::const_pointer_cast<IReflectableData>(reflectablePtr);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableDataTest, int8Array)
{
    auto rawArray = std::vector<Int8>({-10, -20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int8 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8);
    });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectablePtr),
            [&](Int8 value, const IReflectableDataConstPtr& elementReflectable) {
                checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8);
            });

    reflectablePtr->resize(0);
    ASSERT_EQ(0, reflectablePtr->size());
    reflectablePtr->append(Any(Int8(13)));
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(13, reflectablePtr->at(0)->getInt8());
    reflectablePtr->setAt(Any(Int8(42)), 0);
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(42, reflectablePtr->at(0)->getInt8());
    reflectablePtr->resize(2);
    ASSERT_EQ(2, reflectablePtr->size());

    // out of range
    ASSERT_THROW(reflectablePtr->setAt(Any(Int8(-42)), 2), CppRuntimeException);
}

TEST_F(ReflectableDataTest, int16ConstArray)
{
    const auto rawArray = std::vector<Int16>({-100, -200, 300, 400});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int16 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt16);
    });
}

TEST_F(ReflectableDataTest, int16Array)
{
    auto rawArray = std::vector<Int16>({-100, -200, 300, 400});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](Int16 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt16);
    });
}

TEST_F(ReflectableDataTest, int32ConstArray)
{
    const auto rawArray = std::vector<Int32>({-10000, -20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int32 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt32);
    });
}

TEST_F(ReflectableDataTest, int32Array)
{
    auto rawArray = std::vector<Int32>({-10000, -20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int32 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt32);
    });
}

TEST_F(ReflectableDataTest, int64ConstArray)
{
    const auto rawArray = std::vector<Int64>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](Int64 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64);
    });
}

TEST_F(ReflectableDataTest, int64Array)
{
    auto rawArray = std::vector<Int64>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](Int64 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64);
    });
}

TEST_F(ReflectableDataTest, uint8ConstArray)
{
    const auto rawArray = std::vector<UInt8>({10, 20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt8 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8);
    });
}

TEST_F(ReflectableDataTest, uint8Array)
{
    auto rawArray = std::vector<UInt8>{{10, 20, 30, 40}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt8 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8);
    });
}

TEST_F(ReflectableDataTest, uint16ConstArray)
{
    const auto rawArray = std::vector<UInt16>({100, 200, 300, 400});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt16 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt16);
    });
}

TEST_F(ReflectableDataTest, uint16Array)
{
    auto rawArray = std::vector<UInt16>{{100, 200, 300, 400}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt16 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt16);
    });
}

TEST_F(ReflectableDataTest, uint32ConstArray)
{
    const auto rawArray = std::vector<UInt32>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt32 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32);
    });
}

TEST_F(ReflectableDataTest, uint32Array)
{
    auto rawArray = std::vector<UInt32>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt32 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32);
    });
}

TEST_F(ReflectableDataTest, uint64ConstArray)
{
    const auto rawArray = std::vector<UInt64>({10000000, 20000000, 30000000, 40000000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt64 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64);
    });
}

TEST_F(ReflectableDataTest, uint64Array)
{
    auto rawArray = std::vector<UInt64>{{10000000, 20000000, 30000000, 40000000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt64 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64);
    });
}

TEST_F(ReflectableDataTest, fixedSignedBitField5ConstArray)
{
    const auto rawArray = std::vector<Int5>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int5 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8);
    });
}

TEST_F(ReflectableDataTest, fixedSignedBitField5Array)
{
    auto rawArray = std::vector<Int5>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int5 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8);
    });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectablePtr),
            [&](Int5 value, const IReflectableDataConstPtr& elementReflectable) {
                checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8);
            });

    reflectablePtr->resize(0);
    ASSERT_EQ(0, reflectablePtr->size());
    reflectablePtr->append(Any(Int5(13)));
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(13, reflectablePtr->at(0)->getInt8());
    reflectablePtr->setAt(Any(Int5(42)), 0);
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(42, reflectablePtr->at(0)->getInt8());
    reflectablePtr->resize(2);
    ASSERT_EQ(2, reflectablePtr->size());

    // out of range
    ASSERT_THROW(reflectablePtr->setAt(Any(Int5(42)), 2), CppRuntimeException);
}

TEST_F(ReflectableDataTest, fixedUnsignedBitField5ConstArray)
{
    const auto rawArray = std::vector<UInt5>{{3, 1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt5 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8);
    });
}

TEST_F(ReflectableDataTest, fixedUnsignedBitField5Array)
{
    auto rawArray = std::vector<UInt5>{{3, 1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt5 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8);
    });
}

TEST_F(ReflectableDataTest, dynamicSignedBitField5ConstArray)
{
    const auto rawArray = std::vector<DynInt8<>>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](DynInt8<> value, const IReflectableDataConstPtr& elementReflectable) {
                checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8);
            });

    auto nonConstReflectable = std::const_pointer_cast<IReflectableData>(reflectablePtr);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableDataTest, dynamicSignedBitField5Array)
{
    auto rawArray = std::vector<DynInt8<>>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](DynInt8<> value, const IReflectableDataPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8);
    });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectablePtr),
            [&](DynInt8<> value, const IReflectableDataConstPtr& elementReflectable) {
                checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8);
            });

    reflectablePtr->resize(0);
    ASSERT_EQ(0, reflectablePtr->size());
    reflectablePtr->append(Any(DynInt8<>(13)));
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(13, reflectablePtr->at(0)->getInt8());
    reflectablePtr->setAt(Any(DynInt8<>(42)), 0);
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(42, reflectablePtr->at(0)->getInt8());
    reflectablePtr->resize(2);
    ASSERT_EQ(2, reflectablePtr->size());

    // out of range
    ASSERT_THROW(reflectablePtr->setAt(Any(DynInt8<>(42)), 2), CppRuntimeException);
}

TEST_F(ReflectableDataTest, dynamicUnsignedBitField5ConstArray)
{
    const auto rawArray = std::vector<DynUInt8<>>{{3, 1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](DynUInt8<> value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8);
            });
}

TEST_F(ReflectableDataTest, dynamicUnsignedBitField5Array)
{
    auto rawArray = std::vector<DynUInt8<>>{{3, 1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](DynUInt8<> value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8);
            });
}

TEST_F(ReflectableDataTest, varint16ConstArray)
{
    const auto rawArray = std::vector<VarInt16>({-10, -20, 30, 40});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt16 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt16);
    });
}

TEST_F(ReflectableDataTest, varint16Array)
{
    auto rawArray = std::vector<VarInt16>{{-10, -20, 30, 40}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt16 value, const IReflectableDataPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt16);
    });
}

TEST_F(ReflectableDataTest, varint32ConstArray)
{
    const auto rawArray = std::vector<VarInt32>({-10000, -20000, 30000, 40000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt32 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt32);
    });
}

TEST_F(ReflectableDataTest, varint32Array)
{
    auto rawArray = std::vector<VarInt32>{{-10000, -20000, 30000, 40000}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt32 value, const IReflectableDataPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt32);
    });
}

TEST_F(ReflectableDataTest, varint64ConstArray)
{
    const auto rawArray = std::vector<VarInt64>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt64 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64);
    });
}

TEST_F(ReflectableDataTest, varint64Array)
{
    auto rawArray = std::vector<VarInt64>{{-10000000, -20000000, 30000000, 40000000}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt64 value, const IReflectableDataPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64);
    });
}

TEST_F(ReflectableDataTest, varintConstArray)
{
    const auto rawArray = std::vector<VarInt>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64);
    });
}

TEST_F(ReflectableDataTest, varintArray)
{
    auto rawArray = std::vector<VarInt>{{-10000000, -20000000, 30000000, 40000000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarInt value, const IReflectableDataPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64);
    });
}

TEST_F(ReflectableDataTest, varuint16ConstArray)
{
    const auto rawArray = std::vector<VarUInt16>({10, 20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](VarUInt16 value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt16);
            });
}

TEST_F(ReflectableDataTest, varuint16Array)
{
    auto rawArray = std::vector<VarUInt16>{{10, 20, 30, 40}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt16 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt16);
    });
}

TEST_F(ReflectableDataTest, varuint32ConstArray)
{
    const auto rawArray = std::vector<VarUInt32>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](VarUInt32 value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32);
            });
}

TEST_F(ReflectableDataTest, varuint32Array)
{
    auto rawArray = std::vector<VarUInt32>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt32 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32);
    });
}

TEST_F(ReflectableDataTest, varuint64ConstArray)
{
    const auto rawArray = std::vector<VarUInt64>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](VarUInt64 value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64);
            });
}

TEST_F(ReflectableDataTest, varuint64Array)
{
    auto rawArray = std::vector<VarUInt64>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt64 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64);
    });
}

TEST_F(ReflectableDataTest, varuintConstArray)
{
    const auto rawArray = std::vector<VarUInt64>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](VarUInt64 value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64);
            });
}

TEST_F(ReflectableDataTest, varuintArray)
{
    auto rawArray = std::vector<VarUInt>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64);
    });
}

TEST_F(ReflectableDataTest, varsizeConstArray)
{
    const auto rawArray = std::vector<VarSize>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](VarSize value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32);
            });
}

TEST_F(ReflectableDataTest, varsizeArray)
{
    auto rawArray = std::vector<VarSize>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarSize value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32);
    });
}

TEST_F(ReflectableDataTest, float16ConstArray)
{
    const auto rawArray = std::vector<Float16>{{2.0F, 0.0F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](Float16 value, const IReflectableDataConstPtr& elementReflectable) {
                checkFloatingPoint(value, elementReflectable, &IReflectableData::getFloat);
            });
}

TEST_F(ReflectableDataTest, float16Array)
{
    auto rawArray = std::vector<Float16>{{2.0F, 0.0F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float16 value, const IReflectableDataPtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectableData::getFloat);
    });
}

TEST_F(ReflectableDataTest, float32ConstArray)
{
    const auto rawArray = std::vector<Float32>{{2.0F, 0.0F, 1.2F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](Float32 value, const IReflectableDataConstPtr& elementReflectable) {
                checkFloatingPoint(value, elementReflectable, &IReflectableData::getFloat);
            });
}

TEST_F(ReflectableDataTest, float32Array)
{
    auto rawArray = std::vector<Float32>{{2.0F, 0.0F, 1.2F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float32 value, const IReflectableDataPtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectableData::getFloat);
    });
}

TEST_F(ReflectableDataTest, float64ConstArray)
{
    const auto rawArray = std::vector<Float64>{{2.0, 0.0, 1.2}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](Float64 value, const IReflectableDataConstPtr& elementReflectable) {
                checkFloatingPoint(value, elementReflectable, &IReflectableData::getDouble);
            });
}

TEST_F(ReflectableDataTest, float64Array)
{
    auto rawArray = std::vector<Float64>{{2.0, 0.0, 1.2}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float64 value, const IReflectableDataPtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectableData::getDouble);
    });
}

TEST_F(ReflectableDataTest, stringConstArray)
{
    const auto rawArray = std::vector<std::string>{{"one", "two", "three"}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](std::string_view value, const IReflectableDataConstPtr& elementReflectable) {
                checkString(value, elementReflectable);
            });

    auto nonConstReflectable = std::const_pointer_cast<IReflectableData>(reflectablePtr);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableDataTest, stringArray)
{
    auto rawArray = std::vector<std::string>{{"one", "two", "three"}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectable, [&](std::string_view value, const IReflectableDataPtr& elementReflectable) {
                checkString(value, elementReflectable);
            });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectable),
            [&](std::string_view value, const IReflectableDataConstPtr& elementReflectable) {
                checkString(value, elementReflectable);
            });

    reflectable->resize(0);
    ASSERT_EQ(0, reflectable->size());
    reflectable->append(Any(std::string("appended")));
    ASSERT_EQ(1, reflectable->size());
    ASSERT_EQ("appended"sv, reflectable->at(0)->getStringView());
    reflectable->setAt(Any(std::string("set")), 0);
    ASSERT_EQ(1, reflectable->size());
    ASSERT_EQ("set"sv, reflectable->at(0)->getStringView());
    reflectable->resize(2);
    ASSERT_EQ(2, reflectable->size());

    // out of range
    ASSERT_THROW(reflectable->setAt(Any(std::string("set")), 2), CppRuntimeException);
}

TEST_F(ReflectableDataTest, bitBufferConstArray)
{
    const auto rawArray = std::vector<BitBuffer>{{BitBuffer({0xF8}, 5), BitBuffer({0xAB, 0xCD}, 16)}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable,
            [&](const BitBuffer& value, const IReflectableDataConstPtr& elementReflectable) {
                checkBitBuffer(value, elementReflectable);
            });
}

TEST_F(ReflectableDataTest, bitBufferArray)
{
    auto rawArray = std::vector<BitBuffer>{{BitBuffer({0xF8}, 5), BitBuffer({0xAB, 0xCD}, 16)}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](const BitBuffer& value, const IReflectableDataPtr& elementReflectable) {
                checkBitBuffer(value, elementReflectable);
            });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectablePtr),
            [&](const BitBuffer& value, const IReflectableDataConstPtr& elementReflectable) {
                checkBitBuffer(value, elementReflectable);
            });

    reflectablePtr->resize(0);
    ASSERT_EQ(0, reflectablePtr->size());
    reflectablePtr->append(Any(BitBuffer()));
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(0, reflectablePtr->at(0)->getBitBuffer().getBitSize());
    reflectablePtr->setAt(Any(BitBuffer({0xA0}, 4)), 0);
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(4, reflectablePtr->at(0)->getBitBuffer().getBitSize());
    reflectablePtr->resize(2);
    ASSERT_EQ(2, reflectablePtr->size());

    // out of range
    ASSERT_THROW(reflectablePtr->setAt(Any(BitBuffer()), 2), CppRuntimeException);
}

TEST_F(ReflectableDataTest, bytesConstArray)
{
    const auto rawArray = std::vector<Bytes>{{{{0x00, 0x01}}, {{0xFF, 0xFE}}}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](const Bytes& value, const IReflectableDataConstPtr& elementReflectable) {
                checkBytes(value, elementReflectable);
            });
}

TEST_F(ReflectableDataTest, bytesArray)
{
    auto rawArray = std::vector<Bytes>{{{{0x00, 0x01}}, {{0xFF, 0xFE}}}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](const Bytes& value, const IReflectableDataPtr& elementReflectable) {
                checkBytes(value, elementReflectable);
            });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectablePtr),
            [&](const Bytes& value, const IReflectableDataConstPtr& elementReflectable) {
                checkBytes(value, elementReflectable);
            });

    reflectablePtr->resize(0);
    ASSERT_EQ(0, reflectablePtr->size());
    reflectablePtr->append(Any(Bytes()));
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(0, reflectablePtr->at(0)->getBytes().size());
    reflectablePtr->setAt(Any(Bytes{{0xAB, 0xCD}}), 0);
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(2, reflectablePtr->at(0)->getBytes().size());
    reflectablePtr->resize(2);
    ASSERT_EQ(2, reflectablePtr->size());

    // out of range
    ASSERT_THROW(reflectablePtr->setAt(Any(Bytes()), 2), CppRuntimeException);
}

TEST_F(ReflectableDataTest, bitmaskConstReflectable)
{
    const ReflectableBitmask bitmask = ReflectableBitmask::Values::WRITE;
    auto reflectablePtr = reflectable(bitmask);
    checkBitmask(bitmask, reflectablePtr);
}

TEST_F(ReflectableDataTest, bitmaskReflectable)
{
    ReflectableBitmask bitmask = ReflectableBitmask::Values::WRITE;
    auto reflectablePtr = reflectable(bitmask);
    checkBitmask(bitmask, reflectablePtr);
}

TEST_F(ReflectableDataTest, bitmaskConstArray)
{
    const auto rawArray = std::vector<ReflectableBitmask>{{ReflectableBitmask::Values::WRITE,
            ReflectableBitmask::Values::CREATE, ReflectableBitmask::Values::READ}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](ReflectableBitmask value, const IReflectableDataConstPtr& elementReflectable) {
                checkBitmask(value, elementReflectable);
            });

    auto nonConstReflectable = std::const_pointer_cast<IReflectableData>(reflectablePtr);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableDataTest, bitmaskArray)
{
    auto rawArray = std::vector<ReflectableBitmask>{{ReflectableBitmask::Values::WRITE,
            ReflectableBitmask::Values::CREATE, ReflectableBitmask::Values::READ}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](ReflectableBitmask value, const IReflectableDataPtr& elementReflectable) {
                checkBitmask(value, elementReflectable);
            });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectablePtr),
            [&](ReflectableBitmask value, const IReflectableDataConstPtr& elementReflectable) {
                checkBitmask(value, elementReflectable);
            });

    reflectablePtr->resize(0);
    ASSERT_EQ(0, reflectablePtr->size());
    reflectablePtr->append(Any(ReflectableBitmask(ReflectableBitmask::Values::READ)));
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(ReflectableBitmask::Values::READ, ReflectableBitmask(reflectablePtr->at(0)->getUInt8()));
    reflectablePtr->setAt(Any(ReflectableBitmask(ReflectableBitmask::Values::CREATE)), 0);
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(ReflectableBitmask::Values::CREATE, ReflectableBitmask(reflectablePtr->at(0)->getUInt8()));
    reflectablePtr->resize(2);
    ASSERT_EQ(2, reflectablePtr->size());

    reflectablePtr->append(Any(ReflectableBitmask(ReflectableBitmask::Values::WRITE).getValue()));
    ASSERT_EQ(3, reflectablePtr->size());
    ASSERT_EQ(ReflectableBitmask::Values::WRITE, ReflectableBitmask(reflectablePtr->at(2)->getUInt8()));
    reflectablePtr->setAt(Any(ReflectableBitmask(ReflectableBitmask::Values::READ).getValue()), 2);
    ASSERT_EQ(3, reflectablePtr->size());
    ASSERT_EQ(ReflectableBitmask::Values::READ, ReflectableBitmask(reflectablePtr->at(2)->getUInt8()));

    // out of range
    ASSERT_THROW(reflectablePtr->setAt(Any(ReflectableBitmask::Values::CREATE), 3), CppRuntimeException);
}

TEST_F(ReflectableDataTest, enumReflectable)
{
    const ReflectableEnum enumeration = ReflectableEnum::VALUE1;
    auto reflectablePtr = reflectable(enumeration);
    checkEnum(enumeration, reflectablePtr);
}

TEST_F(ReflectableDataTest, enumConstArray)
{
    const auto rawArray = std::vector<ReflectableEnum>{
            {ReflectableEnum::VALUE1, ReflectableEnum::VALUE2, ReflectableEnum::VALUE3}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](ReflectableEnum value, const IReflectableDataConstPtr& elementReflectable) {
                checkEnum(value, elementReflectable);
            });

    auto nonConstReflectable = std::const_pointer_cast<IReflectableData>(reflectablePtr);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableDataTest, enumArray)
{
    auto rawArray = std::vector<ReflectableEnum>{
            {ReflectableEnum::VALUE1, ReflectableEnum::VALUE2, ReflectableEnum::VALUE3}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](ReflectableEnum value, const IReflectableDataPtr& elementReflectable) {
                checkEnum(value, elementReflectable);
            });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectablePtr),
            [&](ReflectableEnum value, const IReflectableDataConstPtr& elementReflectable) {
                checkEnum(value, elementReflectable);
            });

    reflectablePtr->resize(0);
    ASSERT_EQ(0, reflectablePtr->size());
    reflectablePtr->append(Any(ReflectableEnum(ReflectableEnum::VALUE3)));
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(enumToValue(ReflectableEnum::VALUE3), reflectablePtr->at(0)->getInt8());
    reflectablePtr->setAt(Any(ReflectableEnum(ReflectableEnum::VALUE2)), 0);
    ASSERT_EQ(1, reflectablePtr->size());
    ASSERT_EQ(enumToValue(ReflectableEnum::VALUE2), reflectablePtr->at(0)->getInt8());
    reflectablePtr->resize(2);
    ASSERT_EQ(2, reflectablePtr->size());

    reflectablePtr->append(Any(enumToValue(ReflectableEnum::VALUE1)));
    ASSERT_EQ(3, reflectablePtr->size());
    ASSERT_EQ(enumToValue(ReflectableEnum::VALUE1), reflectablePtr->at(2)->getInt8());
    reflectablePtr->setAt(Any(enumToValue(ReflectableEnum::VALUE2)), 2);
    ASSERT_EQ(3, reflectablePtr->size());
    ASSERT_EQ(enumToValue(ReflectableEnum::VALUE2), reflectablePtr->at(2)->getInt8());

    // out of range
    ASSERT_THROW(reflectablePtr->setAt(Any(ReflectableEnum(ReflectableEnum::VALUE2)), 3), CppRuntimeException);
}

TEST_F(ReflectableDataTest, compoundConst)
{
    const ReflectableObject reflectableObject = createReflectableObject("test", 13);
    auto reflectablePtr = reflectable(reflectableObject);
    checkCompound(reflectableObject, reflectablePtr);

    IReflectableDataPtr nonConstReflectable = std::const_pointer_cast<IReflectableData>(reflectablePtr);
    ASSERT_NO_THROW(reflectablePtr->getField("reflectableNested"));
    ASSERT_THROW(nonConstReflectable->getField("reflectableNested"), CppRuntimeException);
    ASSERT_NO_THROW(reflectablePtr->getAnyValue());
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);

    IReflectableDataConstPtr childReflectable = reflectablePtr->getField("reflectableNested");
    IReflectableDataPtr nonConstChildReflectable = std::const_pointer_cast<IReflectableData>(childReflectable);
    ASSERT_THROW(
            nonConstChildReflectable->setField("value", Any(static_cast<uint32_t>(11))), CppRuntimeException);
}

TEST_F(ReflectableDataTest, compound)
{
    ReflectableObject reflectableObject = createReflectableObject("test", 13);
    auto reflectablePtr = reflectable(reflectableObject);

    checkCompound(reflectableObject, reflectablePtr);
}

TEST_F(ReflectableDataTest, compoundConstArray)
{
    ReflectableObject reflectableObject1 = createReflectableObject("1", 13);
    const auto rawArray =
            std::vector<ReflectableObject>{{reflectableObject1, createReflectableObject("2", 42)}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](const ReflectableObject& value, const IReflectableDataConstPtr& elementReflectable) {
                checkCompound(value, elementReflectable);
            });

    IReflectableDataPtr nonConstReflectable = std::const_pointer_cast<IReflectableData>(reflectablePtr);
    ASSERT_THROW(nonConstReflectable->at(0), CppRuntimeException);
    ASSERT_THROW((*nonConstReflectable)[0], CppRuntimeException);
    ASSERT_THROW(nonConstReflectable->resize(nonConstReflectable->size() + 1), CppRuntimeException);
    ASSERT_THROW(nonConstReflectable->setAt(Any(createReflectableObject("test", 0)), 0), CppRuntimeException);
    ASSERT_THROW(nonConstReflectable->append(Any(createReflectableObject("test", 0))), CppRuntimeException);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableDataTest, compoundArray)
{
    auto rawArray = std::vector<ReflectableObject>{
            {createReflectableObject("1", 13), createReflectableObject("2", 42)}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](const ReflectableObject& value, const IReflectableDataPtr& elementReflectable) {
                checkCompound(value, elementReflectable);
            });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectablePtr),
            [&](const ReflectableObject& value, const IReflectableDataConstPtr& elementReflectable) {
                checkCompound(value, elementReflectable);
            });

    reflectablePtr->resize(reflectablePtr->size() + 1);
    IReflectableDataPtr newCompound = reflectablePtr->at(reflectablePtr->size() - 1);
    ASSERT_TRUE(newCompound);

    reflectablePtr->setAt(Any(createReflectableObject("test", 0)), 0);
    ASSERT_EQ(0, reflectablePtr->at(0)->find("reflectableNested.value")->getUInt32());
    reflectablePtr->append(Any(createReflectableObject("test|", 1)));
    ASSERT_EQ(1, reflectablePtr->at(reflectablePtr->size() - 1)->find("reflectableNested.value")->getUInt32());

    const size_t size = reflectablePtr->size();
    ASSERT_THROW(reflectablePtr->setAt(Any(), size), CppRuntimeException); // out of range
}

TEST_F(ReflectableDataTest, defaultUnimplementedMethods)
{
    class Reflectable : public detail::ReflectableDataBase<std::allocator<uint8_t>>
    {
    public:
        using ReflectableDataBase<std::allocator<uint8_t>>::ReflectableDataBase;
    };

    Reflectable reflectable(typeInfo<UInt8>());
    ASSERT_THROW(reflectable.getAnyValue(), CppRuntimeException);

    const Reflectable& constReflectableRef = reflectable;
    ASSERT_THROW(constReflectableRef.getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableDataTest, reflectableOwner)
{
    auto reflectable = typeInfo<ReflectableObject>().createInstance();

    IReflectableDataConstPtr constReflectable = reflectable;

    ReflectableObject defaultReflectableObject;
    ASSERT_EQ(defaultReflectableObject,
            reflectable->getAnyValue().template get<std::reference_wrapper<ReflectableObject>>().get());
    ASSERT_EQ(defaultReflectableObject,
            constReflectable->getAnyValue().template get<std::reference_wrapper<ReflectableObject>>().get());

    ASSERT_TRUE(TypeInfoUtil::isCompound(reflectable->getTypeInfo().getSchemaType()));
    ASSERT_FALSE(reflectable->isArray());
    reflectable->setField("reflectableNested", Any(ReflectableNested{42}));
    ASSERT_EQ(42, reflectable->getField("reflectableNested")->getField("value")->getUInt32());
    ASSERT_EQ(42, constReflectable->getField("reflectableNested")->getField("value")->getUInt32());
    ASSERT_THROW(reflectable->createField("nonexistent"), CppRuntimeException);
    ASSERT_THROW(reflectable->getChoice(), CppRuntimeException);
    ASSERT_THROW(constReflectable->getChoice(), CppRuntimeException);
    ASSERT_FALSE(reflectable->find("nonexistent"));
    ASSERT_FALSE(constReflectable->find("nonexistent"));
    ASSERT_FALSE((*reflectable)["nonexistent"]);
    ASSERT_FALSE((*constReflectable)["nonexistent"]);

    ASSERT_THROW(reflectable->size(), CppRuntimeException); // not an array
    ASSERT_THROW(reflectable->resize(0), CppRuntimeException); // not an array
    ASSERT_THROW(reflectable->at(0), CppRuntimeException); // not an array
    ASSERT_THROW(constReflectable->at(0), CppRuntimeException); // not an array
    ASSERT_THROW((*reflectable)[0], CppRuntimeException); // not an array
    ASSERT_THROW((*constReflectable)[0], CppRuntimeException); // not an array
    ASSERT_THROW(reflectable->setAt(Any(), 0), CppRuntimeException); // not an array
    ASSERT_THROW(reflectable->append(Any()), CppRuntimeException); // not an array

    ASSERT_THROW(reflectable->getBool(), CppRuntimeException);
    ASSERT_THROW(reflectable->getInt8(), CppRuntimeException);
    ASSERT_THROW(reflectable->getInt16(), CppRuntimeException);
    ASSERT_THROW(reflectable->getInt32(), CppRuntimeException);
    ASSERT_THROW(reflectable->getInt64(), CppRuntimeException);
    ASSERT_THROW(reflectable->getUInt8(), CppRuntimeException);
    ASSERT_THROW(reflectable->getUInt16(), CppRuntimeException);
    ASSERT_THROW(reflectable->getUInt32(), CppRuntimeException);
    ASSERT_THROW(reflectable->getUInt64(), CppRuntimeException);
    ASSERT_THROW(reflectable->getFloat(), CppRuntimeException);
    ASSERT_THROW(reflectable->getDouble(), CppRuntimeException);
    ASSERT_THROW(reflectable->getBytes(), CppRuntimeException);
    ASSERT_THROW(reflectable->getStringView(), CppRuntimeException);
    ASSERT_THROW(reflectable->getBitBuffer(), CppRuntimeException);

    ASSERT_THROW(reflectable->toInt(), CppRuntimeException);
    ASSERT_THROW(reflectable->toUInt(), CppRuntimeException);
    ASSERT_THROW(reflectable->toDouble(), CppRuntimeException);
    ASSERT_THROW(reflectable->toString(), CppRuntimeException);
}

} // namespace zserio
