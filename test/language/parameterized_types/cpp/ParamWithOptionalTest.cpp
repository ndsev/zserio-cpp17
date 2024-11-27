#include <memory>

#include "gtest/gtest.h"
#include "parameterized_types/param_with_optional/Holder.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace param_with_optional
{

using AllocatorType = Holder::AllocatorType;

class ParamWithOptionalTest : public ::testing::Test
{
protected:
    void fillHolder(Holder& holder)
    {
        const Param param(HAS_EXTRA, EXTRA_PARAM);
        holder.param = param;
        Value value;
        value.extraValue = ExtraValue(EXTRA_VALUE);
        holder.value = value;
    }

    void writeHolderToByteArray(zserio::BitStreamWriter& writer)
    {
        writer.writeBool(HAS_EXTRA);
        writer.writeUnsignedBits32(EXTRA_PARAM, 7);
        writer.writeUnsignedBits64(EXTRA_VALUE, 64);
    }

    void checkHolder(const Holder& holder)
    {
        ASSERT_EQ(HAS_EXTRA, holder.param.hasExtra);
        ASSERT_EQ(EXTRA_PARAM, holder.param.extraParam);
        ASSERT_EQ(EXTRA_VALUE, holder.value.extraValue->value);
    }

    void checkHolderView(const zserio::View<Holder>& view)
    {
        ASSERT_EQ(HAS_EXTRA, view.param().hasExtra());
        ASSERT_EQ(EXTRA_PARAM, view.param().extraParam());
        ASSERT_EQ(EXTRA_VALUE, view.value().extraValue()->value());
        ASSERT_EQ(EXTRA_PARAM, view.value().param().extraParam()); // parameter getter
    }

    static constexpr std::string_view BLOB_NAME = "language/parameterized_types/param_with_optional.blob";

    static constexpr bool HAS_EXTRA = true;
    static constexpr zserio::UInt7 EXTRA_PARAM = 0x00;
    static constexpr zserio::UInt64 EXTRA_VALUE = 0xDEAD;
    static constexpr size_t HOLDER_BIT_SIZE = 1 + 7 + 64;

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

TEST_F(ParamWithOptionalTest, copyConstructor)
{
    Holder holder;
    fillHolder(holder);

    const Holder holderCopy(holder);
    ASSERT_EQ(holder, holderCopy);

    zserio::View view(holder);
    const zserio::View viewCopy(view);
    ASSERT_EQ(view, viewCopy);
}

TEST_F(ParamWithOptionalTest, moveConstructor)
{
    Holder holder;
    fillHolder(holder);

    const Holder holderCopy(holder);
    const Holder holderMove(std::move(holder));
    ASSERT_EQ(holderCopy, holderMove);
}

TEST_F(ParamWithOptionalTest, assignmentOperator)
{
    Holder holder;
    fillHolder(holder);

    const Holder holderAssignment = holder;
    ASSERT_EQ(holder, holderAssignment);
}

TEST_F(ParamWithOptionalTest, moveAssignmentOperator)
{
    Holder holder;
    fillHolder(holder);

    const Holder holderCopy(holder);
    Holder holderMoveAssignment;
    holderMoveAssignment = std::move(holder);
    ASSERT_EQ(holderCopy, holderMoveAssignment);
}

TEST_F(ParamWithOptionalTest, bitSizeOf)
{
    Holder holder;
    fillHolder(holder);

    zserio::View view(holder);
    ASSERT_EQ(HOLDER_BIT_SIZE, zserio::detail::bitSizeOf(view, 0));
    ASSERT_EQ(HOLDER_BIT_SIZE, zserio::detail::bitSizeOf(view, 1));
    ASSERT_EQ(HOLDER_BIT_SIZE, zserio::detail::bitSizeOf(view, 100));
}

TEST_F(ParamWithOptionalTest, read)
{
    zserio::BitStreamWriter writer(bitBuffer);
    writeHolderToByteArray(writer);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    Holder holder;
    zserio::View view = zserio::detail::read(reader, holder);
    checkHolder(holder);
    checkHolderView(view);
}

TEST_F(ParamWithOptionalTest, writeRead)
{
    Holder holder;
    fillHolder(holder);

    zserio::BitStreamWriter writer(bitBuffer);
    zserio::View view(holder);
    zserio::detail::write(writer, view);

    ASSERT_EQ(zserio::detail::bitSizeOf(view, 0), writer.getBitPosition());

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    Holder readHolder;
    zserio::View readView = zserio::detail::read(reader, readHolder);
    checkHolder(readHolder);
    checkHolderView(readView);
    ASSERT_EQ(view, readView);
}

} // namespace param_with_optional
} // namespace parameterized_types
