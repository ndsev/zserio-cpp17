#include <limits>
#include <string_view>

#include "choice_types/multiple_param_choice/TestChoice.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ChoiceCaseException.h"
#include "zserio/RebindAlloc.h"

namespace choice_types
{
namespace multiple_param_choice
{

using AllocatorType = TestChoice::allocator_type;

template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class MultipleParamChoiceTest : public ::testing::Test
{
protected:
    static constexpr std::string_view BLOB_NAME_BASE = "language/choice_types/multiple_param_choice_";
    static constexpr zserio::UInt7 ARRAY5_SELECTOR = 5;
    static constexpr zserio::UInt7 ARRAY13_SELECTOR = 13;
    static constexpr zserio::UInt7 FIELD17_SELECTOR = 17;
    static constexpr zserio::UInt7 DYN_BIT_FIELD_SELECTOR = 20;
};

TEST_F(MultipleParamChoiceTest, selector)
{
    TestChoice data;
    const VectorType<zserio::UInt5> value = {1, 10, 15};
    data.emplace<TestChoice::Tag::array5>(value);
    zserio::View viewA(data, ARRAY5_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    ASSERT_EQ(ARRAY5_SELECTOR, viewA.selector());

    zserio::View viewB(data, ARRAY13_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    ASSERT_EQ(ARRAY13_SELECTOR, viewB.selector());

    zserio::View viewC(data, FIELD17_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    ASSERT_EQ(FIELD17_SELECTOR, viewC.selector());

    zserio::View viewD(data, DYN_BIT_FIELD_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    ASSERT_EQ(DYN_BIT_FIELD_SELECTOR, viewD.selector());
}

TEST_F(MultipleParamChoiceTest, arrayLength)
{
    TestChoice data;
    const VectorType<zserio::UInt5> value = {1, 10, 15};
    data.emplace<TestChoice::Tag::array5>(value);
    zserio::View view(data, ARRAY5_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    ASSERT_EQ(static_cast<uint32_t>(value.size()), view.arrayLength());
}

TEST_F(MultipleParamChoiceTest, bitFieldLength)
{
    TestChoice data;
    const VectorType<zserio::UInt5> value = {1, 10, 15};
    data.emplace<TestChoice::Tag::array5>(value);
    zserio::View view(data, ARRAY5_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    ASSERT_EQ(1, view.bitFieldLength());
}

TEST_F(MultipleParamChoiceTest, zserioChoiceTag)
{
    {
        TestChoice data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<TestChoice::Tag::array5>(value);
        zserio::View view(data, ARRAY5_SELECTOR, static_cast<uint32_t>(value.size()), 1);
        ASSERT_EQ(TestChoice::Tag::array5, view.zserioChoiceTag());
    }
    {
        TestChoice data;
        const VectorType<Data13> value = {
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
        };
        data.emplace<TestChoice::Tag::array13>(value);
        zserio::View view(data, ARRAY13_SELECTOR, static_cast<uint32_t>(value.size()), 1);
        ASSERT_EQ(TestChoice::Tag::array13, view.zserioChoiceTag());
    }
    {
        TestChoice data;
        const zserio::Int17 value = 65535;
        data.emplace<TestChoice::Tag::field17>(value);
        zserio::View view(data, FIELD17_SELECTOR, 1, 1);
        ASSERT_EQ(TestChoice::Tag::field17, view.zserioChoiceTag());
    }
    {
        TestChoice data;
        const zserio::DynInt16 value = 16383;
        data.emplace<TestChoice::Tag::dynBitField>(value);
        zserio::View view(data, DYN_BIT_FIELD_SELECTOR, 1, 15);
        ASSERT_EQ(TestChoice::Tag::dynBitField, view.zserioChoiceTag());
    }
}

TEST_F(MultipleParamChoiceTest, array5)
{
    TestChoice data;
    const VectorType<zserio::UInt5> value = {1, 10, 15};
    data.emplace<TestChoice::Tag::array5>(value);
    ASSERT_THROW(zserio::get<TestChoice::Tag::array13>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestChoice::Tag::field17>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestChoice::Tag::dynBitField>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<TestChoice::Tag::array5>(data));

    zserio::View view(data, ARRAY5_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    ASSERT_THROW(view.array13(), zserio::BadVariantAccess);
    ASSERT_THROW(view.field17(), zserio::BadVariantAccess);
    ASSERT_THROW(view.dynBitField(), zserio::BadVariantAccess);
    zserio::ArrayView<const zserio::UInt5> expectedArray5View(value);
    ASSERT_EQ(expectedArray5View, view.array5());
}

TEST_F(MultipleParamChoiceTest, array13)
{
    TestChoice data;
    const VectorType<Data13> value = {
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
    };
    data.emplace<TestChoice::Tag::array13>(value);
    ASSERT_THROW(zserio::get<TestChoice::Tag::array5>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestChoice::Tag::field17>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestChoice::Tag::dynBitField>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<TestChoice::Tag::array13>(data));

    zserio::View view(data, ARRAY13_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    ASSERT_THROW(view.array5(), zserio::BadVariantAccess);
    ASSERT_THROW(view.field17(), zserio::BadVariantAccess);
    ASSERT_THROW(view.dynBitField(), zserio::BadVariantAccess);
    zserio::ArrayView<const Data13, zserio::View<TestChoice>::ZserioArray13ArrayTraits> expectedArray13View(
            value, view);
    ASSERT_EQ(expectedArray13View, view.array13());
}

TEST_F(MultipleParamChoiceTest, field17)
{
    TestChoice data;
    const zserio::Int17 value = 65535;
    data.emplace<TestChoice::Tag::field17>(value);
    ASSERT_THROW(zserio::get<TestChoice::Tag::array5>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestChoice::Tag::array13>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestChoice::Tag::dynBitField>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<TestChoice::Tag::field17>(data));

    zserio::View view(data, FIELD17_SELECTOR, 1, 1);
    ASSERT_THROW(view.array5(), zserio::BadVariantAccess);
    ASSERT_THROW(view.array13(), zserio::BadVariantAccess);
    ASSERT_THROW(view.dynBitField(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.field17());
}

TEST_F(MultipleParamChoiceTest, dynBitField)
{
    TestChoice data;
    const zserio::DynInt16 value = 16383;
    data.emplace<TestChoice::Tag::dynBitField>(value);
    ASSERT_THROW(zserio::get<TestChoice::Tag::array5>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestChoice::Tag::array13>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestChoice::Tag::field17>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<TestChoice::Tag::dynBitField>(data));

    zserio::View view(data, DYN_BIT_FIELD_SELECTOR, 1, 15);
    ASSERT_THROW(view.array5(), zserio::BadVariantAccess);
    ASSERT_THROW(view.array13(), zserio::BadVariantAccess);
    ASSERT_THROW(view.field17(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.dynBitField());
}

TEST_F(MultipleParamChoiceTest, comparisonOperators)
{
    TestChoice data;
    data.emplace<TestChoice::Tag::field17>(2);
    TestChoice equalData;
    equalData.emplace<TestChoice::Tag::field17>(2);
    TestChoice lessThenData;
    lessThenData.emplace<TestChoice::Tag::field17>(0);
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    const VectorType<zserio::UInt5> value = {1, 10, 15};
    zserio::View view(data, FIELD17_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    zserio::View equalView(equalData, FIELD17_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    zserio::View lessThenView(lessThenData, FIELD17_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);

    TestChoice anotherLessThenData;
    anotherLessThenData.emplace<TestChoice::Tag::array5>(value);
    test_utils::comparisonOperatorsTest(data, equalData, anotherLessThenData);

    zserio::View anotherLessThenView(
            anotherLessThenData, ARRAY5_SELECTOR, static_cast<uint32_t>(value.size()), 1);
    test_utils::comparisonOperatorsTest(view, equalView, anotherLessThenView);
}

TEST_F(MultipleParamChoiceTest, validate)
{
    {
        // parameter selector is out of range
        TestChoice data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<TestChoice::Tag::array5>(value);
        zserio::View view(data, 255, static_cast<uint32_t>(value.size()), 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // parameter arrayLength is out of range
        TestChoice data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<TestChoice::Tag::array5>(value);
        zserio::View view(data, ARRAY5_SELECTOR, std::numeric_limits<uint32_t>::max(), 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // parameter bitFieldLength is out of range
        TestChoice data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<TestChoice::Tag::array5>(value);
        zserio::View view(data, ARRAY5_SELECTOR, static_cast<uint32_t>(value.size()), 16);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // field array5 is out of range
        TestChoice data;
        const VectorType<zserio::UInt5> value = {1, 10, 255};
        data.emplace<TestChoice::Tag::array5>(value);
        zserio::View view1(data, ARRAY13_SELECTOR, static_cast<uint32_t>(value.size()), 1);
        ASSERT_THROW(zserio::detail::validate(view1), zserio::ChoiceCaseException);
        zserio::View view2(data, ARRAY5_SELECTOR, static_cast<uint32_t>(value.size()), 1);
        ASSERT_THROW(zserio::detail::validate(view2), zserio::OutOfRangeException);
    }
    {
        // field array5 has wrong length
        TestChoice data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<TestChoice::Tag::array5>(value);
        zserio::View view(data, ARRAY5_SELECTOR, 1, 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::ArrayLengthException);
    }
    {
        // field array13 is out of range
        TestChoice data;
        const VectorType<Data13> value = {
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 32767}},
        };
        data.emplace<TestChoice::Tag::array13>(value);
        zserio::View view1(data, ARRAY5_SELECTOR, static_cast<uint32_t>(value.size()), 1);
        ASSERT_THROW(zserio::detail::validate(view1), zserio::ChoiceCaseException);
        zserio::View view2(data, ARRAY13_SELECTOR, static_cast<uint32_t>(value.size()), 1);
        ASSERT_THROW(zserio::detail::validate(view2), zserio::OutOfRangeException);
    }
    {
        // field array13 has wrong length
        TestChoice data;
        const VectorType<Data13> value = {
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
        };
        data.emplace<TestChoice::Tag::array13>(value);
        zserio::View view(data, ARRAY13_SELECTOR, 1, 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::ArrayLengthException);
    }
    {
        // field field17 is out of range
        TestChoice data;
        const zserio::Int17 value = 131072;
        data.emplace<TestChoice::Tag::field17>(value);
        zserio::View view1(data, ARRAY5_SELECTOR, 1, 1);
        ASSERT_THROW(zserio::detail::validate(view1), zserio::ChoiceCaseException);
        zserio::View view2(data, FIELD17_SELECTOR, 1, 1);
        ASSERT_THROW(zserio::detail::validate(view2), zserio::OutOfRangeException);
    }
    {
        // field dynBitField is out of range
        TestChoice data;
        const zserio::DynInt16 value = 32767;
        data.emplace<TestChoice::Tag::dynBitField>(value);
        zserio::View view1(data, FIELD17_SELECTOR, 1, 15);
        ASSERT_THROW(zserio::detail::validate(view1), zserio::ChoiceCaseException);
        zserio::View view2(data, DYN_BIT_FIELD_SELECTOR, 1, 15);
        ASSERT_THROW(zserio::detail::validate(view2), zserio::OutOfRangeException);
    }
}

TEST_F(MultipleParamChoiceTest, writeRead)
{
    {
        TestChoice data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<TestChoice::Tag::array5>(value);
        test_utils::writeReadTest(
                data, ARRAY5_SELECTOR, zserio::VarSize{static_cast<uint32_t>(value.size())}, zserio::UInt4{1});
    }
    {
        TestChoice data;
        const VectorType<Data13> value = {
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
        };
        data.emplace<TestChoice::Tag::array13>(value);
        test_utils::writeReadTest(
                data, ARRAY13_SELECTOR, zserio::VarSize{static_cast<uint32_t>(value.size())}, zserio::UInt4{1});
    }
    {
        TestChoice data;
        const zserio::Int17 value = 65535;
        data.emplace<TestChoice::Tag::field17>(value);
        test_utils::writeReadTest(data, FIELD17_SELECTOR, zserio::VarSize{1}, zserio::UInt4{1});
    }
    {
        TestChoice data;
        const zserio::DynInt16 value = 16383;
        data.emplace<TestChoice::Tag::dynBitField>(value);
        test_utils::writeReadTest(data, DYN_BIT_FIELD_SELECTOR, zserio::VarSize{1}, zserio::UInt4{15});
    }
}

TEST_F(MultipleParamChoiceTest, writeReadFile)
{
    {
        TestChoice data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<TestChoice::Tag::array5>(value);
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "array5.blob", data, ARRAY5_SELECTOR,
                zserio::VarSize{static_cast<uint32_t>(value.size())}, zserio::UInt4{1});
    }
    {
        TestChoice data;
        const VectorType<Data13> value = {
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
        };
        data.emplace<TestChoice::Tag::array13>(value);
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "array13.blob", data, ARRAY13_SELECTOR,
                zserio::VarSize{static_cast<uint32_t>(value.size())}, zserio::UInt4{1});
    }
    {
        TestChoice data;
        const zserio::Int17 value = 65535;
        data.emplace<TestChoice::Tag::field17>(value);
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "field17.blob", data, FIELD17_SELECTOR,
                zserio::VarSize{1}, zserio::UInt4{1});
    }
    {
        TestChoice data;
        const zserio::DynInt16 value = 16383;
        data.emplace<TestChoice::Tag::dynBitField>(value);
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "dynBitField.blob", data,
                DYN_BIT_FIELD_SELECTOR, zserio::VarSize{1}, zserio::UInt4{15});
    }
}

} // namespace multiple_param_choice
} // namespace choice_types
