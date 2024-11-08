#include <string>

#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "union_types/simple_union/SimpleUnion.h"

namespace union_types
{
namespace simple_union
{

using AllocatorType = SimpleUnion::AllocatorType;
using StringType = zserio::BasicString<AllocatorType>;
using ChoiceTag = SimpleUnion::ChoiceTag;

class SimpleUnionTest : public ::testing::Test
{
protected:
    static constexpr int8_t CASE1_FIELD = 13;
    static constexpr uint16_t CASE2_FIELD = 65535;
    static constexpr std::string_view CASE3_FIELD = "SimpleUnion";
    static constexpr int8_t CASE4_FIELD = 42;
};

TEST_F(SimpleUnionTest, constructors)
{
    {
        SimpleUnion data;
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        SimpleUnion data = {};
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        SimpleUnion data(AllocatorType{});
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, data.index());
    }
    {
        SimpleUnion data;
        zserio::View<SimpleUnion> view(data);
        ASSERT_EQ(ChoiceTag::UNDEFINED_CHOICE, view.zserioChoiceTag());
    }
}

TEST_F(SimpleUnionTest, zserioChoiceTag)
{
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case1Field>(CASE1_FIELD);
        zserio::View<SimpleUnion> view(data);
        ASSERT_EQ(ChoiceTag::CHOICE_case1Field, view.zserioChoiceTag());
    }
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case2Field>(CASE2_FIELD);
        zserio::View<SimpleUnion> view(data);
        ASSERT_EQ(ChoiceTag::CHOICE_case2Field, view.zserioChoiceTag());
    }
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case3Field>(CASE3_FIELD);
        zserio::View<SimpleUnion> view(data);
        ASSERT_EQ(ChoiceTag::CHOICE_case3Field, view.zserioChoiceTag());
    }
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case4Field>(CASE4_FIELD);
        zserio::View<SimpleUnion> view(data);
        ASSERT_EQ(ChoiceTag::CHOICE_case4Field, view.zserioChoiceTag());
    }
}

TEST_F(SimpleUnionTest, case1Field)
{
    SimpleUnion data;
    data.emplace<ChoiceTag::CHOICE_case1Field>(CASE1_FIELD);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case2Field>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case3Field>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case4Field>(data), zserio::BadVariantAccess);
    ASSERT_EQ(CASE1_FIELD, zserio::get<ChoiceTag::CHOICE_case1Field>(data));

    zserio::View<SimpleUnion> view(data);
    ASSERT_THROW(view.case2Field(), zserio::BadVariantAccess);
    ASSERT_THROW(view.case3Field(), zserio::BadVariantAccess);
    ASSERT_THROW(view.case4Field(), zserio::BadVariantAccess);
    ASSERT_EQ(CASE1_FIELD, view.case1Field());
}

TEST_F(SimpleUnionTest, case2Field)
{
    SimpleUnion data;
    data.emplace<ChoiceTag::CHOICE_case2Field>(CASE2_FIELD);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case1Field>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case3Field>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case4Field>(data), zserio::BadVariantAccess);
    ASSERT_EQ(CASE2_FIELD, zserio::get<ChoiceTag::CHOICE_case2Field>(data));

    zserio::View<SimpleUnion> view(data);
    ASSERT_THROW(view.case1Field(), zserio::BadVariantAccess);
    ASSERT_THROW(view.case3Field(), zserio::BadVariantAccess);
    ASSERT_THROW(view.case4Field(), zserio::BadVariantAccess);
    ASSERT_EQ(CASE2_FIELD, view.case2Field());
}

TEST_F(SimpleUnionTest, case3Field)
{
    SimpleUnion data;
    data.emplace<ChoiceTag::CHOICE_case3Field>(CASE3_FIELD);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case1Field>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case2Field>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case4Field>(data), zserio::BadVariantAccess);
    ASSERT_EQ(CASE3_FIELD, zserio::get<ChoiceTag::CHOICE_case3Field>(data));

    zserio::View<SimpleUnion> view(data);
    ASSERT_THROW(view.case1Field(), zserio::BadVariantAccess);
    ASSERT_THROW(view.case2Field(), zserio::BadVariantAccess);
    ASSERT_THROW(view.case4Field(), zserio::BadVariantAccess);
    ASSERT_EQ(CASE3_FIELD, view.case3Field());

    StringType movedString(1000, 'a'); // long enough to prevent small string optimization
    const void* ptr = movedString.data();
    data.emplace<ChoiceTag::CHOICE_case3Field>(std::move(movedString));
    const void* movedPtr = zserio::get<ChoiceTag::CHOICE_case3Field>(data).data();
    ASSERT_EQ(ptr, movedPtr);
    StringType& case3 = zserio::get<ChoiceTag::CHOICE_case3Field>(data);
    case3 = CASE3_FIELD;
    ASSERT_EQ(CASE3_FIELD, zserio::get<ChoiceTag::CHOICE_case3Field>(data));
}

TEST_F(SimpleUnionTest, case4Field)
{
    SimpleUnion data;
    data.emplace<ChoiceTag::CHOICE_case4Field>(CASE4_FIELD);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case1Field>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case2Field>(data), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_case3Field>(data), zserio::BadVariantAccess);
    ASSERT_EQ(CASE4_FIELD, zserio::get<ChoiceTag::CHOICE_case4Field>(data));

    zserio::View<SimpleUnion> view(data);
    ASSERT_THROW(view.case1Field(), zserio::BadVariantAccess);
    ASSERT_THROW(view.case2Field(), zserio::BadVariantAccess);
    ASSERT_THROW(view.case3Field(), zserio::BadVariantAccess);
    ASSERT_EQ(CASE4_FIELD, view.case4Field());
}

TEST_F(SimpleUnionTest, comparisonOperators)
{
    SimpleUnion data;
    data.emplace<ChoiceTag::CHOICE_case2Field>(CASE2_FIELD);
    SimpleUnion equalData;
    equalData.emplace<ChoiceTag::CHOICE_case2Field>(CASE2_FIELD);
    SimpleUnion lessThenData;
    lessThenData.emplace<ChoiceTag::CHOICE_case1Field>(CASE1_FIELD);
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View<SimpleUnion> view(data);
    zserio::View<SimpleUnion> equalView(equalData);
    zserio::View<SimpleUnion> lessThenView(lessThenData);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);

    SimpleUnion anotherLessThenData;
    anotherLessThenData.emplace<ChoiceTag::CHOICE_case2Field>(CASE2_FIELD - 1);
    test_utils::comparisonOperatorsTest(data, equalData, anotherLessThenData);

    zserio::View<SimpleUnion> anotherLessThenView(anotherLessThenData);
    test_utils::comparisonOperatorsTest(view, equalView, anotherLessThenView);
}

TEST_F(SimpleUnionTest, validate)
{
    // TODO
}

TEST_F(SimpleUnionTest, bitSizeOf)
{
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case1Field>(CASE1_FIELD);
        zserio::View<SimpleUnion> view(data);
        ASSERT_EQ(8 + 8, zserio::detail::bitSizeOf(view));
    }
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case2Field>(CASE2_FIELD);
        zserio::View<SimpleUnion> view(data);
        ASSERT_EQ(8 + 16, zserio::detail::bitSizeOf(view));
    }
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case3Field>(CASE3_FIELD);
        zserio::View<SimpleUnion> view(data);
        ASSERT_EQ(8 + CASE3_FIELD.length() * 8 + 8, zserio::detail::bitSizeOf(view));
    }
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case4Field>(CASE4_FIELD);
        zserio::View<SimpleUnion> view(data);
        ASSERT_EQ(8 + 8, zserio::detail::bitSizeOf(view));
    }
}

TEST_F(SimpleUnionTest, writeRead)
{
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case1Field>(CASE1_FIELD);
        test_utils::writeReadTest(data);
    }
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case2Field>(CASE2_FIELD);
        test_utils::writeReadTest(data);
    }
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case3Field>(CASE3_FIELD);
        test_utils::writeReadTest(data);
    }
    {
        SimpleUnion data;
        data.emplace<ChoiceTag::CHOICE_case4Field>(CASE4_FIELD);
        test_utils::writeReadTest(data);
    }
}

TEST_F(SimpleUnionTest, read)
{
    zserio::BitBuffer bitBuffer = zserio::BitBuffer(8 + CASE3_FIELD.length() * 8 + 8);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, ::zserio::VarSize(ChoiceTag::CHOICE_case3Field));
    zserio::detail::write(writer, CASE3_FIELD);

    SimpleUnion expectedReadData(zserio::in_place_index<ChoiceTag::CHOICE_case3Field>, CASE3_FIELD);
    test_utils::readTest(writer, expectedReadData);
}

TEST_F(SimpleUnionTest, stdHash)
{
    SimpleUnion data;
    data.emplace<ChoiceTag::CHOICE_case1Field>(CASE1_FIELD);
    const size_t dataHash = 31537; // hardcoded value to check that the hash code is stable

    SimpleUnion equalData;
    equalData.emplace<ChoiceTag::CHOICE_case1Field>(CASE1_FIELD);

    SimpleUnion diffData;
    diffData.emplace<ChoiceTag::CHOICE_case4Field>(CASE4_FIELD);
    const size_t diffDataHash = 31677; // hardcoded value to check that the hash code is stable

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View<SimpleUnion> view(data);
    const size_t viewHash = 864; // hardcoded value to check that the hash code is stable
    zserio::View<SimpleUnion> equalView(equalData);
    zserio::View<SimpleUnion> diffView(diffData);
    const size_t diffHash = 893; // hardcoded value to check that the hash code is stable
    test_utils::hashTest(view, viewHash, equalView, diffView, diffHash);
}

} // namespace simple_union
} // namespace union_types
