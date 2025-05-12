#include "gtest/gtest.h"
#include "zserio/Types.h"

namespace zserio
{

static void testBoolOperators()
{
    Bool valueMin = NumericLimits<Bool>::min();
    Bool valueMax = NumericLimits<Bool>::max();

    // constructor
    Bool copy(valueMin);
    Bool copy2 = valueMin;
    (void)copy;
    (void)copy2;

    // logical
    ASSERT_FALSE(!valueMax);
    ASSERT_FALSE(valueMin && valueMax);
    ASSERT_TRUE(valueMin || valueMax);

    // comparison
    ASSERT_FALSE(valueMin == valueMax);
    ASSERT_TRUE(valueMin != valueMax);
    ASSERT_TRUE(valueMin < valueMax);
    ASSERT_FALSE(valueMin > valueMax);
    ASSERT_TRUE(valueMin <= valueMax);
    ASSERT_FALSE(valueMin >= valueMax);

    // ternary
    ASSERT_TRUE(valueMin ? valueMin : valueMax);
}

template <typename T>
void testIntOperators(typename T::ValueType minValue = NumericLimits<T>::min())
{
    T value0;
    T valueMin = minValue;
    Int64 valueMax = NumericLimits<Int64>::max();

    // constructor
    Int64 copy(valueMin);
    Int64 copy2 = Int64(valueMin);
    (void)copy;
    (void)copy2;

    // arithmetic
    Int64 value = +valueMin;
    value = -valueMax;
    value = valueMin + valueMax;
    value = valueMax - value0;
    value = value0 * valueMax;
    if (valueMax != 0) // == 0 for Int1
    {
        value = valueMin / valueMax;
        value = valueMin % valueMax;
    }

    // increment / decrement
    value = +valueMin;
    Int64 origValue = value;
    ASSERT_EQ(origValue + 1, ++value);
    ASSERT_EQ(origValue, --value);
    ASSERT_EQ(origValue, value++);
    ASSERT_EQ(origValue + 1, value);
    ASSERT_EQ(origValue + 1, value--);
    ASSERT_EQ(origValue, value);

    // logical
    ASSERT_FALSE(!valueMin);
    ASSERT_TRUE(valueMin && valueMax);
    ASSERT_TRUE(valueMin || valueMax);

    // comparison
    ASSERT_FALSE(valueMin == valueMax);
    ASSERT_TRUE(valueMin != valueMax);
    ASSERT_TRUE(valueMin < valueMax);
    ASSERT_FALSE(valueMin > valueMax);
    ASSERT_TRUE(valueMin <= valueMax);
    ASSERT_FALSE(valueMin >= valueMax);

    // assignment
    value = Int64(valueMin);
    value += valueMax;
    value -= valueMin;
    value *= value0;
    if (valueMax != 0) // == 0 for Int1
    {
        value /= valueMax;
        value %= valueMax;
    }

    // ternary
    value = Int64(valueMin ? valueMin : value0);
    value = (valueMin ? +(valueMin) : +(valueMax));
    value = (valueMin ? +(valueMin + value0) : +(valueMax + value0));
    value = (valueMin ? valueMin + value0 : valueMax + value0);
    value = (valueMin ? valueMin : valueMax);

    (void)value;
}

template <typename T>
void testUIntOperators(typename T::ValueType minValue = NumericLimits<T>::min())
{
    T value0;
    T valueMin = minValue;
    ASSERT_EQ(value0, valueMin);
    UInt64 valueMax = NumericLimits<UInt64>::max();

    // constructor
    UInt64 copy(valueMin);
    UInt64 copy2 = UInt64(valueMin);
    (void)copy;
    (void)copy2;

    // arithmetic
    UInt64 value = +valueMin;
    value = valueMin + valueMax;
    value = valueMax - valueMin;
    value = valueMin * valueMax;
    value = valueMin / valueMax;
    value = valueMin % valueMax;
    value = valueMin & valueMax;
    value = valueMin | valueMax;
    value = valueMin ^ valueMax;
    if constexpr (sizeof(T) < 4) // bypass integral promotion
    {
        value = valueMax << value0;
        value = valueMax >> value0;
    }
    else
    {
        value = valueMin << value0;
        value = valueMin >> value0;
    }

    // increment / decrement
    value = +valueMin;
    UInt64 origValue = value;
    ASSERT_EQ(origValue + 1, ++value);
    ASSERT_EQ(origValue, --value);
    ASSERT_EQ(origValue, value++);
    ASSERT_EQ(origValue + 1, value);
    ASSERT_EQ(origValue + 1, value--);
    ASSERT_EQ(origValue, value);

    // logical
    ASSERT_FALSE(!valueMax);
    ASSERT_FALSE(valueMin && valueMax);
    ASSERT_TRUE(valueMin || valueMax);

    // comparison
    ASSERT_FALSE(valueMin == valueMax);
    ASSERT_TRUE(valueMin != valueMax);
    ASSERT_TRUE(valueMin < valueMax);
    ASSERT_FALSE(valueMin > valueMax);
    ASSERT_TRUE(valueMin <= valueMax);
    ASSERT_FALSE(valueMin >= valueMax);

    // assignment
    value = UInt64(valueMin);
    value += valueMin;
    value -= valueMin;
    value *= valueMin;
    value /= valueMax;
    value %= valueMax;
    value &= valueMin;
    value |= valueMin;
    value ^= valueMin;
    value <<= value0;
    value >>= value0;

    // ternary
    value = UInt64(valueMin ? valueMin : value0);
    value = (valueMin ? +(valueMin) : +(valueMax));
    value = (valueMin ? +(valueMin + value0) : +(valueMax + value0));
    value = (valueMin ? valueMin + value0 : valueMax + value0);
    value = (valueMin ? valueMin : valueMax);

    (void)value;
}

template <typename T>
struct FloatTypeHelper
{
    using Type = Float32;
};

template <>
struct FloatTypeHelper<Float64>
{
    using Type = Float64;
};

template <typename T>
void testFloatOperators()
{
    using FloatWrapper = typename FloatTypeHelper<T>::Type;

    T value0;
    T valueMin = NumericLimits<T>::min();
    FloatWrapper valueMax = NumericLimits<FloatWrapper>::max();

    // constructor
    FloatWrapper copy(valueMin);
    FloatWrapper copy2 = FloatWrapper(valueMin);
    (void)copy;
    (void)copy2;

    // arithmetic
    FloatWrapper value = +valueMin;
    value = -valueMin;
    value = valueMin + valueMax;
    value = valueMin - valueMax;
    value = valueMin * valueMax;
    value = valueMax / valueMin;

    // increment / decrement
    value = static_cast<typename FloatWrapper::ValueType>(0);
    FloatWrapper origValue = value;
    ASSERT_DOUBLE_EQ(origValue + 1, ++value);
    ASSERT_DOUBLE_EQ(origValue, --value);
    ASSERT_DOUBLE_EQ(origValue, value++);
    ASSERT_DOUBLE_EQ(origValue + 1, value);
    ASSERT_DOUBLE_EQ(origValue + 1, value--);
    ASSERT_DOUBLE_EQ(origValue, value);

    // comparison
    ASSERT_TRUE(valueMin < valueMax);
    ASSERT_FALSE(valueMin > valueMax);
    ASSERT_TRUE(valueMin <= valueMax);
    ASSERT_FALSE(valueMin >= valueMax);

    // assignment
    value = FloatWrapper(valueMin);
    value += valueMin;
    value -= valueMin;
    value *= valueMin;
    value /= valueMin;

    // ternary
    value = FloatWrapper(valueMin < 0 ? valueMin : value0);
    value = (valueMin < 0 ? +(valueMin) : +(valueMax));
    value = (valueMin < 0 ? +(valueMin + value0) : +(valueMax + value0));
    value = (valueMin < 0 ? valueMin + value0 : valueMax + value0);
    value = (valueMin < 0 ? valueMin : valueMax);

    (void)value;
}

template <typename T>
void testSignedStdInt()
{
    using ValueType = typename T::ValueType;

    static_assert(NumericLimits<T>::min() == std::numeric_limits<ValueType>::min(), "shall be constexpr");
    static_assert(NumericLimits<T>::max() == std::numeric_limits<ValueType>::max(), "shall be constexpr");
    static_assert(toCheckedValue(NumericLimits<T>::max()) == std::numeric_limits<ValueType>::max(),
            "shall be constexpr");
    static_assert(NumericLimits<T>::max() == fromCheckedValue<T>(std::numeric_limits<ValueType>::max()),
            "shall be constexpr");

    static_assert(std::is_nothrow_constructible_v<T, ValueType>, "shall be noexcept");
    static_assert(noexcept(fromCheckedValue<T>(std::declval<ValueType>())), "shall be noexcept");
    static_assert(noexcept(toCheckedValue(std::declval<T>())), "shall be noexcept");

    const T minValue(NumericLimits<T>::min());
    ASSERT_EQ(NumericLimits<T>::min(), minValue);

    const T maxValue(NumericLimits<T>::max());
    ASSERT_EQ(NumericLimits<T>::max(), maxValue);

    T checkedMin = fromCheckedValue<T>(NumericLimits<T>::min());
    ASSERT_EQ(NumericLimits<T>::min(), toCheckedValue(checkedMin));

    T checkedMax = fromCheckedValue<T>(NumericLimits<T>::max());
    ASSERT_EQ(NumericLimits<T>::max(), toCheckedValue(checkedMax));

    T value;
    value = static_cast<ValueType>(0);
    ASSERT_EQ(0, value);
    value = static_cast<ValueType>(1);
    ASSERT_EQ(1, value);
    value = static_cast<ValueType>(-1);
    ASSERT_EQ(-1, value);

    testIntOperators<T>();
}

template <typename T>
void testUnsignedStdInt()
{
    using ValueType = typename T::ValueType;

    static_assert(NumericLimits<T>::min() == std::numeric_limits<ValueType>::min(), "shall be constexpr");
    static_assert(NumericLimits<T>::max() == std::numeric_limits<ValueType>::max(), "shall be constexpr");
    static_assert(toCheckedValue(NumericLimits<T>::max()) == std::numeric_limits<ValueType>::max(),
            "shall be constexpr");
    static_assert(NumericLimits<T>::max() == fromCheckedValue<T>(std::numeric_limits<ValueType>::max()),
            "shall be constexpr");

    static_assert(std::is_nothrow_constructible_v<T, ValueType>, "shall be noexcept");
    static_assert(noexcept(fromCheckedValue<T>(std::declval<ValueType>())), "shall be noexcept");
    static_assert(noexcept(toCheckedValue(std::declval<T>())), "shall be noexcept");

    const T minValue(NumericLimits<T>::min());
    ASSERT_EQ(NumericLimits<T>::min(), minValue);

    const T maxValue(NumericLimits<T>::max());
    ASSERT_EQ(NumericLimits<T>::max(), maxValue);

    T checkedMin = fromCheckedValue<T>(NumericLimits<T>::min());
    ASSERT_EQ(NumericLimits<T>::min(), toCheckedValue(checkedMin));

    T checkedMax = fromCheckedValue<T>(NumericLimits<T>::max());
    ASSERT_EQ(NumericLimits<T>::max(), toCheckedValue(checkedMax));

    T value;
    value = static_cast<ValueType>(0);
    ASSERT_EQ(0, value);
    value = static_cast<ValueType>(1);
    ASSERT_EQ(1, value);
    value = static_cast<ValueType>(2);
    ASSERT_EQ(2, value);

    testUIntOperators<T>();
}

template <typename T, typename T::ValueType MIN_VALUE, typename T::ValueType MAX_VALUE,
        bool IS_NOEXCEPT = false>
void testSignedInt()
{
    using ValueType = typename T::ValueType;

    static_assert(NumericLimits<T>::min() == MIN_VALUE, "shall be constexpr");
    static_assert(NumericLimits<T>::max() == MAX_VALUE, "shall be constexpr");
    static_assert(toCheckedValue(NumericLimits<T>::max()) == MAX_VALUE, "shall be constexpr");
    static_assert(NumericLimits<T>::max() == fromCheckedValue<T>(MAX_VALUE), "shall be constexpr");

    static_assert(std::is_nothrow_constructible_v<T, ValueType>, "shall be noexcept");
    static_assert(
            IS_NOEXCEPT == noexcept(fromCheckedValue<T>(std::declval<ValueType>())), "unexpected noexcept");
    static_assert(IS_NOEXCEPT == noexcept(toCheckedValue(std::declval<T>())), "unexpected noexcept");

    const T minValue(NumericLimits<T>::min());
    ASSERT_EQ(NumericLimits<T>::min(), minValue);

    const T maxValue(NumericLimits<T>::max());
    ASSERT_EQ(NumericLimits<T>::max(), maxValue);

    T checkedMin = fromCheckedValue<T>(NumericLimits<T>::min());
    ASSERT_EQ(NumericLimits<T>::min(), toCheckedValue(checkedMin));

    T checkedMax = fromCheckedValue<T>(NumericLimits<T>::max());
    ASSERT_EQ(NumericLimits<T>::max(), toCheckedValue(checkedMax));

    T value;
    ASSERT_EQ(0, value);
    value = -1;
    ASSERT_EQ(-1, value);
    value = 0;
    ASSERT_EQ(0, value);

    if constexpr (!IS_NOEXCEPT)
    {
        ASSERT_THROW(fromCheckedValue<T>(NumericLimits<T>::min() - 1), OutOfRangeException);
        ASSERT_THROW(fromCheckedValue<T>(NumericLimits<T>::max() + 1), OutOfRangeException);
        const T underMin = minValue - 1;
        ASSERT_THROW(toCheckedValue(underMin), OutOfRangeException);
        const T overMax = maxValue + 1;
        ASSERT_THROW(toCheckedValue(overMax), OutOfRangeException);
    }

    testIntOperators<T>();
}

template <typename T, typename T::ValueType MAX_VALUE, bool IS_NOEXCEPT = false>
void testUnsignedInt()
{
    using ValueType = typename T::ValueType;

    static_assert(NumericLimits<T>::min() == 0, "shall be constexpr");
    static_assert(NumericLimits<T>::max() == MAX_VALUE, "shall be constexpr");
    static_assert(toCheckedValue(NumericLimits<T>::max()) == MAX_VALUE, "shall be constexpr");
    static_assert(NumericLimits<T>::max() == fromCheckedValue<T>(MAX_VALUE), "shall be constexpr");

    static_assert(std::is_nothrow_constructible_v<T, ValueType>, "shall be noexcept");
    static_assert(
            IS_NOEXCEPT == noexcept(fromCheckedValue<T>(std::declval<ValueType>())), "unexpected noexcept");
    static_assert(IS_NOEXCEPT == noexcept(toCheckedValue(std::declval<T>())), "unexpected noexcept");

    const T minValue(NumericLimits<T>::min());
    ASSERT_EQ(NumericLimits<T>::min(), minValue);

    const T maxValue(NumericLimits<T>::max());
    ASSERT_EQ(NumericLimits<T>::max(), maxValue);

    T checkedMin = fromCheckedValue<T>(NumericLimits<T>::min());
    ASSERT_EQ(NumericLimits<T>::min(), toCheckedValue(checkedMin));

    T checkedMax = fromCheckedValue<T>(NumericLimits<T>::max());
    ASSERT_EQ(NumericLimits<T>::max(), toCheckedValue(checkedMax));

    T value;
    ASSERT_EQ(0, value);
    value = 1;
    ASSERT_EQ(1, value);
    value = NumericLimits<T>::max() - 1;
    ASSERT_EQ(NumericLimits<T>::max() - 1, value);

    if constexpr (!IS_NOEXCEPT)
    {
        ASSERT_THROW(fromCheckedValue<T>(NumericLimits<T>::max() + 1), OutOfRangeException);
        const T overMax = maxValue + 1;
        ASSERT_THROW(toCheckedValue(overMax), OutOfRangeException);
    }

    testUIntOperators<T>();
}

template <typename T>
void testSignedDynInt()
{
    using ValueType = typename T::ValueType;

    static_assert(NumericLimits<T>::min(8) == -128, "shall be constexpr");
    static_assert(NumericLimits<T>::max(8) == 127, "shall be constexpr");
    static_assert(toCheckedValue(NumericLimits<T>::max(8), 8) == 127, "shall be constexpr");
    static_assert(NumericLimits<T>::max(8) == fromCheckedValue<T>(127, 8), "shall be constexpr");

    static_assert(std::is_nothrow_constructible_v<T, ValueType>, "shall be noexcept");
    static_assert(!noexcept(fromCheckedValue<T>(std::declval<ValueType>(), std::declval<BitSize>())),
            "shall not be noexcept");
    static_assert(
            !noexcept(toCheckedValue(std::declval<T>(), std::declval<BitSize>())), "shall not be noexcept");

    ASSERT_THROW(fromCheckedValue<T>(0, 0), OutOfRangeException);
    ASSERT_THROW(fromCheckedValue<T>(0, sizeof(ValueType) * 8 + 1), OutOfRangeException);

    for (BitSize numBits = 1; numBits <= sizeof(ValueType) * 8; ++numBits)
    {
        const T minValue(NumericLimits<T>::min(numBits));
        ASSERT_EQ(NumericLimits<T>::min(numBits), minValue);

        const T maxValue(NumericLimits<T>::max(numBits));
        ASSERT_EQ(NumericLimits<T>::max(numBits), maxValue);

        T checkedMin = fromCheckedValue<T>(NumericLimits<T>::min(numBits), numBits);
        ASSERT_EQ(NumericLimits<T>::min(numBits), toCheckedValue(checkedMin, numBits));

        T checkedMax = fromCheckedValue<T>(NumericLimits<T>::max(numBits), numBits);
        ASSERT_EQ(NumericLimits<T>::max(numBits), toCheckedValue(checkedMax, numBits));

        T value;
        ASSERT_EQ(0, value);
        value = -1;
        ASSERT_EQ(-1, value);
        value = 0;
        ASSERT_EQ(0, value);

        if (numBits < sizeof(ValueType) * 8)
        {
            ASSERT_THROW(fromCheckedValue<T>(NumericLimits<T>::min(numBits) - 1, numBits), OutOfRangeException)
                    << numBits;
            ASSERT_THROW(fromCheckedValue<T>(NumericLimits<T>::max(numBits) + 1, numBits), OutOfRangeException);
            const T underMin = minValue - 1;
            ASSERT_THROW(toCheckedValue(underMin, numBits), OutOfRangeException);
            const T overMax = maxValue + 1;
            ASSERT_THROW(toCheckedValue(overMax, numBits), OutOfRangeException);
        }

        testIntOperators<T>(NumericLimits<T>::min(numBits));
    }
}

template <typename T>
void testUnsignedDynInt()
{
    using ValueType = typename T::ValueType;

    static_assert(NumericLimits<T>::min(8) == 0, "shall be constexpr");
    static_assert(NumericLimits<T>::max(8) == 255, "shall be constexpr");
    static_assert(toCheckedValue(NumericLimits<T>::max(8), 8) == 255, "shall be constexpr");
    static_assert(NumericLimits<T>::max(8) == fromCheckedValue<T>(255, 8), "shall be constexpr");

    static_assert(std::is_nothrow_constructible_v<T, ValueType>, "shall be noexcept");
    static_assert(!noexcept(fromCheckedValue<T>(std::declval<ValueType>(), std::declval<BitSize>())),
            "shall not be noexcept");
    static_assert(
            !noexcept(toCheckedValue(std::declval<T>(), std::declval<BitSize>())), "shall not be noexcept");

    ASSERT_THROW(fromCheckedValue<T>(0, 0), OutOfRangeException);
    ASSERT_THROW(fromCheckedValue<T>(0, sizeof(ValueType) * 8 + 1), OutOfRangeException);

    for (BitSize numBits = 1; numBits <= sizeof(ValueType) * 8; ++numBits)
    {
        const T minValue(NumericLimits<T>::min(numBits));
        ASSERT_EQ(NumericLimits<T>::min(numBits), minValue);

        const T maxValue(NumericLimits<T>::max(numBits));
        ASSERT_EQ(NumericLimits<T>::max(numBits), maxValue);

        T checkedMin = fromCheckedValue<T>(NumericLimits<T>::min(numBits), numBits);
        ASSERT_EQ(NumericLimits<T>::min(numBits), toCheckedValue(checkedMin, numBits));

        T checkedMax = fromCheckedValue<T>(NumericLimits<T>::max(numBits), numBits);
        ASSERT_EQ(NumericLimits<T>::max(numBits), toCheckedValue(checkedMax, numBits));

        T value;
        ASSERT_EQ(0, value);
        value = 1;
        ASSERT_EQ(1, value);
        value = NumericLimits<T>::max(numBits) - 1;
        ASSERT_EQ(NumericLimits<T>::max(numBits) - 1, value);

        if (numBits < sizeof(ValueType) * 8)
        {
            ASSERT_THROW(fromCheckedValue<T>(NumericLimits<T>::max(numBits) + 1, numBits), OutOfRangeException);
            const T overMax = maxValue + 1;
            ASSERT_THROW(toCheckedValue(overMax, numBits), OutOfRangeException);
        }

        testUIntOperators<T>(NumericLimits<T>::min(numBits));
    }
}

template <typename T>
constexpr void testFloat()
{
    using ValueType = typename T::ValueType;

    if constexpr (std::is_same_v<T, Float16>)
    {
        // cannot compare floating point numbers directly
        static_assert(NumericLimits<T>::min() > 0.F, "shall be constexpr");
        static_assert(NumericLimits<T>::max() > 0.F, "shall be constexpr");
        static_assert(toCheckedValue(NumericLimits<T>::max()) > 0.F, "shall be constexpr");
        static_assert(NumericLimits<T>::max() > fromCheckedValue<T>(0.F), "shall be constexpr");

        ASSERT_FLOAT_EQ(NumericLimits<T>::min(), 6.103515625e-05F);
        ASSERT_FLOAT_EQ(NumericLimits<T>::max(), 65504.F);
    }
    else
    {
        // cannot compare floating point numbers directly
        static_assert(NumericLimits<T>::min() > static_cast<ValueType>(0.), "shall be constexpr");
        static_assert(NumericLimits<T>::max() > static_cast<ValueType>(0.), "shall be constexpr");

        ASSERT_DOUBLE_EQ(NumericLimits<T>::min(), std::numeric_limits<ValueType>::min());
    }

    static_assert(std::is_nothrow_constructible_v<T, ValueType>, "shall be noexcept");
    static_assert(noexcept(fromCheckedValue<T>(std::declval<ValueType>())), "shall be noexcept");
    static_assert(noexcept(toCheckedValue(std::declval<T>())), "shall be noexcept");

    const T minValue(NumericLimits<T>::min());
    ASSERT_DOUBLE_EQ(NumericLimits<T>::min(), minValue);

    const T maxValue(NumericLimits<T>::max());
    ASSERT_DOUBLE_EQ(NumericLimits<T>::max(), maxValue);

    T checkedMin = fromCheckedValue<T>(NumericLimits<T>::min());
    ASSERT_DOUBLE_EQ(NumericLimits<T>::min(), toCheckedValue(checkedMin));

    T checkedMax = fromCheckedValue<T>(NumericLimits<T>::max());
    ASSERT_DOUBLE_EQ(NumericLimits<T>::max(), toCheckedValue(checkedMax));

    T value;
    ASSERT_DOUBLE_EQ(static_cast<ValueType>(0.0), value);
    value = static_cast<ValueType>(1.0);
    ASSERT_DOUBLE_EQ(static_cast<ValueType>(1.0), value);
    value = static_cast<ValueType>(0.0);
    ASSERT_DOUBLE_EQ(static_cast<ValueType>(0.0), value);

    T checked_value = fromCheckedValue<T>(static_cast<ValueType>(1.0));
    ASSERT_DOUBLE_EQ(static_cast<ValueType>(1.0), toCheckedValue(checked_value));

    testFloatOperators<T>();
}

TEST(TypesTest, boolType)
{
    static_assert(std::is_nothrow_constructible_v<Bool, bool>, "shall be noexcept");
    static_assert(noexcept(fromCheckedValue<Bool>(std::declval<bool>())), "shall be noexcept");
    static_assert(noexcept(toCheckedValue(std::declval<Bool>())), "shall be noexcept");

    const Bool minValue(NumericLimits<Bool>::min());
    ASSERT_EQ(NumericLimits<Bool>::min(), minValue);

    const Bool maxValue(NumericLimits<Bool>::max());
    ASSERT_EQ(NumericLimits<Bool>::max(), maxValue);

    Bool checkedMin = fromCheckedValue<Bool>(NumericLimits<Bool>::min());
    ASSERT_EQ(NumericLimits<Bool>::min(), toCheckedValue(checkedMin));

    Bool checkedMax = fromCheckedValue<Bool>(NumericLimits<Bool>::max());
    ASSERT_EQ(NumericLimits<Bool>::max(), toCheckedValue(checkedMax));

    Bool value;
    ASSERT_EQ(false, value);
    value = true;
    ASSERT_EQ(true, value);
    value = false;
    ASSERT_EQ(false, value);

    testBoolOperators();
}

TEST(TypesTest, signedStdIntTypes)
{
    testSignedStdInt<Int8>();
    testSignedStdInt<Int16>();
    testSignedStdInt<Int32>();
    testSignedStdInt<Int64>();
}

TEST(TypesTest, unsignedStdIntTypes)
{
    testUnsignedStdInt<UInt8>();
    testUnsignedStdInt<UInt16>();
    testUnsignedStdInt<UInt32>();
    testUnsignedStdInt<UInt64>();
}

TEST(TypesTest, signedBitFieldTypes)
{
    testSignedInt<Int1, -1, 0>();
    testSignedInt<Int2, -2, 1>();
    testSignedInt<Int3, -4, 3>();
    testSignedInt<Int4, -8, 7>();
    testSignedInt<Int5, -16, 15>();
    testSignedInt<Int6, -32, 31>();
    testSignedInt<Int7, -64, 63>();
    testSignedInt<Int9, -256, 255>();
    testSignedInt<Int10, -512, 511>();
    testSignedInt<Int11, -1024, 1023>();
    testSignedInt<Int12, -2048, 2047>();
    testSignedInt<Int13, -4096, 4095>();
    testSignedInt<Int14, -8192, 8191>();
    testSignedInt<Int15, -16384, 16383>();
    testSignedInt<Int17, -65536, 65535>();
    testSignedInt<Int18, -131072, 131071>();
    testSignedInt<Int19, -262144, 262143>();
    testSignedInt<Int20, -524288, 524287>();
    testSignedInt<Int21, -1048576, 1048575>();
    testSignedInt<Int22, -2097152, 2097151>();
    testSignedInt<Int23, -4194304, 4194303>();
    testSignedInt<Int24, -8388608, 8388607>();
    testSignedInt<Int25, -16777216, 16777215>();
    testSignedInt<Int26, -33554432, 33554431>();
    testSignedInt<Int27, -67108864, 67108863>();
    testSignedInt<Int28, -134217728, 134217727>();
    testSignedInt<Int29, -268435456, 268435455>();
    testSignedInt<Int30, -536870912, 536870911>();
    testSignedInt<Int31, -1073741824, 1073741823>();
    testSignedInt<Int33, -4294967296LL, 4294967295LL>();
    testSignedInt<Int34, -8589934592LL, 8589934591LL>();
    testSignedInt<Int35, -17179869184LL, 17179869183LL>();
    testSignedInt<Int36, -34359738368LL, 34359738367LL>();
    testSignedInt<Int37, -68719476736LL, 68719476735LL>();
    testSignedInt<Int38, -137438953472LL, 137438953471LL>();
    testSignedInt<Int39, -274877906944LL, 274877906943LL>();
    testSignedInt<Int40, -549755813888LL, 549755813887LL>();
    testSignedInt<Int41, -1099511627776LL, 1099511627775LL>();
    testSignedInt<Int42, -2199023255552LL, 2199023255551LL>();
    testSignedInt<Int43, -4398046511104LL, 4398046511103LL>();
    testSignedInt<Int44, -8796093022208LL, 8796093022207LL>();
    testSignedInt<Int45, -17592186044416LL, 17592186044415LL>();
    testSignedInt<Int46, -35184372088832LL, 35184372088831LL>();
    testSignedInt<Int47, -70368744177664LL, 70368744177663LL>();
    testSignedInt<Int48, -140737488355328LL, 140737488355327LL>();
    testSignedInt<Int49, -281474976710656LL, 281474976710655LL>();
    testSignedInt<Int50, -562949953421312LL, 562949953421311LL>();
    testSignedInt<Int51, -1125899906842624LL, 1125899906842623LL>();
    testSignedInt<Int52, -2251799813685248LL, 2251799813685247LL>();
    testSignedInt<Int53, -4503599627370496LL, 4503599627370495LL>();
    testSignedInt<Int54, -9007199254740992LL, 9007199254740991LL>();
    testSignedInt<Int55, -18014398509481984LL, 18014398509481983LL>();
    testSignedInt<Int56, -36028797018963968LL, 36028797018963967LL>();
    testSignedInt<Int57, -72057594037927936LL, 72057594037927935LL>();
    testSignedInt<Int58, -144115188075855872LL, 144115188075855871LL>();
    testSignedInt<Int59, -288230376151711744LL, 288230376151711743LL>();
    testSignedInt<Int60, -576460752303423488LL, 576460752303423487LL>();
    testSignedInt<Int61, -1152921504606846976LL, 1152921504606846975LL>();
    testSignedInt<Int62, -2305843009213693952LL, 2305843009213693951LL>();
    testSignedInt<Int63, -4611686018427387904LL, 4611686018427387903LL>();

    testSignedInt<Int<5>, -16, 15>();
    testSignedStdInt<Int<8>>();

    ASSERT_TRUE(detail::needs_range_check_v<Int<5>>);
    ASSERT_FALSE(detail::needs_range_check_v<Int<8>>);
}

TEST(TypesTest, unsignedBitFieldTypes)
{
    testUnsignedInt<UInt1, 1ULL>();
    testUnsignedInt<UInt2, 3ULL>();
    testUnsignedInt<UInt3, 7ULL>();
    testUnsignedInt<UInt4, 15ULL>();
    testUnsignedInt<UInt5, 31ULL>();
    testUnsignedInt<UInt6, 63ULL>();
    testUnsignedInt<UInt7, 127ULL>();
    testUnsignedInt<UInt9, 511ULL>();
    testUnsignedInt<UInt10, 1023ULL>();
    testUnsignedInt<UInt11, 2047ULL>();
    testUnsignedInt<UInt12, 4095ULL>();
    testUnsignedInt<UInt13, 8191ULL>();
    testUnsignedInt<UInt14, 16383ULL>();
    testUnsignedInt<UInt15, 32767ULL>();
    testUnsignedInt<UInt17, 131071ULL>();
    testUnsignedInt<UInt18, 262143ULL>();
    testUnsignedInt<UInt19, 524287ULL>();
    testUnsignedInt<UInt20, 1048575ULL>();
    testUnsignedInt<UInt21, 2097151ULL>();
    testUnsignedInt<UInt22, 4194303ULL>();
    testUnsignedInt<UInt23, 8388607ULL>();
    testUnsignedInt<UInt24, 16777215ULL>();
    testUnsignedInt<UInt25, 33554431ULL>();
    testUnsignedInt<UInt26, 67108863ULL>();
    testUnsignedInt<UInt27, 134217727ULL>();
    testUnsignedInt<UInt28, 268435455ULL>();
    testUnsignedInt<UInt29, 536870911ULL>();
    testUnsignedInt<UInt30, 1073741823ULL>();
    testUnsignedInt<UInt31, 2147483647ULL>();
    testUnsignedInt<UInt33, 8589934591ULL>();
    testUnsignedInt<UInt34, 17179869183ULL>();
    testUnsignedInt<UInt35, 34359738367ULL>();
    testUnsignedInt<UInt36, 68719476735ULL>();
    testUnsignedInt<UInt37, 137438953471ULL>();
    testUnsignedInt<UInt38, 274877906943ULL>();
    testUnsignedInt<UInt39, 549755813887ULL>();
    testUnsignedInt<UInt40, 1099511627775ULL>();
    testUnsignedInt<UInt41, 2199023255551ULL>();
    testUnsignedInt<UInt42, 4398046511103ULL>();
    testUnsignedInt<UInt43, 8796093022207ULL>();
    testUnsignedInt<UInt44, 17592186044415ULL>();
    testUnsignedInt<UInt45, 35184372088831ULL>();
    testUnsignedInt<UInt46, 70368744177663ULL>();
    testUnsignedInt<UInt47, 140737488355327ULL>();
    testUnsignedInt<UInt48, 281474976710655ULL>();
    testUnsignedInt<UInt49, 562949953421311ULL>();
    testUnsignedInt<UInt50, 1125899906842623ULL>();
    testUnsignedInt<UInt51, 2251799813685247ULL>();
    testUnsignedInt<UInt52, 4503599627370495ULL>();
    testUnsignedInt<UInt53, 9007199254740991ULL>();
    testUnsignedInt<UInt54, 18014398509481983ULL>();
    testUnsignedInt<UInt55, 36028797018963967ULL>();
    testUnsignedInt<UInt56, 72057594037927935ULL>();
    testUnsignedInt<UInt57, 144115188075855871ULL>();
    testUnsignedInt<UInt58, 288230376151711743ULL>();
    testUnsignedInt<UInt59, 576460752303423487ULL>();
    testUnsignedInt<UInt60, 1152921504606846975ULL>();
    testUnsignedInt<UInt61, 2305843009213693951ULL>();
    testUnsignedInt<UInt62, 4611686018427387903ULL>();
    testUnsignedInt<UInt63, 9223372036854775807ULL>();

    testUnsignedInt<UInt<5>, 31ULL>();
    testUnsignedStdInt<UInt<8>>();

    ASSERT_TRUE(detail::needs_range_check_v<UInt<5>>);
    ASSERT_FALSE(detail::needs_range_check_v<UInt<8>>);
}

TEST(TypesTest, signedDynIntTypes)
{
    testSignedDynInt<DynInt8>();
    testSignedDynInt<DynInt16>();
    testSignedDynInt<DynInt32>();
    testSignedDynInt<DynInt64>();

    ASSERT_TRUE(detail::needs_range_check_v<DynInt8>);
}

TEST(TypesTest, unsignedDynIntTypes)
{
    testUnsignedDynInt<DynUInt8>();
    testUnsignedDynInt<DynUInt16>();
    testUnsignedDynInt<DynUInt32>();
    testUnsignedDynInt<DynUInt64>();

    ASSERT_TRUE(detail::needs_range_check_v<DynUInt8>);
}

TEST(TypesTest, signedVarIntTypes)
{
    testSignedInt<VarInt16, -16383, 16383>();
    testSignedInt<VarInt32, -268435455, 268435455>();
    testSignedInt<VarInt64, -72057594037927935LL, 72057594037927935LL>();
    testSignedInt<VarInt, INT64_MIN, INT64_MAX, true>();
}

TEST(TypesTest, unsignedVarIntTypes)
{
    testUnsignedInt<VarUInt16, 32767>();
    testUnsignedInt<VarUInt32, 536870911>();
    testUnsignedInt<VarUInt64, 144115188075855871ULL>();
    testUnsignedInt<VarUInt, UINT64_MAX, true>();
    testUnsignedInt<VarSize, 2147483647>();
}

TEST(TypesTest, floatTypes)
{
    testFloat<Float16>();
    testFloat<Float32>();
    testFloat<Float64>();
}

TEST(TypesTest, staticCastNoWarning)
{
    {
        Int32 i32;
        Int16 i16 = static_cast<Int16::ValueType>(i32);
        i32 = Int32(i16);
    }
    {
        Int16 i16;
        DynInt8 dynI8 = static_cast<DynInt8::ValueType>(i16);
        i16 = Int16(dynI8);
    }
    {
        Float16 f16;
        Int16 i16 = static_cast<Int16::ValueType>(f16);
        f16 = static_cast<Float16::ValueType>(i16);
    }
}

TEST(TypesTest, bitSizeOfBool)
{
    EXPECT_EQ(1, detail::bitSizeOf(Bool(true)));
    EXPECT_EQ(1, detail::bitSizeOf(Bool(false)));
}

TEST(TypesTest, bitSizeOfIntWrapper)
{
    EXPECT_EQ(1, detail::bitSizeOf(Int1()));
    EXPECT_EQ(7, detail::bitSizeOf(Int7()));
    EXPECT_EQ(8, detail::bitSizeOf(Int8()));
    EXPECT_EQ(15, detail::bitSizeOf(Int15()));
    EXPECT_EQ(16, detail::bitSizeOf(Int16()));
    EXPECT_EQ(17, detail::bitSizeOf(Int17()));
    EXPECT_EQ(31, detail::bitSizeOf(Int31()));
    EXPECT_EQ(32, detail::bitSizeOf(Int32()));
    EXPECT_EQ(33, detail::bitSizeOf(Int33()));
    EXPECT_EQ(63, detail::bitSizeOf(Int63()));
    EXPECT_EQ(64, detail::bitSizeOf(Int64()));

    EXPECT_EQ(1, detail::bitSizeOf(UInt1()));
    EXPECT_EQ(7, detail::bitSizeOf(UInt7()));
    EXPECT_EQ(8, detail::bitSizeOf(UInt8()));
    EXPECT_EQ(15, detail::bitSizeOf(UInt15()));
    EXPECT_EQ(16, detail::bitSizeOf(UInt16()));
    EXPECT_EQ(17, detail::bitSizeOf(UInt17()));
    EXPECT_EQ(31, detail::bitSizeOf(UInt31()));
    EXPECT_EQ(32, detail::bitSizeOf(UInt32()));
    EXPECT_EQ(33, detail::bitSizeOf(UInt33()));
    EXPECT_EQ(63, detail::bitSizeOf(UInt63()));
    EXPECT_EQ(64, detail::bitSizeOf(UInt64()));

    EXPECT_EQ(13, detail::bitSizeOf(Int<13>()));
}

TEST(TypesTest, bitSizeOfVarInt16)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarInt16(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarInt16(static_cast<int16_t>(1U << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt16(-static_cast<int16_t>(1U << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt16(static_cast<int16_t>((1U << (6U)) - 1))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt16(-static_cast<int16_t>((1U << (6U)) - 1))));

    EXPECT_EQ(16, detail::bitSizeOf(VarInt16(static_cast<int16_t>(1U << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt16(-static_cast<int16_t>(1U << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt16(static_cast<int16_t>((1U << (6U + 8)) - 1))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt16(-static_cast<int16_t>((1U << (6U + 8)) - 1))));

    const int16_t outOfRangeValue = static_cast<int16_t>(1U << (6U + 8));
    ASSERT_THROW(detail::bitSizeOf(VarInt16(outOfRangeValue)), CppRuntimeException);
}

TEST(TypesTest, bitSizeOfVarInt32)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarInt32(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarInt32(static_cast<int32_t>(1U << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(1U << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt32(static_cast<int32_t>((1U << (6U)) - 1))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(((1U << (6U)) - 1)))));

    EXPECT_EQ(16, detail::bitSizeOf(VarInt32(static_cast<int32_t>(1U << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(1U << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt32(static_cast<int32_t>((1U << (6U + 7)) - 1))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(((1U << (6U + 7)) - 1)))));

    EXPECT_EQ(24, detail::bitSizeOf(VarInt32(static_cast<int32_t>(1U << (6U + 7)))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(1U << (6U + 7)))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt32(static_cast<int32_t>((1U << (6U + 7 + 7)) - 1))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt32(-static_cast<int32_t>((1U << (6U + 7 + 7)) - 1))));

    EXPECT_EQ(32, detail::bitSizeOf(VarInt32(static_cast<int32_t>(1U << (6U + 7 + 7)))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt32(-static_cast<int32_t>(1U << (6U + 7 + 7)))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt32(static_cast<int32_t>((1U << (6U + 7 + 7 + 8)) - 1))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt32(-static_cast<int32_t>((1U << (6U + 7 + 7 + 8)) - 1))));

    const int32_t outOfRangeValue = static_cast<int32_t>(1U << (6U + 7 + 7 + 8));
    ASSERT_THROW(detail::bitSizeOf(VarInt32(outOfRangeValue)), CppRuntimeException);
}

TEST(TypesTest, bitSizeOfVarInt64)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarInt64(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (0U)))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U)) - 1))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U)) - 1))));

    EXPECT_EQ(16, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U)))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7)) - 1))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7)) - 1))));

    EXPECT_EQ(24, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7)))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7)))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7)) - 1))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7)) - 1))));

    EXPECT_EQ(32, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7)))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7)))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7)) - 1))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7)) - 1))));

    EXPECT_EQ(40, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7)))));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7)))));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7)) - 1))));
    EXPECT_EQ(
            40, detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7)) - 1))));

    EXPECT_EQ(48, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(48, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(48,
            detail::bitSizeOf(VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7)) - 1))));
    EXPECT_EQ(48,
            detail::bitSizeOf(VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7)) - 1))));

    EXPECT_EQ(56, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(56, detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(56,
            detail::bitSizeOf(
                    VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7)) - 1))));
    EXPECT_EQ(56,
            detail::bitSizeOf(
                    VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7)) - 1))));

    EXPECT_EQ(
            64, detail::bitSizeOf(VarInt64(static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(64,
            detail::bitSizeOf(VarInt64(-static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7)))));
    EXPECT_EQ(64,
            detail::bitSizeOf(
                    VarInt64(static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7 + 8)) - 1))));
    EXPECT_EQ(64,
            detail::bitSizeOf(
                    VarInt64(-static_cast<int64_t>((UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7 + 8)) - 1))));

    const int64_t outOfRangeValue = static_cast<int64_t>(UINT64_C(1) << (6U + 7 + 7 + 7 + 7 + 7 + 7 + 8));
    ASSERT_THROW(detail::bitSizeOf(VarInt64(outOfRangeValue)), CppRuntimeException);
}

TEST(TypesTest, bitSizeOfVarUInt16)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt16(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarUInt16(1U << (0U))));
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt16((1U << (7U)) - 1)));

    EXPECT_EQ(16, detail::bitSizeOf(VarUInt16(1U << (7U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarUInt16((1U << (7U + 8)) - 1)));

    const uint16_t outOfRangeValue = 1U << (7U + 8);
    ASSERT_THROW(detail::bitSizeOf(VarUInt16(outOfRangeValue)), CppRuntimeException);
}

TEST(TypesTest, bitSizeOfVarUInt32)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt32(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarUInt32(1U << (0U))));
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt32((1U << (7U)) - 1)));

    EXPECT_EQ(16, detail::bitSizeOf(VarUInt32(1U << (7U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarUInt32((1U << (7U + 7)) - 1)));

    EXPECT_EQ(24, detail::bitSizeOf(VarUInt32(1U << (7U + 7))));
    EXPECT_EQ(24, detail::bitSizeOf(VarUInt32((1U << (7U + 7 + 7)) - 1)));

    EXPECT_EQ(32, detail::bitSizeOf(VarUInt32(1U << (7U + 7 + 7))));
    EXPECT_EQ(32, detail::bitSizeOf(VarUInt32((1U << (7U + 7 + 7 + 8)) - 1)));

    const uint32_t outOfRangeValue = 1U << (7U + 7 + 7 + 8);
    ASSERT_THROW(detail::bitSizeOf(VarUInt32(outOfRangeValue)), CppRuntimeException);
}

TEST(TypesTest, bitSizeOfVarUInt64)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt64(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (0U))));
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U)) - 1)));

    EXPECT_EQ(16, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7)) - 1)));

    EXPECT_EQ(24, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7))));
    EXPECT_EQ(24, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7)) - 1)));

    EXPECT_EQ(32, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7 + 7))));
    EXPECT_EQ(32, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7 + 7)) - 1)));

    EXPECT_EQ(40, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7 + 7 + 7))));
    EXPECT_EQ(40, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7 + 7 + 7)) - 1)));

    EXPECT_EQ(48, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7 + 7 + 7 + 7))));
    EXPECT_EQ(48, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7)) - 1)));

    EXPECT_EQ(56, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7))));
    EXPECT_EQ(56, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7 + 7)) - 1)));

    EXPECT_EQ(64, detail::bitSizeOf(VarUInt64(UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7 + 7))));
    EXPECT_EQ(64, detail::bitSizeOf(VarUInt64((UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7 + 7 + 8)) - 1)));

    const uint64_t outOfRangeValue = UINT64_C(1) << (7U + 7 + 7 + 7 + 7 + 7 + 7 + 8);
    ASSERT_THROW(detail::bitSizeOf(VarUInt64(outOfRangeValue)), CppRuntimeException);
}

TEST(TypesTest, bitSizeOfVarInt)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarInt(INT64_C(0))));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 6U) + 1)));
    EXPECT_EQ(8, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 6U) - 1)));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 6U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 6U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 13U) + 1)));
    EXPECT_EQ(16, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 13U) - 1)));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 13U))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 13U))));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 20U) + 1)));
    EXPECT_EQ(24, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 20U) - 1)));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 20U))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 20U))));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 27U) + 1)));
    EXPECT_EQ(32, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 27U) - 1)));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 27U))));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 27U))));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 34U) + 1)));
    EXPECT_EQ(40, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 34U) - 1)));
    EXPECT_EQ(48, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 34U))));
    EXPECT_EQ(48, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 34U))));
    EXPECT_EQ(48, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 41U) + 1)));
    EXPECT_EQ(48, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 41U) - 1)));
    EXPECT_EQ(56, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 41U))));
    EXPECT_EQ(56, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 41U))));
    EXPECT_EQ(56, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 48U) + 1)));
    EXPECT_EQ(56, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 48U) - 1)));
    EXPECT_EQ(64, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 48U))));
    EXPECT_EQ(64, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 48U))));
    EXPECT_EQ(64, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 55U) + 1)));
    EXPECT_EQ(64, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 55U) - 1)));
    EXPECT_EQ(72, detail::bitSizeOf(VarInt(-static_cast<int64_t>(UINT64_C(1) << 55U))));
    EXPECT_EQ(72, detail::bitSizeOf(VarInt(static_cast<int64_t>(UINT64_C(1) << 55U))));
    EXPECT_EQ(72, detail::bitSizeOf(VarInt(INT64_MIN + 1)));
    EXPECT_EQ(72, detail::bitSizeOf(VarInt(INT64_MAX)));

    // special case, INT64_MIN is stored as -0
    EXPECT_EQ(8, detail::bitSizeOf(VarInt(INT64_MIN)));
}

TEST(TypesTest, bitSizeOfVarUInt)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt(UINT64_C(0))));
    EXPECT_EQ(8, detail::bitSizeOf(VarUInt((UINT64_C(1) << 7U) - 1)));
    EXPECT_EQ(16, detail::bitSizeOf(VarUInt((UINT64_C(1) << 7U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarUInt((UINT64_C(1) << 14U) - 1)));
    EXPECT_EQ(24, detail::bitSizeOf(VarUInt((UINT64_C(1) << 14U))));
    EXPECT_EQ(24, detail::bitSizeOf(VarUInt((UINT64_C(1) << 21U) - 1)));
    EXPECT_EQ(32, detail::bitSizeOf(VarUInt((UINT64_C(1) << 21U))));
    EXPECT_EQ(32, detail::bitSizeOf(VarUInt((UINT64_C(1) << 28U) - 1)));
    EXPECT_EQ(40, detail::bitSizeOf(VarUInt((UINT64_C(1) << 28U))));
    EXPECT_EQ(40, detail::bitSizeOf(VarUInt((UINT64_C(1) << 35U) - 1)));
    EXPECT_EQ(48, detail::bitSizeOf(VarUInt((UINT64_C(1) << 35U))));
    EXPECT_EQ(48, detail::bitSizeOf(VarUInt((UINT64_C(1) << 42U) - 1)));
    EXPECT_EQ(56, detail::bitSizeOf(VarUInt((UINT64_C(1) << 42U))));
    EXPECT_EQ(56, detail::bitSizeOf(VarUInt((UINT64_C(1) << 49U) - 1)));
    EXPECT_EQ(64, detail::bitSizeOf(VarUInt((UINT64_C(1) << 49U))));
    EXPECT_EQ(64, detail::bitSizeOf(VarUInt((UINT64_C(1) << 56U) - 1)));
    EXPECT_EQ(72, detail::bitSizeOf(VarUInt((UINT64_C(1) << 56U))));
    EXPECT_EQ(72, detail::bitSizeOf(VarUInt(UINT64_MAX)));
}

TEST(TypesTest, bitSizeOfVarSize)
{
    EXPECT_EQ(8, detail::bitSizeOf(VarSize(0)));

    EXPECT_EQ(8, detail::bitSizeOf(VarSize(1U << (0U))));
    EXPECT_EQ(8, detail::bitSizeOf(VarSize((1U << (7U)) - 1)));

    EXPECT_EQ(16, detail::bitSizeOf(VarSize(1U << (7U))));
    EXPECT_EQ(16, detail::bitSizeOf(VarSize((1U << (7U + 7)) - 1)));

    EXPECT_EQ(24, detail::bitSizeOf(VarSize(1U << (7U + 7))));
    EXPECT_EQ(24, detail::bitSizeOf(VarSize((1U << (7U + 7 + 7)) - 1)));

    EXPECT_EQ(32, detail::bitSizeOf(VarSize(1U << (7U + 7 + 7))));
    EXPECT_EQ(32, detail::bitSizeOf(VarSize((1U << (7U + 7 + 7 + 7)) - 1)));

    EXPECT_EQ(40, detail::bitSizeOf(VarSize(1U << (7U + 7 + 7 + 7))));
    EXPECT_EQ(40, detail::bitSizeOf(VarSize((1U << (2U + 7 + 7 + 7 + 8)) - 1)));

    const uint32_t outOfRangeValue = 1U << (2U + 7 + 7 + 7 + 8);
    ASSERT_THROW(detail::bitSizeOf(VarSize(outOfRangeValue)), CppRuntimeException);
}

TEST(TypesTest, bitSizeOfFloat)
{
    EXPECT_EQ(16, detail::bitSizeOf(Float16()));
    EXPECT_EQ(32, detail::bitSizeOf(Float32()));
    EXPECT_EQ(64, detail::bitSizeOf(Float64()));
}

} // namespace zserio
