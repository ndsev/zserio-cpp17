#include "gtest/gtest.h"
#include "zserio/GenericAccessor.h"

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

struct TestObj
{
    UInt32 field;
    Optional<UInt32> optField;
    Extended<UInt32> extField;
    Extended<Optional<UInt32>> extOptField;
};

} // namespace

TEST(GenericAccessorTest, genericAccessorEnum)
{
    EXPECT_EQ(Color::NONE, generic_accessor_t<Color>::NONE);
    EXPECT_EQ(Color::RED, generic_accessor_t<Color>::RED);
    EXPECT_EQ(Color::BLUE, generic_accessor_t<Color>::BLUE);
    EXPECT_EQ(Color::BLACK, generic_accessor_t<Color>::BLACK);
}

TEST(GenericAccessorTest, genericAccessorBitmask)
{
    TestBitmask testBitmask = TestBitmask::Values::READ;
    EXPECT_EQ(testBitmask.getValue(), TestBitmask(generic_accessor_t<TestBitmask>::READ).getValue());
    EXPECT_EQ(TestBitmask(UInt4(0x2)), TestBitmask(generic_accessor_t<TestBitmask>::WRITE));
    EXPECT_EQ(0x3, static_cast<UInt4>(TestBitmask(generic_accessor_t<TestBitmask>::READ_WRITE)));
}

TEST(GenericAccessorTest, genericAccessor)
{
    TestObj testObj;
    testObj.field = 13;
    testObj.optField = 42;
    *(testObj.extField) = 68;
    *(testObj.extOptField) = 99;

    ASSERT_EQ(13, genericAccessor(testObj.field));
    ASSERT_EQ(42, genericAccessor(testObj.optField));
    ASSERT_EQ(68, genericAccessor(testObj.extField));
    ASSERT_EQ(99, genericAccessor(testObj.extOptField));
}

} // namespace zserio
