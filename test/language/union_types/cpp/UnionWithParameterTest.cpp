#include <limits>
#include <string>
#include <string_view>

#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "union_types/union_with_parameter/TestUnion.h"
#include "zserio/ConstraintException.h"
#include "zserio/RebindAlloc.h"
#include "zserio/UnionCaseException.h"

namespace union_types
{
namespace union_with_parameter
{

TEST(UnionWithParameterTest, case1Allowed)
{
    TestUnion data;
    const zserio::Int32 value = 1;
    data.emplace<TestUnion::Tag::case1Field>(value);
    zserio::View view(data, true);
    ASSERT_EQ(true, view.case1Allowed());
}

TEST(UnionWithParameterTest, zserioChoiceTag)
{
    {
        TestUnion data;
        const zserio::Int32 value = 1;
        data.emplace<TestUnion::Tag::case1Field>(value);
        zserio::View view(data, true);
        ASSERT_EQ(TestUnion::Tag::case1Field, view.zserioChoiceTag());
    }
    {
        TestUnion data;
        const zserio::Int16 value = 2;
        data.emplace<TestUnion::Tag::case2Field>(value);
        zserio::View view(data, false);
        ASSERT_EQ(TestUnion::Tag::case2Field, view.zserioChoiceTag());
    }
    {
        TestUnion data;
        const zserio::Int8 value = 3;
        data.emplace<TestUnion::Tag::case3Field>(value);
        zserio::View view(data, false);
        ASSERT_EQ(TestUnion::Tag::case3Field, view.zserioChoiceTag());
    }
}

TEST(UnionWithParameterTest, case1Field)
{
    TestUnion data;
    const zserio::Int32 value = 1;
    data.emplace<TestUnion::Tag::case1Field>(value);
    ASSERT_THROW(zserio::get<TestUnion::Tag::case2Field>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestUnion::Tag::case3Field>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<TestUnion::Tag::case1Field>(data));

    zserio::View view(data, true);
    ASSERT_THROW(view.case2Field(), zserio::BadVariantAccess);
    ASSERT_THROW(view.case3Field(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.case1Field());
}

TEST(UnionWithParameterTest, case2Field)
{
    TestUnion data;
    const zserio::Int16 value = 2;
    data.emplace<TestUnion::Tag::case2Field>(value);
    ASSERT_THROW(zserio::get<TestUnion::Tag::case1Field>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestUnion::Tag::case3Field>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<TestUnion::Tag::case2Field>(data));

    zserio::View view(data, false);
    ASSERT_THROW(view.case1Field(), zserio::BadVariantAccess);
    ASSERT_THROW(view.case3Field(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.case2Field());
}

TEST(UnionWithParameterTest, case3Field)
{
    TestUnion data;
    const zserio::Int8 value = 3;
    data.emplace<TestUnion::Tag::case3Field>(value);
    ASSERT_THROW(zserio::get<TestUnion::Tag::case1Field>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestUnion::Tag::case2Field>(data), zserio::BadVariantAccess);
    ASSERT_EQ(value, zserio::get<TestUnion::Tag::case3Field>(data));

    zserio::View view(data, false);
    ASSERT_THROW(view.case1Field(), zserio::BadVariantAccess);
    ASSERT_THROW(view.case2Field(), zserio::BadVariantAccess);
    ASSERT_EQ(value, view.case3Field());
}

TEST(UnionWithParameterTest, comparisonOperators)
{
    TestUnion data;
    data.emplace<TestUnion::Tag::case3Field>(2);
    TestUnion equalData;
    equalData.emplace<TestUnion::Tag::case3Field>(2);
    TestUnion lessThenData;
    lessThenData.emplace<TestUnion::Tag::case3Field>(0);
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data, false);
    zserio::View equalView(equalData, false);
    zserio::View lessThenView(lessThenData, false);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);

    TestUnion anotherLessThenData;
    anotherLessThenData.emplace<TestUnion::Tag::case1Field>(3);
    test_utils::comparisonOperatorsTest(data, equalData, anotherLessThenData);

    zserio::View anotherLessThenView(anotherLessThenData, false);
    test_utils::comparisonOperatorsTest(view, equalView, anotherLessThenView);
}

TEST(UnionWithParameterTest, validate)
{
    {
        // initialized union
        TestUnion data;
        zserio::View view(data, false);
        ASSERT_THROW(zserio::detail::validate(view), zserio::UnionCaseException);
    }
    {
        // field case1Field has wrong constraint
        TestUnion data;
        const zserio::Int32 value = 1;
        data.emplace<TestUnion::Tag::case1Field>(value);
        zserio::View view(data, false);
        // TODO[mikir] ASSERT_THROW(zserio::detail::validate(view), zserio::ConstraintException);
    }
}

TEST(UnionWithParameterTest, bitSizeOf)
{
    {
        TestUnion data;
        data.emplace<TestUnion::Tag::case1Field>(1);
        zserio::View view(data, true);
        ASSERT_EQ(8 + 32, zserio::detail::bitSizeOf(view));
    }
    {
        TestUnion data;
        data.emplace<TestUnion::Tag::case2Field>(1);
        zserio::View view(data, false);
        ASSERT_EQ(8 + 16, zserio::detail::bitSizeOf(view));
    }
    {
        TestUnion data;
        data.emplace<TestUnion::Tag::case3Field>(1);
        zserio::View view(data, false);
        ASSERT_EQ(8 + 8, zserio::detail::bitSizeOf(view));
    }
}

TEST(UnionWithParameterTest, writeRead)
{
    {
        TestUnion data;
        data.emplace<TestUnion::Tag::case1Field>(32);
        test_utils::writeReadTest(data, zserio::Bool(true));
    }
    {
        TestUnion data;
        data.emplace<TestUnion::Tag::case2Field>(16);
        test_utils::writeReadTest(data, zserio::Bool(false));
    }
    {
        TestUnion data;
        data.emplace<TestUnion::Tag::case3Field>(8);
        test_utils::writeReadTest(data, zserio::Bool(false));
    }
}

} // namespace union_with_parameter
} // namespace union_types
