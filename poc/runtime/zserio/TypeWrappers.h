#ifndef ZSERIO_TYPE_WRAPPERS_H_INC
#define ZSERIO_TYPE_WRAPPERS_H_INC

#include <stdint.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/BitStreamReader.h>
#include <zserio/String.h>
#include <zserio/BitSizeOfCalculator.h>
#include <zserio/BitFieldUtil.h>
#include <zserio/AllocatorPropagatingCopy.h>
#include <zserio/pmr/PolymorphicAllocator.h>
#include <zserio/ArrayTraits.h>

namespace zserio
{

template <class T>
struct StdTypeArrayTraits;

// Integer

enum INTEGERTYPE
{
    DYNAMIC = -1,
};

template <typename T, int N = 8*sizeof(T)>
struct Integer
{
    using value_type = T;
    using allocator_type = ::std::allocator<uint8_t>;
    using ZserioPackingContext = ::zserio::DeltaContext;
    using needs_initialize_offsets = ::std::false_type;

    // all wrappers can be constructed with allocator even unused
    // so that templated field gets always initialized correctly
    template <class ALLOC, typename = std::enable_if_t<is_allocator<ALLOC>::value>>
    explicit Integer(const ALLOC &)
        : m_value_()
    {
    }

    Integer(T v = {})
        : m_value_(v)
    {}

    T get() const
    {
        return m_value_;
    }

    void set(T value)
    {
        m_value_ = value;
    }

    operator T () const
    {
        return m_value_;
    }

    // todo: ZserioPackingContext

private:
    value_type m_value_;
};

using Int8 = Integer<int8_t>;
using UInt5 = Integer<uint8_t, 5>;
using Int16 = Integer<int16_t>;
using Int32 = Integer<int32_t>;
using UInt32 = Integer<uint32_t>;
using DynInt32 = Integer<int32_t, DYNAMIC>;

namespace detail
{

template <typename T, int N>
void writeImpl(::zserio::BitStreamWriter& out, Integer<T, N> value, std::false_type, std::false_type)
{
    out.writeBits(value.get(), UINT8_C(N));
}

template <typename T, int N>
void writeImpl(::zserio::BitStreamWriter& out, Integer<T, N> value, std::false_type, std::true_type)
{
    out.writeBits64(value.get(), UINT8_C(N));
}

template <typename T, int N>
void writeImpl(::zserio::BitStreamWriter& out, Integer<T, N> value, std::true_type, std::false_type)
{
    out.writeSignedBits(value.get(), UINT8_C(N));
}

template <typename T, int N>
void writeImpl(::zserio::BitStreamWriter& out, Integer<T, N> value, std::true_type, std::true_type)
{
    out.writeSignedBits64(value.get(), UINT8_C(N));
}

template <typename T>
void writeImpl(::zserio::BitStreamWriter& out, Integer<T, ::zserio::DYNAMIC> value, std::false_type, int N)
{
    if (N > 32)
        out.writeBits64(value.get(), UINT8_C(N));
    else
        out.writeBits(value.get(), UINT8_C(N));
}

template <typename T>
void writeImpl(::zserio::BitStreamWriter& out, Integer<T, ::zserio::DYNAMIC> value, std::true_type, int N)
{
    if (N > 32)
        out.writeSignedBits64(value.get(), UINT8_C(N));
    else
        out.writeSignedBits(value.get(), UINT8_C(N));
}

template <typename T, int N>
void readImpl(::zserio::BitStreamReader& in, Integer<T, N>& value, std::false_type, std::false_type)
{
    value = in.readBits(UINT8_C(N));
}

template <typename T, int N>
void readImpl(::zserio::BitStreamReader& in, Integer<T, N>& value, std::false_type, std::true_type)
{
    value = in.readBits64(UINT8_C(N));
}

template <typename T, int N>
void readImpl(::zserio::BitStreamReader& in, Integer<T, N>& value, std::true_type, std::false_type)
{
    value = in.readSignedBits(UINT8_C(N));
}

template <typename T, int N>
void readImpl(::zserio::BitStreamReader& in, Integer<T, N>& value, std::true_type, std::true_type)
{
    value = in.readSignedBits64(UINT8_C(N));
}

template <typename T>
void readImpl(::zserio::BitStreamReader& in, Integer<T, ::zserio::DYNAMIC>& value, std::false_type, int N)
{
    if (N > 32)
        value = static_cast<T>(in.readBits64(UINT8_C(N)));
    else
        value = static_cast<T>(in.readBits(UINT8_C(N)));
}

template <typename T>
void readImpl(::zserio::BitStreamReader& in, Integer<T, ::zserio::DYNAMIC>& value, std::true_type, int N)
{
    if (N > 32)
        value = static_cast<T>(in.readSignedBits64(UINT8_C(N)));
    else
        value = static_cast<T>(in.readSignedBits(UINT8_C(N)));
}

template <typename T>
void validate(Integer<T, ::zserio::DYNAMIC> value, int N)
{
    const int32_t lowerBound = static_cast<int32_t>(::zserio::getBitFieldLowerBound(N, std::is_signed_v<T>));
    const int32_t upperBound = static_cast<int32_t>(::zserio::getBitFieldUpperBound(N, std::is_signed_v<T>));
    if (value < lowerBound || value > upperBound)
    {
        throw ::zserio::CppRuntimeException("Value ") << value.get() <<
                " exceeds the range of <" <<
                lowerBound << ".." << upperBound << ">!";
    }
}

template <typename T, int N>
void write(::zserio::BitStreamWriter& out, Integer<T, N> value)
{
    static_assert(N != ::zserio::DYNAMIC);
    writeImpl(out, value, std::is_signed<T>(), std::bool_constant<(N > 32)>());
}

template <typename T, int N>
void write(::zserio::BitStreamWriter& out, typename Integer<T, N>::ZserioPackingContext& context, Integer<T, N> value)
{
    static_assert(N != ::zserio::DYNAMIC);
    context.template write<::zserio::StdTypeArrayTraits<Integer<T, N>>>(out, value);
}

template <typename T>
void write(::zserio::BitStreamWriter& out, Integer<T, ::zserio::DYNAMIC> value, int N)
{
    writeImpl(out, value, std::is_signed<T>(), N);
}

template <typename T, int N>
void read(::zserio::BitStreamReader& in, Integer<T, N>& value, const typename Integer<T, N>::allocator_type & = {})
{
    static_assert(N != ::zserio::DYNAMIC);
    readImpl(in, value, std::is_signed<T>(), std::bool_constant<(N > 32)>());
}

template <typename T, int N>
void read(::zserio::BitStreamReader& in, typename Integer<T, N>::ZserioPackingContext& context, Integer<T, N>& value, const typename Integer<T, N>::allocator_type & = {})
{
    static_assert(N != ::zserio::DYNAMIC);
    value = context.template read<::zserio::StdTypeArrayTraits<Integer<T, N>>>(in);
}

template <typename T>
void read(::zserio::BitStreamReader& in, Integer<T, ::zserio::DYNAMIC>& value, int N)
{
    readImpl(in, value, std::is_signed<T>(), N);
}

template <typename T, int N>
size_t bitSizeOf(Integer<T, N>, size_t)
{
    static_assert(N != ::zserio::DYNAMIC);
    return UINT8_C(N);
}

template <typename T>
size_t bitSizeOf(Integer<T, ::zserio::DYNAMIC>, int N, size_t)
{
    return UINT8_C(N);
}

template <typename T, int N>
size_t bitSizeOf(Integer<T, N> value, typename Integer<T, N>::ZserioPackingContext& context, size_t)
{
    return context.template bitSizeOf<::zserio::StdTypeArrayTraits<Integer<T, N>>>(value);
}

template <typename T, int N>
size_t initializeOffsets(Integer<T, N> value, size_t bitPosition)
{
    static_assert(N != ::zserio::DYNAMIC);
    return bitPosition + bitSizeOf(value, bitPosition);
}

template <typename T>
size_t initializeOffsets(Integer<T, ::zserio::DYNAMIC> value, int N, size_t bitPosition)
{
    return bitPosition + bitSizeOf(value, N, bitPosition);
}

template <typename T, int N>
void initPackingContext(Integer<T, N> value, typename Integer<T, N>::ZserioPackingContext& context)
{
    context.template init<::zserio::StdTypeArrayTraits<Integer<T, N>>>(value);
}

} // namespace detail

template <typename T, int N>
inline CppRuntimeException& operator<<(CppRuntimeException& exception, Integer<T, N> value)
{
    return exception << value.get();
}

// Boolean

using Boolean = Integer<bool, 1>;

// VarInt

enum VARTYPE
{
    VAR,
    VAR16,
    VAR32,
    VAR64,
    VARSIZE,
};

template <typename T, VARTYPE N>
struct VarInt
{
    using value_type = T;
    using allocator_type = ::std::allocator<uint8_t>;
    using ZserioPackingContext = ::zserio::DeltaContext;
    using needs_initialize_offsets = ::std::false_type;

    template <class ALLOC, typename = std::enable_if_t<is_allocator<ALLOC>::value>>
    explicit VarInt(const ALLOC&)
        : m_value_()
    {
    }

    VarInt(T v = {})
        : m_value_(v)
    {}

    VarInt(zserio::PropagateAllocatorT, VarInt a, const allocator_type& allocator)
        : m_value_(::zserio::allocatorPropagatingCopy(a.m_value_, allocator))
    {}

    T get() const
    {
        return m_value_;
    }

    void set(T value)
    {
        m_value_ = value;
    }

    operator T () const
    {
        return m_value_;
    }

    // todo: ZserioPackingContext

private:
    value_type m_value_;
};

using VarSize = VarInt<uint32_t, VARSIZE>;

namespace detail
{

template <typename T, VARTYPE N>
size_t bitSizeOfImpl(VarInt<T, N> value, std::false_type, std::integral_constant<int, VAR>)
{
    return zserio::bitSizeOfVarUInt(value);
}

template <typename T, VARTYPE N>
size_t bitSizeOfImpl(VarInt<T, N> value, std::false_type, std::integral_constant<int, VAR16>)
{
    return zserio::bitSizeOfVarUInt16(value);
}

template <typename T, VARTYPE N>
size_t bitSizeOfImpl(VarInt<T, N> value, std::false_type, std::integral_constant<int, VAR32>)
{
    return zserio::bitSizeOfVarUInt32(value);
}

template <typename T, VARTYPE N>
size_t bitSizeOfImpl(VarInt<T, N> value, std::false_type, std::integral_constant<int, VAR64>)
{
    return zserio::bitSizeOfVarUInt64(value);
}

template <typename T, VARTYPE N>
size_t bitSizeOfImpl(VarInt<T, N> value, std::false_type, std::integral_constant<int, VARSIZE>)
{
    return zserio::bitSizeOfVarSize(value);
}

template <typename T, VARTYPE N>
size_t bitSizeOfImpl(VarInt<T, N> value, std::true_type, std::integral_constant<int, VAR>)
{
    return zserio::bitSizeOfVarInt(value);
}

template <typename T, VARTYPE N>
size_t bitSizeOfImpl(VarInt<T, N> value, std::true_type, std::integral_constant<int, VAR16>)
{
    return zserio::bitSizeOfVarInt16(value);
}

template <typename T, VARTYPE N>
size_t bitSizeOfImpl(VarInt<T, N> value, std::true_type, std::integral_constant<int, VAR32>)
{
    return zserio::bitSizeOfVarInt32(value);
}

template <typename T, VARTYPE N>
size_t bitSizeOfImpl(VarInt<T, N> value, std::true_type, std::integral_constant<int, VAR64>)
{
    return zserio::bitSizeOfVarInt64(value);
}

template <typename T, VARTYPE N>
void writeImpl(::zserio::BitStreamWriter& out, VarInt<T, N> value, std::false_type, std::integral_constant<int, VAR>)
{
    out.writeVarUInt(value);
}

template <typename T, VARTYPE N>
void writeImpl(::zserio::BitStreamWriter& out, VarInt<T, N> value, std::false_type, std::integral_constant<int, VAR16>)
{
    out.writeVarUInt16(value);
}

template <typename T, VARTYPE N>
void writeImpl(::zserio::BitStreamWriter& out, VarInt<T, N> value, std::false_type, std::integral_constant<int, VAR32>)
{
    out.writeVarUInt32(value);
}

template <typename T, VARTYPE N>
void writeImpl(::zserio::BitStreamWriter& out, VarInt<T, N> value, std::false_type, std::integral_constant<int, VAR64>)
{
    out.writeVarUInt64(value);
}

template <typename T, VARTYPE N>
void writeImpl(::zserio::BitStreamWriter& out, VarInt<T, N> value, std::false_type, std::integral_constant<int, VARSIZE>)
{
    out.writeVarSize(value);
}

template <typename T, VARTYPE N>
void writeImpl(::zserio::BitStreamWriter& out, VarInt<T, N> value, std::true_type, std::integral_constant<int, VAR>)
{
    out.writeVarInt(value);
}

template <typename T, VARTYPE N>
void writeImpl(::zserio::BitStreamWriter& out, VarInt<T, N> value, std::true_type, std::integral_constant<int, VAR16>)
{
    out.writeVarInt16(value);
}

template <typename T, VARTYPE N>
void writeImpl(::zserio::BitStreamWriter& out, VarInt<T, N> value, std::true_type, std::integral_constant<int, VAR32>)
{
    out.writeVarInt32(value);
}

template <typename T, VARTYPE N>
void writeImpl(::zserio::BitStreamWriter& out, VarInt<T, N> value, std::true_type, std::integral_constant<int, VAR64>)
{
    out.writeVarInt64(value);
}

template <typename T, VARTYPE N>
void readImpl(::zserio::BitStreamReader& in, VarInt<T, N>& value, std::false_type, std::integral_constant<int, VAR>)
{
    value = in.readVarUInt();
}

template <typename T, VARTYPE N>
void readImpl(::zserio::BitStreamReader& in, VarInt<T, N>& value, std::false_type, std::integral_constant<int, VAR16>)
{
    value = in.readVarUInt16();
}

template <typename T, VARTYPE N>
void readImpl(::zserio::BitStreamReader& in, VarInt<T, N>& value, std::false_type, std::integral_constant<int, VAR32>)
{
    value = in.readVarUInt32();
}

template <typename T, VARTYPE N>
void readImpl(::zserio::BitStreamReader& in, VarInt<T, N>& value, std::false_type, std::integral_constant<int, VAR64>)
{
    value = in.readVarUInt64();
}

template <typename T, VARTYPE N>
void readImpl(::zserio::BitStreamReader& in, VarInt<T, N>& value, std::false_type, std::integral_constant<int, VARSIZE>)
{
    value = in.readVarSize();
}

template <typename T, VARTYPE N>
void readImpl(::zserio::BitStreamReader& in, VarInt<T, N>& value, std::true_type, std::integral_constant<int, VAR>)
{
    value = in.readVarInt();
}

template <typename T, VARTYPE N>
void readImpl(::zserio::BitStreamReader& in, VarInt<T, N>& value, std::true_type, std::integral_constant<int, VAR16>)
{
    value = in.readVarInt16();
}

template <typename T, VARTYPE N>
void readImpl(::zserio::BitStreamReader& in, VarInt<T, N>& value, std::true_type, std::integral_constant<int, VAR32>)
{
    value = in.readVarInt32();
}

template <typename T, VARTYPE N>
void readImpl(::zserio::BitStreamReader& in, VarInt<T, N>& value, std::true_type, std::integral_constant<int, VAR64>)
{
    value = in.readVarInt64();
}

template <typename T, VARTYPE N>
size_t bitSizeOf(VarInt<T, N> value, size_t bitPosition)
{
    return bitSizeOfImpl(value, std::is_signed<T>(), std::integral_constant<int, N>());
}

template <typename T, VARTYPE N>
size_t bitSizeOf(VarInt<T, N> value, typename VarInt<T, N>::ZserioPackingContext& context, size_t)
{
    return context.template bitSizeOf<::zserio::StdTypeArrayTraits<VarInt<T, N>>>(value);
}

template <typename T, VARTYPE N>
size_t initializeOffsets(VarInt<T, N> value, size_t bitPosition)
{
    return bitPosition + bitSizeOf(value, bitPosition);
}

template <typename T, VARTYPE N>
void write(::zserio::BitStreamWriter& out, VarInt<T, N> value)
{
    writeImpl(out, value, std::is_signed<T>(), std::integral_constant<int, N>());
}

template <typename T, VARTYPE N>
void write(::zserio::BitStreamWriter& out, typename VarInt<T, N>::ZserioPackingContext& context, VarInt<T, N> value)
{
    context.template write<::zserio::StdTypeArrayTraits<VarInt<T, N>>>(out, value);
}

template <typename T, VARTYPE N>
void read(::zserio::BitStreamReader& in, VarInt<T, N>& value, const typename VarInt<T, N>::allocator_type & = {})
{
    readImpl(in, value, std::is_signed<T>(), std::integral_constant<int, N>());
}

template <typename T, VARTYPE N>
void read(::zserio::BitStreamReader& in, typename VarInt<T, N>::ZserioPackingContext& context, VarInt<T, N>& value, const typename VarInt<T, N>::allocator_type & = {})
{
    value = context.template read<::zserio::StdTypeArrayTraits<VarInt<T, N>>>(in);
}

template <typename T, VARTYPE N>
void initPackingContext(VarInt<T, N> value, typename VarInt<T, N>::ZserioPackingContext& context)
{
    context.template init<::zserio::StdTypeArrayTraits<VarInt<T, N>>>(value);
}

template <typename T, VARTYPE N>
inline CppRuntimeException& operator<<(CppRuntimeException& exception, VarInt<T, N> value)
{
    return exception << value.get();
}

} // namespace detail

// todo: Float

template <typename T, int N = sizeof(T)>
struct Float
{
    using value_type = T;
    using allocator_type = ::std::allocator<uint8_t>;

    //todo
};

// String

template <class ALLOC>
struct BasicString
{
    using value_type = zserio::string<ALLOC>;
    using allocator_type = ALLOC;
    using ZserioPackingContext = ::std::nullptr_t;
    using needs_initialize_offsets = ::std::false_type;

    explicit BasicString(const allocator_type& allocator = {})
        : m_value_(allocator)
    {}
    explicit BasicString(zserio::BitStreamReader& in, const allocator_type& alloc = {})
    {
        read(in, alloc);
    }
    BasicString(zserio::PropagateAllocatorT, const BasicString& a, const allocator_type& allocator)
        : m_value_(::zserio::allocatorPropagatingCopy(a.m_value_, allocator))
    {}

    BasicString(const char* s)
        : m_value_(s)
    {}

    BasicString(const value_type& s)
        : m_value_(s)
    {
    }

    BasicString(value_type&& s)
        : m_value_(std::move(s))
    {
    }

    const value_type& get() const
    {
        return m_value_;
    }

    value_type& get()
    {
        return m_value_;
    }

    operator const value_type& () const
    {
        return m_value_;
    }

    void set(const value_type& value)
    {
        m_value_ = value;
    }

    void set(value_type&& value)
    {
        m_value_ = std::move(value);
    }

    bool operator== (const BasicString& c) const
    {
        return m_value_ == c.m_value_;
    }

    bool operator< (const BasicString& c) const
    {
        return m_value_ < c.m_value_;
    }

private:
    value_type m_value_;
};

namespace detail
{

template <typename A>
void write(::zserio::BitStreamWriter& out, const BasicString<A>& value)
{
    out.writeString(value.get());
}

template <typename A>
void write(::zserio::BitStreamWriter& out, typename BasicString<A>::ZserioPackingContext&, const BasicString<A>& value)
{
    // string can't be packed
    write(out, value);
}

template <typename A>
void read(::zserio::BitStreamReader& in, BasicString<A>& value, const typename BasicString<A>::allocator_type& alloc = {})
{
    value = in.readString(alloc);
}

// this will work with reading into pmr::String (PropagatingPolymorphicAllocator<char>) from
// PropagatingPolymorphicAllocator<uint8_t>
// For some reason allocators are slightly different and old zserio is doing type conversion
// which seems to be unnecessary
template <typename A, typename ALLOC,
    typename = std::enable_if_t<!std::is_same_v<A, ALLOC>>>
void read(::zserio::BitStreamReader& in, BasicString<A>& value, const ALLOC& alloc)
{
    value = in.readString(typename BasicString<A>::allocator_type());
}

template <typename A, typename ALLOC = A>
void read(::zserio::BitStreamReader& in, typename BasicString<A>::ZserioPackingContext&, BasicString<A>& value, const ALLOC& alloc = {})
{
    // string can't be packed
    read(in, value, alloc);
}

template <typename A>
size_t bitSizeOf(const BasicString<A>& value, size_t bitPosition)
{
    return ::zserio::bitSizeOfString(value.get());
}

template <typename A>
size_t bitSizeOf(const BasicString<A>& value, typename BasicString<A>::ZserioPackingContext&, size_t bitPosition)
{
    // string can't be packed
    return bitSizeOf(value, bitPosition);
}

template <typename A>
size_t initializeOffsets(const BasicString<A>& value, size_t bitPosition)
{
    return bitPosition + bitSizeOf(value, bitPosition);
}

template <typename A>
void initPackingContext(const BasicString<A>&, typename BasicString<A>::ZserioPackingContext&)
{
    // string can't be packed
}

} // namespace detail

template <class ALLOC>
inline CppRuntimeException& operator<<(CppRuntimeException& exception, const BasicString<ALLOC>& value)
{
    return exception << value.c_str();
}

using String = BasicString<std::allocator<char>>;

namespace pmr
{
    using String = zserio::BasicString<PropagatingPolymorphicAllocator<char>>;
}

} //zserio

namespace std
{

template <typename T, int N>
struct hash<::zserio::Integer<T, N>>
{
    size_t operator()(::zserio::Integer<T, N> value) const
    {
        return ::zserio::calcHashCode(::zserio::HASH_SEED, value.get());
    }
};

template <typename T, ::zserio::VARTYPE N>
struct hash<::zserio::VarInt<T, N>>
{
    size_t operator()(::zserio::VarInt<T, N> value) const
    {
        return ::zserio::calcHashCode(::zserio::HASH_SEED, value.get());
    }
};

template <typename A>
struct hash<::zserio::BasicString<A>>
{
    size_t operator()(const ::zserio::BasicString<A>& value) const
    {
        return ::zserio::calcHashCode(::zserio::HASH_SEED, value.get());
    }
};

} //namespace std

#endif
