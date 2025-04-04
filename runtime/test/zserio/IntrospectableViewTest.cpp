#include <functional>

#include "gtest/gtest.h"
#include "zserio/IntrospectableView.h"
#include "zserio/View.h"

using namespace std::placeholders;

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
        (void)value;
        (void)introspectableView;
        (void)readFunc;
        (void)bitBufferSize;

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
    void checkCppTypeGetter(T value, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_EQ(value, ((*introspectableViewPtr).*getter)()) << testName << " : " << getterName;
    }

    template <typename T, typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<typename T::ValueType, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(T, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr, const GETTER& getter,
            const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*introspectableViewPtr).*getter)(), CppRuntimeException)
                << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<std::is_same_v<BytesView, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(BytesView value, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr,
            const GETTER& getter, const char* testName, const char* getterName)
    {
        ASSERT_EQ(value.size(), ((*introspectableViewPtr).*getter)().size()) << testName << " : " << getterName;
        ASSERT_EQ(value.data(), ((*introspectableViewPtr).*getter)().data()) << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<BytesView, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(BytesView, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr,
            const GETTER& getter, const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*introspectableViewPtr).*getter)(), CppRuntimeException)
                << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<std::is_same_v<std::string_view, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(std::string_view value, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr,
            const GETTER& getter, const char* testName, const char* getterName)
    {
        ASSERT_EQ(value, ((*introspectableViewPtr).*getter)()) << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<std::string_view, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(std::string_view, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr,
            const GETTER& getter, const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*introspectableViewPtr).*getter)(), CppRuntimeException)
                << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<std::is_same_v<const BitBuffer&, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(const BitBuffer& value, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr,
            const GETTER& getter, const char* testName, const char* getterName)
    {
        ASSERT_EQ(value, ((*introspectableViewPtr).*getter)()) << testName << " : " << getterName;
    }

    template <typename INTROSPECTABLE_VIEW_PTR, typename GETTER,
            std::enable_if_t<!std::is_same_v<const BitBuffer&, detail::return_type_t<GETTER>>, int> = 0>
    void checkCppTypeGetter(const BitBuffer&, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr,
            const GETTER& getter, const char* testName, const char* getterName)
    {
        ASSERT_THROW(((*introspectableViewPtr).*getter)(), CppRuntimeException)
                << testName << " : " << getterName;
    }

    template <typename T, typename INTROSPECTABLE_VIEW_PTR>
    void checkCppTypeGetters(
            T value, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr, const char* testName)
    {
        checkCppTypeGetter(value, introspectableViewPtr, &IIntrospectableView::getBool, testName, "getBool");

        checkCppTypeGetter(value, introspectableViewPtr, &IIntrospectableView::getInt8, testName, "getInt8");
        checkCppTypeGetter(value, introspectableViewPtr, &IIntrospectableView::getInt16, testName, "getInt16");
        checkCppTypeGetter(value, introspectableViewPtr, &IIntrospectableView::getInt32, testName, "getInt32");
        checkCppTypeGetter(value, introspectableViewPtr, &IIntrospectableView::getInt64, testName, "getInt64");

        checkCppTypeGetter(value, introspectableViewPtr, &IIntrospectableView::getUInt8, testName, "getUInt8");
        checkCppTypeGetter(
                value, introspectableViewPtr, &IIntrospectableView::getUInt16, testName, "getUInt16");
        checkCppTypeGetter(
                value, introspectableViewPtr, &IIntrospectableView::getUInt32, testName, "getUInt32");
        checkCppTypeGetter(
                value, introspectableViewPtr, &IIntrospectableView::getUInt64, testName, "getUInt64");

        checkCppTypeGetter(value, introspectableViewPtr, &IIntrospectableView::getFloat, testName, "getFloat");
        checkCppTypeGetter(
                value, introspectableViewPtr, &IIntrospectableView::getDouble, testName, "getDouble");

        checkCppTypeGetter(value, introspectableViewPtr, &IIntrospectableView::getBytes, testName, "getBytes");
        checkCppTypeGetter(
                value, introspectableViewPtr, &IIntrospectableView::getStringView, testName, "getStringView");
        checkCppTypeGetter(
                value, introspectableViewPtr, &IIntrospectableView::getBitBuffer, testName, "getBitBuffer");
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
    void checkIntegral(T value, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr, const GETTER& getter,
            const READ_FUNC& readFunc, size_t bitSize, const char* testName)
    {
        (void)readFunc;
        (void)bitSize;
        (void)testName;
        ASSERT_EQ(value, ((*introspectableViewPtr).*getter)());

        ASSERT_EQ(value, introspectableViewPtr->getAnyValue().template get<T>());

        ASSERT_DOUBLE_EQ(static_cast<double>(value), introspectableViewPtr->toDouble());
        ASSERT_EQ(zserio::toString(value), introspectableViewPtr->toString());

        checkCppTypeGetters(value, introspectableViewPtr, testName);

        checkNonCompound(introspectableViewPtr);

        checkNonArray(introspectableViewPtr);

        checkWriteRead(value, introspectableViewPtr, readFunc, bitSize);
    }

    template <typename T, typename INTROSPECTABLE_VIEW_PTR, typename GETTER, typename READ_FUNC>
    void checkSignedIntegral(T value, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr,
            const GETTER& getter, const READ_FUNC& readFunc, size_t bitSize = sizeof(T) * 8)
    {
        ASSERT_EQ(value, introspectableViewPtr->toInt());
        ASSERT_THROW(introspectableViewPtr->toUInt(), CppRuntimeException);

        checkIntegral(value, introspectableViewPtr, getter, readFunc, bitSize, "checkSignedIntegral");
    }

    template <typename T, typename INTROSPECTABLE_VIEW_PTR, typename GETTER, typename READ_FUNC>
    void checkUnsignedIntegral(T value, const INTROSPECTABLE_VIEW_PTR& introspectableViewPtr,
            const GETTER& getter, const READ_FUNC& readFunc, size_t bitSize = sizeof(T) * 8)
    {
        ASSERT_EQ(value, introspectableViewPtr->toUInt());
        ASSERT_THROW(introspectableViewPtr->toInt(), CppRuntimeException);

        checkIntegral(value, introspectableViewPtr, getter, readFunc, bitSize, "checkUnsignedIntegral");
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

    template <typename ARRAY>
    void checkArrayAnyValue(const ARRAY& array, const IIntrospectableViewConstPtr& introspectableView)
    {
        (void)array;
        (void)introspectableView;
        // !@# Span comparison
        // ASSERT_EQ(array, introspectableView->getAnyValue().template get<const ARRAY>());
    }

    template <typename ARRAY, typename INTROSPECTABLE_VIEW_PTR, typename ELEMENT_CHECKER>
    void checkArray(const ARRAY& array, const INTROSPECTABLE_VIEW_PTR& introspectableView,
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

        checkNonCompound(introspectableView);
    }
#if 0 // !@#

    template <typename INTROSPECTABLE_VIEW_PTR>
    void checkBitmask(IntrospectableViewBitmask bitmask, const INTROSPECTABLE_VIEW_PTR& introspectableView)
    {
        ASSERT_EQ(bitmask.getValue(), introspectableView->getUInt8());

        ASSERT_EQ(bitmask, introspectableView->getAnyValue().template get<IntrospectableViewBitmask>());

        ASSERT_EQ(bitmask.getValue(), introspectableView->toUInt());
        ASSERT_EQ(bitmask.getValue(), introspectableView->toDouble());
        ASSERT_EQ(bitmask.toString(), introspectableView->toString());
        ASSERT_THROW(introspectableView->toInt(), CppRuntimeException);

        checkCppTypeGetters(bitmask.getValue(), introspectableView, "checkBitmask");

        checkNonCompound(introspectableView);
        checkNonArray(introspectableView);
    }

    template <typename INTROSPECTABLE_VIEW_PTR>
    void checkEnum(IntrospectableViewEnum enumeration, const INTROSPECTABLE_VIEW_PTR& introspectableView)
    {
        ASSERT_EQ(enumToValue(enumeration), introspectableView->getInt8());

        ASSERT_EQ(enumeration, introspectableView->getAnyValue().template get<IntrospectableViewEnum>());

        ASSERT_EQ(enumToValue(enumeration), introspectableView->toInt());
        ASSERT_EQ(enumToValue(enumeration), introspectableView->toDouble());
        ASSERT_EQ(enumToString(enumeration), introspectableView->toString());
        ASSERT_THROW(introspectableView->toUInt(), CppRuntimeException);

        checkCppTypeGetters(enumToValue(enumeration), introspectableView, "checkEnum");

        checkNonCompound(introspectableView);
        checkNonArray(introspectableView);
    }

    void checkCompoundAnyValue(
            const IntrospectableViewObject& introspectableViewObject, const IIntrospectableViewConstPtr& introspectableView)
    {
        ASSERT_EQ(introspectableViewObject,
                introspectableView->getAnyValue()
                        .template get<std::reference_wrapper<const IntrospectableViewObject>>()
                        .get());
        ASSERT_EQ(introspectableViewObject.introspectableViewNested,
                introspectableView->getField("introspectableViewNested")
                        ->getAnyValue()
                        .template get<std::reference_wrapper<const IntrospectableViewNested>>()
                        .get());
    }

    void checkCompoundAnyValue(
            const IntrospectableViewObject& introspectableViewObject, const IIntrospectableViewPtr& introspectableView)
    {
        ASSERT_EQ(introspectableViewObject,
                introspectableView->getAnyValue().template get<std::reference_wrapper<IntrospectableViewObject>>().get());
        ASSERT_EQ(introspectableViewObject.introspectableViewNested,
                introspectableView->getField("introspectableViewNested")
                        ->getAnyValue()
                        .template get<std::reference_wrapper<IntrospectableViewNested>>()
                        .get());
    }

    template <typename INTROSPECTABLE_VIEW_PTR>
    void checkCompoundConstMethods(
            const IntrospectableViewObject& introspectableViewObject, const INTROSPECTABLE_VIEW_PTR& introspectableView)
    {
        ASSERT_TRUE(TypeInfoUtil::isCompound(introspectableView->getTypeInfo().getSchemaType()));

        // field getter
        ASSERT_EQ(introspectableViewObject.introspectableViewNested.value,
                introspectableView->getField("introspectableViewNested")->getField("value")->getUInt32());
        ASSERT_THROW(introspectableView->getField("nonexistent"), CppRuntimeException);
        ASSERT_THROW(introspectableView->getField("introspectableViewNested")->getField("nonexistent"), CppRuntimeException);

        ASSERT_EQ("valueStr", introspectableView->getField("introspectableViewChoice")->getChoice());
        ASSERT_EQ("value32", introspectableView->getField("introspectableViewUnion")->getChoice());

        // find field
        ASSERT_EQ(introspectableViewObject.introspectableViewNested.value,
                introspectableView->find("introspectableViewNested.value")->toUInt());
        ASSERT_EQ(introspectableViewObject.introspectableViewNested.value,
                (*introspectableView)["introspectableViewNested.value"]->toDouble());

        // find failed
        ASSERT_EQ(nullptr, introspectableView->find("introspectableViewNested.nonexistent"));
        ASSERT_EQ(nullptr, introspectableView->find("nonexistent"));
        ASSERT_EQ(nullptr, introspectableView->find("introspectableViewNested.value.nonexistent"));

        checkCompoundAnyValue(introspectableViewObject, introspectableView);

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
    }

    void checkCompound(const IntrospectableViewObject& introspectableViewObject, const IIntrospectableViewConstPtr& introspectableView)
    {
        checkCompoundConstMethods(introspectableViewObject, introspectableView);
    }

    void checkCompound(const IntrospectableViewObject& introspectableViewObject, const IIntrospectableViewPtr& introspectableView)
    {
        checkCompoundConstMethods(introspectableViewObject, introspectableView);
        checkCompoundConstMethods(introspectableViewObject, static_cast<IIntrospectableViewConstPtr>(introspectableView));

        // setter
        introspectableView->getField("introspectableViewNested")->setField("value", Any(UInt31(11)));
        ASSERT_EQ(11, introspectableViewObject.introspectableViewNested.value);
        introspectableView->find("introspectableViewNested")->setField("value", Any(UInt31(11)));
        ASSERT_THROW(introspectableView->setField("nonexistent", Any()), CppRuntimeException);
        ASSERT_THROW(
                introspectableView->find("introspectableViewNested")->setField("nonexistent", Any()), CppRuntimeException);
        introspectableView->find("introspectableViewChoice")->setField("value32", Any(UInt32(42)));
        ASSERT_EQ(introspectableViewObject.introspectableViewChoice.index(), IntrospectableViewChoice::ChoiceTag::CHOICE_value32);
        introspectableView->getField("introspectableViewUnion")->setField("valueStr", Any(std::string("test")));
        ASSERT_EQ(introspectableViewObject.introspectableViewUnion.index(), IntrospectableViewUnion::ChoiceTag::CHOICE_valueStr);
        // return back to let checkCompoundConstMethods pass
        introspectableView->find("introspectableViewChoice")->setField("valueStr", Any(std::string("test")));
        introspectableView->find("introspectableViewUnion")->setField("value32", Any(UInt32(13)));

        // any value
        ASSERT_EQ(introspectableViewObject.introspectableViewNested,
                introspectableView->find("introspectableViewNested")
                        ->getAnyValue()
                        .template get<std::reference_wrapper<IntrospectableViewNested>>()
                        .get());

        introspectableView->createField("introspectableViewNested");
        ASSERT_EQ(uint32_t(), introspectableViewObject.introspectableViewNested.value);

        introspectableView->setField("introspectableViewNested", Any(IntrospectableViewNested{42}));
        ASSERT_EQ(42, introspectableViewObject.introspectableViewNested.value);
    }

    static IntrospectableViewObject createIntrospectableViewObject(std::string_view stringField, UInt31 value = 13,
            IntrospectableViewEnum introspectableViewEnum = IntrospectableViewEnum::VALUE1,
            IntrospectableViewUnion::ChoiceTag unionTag = IntrospectableViewUnion::ChoiceTag::CHOICE_value32)
    {
        IntrospectableViewObject introspectableViewObject;
        introspectableViewObject.stringField = stringField;
        introspectableViewObject.introspectableViewNested.value = value;
        introspectableViewObject.introspectableViewEnum = introspectableViewEnum;

        switch (introspectableViewEnum)
        {
        case IntrospectableViewEnum::VALUE1:
            introspectableViewObject.introspectableViewChoice.emplace<IntrospectableViewChoice::ChoiceTag::CHOICE_valueStr>("test");
            break;
        case IntrospectableViewEnum::VALUE2:
            introspectableViewObject.introspectableViewChoice.emplace<IntrospectableViewChoice::ChoiceTag::CHOICE_value32>(13);
            break;
        default:
            // empty
            break;
        }

        switch (unionTag)
        {
        case IntrospectableViewUnion::ChoiceTag::CHOICE_value32:
            introspectableViewObject.introspectableViewUnion.emplace<IntrospectableViewUnion::ChoiceTag::CHOICE_value32>(13);
            break;
        case IntrospectableViewUnion::ChoiceTag::CHOICE_valueStr:
            introspectableViewObject.introspectableViewUnion.emplace<IntrospectableViewUnion::ChoiceTag::CHOICE_valueStr>("test");
            break;
        default:
            // shouldn't occur
            break;
        }

        return introspectableViewObject;
    }
#endif // !@#
};

TEST_F(IntrospectableViewTest, boolIntrospectableView)
{
    const Bool value = true;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getBool,
            std::bind(&BitStreamReader::readBool, _1));
}

TEST_F(IntrospectableViewTest, int8IntrospectableView)
{
    const Int8 value = -12;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, 8));
}

TEST_F(IntrospectableViewTest, int16IntrospectableView)
{
    const Int16 value = -1234;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, 16));
}

TEST_F(IntrospectableViewTest, int32IntrospectableView)
{
    const Int32 value = -123456;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, 32));
}

TEST_F(IntrospectableViewTest, int64IntrospectableView)
{
    const Int64 value = -1234567890;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, 64));
}

TEST_F(IntrospectableViewTest, uint8IntrospectableView)
{
    const UInt8 value = 0xFF;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 8));
}

TEST_F(IntrospectableViewTest, uint16IntrospectableView)
{
    const UInt16 value = 0xFFFF;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 16));
}

TEST_F(IntrospectableViewTest, uint32IntrospectableView)
{
    const UInt32 value = 0xFFFFFFFF;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 32));
}

TEST_F(IntrospectableViewTest, uint64IntrospectableView)
{
    const UInt64 value = 0xFFFFFFFFFFFF;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, 64));
}

TEST_F(IntrospectableViewTest, fixedSignedBitField5) // mapped to int8_t
{
    const Int5 value = 15;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, 5), 5);
}

TEST_F(IntrospectableViewTest, fixedSignedBitField15) // mapped to int16_t
{
    const Int15 value = -15;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, 15), 15);
}

TEST_F(IntrospectableViewTest, fixedSignedBitField31) // mapped to int32_t
{
    const Int31 value = -12345678;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, 31), 31);
}

TEST_F(IntrospectableViewTest, fixedSignedBitField60) // mapped to int64_t
{
    const Int60 value = 1234567890;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, 60), 60);
}

TEST_F(IntrospectableViewTest, fixedUnsignedBitField7) // mapped to uint8_t
{
    const UInt7 value = 0x2F;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 7), 7);
}

TEST_F(IntrospectableViewTest, fixedUnsignedBitField9) // mapped to uint16_t
{
    const UInt9 value = 0x1FF;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 9), 9);
}

TEST_F(IntrospectableViewTest, fixedUnsignedBitField31) // mapped to uint32_t
{
    const UInt31 value = UINT32_MAX >> 1U;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, 31), 31);
}

TEST_F(IntrospectableViewTest, fixedUnsignedBitField33) // mapped to uint64_t
{
    const UInt33 value = static_cast<uint64_t>(UINT32_MAX) << 1U;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, 33), 33);
}

TEST_F(IntrospectableViewTest, dynamicSignedBitField5) // mapped to int8_t
{
    const uint8_t numBits = 5;
    const DynInt8<> value = 15;
    const View view(value, numBits);
    auto introspectableViewPtr = introspectable(view);
    checkSignedIntegral(view, introspectableViewPtr, &IIntrospectableView::getInt8,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicSignedBitField15) // mapped to int16_t
{
    const uint8_t numBits = 15;
    const DynInt16<> value = -15;
    const View view(value, numBits);
    auto introspectableViewPtr = introspectable(view);
    checkSignedIntegral(view, introspectableViewPtr, &IIntrospectableView::getInt16,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicSignedBitField31) // mapped to int32_t
{
    const uint8_t numBits = 31;
    const DynInt32<> value = -12345678;
    const View view(value, numBits);
    auto introspectableViewPtr = introspectable(view);
    checkSignedIntegral(view, introspectableViewPtr, &IIntrospectableView::getInt32,
            std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicSignedBitField60) // mapped to int64_t
{
    const uint8_t numBits = 60;
    const DynInt64<> value = 1234567890;
    const View view(value, numBits);
    auto introspectableViewPtr = introspectable(view);
    checkSignedIntegral(view, introspectableViewPtr, &IIntrospectableView::getInt64,
            std::bind(&BitStreamReader::readSignedBits64, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicUnsignedBitField7) // mapped to uint8_t
{
    const uint8_t numBits = 7;
    const DynUInt8<> value = 0x2F;
    const View view(value, numBits);
    auto introspectableViewPtr = introspectable(view);
    checkUnsignedIntegral(view, introspectableViewPtr, &IIntrospectableView::getUInt8,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicUnsignedBitField9) // mapped to uint16_t
{
    const uint8_t numBits = 9;
    const DynUInt16<> value = 0x1FF;
    const View view(value, numBits);
    auto introspectableViewPtr = introspectable(view);
    checkUnsignedIntegral(view, introspectableViewPtr, &IIntrospectableView::getUInt16,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicUnsignedBitField31) // mapped to uint32_t
{
    const uint8_t numBits = 31;
    const DynUInt32<> value = UINT32_MAX >> 1U;
    const View view(value, numBits);
    auto introspectableViewPtr = introspectable(view);
    checkUnsignedIntegral(view, introspectableViewPtr, &IIntrospectableView::getUInt32,
            std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, dynamicUnsignedBitField33) // mapped to uint64_t
{
    const uint8_t numBits = 33;
    const DynUInt64<> value = static_cast<uint64_t>(UINT32_MAX) << 1U;
    const View view(value, numBits);
    auto introspectableViewPtr = introspectable(view);
    checkUnsignedIntegral(view, introspectableViewPtr, &IIntrospectableView::getUInt64,
            std::bind(&BitStreamReader::readUnsignedBits64, _1, numBits), numBits);
}

TEST_F(IntrospectableViewTest, varint16IntrospectableView)
{
    const VarInt16 value = -1234;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt16,
            std::bind(&BitStreamReader::readVarInt16, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varint32IntrospectableView)
{
    const VarInt32 value = 54321;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt32,
            std::bind(&BitStreamReader::readVarInt32, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varint64IntrospectableView)
{
    const VarInt64 value = -87654321;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt64,
            std::bind(&BitStreamReader::readVarInt64, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varintIntrospectableView)
{
    const VarInt value = INT64_MAX;
    auto introspectableViewPtr = introspectable(value);
    checkSignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getInt64,
            std::bind(&BitStreamReader::readVarInt, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varuint16IntrospectableView)
{
    const VarUInt16 value = 1234;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt16,
            std::bind(&BitStreamReader::readVarUInt16, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varuint32IntrospectableView)
{
    const VarUInt32 value = 0x1FFFFFFF;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt32,
            std::bind(&BitStreamReader::readVarUInt32, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varuint64IntrospectableView)
{
    const VarUInt64 value = 4242424242;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt64,
            std::bind(&BitStreamReader::readVarUInt64, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varuintIntrospectableView)
{
    const VarUInt value = UINT64_MAX;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt64,
            std::bind(&BitStreamReader::readVarUInt, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, varsizeIntrospectableView)
{
    const VarSize value = (UINT32_C(1) << (7U + 7 + 7 + 7 + 3)) - 1U;
    auto introspectableViewPtr = introspectable(value);
    checkUnsignedIntegral(value, introspectableViewPtr, &IIntrospectableView::getUInt32,
            std::bind(&BitStreamReader::readVarSize, _1), zserio::detail::bitSizeOf(value));
}

TEST_F(IntrospectableViewTest, float16IntrospectableView)
{
    const Float16 value = 2.0F;
    auto introspectableViewPtr = introspectable(value);
    checkFloatingPoint(value, introspectableViewPtr, &IIntrospectableView::getFloat,
            std::bind(&BitStreamReader::readFloat16, _1));
}

TEST_F(IntrospectableViewTest, float32IntrospectableView)
{
    const Float32 value = 1.2F;
    auto introspectableViewPtr = introspectable(value);
    checkFloatingPoint(value, introspectableViewPtr, &IIntrospectableView::getFloat,
            std::bind(&BitStreamReader::readFloat32, _1));
}

TEST_F(IntrospectableViewTest, float64IntrospectableView)
{
    const Float64 value = 1.2;
    auto introspectableViewPtr = introspectable(value);
    checkFloatingPoint(value, introspectableViewPtr, &IIntrospectableView::getDouble,
            std::bind(&BitStreamReader::readFloat64, _1));
}

TEST_F(IntrospectableViewTest, bytesIntrospectableView)
{
    const Bytes value{{0, 127, 128, 255}};
    auto introspectableViewPtr = introspectable(BytesView(value.data(), value.size()));
    checkBytes(value, introspectableViewPtr);
}

TEST_F(IntrospectableViewTest, stringIntrospectableView)
{
    auto value = std::string_view("some text as a string view");
    auto introspectableViewPtr = introspectable(value);
    checkString(value, introspectableViewPtr);
}

TEST_F(IntrospectableViewTest, bitBufferIntrospectableView)
{
    const BitBuffer value = BitBuffer{std::vector<uint8_t>({0xAB, 0xF0}), 12};
    auto introspectableViewPtr = introspectable(value);
    checkBitBuffer(value, introspectableViewPtr);
}

TEST_F(IntrospectableViewTest, boolConstArray)
{
    const auto rawArray = std::vector<Bool>({true, false, true, false});
    const Span<const Bool> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](Bool value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getBool,
                        std::bind(&BitStreamReader::readBool, _1));
            });
}

TEST_F(IntrospectableViewTest, int8ConstArray)
{
    const auto rawArray = std::vector<Int8>({-10, -20, 30, 40});
    const Span<const Int8> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](Int8 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt8,
                        std::bind(&BitStreamReader::readSignedBits32, _1, 8));
            });
}

TEST_F(IntrospectableViewTest, int16ConstArray)
{
    const auto rawArray = std::vector<Int16>({-100, -200, 300, 400});
    const Span<const Int16> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](Int16 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt16,
                        std::bind(&BitStreamReader::readSignedBits32, _1, 16));
            });
}

TEST_F(IntrospectableViewTest, int32ConstArray)
{
    const auto rawArray = std::vector<Int32>({-10000, -20000, 30000, 40000});
    const Span<const Int32> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](Int32 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt32,
                        std::bind(&BitStreamReader::readSignedBits32, _1, 32));
            });
}

TEST_F(IntrospectableViewTest, int64ConstArray)
{
    const auto rawArray = std::vector<Int64>({-10000000, -20000000, 30000000, 40000000});
    const Span<const Int64> array(rawArray.data(), rawArray.size());
    auto introspectableView = introspectable(array);
    checkArray(array, introspectableView,
            [&](Int64 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt64,
                        std::bind(&BitStreamReader::readSignedBits64, _1, 64));
            });
}

TEST_F(IntrospectableViewTest, uint8ConstArray)
{
    const auto rawArray = std::vector<UInt8>({10, 20, 30, 40});
    const Span<const UInt8> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](UInt8 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt8,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, 8));
            });
}

TEST_F(IntrospectableViewTest, uint16ConstArray)
{
    const auto rawArray = std::vector<UInt16>({100, 200, 300, 400});
    const Span<const UInt16> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](UInt16 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt16,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, 16));
            });
}

TEST_F(IntrospectableViewTest, uint32ConstArray)
{
    const auto rawArray = std::vector<UInt32>({10000, 20000, 30000, 40000});
    const Span<const UInt32> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](UInt32 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt32,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, 32));
            });
}

TEST_F(IntrospectableViewTest, uint64ConstArray)
{
    const auto rawArray = std::vector<UInt64>({10000000, 20000000, 30000000, 40000000});
    const Span<const UInt64> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](UInt64 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt64,
                        std::bind(&BitStreamReader::readUnsignedBits64, _1, 64));
            });
}

TEST_F(IntrospectableViewTest, fixedSignedBitField5ConstArray)
{
    uint8_t numBits = 5;
    const auto rawArray = std::vector<Int5>{{-3, -1, 2, 4, 6}};
    const Span<const Int5> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](Int5 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt8,
                        std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
            });
}

TEST_F(IntrospectableViewTest, fixedUnsignedBitField5ConstArray)
{
    const uint8_t numBits = 5;
    const auto rawArray = std::vector<UInt5>{{3, 1, 2, 4, 6}};
    const Span<const UInt5> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](UInt5 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt8,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
            });
}

#if 0 // !@# TODO

TEST_F(IntrospectableViewTest, dynamicSignedBitField5ConstArray)
{
    const uint8_t numBits = 5;
    const auto rawArray = std::vector<DynInt8<>>{{-3, -1, 2, 4, 6}};
    const Span<const DynInt8<>> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](DynInt8<> value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt8,
                        std::bind(&BitStreamReader::readSignedBits32, _1, numBits), numBits);
            });
}

TEST_F(IntrospectableViewTest, dynamicUnsignedBitField5ConstArray)
{
    const uint8_t numBits = 5;
    const auto rawArray = std::vector<DynUInt8<>>{{3, 1, 2, 4, 6}};
    const Span<const DynUInt8<>> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](DynUInt8<> value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt8,
                        std::bind(&BitStreamReader::readUnsignedBits32, _1, numBits), numBits);
            });
}

#endif // !@#

TEST_F(IntrospectableViewTest, varint16ConstArray)
{
    const auto rawArray = std::vector<VarInt16>({-10, -20, 30, 40});
    const Span<const VarInt16> array(rawArray.data(), rawArray.size());
    auto introspectableView = introspectable(array);
    checkArray(array, introspectableView,
            [&](VarInt16 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt16,
                        std::bind(&BitStreamReader::readVarInt16, _1));
            });
}

TEST_F(IntrospectableViewTest, varint32ConstArray)
{
    const auto rawArray = std::vector<VarInt32>({-10000, -20000, 30000, 40000});
    const Span<const VarInt32> array(rawArray.data(), rawArray.size());
    auto introspectableView = introspectable(array);
    checkArray(array, introspectableView,
            [&](VarInt32 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt32,
                        std::bind(&BitStreamReader::readVarInt32, _1));
            });
}

TEST_F(IntrospectableViewTest, varint64ConstArray)
{
    const auto rawArray = std::vector<VarInt64>({-10000000, -20000000, 30000000, 40000000});
    const Span<const VarInt64> array(rawArray.data(), rawArray.size());
    auto introspectableView = introspectable(array);
    checkArray(array, introspectableView,
            [&](VarInt64 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt64,
                        std::bind(&BitStreamReader::readVarInt64, _1));
            });
}

TEST_F(IntrospectableViewTest, varintConstArray)
{
    const auto rawArray = std::vector<VarInt>({-10000000, -20000000, 30000000, 40000000});
    const Span<const VarInt> array(rawArray.data(), rawArray.size());
    auto introspectableView = introspectable(array);
    checkArray(array, introspectableView,
            [&](VarInt value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkSignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getInt64,
                        std::bind(&BitStreamReader::readVarInt, _1));
            });
}

TEST_F(IntrospectableViewTest, varuint16ConstArray)
{
    const auto rawArray = std::vector<VarUInt16>({10, 20, 30, 40});
    const Span<const VarUInt16> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](VarUInt16 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt16,
                        std::bind(&BitStreamReader::readVarUInt16, _1));
            });
}

TEST_F(IntrospectableViewTest, varuint32ConstArray)
{
    const auto rawArray = std::vector<VarUInt32>({10000, 20000, 30000, 40000});
    const Span<const VarUInt32> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](VarUInt32 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt32,
                        std::bind(&BitStreamReader::readVarUInt32, _1));
            });
}

TEST_F(IntrospectableViewTest, varuint64ConstArray)
{
    const auto rawArray = std::vector<VarUInt64>({10000, 20000, 30000, 40000});
    const Span<const VarUInt64> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](VarUInt64 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt64,
                        std::bind(&BitStreamReader::readVarUInt64, _1));
            });
}

TEST_F(IntrospectableViewTest, varuintConstArray)
{
    const auto rawArray = std::vector<VarUInt>({10000, 20000, 30000, 40000});
    const Span<const VarUInt> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](VarUInt value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt64,
                        std::bind(&BitStreamReader::readVarUInt, _1));
            });
}

TEST_F(IntrospectableViewTest, varsizeConstArray)
{
    const auto rawArray = std::vector<VarSize>({10000, 20000, 30000, 40000});
    const Span<const VarSize> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](VarSize value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkUnsignedIntegral(value, elementIntrospectableView, &IIntrospectableView::getUInt32,
                        std::bind(&BitStreamReader::readVarSize, _1));
            });
}

TEST_F(IntrospectableViewTest, float16ConstArray)
{
    const auto rawArray = std::vector<Float16>{{2.0F, 0.0F}};
    const Span<const Float16> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](Float16 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkFloatingPoint(value, elementIntrospectableView, &IIntrospectableView::getFloat,
                        std::bind(&BitStreamReader::readFloat16, _1));
            });
}

TEST_F(IntrospectableViewTest, float32ConstArray)
{
    const auto rawArray = std::vector<Float32>{{2.0F, 0.0F, 1.2F}};
    const Span<const Float32> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](Float32 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkFloatingPoint(value, elementIntrospectableView, &IIntrospectableView::getFloat,
                        std::bind(&BitStreamReader::readFloat32, _1));
            });
}

TEST_F(IntrospectableViewTest, float64ConstArray)
{
    const auto rawArray = std::vector<Float64>{{2.0, 0.0, 1.2}};
    const Span<const Float64> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](Float64 value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkFloatingPoint(value, elementIntrospectableView, &IIntrospectableView::getDouble,
                        std::bind(&BitStreamReader::readFloat64, _1));
            });
}

TEST_F(IntrospectableViewTest, stringConstArray)
{
    const auto rawArray = std::vector<std::string>{{"one", "two", "three"}};
    const Span<const std::string> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](std::string_view value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkString(value, elementIntrospectableView);
            });
}

TEST_F(IntrospectableViewTest, bitBufferConstArray)
{
    const auto rawArray = std::vector<BitBuffer>{{BitBuffer({0xF8}, 5), BitBuffer({0xAB, 0xCD}, 16)}};
    const Span<const BitBuffer> array(rawArray.data(), rawArray.size());
    auto introspectableView = introspectable(array);
    checkArray(array, introspectableView,
            [&](const BitBuffer& value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkBitBuffer(value, elementIntrospectableView);
            });
}

TEST_F(IntrospectableViewTest, bytesConstArray)
{
    const auto rawArray = std::vector<Bytes>{{{{0x00, 0x01}}, {{0xFF, 0xFE}}}};
    const Span<const Bytes> array(rawArray.data(), rawArray.size());
    auto introspectableViewPtr = introspectable(array);
    checkArray(array, introspectableViewPtr,
            [&](const Bytes& value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkBytes(value, elementIntrospectableView);
            });
}

#if 0 // !@#
TEST_F(IntrospectableViewTest, bitmaskConstIntrospectableView)
{
    const IntrospectableViewBitmask bitmask = IntrospectableViewBitmask::Values::WRITE;
    auto introspectableViewPtr = introspectableView(bitmask);
    checkBitmask(bitmask, introspectableViewPtr);
}

TEST_F(IntrospectableViewTest, bitmaskIntrospectableView)
{
    IntrospectableViewBitmask bitmask = IntrospectableViewBitmask::Values::WRITE;
    auto introspectableViewPtr = introspectableView(bitmask);
    checkBitmask(bitmask, introspectableViewPtr);
}

TEST_F(IntrospectableViewTest, bitmaskConstArray)
{
    const auto rawArray = std::vector<IntrospectableViewBitmask>{{IntrospectableViewBitmask::Values::WRITE,
            IntrospectableViewBitmask::Values::CREATE, IntrospectableViewBitmask::Values::READ}};
    auto introspectableViewPtr = introspectableViewArray(rawArray);
    checkArray(rawArray, introspectableViewPtr,
            [&](IntrospectableViewBitmask value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkBitmask(value, elementIntrospectableView);
            });

    auto nonConstIntrospectableView = std::const_pointer_cast<IIntrospectableView>(introspectableViewPtr);
    ASSERT_THROW(nonConstIntrospectableView->getAnyValue(), CppRuntimeException);
}

TEST_F(IntrospectableViewTest, bitmaskArray)
{
    auto rawArray = std::vector<IntrospectableViewBitmask>{{IntrospectableViewBitmask::Values::WRITE,
            IntrospectableViewBitmask::Values::CREATE, IntrospectableViewBitmask::Values::READ}};
    auto introspectableViewPtr = introspectableViewArray(rawArray);
    checkArray(rawArray, introspectableViewPtr,
            [&](IntrospectableViewBitmask value, const IIntrospectableViewPtr& elementIntrospectableView) {
                checkBitmask(value, elementIntrospectableView);
            });
    checkArray(rawArray, static_cast<IIntrospectableViewConstPtr>(introspectableViewPtr),
            [&](IntrospectableViewBitmask value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkBitmask(value, elementIntrospectableView);
            });

    introspectableViewPtr->resize(0);
    ASSERT_EQ(0, introspectableViewPtr->size());
    introspectableViewPtr->append(Any(IntrospectableViewBitmask(IntrospectableViewBitmask::Values::READ)));
    ASSERT_EQ(1, introspectableViewPtr->size());
    ASSERT_EQ(IntrospectableViewBitmask::Values::READ, IntrospectableViewBitmask(introspectableViewPtr->at(0)->getUInt8()));
    introspectableViewPtr->setAt(Any(IntrospectableViewBitmask(IntrospectableViewBitmask::Values::CREATE)), 0);
    ASSERT_EQ(1, introspectableViewPtr->size());
    ASSERT_EQ(IntrospectableViewBitmask::Values::CREATE, IntrospectableViewBitmask(introspectableViewPtr->at(0)->getUInt8()));
    introspectableViewPtr->resize(2);
    ASSERT_EQ(2, introspectableViewPtr->size());

    introspectableViewPtr->append(Any(IntrospectableViewBitmask(IntrospectableViewBitmask::Values::WRITE).getValue()));
    ASSERT_EQ(3, introspectableViewPtr->size());
    ASSERT_EQ(IntrospectableViewBitmask::Values::WRITE, IntrospectableViewBitmask(introspectableViewPtr->at(2)->getUInt8()));
    introspectableViewPtr->setAt(Any(IntrospectableViewBitmask(IntrospectableViewBitmask::Values::READ).getValue()), 2);
    ASSERT_EQ(3, introspectableViewPtr->size());
    ASSERT_EQ(IntrospectableViewBitmask::Values::READ, IntrospectableViewBitmask(introspectableViewPtr->at(2)->getUInt8()));

    // out of range
    ASSERT_THROW(introspectableViewPtr->setAt(Any(IntrospectableViewBitmask::Values::CREATE), 3), CppRuntimeException);
}

TEST_F(IntrospectableViewTest, enumIntrospectableView)
{
    const IntrospectableViewEnum enumeration = IntrospectableViewEnum::VALUE1;
    auto introspectableViewPtr = introspectableView(enumeration);
    checkEnum(enumeration, introspectableViewPtr);
}

TEST_F(IntrospectableViewTest, enumConstArray)
{
    const auto rawArray = std::vector<IntrospectableViewEnum>{
            {IntrospectableViewEnum::VALUE1, IntrospectableViewEnum::VALUE2, IntrospectableViewEnum::VALUE3}};
    auto introspectableViewPtr = introspectableViewArray(rawArray);
    checkArray(rawArray, introspectableViewPtr,
            [&](IntrospectableViewEnum value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkEnum(value, elementIntrospectableView);
            });

    auto nonConstIntrospectableView = std::const_pointer_cast<IIntrospectableView>(introspectableViewPtr);
    ASSERT_THROW(nonConstIntrospectableView->getAnyValue(), CppRuntimeException);
}

TEST_F(IntrospectableViewTest, enumArray)
{
    auto rawArray = std::vector<IntrospectableViewEnum>{
            {IntrospectableViewEnum::VALUE1, IntrospectableViewEnum::VALUE2, IntrospectableViewEnum::VALUE3}};
    auto introspectableViewPtr = introspectableViewArray(rawArray);
    checkArray(rawArray, introspectableViewPtr,
            [&](IntrospectableViewEnum value, const IIntrospectableViewPtr& elementIntrospectableView) {
                checkEnum(value, elementIntrospectableView);
            });
    checkArray(rawArray, static_cast<IIntrospectableViewConstPtr>(introspectableViewPtr),
            [&](IntrospectableViewEnum value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkEnum(value, elementIntrospectableView);
            });

    introspectableViewPtr->resize(0);
    ASSERT_EQ(0, introspectableViewPtr->size());
    introspectableViewPtr->append(Any(IntrospectableViewEnum(IntrospectableViewEnum::VALUE3)));
    ASSERT_EQ(1, introspectableViewPtr->size());
    ASSERT_EQ(enumToValue(IntrospectableViewEnum::VALUE3), introspectableViewPtr->at(0)->getInt8());
    introspectableViewPtr->setAt(Any(IntrospectableViewEnum(IntrospectableViewEnum::VALUE2)), 0);
    ASSERT_EQ(1, introspectableViewPtr->size());
    ASSERT_EQ(enumToValue(IntrospectableViewEnum::VALUE2), introspectableViewPtr->at(0)->getInt8());
    introspectableViewPtr->resize(2);
    ASSERT_EQ(2, introspectableViewPtr->size());

    introspectableViewPtr->append(Any(enumToValue(IntrospectableViewEnum::VALUE1)));
    ASSERT_EQ(3, introspectableViewPtr->size());
    ASSERT_EQ(enumToValue(IntrospectableViewEnum::VALUE1), introspectableViewPtr->at(2)->getInt8());
    introspectableViewPtr->setAt(Any(enumToValue(IntrospectableViewEnum::VALUE2)), 2);
    ASSERT_EQ(3, introspectableViewPtr->size());
    ASSERT_EQ(enumToValue(IntrospectableViewEnum::VALUE2), introspectableViewPtr->at(2)->getInt8());

    // out of range
    ASSERT_THROW(introspectableViewPtr->setAt(Any(IntrospectableViewEnum(IntrospectableViewEnum::VALUE2)), 3), CppRuntimeException);
}

TEST_F(IntrospectableViewTest, compoundConst)
{
    const IntrospectableViewObject introspectableViewObject = createIntrospectableViewObject("test", 13);
    auto introspectableViewPtr = introspectableView(introspectableViewObject);
    checkCompound(introspectableViewObject, introspectableViewPtr);

    IIntrospectableViewPtr nonConstIntrospectableView = std::const_pointer_cast<IIntrospectableView>(introspectableViewPtr);
    ASSERT_NO_THROW(introspectableViewPtr->getField("introspectableViewNested"));
    ASSERT_THROW(nonConstIntrospectableView->getField("introspectableViewNested"), CppRuntimeException);
    ASSERT_NO_THROW(introspectableViewPtr->getAnyValue());
    ASSERT_THROW(nonConstIntrospectableView->getAnyValue(), CppRuntimeException);

    IIntrospectableViewConstPtr childIntrospectableView = introspectableViewPtr->getField("introspectableViewNested");
    IIntrospectableViewPtr nonConstChildIntrospectableView = std::const_pointer_cast<IIntrospectableView>(childIntrospectableView);
    ASSERT_THROW(
            nonConstChildIntrospectableView->setField("value", Any(static_cast<uint32_t>(11))), CppRuntimeException);
}

TEST_F(IntrospectableViewTest, compound)
{
    IntrospectableViewObject introspectableViewObject = createIntrospectableViewObject("test", 13);
    auto introspectableViewPtr = introspectableView(introspectableViewObject);

    checkCompound(introspectableViewObject, introspectableViewPtr);
}

TEST_F(IntrospectableViewTest, compoundConstArray)
{
    IntrospectableViewObject introspectableViewObject1 = createIntrospectableViewObject("1", 13);
    const auto rawArray =
            std::vector<IntrospectableViewObject>{{introspectableViewObject1, createIntrospectableViewObject("2", 42)}};
    auto introspectableViewPtr = introspectableViewArray(rawArray);
    checkArray(rawArray, introspectableViewPtr,
            [&](const IntrospectableViewObject& value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkCompound(value, elementIntrospectableView);
            });

    IIntrospectableViewPtr nonConstIntrospectableView = std::const_pointer_cast<IIntrospectableView>(introspectableViewPtr);
    ASSERT_THROW(nonConstIntrospectableView->at(0), CppRuntimeException);
    ASSERT_THROW((*nonConstIntrospectableView)[0], CppRuntimeException);
    ASSERT_THROW(nonConstIntrospectableView->resize(nonConstIntrospectableView->size() + 1), CppRuntimeException);
    ASSERT_THROW(nonConstIntrospectableView->setAt(Any(createIntrospectableViewObject("test", 0)), 0), CppRuntimeException);
    ASSERT_THROW(nonConstIntrospectableView->append(Any(createIntrospectableViewObject("test", 0))), CppRuntimeException);
    ASSERT_THROW(nonConstIntrospectableView->getAnyValue(), CppRuntimeException);
}

TEST_F(IntrospectableViewTest, compoundArray)
{
    auto rawArray = std::vector<IntrospectableViewObject>{
            {createIntrospectableViewObject("1", 13), createIntrospectableViewObject("2", 42)}};
    auto introspectableViewPtr = introspectableViewArray(rawArray);
    checkArray(rawArray, introspectableViewPtr,
            [&](const IntrospectableViewObject& value, const IIntrospectableViewPtr& elementIntrospectableView) {
                checkCompound(value, elementIntrospectableView);
            });
    checkArray(rawArray, static_cast<IIntrospectableViewConstPtr>(introspectableViewPtr),
            [&](const IntrospectableViewObject& value, const IIntrospectableViewConstPtr& elementIntrospectableView) {
                checkCompound(value, elementIntrospectableView);
            });

    introspectableViewPtr->resize(introspectableViewPtr->size() + 1);
    IIntrospectableViewPtr newCompound = introspectableViewPtr->at(introspectableViewPtr->size() - 1);
    ASSERT_TRUE(newCompound);

    introspectableViewPtr->setAt(Any(createIntrospectableViewObject("test", 0)), 0);
    ASSERT_EQ(0, introspectableViewPtr->at(0)->find("introspectableViewNested.value")->getUInt32());
    introspectableViewPtr->append(Any(createIntrospectableViewObject("test|", 1)));
    ASSERT_EQ(1, introspectableViewPtr->at(introspectableViewPtr->size() - 1)->find("introspectableViewNested.value")->getUInt32());

    const size_t size = introspectableViewPtr->size();
    ASSERT_THROW(introspectableViewPtr->setAt(Any(), size), CppRuntimeException); // out of range
}

TEST_F(IntrospectableViewTest, defaultUnimplementedMethods)
{
    class IntrospectableView : public IntrospectableViewBase<std::allocator<uint8_t>>
    {
    public:
        using IntrospectableViewBase<std::allocator<uint8_t>>::IntrospectableViewBase;
    };

    IntrospectableView introspectableView(typeInfo<UInt8>());
    ASSERT_THROW(introspectableView.getAnyValue(), CppRuntimeException);

    const IntrospectableView& constIntrospectableViewRef = introspectableView;
    ASSERT_THROW(constIntrospectableViewRef.getAnyValue(), CppRuntimeException);
}

TEST_F(IntrospectableViewTest, introspectableViewOwner)
{
    auto introspectableView = typeInfo<IntrospectableViewObject>().createInstance();

    IIntrospectableViewConstPtr constIntrospectableView = introspectableView;

    IntrospectableViewObject defaultIntrospectableViewObject;
    ASSERT_EQ(defaultIntrospectableViewObject,
            introspectableView->getAnyValue().template get<std::reference_wrapper<IntrospectableViewObject>>().get());
    ASSERT_EQ(defaultIntrospectableViewObject,
            constIntrospectableView->getAnyValue().template get<std::reference_wrapper<IntrospectableViewObject>>().get());

    ASSERT_TRUE(TypeInfoUtil::isCompound(introspectableView->getTypeInfo().getSchemaType()));
    ASSERT_FALSE(introspectableView->isArray());
    introspectableView->setField("introspectableViewNested", Any(IntrospectableViewNested{42}));
    ASSERT_EQ(42, introspectableView->getField("introspectableViewNested")->getField("value")->getUInt32());
    ASSERT_EQ(42, constIntrospectableView->getField("introspectableViewNested")->getField("value")->getUInt32());
    ASSERT_THROW(introspectableView->createField("nonexistent"), CppRuntimeException);
    ASSERT_THROW(introspectableView->getChoice(), CppRuntimeException);
    ASSERT_THROW(constIntrospectableView->getChoice(), CppRuntimeException);
    ASSERT_FALSE(introspectableView->find("nonexistent"));
    ASSERT_FALSE(constIntrospectableView->find("nonexistent"));
    ASSERT_FALSE((*introspectableView)["nonexistent"]);
    ASSERT_FALSE((*constIntrospectableView)["nonexistent"]);

    ASSERT_THROW(introspectableView->size(), CppRuntimeException); // not an array
    ASSERT_THROW(introspectableView->resize(0), CppRuntimeException); // not an array
    ASSERT_THROW(introspectableView->at(0), CppRuntimeException); // not an array
    ASSERT_THROW(constIntrospectableView->at(0), CppRuntimeException); // not an array
    ASSERT_THROW((*introspectableView)[0], CppRuntimeException); // not an array
    ASSERT_THROW((*constIntrospectableView)[0], CppRuntimeException); // not an array
    ASSERT_THROW(introspectableView->setAt(Any(), 0), CppRuntimeException); // not an array
    ASSERT_THROW(introspectableView->append(Any()), CppRuntimeException); // not an array

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
}
#endif // !@#
} // namespace zserio
