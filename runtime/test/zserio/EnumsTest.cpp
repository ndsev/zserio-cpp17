#include <array>

#include "gtest/gtest.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/Enums.h"
#include "zserio/HashCodeUtil.h"

namespace zserio
{

enum class Color : UInt8::ValueType
{
    NONE = UINT8_C(0),
    RED = UINT8_C(2),
    BLUE = UINT8_C(3),
    BLACK = UINT8_C(7)
};

template <>
struct EnumTraits<Color>
{
    static constexpr std::array<std::string_view, 4> names = {{"NONE", "RED", "BLUE", "BLACK"}};
    static constexpr std::array<Color, 4> values = {{Color::NONE, Color::RED, Color::BLUE, Color::BLACK}};
    static constexpr std::string_view enumName = "Color";
    using ZserioType = UInt8;
};

template <>
inline size_t enumToOrdinal<Color>(Color value)
{
    switch (value)
    {
    case Color::NONE:
        return 0;
    case Color::RED:
        return 1;
    case Color::BLUE:
        return 2;
    case Color::BLACK:
        return 3;
    default:
        throw CppRuntimeException("Unknown value for enumeration Color: ")
                << static_cast<uint8_t>(value) << "!";
    }
}

template <>
inline Color valueToEnum<Color>(typename EnumTraits<Color>::ZserioType rawValue)
{
    switch (rawValue)
    {
    case UINT8_C(0):
    case UINT8_C(2):
    case UINT8_C(3):
    case UINT8_C(7):
        return static_cast<Color>(static_cast<std::underlying_type_t<Color>>(rawValue));
    default:
        throw CppRuntimeException("Unknown value for enumeration Color: ") << rawValue << "!";
    }
}

} // namespace zserio

namespace std
{

template <>
struct hash<zserio::Color>
{
    size_t operator()(const zserio::Color& value) const
    {
        uint32_t result = zserio::HASH_SEED;
        // TODO[Mi-L@]: Should calcHashCode work with wrapper types?
        result = zserio::calcHashCode(
                result, static_cast<std::underlying_type_t<zserio::Color>>(zserio::enumToValue(value)));
        return static_cast<size_t>(result);
    }
};

} // namespace std

namespace zserio
{

TEST(EnumsTest, enumToOrdinal)
{
    EXPECT_EQ(0, enumToOrdinal(Color::NONE));
    EXPECT_EQ(1, enumToOrdinal(Color::RED));
    EXPECT_EQ(2, enumToOrdinal(Color::BLUE));
    EXPECT_EQ(3, enumToOrdinal(Color::BLACK));

    EXPECT_THROW(enumToOrdinal(static_cast<Color>(1)), CppRuntimeException);
}

TEST(EnumsTest, valueToEnum)
{
    EXPECT_EQ(Color::NONE, valueToEnum<Color>(0));
    EXPECT_EQ(Color::RED, valueToEnum<Color>(2));
    EXPECT_EQ(Color::BLUE, valueToEnum<Color>(3));
    EXPECT_EQ(Color::BLACK, valueToEnum<Color>(7));

    EXPECT_THROW(valueToEnum<Color>(1), CppRuntimeException);
}

TEST(EnumsTest, enumToValue)
{
    EXPECT_EQ(0, enumToValue(Color::NONE));
    EXPECT_EQ(2, enumToValue(Color::RED));
    EXPECT_EQ(3, enumToValue(Color::BLUE));
    EXPECT_EQ(7, enumToValue(Color::BLACK));
}

TEST(EnumsTest, stringToEnum)
{
    EXPECT_EQ(Color::NONE, stringToEnum<Color>("NONE"));
    EXPECT_EQ(Color::RED, stringToEnum<Color>("RED"));
    EXPECT_EQ(Color::BLUE, stringToEnum<Color>("BLUE"));
    EXPECT_EQ(Color::BLACK, stringToEnum<Color>("BLACK"));

    EXPECT_THROW(stringToEnum<Color>("NONEXISTING"), CppRuntimeException);
}

TEST(EnumsTest, enumToString)
{
    // use std::string to prevent comparison of pointer values (which happened on MSVC in debug)
    EXPECT_EQ("NONE", enumToString(Color::NONE));
    EXPECT_EQ("RED", enumToString(Color::RED));
    EXPECT_EQ("BLUE", enumToString(Color::BLUE));
    EXPECT_EQ("BLACK", enumToString(Color::BLACK));

    EXPECT_THROW(enumToString(static_cast<Color>(1)), CppRuntimeException);
}

TEST(EnumsTest, bitSizeOf)
{
    EXPECT_EQ(detail::bitSizeOf(UInt8()), detail::bitSizeOf(Color::NONE));
}

TEST(EnumTest, write)
{
    BitBuffer enumBuffer(8);
    BitBuffer underlyingBuffer(8);
    BitStreamWriter enumWriter(enumBuffer);
    BitStreamWriter underlyingWriter(underlyingBuffer);

    detail::write(enumWriter, Color::BLACK);
    detail::write(underlyingWriter, enumToValue(Color::BLACK));

    EXPECT_EQ(underlyingBuffer, enumBuffer);
}

TEST(EnumTest, read)
{
    BitBuffer buffer(8);
    BitStreamWriter writer(buffer);
    detail::write(writer, enumToValue(Color::BLACK));

    BitStreamReader reader(buffer);

    Color color{};
    detail::read(reader, color);
    EXPECT_EQ(Color::BLACK, color);
}

TEST(EnumsTest, cppRuntimeExceptionOperator)
{
    CppRuntimeException exception = CppRuntimeException() << Color::NONE;
    ASSERT_STREQ("NONE", exception.what());
}

TEST(EnumsTest, stdHash)
{
    EXPECT_EQ((HASH_PRIME_NUMBER * HASH_SEED + enumToValue(Color::NONE)), std::hash<Color>{}(Color::NONE));
}

} // namespace zserio
