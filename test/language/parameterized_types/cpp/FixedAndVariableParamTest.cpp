#include "gtest/gtest.h"
#include "parameterized_types/fixed_and_variable_param/FixedAndVariableParam.h"
#include "test_utils/TestUtility.h"
#include "zserio/ArrayLengthException.h"
#include "zserio/ConstraintException.h"

namespace parameterized_types
{
namespace fixed_and_variable_param
{

class FixedAndVariableParamTest : public ::testing::Test
{
protected:
    void fillArrayHolder(ArrayHolder& arrayHolder, uint32_t size, Color color, Access access, float floatValue)
    {
        auto& array = arrayHolder.array;
        array.clear();
        for (uint32_t i = 0; i < size; ++i)
        {
            array.push_back(static_cast<uint32_t>(i * i));
        }
        arrayHolder.extraValue = EXTRA_VALUE;
        arrayHolder.hasBlack = color == Color::BLACK;
        arrayHolder.hasRead = (access & Access::Values::READ) == Access::Values::READ;
        arrayHolder.hasFloatBiggerThanOne = floatValue > 1.0F;
    }

    void fillData(FixedAndVariableParam& data, uint32_t size, uint8_t extraLimit, uint8_t limit, Color color,
            Access access, float floatValue)
    {
        data.extraLimit = extraLimit;

        LimitHolder& limitHolder = data.limitHolder;
        limitHolder.limit = limit;

        data.color = color;
        data.access = access;
        data.floatValue = floatValue;

        ArrayHolder arrayHolder;
        fillArrayHolder(arrayHolder, size, color, access, floatValue);
        data.arrayHolder = arrayHolder;
    }

    static void writeArrayHolder(zserio::BitStreamWriter& writer, const ArrayHolder& arrayHolder, uint32_t size,
            Color color, Access access, float floatValue)
    {
        for (uint32_t i = 0; i < size; ++i)
        {
            writer.writeVarUInt(arrayHolder.array.at(i));
        }

        writer.writeUnsignedBits32(arrayHolder.extraValue, 3);
        writer.writeBool(color == Color::BLACK);
        writer.writeBool((access & Access::Values::READ) == Access::Values::READ);
        writer.writeBool(floatValue > 1.0F);
    }

    static void writeData(zserio::BitStreamWriter& writer, const FixedAndVariableParam& data, uint32_t size)
    {
        writer.writeUnsignedBits32(data.extraLimit, 8);
        writer.writeUnsignedBits32(data.limitHolder.limit, 8);
        writer.writeUnsignedBits32(zserio::enumToValue(data.color), 2);
        writer.writeUnsignedBits32(data.access.getValue(), 4);
        writer.writeFloat16(data.floatValue);
        const ArrayHolder& arrayHolder = data.arrayHolder;
        writeArrayHolder(writer, arrayHolder, size, data.color, data.access, data.floatValue);
    }

    static constexpr std::string_view BLOB_NAME = "language/parameterized_types/fixed_and_variable_param.blob";

    static constexpr uint8_t EXTRA_VALUE = 0x05;
    static constexpr uint32_t ARRAY_SIZE = 1000;
    static constexpr uint32_t WRONG_ARRAY_SIZE = 1001;
    static constexpr uint8_t EXTRA_LIMIT = 0x05;
    static constexpr uint8_t WRONG_EXTRA_LIMIT = 0x04;
    static constexpr uint8_t LIMIT = 0x06;
    static constexpr Color COLOR = Color::BLACK;
    static constexpr Color WRONG_COLOR = Color::WHITE;
    static constexpr Access ACCESS = Access::Values::READ;
    static constexpr Access WRONG_ACCESS = Access::Values::WRITE;
    static constexpr float FLOAT_VALUE = 2.0;
    static constexpr float WRONG_FLOAT_VALUE = 1.0;
};

TEST_F(FixedAndVariableParamTest, writeRead)
{
    FixedAndVariableParam data;
    fillData(data, ARRAY_SIZE, EXTRA_LIMIT, LIMIT, COLOR, ACCESS, FLOAT_VALUE);

    test_utils::writeReadTest(data);
}

TEST_F(FixedAndVariableParamTest, writeReadFile)
{
    FixedAndVariableParam data;
    fillData(data, ARRAY_SIZE, EXTRA_LIMIT, LIMIT, COLOR, ACCESS, FLOAT_VALUE);

    test_utils::writeReadFileTest(BLOB_NAME, data);
}

TEST_F(FixedAndVariableParamTest, read)
{
    FixedAndVariableParam data;
    fillData(data, ARRAY_SIZE, EXTRA_LIMIT, LIMIT, COLOR, ACCESS, FLOAT_VALUE);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, data, ARRAY_SIZE), data);
}

TEST_F(FixedAndVariableParamTest, validateFailureWrongArraySize)
{
    FixedAndVariableParam data;
    fillData(data, WRONG_ARRAY_SIZE, EXTRA_LIMIT, LIMIT, COLOR, ACCESS, FLOAT_VALUE);

    zserio::View view(data);
    ASSERT_THROW(zserio::detail::validate(view), zserio::ArrayLengthException);
}

TEST_F(FixedAndVariableParamTest, validateFailureWrongExtraLimit)
{
    FixedAndVariableParam data;
    fillData(data, ARRAY_SIZE, WRONG_EXTRA_LIMIT, LIMIT, COLOR, ACCESS, FLOAT_VALUE);

    zserio::View view(data);
    ASSERT_THROW(zserio::detail::validate(view), zserio::ConstraintException);
}

TEST_F(FixedAndVariableParamTest, validateFailureWrongColor)
{
    FixedAndVariableParam data;
    fillData(data, ARRAY_SIZE, EXTRA_LIMIT, LIMIT, COLOR, ACCESS, FLOAT_VALUE);
    data.color = WRONG_COLOR;

    zserio::View view(data);
    ASSERT_THROW(zserio::detail::validate(view), zserio::ConstraintException);
}

TEST_F(FixedAndVariableParamTest, validateFailureWrongAccess)
{
    FixedAndVariableParam data;
    fillData(data, ARRAY_SIZE, EXTRA_LIMIT, LIMIT, COLOR, ACCESS, FLOAT_VALUE);
    data.access = WRONG_ACCESS;

    zserio::View view(data);
    ASSERT_THROW(zserio::detail::validate(view), zserio::ConstraintException);
}

TEST_F(FixedAndVariableParamTest, validateFailureWrongFloatValue)
{
    FixedAndVariableParam data;
    fillData(data, ARRAY_SIZE, EXTRA_LIMIT, LIMIT, COLOR, ACCESS, FLOAT_VALUE);
    data.floatValue = WRONG_FLOAT_VALUE;

    zserio::View view(data);
    ASSERT_THROW(zserio::detail::validate(view), zserio::ConstraintException);
}

} // namespace fixed_and_variable_param
} // namespace parameterized_types
