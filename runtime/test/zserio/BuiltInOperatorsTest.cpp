#include "gtest/gtest.h"
#include "zserio/BuiltInOperators.h"
#include "zserio/Enums.h"
#include "zserio/Types.h"

namespace zserio
{

namespace
{

enum class Color : UInt8::ValueType
{
    NONE = UINT8_C(0),
    RED = UINT8_C(2),
    BLUE = UINT8_C(3),
    BLACK = UINT8_C(7)
};

} // namespace

template <>
struct EnumTraits<Color>
{
    using ZserioType = UInt8;
};

namespace builtin
{

namespace
{

class DummyBitmask
{
public:
    using ZserioType = UInt8;

    enum class Values : ZserioType::ValueType
    {
        READ = 1U,
        WRITE = 2U,
        CREATE = 1U | 2U
    };

    constexpr DummyBitmask(Values value) noexcept :
            m_value(static_cast<ZserioType::ValueType>(value))
    {}

    constexpr explicit DummyBitmask(ZserioType value) noexcept :
            m_value(value)
    {}

    constexpr ZserioType getValue() const
    {
        return m_value;
    }

private:
    ZserioType m_value;
};

inline bool operator==(const DummyBitmask& lhs, const DummyBitmask& rhs)
{
    return lhs.getValue() == rhs.getValue();
}

inline DummyBitmask operator|(const DummyBitmask& lhs, const DummyBitmask& rhs)
{
    return DummyBitmask(lhs.getValue() | rhs.getValue());
}

inline DummyBitmask operator|(DummyBitmask::Values lhs, DummyBitmask::Values rhs)
{
    return DummyBitmask(lhs) | DummyBitmask(rhs);
}

inline DummyBitmask operator&(const DummyBitmask& lhs, const DummyBitmask& rhs)
{
    return DummyBitmask(lhs.getValue() & rhs.getValue());
}

inline DummyBitmask operator&(DummyBitmask::Values lhs, DummyBitmask::Values rhs)
{
    return DummyBitmask(lhs) & DummyBitmask(rhs);
}

} // namespace

TEST(BuiltInOperatorsTest, isSet)
{
    // randomly mix bitmask instances with DummyBitmask::Values enum values to check that all variants work
    ASSERT_TRUE(isSet(DummyBitmask(DummyBitmask::Values::READ), DummyBitmask::Values::READ));
    ASSERT_TRUE(isSet(DummyBitmask::Values::CREATE, DummyBitmask::Values::READ));
    ASSERT_TRUE(isSet(DummyBitmask::Values::CREATE, DummyBitmask(DummyBitmask::Values::WRITE)));
    ASSERT_TRUE(isSet(DummyBitmask::Values::CREATE, DummyBitmask::Values::CREATE));
    ASSERT_TRUE(isSet(DummyBitmask::Values::CREATE, DummyBitmask::Values::READ | DummyBitmask::Values::WRITE));
    ASSERT_FALSE(isSet(DummyBitmask(DummyBitmask::Values::READ), DummyBitmask(DummyBitmask::Values::WRITE)));
    ASSERT_FALSE(isSet(DummyBitmask::Values::READ, DummyBitmask::Values::CREATE));
}

TEST(BuiltInOperatorsTest, numBits)
{
    EXPECT_EQ(0, numBits(0));
    EXPECT_EQ(1, numBits(1));
    EXPECT_EQ(1, numBits(2));
    EXPECT_EQ(2, numBits(3));
    EXPECT_EQ(2, numBits(4));
    EXPECT_EQ(3, numBits(5));
    EXPECT_EQ(3, numBits(6));
    EXPECT_EQ(3, numBits(7));
    EXPECT_EQ(3, numBits(8));
    EXPECT_EQ(4, numBits(16));
    EXPECT_EQ(5, numBits(32));
    EXPECT_EQ(6, numBits(64));
    EXPECT_EQ(7, numBits(128));
    EXPECT_EQ(8, numBits(256));
    EXPECT_EQ(9, numBits(512));
    EXPECT_EQ(10, numBits(1024));
    EXPECT_EQ(11, numBits(2048));
    EXPECT_EQ(12, numBits(4096));
    EXPECT_EQ(13, numBits(8192));
    EXPECT_EQ(14, numBits(16384));
    EXPECT_EQ(15, numBits(32768));
    EXPECT_EQ(16, numBits(65536));
    EXPECT_EQ(24, numBits(UINT64_C(1) << 24U));
    EXPECT_EQ(25, numBits((UINT64_C(1) << 24U) + 1));
    EXPECT_EQ(32, numBits(UINT64_C(1) << 32U));
    EXPECT_EQ(33, numBits((UINT64_C(1) << 32U) + 1));
    EXPECT_EQ(63, numBits(UINT64_C(1) << 63U));
    EXPECT_EQ(64, numBits((UINT64_C(1) << 63U) + 1));
}

TEST(BuiltinOperatorsTest, valueOf)
{
    EXPECT_EQ(DummyBitmask(DummyBitmask::Values::READ).getValue(),
            valueOf(DummyBitmask(DummyBitmask::Values::READ)));
    EXPECT_EQ(DummyBitmask(DummyBitmask::Values::WRITE).getValue(),
            valueOf(DummyBitmask(DummyBitmask::Values::WRITE)));
    EXPECT_EQ(DummyBitmask(DummyBitmask::Values::CREATE).getValue(),
            valueOf(DummyBitmask(DummyBitmask::Values::CREATE)));

    EXPECT_EQ(enumToValue(Color::NONE), valueOf(Color::NONE));
    EXPECT_EQ(enumToValue(Color::RED), valueOf(Color::RED));
    EXPECT_EQ(enumToValue(Color::BLUE), valueOf(Color::BLUE));
    EXPECT_EQ(enumToValue(Color::BLACK), valueOf(Color::BLACK));
}

} // namespace builtin

} // namespace zserio
