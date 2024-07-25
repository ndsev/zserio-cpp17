# Overview

This document outlines the design framework for the new Zserio C++ support, embracing C++17 standards over
the previously utilized C++11.

This shift allows Zserio to use the advancements offered by C++17 instead of re-implementing concepts like
Polymorphic Allocators (PMR) support, `std::string_view`, etc... Central to our approach is
adherence to the MISRA C++ 2023 guidelines, guaranteeing that our implementations are not only modern
but also secure and reliable (functional safety).

## Design

[Motivation](#motivation)

[Built-in Types](#built-in-types)

[Constants](#constants)

[Enumeration Types](#enumeration-types)

[Bitmask Types](#bitmask-types)

[Compound Types](#compound-types)

[Array Types](#array-types)

[Offsets](#offsets)

[Templates](#templates)

[Service Types](#service-types)

[Pubsub Types](#pubsub-types)

[SQL Tables](#sql-tables)

[SQL Databases](#sql-databases)

### Motivation

The user experience of the current C++ generator brought new ideas and enhancements which could improve
usability of the generated code. Besides of that as time flies, users asked for support of new modern
C++17 standard.

However, such changes in generated code would break backward compatibility of user applications, which is
undesirable. Therefore, our idea is to  implement a brand new C++17 generator with incompatible API at one step.

The main aim of this document is to describe the design of the new C++17 generator in detail.

The following two main features had been reconsidered during designing:

- Implementation of the Parameterized Types

  New C++17 generator should model the Zserio Structures, Choices and Unions by new Data View abstraction.
  This new abstraction will solve naturally implementation of the Parameterized Types without a need of
  two-phase initialization combined by custom copy and move constructors (for more information please
  see [Data View Approach](#data-view-approach) section).

- Implementation of the Templates

  New C++17 generator should model the Zserio templates by C++ native templates. Because there is a need to
  distinguish which Zserio native type is used as a template argument, this solution involves as well
  implementation of dedicated C++ Zserio types for all Zserio built-in types in the C++ runtime library
  (for more information please see [Built-in Types](#built-in-types) section).

### Built-in Types

Zserio built-in types will be implemented in C++ runtime library as dedicated C++ Zserio type classes.
These classes will provide

- implicit constructor from the C++ native type and
- implicit conversion to the C++ native type.

Implicit constructors from the C++ native types and implicit conversion to the C++ native type is convenient to
keep integral expressions as simple as possible.

Beside of that, dedicated C++ Zserio type classes will implement explicit conversion to the C++ native type
which automatically checks a range check of the value.

The following table shows the mapping of all Zserio built-in types into the C++ Zserio type together with
the C++ native type:

Zserio Type     | C++ Zserio Type                    | C++ Native Type         |
----------------|-------------------------------------|-------------------------|
uint8           | `zserio::UInt8`                     | `uint8_t`               |
uint16          | `zserio::UInt16`                    | `uint16_t`              |
uint32          | `zserio::UInt32`                    | `uint32_t`              |
uint64          | `zserio::UInt64`                    | `uint64_t`              |
int8            | `zserio::Int8`                      | `int8_t`                |
int16           | `zserio::Int16`                     | `int16_t`               |
int32           | `zserio::Int32`                     | `int32_t`               |
int64           | `zserio::Int64`                     | `int64_t`               |
bit:1...bit:8   | `zserio::UInt1`...`zserio::UInt8`   | `uint8_t`               |
bit:9...bit:16  | `zserio::UInt9`...`zserio::UInt16`  | `uint16_t`              |
bit:17...bit:32 | `zserio::UInt17`...`zserio::UInt32` | `uint32_t`              |
bit:33...bit:64 | `zserio::UInt33`...`zserio::UInt64` | `uint64_t`              |
int:1...int:8   | `zserio::Int1`...`zserio::Int8`     | `int8_t`                |
int:9...int:16  | `zserio::Int9`...`zserio::Int16`    | `int16_t`               |
int:17...int:32 | `zserio::Int17`...`zserio::Int32`   | `int32_t`               |
int:33...int:64 | `zserio::Int33`...`zserio::Int64`   | `int64_t`               |
float16         | `zserio::Float16`                   | `float`                 |
float32         | `zserio::Float32`                   | `float`                 |
float64         | `zserio::Double`                    | `double`                |
varint16        | `zserio::VarInt16`                  | `int16_t`               |
varint32        | `zserio::VarInt32`                  | `int32_t`               |
varint64        | `zserio::VarInt64`                  | `int64_t`               |
varint          | `zserio::VarInt`                    | `int64_t`               |
varuint16       | `zserio::VarUInt16`                 | `uint16_t`              |
varuint32       | `zserio::VarUInt32`                 | `uint32_t`              |
varuint64       | `zserio::VarUInt64`                 | `uint64_t`              |
varuint         | `zserio::VarUInt`                   | `uint64_t`              |
bool            | `zserio::Bool`                      | `bool`                  |
string          | `zserio::String`                    | `std::string`           |
extern          | `zserio::BitBuffer`                 | N/A                      |
bytes           | `zserio::Bytes`                     | `std::vector<uint8_t>`  |

> Note that the implicit constructors from the C++ native type will break a MISRA 2023 rule 15.1.3
  (Conversion operators and constructors that are callable with a single argument shall be explicit).

### Constants

The Constants will be implemented using `constexpr` keyword instead of `const` keyword. This is possible
because Zserio constants can be defined by expressions which contain only another constants, literals
and enumeration or bitmask values (using `valueof` operator).

### Enumeration Types

The Enumeration Types will be implemented in the same way as in the old C++11 generator.

This means that the Enumeration Type will be modeled by the C++11 enumeration together with the specialization
of the enumeration traits and methods implemented in the C++ runtime library.

### Bitmask Types

The Bitmask Types will be implemented in the same way as in the old C++11 generator.

This means that the Bitmask Type will be modeled by the custom class which will contain a inner enumeration
of the bitmask values. All bitmask operators (`==`, `!=`, `<`, `|`, `&`, `^`, `~`, `|=`, `&=`, `^=`)
will be implemented as non-member functions.

> Note that the generated Bitmask Type class will implement default constructor from the bitmask value. This is
  intended and it will break a MISRA 2023 rule 15.1.3 (Conversion operators and constructors that are callable
  with a single argument shall be explicit).

### Compound Types

The Compound Types will be implemented using new Data-View approach which will solve naturally implementation
of the Parameterized Types.

#### Data View Approach

The solution separates data (`Data`) from the schema logic (`View`).

The main idea is that all the parameters are already present somewhere in the BLOB structure. For just keeping
such a BLOB data (`Data`), it's not needed to store also auxiliary data in memory
(i.e. references to parameters). `Data` will be only simple data class. Parameters can be simply calculated
on the fly once user needs to work with the BLOB - just by constructing the `View` which knows how and where
to get the parameters. The `Data` is then used to access the actual data.

The `View` only contains simple parameters stored by value and a reference to the underlying data.
Users will be responsible to keep the underlying data alive while working with the `View`. `View` can become
invalid once the underlying data are changed, which is similar to how the `span`, `string_view` or e.g.
`vector::iterator` behave (reallocation of the underlying data).

The `View` by itself is immutable and provide only getters together with functions defined in the schema.
There is no way how to modify underlying data through `View` interface.

Once working with the `View`, the getters also returns `View`s and thus parameters are always available.

Zserio features like `write`, `read`, `bitSizeOf` will be implemented by means of the global non-member
functions in the `detail` namespace to emphasize that this interface is not part of the user API. These global
non-member functions will be called by the global functions
`::zserio::serialize`, `::zserio::deserialize` and `::zserio::bitSizeOf`.

`View` will be implemented as a templated class `View<Data>` instead of an inner class. This is
because native templates with `Data<T>::View` cause many problems. For example, compiler cannot deduce
such a View, so all function calls require explicit type specification, like `operator==<T>(view1, view2)`.
This prevents storing Views in `std::unordered/map` without an extra comparator.

##### Example

Using the Zserio schema

```
struct Param(uint16 parameter)
{
    uint16 value;
    uint32 extraValue if parameter == 11;
};

struct ParameterizedParamHolder
{
    uint16 parameter = 11;
    Param(parameter) param;
};
```

the C++ generated code will look like the following:

```cpp
struct Param
{
    using allocator_type = ::std::allocator<uint8_t>;

    Param() noexcept;

    explicit Param(const allocator_type& allocator);

    Param(
        ::zserio::UInt16 value_,
        ::zserio::Optional<::zserio::UInt32> extraValue);

    ::zserio::UInt16 value;
    ::zserio::Optional<::zserio::UInt32> extraValue;
};

bool operator==(const Param& lhs, const Param& rhs);
bool operator<(const Param& lhs, const Param& rhs);
bool operator!=(const Param& lhs, const Param& rhs);
bool operator>(const Param& lhs, const Param& rhs);
bool operator<=(const Param& lhs, const Param& rhs);
bool operator>=(const Param& lhs, const Param& rhs);

namespace zserio
{

template <>
class View<Param>
{
public:
    View(
        const Param& data,
        ::zserio::UInt16 parameter_) noexcept;

    ::zserio::UInt16 getParameter() const;
    ::zserio::UInt16 getValue() const;
    ::zserio::Optional<::zserio::UInt32> getExtraValue() const;

private:
    const Param& m_data;
    ::zserio::UInt16 m_parameter_;
};

bool operator==(const View<Param>& lhs, const View<Param>& rhs);
bool operator<(const View<Param>& lhs, const View<Param>& rhs);
bool operator!=(const View<Param>& lhs, const View<Param>& rhs);
bool operator>(const View<Param>& lhs, const View<Param>& rhs);
bool operator<=(const View<Param>& lhs, const View<Param>& rhs);
bool operator>=(const View<Param>& lhs, const View<Param>& rhs);

namespace detail
{

void validate(const ::zserio::View<Param>& view);

void write(::zserio::BitStreamWriter& writer, const ::zserio::View<Param>& view);

View<Param> read(::zserio::BitStreamReader& reader, Param& data, ::zserio::UInt16 parameter_,
        const Param::allocator_type& allocator = {});

size_t bitSizeOf(const ::zserio::View<Param>& view, size_t bitPosition);

} // namespace detail

} // namespace zserio

namespace std
{

template<>
struct hash<Param>
{
    size_t operator()(const Param& data) const;
};

template<>
struct hash<::zserio::View<Param>>
{
    size_t operator()(const ::zserio::View<Param>& view) const;
};

} // namespace std
```

```cpp
class ParameterizedParamHolder
{
struct:
    using allocator_type = ::std::allocator<uint8_t>;

    ParameterizedParamHolder() noexcept;

    explicit ParameterizedParamHolder(const allocator_type& allocator);

    ParameterizedParamHolder(
        ::zserio::UInt16 parameter_,
        ::param_types::Param param_
    );

    ::zserio::UInt16 parameter;
    ::param_types::Param param;
};

bool operator==(const ParameterizedParamHolder& lhs, const ParameterizedParamHolder& rhs);
bool operator<(const ParameterizedParamHolder& lhs, const ParameterizedParamHolder& rhs);
bool operator!=(const ParameterizedParamHolder& lhs, const ParameterizedParamHolder& rhs);
bool operator>(const ParameterizedParamHolder& lhs, const ParameterizedParamHolder& rhs);
bool operator<=(const ParameterizedParamHolder& lhs, const ParameterizedParamHolder& rhs);
bool operator>=(const ParameterizedParamHolder& lhs, const ParameterizedParamHolder& rhs);

namespace zserio
{

template <>
class View<ParameterizedParamHolder>
{
public:
    View(const ParameterizedParamHolder& data) noexcept :

    ::zserio::UInt16 getParameter() const;
    ::param_types::Param::View getParam() const;

private:
    const ParameterizedParamHolder& m_data;
};

bool operator==(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);
bool operator<(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);
bool operator!=(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);
bool operator>(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);
bool operator<=(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);
bool operator>=(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);

namespace zserio
{

void validate(const ::zserio::View<ParameterizedParamHolder>& view);

void write(::zserio::BitStreamWriter& writer, const ::zserio::View<ParameterizedParamHolder>& view);

View<ParameterizedParamHolder> read(::zserio::BitStreamReader& reader, ParameterizedParamHolder& data,
        const ParameterizedParamHolder::allocator_type& allocator = {});

size_t bitSizeOf(const ::zserio::View<ParameterizedParamHolder>& view, size_t bitPosition);

} // namespace detail

} // namespace zserio

namespace std
{

template <>
struct hash<ParameterizedParamHolder>
{
    size_t operator()(const ParameterizedParamHolder& data) const;
};

template <>
struct hash<::zserio::View<ParameterizedParamHolder>>
{
    size_t operator()(const ::zserio::View<ParameterizedParamHolder>& view) const;
};

} // namespace std
```

##### Writer Flow

```cpp
// data can be simply filled in any order without any restrictions
ParameterizedParamHolder holderData;
holderData.parameter = 11;
holderData.param.value = 1;
holderData.param.extraValue = 5;

// ::zserio::serializeToFile checks all the constraints (calls ::zserio::detail::validate() method)
ParameterizedParamHolder::View holderView(holderData);
::zserio::serializeToFile(holderView, "holder.blob");
```

##### Reader Flow

```cpp
ParameterizedParamHolder holderData;
const ::zserio::View<ParameterizedParamHolder> holderView = ::zserio::deserializeFromFile("holder.blob", holderData);
// parameter is stored in the view so it doesn't need to be provided
::std::cout << holderView.getParam().getParameter() << ::std::endl;
```

#####

#### Choice And Union Types

The Choice Types will use a dedicated abstraction `::zserio::Variant` with the same interface as
`std::variant` from C++17 standard.

The only implementation difference will be that `::zserio::Variant` will allocate additional dynamic memory
for types bigger than some threshold with a default provided one for users that don't need/want to deal with it.
Thus, allocator must be provided to `::zserio::Variant` during construction.

Such optimization is desirable to save memory for choices where one case contains significantly bigger types
than other cases. If such choice is stored in a large array, it can reserve significant amount of memory.

#### Optional Members

The Optional Members will use a dedicated abstraction `::zserio::Optional` with the same interface as
`std::optional` from C++17 standard.

The only implementation difference will be that `::zserio::Optional` will allocate additional dynamic memory
for types bigger than some threshold and for recursive Optional Members. Thus, allocator must be provided to 
`::zserio::Optional` during construction.

Such optimization is desirable to save memory for large optionals which are not present.

### Array Types

The Array Types will use normal `std::vector` abstraction from C++17 standard library in the `Data`.
Because array of the `Data` elements is not enough for a `View`, there is a new
dedicated `::zserio::ArrayView` abstraction which is used by `View`.

##### Example

Using the Zserio schema

```
struct ArrayHolder
{
    int8 array[];
};
```

the C++ generated code will look like the following:

```cpp
class ArrayHolder
{
struct:
    using allocator_type = ::std::allocator<uint8_t>;

    ArrayHolder() noexcept;

    explicit ArrayHolder(const allocator_type& allocator);

    ArrayHolder(
        ::zserio::vector<::zserio::Int8> array_);

    ::zserio::vector<::zserio::Int8> array;
};

bool operator==(const ArrayHolder& lhs, const ArrayHolder& rhs);
bool operator<(const ArrayHolder& lhs, const ArrayHolder& rhs);
bool operator!=(const ArrayHolder& lhs, const ArrayHolder& rhs);
bool operator>(const ArrayHolder& lhs, const ArrayHolder& rhs);
bool operator<=(const ArrayHolder& lhs, const ArrayHolder& rhs);
bool operator>=(const ArrayHolder& lhs, const ArrayHolder& rhs);

namespace zserio
{

template <>
class View<ArrayHolder>
{
public:
    View(const ArrayHolder& data) noexcept;

    ::zserio::ArrayView<::zserio::Int8> getArray() const;

private:
    CoordShift m_shift_;
    const ArrayHolder& m_data;
};

bool operator==(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);
bool operator<(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);
bool operator!=(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);
bool operator>(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);
bool operator<=(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);
bool operator>=(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);

namespace detail
{

void validate(const ::zserio::View<ArrayHolder>& view);

void write(::zserio::BitStreamWriter& writer, const ::zserio::View<ArrayHolder>& view);

View<ArrayHolder> read(::zserio::BitStreamReader& reader, ArrayHolder& data,
        const ArrayHolder::allocator_type& allocator = {});

size_t bitSizeOf(const ::zserio::View<ArrayHolder>& view, size_t bitPosition);

} // namespace detail

} // namespace zserio

namespace std
{

template <>
struct hash<ArrayHolder>
{
    size_t operator()(const ArrayHolder& data) const;
};

template <>
struct hash<::zserio::View<ArrayHolder>>
{
    size_t operator()(const ::zserio::View<ArrayHolder>& view) const;
};

} // namespace std
```

### Offsets

The offsets will be initialized automatically during `::zserio::serialize` method by means of the method
`::zserio::detail::initializeOffsets`. Because of that the method `::zserio::serialize`
must accept non-const reference to the `Data`. However, `::zserio::serialize` method will have an overload
which accepts const reference to the `Data`. This overload could be called only for Zserio structures without
any offsets. Any attempt to call this overload for Zserio structures with offsets will result in the C++
compilation error.

### Templates

The Templates will use C++ native templates. If the template argument is a Zserio built-in type, Zserio C++ type
will be used instead of C++ native types.

##### Example

Using the Zserio schema

```
struct Field<T>
{
    T value;
};

struct Compound
{
    uint32 value;
};

struct StructTemplatedField
{
    Field<uint32> uint32Field;
    Field<Compound> compoundField;
};
```

the C++ generated code will look like the following:

```cpp
template <typename T>
struct Field
{
    using allocator_type = ::std::allocator<uint8_t>;

    Field() noexcept :
            Field(allocator_type())
    {}

    explicit Field(const allocator_type& allocator) :
            value(allocator)
    {}

    Field(U field_
    ) :
            value(::std::move(value_))
    {}

    T value;
};

template <typename T>
bool operator==(const Field<T>& lhs, const Field<T>& rhs);

template <typename T>
bool operator<(const Field<T>& lhs, const Field<T>& rhs);

template <typename T>
bool operator!=(const Field<T>& lhs, const Field<T>& rhs);

template <typename T>
bool operator>(const Field<T>& lhs, const Field<T>& rhs);

template <typename T>
bool operator<=(const Field<T>& lhs, const Field<T>& rhs);

template <typename T>
bool operator>=(const Field<T>& lhs, const Field<T>& rhs);

namespace zserio
{

template <typename T>
class View<Field<T>>
{
public:
    View(const Field<T>& data) noexcept;

    T value() const;

private:
    const Field<T>& m_data;
};

template <typename T>
bool operator==(const ::zserio::View<Field<T>>& lhs, const ::zserio::View<Field<T>>& rhs);

template <typename T>
bool operator<(const ::zserio::View<Field<T>>& lhs, const ::zserio::View<Field<T>>& rhs);

template <typename T>
bool operator!=(const ::zserio::View<Field<T>>& lhs, const ::zserio::View<Field<T>>& rhs);

template <typename T>
bool operator>(const ::zserio::View<Field<T>>& lhs, const ::zserio::View<Field<T>>& rhs);

template <typename T>
bool operator<=(const ::zserio::View<Field<T>>& lhs, const ::zserio::View<Field<T>>& rhs);

template <typename T>
bool operator>=(const ::zserio::View<Field<T>>& lhs, const ::zserio::View<Field<T>>& rhs);

namespace detail
{

template <typename T>
inline void validate(const ::zserio::View<Field<T>>& view);

template <typename T>
inline void write(::zserio::BitStreamWriter& writer, const ::zserio::View<Field<T>>& view);

template <typename T>
inline ::zserio::View<Field<T>> read(::zserio::BitStreamReader& reader, Field<T>& data,
        const typename Field<T>::allocator_type& allocator = {});

template <typename T>
inline size_t bitSizeOf(const ::zserio::View<Field<T>>& view, size_t bitPosition);

} // namespace detail

} // namespace zserio

namespace std
{

template<>
struct hash<Field<T>>
{
    size_t operator()(const Field<T>& data) const;
};

template<>
struct hash<Field<T>::View>
{
    size_t operator()(const Field<T>::View& view) const;
};

} // namespace std
```

```cpp
struct StructTemplatedField
{
    using allocator_type = ::std::allocator<uint8_t>;

    StructTemplatedField() noexcept;

    explicit StructTemplatedField(const allocator_type& allocator);

    StructTemplatedField(
        Field<::zserio::UInt32> uint32Field_,
        Field<Compound> compoundField_
    );

    Field<::zserio::UInt32> uint32Field;
    Field<Compound> compoundField;
};

...
```

> Note that there is a potential risk to instantiate templates using template arguments with are not checked
  by Zserio compiler (which are not used in the Zserio schema).

### Service Types

The Service Types will be implemented in the same way as in the old C++11 generator.

### Pubsub Types

The Pubsub Types will be implemented in the same way as in the old C++11 generator.

### SQL Tables

The SQL Tables will be implemented in the same way as in the old C++11 generator.

### SQL Databases

The SQL Databases will be implemented in the same way as in the old C++11 generator.
