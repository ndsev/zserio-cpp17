#include "array_types/auto_array_bitfield_param/ParameterizedBitfieldLength.h"
#include "gtest/gtest.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/SerializeUtil.h"

namespace array_types
{
namespace auto_array_bitfield_param
{

using allocator_type = ParameterizedBitfieldLength::allocator_type;
template <typename T>
using vector_type = zserio::vector<T, allocator_type>;

class AutoArrayBitfieldParamTest : public ::testing::Test
{
protected:
    void fillParameterizedBitfieldLength(ParameterizedBitfieldLength& parameterizedBitfieldLength)
    {
        vector_type<zserio::DynUInt16<>>& dynamicBitfieldArray =
                parameterizedBitfieldLength.dynamicBitfieldArray;
        for (uint16_t i = 0; i < DYNAMIC_BITFIELD_ARRAY_SIZE; ++i)
        {
            dynamicBitfieldArray.push_back(i);
        }
    }

    void checkParameterizedBitfieldLengthInBitStream(zserio::BitStreamReader& reader,
            const zserio::View<ParameterizedBitfieldLength>& parameterizedBitfieldLength)
    {
        ASSERT_EQ(NUM_BITS_PARAM, parameterizedBitfieldLength.numBits());
        ASSERT_EQ(DYNAMIC_BITFIELD_ARRAY_SIZE, reader.readVarSize());
        for (uint16_t i = 0; i < DYNAMIC_BITFIELD_ARRAY_SIZE; ++i)
        {
            ASSERT_EQ(i, reader.readUnsignedBits32(NUM_BITS_PARAM));
        }
        reader.setBitPosition(0);
    }

    static constexpr std::string_view BLOB_NAME = "language/array_types/auto_array_bitfield_param.blob";
    static constexpr zserio::UInt4 NUM_BITS_PARAM = 9;

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);

private:
    static constexpr size_t DYNAMIC_BITFIELD_ARRAY_SIZE = (1U << 9U) - 1;
};

TEST_F(AutoArrayBitfieldParamTest, copyConstructor)
{
    ParameterizedBitfieldLength parameterizedBitfieldLengthOrig;
    fillParameterizedBitfieldLength(parameterizedBitfieldLengthOrig);
    zserio::View<ParameterizedBitfieldLength> viewOrig(parameterizedBitfieldLengthOrig, NUM_BITS_PARAM);
    const zserio::BitSize origBitSize = zserio::detail::bitSizeOf(viewOrig);

    zserio::BitBuffer bitBufferOrig(origBitSize);
    zserio::BitStreamWriter writerOrig(bitBufferOrig);
    zserio::detail::write(writerOrig, viewOrig);

    ParameterizedBitfieldLength parameterizedBitfieldLengthCopied(parameterizedBitfieldLengthOrig);
    zserio::View<ParameterizedBitfieldLength> viewCopied(parameterizedBitfieldLengthCopied, NUM_BITS_PARAM);
    ASSERT_EQ(origBitSize, zserio::detail::bitSizeOf(viewCopied));

    zserio::BitBuffer bitBufferCopied(origBitSize);
    zserio::BitStreamWriter writerCopied(bitBufferCopied);
    zserio::detail::write(writerCopied, viewCopied);
    ASSERT_EQ(bitBufferOrig, bitBufferCopied);
}

TEST_F(AutoArrayBitfieldParamTest, moveConstructor)
{
    ParameterizedBitfieldLength parameterizedBitfieldLengthOrig;
    fillParameterizedBitfieldLength(parameterizedBitfieldLengthOrig);
    zserio::View<ParameterizedBitfieldLength> viewOrig(parameterizedBitfieldLengthOrig, NUM_BITS_PARAM);
    const size_t origBitSize = zserio::detail::bitSizeOf(viewOrig);

    zserio::BitBuffer bitBufferOrig(origBitSize);
    zserio::BitStreamWriter writerOrig(bitBufferOrig);
    zserio::detail::write(writerOrig, viewOrig);

    ParameterizedBitfieldLength parameterizedBitfieldLengthMoved(std::move(parameterizedBitfieldLengthOrig));
    zserio::View<ParameterizedBitfieldLength> viewMoved(parameterizedBitfieldLengthMoved, NUM_BITS_PARAM);
    ASSERT_EQ(origBitSize, zserio::detail::bitSizeOf(viewMoved));

    zserio::BitBuffer bitBufferMoved(origBitSize);
    zserio::BitStreamWriter writerMoved(bitBufferMoved);
    zserio::detail::write(writerMoved, viewMoved);
    ASSERT_EQ(bitBufferOrig, bitBufferMoved);
}

TEST_F(AutoArrayBitfieldParamTest, copyAssignmentOperator)
{
    ParameterizedBitfieldLength parameterizedBitfieldLengthCopied;
    size_t origBitSize = 0;
    {
        ParameterizedBitfieldLength parameterizedBitfieldLengthOrig;
        fillParameterizedBitfieldLength(parameterizedBitfieldLengthOrig);
        zserio::View<ParameterizedBitfieldLength> viewOrig(parameterizedBitfieldLengthOrig, NUM_BITS_PARAM);
        origBitSize = zserio::detail::bitSizeOf(viewOrig);

        parameterizedBitfieldLengthCopied = parameterizedBitfieldLengthOrig;
    }

    zserio::View<ParameterizedBitfieldLength> viewCopied(parameterizedBitfieldLengthCopied, NUM_BITS_PARAM);
    ASSERT_EQ(origBitSize, zserio::detail::bitSizeOf(viewCopied));
}

TEST_F(AutoArrayBitfieldParamTest, moveAssignmentOperator)
{
    ParameterizedBitfieldLength parameterizedBitfieldLengthMoved;
    size_t origBitSize = 0;
    {
        ParameterizedBitfieldLength parameterizedBitfieldLengthOrig;
        fillParameterizedBitfieldLength(parameterizedBitfieldLengthOrig);
        zserio::View<ParameterizedBitfieldLength> viewOrig(parameterizedBitfieldLengthOrig, NUM_BITS_PARAM);
        origBitSize = zserio::detail::bitSizeOf(viewOrig);

        parameterizedBitfieldLengthMoved = std::move(parameterizedBitfieldLengthOrig);
    }

    zserio::View<ParameterizedBitfieldLength> viewMoved(parameterizedBitfieldLengthMoved, NUM_BITS_PARAM);
    ASSERT_EQ(origBitSize, zserio::detail::bitSizeOf(viewMoved));
}

TEST_F(AutoArrayBitfieldParamTest, writeRead)
{
    ParameterizedBitfieldLength parameterizedBitfieldLength;
    fillParameterizedBitfieldLength(parameterizedBitfieldLength);
    zserio::View<ParameterizedBitfieldLength> view(parameterizedBitfieldLength, NUM_BITS_PARAM);

    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    checkParameterizedBitfieldLengthInBitStream(reader, view);

    ParameterizedBitfieldLength readParameterizedBitfieldLength;
    zserio::View<ParameterizedBitfieldLength> readView =
            zserio::detail::read(reader, readParameterizedBitfieldLength, NUM_BITS_PARAM);
    ASSERT_EQ(parameterizedBitfieldLength, readParameterizedBitfieldLength);
    ASSERT_EQ(view, readView);
}

TEST_F(AutoArrayBitfieldParamTest, writeReadFile)
{
    ParameterizedBitfieldLength parameterizedBitfieldLength;
    fillParameterizedBitfieldLength(parameterizedBitfieldLength);
    zserio::View<ParameterizedBitfieldLength> view(parameterizedBitfieldLength, NUM_BITS_PARAM);

    zserio::serializeToFile(view, std::string(BLOB_NAME));

    ParameterizedBitfieldLength readParameterizedBitfieldLength;
    const auto readView = zserio::deserializeFromFile(
            std::string(BLOB_NAME), readParameterizedBitfieldLength, NUM_BITS_PARAM);
    ASSERT_EQ(parameterizedBitfieldLength, readParameterizedBitfieldLength);
    ASSERT_EQ(view, readView);
}

} // namespace auto_array_bitfield_param
} // namespace array_types
