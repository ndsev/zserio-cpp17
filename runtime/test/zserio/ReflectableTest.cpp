#include <functional>
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

using namespace std::placeholders;
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

class ReflectableTest : public ::testing::Test
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

    template <typename T, typename REFLECTABLE_PTR, typename READ_FUNC>
    void checkWriteRead(
            T value, const REFLECTABLE_PTR& reflectable, const READ_FUNC& readFunc, size_t bitBufferSize)
    {
        (void)value;
        (void)reflectable;
        (void)readFunc;
        (void)bitBufferSize;
        /*
        BitBuffer bitBuffer(bitBufferSize);
        BitStreamWriter writer(bitBuffer);
        reflectable->write(writer);
        const size_t bitSizeOfValue = reflectable->bitSizeOf();
        ASSERT_EQ(bitSizeOfValue, writer.getBitPosition());

        BitStreamReader reader(bitBuffer);
        ASSERT_EQ(value, readFunc(reader));
        ASSERT_EQ(bitSizeOfValue, reader.getBitPosition());
        */
    }

    void checkNonArray(const IReflectableDataConstPtr& reflectable)
    {
        ASSERT_FALSE(reflectable->isArray());
        ASSERT_THROW(reflectable->size(), CppRuntimeException);
        ASSERT_THROW(reflectable->at(0), CppRuntimeException);
        // ASSERT_THROW((*reflectable)[0], CppRuntimeException);
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

    template <typename T, typename REFLECTABLE_PTR, typename GETTER, typename READ_FUNC>
    void checkFloatingPoint(T value, const REFLECTABLE_PTR& reflectable, const GETTER& getter,
            const READ_FUNC& readFunc, size_t bitSize = sizeof(T) * 8)
    {
        ASSERT_DOUBLE_EQ(value, ((*reflectable).*getter)());

        ASSERT_DOUBLE_EQ(value, reflectable->toDouble());
        ASSERT_THROW(reflectable->toInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toUInt(), CppRuntimeException);
        ASSERT_THROW(reflectable->toString(), CppRuntimeException); // NOT IMPLEMENTED!

        checkCppTypeGetters(value, reflectable, "checkFloatingPoint");

        checkNonCompound(reflectable);
        checkNonArray(reflectable);

        checkWriteRead(value, reflectable, readFunc, bitSize);
    }

    template <typename T, typename REFLECTABLE_PTR, typename GETTER, typename READ_FUNC>
    void checkIntegral(T value, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter,
            const READ_FUNC& readFunc, size_t bitSize, const char* testName)
    {
        ASSERT_EQ(value, ((*reflectablePtr).*getter)());

        ASSERT_EQ(value, reflectablePtr->getAnyValue().template get<T>());

        ASSERT_DOUBLE_EQ(static_cast<double>(value), reflectablePtr->toDouble());
        ASSERT_EQ(zserio::toString(value), reflectablePtr->toString());

        checkCppTypeGetters(value, reflectablePtr, testName);

        checkNonCompound(reflectablePtr);

        checkNonArray(reflectablePtr);

        checkWriteRead(value, reflectablePtr, readFunc, bitSize);
    }

    template <typename T, typename REFLECTABLE_PTR, typename GETTER, typename READ_FUNC>
    void checkSignedIntegral(T value, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter,
            const READ_FUNC& readFunc, size_t bitSize = sizeof(T) * 8)
    {
        ASSERT_EQ(value, reflectablePtr->toInt());
        ASSERT_THROW(reflectablePtr->toUInt(), CppRuntimeException);

        checkIntegral(value, reflectablePtr, getter, readFunc, bitSize, "checkSignedIntegral");
    }

    template <typename T, typename REFLECTABLE_PTR, typename GETTER, typename READ_FUNC>
    void checkUnsignedIntegral(T value, const REFLECTABLE_PTR& reflectablePtr, const GETTER& getter,
            const READ_FUNC& readFunc, size_t bitSize = sizeof(T) * 8)
    {
        ASSERT_EQ(value, reflectablePtr->toUInt());
        ASSERT_THROW(reflectablePtr->toInt(), CppRuntimeException);

        checkIntegral(value, reflectablePtr, getter, readFunc, bitSize, "checkUnsignedIntegral");
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

        checkWriteRead(toString(value), reflectable,
                std::bind(&BitStreamReader::readString<>, _1, std::allocator<uint8_t>()),
                detail::bitSizeOf(VarSize(convertSizeToUInt32(value.size()))) + value.size() * 8);
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

        checkWriteRead(value, reflectable,
                std::bind(&BitStreamReader::readBitBuffer<>, _1, std::allocator<uint8_t>()),
                detail::bitSizeOf(VarSize(convertSizeToUInt32(value.getBitSize()))) + value.getBitSize());
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

        const size_t bitSize = value.size() * 8;
        checkWriteRead(value, reflectable,
                std::bind(&BitStreamReader::readBytes<>, _1, std::allocator<uint8_t>()),
                detail::bitSizeOf(VarSize(convertSizeToUInt32(bitSize))) + bitSize);
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
        ASSERT_EQ(reflectableObject.reflectableChoice.index(), ReflectableChoice::ChoiceTag::CHOICE_value32);
        reflectable->getField("reflectableUnion")->setField("valueStr", Any(std::string("test")));
        ASSERT_EQ(reflectableObject.reflectableUnion.index(), ReflectableUnion::ChoiceTag::CHOICE_valueStr);
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
            ReflectableUnion::ChoiceTag unionTag = ReflectableUnion::ChoiceTag::CHOICE_value32)
    {
        ReflectableObject reflectableObject;
        reflectableObject.stringField = stringField;
        reflectableObject.reflectableNested.value = value;
        reflectableObject.reflectableEnum = reflectableEnum;

        switch (reflectableEnum)
        {
        case ReflectableEnum::VALUE1:
            reflectableObject.reflectableChoice.emplace<ReflectableChoice::ChoiceTag::CHOICE_valueStr>("test");
            break;
        case ReflectableEnum::VALUE2:
            reflectableObject.reflectableChoice.emplace<ReflectableChoice::ChoiceTag::CHOICE_value32>(13);
            break;
        default:
            // empty
            break;
        }

        switch (unionTag)
        {
        case ReflectableUnion::ChoiceTag::CHOICE_value32:
            reflectableObject.reflectableUnion.emplace<ReflectableUnion::ChoiceTag::CHOICE_value32>(13);
            break;
        case ReflectableUnion::ChoiceTag::CHOICE_valueStr:
            reflectableObject.reflectableUnion.emplace<ReflectableUnion::ChoiceTag::CHOICE_valueStr>("test");
            break;
        default:
            // shouldn't occur
            break;
        }

        return reflectableObject;
    }
};

TEST_F(ReflectableTest, boolReflectable)
{
    const Bool value = true;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(
            value, reflectablePtr, &IReflectableData::getBool, std::bind(&BitStreamReader::readBool, _1));
}

TEST_F(ReflectableTest, int8Reflectable)
{
    const Int8 value = -12;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, 8));
}

TEST_F(ReflectableTest, int16Reflectable)
{
    const Int16 value = -1234;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, 16));
}

TEST_F(ReflectableTest, int32Reflectable)
{
    const Int32 value = -123456;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, 32));
}

TEST_F(ReflectableTest, int64Reflectable)
{
    const Int64 value = -1234567890;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, 64));
}

TEST_F(ReflectableTest, uint8Reflectable)
{
    const UInt8 value = 0xFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 8));
}

TEST_F(ReflectableTest, uint16Reflectable)
{
    const UInt16 value = 0xFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 16));
}

TEST_F(ReflectableTest, uint32Reflectable)
{
    const UInt32 value = 0xFFFFFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 32));
}

TEST_F(ReflectableTest, uint64Reflectable)
{
    const UInt64 value = 0xFFFFFFFFFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, 64));
}

TEST_F(ReflectableTest, fixedSignedBitField5) // mapped to int8_t
{
    const Int5 value = 15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, 5), 5);
}

TEST_F(ReflectableTest, fixedSignedBitField15) // mapped to int16_t
{
    const Int15 value = -15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, 15), 15);
}

TEST_F(ReflectableTest, fixedSignedBitField31) // mapped to int32_t
{
    const Int31 value = -12345678;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, 31), 31);
}

TEST_F(ReflectableTest, fixedSignedBitField60) // mapped to int64_t
{
    const Int60 value = 1234567890;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, 60), 60);
}

TEST_F(ReflectableTest, fixedUnsignedBitField7) // mapped to uint8_t
{
    const UInt7 value = 0x2F;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 7), 7);
}

TEST_F(ReflectableTest, fixedUnsignedBitField9) // mapped to uint16_t
{
    const UInt9 value = 0x1FF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 9), 9);
}

TEST_F(ReflectableTest, fixedUnsignedBitField31) // mapped to uint32_t
{
    const UInt31 value = UINT32_MAX >> 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 31), 31);
}

TEST_F(ReflectableTest, fixedUnsignedBitField33) // mapped to uint64_t
{
    const UInt33 value = static_cast<uint64_t>(UINT32_MAX) << 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, 33), 33);
}

TEST_F(ReflectableTest, dynamicSignedBitField5) // mapped to int8_t
{
    const uint8_t numBits = 5;
    const DynInt8<> value = 15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicSignedBitField15) // mapped to int16_t
{
    const uint8_t numBits = 15;
    const DynInt16<> value = -15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicSignedBitField31) // mapped to int32_t
{
    const uint8_t numBits = 31;
    const DynInt32<> value = -12345678;
    const View view(value, numBits);
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicSignedBitField60) // mapped to int64_t
{
    const uint8_t numBits = 60;
    const DynInt64<> value = 1234567890;
    const View view(value, numBits);
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicUnsignedBitField7) // mapped to uint8_t
{
    const uint8_t numBits = 7;
    const DynUInt8<> value = 0x2F;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicUnsignedBitField9) // mapped to uint16_t
{
    const uint8_t numBits = 9;
    const DynUInt16<> value = 0x1FF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicUnsignedBitField31) // mapped to uint32_t
{
    const uint8_t numBits = 31;
    const DynUInt32<> value = UINT32_MAX >> 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicUnsignedBitField33) // mapped to uint64_t
{
    const uint8_t numBits = 33;
    const DynUInt64<> value = static_cast<uint64_t>(UINT32_MAX) << 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, numBits), numBits);
}

TEST_F(ReflectableTest, varint16Reflectable)
{
    const VarInt16 value = -1234;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt16,
            std::bind(&BitStreamReader::readVarInt16, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varint32Reflectable)
{
    const VarInt32 value = 54321;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt32,
            std::bind(&BitStreamReader::readVarInt32, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varint64Reflectable)
{
    const VarInt64 value = -87654321;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt64,
            std::bind(&BitStreamReader::readVarInt64, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varintReflectable)
{
    const VarInt value = INT64_MAX;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectableData::getInt64,
            std::bind(&BitStreamReader::readVarInt, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varuint16Reflectable)
{
    const VarUInt16 value = 1234;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt16,
            std::bind(&BitStreamReader::readVarUInt16, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varuint32Reflectable)
{
    const VarUInt32 value = 0x1FFFFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt32,
            std::bind(&BitStreamReader::readVarUInt32, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varuint64Reflectable)
{
    const VarUInt64 value = 4242424242;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt64,
            std::bind(&BitStreamReader::readVarUInt64, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varuintReflectable)
{
    const VarUInt value = UINT64_MAX;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt64,
            std::bind(&BitStreamReader::readVarUInt, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varsizeReflectable)
{
    const VarSize value = (UINT32_C(1) << (7U + 7 + 7 + 7 + 3)) - 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectableData::getUInt32,
            std::bind(&BitStreamReader::readVarSize, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, float16Reflectable)
{
    const Float16 value = 2.0F;
    auto reflectablePtr = reflectable(value);
    checkFloatingPoint(
            value, reflectablePtr, &IReflectableData::getFloat, std::bind(&BitStreamReader::readFloat16, _1));
}

TEST_F(ReflectableTest, float32Reflectable)
{
    const Float32 value = 1.2F;
    auto reflectablePtr = reflectable(value);
    checkFloatingPoint(
            value, reflectablePtr, &IReflectableData::getFloat, std::bind(&BitStreamReader::readFloat32, _1));
}

TEST_F(ReflectableTest, float64Reflectable)
{
    const Float64 value = 1.2;
    auto reflectablePtr = reflectable(value);
    checkFloatingPoint(
            value, reflectablePtr, &IReflectableData::getDouble, std::bind(&BitStreamReader::readFloat64, _1));
}

TEST_F(ReflectableTest, bytesReflectable)
{
    const Bytes value{{0, 127, 128, 255}};
    auto reflectablePtr = reflectable(value);
    checkBytes(value, reflectablePtr);
}

TEST_F(ReflectableTest, stringReflectable)
{
    const std::string value = "some longer string value to have a chance that some allocation hopefully occurs";
    auto reflectablePtr = reflectable(value);
    checkString(value, reflectablePtr);
}

TEST_F(ReflectableTest, stringViewReflectable)
{
    auto view = std::string_view("some text as a string view");
    auto reflectablePtr = reflectable(view);
    checkString(view, reflectablePtr);
}

TEST_F(ReflectableTest, bitBufferReflectable)
{
    const BitBuffer value = BitBuffer{std::vector<uint8_t>({0xAB, 0xF0}), 12};
    auto reflectablePtr = reflectable(value);
    checkBitBuffer(value, reflectablePtr);
}

TEST_F(ReflectableTest, boolConstArray)
{
    const auto rawArray = std::vector<Bool>({true, false, true, false});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Bool value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getBool,
                std::bind(&BitStreamReader::readBool, _1));
    });
}

TEST_F(ReflectableTest, boolArray)
{
    auto rawArray = std::vector<Bool>({true, false, true, false});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Bool value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getBool,
                std::bind(&BitStreamReader::readBool, _1));
    });
}

TEST_F(ReflectableTest, int8ConstArray)
{
    const auto rawArray = std::vector<Int8>({-10, -20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int8 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8,
                std::bind(&BitStreamReader::readSignedBits32, _1, 8));
    });

    auto nonConstReflectable = std::const_pointer_cast<IReflectableData>(reflectablePtr);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableTest, int8Array)
{
    auto rawArray = std::vector<Int8>({-10, -20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int8 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8,
                std::bind(&BitStreamReader::readSignedBits32, _1, 8));
    });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectablePtr),
            [&](Int8 value, const IReflectableDataConstPtr& elementReflectable) {
                checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8,
                        std::bind(&BitStreamReader::readSignedBits32, _1, 8));
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

TEST_F(ReflectableTest, int16ConstArray)
{
    const auto rawArray = std::vector<Int16>({-100, -200, 300, 400});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int16 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt16,
                std::bind(&BitStreamReader::readSignedBits32, _1, 16));
    });
}

TEST_F(ReflectableTest, int16Array)
{
    auto rawArray = std::vector<Int16>({-100, -200, 300, 400});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](Int16 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt16,
                std::bind(&BitStreamReader::readSignedBits32, _1, 16));
    });
}

TEST_F(ReflectableTest, int32ConstArray)
{
    const auto rawArray = std::vector<Int32>({-10000, -20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int32 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt32,
                std::bind(&BitStreamReader::readSignedBits32, _1, 32));
    });
}

TEST_F(ReflectableTest, int32Array)
{
    auto rawArray = std::vector<Int32>({-10000, -20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int32 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt32,
                std::bind(&BitStreamReader::readSignedBits32, _1, 32));
    });
}

TEST_F(ReflectableTest, int64ConstArray)
{
    const auto rawArray = std::vector<Int64>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](Int64 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64,
                std::bind(&BitStreamReader::readSignedBits64, _1, 64));
    });
}

TEST_F(ReflectableTest, int64Array)
{
    auto rawArray = std::vector<Int64>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](Int64 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64,
                std::bind(&BitStreamReader::readSignedBits64, _1, 64));
    });
}

TEST_F(ReflectableTest, uint8ConstArray)
{
    const auto rawArray = std::vector<UInt8>({10, 20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt8 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 8));
    });
}

TEST_F(ReflectableTest, uint8Array)
{
    auto rawArray = std::vector<UInt8>{{10, 20, 30, 40}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt8 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 8));
    });
}

TEST_F(ReflectableTest, uint16ConstArray)
{
    const auto rawArray = std::vector<UInt16>({100, 200, 300, 400});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt16 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt16,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 16));
    });
}

TEST_F(ReflectableTest, uint16Array)
{
    auto rawArray = std::vector<UInt16>{{100, 200, 300, 400}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt16 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt16,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 16));
    });
}

TEST_F(ReflectableTest, uint32ConstArray)
{
    const auto rawArray = std::vector<UInt32>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt32 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 32));
    });
}

TEST_F(ReflectableTest, uint32Array)
{
    auto rawArray = std::vector<UInt32>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt32 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 32));
    });
}

TEST_F(ReflectableTest, uint64ConstArray)
{
    const auto rawArray = std::vector<UInt64>({10000000, 20000000, 30000000, 40000000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt64 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64,
                std::bind(&BitStreamReader::readUnsignedBits64, _1, 64));
    });
}

TEST_F(ReflectableTest, uint64Array)
{
    auto rawArray = std::vector<UInt64>{{10000000, 20000000, 30000000, 40000000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt64 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64,
                std::bind(&BitStreamReader::readUnsignedBits64, _1, 64));
    });
}

TEST_F(ReflectableTest, fixedSignedBitField5ConstArray)
{
    uint8_t numBits = 5;
    const auto rawArray = std::vector<Int5>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int5 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8,
                std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
    });
}

TEST_F(ReflectableTest, fixedSignedBitField5Array)
{
    uint8_t numBits = 5;
    auto rawArray = std::vector<Int5>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int5 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8,
                std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
    });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectablePtr),
            [&](Int5 value, const IReflectableDataConstPtr& elementReflectable) {
                checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8,
                        std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
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

TEST_F(ReflectableTest, fixedUnsignedBitField5ConstArray)
{
    const uint8_t numBits = 5;
    const auto rawArray = std::vector<UInt5>{{3, 1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt5 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
    });
}

TEST_F(ReflectableTest, fixedUnsignedBitField5Array)
{
    const uint8_t numBits = 5;
    auto rawArray = std::vector<UInt5>{{3, 1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt5 value, const IReflectableDataConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
    });
}

TEST_F(ReflectableTest, dynamicSignedBitField5ConstArray)
{
    const uint8_t numBits = 5;
    const auto rawArray = std::vector<DynInt8<>>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](DynInt8<> value, const IReflectableDataConstPtr& elementReflectable) {
                checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8,
                        std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
            });

    auto nonConstReflectable = std::const_pointer_cast<IReflectableData>(reflectablePtr);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableTest, dynamicSignedBitField5Array)
{
    const uint8_t numBits = 5;
    auto rawArray = std::vector<DynInt8<>>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](DynInt8<> value, const IReflectableDataPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8,
                std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
    });
    checkArray(rawArray, static_cast<IReflectableDataConstPtr>(reflectablePtr),
            [&](DynInt8<> value, const IReflectableDataConstPtr& elementReflectable) {
                checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt8,
                        std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
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

TEST_F(ReflectableTest, dynamicUnsignedBitField5ConstArray)
{
    const uint8_t numBits = 5;
    const auto rawArray = std::vector<DynUInt8<>>{{3, 1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](DynUInt8<> value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
            });
}

TEST_F(ReflectableTest, dynamicUnsignedBitField5Array)
{
    const uint8_t numBits = 5;
    auto rawArray = std::vector<DynUInt8<>>{{3, 1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](DynUInt8<> value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt8,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
            });
}

TEST_F(ReflectableTest, varint16ConstArray)
{
    const auto rawArray = std::vector<VarInt16>({-10, -20, 30, 40});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt16 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt16,
                std::bind(&BitStreamReader::readVarInt16, _1));
    });
}

TEST_F(ReflectableTest, varint16Array)
{
    auto rawArray = std::vector<VarInt16>{{-10, -20, 30, 40}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt16 value, const IReflectableDataPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt16,
                std::bind(&BitStreamReader::readVarInt16, _1));
    });
}

TEST_F(ReflectableTest, varint32ConstArray)
{
    const auto rawArray = std::vector<VarInt32>({-10000, -20000, 30000, 40000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt32 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt32,
                std::bind(&BitStreamReader::readVarInt32, _1));
    });
}

TEST_F(ReflectableTest, varint32Array)
{
    auto rawArray = std::vector<VarInt32>{{-10000, -20000, 30000, 40000}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt32 value, const IReflectableDataPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt32,
                std::bind(&BitStreamReader::readVarInt32, _1));
    });
}

TEST_F(ReflectableTest, varint64ConstArray)
{
    const auto rawArray = std::vector<VarInt64>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt64 value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64,
                std::bind(&BitStreamReader::readVarInt64, _1));
    });
}

TEST_F(ReflectableTest, varint64Array)
{
    auto rawArray = std::vector<VarInt64>{{-10000000, -20000000, 30000000, 40000000}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt64 value, const IReflectableDataPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64,
                std::bind(&BitStreamReader::readVarInt64, _1));
    });
}

TEST_F(ReflectableTest, varintConstArray)
{
    const auto rawArray = std::vector<VarInt>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt value, const IReflectableDataConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64,
                std::bind(&BitStreamReader::readVarInt, _1));
    });
}

TEST_F(ReflectableTest, varintArray)
{
    auto rawArray = std::vector<VarInt>{{-10000000, -20000000, 30000000, 40000000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarInt value, const IReflectableDataPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectableData::getInt64,
                std::bind(&BitStreamReader::readVarInt, _1));
    });
}

TEST_F(ReflectableTest, varuint16ConstArray)
{
    const auto rawArray = std::vector<VarUInt16>({10, 20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](VarUInt16 value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt16,
                        std::bind(&BitStreamReader::readVarUInt16, _1));
            });
}

TEST_F(ReflectableTest, varuint16Array)
{
    auto rawArray = std::vector<VarUInt16>{{10, 20, 30, 40}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt16 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt16,
                std::bind(&BitStreamReader::readVarUInt16, _1));
    });
}

TEST_F(ReflectableTest, varuint32ConstArray)
{
    const auto rawArray = std::vector<VarUInt32>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](VarUInt32 value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32,
                        std::bind(&BitStreamReader::readVarUInt32, _1));
            });
}

TEST_F(ReflectableTest, varuint32Array)
{
    auto rawArray = std::vector<VarUInt32>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt32 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32,
                std::bind(&BitStreamReader::readVarUInt32, _1));
    });
}

TEST_F(ReflectableTest, varuint64ConstArray)
{
    const auto rawArray = std::vector<VarUInt64>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](VarUInt64 value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64,
                        std::bind(&BitStreamReader::readVarUInt64, _1));
            });
}

TEST_F(ReflectableTest, varuint64Array)
{
    auto rawArray = std::vector<VarUInt64>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt64 value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64,
                std::bind(&BitStreamReader::readVarUInt64, _1));
    });
}

TEST_F(ReflectableTest, varuintConstArray)
{
    const auto rawArray = std::vector<VarUInt64>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](VarUInt64 value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64,
                        std::bind(&BitStreamReader::readVarUInt, _1));
            });
}

TEST_F(ReflectableTest, varuintArray)
{
    auto rawArray = std::vector<VarUInt>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt64,
                std::bind(&BitStreamReader::readVarUInt, _1));
    });
}

TEST_F(ReflectableTest, varsizeConstArray)
{
    const auto rawArray = std::vector<VarSize>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](VarSize value, const IReflectableDataConstPtr& elementReflectable) {
                checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32,
                        std::bind(&BitStreamReader::readVarSize, _1));
            });
}

TEST_F(ReflectableTest, varsizeArray)
{
    auto rawArray = std::vector<VarSize>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarSize value, const IReflectableDataPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectableData::getUInt32,
                std::bind(&BitStreamReader::readVarSize, _1));
    });
}

TEST_F(ReflectableTest, float16ConstArray)
{
    const auto rawArray = std::vector<Float16>{{2.0F, 0.0F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](Float16 value, const IReflectableDataConstPtr& elementReflectable) {
                checkFloatingPoint(value, elementReflectable, &IReflectableData::getFloat,
                        std::bind(&BitStreamReader::readFloat16, _1));
            });
}

TEST_F(ReflectableTest, float16Array)
{
    auto rawArray = std::vector<Float16>{{2.0F, 0.0F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float16 value, const IReflectableDataPtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectableData::getFloat,
                std::bind(&BitStreamReader::readFloat16, _1));
    });
}

TEST_F(ReflectableTest, float32ConstArray)
{
    const auto rawArray = std::vector<Float32>{{2.0F, 0.0F, 1.2F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](Float32 value, const IReflectableDataConstPtr& elementReflectable) {
                checkFloatingPoint(value, elementReflectable, &IReflectableData::getFloat,
                        std::bind(&BitStreamReader::readFloat32, _1));
            });
}

TEST_F(ReflectableTest, float32Array)
{
    auto rawArray = std::vector<Float32>{{2.0F, 0.0F, 1.2F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float32 value, const IReflectableDataPtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectableData::getFloat,
                std::bind(&BitStreamReader::readFloat32, _1));
    });
}

TEST_F(ReflectableTest, float64ConstArray)
{
    const auto rawArray = std::vector<Float64>{{2.0, 0.0, 1.2}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](Float64 value, const IReflectableDataConstPtr& elementReflectable) {
                checkFloatingPoint(value, elementReflectable, &IReflectableData::getDouble,
                        std::bind(&BitStreamReader::readFloat64, _1));
            });
}

TEST_F(ReflectableTest, float64Array)
{
    auto rawArray = std::vector<Float64>{{2.0, 0.0, 1.2}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float64 value, const IReflectableDataPtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectableData::getDouble,
                std::bind(&BitStreamReader::readFloat64, _1));
    });
}

TEST_F(ReflectableTest, stringConstArray)
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

TEST_F(ReflectableTest, stringArray)
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

TEST_F(ReflectableTest, bitBufferConstArray)
{
    const auto rawArray = std::vector<BitBuffer>{{BitBuffer({0xF8}, 5), BitBuffer({0xAB, 0xCD}, 16)}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable,
            [&](const BitBuffer& value, const IReflectableDataConstPtr& elementReflectable) {
                checkBitBuffer(value, elementReflectable);
            });
}

TEST_F(ReflectableTest, bitBufferArray)
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

TEST_F(ReflectableTest, bytesConstArray)
{
    const auto rawArray = std::vector<Bytes>{{{{0x00, 0x01}}, {{0xFF, 0xFE}}}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](const Bytes& value, const IReflectableDataConstPtr& elementReflectable) {
                checkBytes(value, elementReflectable);
            });
}

TEST_F(ReflectableTest, bytesArray)
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

TEST_F(ReflectableTest, bitmaskConstReflectable)
{
    const ReflectableBitmask bitmask = ReflectableBitmask::Values::WRITE;
    auto reflectablePtr = reflectable(bitmask);
    checkBitmask(bitmask, reflectablePtr);
}

TEST_F(ReflectableTest, bitmaskReflectable)
{
    ReflectableBitmask bitmask = ReflectableBitmask::Values::WRITE;
    auto reflectablePtr = reflectable(bitmask);
    checkBitmask(bitmask, reflectablePtr);
}

TEST_F(ReflectableTest, bitmaskConstArray)
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

TEST_F(ReflectableTest, bitmaskArray)
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

TEST_F(ReflectableTest, enumReflectable)
{
    const ReflectableEnum enumeration = ReflectableEnum::VALUE1;
    auto reflectablePtr = reflectable(enumeration);
    checkEnum(enumeration, reflectablePtr);
}

TEST_F(ReflectableTest, enumConstArray)
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

TEST_F(ReflectableTest, enumArray)
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

TEST_F(ReflectableTest, compoundConst)
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

TEST_F(ReflectableTest, compound)
{
    ReflectableObject reflectableObject = createReflectableObject("test", 13);
    auto reflectablePtr = reflectable(reflectableObject);

    checkCompound(reflectableObject, reflectablePtr);
}

TEST_F(ReflectableTest, compoundConstArray)
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

TEST_F(ReflectableTest, compoundArray)
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

TEST_F(ReflectableTest, defaultUnimplementedMethods)
{
    class Reflectable : public ReflectableDataBase<std::allocator<uint8_t>>
    {
    public:
        using ReflectableDataBase<std::allocator<uint8_t>>::ReflectableDataBase;
    };

    Reflectable reflectable(typeInfo<UInt8>());
    ASSERT_THROW(reflectable.getAnyValue(), CppRuntimeException);

    const Reflectable& constReflectableRef = reflectable;
    ASSERT_THROW(constReflectableRef.getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableTest, reflectableOwner)
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
