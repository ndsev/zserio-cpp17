#include <utility>

#include "functions/structure_optional/ValueConsumerCreator.h"
#include "gtest/gtest.h"
#include "test_utils/ReadTest.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/BitStreamWriter.h"

namespace functions
{
namespace structure_optional
{

class StructureOptionalTest : public ::testing::Test
{
protected:
    static zserio::UInt4 calculateValue(zserio::UInt4 defaultValue, zserio::UInt4 externalValue)
    {
        return (defaultValue != INVALID_DEFAULT_VALUE) ? defaultValue : externalValue;
    }

    static void writeData(
            zserio::BitStreamWriter& writer, zserio::UInt4 defaultValue, zserio::UInt4 externalValue)
    {
        zserio::detail::write(writer, defaultValue);
        if (defaultValue == INVALID_DEFAULT_VALUE)
        {
            zserio::detail::write(writer, externalValue);
        }

        writer.writeBool((calculateValue(defaultValue, externalValue) < SMALL_VALUE_THRESHOLD));
    }

    static void fillData(ValueConsumerCreator& data, zserio::UInt4 defaultValue, zserio::UInt4 externalValue)
    {
        ValueCalculator& valueCalculator = data.valueCalculator;
        valueCalculator.defaultValue = defaultValue;
        if (defaultValue == INVALID_DEFAULT_VALUE)
        {
            valueCalculator.externalValue = externalValue;
        }

        ValueConsumer& valueConsumer = data.valueConsumer;
        valueConsumer.isSmall = calculateValue(defaultValue, externalValue) < SMALL_VALUE_THRESHOLD;
    }

    static void checkValueConsumerCreator(uint8_t defaultValue, uint8_t externalValue)
    {
        ValueConsumerCreator data;
        fillData(data, defaultValue, externalValue);
        zserio::View<ValueConsumerCreator> view(data);

        const uint8_t expectedValue = calculateValue(defaultValue, externalValue);
        ASSERT_EQ(expectedValue, view.valueCalculator().value());

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, defaultValue, externalValue), data);
        test_utils::writeReadTest(data);
    }

    static constexpr zserio::UInt4 INVALID_DEFAULT_VALUE = 0;
    static constexpr zserio::UInt4 DEFAULT_VALUE = 1;
    static constexpr zserio::UInt4 EXTERNAL_VALUE = 2;
    static constexpr zserio::UInt4 SMALL_VALUE_THRESHOLD = 8;
};

TEST_F(StructureOptionalTest, checkDefaultValueConsumerCreator)
{
    checkValueConsumerCreator(DEFAULT_VALUE, EXTERNAL_VALUE);
}

TEST_F(StructureOptionalTest, checkExternalValueConsumerCreator)
{
    checkValueConsumerCreator(INVALID_DEFAULT_VALUE, EXTERNAL_VALUE);
}

} // namespace structure_optional
} // namespace functions
