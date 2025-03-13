#include <functional>
#include <string>
#include <type_traits>

#include "gtest/gtest.h"
#include "zserio/ArrayTraits.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/Reflectable.h"
#include "zserio/Vector.h"

using namespace std::placeholders;
using namespace std::literals;

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
    void checkArrayAnyValue(const ARRAY& array, const IReflectableConstPtr& reflectable)
    {
        ASSERT_EQ(array, reflectable->getAnyValue().template get<std::reference_wrapper<const ARRAY>>().get());
    }

    template <typename ARRAY>
    void checkArrayAnyValue(const ARRAY& array, const IReflectablePtr& reflectable)
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

    void checkNonArray(const IReflectableConstPtr& reflectable)
    {
        ASSERT_FALSE(reflectable->isArray());
        ASSERT_THROW(reflectable->size(), CppRuntimeException);
        ASSERT_THROW(reflectable->at(0), CppRuntimeException);
        ASSERT_THROW((*reflectable)[0], CppRuntimeException);
    }

    void checkNonArray(const IReflectablePtr& reflectable)
    {
        checkNonArray(static_cast<IReflectableConstPtr>(reflectable));

        ASSERT_THROW(reflectable->at(0), CppRuntimeException);
        ASSERT_THROW((*reflectable)[0], CppRuntimeException);
        ASSERT_THROW(reflectable->resize(1), CppRuntimeException);
        ASSERT_THROW(reflectable->setAt(Any{}, 0), CppRuntimeException);
    }

    template <typename REFLECTABLE_PTR>
    void checkNonCompoundConstMethods(const REFLECTABLE_PTR& reflectable)
    {
        ASSERT_THROW(reflectable->getField("field"), CppRuntimeException);

        // TODO[Mi-L@]: Do we need find on reflectable?
        // ASSERT_EQ(nullptr, reflectable->find("some.field"));
        // ASSERT_EQ(nullptr, (*reflectable)["some.field"]);
    }

    void checkNonCompound(const IReflectablePtr& reflectable)
    {
        ASSERT_THROW(reflectable->setField("field", Any{}), CppRuntimeException);
        ASSERT_THROW(reflectable->createField("field"), CppRuntimeException);

        checkNonCompoundConstMethods(reflectable);
    }

    void checkNonCompound(const IReflectableConstPtr& reflectable)
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
        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getBool, testName, "getBool");

        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getInt8, testName, "getInt8");
        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getInt16, testName, "getInt16");
        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getInt32, testName, "getInt32");
        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getInt64, testName, "getInt64");

        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getUInt8, testName, "getUInt8");
        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getUInt16, testName, "getUInt16");
        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getUInt32, testName, "getUInt32");
        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getUInt64, testName, "getUInt64");

        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getFloat, testName, "getFloat");
        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getDouble, testName, "getDouble");

        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getBytes, testName, "getBytes");
        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getStringView, testName, "getStringView");
        checkCppTypeGetter(value, reflectablePtr, &IReflectable::getBitBuffer, testName, "getBitBuffer");
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
    /*
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

        checkWriteRead(
                bitmask, reflectable, [](BitStreamReader& reader) { return ReflectableBitmask(reader); }, 8);
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

        checkWriteRead(
                enumeration, reflectable,
                [](BitStreamReader& reader) { return zserio::read<ReflectableEnum>(reader); }, 8);
    }

    void checkCompoundAnyValue(
            const ReflectableObject& reflectableObject, const IReflectableConstPtr& reflectable)
    {
        ASSERT_EQ(reflectableObject,
                reflectable->getAnyValue()
                        .template get<std::reference_wrapper<const ReflectableObject>>()
                        .get());
        ASSERT_EQ(reflectableObject.getReflectableNested(),
                reflectable->getField("reflectableNested")
                        ->getAnyValue()
                        .template get<std::reference_wrapper<const ReflectableNested>>()
                        .get());
    }

    void checkCompoundAnyValue(const ReflectableObject& reflectableObject, const IReflectablePtr& reflectable)
    {
        ASSERT_EQ(reflectableObject,
                reflectable->getAnyValue().template get<std::reference_wrapper<ReflectableObject>>().get());
        ASSERT_EQ(reflectableObject.getReflectableNested(),
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
        ASSERT_EQ(reflectableObject.getReflectableNested().getValue(),
                reflectable->getField("reflectableNested")->getField("value")->getUInt32());
        ASSERT_THROW(reflectable->getField("nonexistent"), CppRuntimeException);
        ASSERT_THROW(reflectable->getField("reflectableNested")->getField("nonexistent"), CppRuntimeException);

        // find field
        ASSERT_EQ(reflectableObject.getReflectableNested().getValue(),
                reflectable->find("reflectableNested.value")->toUInt());
        ASSERT_EQ(reflectableObject.getReflectableNested().getValue(),
                (*reflectable)["reflectableNested.value"]->toDouble());

        // find parameter
        ASSERT_NO_THROW(reflectable->getField("reflectableNested")->getParameter("dummyParam"));
        ASSERT_EQ(13, (*reflectable)["reflectableNested.dummyParam"]->getInt32());
        ASSERT_NO_THROW(reflectable->getField("reflectableNested")->getParameter("stringParam"));
        ASSERT_EQ(reflectableObject.getStringField(),
                (*reflectable)["reflectableNested.stringParam"]->toString());
        ASSERT_THROW(
                reflectable->getField("reflectableNested")->getParameter("nonexistent"), CppRuntimeException);

        // find function
        ASSERT_EQ(reflectableObject.getReflectableNested().getValue(),
                (*reflectable)["reflectableNested.getValue"]->getUInt32());
        ASSERT_THROW(
                reflectable->getField("reflectableNested")->callFunction("nonexistent"), CppRuntimeException);

        // find failed
        ASSERT_EQ(nullptr, reflectable->find("reflectableNested.nonexistent"));
        ASSERT_EQ(nullptr, reflectable->find("nonexistent"));
        ASSERT_EQ(nullptr, reflectable->find("reflectableNested.value.nonexistent"));
        ASSERT_EQ(nullptr, reflectable->find("reflectableNested.dummyParam.nonexistent"));
        ASSERT_EQ(nullptr, reflectable->find("reflectableNested.getValue.nonexistent"));
        // find failed because the underlying code throws
        ASSERT_EQ(nullptr, reflectable->find("reflectableNested.throwingFunction.nonexistent"));

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

        checkWriteRead(
                reflectableObject, reflectable,
                [](BitStreamReader& reader) { return ReflectableObject(reader); },
                reflectableObject.bitSizeOf());
    }

    void checkCompound(const ReflectableObject& reflectableObject, const IReflectableConstPtr& reflectable)
    {
        checkCompoundConstMethods(reflectableObject, reflectable);
    }

    void checkCompound(const ReflectableObject& reflectableObject, const IReflectablePtr& reflectable)
    {
        checkCompoundConstMethods(reflectableObject, reflectable);
        checkCompoundConstMethods(reflectableObject, static_cast<IReflectableConstPtr>(reflectable));

        // setter
        reflectable->getField("reflectableNested")->setField("value", AnyHolder<>(static_cast<uint32_t>(11)));
        ASSERT_EQ(11, reflectableObject.getReflectableNested().getValue());
        ASSERT_THROW(reflectable->setField("nonexistent", AnyHolder<>()), CppRuntimeException);
        ASSERT_THROW(reflectable->find("reflectableNested")->setField("nonexistent", AnyHolder<>()),
                CppRuntimeException);

        // any value
        ASSERT_EQ(reflectableObject.getReflectableNested(),
                reflectable->find("reflectableNested")
                        ->getAnyValue()
                        .template get<std::reference_wrapper<ReflectableNested>>()
                        .get());

        reflectable->createField("reflectableNested");
        ASSERT_EQ(uint32_t(), reflectableObject.getReflectableNested().getValue());

        reflectable->setField("reflectableNested", AnyHolder<>(ReflectableNested{42}));
        ASSERT_EQ(42, reflectableObject.getReflectableNested().getValue());
        reflectable->initializeChildren(); // keep the reflectable initialized for following tests
    }
    */
};

TEST_F(ReflectableTest, boolReflectable)
{
    const Bool value = true;
    auto reflectableValue = reflectable(value);
    checkUnsignedIntegral(
            value, reflectableValue, &IReflectable::getBool, std::bind(&BitStreamReader::readBool, _1));
}

TEST_F(ReflectableTest, int8Reflectable)
{
    const Int8 value = -12;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, 8));
}

TEST_F(ReflectableTest, int16Reflectable)
{
    const Int16 value = -1234;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, 16));
}

TEST_F(ReflectableTest, int32Reflectable)
{
    const Int32 value = -123456;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, 32));
}

TEST_F(ReflectableTest, int64Reflectable)
{
    const Int64 value = -1234567890;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, 64));
}

TEST_F(ReflectableTest, uint8Reflectable)
{
    const UInt8 value = 0xFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 8));
}

TEST_F(ReflectableTest, uint16Reflectable)
{
    const UInt16 value = 0xFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 16));
}

TEST_F(ReflectableTest, uint32Reflectable)
{
    const UInt32 value = 0xFFFFFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 32));
}

TEST_F(ReflectableTest, uint64Reflectable)
{
    const UInt64 value = 0xFFFFFFFFFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, 64));
}

TEST_F(ReflectableTest, fixedSignedBitField5) // mapped to int8_t
{
    const Int5 value = 15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, 5), 5);
}

TEST_F(ReflectableTest, fixedSignedBitField15) // mapped to int16_t
{
    const Int15 value = -15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, 15), 15);
}

TEST_F(ReflectableTest, fixedSignedBitField31) // mapped to int32_t
{
    const Int31 value = -12345678;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, 31), 31);
}

TEST_F(ReflectableTest, fixedSignedBitField60) // mapped to int64_t
{
    const Int60 value = 1234567890;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, 60), 60);
}

TEST_F(ReflectableTest, fixedUnsignedBitField7) // mapped to uint8_t
{
    const UInt7 value = 0x2F;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 7), 7);
}

TEST_F(ReflectableTest, fixedUnsignedBitField9) // mapped to uint16_t
{
    const UInt9 value = 0x1FF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 9), 9);
}

TEST_F(ReflectableTest, fixedUnsignedBitField31) // mapped to uint32_t
{
    const UInt31 value = UINT32_MAX >> 1;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 31), 31);
}

TEST_F(ReflectableTest, fixedUnsignedBitField33) // mapped to uint64_t
{
    const UInt33 value = static_cast<uint64_t>(UINT32_MAX) << 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, 33), 33);
}

TEST_F(ReflectableTest, dynamicSignedBitField5) // mapped to int8_t
{
    const uint8_t numBits = 5;
    const DynInt8<> value = 15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicSignedBitField15) // mapped to int16_t
{
    const uint8_t numBits = 15;
    const DynInt16<> value = -15;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicSignedBitField31) // mapped to int32_t
{
    const uint8_t numBits = 31;
    const DynInt32<> value = -12345678;
    const View view(value, numBits);
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicSignedBitField60) // mapped to int64_t
{
    const uint8_t numBits = 60;
    const DynInt64<> value = 1234567890;
    const View view(value, numBits);
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicUnsignedBitField7) // mapped to uint8_t
{
    const uint8_t numBits = 7;
    const DynUInt8<> value = 0x2F;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicUnsignedBitField9) // mapped to uint16_t
{
    const uint8_t numBits = 9;
    const DynUInt16<> value = 0x1FF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicUnsignedBitField31) // mapped to uint32_t
{
    const uint8_t numBits = 31;
    const DynUInt32<> value = UINT32_MAX >> 1;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(ReflectableTest, dynamicUnsignedBitField33) // mapped to uint64_t
{
    const uint8_t numBits = 33;
    const DynUInt64<> value = static_cast<uint64_t>(UINT32_MAX) << 1U;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, numBits), numBits);
}

TEST_F(ReflectableTest, varint16Reflectable)
{
    const VarInt16 value = -1234;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt16,
            std::bind(&BitStreamReader::readVarInt16, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varint32Reflectable)
{
    const VarInt32 value = 54321;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt32,
            std::bind(&BitStreamReader::readVarInt32, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varint64Reflectable)
{
    const VarInt64 value = -87654321;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt64,
            std::bind(&BitStreamReader::readVarInt64, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varintReflectable)
{
    const VarInt value = INT64_MAX;
    auto reflectablePtr = reflectable(value);
    checkSignedIntegral(value, reflectablePtr, &IReflectable::getInt64,
            std::bind(&BitStreamReader::readVarInt, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varuint16Reflectable)
{
    const VarUInt16 value = 1234;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt16,
            std::bind(&BitStreamReader::readVarUInt16, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varuint32Reflectable)
{
    const VarUInt32 value = 0x1FFFFFFF;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt32,
            std::bind(&BitStreamReader::readVarUInt32, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varuint64Reflectable)
{
    const VarUInt64 value = 4242424242;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt64,
            std::bind(&BitStreamReader::readVarUInt64, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varuintReflectable)
{
    const VarUInt value = UINT64_MAX;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt64,
            std::bind(&BitStreamReader::readVarUInt, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, varsizeReflectable)
{
    const VarSize value = (UINT32_C(1) << (7 + 7 + 7 + 7 + 3)) - 1;
    auto reflectablePtr = reflectable(value);
    checkUnsignedIntegral(value, reflectablePtr, &IReflectable::getUInt32,
            std::bind(&BitStreamReader::readVarSize, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(ReflectableTest, float16Reflectable)
{
    const Float16 value = 2.0F;
    auto reflectablePtr = reflectable(value);
    checkFloatingPoint(
            value, reflectablePtr, &IReflectable::getFloat, std::bind(&BitStreamReader::readFloat16, _1));
}

TEST_F(ReflectableTest, float32Reflectable)
{
    const Float32 value = 1.2F;
    auto reflectablePtr = reflectable(value);
    checkFloatingPoint(
            value, reflectablePtr, &IReflectable::getFloat, std::bind(&BitStreamReader::readFloat32, _1));
}

TEST_F(ReflectableTest, float64Reflectable)
{
    const Float64 value = 1.2;
    auto reflectablePtr = reflectable(value);
    checkFloatingPoint(
            value, reflectablePtr, &IReflectable::getDouble, std::bind(&BitStreamReader::readFloat64, _1));
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
    checkArray(rawArray, reflectablePtr, [&](Bool value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(
                value, elementReflectable, &IReflectable::getBool, std::bind(&BitStreamReader::readBool, _1));
    });
}

TEST_F(ReflectableTest, boolArray)
{
    auto rawArray = std::vector<Bool>({true, false, true, false});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Bool value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(
                value, elementReflectable, &IReflectable::getBool, std::bind(&BitStreamReader::readBool, _1));
    });
}

TEST_F(ReflectableTest, int8ConstArray)
{
    const auto rawArray = std::vector<Int8>({-10, -20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int8 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt8,
                std::bind(&BitStreamReader::readSignedBits32, _1, 8));
    });

    auto nonConstReflectable = std::const_pointer_cast<IReflectable>(reflectablePtr);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableTest, int8Array)
{
    auto rawArray = std::vector<Int8>({-10, -20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int8 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt8,
                std::bind(&BitStreamReader::readSignedBits32, _1, 8));
    });
    checkArray(rawArray, static_cast<IReflectableConstPtr>(reflectablePtr),
            [&](Int8 value, const IReflectableConstPtr& elementReflectable) {
                checkSignedIntegral(value, elementReflectable, &IReflectable::getInt8,
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
    checkArray(rawArray, reflectablePtr, [&](Int16 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt16,
                std::bind(&BitStreamReader::readSignedBits32, _1, 16));
    });
}

TEST_F(ReflectableTest, int16Array)
{
    auto rawArray = std::vector<Int16>({-100, -200, 300, 400});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](Int16 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt16,
                std::bind(&BitStreamReader::readSignedBits32, _1, 16));
    });
}

TEST_F(ReflectableTest, int32ConstArray)
{
    const auto rawArray = std::vector<Int32>({-10000, -20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int32 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt32,
                std::bind(&BitStreamReader::readSignedBits32, _1, 32));
    });
}

TEST_F(ReflectableTest, int32Array)
{
    auto rawArray = std::vector<Int32>({-10000, -20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int32 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt32,
                std::bind(&BitStreamReader::readSignedBits32, _1, 32));
    });
}

TEST_F(ReflectableTest, int64ConstArray)
{
    const auto rawArray = std::vector<Int64>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](Int64 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt64,
                std::bind(&BitStreamReader::readSignedBits64, _1, 64));
    });
}

TEST_F(ReflectableTest, int64Array)
{
    auto rawArray = std::vector<Int64>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](Int64 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt64,
                std::bind(&BitStreamReader::readSignedBits64, _1, 64));
    });
}

TEST_F(ReflectableTest, uint8ConstArray)
{
    const auto rawArray = std::vector<UInt8>({10, 20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt8 value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt8,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 8));
    });
}

TEST_F(ReflectableTest, uint8Array)
{
    auto rawArray = std::vector<UInt8>{{10, 20, 30, 40}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt8 value, const IReflectablePtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt8,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 8));
    });
}

TEST_F(ReflectableTest, uint16ConstArray)
{
    const auto rawArray = std::vector<UInt16>({100, 200, 300, 400});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt16 value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt16,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 16));
    });
}

TEST_F(ReflectableTest, uint16Array)
{
    auto rawArray = std::vector<UInt16>{{100, 200, 300, 400}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt16 value, const IReflectablePtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt16,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 16));
    });
}

TEST_F(ReflectableTest, uint32ConstArray)
{
    const auto rawArray = std::vector<UInt32>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt32 value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt32,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 32));
    });
}

TEST_F(ReflectableTest, uint32Array)
{
    auto rawArray = std::vector<UInt32>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt32 value, const IReflectablePtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt32,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, 32));
    });
}

TEST_F(ReflectableTest, uint64ConstArray)
{
    const auto rawArray = std::vector<UInt64>({10000000, 20000000, 30000000, 40000000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt64 value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt64,
                std::bind(&BitStreamReader::readUnsignedBits64, _1, 64));
    });
}

TEST_F(ReflectableTest, uint64Array)
{
    auto rawArray = std::vector<UInt64>{{10000000, 20000000, 30000000, 40000000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt64 value, const IReflectablePtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt64,
                std::bind(&BitStreamReader::readUnsignedBits64, _1, 64));
    });
}

TEST_F(ReflectableTest, fixedSignedBitField5ConstArray)
{
    uint8_t numBits = 5;
    const auto rawArray = std::vector<Int5>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int5 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt8,
                std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
    });
}

TEST_F(ReflectableTest, fixedSignedBitField5Array)
{
    uint8_t numBits = 5;
    auto rawArray = std::vector<Int5>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Int5 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt8,
                std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
    });
    checkArray(rawArray, static_cast<IReflectableConstPtr>(reflectablePtr),
            [&](Int5 value, const IReflectableConstPtr& elementReflectable) {
                checkSignedIntegral(value, elementReflectable, &IReflectable::getInt8,
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
    checkArray(rawArray, reflectablePtr, [&](UInt5 value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt8,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
    });
}

TEST_F(ReflectableTest, fixedUnsignedBitField5Array)
{
    const uint8_t numBits = 5;
    auto rawArray = std::vector<UInt5>{{3, 1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](UInt5 value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt8,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
    });
}

TEST_F(ReflectableTest, dynamicSignedBitField5ConstArray)
{
    const uint8_t numBits = 5;
    const auto rawArray = std::vector<DynInt8<>>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](DynInt8<> value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt8,
                std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
    });

    auto nonConstReflectable = std::const_pointer_cast<IReflectable>(reflectablePtr);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableTest, dynamicSignedBitField5Array)
{
    const uint8_t numBits = 5;
    auto rawArray = std::vector<DynInt8<>>{{-3, -1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](DynInt8<> value, const IReflectablePtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt8,
                std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
    });
    checkArray(rawArray, static_cast<IReflectableConstPtr>(reflectablePtr),
            [&](DynInt8<> value, const IReflectableConstPtr& elementReflectable) {
                checkSignedIntegral(value, elementReflectable, &IReflectable::getInt8,
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
    checkArray(rawArray, reflectablePtr, [&](DynUInt8<> value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt8,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
    });
}

TEST_F(ReflectableTest, dynamicUnsignedBitField5Array)
{
    const uint8_t numBits = 5;
    auto rawArray = std::vector<DynUInt8<>>{{3, 1, 2, 4, 6}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](DynUInt8<> value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt8,
                std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
    });
}

TEST_F(ReflectableTest, varint16ConstArray)
{
    const auto rawArray = std::vector<VarInt16>({-10, -20, 30, 40});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt16 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt16,
                std::bind(&BitStreamReader::readVarInt16, _1));
    });
}

TEST_F(ReflectableTest, varint16Array)
{
    auto rawArray = std::vector<VarInt16>{{-10, -20, 30, 40}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt16 value, const IReflectablePtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt16,
                std::bind(&BitStreamReader::readVarInt16, _1));
    });
}

TEST_F(ReflectableTest, varint32ConstArray)
{
    const auto rawArray = std::vector<VarInt32>({-10000, -20000, 30000, 40000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt32 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt32,
                std::bind(&BitStreamReader::readVarInt32, _1));
    });
}

TEST_F(ReflectableTest, varint32Array)
{
    auto rawArray = std::vector<VarInt32>{{-10000, -20000, 30000, 40000}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt32 value, const IReflectablePtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt32,
                std::bind(&BitStreamReader::readVarInt32, _1));
    });
}

TEST_F(ReflectableTest, varint64ConstArray)
{
    const auto rawArray = std::vector<VarInt64>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt64 value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt64,
                std::bind(&BitStreamReader::readVarInt64, _1));
    });
}

TEST_F(ReflectableTest, varint64Array)
{
    auto rawArray = std::vector<VarInt64>{{-10000000, -20000000, 30000000, 40000000}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt64 value, const IReflectablePtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt64,
                std::bind(&BitStreamReader::readVarInt64, _1));
    });
}

TEST_F(ReflectableTest, varintConstArray)
{
    const auto rawArray = std::vector<VarInt>({-10000000, -20000000, 30000000, 40000000});
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](VarInt value, const IReflectableConstPtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt64,
                std::bind(&BitStreamReader::readVarInt, _1));
    });
}

TEST_F(ReflectableTest, varintArray)
{
    auto rawArray = std::vector<VarInt>{{-10000000, -20000000, 30000000, 40000000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarInt value, const IReflectablePtr& elementReflectable) {
        checkSignedIntegral(value, elementReflectable, &IReflectable::getInt64,
                std::bind(&BitStreamReader::readVarInt, _1));
    });
}

TEST_F(ReflectableTest, varuint16ConstArray)
{
    const auto rawArray = std::vector<VarUInt16>({10, 20, 30, 40});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt16 value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt16,
                std::bind(&BitStreamReader::readVarUInt16, _1));
    });
}

TEST_F(ReflectableTest, varuint16Array)
{
    auto rawArray = std::vector<VarUInt16>{{10, 20, 30, 40}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt16 value, const IReflectablePtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt16,
                std::bind(&BitStreamReader::readVarUInt16, _1));
    });
}

TEST_F(ReflectableTest, varuint32ConstArray)
{
    const auto rawArray = std::vector<VarUInt32>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt32 value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt32,
                std::bind(&BitStreamReader::readVarUInt32, _1));
    });
}

TEST_F(ReflectableTest, varuint32Array)
{
    auto rawArray = std::vector<VarUInt32>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt32 value, const IReflectablePtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt32,
                std::bind(&BitStreamReader::readVarUInt32, _1));
    });
}

TEST_F(ReflectableTest, varuint64ConstArray)
{
    const auto rawArray = std::vector<VarUInt64>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt64 value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt64,
                std::bind(&BitStreamReader::readVarUInt64, _1));
    });
}

TEST_F(ReflectableTest, varuint64Array)
{
    auto rawArray = std::vector<VarUInt64>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt64 value, const IReflectablePtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt64,
                std::bind(&BitStreamReader::readVarUInt64, _1));
    });
}

TEST_F(ReflectableTest, varuintConstArray)
{
    const auto rawArray = std::vector<VarUInt64>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt64 value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt64,
                std::bind(&BitStreamReader::readVarUInt, _1));
    });
}

TEST_F(ReflectableTest, varuintArray)
{
    auto rawArray = std::vector<VarUInt>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarUInt value, const IReflectablePtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt64,
                std::bind(&BitStreamReader::readVarUInt, _1));
    });
}

TEST_F(ReflectableTest, varsizeConstArray)
{
    const auto rawArray = std::vector<VarSize>({10000, 20000, 30000, 40000});
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarSize value, const IReflectableConstPtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt32,
                std::bind(&BitStreamReader::readVarSize, _1));
    });
}

TEST_F(ReflectableTest, varsizeArray)
{
    auto rawArray = std::vector<VarSize>{{10000, 20000, 30000, 40000}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](VarSize value, const IReflectablePtr& elementReflectable) {
        checkUnsignedIntegral(value, elementReflectable, &IReflectable::getUInt32,
                std::bind(&BitStreamReader::readVarSize, _1));
    });
}

TEST_F(ReflectableTest, float16ConstArray)
{
    const auto rawArray = std::vector<Float16>{{2.0F, 0.0F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float16 value, const IReflectableConstPtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectable::getFloat,
                std::bind(&BitStreamReader::readFloat16, _1));
    });
}

TEST_F(ReflectableTest, float16Array)
{
    auto rawArray = std::vector<Float16>{{2.0F, 0.0F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float16 value, const IReflectablePtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectable::getFloat,
                std::bind(&BitStreamReader::readFloat16, _1));
    });
}

TEST_F(ReflectableTest, float32ConstArray)
{
    const auto rawArray = std::vector<Float32>{{2.0F, 0.0F, 1.2F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float32 value, const IReflectableConstPtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectable::getFloat,
                std::bind(&BitStreamReader::readFloat32, _1));
    });
}

TEST_F(ReflectableTest, float32Array)
{
    auto rawArray = std::vector<Float32>{{2.0F, 0.0F, 1.2F}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float32 value, const IReflectablePtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectable::getFloat,
                std::bind(&BitStreamReader::readFloat32, _1));
    });
}

TEST_F(ReflectableTest, float64ConstArray)
{
    const auto rawArray = std::vector<Float64>{{2.0, 0.0, 1.2}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float64 value, const IReflectableConstPtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectable::getDouble,
                std::bind(&BitStreamReader::readFloat64, _1));
    });
}

TEST_F(ReflectableTest, float64Array)
{
    auto rawArray = std::vector<Float64>{{2.0, 0.0, 1.2}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](Float64 value, const IReflectablePtr& elementReflectable) {
        checkFloatingPoint(value, elementReflectable, &IReflectable::getDouble,
                std::bind(&BitStreamReader::readFloat64, _1));
    });
}

TEST_F(ReflectableTest, stringConstArray)
{
    const auto rawArray = std::vector<std::string>{{"one", "two", "three"}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr,
            [&](std::string_view value, const IReflectableConstPtr& elementReflectable) {
                checkString(value, elementReflectable);
            });

    auto nonConstReflectable = std::const_pointer_cast<IReflectable>(reflectablePtr);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableTest, stringArray)
{
    auto rawArray = std::vector<std::string>{{"one", "two", "three"}};
    auto reflectable = reflectableArray(rawArray);
    checkArray(rawArray, reflectable, [&](std::string_view value, const IReflectablePtr& elementReflectable) {
        checkString(value, elementReflectable);
    });
    checkArray(rawArray, static_cast<IReflectableConstPtr>(reflectable),
            [&](std::string_view value, const IReflectableConstPtr& elementReflectable) {
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
    checkArray(
            rawArray, reflectable, [&](const BitBuffer& value, const IReflectableConstPtr& elementReflectable) {
                checkBitBuffer(value, elementReflectable);
            });
}

TEST_F(ReflectableTest, bitBufferArray)
{
    auto rawArray = std::vector<BitBuffer>{{BitBuffer({0xF8}, 5), BitBuffer({0xAB, 0xCD}, 16)}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(
            rawArray, reflectablePtr, [&](const BitBuffer& value, const IReflectablePtr& elementReflectable) {
                checkBitBuffer(value, elementReflectable);
            });
    checkArray(rawArray, static_cast<IReflectableConstPtr>(reflectablePtr),
            [&](const BitBuffer& value, const IReflectableConstPtr& elementReflectable) {
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
    checkArray(
            rawArray, reflectablePtr, [&](const Bytes& value, const IReflectableConstPtr& elementReflectable) {
                checkBytes(value, elementReflectable);
            });
}

TEST_F(ReflectableTest, bytesArray)
{
    auto rawArray = std::vector<Bytes>{{{{0x00, 0x01}}, {{0xFF, 0xFE}}}};
    auto reflectablePtr = reflectableArray(rawArray);
    checkArray(rawArray, reflectablePtr, [&](const Bytes& value, const IReflectablePtr& elementReflectable) {
        checkBytes(value, elementReflectable);
    });
    checkArray(rawArray, static_cast<IReflectableConstPtr>(reflectablePtr),
            [&](const Bytes& value, const IReflectableConstPtr& elementReflectable) {
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
/*
TEST_F(ReflectableTest, bitmaskConstReflectable)
{
    const ReflectableBitmask bitmask = ReflectableBitmask::Values::WRITE;
    auto reflectable = bitmask.reflectable();
    checkBitmask(bitmask, reflectable);
}

TEST_F(ReflectableTest, bitmaskReflectable)
{
    ReflectableBitmask bitmask = ReflectableBitmask::Values::WRITE;
    auto reflectable = bitmask.reflectable();
    checkBitmask(bitmask, reflectable);
}

TEST_F(ReflectableTest, bitmaskConstArray)
{
    const auto rawArray = std::vector<ReflectableBitmask>{{ReflectableBitmask::Values::WRITE,
            ReflectableBitmask::Values::CREATE, ReflectableBitmask::Values::READ}};
    auto reflectable = ReflectableFactory::getBitmaskArray(rawArray);
    checkArray(rawArray, reflectable,
            [&](ReflectableBitmask value, const IReflectableConstPtr& elementReflectable) {
                checkBitmask(value, elementReflectable);
            });

    auto nonConstReflectable = std::const_pointer_cast<IReflectable>(reflectable);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableTest, bitmaskArray)
{
    auto rawArray = std::vector<ReflectableBitmask>{{ReflectableBitmask::Values::WRITE,
            ReflectableBitmask::Values::CREATE, ReflectableBitmask::Values::READ}};
    auto reflectable = ReflectableFactory::getBitmaskArray(rawArray);
    checkArray(rawArray, reflectable, [&](ReflectableBitmask value, const IReflectablePtr& elementReflectable) {
        checkBitmask(value, elementReflectable);
    });
    checkArray(rawArray, static_cast<IReflectableConstPtr>(reflectable),
            [&](ReflectableBitmask value, const IReflectableConstPtr& elementReflectable) {
                checkBitmask(value, elementReflectable);
            });

    reflectable->resize(0);
    ASSERT_EQ(0, reflectable->size());
    reflectable->append(AnyHolder<>(ReflectableBitmask(ReflectableBitmask::Values::READ)));
    ASSERT_EQ(1, reflectable->size());
    ASSERT_EQ(ReflectableBitmask::Values::READ, ReflectableBitmask(reflectable->at(0)->getUInt8()));
    reflectable->setAt(AnyHolder<>(ReflectableBitmask(ReflectableBitmask::Values::CREATE)), 0);
    ASSERT_EQ(1, reflectable->size());
    ASSERT_EQ(ReflectableBitmask::Values::CREATE, ReflectableBitmask(reflectable->at(0)->getUInt8()));
    reflectable->resize(2);
    ASSERT_EQ(2, reflectable->size());

    reflectable->append(AnyHolder<>(ReflectableBitmask(ReflectableBitmask::Values::WRITE).getValue()));
    ASSERT_EQ(3, reflectable->size());
    ASSERT_EQ(ReflectableBitmask::Values::WRITE, ReflectableBitmask(reflectable->at(2)->getUInt8()));
    reflectable->setAt(AnyHolder<>(ReflectableBitmask(ReflectableBitmask::Values::READ).getValue()), 2);
    ASSERT_EQ(3, reflectable->size());
    ASSERT_EQ(ReflectableBitmask::Values::READ, ReflectableBitmask(reflectable->at(2)->getUInt8()));

    // out of range
    ASSERT_THROW(reflectable->setAt(AnyHolder<>(ReflectableBitmask::Values::CREATE), 3), CppRuntimeException);
}

TEST_F(ReflectableTest, enumReflectable)
{
    const ReflectableEnum enumeration = ReflectableEnum::VALUE1;
    auto reflectable = enumReflectable(enumeration);
    checkEnum(enumeration, reflectable);
}

TEST_F(ReflectableTest, enumConstArray)
{
    const auto rawArray = std::vector<ReflectableEnum>{
            {ReflectableEnum::VALUE1, ReflectableEnum::VALUE2, ReflectableEnum::VALUE3}};
    auto reflectable = ReflectableFactory::getEnumArray(rawArray);
    checkArray(
            rawArray, reflectable, [&](ReflectableEnum value, const IReflectableConstPtr& elementReflectable) {
                checkEnum(value, elementReflectable);
            });

    auto nonConstReflectable = std::const_pointer_cast<IReflectable>(reflectable);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableTest, enumArray)
{
    auto rawArray = std::vector<ReflectableEnum>{
            {ReflectableEnum::VALUE1, ReflectableEnum::VALUE2, ReflectableEnum::VALUE3}};
    auto reflectable = ReflectableFactory::getEnumArray(rawArray);
    checkArray(rawArray, reflectable, [&](ReflectableEnum value, const IReflectablePtr& elementReflectable) {
        checkEnum(value, elementReflectable);
    });
    checkArray(rawArray, static_cast<IReflectableConstPtr>(reflectable),
            [&](ReflectableEnum value, const IReflectableConstPtr& elementReflectable) {
                checkEnum(value, elementReflectable);
            });

    reflectable->resize(0);
    ASSERT_EQ(0, reflectable->size());
    reflectable->append(Any(ReflectableEnum::VALUE3));
    ASSERT_EQ(1, reflectable->size());
    ASSERT_EQ(enumToValue(ReflectableEnum::VALUE3), reflectable->at(0)->getInt8());
    reflectable->setAt(Any(ReflectableEnum::VALUE2), 0);
    ASSERT_EQ(1, reflectable->size());
    ASSERT_EQ(enumToValue(ReflectableEnum::VALUE2), reflectable->at(0)->getInt8());
    reflectable->resize(2);
    ASSERT_EQ(2, reflectable->size());

    reflectable->append(Any(enumToValue(ReflectableEnum::VALUE1)));
    ASSERT_EQ(3, reflectable->size());
    ASSERT_EQ(enumToValue(ReflectableEnum::VALUE1), reflectable->at(2)->getInt8());
    reflectable->setAt(Any(enumToValue(ReflectableEnum::VALUE2)), 2);
    ASSERT_EQ(3, reflectable->size());
    ASSERT_EQ(enumToValue(ReflectableEnum::VALUE2), reflectable->at(2)->getInt8());

    // out of range
    ASSERT_THROW(reflectable->setAt(Any(ReflectableEnum::VALUE2), 3), CppRuntimeException);
}

TEST_F(ReflectableTest, compoundConst)
{
    {
        const ReflectableObject reflectableObjectUninitialized =
                ReflectableObject{"test", ReflectableNested{13}};
        auto reflectable = reflectableObjectUninitialized.reflectable();
        ASSERT_FALSE(reflectable->find("reflectableNested.stringParam"));
    }

    const ReflectableObject reflectableObject = createInitializedReflectableObject("test", 13);
    auto reflectable = reflectableObject.reflectable();
    checkCompound(reflectableObject, reflectable);

    IReflectablePtr nonConstReflectable = std::const_pointer_cast<IReflectable>(reflectable);
    ASSERT_THROW(nonConstReflectable->initializeChildren(), CppRuntimeException);
    ASSERT_THROW(nonConstReflectable->initialize(vector<AnyHolder<>>()), CppRuntimeException);
    ASSERT_NO_THROW(reflectable->getField("reflectableNested"));
    ASSERT_THROW(nonConstReflectable->getField("reflectableNested"), CppRuntimeException);
    ASSERT_NO_THROW(reflectable->getAnyValue());
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);

    IReflectableConstPtr childReflectable = reflectable->getField("reflectableNested");
    IReflectablePtr nonConstChildReflectable = std::const_pointer_cast<IReflectable>(childReflectable);
    ASSERT_THROW(nonConstChildReflectable->setField("value", AnyHolder<>(static_cast<uint32_t>(11))),
            CppRuntimeException);
    ASSERT_NO_THROW(childReflectable->getParameter("dummyParam"));
    ASSERT_THROW(nonConstChildReflectable->getParameter("dummyParam"), CppRuntimeException);
    ASSERT_NO_THROW(childReflectable->callFunction("getValue"));
    ASSERT_THROW(nonConstChildReflectable->callFunction("getValue"), CppRuntimeException);
    ASSERT_THROW(nonConstChildReflectable->initializeOffsets(0), CppRuntimeException);
}

TEST_F(ReflectableTest, compound)
{
    {
        ReflectableObject reflectableObjectUninitialized = ReflectableObject{"test", ReflectableNested{13}};
        auto reflectable = reflectableObjectUninitialized.reflectable();
        ASSERT_FALSE(reflectable->find("reflectableNested.stringParam"));
    }

    ReflectableObject reflectableObject = ReflectableObject{"test", ReflectableNested{13}};
    auto reflectable = reflectableObject.reflectable();

    // not initialized
    ASSERT_THROW(reflectable->getField("reflectableNested")->getParameter("dummyParam"), CppRuntimeException);
    ASSERT_THROW(reflectable->getField("reflectableNested")->getParameter("stringParam"), CppRuntimeException);
    ASSERT_FALSE(static_cast<IReflectableConstPtr>(reflectable)->find("reflectableNested.stringParam"));

    reflectable->initializeChildren();
    checkCompound(reflectableObject, reflectable);
}

TEST_F(ReflectableTest, compoundConstArray)
{
    ReflectableObject reflectableObject1;
    reflectableObject1 = createInitializedReflectableObject("1", 13); // to cover assignment operator
    const auto rawArray =
            std::vector<ReflectableObject>{{reflectableObject1, createInitializedReflectableObject("2", 42)}};
    auto reflectable = ReflectableFactory::getCompoundArray(rawArray);
    checkArray(rawArray, reflectable,
            [&](const ReflectableObject& value, const IReflectableConstPtr& elementReflectable) {
                checkCompound(value, elementReflectable);
            });

    IReflectablePtr nonConstReflectable = std::const_pointer_cast<IReflectable>(reflectable);
    ASSERT_THROW(nonConstReflectable->at(0), CppRuntimeException);
    ASSERT_THROW((*nonConstReflectable)[0], CppRuntimeException);
    ASSERT_THROW(nonConstReflectable->resize(nonConstReflectable->size() + 1), CppRuntimeException);
    ASSERT_THROW(nonConstReflectable->setAt(AnyHolder<>(ReflectableObject{"test", ReflectableNested{0}}), 0),
            CppRuntimeException);
    ASSERT_THROW(nonConstReflectable->append(AnyHolder<>(ReflectableObject{"test", ReflectableNested{0}})),
            CppRuntimeException);
    ASSERT_THROW(nonConstReflectable->getAnyValue(), CppRuntimeException);
}

TEST_F(ReflectableTest, compoundArray)
{
    auto rawArray = std::vector<ReflectableObject>{
            {createInitializedReflectableObject("1", 13), createInitializedReflectableObject("2", 42)}};
    auto reflectable = ReflectableFactory::getCompoundArray(rawArray);
    checkArray(rawArray, reflectable,
            [&](const ReflectableObject& value, const IReflectablePtr& elementReflectable) {
                checkCompound(value, elementReflectable);
            });
    checkArray(rawArray, static_cast<IReflectableConstPtr>(reflectable),
            [&](const ReflectableObject& value, const IReflectableConstPtr& elementReflectable) {
                checkCompound(value, elementReflectable);
            });

    reflectable->resize(reflectable->size() + 1);
    IReflectablePtr newCompound = reflectable->at(reflectable->size() - 1);
    ASSERT_TRUE(newCompound);

    reflectable->setAt(AnyHolder<>(ReflectableObject{"test", ReflectableNested{0}}), 0);
    ASSERT_EQ(0, reflectable->at(0)->find("reflectableNested.value")->getUInt32());
    reflectable->append(AnyHolder<>(ReflectableObject{"test|", ReflectableNested{1}}));
    ASSERT_EQ(1, reflectable->at(reflectable->size() - 1)->find("reflectableNested.value")->getUInt32());

    const size_t size = reflectable->size();
    ASSERT_THROW(reflectable->setAt(AnyHolder<>(), size), CppRuntimeException); // out of range
}

TEST_F(ReflectableTest, defaultUnimplementedMethods)
{
    class Reflectable : public ReflectableBase<std::allocator<uint8_t>>
    {
    public:
        using ReflectableBase<std::allocator<uint8_t>>::ReflectableBase;
    };

    Reflectable reflectable(BuiltinTypeInfo<>::getUInt8());
    ASSERT_THROW(reflectable.bitSizeOf(0), CppRuntimeException);

    BitBuffer bitBuffer(0);
    BitStreamWriter writer(bitBuffer);
    ASSERT_THROW(reflectable.write(writer), CppRuntimeException);

    ASSERT_THROW(reflectable.getAnyValue(), CppRuntimeException);

    const Reflectable& constReflectableRef = reflectable;
    ASSERT_THROW(constReflectableRef.getAnyValue(), CppRuntimeException);

    ASSERT_THROW(reflectable.parsingInfo(), CppRuntimeException);
}

TEST_F(ReflectableTest, reflectableOwner)
{
    auto reflectable = ReflectableObject::typeInfo().createInstance();

    // must be as the first one to initialize object
    ASSERT_NO_THROW(reflectable->initializeChildren());

    IReflectableConstPtr constReflectable = reflectable;

    // same as default initialized
    ReflectableObject defaultReflectableObject;
    defaultReflectableObject.initializeChildren();
    ASSERT_EQ(defaultReflectableObject,
            reflectable->getAnyValue().template get<std::reference_wrapper<ReflectableObject>>().get());
    ASSERT_EQ(defaultReflectableObject,
            constReflectable->getAnyValue().template get<std::reference_wrapper<ReflectableObject>>().get());

    // has no type arguments
    ASSERT_THROW(reflectable->initialize(std::vector<AnyHolder<>>()), CppRuntimeException);

    ASSERT_TRUE(TypeInfoUtil::isCompound(reflectable->getTypeInfo().getSchemaType()));
    ASSERT_FALSE(reflectable->isArray());
    reflectable->setField("reflectableNested", AnyHolder<>(ReflectableNested{42}));
    ASSERT_EQ(42, reflectable->getField("reflectableNested")->getField("value")->getUInt32());
    ASSERT_EQ(42, constReflectable->getField("reflectableNested")->getField("value")->getUInt32());
    ASSERT_THROW(reflectable->createField("nonexistent"), CppRuntimeException);
    ASSERT_THROW(reflectable->getParameter("nonexistent"), CppRuntimeException);
    ASSERT_THROW(constReflectable->getParameter("nonexistent"), CppRuntimeException);
    ASSERT_THROW(reflectable->callFunction("nonexistent"), CppRuntimeException);
    ASSERT_THROW(constReflectable->callFunction("nonexistent"), CppRuntimeException);
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
    ASSERT_THROW(reflectable->setAt(AnyHolder<>(), 0), CppRuntimeException); // not an array
    ASSERT_THROW(reflectable->append(AnyHolder<>()), CppRuntimeException); // not an array

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

    ASSERT_EQ(reflectable->bitSizeOf(), reflectable->initializeOffsets());

    ASSERT_NO_THROW(reflectable->initializeChildren());

    const size_t bitSizeOfValue = reflectable->bitSizeOf();
    BitBuffer bitBuffer(bitSizeOfValue);
    BitStreamWriter writer(bitBuffer);
    reflectable->write(writer);
    ASSERT_EQ(bitSizeOfValue, writer.getBitPosition());

    const ParsingInfo& parsingInfo = reflectable->parsingInfo();
    ASSERT_THROW(parsingInfo.getBitPosition(), CppRuntimeException);
    ASSERT_THROW(parsingInfo.getBitSize(), CppRuntimeException);
}
*/
} // namespace zserio
