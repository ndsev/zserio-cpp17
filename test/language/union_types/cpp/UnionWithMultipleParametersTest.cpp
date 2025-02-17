#include <limits>
#include <string>
#include <string_view>

#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "union_types/union_with_multiple_parameters/TestUnion.h"
#include "zserio/RebindAlloc.h"
#include "zserio/UnionCaseException.h"

namespace union_types
{
namespace union_with_multiple_parameters
{

using AllocatorType = TestUnion::AllocatorType;
using ChoiceTag = TestUnion::ChoiceTag;

template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class UnionWithMultipleParametersTest : public ::testing::Test
{
protected:
    static constexpr std::string_view BLOB_NAME_BASE = "language/union_types/union_with_multiple_parameters_";
};

TEST_F(UnionWithMultipleParametersTest, arrayLength)
{
    TestUnion data;
    const VectorType<zserio::UInt5> value = {1, 10, 15};
    data.emplace<ChoiceTag::CHOICE_array5>(value);
    zserio::View view(data, static_cast<uint32_t>(value.size()), 1);
    ASSERT_EQ(static_cast<uint32_t>(value.size()), view.arrayLength());
}

TEST_F(UnionWithMultipleParametersTest, bitFieldLength)
{
    TestUnion data;
    const VectorType<zserio::UInt5> value = {1, 10, 15};
    data.emplace<ChoiceTag::CHOICE_array5>(value);
    zserio::View view(data, static_cast<uint32_t>(value.size()), 1);
    ASSERT_EQ(1, view.bitFieldLength());
}

TEST_F(UnionWithMultipleParametersTest, zserioChoiceTag)
{
    {
        TestUnion data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<ChoiceTag::CHOICE_array5>(value);
        zserio::View view(data, static_cast<uint32_t>(value.size()), 1);
        ASSERT_EQ(ChoiceTag::CHOICE_array5, view.zserioChoiceTag());
    }
    {
        TestUnion data;
        const VectorType<Data13> value = {
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
        };
        data.emplace<ChoiceTag::CHOICE_array13>(value);
        zserio::View view(data, static_cast<uint32_t>(value.size()), 1);
        ASSERT_EQ(ChoiceTag::CHOICE_array13, view.zserioChoiceTag());
    }
    {
        TestUnion data;
        const zserio::Int17 value = 65535;
        data.emplace<ChoiceTag::CHOICE_field17>(value);
        zserio::View view(data, 1, 1);
        ASSERT_EQ(ChoiceTag::CHOICE_field17, view.zserioChoiceTag());
    }
    {
        TestUnion data;
        const zserio::DynInt16<> value = 16383;
        data.emplace<ChoiceTag::CHOICE_dynBitField>(value);
        zserio::View view(data, 1, 15);
        ASSERT_EQ(ChoiceTag::CHOICE_dynBitField, view.zserioChoiceTag());
    }
}

TEST_F(UnionWithMultipleParametersTest, array5)
{
    TestUnion data;
    const VectorType<zserio::UInt5> value = {1, 10, 15};
    data.emplace<ChoiceTag::CHOICE_array5>(value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_array13>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_field17>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_dynBitField>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_array5>(data));

    zserio::View view(data, static_cast<uint32_t>(value.size()), 1);
    ASSERT_THROW(view.array13(), zserio::BadVariantAccess);
    ASSERT_THROW(view.field17(), zserio::BadVariantAccess);
    ASSERT_THROW(view.dynBitField(), zserio::BadVariantAccess);
    zserio::Array<VectorType<zserio::UInt5>, zserio::ArrayType::NORMAL> expectedArray5View(value, value.size());
    ASSERT_EQ(expectedArray5View, view.array5());
}

TEST_F(UnionWithMultipleParametersTest, array13)
{
    TestUnion data;
    const VectorType<Data13> value = {
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
    };
    data.emplace<ChoiceTag::CHOICE_array13>(value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_array5>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_field17>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_dynBitField>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_array13>(data));

    zserio::View view(data, static_cast<uint32_t>(value.size()), 1);
    ASSERT_THROW(view.array5(), zserio::BadVariantAccess);
    ASSERT_THROW(view.field17(), zserio::BadVariantAccess);
    ASSERT_THROW(view.dynBitField(), zserio::BadVariantAccess);
    zserio::Array<VectorType<Data13>, zserio::ArrayType::NORMAL, zserio::ArrayStorage::IMMUTABLE,
            zserio::View<TestUnion>::ZserioArray13ArrayTraits>
            expectedArray13View(value, view, value.size());
    ASSERT_EQ(expectedArray13View, view.array13());
}

TEST_F(UnionWithMultipleParametersTest, field17)
{
    TestUnion data;
    const zserio::Int17 value = 65535;
    data.emplace<ChoiceTag::CHOICE_field17>(value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_array5>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_array13>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_dynBitField>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_field17>(data));

    zserio::View view(data, 1, 1);
    ASSERT_THROW(view.array5(), zserio::BadVariantAccess);
    ASSERT_THROW(view.array13(), zserio::BadVariantAccess);
    ASSERT_THROW(view.dynBitField(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.field17());
}

TEST_F(UnionWithMultipleParametersTest, dynBitField)
{
    TestUnion data;
    const zserio::DynInt16<> value = 16383;
    data.emplace<ChoiceTag::CHOICE_dynBitField>(value);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_array5>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_array13>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_field17>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<ChoiceTag::CHOICE_dynBitField>(data));

    zserio::View view(data, 1, 15);
    ASSERT_THROW(view.array5(), zserio::BadVariantAccess);
    ASSERT_THROW(view.array13(), zserio::BadVariantAccess);
    ASSERT_THROW(view.field17(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.dynBitField());
}

TEST_F(UnionWithMultipleParametersTest, comparisonOperators)
{
    TestUnion data;
    data.emplace<ChoiceTag::CHOICE_field17>(2);
    TestUnion equalData;
    equalData.emplace<ChoiceTag::CHOICE_field17>(2);
    TestUnion lessThenData;
    lessThenData.emplace<ChoiceTag::CHOICE_field17>(0);
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    const VectorType<zserio::UInt5> value = {1, 10, 15};
    zserio::View view(data, static_cast<uint32_t>(value.size()), 1);
    zserio::View equalView(equalData, static_cast<uint32_t>(value.size()), 1);
    zserio::View lessThenView(lessThenData, static_cast<uint32_t>(value.size()), 1);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);

    TestUnion anotherLessThenData;
    anotherLessThenData.emplace<ChoiceTag::CHOICE_array5>(value);
    test_utils::comparisonOperatorsTest(data, equalData, anotherLessThenData);

    zserio::View anotherLessThenView(anotherLessThenData, static_cast<uint32_t>(value.size()), 1);
    test_utils::comparisonOperatorsTest(view, equalView, anotherLessThenView);
}

TEST_F(UnionWithMultipleParametersTest, validate)
{
    {
        // initialized union
        TestUnion data;
        zserio::View view(data, 1, 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::UnionCaseException);
    }
    {
        // parameter arrayLength is out of range
        TestUnion data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<ChoiceTag::CHOICE_array5>(value);
        zserio::View view(data, std::numeric_limits<uint32_t>::max(), 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // parameter bitFieldLength is out of range
        TestUnion data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<ChoiceTag::CHOICE_array5>(value);
        zserio::View view(data, static_cast<uint32_t>(value.size()), 16);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // field array5 is out of range
        TestUnion data;
        const VectorType<zserio::UInt5> value = {1, 10, 255};
        data.emplace<ChoiceTag::CHOICE_array5>(value);
        zserio::View view(data, static_cast<uint32_t>(value.size()), 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // field array5 has wrong length
        TestUnion data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<ChoiceTag::CHOICE_array5>(value);
        zserio::View view(data, 1, 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::ArrayLengthException);
    }
    {
        // field array13 is out of range
        TestUnion data;
        const VectorType<Data13> value = {
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 32767}},
        };
        data.emplace<ChoiceTag::CHOICE_array13>(value);
        zserio::View view(data, static_cast<uint32_t>(value.size()), 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // field array13 has wrong length
        TestUnion data;
        const VectorType<Data13> value = {
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
        };
        data.emplace<ChoiceTag::CHOICE_array13>(value);
        zserio::View view(data, 1, 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::ArrayLengthException);
    }
    {
        // field field17 is out of range
        TestUnion data;
        const zserio::Int17 value = 131072;
        data.emplace<ChoiceTag::CHOICE_field17>(value);
        zserio::View view(data, 1, 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // field dynBitField is out of range
        TestUnion data;
        const zserio::DynInt16<> value = 32767;
        data.emplace<ChoiceTag::CHOICE_dynBitField>(value);
        zserio::View view(data, 1, 15);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
}

TEST_F(UnionWithMultipleParametersTest, writeRead)
{
    {
        TestUnion data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<ChoiceTag::CHOICE_array5>(value);
        test_utils::writeReadTest(data, zserio::VarSize{static_cast<uint32_t>(value.size())}, zserio::UInt4{1});
    }
    {
        TestUnion data;
        const VectorType<Data13> value = {
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
        };
        data.emplace<ChoiceTag::CHOICE_array13>(value);
        test_utils::writeReadTest(data, zserio::VarSize{static_cast<uint32_t>(value.size())}, zserio::UInt4{1});
    }
    {
        TestUnion data;
        const zserio::Int17 value = 65535;
        data.emplace<ChoiceTag::CHOICE_field17>(value);
        test_utils::writeReadTest(data, zserio::VarSize{1}, zserio::UInt4{1});
    }
    {
        TestUnion data;
        const zserio::DynInt16<> value = 16383;
        data.emplace<ChoiceTag::CHOICE_dynBitField>(value);
        test_utils::writeReadTest(data, zserio::VarSize{1}, zserio::UInt4{15});
    }
}

TEST_F(UnionWithMultipleParametersTest, writeReadFile)
{
    {
        TestUnion data;
        const VectorType<zserio::UInt5> value = {1, 10, 15};
        data.emplace<ChoiceTag::CHOICE_array5>(value);
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "array5.blob", data,
                zserio::VarSize{static_cast<uint32_t>(value.size())}, zserio::UInt4{1});
    }
    {
        TestUnion data;
        const VectorType<Data13> value = {
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
        };
        data.emplace<ChoiceTag::CHOICE_array13>(value);
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "array13.blob", data,
                zserio::VarSize{static_cast<uint32_t>(value.size())}, zserio::UInt4{1});
    }
    {
        TestUnion data;
        const zserio::Int17 value = 65535;
        data.emplace<ChoiceTag::CHOICE_field17>(value);
        test_utils::writeReadFileTest(
                std::string(BLOB_NAME_BASE) + "field17.blob", data, zserio::VarSize{1}, zserio::UInt4{1});
    }
    {
        TestUnion data;
        const zserio::DynInt16<> value = 16383;
        data.emplace<ChoiceTag::CHOICE_dynBitField>(value);
        test_utils::writeReadFileTest(
                std::string(BLOB_NAME_BASE) + "dynBitField.blob", data, zserio::VarSize{1}, zserio::UInt4{15});
    }
}

} // namespace union_with_multiple_parameters
} // namespace union_types
