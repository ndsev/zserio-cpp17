#include "gtest/gtest.h"
#include "optional_members/optional_expression/BasicColor.h"
#include "optional_members/optional_expression/Container.h"
#include "test_utils/TestUtility.h"
#include "zserio/MissedOptionalException.h"

namespace optional_members
{
namespace optional_expression
{

class OptionalExpressionTest : public ::testing::Test
{
protected:
    static void fillBlackColor(BlackColor& blackColor, uint8_t numBlackTones)
    {
        auto& tones = blackColor.tones;
        for (uint8_t i = 0; i < numBlackTones; ++i)
        {
            tones.push_back(i + 1);
        }
    }

    static void writeData(zserio::BitStreamWriter& writer, BasicColor basicColor, uint8_t numBlackTones)
    {
        writer.writeUnsignedBits32(zserio::enumToValue(basicColor), 8);

        if (basicColor == BasicColor::BLACK)
        {
            writer.writeUnsignedBits32(numBlackTones, 8);
            for (uint8_t i = 0; i < numBlackTones; ++i)
            {
                writer.writeSignedBits32(i + 1, 32);
            }
        }
    }

    static constexpr zserio::UInt8 NUM_BLACK_TONES = 2;
    static constexpr size_t CONTAINER_BIT_SIZE_WITHOUT_OPTIONAL = 8;
    static constexpr size_t CONTAINER_BIT_SIZE_WITH_OPTIONAL = 8 + 8 + 32 * NUM_BLACK_TONES;
};

TEST_F(OptionalExpressionTest, fieldConstructor)
{
    BlackColor blackColor;
    fillBlackColor(blackColor, NUM_BLACK_TONES);

    Container dataWithOptionals(BasicColor::BLACK, NUM_BLACK_TONES, blackColor);
    zserio::View viewWithOptionals(dataWithOptionals);

    ASSERT_EQ(BasicColor::BLACK, viewWithOptionals.basicColor());
    ASSERT_TRUE(viewWithOptionals.numBlackTones());
    ASSERT_EQ(NUM_BLACK_TONES, *viewWithOptionals.numBlackTones());

    ASSERT_TRUE(viewWithOptionals.blackColor());
    ASSERT_EQ(zserio::View(blackColor, NUM_BLACK_TONES), *viewWithOptionals.blackColor());

    const Container dataWithoutOptionals(BasicColor::WHITE, std::nullopt, std::nullopt);
    zserio::View viewWithoutOptionals(dataWithoutOptionals);
    ASSERT_EQ(BasicColor::WHITE, viewWithoutOptionals.basicColor());
    ASSERT_FALSE(viewWithoutOptionals.numBlackTones());
    ASSERT_FALSE(viewWithoutOptionals.blackColor());
}

TEST_F(OptionalExpressionTest, comparisionOperators)
{
    Container data;
    data.basicColor = BasicColor::BLACK;
    data.numBlackTones = NUM_BLACK_TONES;
    BlackColor blackColor;
    fillBlackColor(blackColor, NUM_BLACK_TONES);
    data.blackColor = blackColor;

    Container equalData;
    equalData.basicColor = BasicColor::BLACK;
    equalData.numBlackTones = NUM_BLACK_TONES;
    BlackColor equalBlackColor;
    fillBlackColor(equalBlackColor, NUM_BLACK_TONES);
    equalData.blackColor = equalBlackColor;

    Container lessThanData;
    lessThanData.basicColor = BasicColor::BLACK;

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data);
    zserio::View equalView(equalData);
    zserio::View lessThanView(lessThanData);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(OptionalExpressionTest, validate)
{
    Container data;
    data.basicColor = BasicColor::BLACK;
    data.numBlackTones = NUM_BLACK_TONES;
    zserio::View view(data);
    ASSERT_THROW(zserio::detail::validate(view), zserio::MissedOptionalException);
}

TEST_F(OptionalExpressionTest, bitSizeOf)
{
    Container data;
    data.basicColor = BasicColor::WHITE;
    zserio::View view(data);

    ASSERT_EQ(CONTAINER_BIT_SIZE_WITHOUT_OPTIONAL, zserio::detail::bitSizeOf(view));

    data.basicColor = BasicColor::BLACK;
    data.numBlackTones = NUM_BLACK_TONES;
    BlackColor blackColor;
    fillBlackColor(blackColor, NUM_BLACK_TONES);
    data.blackColor = blackColor;

    ASSERT_EQ(CONTAINER_BIT_SIZE_WITH_OPTIONAL, zserio::detail::bitSizeOf(view));
}

TEST_F(OptionalExpressionTest, writeRead)
{
    Container data;
    data.basicColor = BasicColor::WHITE;

    test_utils::writeReadTest(data);

    data.basicColor = BasicColor::BLACK;
    data.numBlackTones = NUM_BLACK_TONES;
    BlackColor blackColor;
    fillBlackColor(blackColor, NUM_BLACK_TONES);
    data.blackColor = blackColor;

    test_utils::writeReadTest(data);
}

TEST_F(OptionalExpressionTest, read)
{
    Container data;
    data.basicColor = BasicColor::WHITE;

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, BasicColor::WHITE, 0), data);

    data.basicColor = BasicColor::BLACK;
    data.numBlackTones = NUM_BLACK_TONES;
    BlackColor blackColor;
    fillBlackColor(blackColor, NUM_BLACK_TONES);
    data.blackColor = blackColor;

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, BasicColor::BLACK, NUM_BLACK_TONES), data);
}

TEST_F(OptionalExpressionTest, stdHash)
{
    Container data;
    data.basicColor = BasicColor::BLACK;
    data.numBlackTones = NUM_BLACK_TONES;
    BlackColor blackColor;
    fillBlackColor(blackColor, NUM_BLACK_TONES);
    data.blackColor = blackColor;
    const size_t dataHash = 2361638;

    Container equalData;
    equalData.basicColor = BasicColor::BLACK;
    equalData.numBlackTones = NUM_BLACK_TONES;
    BlackColor equalBlackColor;
    fillBlackColor(equalBlackColor, NUM_BLACK_TONES);
    equalData.blackColor = equalBlackColor;

    Container diffData;
    diffData.basicColor = BasicColor::BLACK;
    const size_t diffDataHash = 1702;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data);
    const size_t viewHash = 3497908;
    zserio::View equalView(equalData);
    zserio::View diffView(diffData);
    const size_t diffViewHash = 1702;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace optional_expression
} // namespace optional_members
