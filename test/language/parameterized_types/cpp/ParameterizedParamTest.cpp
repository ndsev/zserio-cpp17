#include "gtest/gtest.h"
#include "parameterized_types/parameterized_param/ParameterizedParamHolder.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace parameterized_param
{

class ParameterizedParamTest : public ::testing::Test
{
protected:
    void fillParameterizedParamHolder(ParameterizedParamHolder& parameterizedParamHolder)
    {
        Param& param = parameterizedParamHolder.param;
        param.value = PARAM_VALUE;
        param.extraValue = PARAM_EXTRA_VALUE;

        ParameterizedParam& parameterizedParam = parameterizedParamHolder.parameterizedParam;
        parameterizedParam.value = PARAMETERIZED_PARAM_VALUE;
        parameterizedParam.extraValue = PARAMETERIZED_PARAM_EXTRA_VALUE;
    }

    void checkParameterizedParamHolderInBitStream(
            zserio::BitStreamReader& reader, const ParameterizedParamHolder& parameterizedParamHolder)
    {
        ASSERT_EQ(parameterizedParamHolder.parameter, reader.readUnsignedBits32(16));

        const Param& param = parameterizedParamHolder.param;
        ASSERT_EQ(param.value, reader.readUnsignedBits32(16));
        ASSERT_EQ(param.extraValue, reader.readUnsignedBits32(32));

        const ParameterizedParam& parameterizedParam = parameterizedParamHolder.parameterizedParam;
        ASSERT_EQ(parameterizedParam.value, reader.readUnsignedBits32(16));
        ASSERT_EQ(parameterizedParam.extraValue, reader.readUnsignedBits32(32));

        reader.setBitPosition(0);
    }

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);

private:
    static constexpr uint16_t PARAMETER = 11;
    static constexpr uint16_t PARAM_VALUE = 0x0BED;
    static constexpr uint32_t PARAM_EXTRA_VALUE = 0x0BEDDEAD;
    static constexpr uint16_t PARAMETERIZED_PARAM_VALUE = 0x0BAD;
    static constexpr uint32_t PARAMETERIZED_PARAM_EXTRA_VALUE = 0x0BADDEAD;
};

TEST_F(ParameterizedParamTest, writeRead)
{
    ParameterizedParamHolder parameterizedParamHolder;
    fillParameterizedParamHolder(parameterizedParamHolder);
    zserio::View<ParameterizedParamHolder> view(parameterizedParamHolder);

    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    checkParameterizedParamHolderInBitStream(reader, parameterizedParamHolder);

    ParameterizedParamHolder readParameterizedParamHolder;
    zserio::View<ParameterizedParamHolder> readView =
            zserio::detail::read(reader, readParameterizedParamHolder);
    ASSERT_EQ(parameterizedParamHolder, readParameterizedParamHolder);
    ASSERT_EQ(view, readView);
}

} // namespace parameterized_param
} // namespace parameterized_types
