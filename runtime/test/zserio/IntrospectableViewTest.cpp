#include <functional>

#include "gtest/gtest.h"
#include "test_object/std_allocator/ReflectableBitmask.h"
#include "test_object/std_allocator/ReflectableChoice.h"
#include "test_object/std_allocator/ReflectableEnum.h"
#include "test_object/std_allocator/ReflectableObject.h"
#include "test_object/std_allocator/ReflectableUnion.h"
#include "zserio/ArrayView.h"
#include "zserio/IntrospectableView.h"
#include "zserio/View.h"

using namespace std::placeholders;

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

class IntrospectableViewTest : public ::testing::Test
{
protected:
    template <typename T, typename INTROSPECTABLE_VIEW_PTR, typename READ_FUNC>
    void checkWriteRead(T value, const INTROSPECTABLE_VIEW_PTR& introspectableView, const READ_FUNC& readFunc,
            size_t bitBufferSize)
    {
        BitBuffer bitBuffer(bitBufferSize);
        BitStreamWriter writer(bitBuffer);
        introspectableView->write(writer);
        const size_t bitSizeOfValue = introspectableView->bitSizeOf();
        ASSERT_EQ(bitSizeOfValue, writer.getBitPosition());

        BitStreamReader reader(bitBuffer);
        ASSERT_EQ(value, readFunc(reader));
        ASSERT_EQ(bitSizeOfValue, reader.getBitPosition());
    }

    void checkNonArray(const IIntrospectableViewConstPtr& introspectableView)
    {
        ASSERT_FALSE(introspectableView->isArray());
        ASSERT_THROW(introspectableView->size(), CppRuntimeException);
        ASSERT_THROW(introspectableView->at(0), CppRuntimeException);
        ASSERT_THROW((*introspectableView)[0], CppRuntimeException);
    }

    void checkNonCompound(const IIntrospectableViewConstPtr& introspectableView)
    {
        ASSERT_THROW(introspectableView->getChoice(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getField("field"), CppRuntimeException);
        ASSERT_THROW(introspectableView->getParameter("parameter"), CppRuntimeException);
        ASSERT_THROW(introspectableView->callFunction("function"), CppRuntimeException);
        ASSERT_THROW(introspectableView->initializeOffsets(), CppRuntimeException);

        ASSERT_EQ(nullptr, introspectableView->find("some.field"));
        ASSERT_EQ(nullptr, (*introspectableView)["some.field"]);
    }

    template <typename T, typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<std::is_same_v<typename T::ValueType, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(T value, const INTROSPECTABLE_VIEW_PTR& introspectableView, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_EQ(value, ((*introspectableView).*getter)()) << testName << " : " << getterName;
    }

    template <typename T, typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<typename T::ValueType, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(T, const INTROSPECTABLE_VIEW_PTR& introspectableView, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*introspectableView).*getter)(), CppRuntimeException) << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<std::is_same_v<BytesView, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(BytesView value, const INTROSPECTABLE_VIEW_PTR& introspectableView,
            const GETTER& getter, const char* testName, const char* getterName)
    {
        ASSERT_EQ(value.size(), ((*introspectableView).*getter)().size()) << testName << " : " << getterName;
        ASSERT_EQ(value.data(), ((*introspectableView).*getter)().data()) << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<BytesView, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(BytesView, const INTROSPECTABLE_VIEW_PTR& introspectableView, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*introspectableView).*getter)(), CppRuntimeException) << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<std::is_same_v<std::string_view, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(std::string_view value, const INTROSPECTABLE_VIEW_PTR& introspectableView,
            const GETTER& getter, const char* testName, const char* getterName)
    {
        ASSERT_EQ(value, ((*introspectableView).*getter)()) << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<std::string_view, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(std::string_view, const INTROSPECTABLE_VIEW_PTR& introspectableView,
            const GETTER& getter, const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*introspectableView).*getter)(), CppRuntimeException) << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<std::is_same_v<const BitBuffer&, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(const BitBuffer& value, const INTROSPECTABLE_VIEW_PTR& introspectableView,
            const GETTER& getter, const char* testName, const char* getterName)
    {
        ASSERT_EQ(value, ((*introspectableView).*getter)()) << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<const BitBuffer&, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(const BitBuffer&, const INTROSPECTABLE_VIEW_PTR& introspectableView,
            const GETTER& getter, const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*introspectableView).*getter)(), CppRuntimeException) << testName << " : " << getterName;
    }

    template <typename T, typename INTROSPECTABLE_VIEW_PTR>
    void checkCppTypeGetters(T value, const INTROSPECTABLE_VIEW_PTR& introspectableView, const char* testName)
    {
        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getBool, testName, "getBool");

        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getInt8, testName, "getInt8");
        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getInt16, testName, "getInt16");
        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getInt32, testName, "getInt32");
        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getInt64, testName, "getInt64");

        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getUInt8, testName, "getUInt8");
        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getUInt16, testName, "getUInt16");
        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getUInt32, testName, "getUInt32");
        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getUInt64, testName, "getUInt64");

        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getFloat, testName, "getFloat");
        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getDouble, testName, "getDouble");

        checkCppTypeGetter(value, introspectableView, &IIntrospectableView::getBytes, testName, "getBytes");
        checkCppTypeGetter(
                value, introspectableView, &IIntrospectableView::getStringView, testName, "getStringView");
        checkCppTypeGetter(
                value, introspectableView, &IIntrospectableView::getBitBuffer, testName, "getBitBuffer");
    }

    template <typename T, typename INTROSPECTABLE_VIEW_PTR, typename GETTER, typename READ_FUNC>
    void checkFloatingPoint(T value, const INTROSPECTABLE_VIEW_PTR& introspectableView, const GETTER& getter,
            const READ_FUNC& readFunc, size_t bitSize = sizeof(T) * 8)
    {
        ASSERT_DOUBLE_EQ(value, ((*introspectableView).*getter)());

        ASSERT_DOUBLE_EQ(value, introspectableView->toDouble());
        ASSERT_THROW(introspectableView->toInt(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toUInt(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toString(), CppRuntimeException); // NOT IMPLEMENTED!

        checkCppTypeGetters(value, introspectableView, "checkFloatingPoint");

        checkNonCompound(introspectableView);
        checkNonArray(introspectableView);

        checkWriteRead(value, introspectableView, readFunc, bitSize);
    }

    template <typename T, typename INTROSPECTABLE_VIEW_PTR, typename GETTER, typename READ_FUNC>
    void checkIntegral(T value, const INTROSPECTABLE_VIEW_PTR& introspectableView, const GETTER& getter,
            const READ_FUNC& readFunc, size_t bitSize, const char* testName)
    {
        ASSERT_EQ(value, ((*introspectableView).*getter)());

        ASSERT_EQ(value, introspectableView->getAnyValue().template get<T>());

        ASSERT_DOUBLE_EQ(static_cast<double>(value), introspectableView->toDouble());
        ASSERT_EQ(zserio::toString(value), introspectableView->toString());

        checkCppTypeGetters(value, introspectableView, testName);

        checkNonCompound(introspectableView);

        checkNonArray(introspectableView);

        checkWriteRead(value, introspectableView, readFunc, bitSize);
    }

    template <typename T, typename INTROSPECTABLE_VIEW_PTR, typename GETTER, typename READ_FUNC>
    void checkSignedIntegral(T value, const INTROSPECTABLE_VIEW_PTR& introspectableView, const GETTER& getter,
            const READ_FUNC& readFunc, size_t bitSize = sizeof(T) * 8)
    {
        ASSERT_EQ(value, introspectableView->toInt());
        ASSERT_THROW(introspectableView->toUInt(), CppRuntimeException);

        checkIntegral(value, introspectableView, getter, readFunc, bitSize, "checkSignedIntegral");
    }

    template <typename T, typename INTROSPECTABLE_VIEW_PTR, typename GETTER, typename READ_FUNC>
    void checkUnsignedIntegral(T value, const INTROSPECTABLE_VIEW_PTR& introspectableView, const GETTER& getter,
            const READ_FUNC& readFunc, size_t bitSize = sizeof(T) * 8)
    {
        ASSERT_EQ(value, introspectableView->toUInt());
        ASSERT_THROW(introspectableView->toInt(), CppRuntimeException);

        checkIntegral(value, introspectableView, getter, readFunc, bitSize, "checkUnsignedIntegral");
    }

    template <typename INTROSPECTABLE_VIEW_PTR>
    void checkString(std::string_view value, const INTROSPECTABLE_VIEW_PTR& introspectableView)
    {
        ASSERT_EQ(value, introspectableView->getAnyValue().template get<std::string_view>());

        ASSERT_EQ(toString(value), introspectableView->toString());
        ASSERT_THROW(introspectableView->toInt(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toUInt(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toDouble(), CppRuntimeException);

        checkCppTypeGetters(value, introspectableView, "checkString");

        checkNonCompound(introspectableView);
        checkNonArray(introspectableView);

        checkWriteRead(toString(value), introspectableView,
                std::bind(&BitStreamReader::readString<>, _1, std::allocator<uint8_t>()),
                detail::bitSizeOf(VarSize(convertSizeToUInt32(value.size()))) + value.size() * 8);
    }

    template <typename INTROSPECTABLE_VIEW_PTR>
    void checkBitBuffer(const BitBuffer& value, const INTROSPECTABLE_VIEW_PTR& introspectableView)
    {
        ASSERT_EQ(value,
                introspectableView->getAnyValue()
                        .template get<std::reference_wrapper<const BitBuffer>>()
                        .get());

        ASSERT_THROW(introspectableView->toString(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toDouble(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toInt(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toUInt(), CppRuntimeException);

        checkCppTypeGetters(value, introspectableView, "checkBitBuffer");

        checkNonCompound(introspectableView);
        checkNonArray(introspectableView);

        checkWriteRead(value, introspectableView,
                std::bind(&BitStreamReader::readBitBuffer<>, _1, std::allocator<uint8_t>()),
                detail::bitSizeOf(VarSize(convertSizeToUInt32(value.getBitSize()))) + value.getBitSize());
    }

    template <typename INTROSPECTABLE_VIEW_PTR>
    void checkBytes(BytesView value, const INTROSPECTABLE_VIEW_PTR& introspectableView)
    {
        ASSERT_EQ(value.data(), introspectableView->getBytes().data());
        ASSERT_EQ(value.size(), introspectableView->getBytes().size());
        auto anyValue = introspectableView->getAnyValue().template get<BytesView>();
        ASSERT_EQ(value.data(), anyValue.data());
        ASSERT_EQ(value.size(), anyValue.size());

        ASSERT_THROW(introspectableView->toString(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toDouble(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toInt(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toUInt(), CppRuntimeException);

        checkCppTypeGetters(value, introspectableView, "checkBytes");

        checkNonCompound(introspectableView);
        checkNonArray(introspectableView);

        const size_t bitSize = value.size() * 8;
        checkWriteRead(value, introspectableView,
                std::bind(&BitStreamReader::readBytes<>, _1, std::allocator<uint8_t>()),
                detail::bitSizeOf(VarSize(convertSizeToUInt32(bitSize))) + bitSize);
    }

    template <typename T, typename TRAITS>
    void checkArrayAnyValue(
            const ArrayView<T, TRAITS>& array, const IIntrospectableViewConstPtr& introspectableView)
    {
        auto introArray = introspectableView->getAnyValue().template get<ArrayView<T, TRAITS>>();
        ASSERT_EQ(array.size(), introArray.size());
        for (size_t i = 0; i < array.size(); ++i)
        {
            EXPECT_EQ(array[i], introArray[i]);
        }
    }

    template <typename T, typename TRAITS, typename INTROSPECTABLE_VIEW_PTR, typename ELEMENT_CHECKER>
    void checkArray(const ArrayView<T, TRAITS>& array, const INTROSPECTABLE_VIEW_PTR& introspectableView,
            const ELEMENT_CHECKER& elementChecker)
    {
        ASSERT_TRUE(introspectableView->isArray());
        ASSERT_EQ(array.size(), introspectableView->size());
        for (size_t i = 0; i < array.size(); ++i)
        {
            if (i % 2 == 0)
            {
                elementChecker(array[i], introspectableView->at(i));
            }
            else
            {
                elementChecker(array[i], (*introspectableView)[i]);
            }
        }

        ASSERT_THROW(introspectableView->at(array.size()), CppRuntimeException);
        ASSERT_THROW((*introspectableView)[array.size()], CppRuntimeException);

        checkArrayAnyValue(array, introspectableView);

        ASSERT_THROW(introspectableView->getBool(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getInt8(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getInt16(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getInt32(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getInt64(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getUInt8(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getUInt16(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getUInt32(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getUInt64(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getFloat(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getDouble(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getBytes(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getStringView(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getBitBuffer(), CppRuntimeException);

        ASSERT_THROW(introspectableView->toInt(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toUInt(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toDouble(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toString(), CppRuntimeException);

        ASSERT_THROW(introspectableView->bitSizeOf(0), CppRuntimeException);

        BitBuffer bitBuffer(0);
        BitStreamWriter writer(bitBuffer);
        ASSERT_THROW(introspectableView->write(writer), CppRuntimeException);

        checkNonCompound(introspectableView);
    }

    template <typename INTROSPECTABLE_VIEW_PTR>
    void checkBitmask(ReflectableBitmask bitmask, const INTROSPECTABLE_VIEW_PTR& introspectableView)
    {
        ASSERT_EQ(bitmask.getValue(), introspectableView->getUInt8());

        ASSERT_EQ(bitmask, introspectableView->getAnyValue().template get<ReflectableBitmask>());

        ASSERT_EQ(bitmask.getValue(), introspectableView->toUInt());
        ASSERT_EQ(bitmask.getValue(), introspectableView->toDouble());
        ASSERT_EQ(bitmask.toString(), introspectableView->toString());
        ASSERT_THROW(introspectableView->toInt(), CppRuntimeException);

        checkCppTypeGetters(bitmask.getValue(), introspectableView, "checkBitmask");

        checkNonCompound(introspectableView);
        checkNonArray(introspectableView);

        checkWriteRead(
                bitmask, introspectableView,
                [](BitStreamReader& reader) {
                    ReflectableBitmask readBitmask;
                    detail::read(reader, readBitmask);
                    return readBitmask;
                },
                detail::bitSizeOf(bitmask));
    }

    template <typename INTROSPECTABLE_VIEW_PTR>
    void checkEnum(ReflectableEnum enumeration, const INTROSPECTABLE_VIEW_PTR& introspectableView)
    {
        ASSERT_EQ(enumToValue(enumeration), introspectableView->getInt8());

        ASSERT_EQ(enumeration, introspectableView->getAnyValue().template get<ReflectableEnum>());

        ASSERT_EQ(enumToValue(enumeration), introspectableView->toInt());
        ASSERT_EQ(enumToValue(enumeration), introspectableView->toDouble());
        ASSERT_EQ(enumToString(enumeration), introspectableView->toString());
        ASSERT_THROW(introspectableView->toUInt(), CppRuntimeException);

        checkCppTypeGetters(enumToValue(enumeration), introspectableView, "checkEnum");

        checkNonCompound(introspectableView);
        checkNonArray(introspectableView);

        checkWriteRead(
                enumeration, introspectableView,
                [](BitStreamReader& reader) {
                    ReflectableEnum readEnumeration;
                    detail::read(reader, readEnumeration);
                    return readEnumeration;
                },
                detail::bitSizeOf(enumeration));
    }

    void checkCompoundAnyValue(const View<ReflectableObject>& reflectableObjectView,
            const IIntrospectableViewConstPtr& introspectableView)
    {
        ASSERT_EQ(reflectableObjectView,
                introspectableView->getAnyValue().template get<View<ReflectableObject>>());
        ASSERT_EQ(reflectableObjectView.reflectableNested(),
                introspectableView->getField("reflectableNested")
                        ->getAnyValue()
                        .template get<View<ReflectableNested>>());
    }

    void checkCompound(const View<ReflectableObject>& reflectableObjectView,
            const IIntrospectableViewConstPtr& introspectableView)
    {
        ASSERT_TRUE(TypeInfoUtil::isCompound(introspectableView->getTypeInfo().getSchemaType()));

        // field getter
        ASSERT_EQ(reflectableObjectView.reflectableNested().value(),
                introspectableView->getField("reflectableNested")->getField("value")->getUInt32());
        ASSERT_THROW(introspectableView->getField("nonexistent"), CppRuntimeException);
        ASSERT_THROW(introspectableView->getField("reflectableNested")->getField("nonexistent"),
                CppRuntimeException);

        ASSERT_EQ("valueStr", introspectableView->getField("reflectableChoice")->getChoice());
        ASSERT_EQ("value32", introspectableView->getField("reflectableUnion")->getChoice());

        // find field
        ASSERT_EQ(reflectableObjectView.reflectableNested().value(),
                introspectableView->find("reflectableNested.value")->toUInt());
        ASSERT_EQ(reflectableObjectView.reflectableNested().value(),
                (*introspectableView)["reflectableNested.value"]->toDouble());

        // find failed
        ASSERT_EQ(nullptr, introspectableView->find("reflectableNested.nonexistent"));
        ASSERT_EQ(nullptr, introspectableView->find("nonexistent"));
        ASSERT_EQ(nullptr, introspectableView->find("reflectableNested.value.nonexistent"));

        checkCompoundAnyValue(reflectableObjectView, introspectableView);

        ASSERT_THROW(introspectableView->getBool(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getInt8(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getInt16(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getInt32(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getInt64(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getUInt8(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getUInt16(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getUInt32(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getUInt64(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getFloat(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getDouble(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getBytes(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getStringView(), CppRuntimeException);
        ASSERT_THROW(introspectableView->getBitBuffer(), CppRuntimeException);

        ASSERT_THROW(introspectableView->toInt(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toUInt(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toDouble(), CppRuntimeException);
        ASSERT_THROW(introspectableView->toString(), CppRuntimeException);

        checkNonArray(introspectableView);

        ReflectableObject readReflectableObject;
        checkWriteRead(
                reflectableObjectView, introspectableView,
                [&](BitStreamReader& reader) {
                    return detail::read(reader, readReflectableObject);
                },
                detail::bitSizeOf(reflectableObjectView));
    }

    static ReflectableObject createReflectableObject(std::string_view stringField, UInt31 value = 13,
            ReflectableEnum ReflectableEnum = ReflectableEnum::VALUE1,
            ReflectableUnion::ChoiceTag unionTag = ReflectableUnion::ChoiceTag::CHOICE_value32)
    {
        ReflectableObject reflectableObject;
        reflectableObject.stringField = stringField;
        reflectableObject.reflectableNested.value = value;
        reflectableObject.reflectableEnum = ReflectableEnum;

        switch (ReflectableEnum)
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

TEST_F(IntrospectableViewTest, boolIntrospectableView)
{
    const Bool value = true;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getBool,
            std::bind(&BitStreamReader::readBool, _1));
}

TEST_F(IntrospectableViewTest, int8IntrospectableView)
{
    const Int8 value = -12;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, 8));
}

TEST_F(IntrospectableViewTest, int16IntrospectableView)
{
    const Int16 value = -1234;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, 16));
}

TEST_F(IntrospectableViewTest, int32IntrospectableView)
{
    const Int32 value = -123456;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, 32));
}

TEST_F(IntrospectableViewTest, int64IntrospectableView)
{
    const Int64 value = -1234567890;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, 64));
}

TEST_F(IntrospectableViewTest, uint8IntrospectableView)
{
    const UInt8 value = 0xFF;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 8));
}

TEST_F(IntrospectableViewTest, uint16IntrospectableView)
{
    const UInt16 value = 0xFFFF;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 16));
}

TEST_F(IntrospectableViewTest, uint32IntrospectableView)
{
    const UInt32 value = 0xFFFFFFFF;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 32));
}

TEST_F(IntrospectableViewTest, uint64IntrospectableView)
{
    const UInt64 value = 0xFFFFFFFFFFFF;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, 64));
}

TEST_F(IntrospectableViewTest, fixedSignedBitField5) // mapped to int8_t
{
    const Int5 value = 15;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, 5), 5);
}

TEST_F(IntrospectableViewTest, fixedSignedBitField15) // mapped to int16_t
{
    const Int15 value = -15;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, 15), 15);
}

TEST_F(IntrospectableViewTest, fixedSignedBitField31) // mapped to int32_t
{
    const Int31 value = -12345678;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, 31), 31);
}

TEST_F(IntrospectableViewTest, fixedSignedBitField60) // mapped to int64_t
{
    const Int60 value = 1234567890;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, 60), 60);
}

TEST_F(IntrospectableViewTest, fixedUnsignedBitField7) // mapped to uint8_t
{
    const UInt7 value = 0x2F;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 7), 7);
}

TEST_F(IntrospectableViewTest, fixedUnsignedBitField9) // mapped to uint16_t
{
    const UInt9 value = 0x1FF;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 9), 9);
}

TEST_F(IntrospectableViewTest, fixedUnsignedBitField31) // mapped to uint32_t
{
    const UInt31 value = UINT32_MAX >> 1U;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 31), 31);
}

TEST_F(IntrospectableViewTest, fixedUnsignedBitField33) // mapped to uint64_t
{
    const UInt33 value = static_cast<uint64_t>(UINT32_MAX) << 1U;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, 33), 33);
}

TEST_F(IntrospectableViewTest, dynamicKnownUnsignedBitField5)
{
    constexpr uint8_t NUM_BITS = 5;
    const DynUInt8<NUM_BITS> value = 15;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, NUM_BITS), NUM_BITS);
}

TEST_F(IntrospectableViewTest, dynamicSignedBitField5) // mapped to int8_t
{
    const uint8_t numBits = 5;
    const DynInt8<> value = 15;
    const View view(value, numBits);
    auto introspectableView = introspectable(view);
    checkSignedIntegral(view, introspectableView, &IIntrospectableView::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicSignedBitField15) // mapped to int16_t
{
    const uint8_t numBits = 15;
    const DynInt16<> value = -15;
    const View view(value, numBits);
    auto introspectableView = introspectable(view);
    checkSignedIntegral(view, introspectableView, &IIntrospectableView::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicSignedBitField31) // mapped to int32_t
{
    const uint8_t numBits = 31;
    const DynInt32<> value = -12345678;
    const View view(value, numBits);
    auto introspectableView = introspectable(view);
    checkSignedIntegral(view, introspectableView, &IIntrospectableView::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicSignedBitField60) // mapped to int64_t
{
    const uint8_t numBits = 60;
    const DynInt64<> value = 1234567890;
    const View view(value, numBits);
    auto introspectableView = introspectable(view);
    checkSignedIntegral(view, introspectableView, &IIntrospectableView::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicUnsignedBitField7) // mapped to uint8_t
{
    const uint8_t numBits = 7;
    const DynUInt8<> value = 0x2F;
    const View view(value, numBits);
    auto introspectableView = introspectable(view);
    checkUnsignedIntegral(view, introspectableView, &IIntrospectableView::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicUnsignedBitField9) // mapped to uint16_t
{
    const uint8_t numBits = 9;
    const DynUInt16<> value = 0x1FF;
    const View view(value, numBits);
    auto introspectableView = introspectable(view);
    checkUnsignedIntegral(view, introspectableView, &IIntrospectableView::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicUnsignedBitField31) // mapped to uint32_t
{
    const uint8_t numBits = 31;
    const DynUInt32<> value = UINT32_MAX >> 1U;
    const View view(value, numBits);
    auto introspectableView = introspectable(view);
    checkUnsignedIntegral(view, introspectableView, &IIntrospectableView::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicUnsignedBitField33) // mapped to uint64_t
{
    const uint8_t numBits = 33;
    const DynUInt64<> value = static_cast<uint64_t>(UINT32_MAX) << 1U;
    const View view(value, numBits);
    auto introspectableView = introspectable(view);
    checkUnsignedIntegral(view, introspectableView, &IIntrospectableView::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, varint16IntrospectableView)
{
    const VarInt16 value = -1234;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt16,
            std::bind(&BitStreamReader::readVarInt16, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varint32IntrospectableView)
{
    const VarInt32 value = 54321;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt32,
            std::bind(&BitStreamReader::readVarInt32, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varint64IntrospectableView)
{
    const VarInt64 value = -87654321;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt64,
            std::bind(&BitStreamReader::readVarInt64, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varintIntrospectableView)
{
    const VarInt value = INT64_MAX;
    auto introspectableView = introspectable(value);
    checkSignedIntegral(value, introspectableView, &IIntrospectableView::getInt64,
            std::bind(&BitStreamReader::readVarInt, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varuint16IntrospectableView)
{
    const VarUInt16 value = 1234;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt16,
            std::bind(&BitStreamReader::readVarUInt16, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varuint32IntrospectableView)
{
    const VarUInt32 value = 0x1FFFFFFF;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt32,
            std::bind(&BitStreamReader::readVarUInt32, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varuint64IntrospectableView)
{
    const VarUInt64 value = 4242424242;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt64,
            std::bind(&BitStreamReader::readVarUInt64, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varuintIntrospectableView)
{
    const VarUInt value = UINT64_MAX;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt64,
            std::bind(&BitStreamReader::readVarUInt, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varsizeIntrospectableView)
{
    const VarSize value = (UINT32_C(1) << (7U + 7 + 7 + 7 + 3)) - 1U;
    auto introspectableView = introspectable(value);
    checkUnsignedIntegral(value, introspectableView, &IIntrospectableView::getUInt32,
            std::bind(&BitStreamReader::readVarSize, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, float16IntrospectableView)
{
    const Float16 value = 2.0F;
    auto introspectableView = introspectable(value);
    checkFloatingPoint(value, introspectableView, &IIntrospectableView::getFloat,
            std::bind(&BitStreamReader::readFloat16, _1));
}

TEST_F(IntrospectableViewTest, float32IntrospectableView)
{
    const Float32 value = 1.2F;
    auto introspectableView = introspectable(value);
    checkFloatingPoint(value, introspectableView, &IIntrospectableView::getFloat,
            std::bind(&BitStreamReader::readFloat32, _1));
}

TEST_F(IntrospectableViewTest, float64IntrospectableView)
{
    const Float64 value = 1.2;
    auto introspectableView = introspectable(value);
    checkFloatingPoint(value, introspectableView, &IIntrospectableView::getDouble,
            std::bind(&BitStreamReader::readFloat64, _1));
}

TEST_F(IntrospectableViewTest, bytesIntrospectableView)
{
    const Bytes value{{0, 127, 128, 255}};
    auto introspectableView = introspectable(BytesView(value.data(), value.size()));
    checkBytes(value, introspectableView);
}

TEST_F(IntrospectableViewTest, stringIntrospectableView)
{
    auto value = std::string_view("some text as a string view");
    auto introspectableView = introspectable(value);
    checkString(value, introspectableView);
}

TEST_F(IntrospectableViewTest, bitBufferIntrospectableView)
{
    const BitBuffer value = BitBuffer{std::vector<uint8_t>({0xAB, 0xF0}), 12};
    const BitBufferView view = value;
    auto introspectableView = introspectable(view);
    checkBitBuffer(value, introspectableView);
}

TEST_F(IntrospectableViewTest, boolConstArray)
{
    const auto rawArray = std::vector<Bool>({true, false, true, false});
    const ArrayView<const Bool> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](Bool value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getBool,
                        std::bind(&BitStreamReader::readBool, _1));
            });
}

TEST_F(IntrospectableViewTest, int8ConstArray)
{
    const auto rawArray = std::vector<Int8>({-10, -20, 30, 40});
    const ArrayView<const Int8> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](Int8 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt8,
                        std::bind(&BitStreamReader::readSignedBits32, _1, 8));
            });
}

TEST_F(IntrospectableViewTest, int16ConstArray)
{
    const auto rawArray = std::vector<Int16>({-100, -200, 300, 400});
    const ArrayView<const Int16> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](Int16 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt16,
                        std::bind(&BitStreamReader::readSignedBits32, _1, 16));
            });
}

TEST_F(IntrospectableViewTest, int32ConstArray)
{
    const auto rawArray = std::vector<Int32>({-10000, -20000, 30000, 40000});
    const ArrayView<const Int32> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](Int32 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt32,
                        std::bind(&BitStreamReader::readSignedBits32, _1, 32));
            });
}

TEST_F(IntrospectableViewTest, int64ConstArray)
{
    const auto rawArray = std::vector<Int64>({-10000000, -20000000, 30000000, 40000000});
    const ArrayView<const Int64> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](Int64 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt64,
                        std::bind(&BitStreamReader::readSignedBits64, _1, 64));
            });
}

TEST_F(IntrospectableViewTest, uint8ConstArray)
{
    const auto rawArray = std::vector<UInt8>({10, 20, 30, 40});
    const ArrayView<const UInt8> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](UInt8 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt8,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, 8));
            });
}

TEST_F(IntrospectableViewTest, uint16ConstArray)
{
    const auto rawArray = std::vector<UInt16>({100, 200, 300, 400});
    const ArrayView<const UInt16> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](UInt16 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt16,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, 16));
            });
}

TEST_F(IntrospectableViewTest, uint32ConstArray)
{
    const auto rawArray = std::vector<UInt32>({10000, 20000, 30000, 40000});
    const ArrayView<const UInt32> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](UInt32 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt32,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, 32));
            });
}

TEST_F(IntrospectableViewTest, uint64ConstArray)
{
    const auto rawArray = std::vector<UInt64>({10000000, 20000000, 30000000, 40000000});
    const ArrayView<const UInt64> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](UInt64 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt64,
                        std::bind(&BitStreamReader::readUnsignedBits64, _1, 64));
            });
}

TEST_F(IntrospectableViewTest, fixedSignedBitField5ConstArray)
{
    uint8_t numBits = 5;
    const auto rawArray = std::vector<Int5>{{-3, -1, 2, 4, 6}};
    const ArrayView<const Int5> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](Int5 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt8,
                        std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
            });
}

TEST_F(IntrospectableViewTest, fixedUnsignedBitField5ConstArray)
{
    const uint8_t numBits = 5;
    const auto rawArray = std::vector<UInt5>{{3, 1, 2, 4, 6}};
    const ArrayView<const UInt5> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](UInt5 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt8,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
            });
}

TEST_F(IntrospectableViewTest, dynamicSignedBitField5ConstArray)
{
    struct ZserioArrayOwner
    {
        uint8_t numBits = 5;
    };

    struct ZserioArrayTraits
    {
        using OwnerType = ZserioArrayOwner;

        static View<DynInt8<>> at(const OwnerType& owner, const DynInt8<>& element, size_t)
        {
            return View<DynInt8<>>(element, owner.numBits);
        }
    };

    ZserioArrayOwner owner;
    const auto rawArray = std::vector<DynInt8<>>{{-3, -1, 2, 4, 6}};
    const ArrayView<const DynInt8<>, ZserioArrayTraits> array(rawArray, owner);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](View<DynInt8<>> value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt8,
                        std::bind(&BitStreamReader::readSignedBits32, _1, owner.numBits), owner.numBits);
            });
}

TEST_F(IntrospectableViewTest, dynamicUnsignedBitField5ConstArray)
{
    struct ZserioArrayOwner
    {
        uint8_t numBits = 5;
    };

    struct ZserioArrayTraits
    {
        using OwnerType = ZserioArrayOwner;

        static View<DynUInt8<>> at(const OwnerType& owner, const DynUInt8<>& element, size_t)
        {
            return View<DynUInt8<>>(element, owner.numBits);
        }
    };

    ZserioArrayOwner owner;
    const auto rawArray = std::vector<DynUInt8<>>{{3, 1, 2, 4, 6}};
    const ArrayView<const DynUInt8<>, ZserioArrayTraits> array(rawArray, owner);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](View<DynUInt8<>> value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt8,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, owner.numBits), owner.numBits);
            });
}

TEST_F(IntrospectableViewTest, varint16ConstArray)
{
    const auto rawArray = std::vector<VarInt16>({-10, -20, 30, 40});
    const ArrayView<const VarInt16> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](VarInt16 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt16,
                        std::bind(&BitStreamReader::readVarInt16, _1));
            });
}

TEST_F(IntrospectableViewTest, varint32ConstArray)
{
    const auto rawArray = std::vector<VarInt32>({-10000, -20000, 30000, 40000});
    const ArrayView<const VarInt32> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](VarInt32 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt32,
                        std::bind(&BitStreamReader::readVarInt32, _1));
            });
}

TEST_F(IntrospectableViewTest, varint64ConstArray)
{
    const auto rawArray = std::vector<VarInt64>({-10000000, -20000000, 30000000, 40000000});
    const ArrayView<const VarInt64> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](VarInt64 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt64,
                        std::bind(&BitStreamReader::readVarInt64, _1));
            });
}

TEST_F(IntrospectableViewTest, varintConstArray)
{
    const auto rawArray = std::vector<VarInt>({-10000000, -20000000, 30000000, 40000000});
    const ArrayView<const VarInt> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](VarInt value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt64,
                        std::bind(&BitStreamReader::readVarInt, _1));
            });
}

TEST_F(IntrospectableViewTest, varuint16ConstArray)
{
    const auto rawArray = std::vector<VarUInt16>({10, 20, 30, 40});
    const ArrayView<const VarUInt16> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](VarUInt16 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt16,
                        std::bind(&BitStreamReader::readVarUInt16, _1));
            });
}

TEST_F(IntrospectableViewTest, varuint32ConstArray)
{
    const auto rawArray = std::vector<VarUInt32>({10000, 20000, 30000, 40000});
    const ArrayView<const VarUInt32> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](VarUInt32 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt32,
                        std::bind(&BitStreamReader::readVarUInt32, _1));
            });
}

TEST_F(IntrospectableViewTest, varuint64ConstArray)
{
    const auto rawArray = std::vector<VarUInt64>({10000, 20000, 30000, 40000});
    const ArrayView<const VarUInt64> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](VarUInt64 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt64,
                        std::bind(&BitStreamReader::readVarUInt64, _1));
            });
}

TEST_F(IntrospectableViewTest, varuintConstArray)
{
    const auto rawArray = std::vector<VarUInt>({10000, 20000, 30000, 40000});
    const ArrayView<const VarUInt> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](VarUInt value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt64,
                        std::bind(&BitStreamReader::readVarUInt, _1));
            });
}

TEST_F(IntrospectableViewTest, varsizeConstArray)
{
    const auto rawArray = std::vector<VarSize>({10000, 20000, 30000, 40000});
    const ArrayView<const VarSize> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](VarSize value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt32,
                        std::bind(&BitStreamReader::readVarSize, _1));
            });
}

TEST_F(IntrospectableViewTest, float16ConstArray)
{
    const auto rawArray = std::vector<Float16>{{2.0F, 0.0F}};
    const ArrayView<const Float16> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](Float16 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkFloatingPoint(value, elementIntrospectableView, &IIntrospectableView::getFloat,
                        std::bind(&BitStreamReader::readFloat16, _1));
            });
}

TEST_F(IntrospectableViewTest, float32ConstArray)
{
    const auto rawArray = std::vector<Float32>{{2.0F, 0.0F, 1.2F}};
    const ArrayView<const Float32> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](Float32 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkFloatingPoint(value, elementIntrospectableView, &IIntrospectableView::getFloat,
                        std::bind(&BitStreamReader::readFloat32, _1));
            });
}

TEST_F(IntrospectableViewTest, float64ConstArray)
{
    const auto rawArray = std::vector<Float64>{{2.0, 0.0, 1.2}};
    const ArrayView<const Float64> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](Float64 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkFloatingPoint(value, elementIntrospectableView, &IIntrospectableView::getDouble,
                        std::bind(&BitStreamReader::readFloat64, _1));
            });
}

TEST_F(IntrospectableViewTest, stringConstArray)
{
    const auto rawArray = std::vector<std::string>{{"one", "two", "three"}};
    const ArrayView<const std::string> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](std::string_view value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkString(value, elementIntrospectableView);
            });
}

TEST_F(IntrospectableViewTest, bitBufferConstArray)
{
    const auto rawArray = std::vector<BitBuffer>{{BitBuffer({0xF8}, 5), BitBuffer({0xAB, 0xCD}, 16)}};
    const ArrayView<const BitBuffer> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](BitBufferView value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkBitBuffer(value, elementIntrospectableView);
            });
}

TEST_F(IntrospectableViewTest, bytesConstArray)
{
    const auto rawArray = std::vector<Bytes>{{{{0x00, 0x01}}, {{0xFF, 0xFE}}}};
    const ArrayView<const Bytes> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](BytesView value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkBytes(value, elementIntrospectableView);
            });
}

TEST_F(IntrospectableViewTest, bitmaskConstIntrospectableView)
{
    const ReflectableBitmask bitmask = ReflectableBitmask::Values::WRITE;
    auto introspectableView = introspectable(bitmask);
    checkBitmask(bitmask, introspectableView);
}

TEST_F(IntrospectableViewTest, bitmaskConstArray)
{
    const auto rawArray = std::vector<ReflectableBitmask>{{ReflectableBitmask::Values::WRITE,
            ReflectableBitmask::Values::CREATE, ReflectableBitmask::Values::READ}};
    const ArrayView<const ReflectableBitmask> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](ReflectableBitmask value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkBitmask(value, elementIntrospectableView);
            });
}

TEST_F(IntrospectableViewTest, enumConstIntrospectableView)
{
    const ReflectableEnum enumeration = ReflectableEnum::VALUE1;
    auto introspectableView = introspectable(enumeration);
    checkEnum(enumeration, introspectableView);
}

TEST_F(IntrospectableViewTest, enumConstArray)
{
    const auto rawArray = std::vector<ReflectableEnum>{
            {ReflectableEnum::VALUE1, ReflectableEnum::VALUE2, ReflectableEnum::VALUE3}};
    const ArrayView<const ReflectableEnum> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](ReflectableEnum value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkEnum(value, elementIntrospectableView);
            });
}

TEST_F(IntrospectableViewTest, compoundConst)
{
    const ReflectableObject reflectableObject = createReflectableObject("test", 13);
    const View view(reflectableObject);
    auto introspectableView = introspectable(view);
    checkCompound(view, introspectableView);
}

TEST_F(IntrospectableViewTest, compoundConstArray)
{
    ReflectableObject reflectableObject1 = createReflectableObject("1", 13);
    const auto rawArray =
            std::vector<ReflectableObject>{{reflectableObject1, createReflectableObject("2", 42)}};
    const ArrayView<const ReflectableObject> array(rawArray);
    auto introspectableView = introspectableArray(array);
    checkArray(array, introspectableView,
            [&](const View<ReflectableObject>& view,
                    const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkCompound(view, elementIntrospectableView);
            });
}

} // namespace zserio
