#include <limits>
#include <string>
#include <string_view>

#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "union_types/union_with_parameterized_field/TestUnion.h"
#include "zserio/RebindAlloc.h"
#include "zserio/UnionCaseException.h"

namespace union_types
{
namespace union_with_parameterized_field
{

using AllocatorType = TestUnion::AllocatorType;

template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

TEST(UnionWithParameterizedFieldTest, zserioChoiceTag)
{
    {
        TestUnion data;
        const zserio::UInt32 value = 1;
        data.emplace<TestUnion::Tag::field>(value);
        zserio::View view(data);
        ASSERT_EQ(TestUnion::Tag::field, view.zserioChoiceTag());
    }
    {
        TestUnion data;
        const ArrayHolder value{VectorType<zserio::UInt32>{1, 10, 11, 20, 21, 30, 31, 40, 41, 50}};
        data.emplace<TestUnion::Tag::arrayHolder>(value);
        zserio::View view(data);
        ASSERT_EQ(TestUnion::Tag::arrayHolder, view.zserioChoiceTag());
    }
}

TEST(UnionWithParameterizedFieldTest, field)
{
    TestUnion data;
    const zserio::UInt32 value = 1;
    data.emplace<TestUnion::Tag::field>(value);
    ASSERT_THROW(zserio::get<TestUnion::Tag::arrayHolder>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<TestUnion::Tag::field>(data));

    zserio::View view(data);
    ASSERT_THROW(view.arrayHolder(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.field());
}

TEST(UnionWithParameterizedFieldTest, arrayHolder)
{
    TestUnion data;
    const ArrayHolder value{VectorType<zserio::UInt32>{1, 10, 11, 20, 21, 30, 31, 40, 41, 50}};
    data.emplace<TestUnion::Tag::arrayHolder>(value);
    ASSERT_THROW(zserio::get<TestUnion::Tag::field>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<TestUnion::Tag::arrayHolder>(data));

    zserio::View view(data);
    ASSERT_THROW(view.field(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.arrayHolder().zserioData());
}

TEST(UnionWithParameterizedFieldTest, comparisonOperators)
{
    TestUnion data;
    data.emplace<TestUnion::Tag::arrayHolder>(
            ArrayHolder{VectorType<zserio::UInt32>{1, 10, 11, 20, 21, 30, 31, 40, 41, 50}});
    TestUnion equalData;
    equalData.emplace<TestUnion::Tag::arrayHolder>(
            ArrayHolder{VectorType<zserio::UInt32>{1, 10, 11, 20, 21, 30, 31, 40, 41, 50}});
    TestUnion lessThenData;
    lessThenData.emplace<TestUnion::Tag::arrayHolder>(
            ArrayHolder{VectorType<zserio::UInt32>{1, 10, 11, 20, 21, 30, 31, 40, 41, 49}});
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThenView(lessThenData);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);

    TestUnion anotherLessThenData;
    anotherLessThenData.emplace<TestUnion::Tag::arrayHolder>(
            ArrayHolder{VectorType<zserio::UInt32>{1, 10, 11, 20, 21, 30, 31, 40, 40, 50}});
    test_utils::comparisonOperatorsTest(data, equalData, anotherLessThenData);

    zserio::View anotherLessThenView(anotherLessThenData);
    test_utils::comparisonOperatorsTest(view, equalView, anotherLessThenView);
}

TEST(UnionWithParameterizedFieldTest, validate)
{
    {
        // initialized union
        TestUnion data;
        zserio::View view(data);
        ASSERT_THROW(zserio::detail::validate(view), zserio::UnionCaseException);
    }
    {
        // field array has wrong length
        TestUnion data;
        data.emplace<TestUnion::Tag::arrayHolder>(
                ArrayHolder{VectorType<zserio::UInt32>{1, 10, 11, 20, 21, 30, 31, 40, 41}});
        zserio::View view(data);
        ASSERT_THROW(zserio::detail::validate(view), zserio::ArrayLengthException);
    }
}

TEST(UnionWithParameterizedFieldTest, bitSizeOf)
{
    {
        TestUnion data;
        data.emplace<TestUnion::Tag::field>(1);
        zserio::View view(data);
        ASSERT_EQ(8 + 32, zserio::detail::bitSizeOf(view));
    }
    {
        TestUnion data;
        const ArrayHolder value{VectorType<zserio::UInt32>{1, 10, 11, 20, 21, 30, 31, 40, 41, 50}};
        data.emplace<TestUnion::Tag::arrayHolder>(value);
        zserio::View view(data);
        ASSERT_EQ(8 + 10 * 32, zserio::detail::bitSizeOf(view));
    }
}

TEST(UnionWithParameterizedFieldTest, writeRead)
{
    {
        TestUnion data;
        data.emplace<TestUnion::Tag::field>(1);
        test_utils::writeReadTest(data);
    }
    {
        TestUnion data;
        const ArrayHolder value{VectorType<zserio::UInt32>{1, 10, 11, 20, 21, 30, 31, 40, 41, 50}};
        data.emplace<TestUnion::Tag::arrayHolder>(value);
        test_utils::writeReadTest(data);
    }
}

} // namespace union_with_parameterized_field
} // namespace union_types
