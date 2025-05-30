#include "gtest/gtest.h"
#include "zserio/BitBuffer.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/Bitmasks.h"

namespace zserio
{

class TestBitmask
{
public:
    using ZserioType = UInt4;

    enum class Values : ZserioType::ValueType
    {
        READ = 0x1,
        WRITE = 0x2,
        READ_WRITE = 0x3
    };

    constexpr TestBitmask(Values value) noexcept :
            m_value(static_cast<ZserioType::ValueType>(value))
    {}

    constexpr explicit TestBitmask(ZserioType value) noexcept :
            m_value(value)
    {}

    constexpr explicit operator ZserioType() const
    {
        return m_value;
    }

    constexpr ZserioType getValue() const
    {
        return m_value;
    }

private:
    ZserioType m_value;
};

inline constexpr bool operator==(const TestBitmask& lhs, const TestBitmask& rhs)
{
    return lhs.getValue() == rhs.getValue();
}

TEST(BitmasksTest, validate)
{
    TestBitmask bitmask = TestBitmask::Values::READ;
    ASSERT_NO_THROW(detail::validate(bitmask, ""));
}

TEST(BitmasksTest, bitSizeOf)
{
    TestBitmask bitmask = TestBitmask::Values::READ;
    ASSERT_EQ(4, detail::bitSizeOf(bitmask));
}

TEST(BitmasksTest, writeRead)
{
    TestBitmask bitmask = TestBitmask::Values::READ;
    const BitSize bitSize = detail::bitSizeOf(bitmask);

    BitBuffer bitBuffer(bitSize);
    BitStreamWriter writer(bitBuffer);
    detail::write(writer, bitmask);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    TestBitmask readBitmask(TestBitmask::Values::WRITE);
    BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), BitsTag());
    detail::read(reader, readBitmask);
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(readBitmask, bitmask);
}

TEST(BitmasksTest, cppRuntimeExceptionOperator)
{
    CppRuntimeException exception = CppRuntimeException() << TestBitmask(TestBitmask::Values::WRITE);
    ASSERT_STREQ("2", exception.what());
}

TEST(BitmasksTest, itemsAccessor)
{
    EXPECT_EQ(TestBitmask::Values::READ, ItemsAccessor<TestBitmask>::Items::READ);
    EXPECT_EQ(TestBitmask::Values::WRITE, ItemsAccessor<TestBitmask>::Items::WRITE);
    EXPECT_EQ(TestBitmask::Values::READ_WRITE, ItemsAccessor<TestBitmask>::Items::READ_WRITE);
}

} // namespace zserio
