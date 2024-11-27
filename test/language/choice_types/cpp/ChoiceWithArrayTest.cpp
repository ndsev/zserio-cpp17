#include <string_view>

#include "choice_types/choice_with_array/TestChoice.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace choice_types
{
namespace choice_with_array
{

using AllocatorType = TestChoice::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;
using ChoiceTag = TestChoice::ChoiceTag;

class ChoiceWithArrayTest : public ::testing::Test
{
protected:
    static constexpr std::string_view BLOB_NAME_BASE = "language/choice_types/choice_with_array_array";
    static constexpr zserio::Int8 ARRAY8_SELECTOR = 8;
    static constexpr zserio::Int8 ARRAY16_SELECTOR = 16;
};

TEST_F(ChoiceWithArrayTest, array8)
{
    TestChoice data;
    const VectorType<Data8> array8(4);
    data.emplace<ChoiceTag::CHOICE_array8>(array8);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_array16>(data), zserio::BadVariantAccess);
    ASSERT_EQ(array8, zserio::get<ChoiceTag::CHOICE_array8>(data));

    zserio::View view(data, ARRAY8_SELECTOR);
    ASSERT_THROW(view.array16(), zserio::BadVariantAccess);
    ASSERT_EQ(array8.size(), view.array8().size());
    for (size_t i = 0; i < array8.size(); ++i)
    {
        ASSERT_EQ(array8.at(i).data, view.array8().at(i).data());
    }
}

TEST_F(ChoiceWithArrayTest, array16)
{
    TestChoice data;
    const VectorType<zserio::Int16> array16(4);
    data.emplace<ChoiceTag::CHOICE_array16>(array16);
    ASSERT_THROW(zserio::get<ChoiceTag::CHOICE_array8>(data), zserio::BadVariantAccess);
    ASSERT_EQ(array16, zserio::get<ChoiceTag::CHOICE_array16>(data));

    zserio::View view(data, ARRAY16_SELECTOR);
    ASSERT_THROW(view.array8(), zserio::BadVariantAccess);
    ASSERT_EQ(array16.size(), view.array16().size());
    for (size_t i = 0; i < array16.size(); ++i)
    {
        ASSERT_EQ(array16.at(i), view.array16().at(i));
    }
}

TEST_F(ChoiceWithArrayTest, writeReadFile)
{
    {
        const VectorType<Data8> array8{Data8{1}, Data8{2}, Data8{3}, Data8{4}};
        TestChoice data(zserio::in_place_index<ChoiceTag::CHOICE_array8>, array8);
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "8.blob", data, ARRAY8_SELECTOR);
    }
    {
        const VectorType<zserio::Int16> array16{10, 20, 30, 40, 50};
        TestChoice data(zserio::in_place_index<ChoiceTag::CHOICE_array16>, array16);
        test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "16.blob", data, ARRAY16_SELECTOR);
    }
}

} // namespace choice_with_array
} // namespace choice_types
