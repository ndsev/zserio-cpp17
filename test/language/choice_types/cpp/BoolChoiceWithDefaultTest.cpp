#include "choice_types/bool_choice_with_default/BoolChoiceWithDefault.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/ChoiceCaseException.h"

namespace choice_types
{
namespace bool_choice_with_default
{

using AllocatorType = BoolChoiceWithDefault::allocator_type;

class BoolChoiceWithDefaultTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, bool /*selector*/, uint8_t value)
    {
        writer.writeUnsignedBits32(value, 8);
    }
};

TEST_F(BoolChoiceWithDefaultTest, constructors)
{
    {
        BoolChoiceWithDefault data;
        ASSERT_EQ(BoolChoiceWithDefault::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        BoolChoiceWithDefault data = {};
        ASSERT_EQ(BoolChoiceWithDefault::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        BoolChoiceWithDefault data(AllocatorType{});
        ASSERT_EQ(BoolChoiceWithDefault::Tag::ZSERIO_UNDEFINED, data.index());
    }
    {
        BoolChoiceWithDefault data{zserio::in_place_index<BoolChoiceWithDefault::Tag::field>, 99};
        zserio::View view(data, true);
        ASSERT_EQ(BoolChoiceWithDefault::Tag::field, view.zserioChoiceTag());
    }
}

TEST_F(BoolChoiceWithDefaultTest, validate)
{
    BoolChoiceWithDefault data(zserio::in_place_index<BoolChoiceWithDefault::Tag::field>, 99);
    ASSERT_NO_THROW(zserio::detail::validate(zserio::View(data, true)));
    data.emplace<BoolChoiceWithDefault::Tag::ZSERIO_UNDEFINED>();
    ASSERT_THROW(zserio::detail::validate(zserio::View(data, true)), zserio::ChoiceCaseException);
}

TEST_F(BoolChoiceWithDefaultTest, bitSizeOf)
{
    BoolChoiceWithDefault data(zserio::in_place_index<BoolChoiceWithDefault::Tag::field>, 99);
    zserio::View view(data, true);
    ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
}

TEST_F(BoolChoiceWithDefaultTest, comparisonOperators)
{
    BoolChoiceWithDefault data;
    data.emplace<BoolChoiceWithDefault::Tag::field>(234);
    BoolChoiceWithDefault equalData;
    equalData.emplace<BoolChoiceWithDefault::Tag::field>(234);
    BoolChoiceWithDefault lessThanData;
    lessThanData.emplace<BoolChoiceWithDefault::Tag::field>(233);
    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data, false);
    zserio::View equalView(equalData, false);
    zserio::View lessThanView(lessThanData, false);
    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(BoolChoiceWithDefaultTest, writeRead)
{
    zserio::Bool selector = false;
    BoolChoiceWithDefault data(zserio::in_place_index<BoolChoiceWithDefault::Tag::field>, 99);
    test_utils::writeReadTest(data, selector);
}

TEST_F(BoolChoiceWithDefaultTest, read)
{
    const zserio::Bool selector = true;
    const int8_t value = 99;
    BoolChoiceWithDefault data(zserio::in_place_index<BoolChoiceWithDefault::Tag::field>, value);

    test_utils::readTest(
            [&](auto& writer) {
                writeData(writer, selector, value);
            },
            data, selector);
}

TEST_F(BoolChoiceWithDefaultTest, stdHash)
{
    BoolChoiceWithDefault data(zserio::in_place_index<BoolChoiceWithDefault::Tag::field>, 99);
    BoolChoiceWithDefault data2 = data;
    const size_t hashValue = 31623;
    test_utils::hashTest(data, hashValue, data2);

    zserio::View view(data, true);
    zserio::View view2(data, true);
    test_utils::hashTest(view, hashValue, view2);
}

} // namespace bool_choice_with_default
} // namespace choice_types
