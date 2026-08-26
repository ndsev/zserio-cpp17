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

[Type Information](#type-information)

[Reflections](#reflections)

[Introspections](#introspections)

[Parsing Info](#parsing-info)

### Motivation

The user experience of the original C++ generator brought new ideas and enhancements which could improve
usability of the generated code. Besides of that as time flies, users asked for support of new modern
C++17 standard.

However, such changes in generated code would break backward compatibility of user applications, which is
undesirable. Therefore, we implemented a brand new C++17 generator with incompatible API but still allowing
the use the old one.

The main aim of this document is to describe the design of the new C++17 generator in detail.

The following two main features had been reenvisioned:

- Implementation of the Parameterized Types

  New C++17 generator models Zserio Structures, Choices and Unions by the new Data View abstraction.
  This new abstraction naturally solves implementation of the Parameterized Types without a need of
  two-phase initialization combined by custom copy and move constructors (for more information please
  see [Data View Approach](#data-view-approach) section).

- Implementation of the Templates

  New C++17 generator models Zserio templates by C++ native templates. Because there is a need to
  distinguish which Zserio native type is used as a template argument, this solution involves
  implementation of dedicated C++ Zserio types for all Zserio built-in types in the C++ runtime library
  (for more information please see [Built-in Types](#built-in-types) section).

### Built-in Types

Zserio built-in types are implemented in C++ runtime library as dedicated C++ Zserio type classes.
These classes provide

- implicit constructor from the C++ native type and
- implicit conversion to the C++ native type.

In this way, Zserio types behave very similarly to native built-in types, which is important, for example,
to keep integral expressions as simple as possible.

> Note that the implicit constructors from the C++ native type break a MISRA 2023 rule 15.1.3
  (Conversion operators and constructors that are callable with a single argument shall be explicit).

C++ Zserio type classes specialize `zserio::NumericLimits<>` template to get minimum and maximum
values.

The `zserio::toCheckedValue` function allows to obtain C++ native type value, automatically performing
range checks to ensure the value falls within the expected range.

Similarly it's possible to construct C++ Zserio type classes from C++ native type value by calling
`zserio::fromCheckedValue` function which automatically checks range check of the value.

The following table shows the mapping between all Zserio built-in types and the C++ Zserio type together with
the C++ native type:

Zserio Type     | C++ Zserio Type                     | C++ Native Type         |
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
varsize         | `zserio::VarSize`                   | `uint32_t`              |
bool            | `zserio::Bool`                      | `bool`                  |
string          | `zserio::String`                    | `std::string`           |
extern          | `zserio::BitBuffer`                 | N/A                     |
bytes           | `zserio::Bytes`                     | `std::vector<uint8_t>`  |


### Constants

The Constants are implemented using `constexpr` keyword instead of `const` keyword. This is possible
because Zserio constants are initialized from expressions which contain only another constants, literals
and enumeration or bitmask values (using `valueof` operator).

### Enumeration Types

The Enumeration Types are implemented in the same way as in the old C++11 generator.

This means that the Enumeration Type is modeled by the C++11 enumeration together with the specialization
of the enumeration traits and methods implemented in the C++ runtime library.

### Bitmask Types

The Bitmask Types are implemented in the same way as in the old C++11 generator.

This means that the Bitmask Type is modeled by the custom class which contains an inner enumeration
of the bitmask values. All bitmask operators (`==`, `!=`, `<`, `|`, `&`, `^`, `~`, `|=`, `&=`, `^=`)
are implemented as non-member functions.

> Note that the generated Bitmask Type class implements default constructor from the bitmask value. This is
  intended and it breaks a MISRA 2023 rule 15.1.3 (Conversion operators and constructors that are callable
  with a single argument shall be explicit).

### Compound Types

The Compound Types are implemented using new Data-View approach which naturally solves implementation
of the Parameterized Types.

#### Data View Approach

The solution separates data (`Data`) from the schema logic (`View`).

The main idea is that all the parameters are already present somewhere in the BLOB structure. For just keeping
such a BLOB data (`Data`), it's not needed to store additional auxiliary data in memory
(i.e. references to parameters) therefore `Data` is only a simple data class having all fields publicly
accessible. Parameters can be calculated on the fly once user needs to work with the BLOB - just by
constructing the `View` which knows how and where to get the parameters. The View holds a pointer to `Data`
class and it accesses `Data` fields as needed.

The `View` by itself is immutable and provides only getters together with functions defined in the schema.
There is no way to modify underlying data through `View` interface.

Secondly the `View` contains simple parameters stored by value and a reference to the underlying data.
Users are responsible to keep the underlying data alive while working with the `View`. `View` can become
invalid once the underlying data are changed, which is similar to how the `span`, `string_view` or e.g.
`vector::iterator` behave (reallocation of the underlying data).

Once working with the `View`, the getters also returns `View`s and thus parameters are always available.

Zserio features like `write`, `read`, `bitSizeOf` are implemented as static member functions inside global
`ObjectTraits<Data>` specialization in the `detail` namespace to emphasize that this interface is not part
of the user API. These functions are called by the global public functions
`::zserio::serialize`, `::zserio::deserialize` and `::zserio::bitSizeOf`.

`View` is implemented as a templated class `View<Data>` instead of an inner class. This is
because native templates like `Data<T>::View` cause many problems. For example, compiler cannot deduce
such a View, so all function calls require explicit type specification, like `operator==<T>(view1, view2)`.
This also prevents storing Views in `std::unordered/map` without an extra comparator.
Direct specialization of `View<Data>` fixes these issues. Nevertheless for convenience `Data` class contains
type alias `View` so one can use familiar syntax for Views as an alternative.

To support STL allocator machinery `allocator_type` alias and constructor variants with allocator argument
are generated as well. That in turn forces generation of other members such as assignment operators
and a destructor.

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

the C++ generated code looks like the following:

```cpp
struct Param
{
    using View = ::zserio::View<Param>;
    using allocator_type = ::std::allocator<uint8_t>;

    Param() noexcept;

    // more constructors, assignment operators and a destructor...

    Param(
        ::zserio::UInt16 value_,
        ::zserio::Optional<::zserio::UInt32> extraValue_,
        const allocator_type& allocator = {});

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
    explicit View(
        const Param& data,
        ::zserio::UInt16 parameter_) noexcept;

    ::zserio::UInt16 parameter() const;

    ::zserio::UInt16 value() const;
    ::zserio::Optional<::zserio::UInt32> extraValue() const;

    const Param& zserioData() const;

protected:
    View(const Param& data, const View& other) noexcept;

private:
    const Param* m_data;
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

template <>
struct ObjectTraits<Param>
{
    using Parameters = std::tuple<
            ::zserio::UInt16>;

    static void validate(const View<Param>& view, ::std::string_view fieldName);
    static BitSize bitSizeOf(const View<Param>& view, BitSize bitPosition);
    static void write(BitStreamWriter& writer, const View<Param>& view);
    static View<Param> read(BitStreamReader& reader, Param& data,
            ::zserio::UInt16 parameter_);
};

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
    using View = ::zserio::View<ParameterizedParamHolder>;
    using allocator_type = ::std::allocator<uint8_t>;

    ParameterizedParamHolder() noexcept;
    explicit ParameterizedParamHolder(const allocator_type& allocator);

    // more constructors, assignment operators and destructor...

    ParameterizedParamHolder(
        ::zserio::UInt16 parameter_,
        Param param_
        const allocator_type& allocator = {});

    ::zserio::UInt16 parameter;
    Param param;
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

    ::zserio::UInt16 parameter() const;
    View<Param> param() const;

    const ParameterizedParamHolder& zserioData() const;

protected:
    View(const ParameterizedParamHolder& data, const View& other) noexcept;

private:
    const ParameterizedParamHolder* m_data;
};

bool operator==(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);
bool operator<(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);
bool operator!=(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);
bool operator>(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);
bool operator<=(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);
bool operator>=(const View<ParameterizedParamHolder>& lhs, const View<ParameterizedParamHolder>& rhs);

namespace detail
{

template <>
struct ObjectTraits<ParameterizedParamHolder>
{
    static void validate(const View<ParameterizedParamHolder>& view, ::std::string_view fieldName);

    static BitSize bitSizeOf(const View<ParameterizedParamHolder>& view, BitSize bitPosition);

    static void write(BitStreamWriter& writer, const View<ParameterizedParamHolder>& view);

    static View<ParameterizedParamHolder> read(BitStreamReader& reader, ParameterizedParamHolder& data);
};

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
const auto holderView = ::zserio::deserializeFromFile("holder.blob", holderData);
// parameter is stored in the view so it doesn't need to be provided
::std::cout << holderView.param().parameter() << ::std::endl;
```

#### Choice And Union Types

The Choice Types use a dedicated abstraction `::zserio::Variant` with the similar interface as
`std::variant` from C++17 standard. The main implementation difference is there is no
way to construct `::zserio::Variant` without the specification of an index (tag). The index (tag)
should be an enumeration. In case of the choice which uses enumeration, the index (tag) should use the same
enumeration as well.

Another implementation difference is `::zserio::Variant` allocates additional dynamic memory
for types bigger than a certain threshold. Variant implementation embeds `::zserio::Any` which already
implements this mechanism. Thus, allocator must be provided to `::zserio::Variant` during construction.

Such optimization is desirable to save memory for choices where one case contains significantly bigger type
than other cases. If such choice is stored in a large array, it would reserve significant amount of memory even
though the elements are set to different cases.

#### Optional Members

Optional Members use a dedicated abstraction `::zserio::Optional` with the similar interface as
`std::optional` from C++17 standard.

The only implementation difference is `::zserio::Optional` allocates additional dynamic memory
for types bigger than certain threshold and for recursive Optional Members. Thus, allocator must be provided to
`::zserio::Optional` during construction.

Such optimization is desirable to save memory for large optionals which are not present.

### Array Types

The Array Types in the Data structure use normal `std::vector` type from C++17 standard library.
Because `View` needs an immutable array wrapper there is a new dedicated `::zserio::ArrayView` abstraction
which is used. Type aliases for all `ArrayView` members are provided for convenience.

##### Example

Using the Zserio schema

```
struct ArrayHolder
{
    int8 array[];
};
```

the C++ generated code looks like the following:

```cpp
class ArrayHolder
{
struct:
    using View = ::zserio::View<ArrayHolder>;
    using allocator_type = ::std::allocator<uint8_t>;

    ArrayHolder() noexcept;
    explicit ArrayHolder(const allocator_type& allocator) noexcept;

    // more constructors, assignment operators and a destructor...

    explicit ArrayHolder(
        ::zserio::vector<::zserio::Int8> array_,
        const allocator_type& allocator = {});

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
    using ArrayType = ArrayView<const ::zserio::Int8>;

    explicit View(const ArrayHolder& data) noexcept;

    ArrayType array() const;
    const ArrayHolder& zserioData() const;

protected:
    View(const ArrayHolder& data, const View& other) noexcept;

private:
    const ArrayHolder* m_data;
};

bool operator==(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);
bool operator<(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);
bool operator!=(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);
bool operator>(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);
bool operator<=(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);
bool operator>=(const View<ArrayHolder>& lhs, const View<ArrayHolder>& rhs);

namespace detail
{

template <>
struct ObjectTraits<ArrayHolder>
{
    static void validate(const View<ArrayHolder>& view, ::std::string_view fieldName);
    static BitSize bitSizeOf(const View<ArrayHolder>& view, BitSize bitPosition);
    static void write(BitStreamWriter& writer, const View<ArrayHolder>& view);
    static View<ArrayHolder> read(BitStreamReader& reader, ArrayHolder& data);
};

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

The offsets are initialized automatically during a call of `::zserio::serialize` function by means of the
`::zserio::detail::initializeOffsets` call which in turns calls
`ObjectTraits<Data>::initializeOffsets` when exists.

To allow calling `::zserio::serialize` with `const Data&` offset itself is a defined as a `mutable` data
member inside the `Data` class so it can be internally modified even when const.

### Templates

The Templates are implemented using C++ native templates. If the template argument is a Zserio built-in type,
Zserio C++ runtime type is used instead of the C++ native type. For convenience all template parameters are
accessible through generated type aliases in both the `Data` and `View<Data>` classes.

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

the C++ generated code looks like the following (skipping member function bodies which are in case of templates
put into the header file):

```cpp
template <typename T>
struct Field
{
    using View = ::zserio::View<Field>;
    using allocator_type = ::std::allocator<uint8_t>;

    using TType = T;

    Field() noexcept :
            Field(allocator_type{})
    {}

    // more allocator aware constructors, assignment operators and a destructor...

    Field(T value_,
            const allocator_type& allocator = {}) :
            value(::zserio::constructWithAllocator<T>(allocator, ::std::move(value_)))
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
    using TType = T;

    View(const Field<T>& data) noexcept;

    view_type_t<T> value() const;
    const Field<T>& zserioData() const;

protected:
    View(const Field<T>& data, const View&) noexcept;

private:
    const Field<T>* m_data;
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
struct ObjectTraits<Field<T>>
{
    static void validate(const View<Field<T>>& view, ::std::string_view);
    static BitSize bitSizeOf(const View<Field<T>>& view, BitSize bitPosition);
    static void write(BitStreamWriter& writer, const View<Field<T>>& view);
    static View<Field<T>> read(BitStreamReader& reader, Field<T>& data);
};

} // namespace detail

} // namespace zserio

namespace std
{

template <typename T>
struct hash<Field<T>>
{
    size_t operator()(const Field<T>& data) const;
};

template <typename T>
struct hash<::zserio::View<Field<T>>>
{
    size_t operator()(const ::zserio::View<Field<T>>& view) const;
};

} // namespace std
```

```cpp
struct StructTemplatedField
{
    using View = ::zserio::View<StructTemplatedField>;
    using allocator_type = ::std::allocator<uint8_t>;

    StructTemplatedField() noexcept;

    // more constructors, assignment operators and a destructor...

    StructTemplatedField(
            Field<::zserio::UInt32> uint32Field_,
            Field<Compound> compoundField_,
            const allocator_type& allocator = {});

    Field<::zserio::UInt32> uint32Field;
    Field<Compound> compoundField;
};

...
```

> Note that there is a potential risk to instantiate templates using template arguments with are not checked
  by Zserio compiler (which are not used in the Zserio schema).

### Service Types

Services are allowed to only contain service methods. Each method must have a single response and single
request type. When no response or request is needed an empty structure can be used.

Using following example:

```
struct UserId
{
    uint32 id;
};

struct User
{
    uint32 id;
    string name;
    string surname;
    string phoneNumber;
};

service Users
{
    User getUser(UserId);
};
```

As for the `service` the following code is generated:

```cpp
namespace Users
{

class Service :
        public ::zserio::IService,
        public ::zserio::AllocatorHolder<::std::allocator<uint8_t>>
{
public:
    explicit Service(const allocator_type& allocator = {});

    // disabled copy constructor and assignment...
    // enabled move constructor and move assignment...

    ::zserio::IServiceDataPtr callMethod(
            ::std::string_view methodName, ::zserio::Span<const uint8_t> requestData,
            void* context) override;

    static ::std::string_view serviceFullName() noexcept;

    static const ::std::array<::std::string_view, 1>& methodNames() noexcept;

private:
    virtual User getUserImpl(const UserId& request, void* context) = 0;

    ::zserio::IServiceDataPtr getUserMethod(
            ::zserio::Span<const uint8_t> requestData, void* context);
};

class Client : public ::zserio::AllocatorHolder<::std::allocator<uint8_t>>
{
public:
    explicit Client(::zserio::IServiceClient& service, const allocator_type& allocator = {});

    // disabled copy constructor, assignment and move assignment...
    // enabled move constructor

    User getUserMethod(const UserId& request, void* context = nullptr);

private:
    ::zserio::IServiceClient& m_service;
};

} // namespace Users
```

In this example:

- Two separate classes `Service` and `Client` are generated inside the namespace formed from the original
  service name.
- The service name is available under the `serviceFullName` call.
- All method names can be enumerated with the `methodNames` call.
- The `Service` class should be inherited in the user code and the implementation of each method should be
  given. This would typically include sending packets over network.
- The `Client` class is lightweight and only forwards all calls to its service instance.

### Pubsub Types

This generates a client supporting the pub/sub pattern.

Example:

```
pubsub WeatherProvider
{
    publish topic("weather/warnings") WeatherWarning warnings;
};

pubsub WeatherClient
{
    subscribe topic("weather/warnings") WeatherWarning weatherWarnings;
};

struct WeatherWarning
{
    string warningMessage;
};
```

Zserio generates following code:

```cpp
class WeatherProvider : public ::zserio::AllocatorHolder<::std::allocator<uint8_t>>
{
public:
    explicit WeatherProvider(::zserio::IPubsub& pubsub, const allocator_type& allocator = {});

    // disabled copy constructor, assignment and move assignment...
    // enabled move constructor...

    void publishWarnings(const WeatherWarning& message, void* context = nullptr);

private:
    template <typename ZSERIO_MESSAGE>
    void publish(ZSERIO_MESSAGE& message, ::std::string_view topic, void* context);

    ::zserio::IPubsub& m_pubsub;
};
```

```cpp
class WeatherClient : public ::zserio::AllocatorHolder<::std::allocator<uint8_t>>
{
public:
    explicit WeatherClient(::zserio::IPubsub& pubsub, const allocator_type& allocator = {});

    // disabled copy constructor, assignment and move assignment...
    // enabled move constructor...

    template <typename ZSERIO_MESSAGE>
    class WeatherClientCallback
    {
    public:
        virtual ~WeatherClientCallback() = default;

        virtual void operator()(::std::string_view topic, const ZSERIO_MESSAGE& message) = 0;
    };

    ::zserio::IPubsub::SubscriptionId subscribeWeatherWarnings(
            const ::std::shared_ptr<WeatherClientCallback<WeatherWarning>>& callback,
            void* context = nullptr);

    void unsubscribe(::zserio::IPubsub::SubscriptionId id);

private:
    ::zserio::IPubsub& m_pubsub;
};
```

The pubsub defines messages which can be either published or subscribed (or both). The example above defines
a `WeatherProvider` pubsub type which defines a single message warnings, which is published under the topic
named "weather/warnings" and the type of the message is `WeatherWarnings` structure. Then it defines
a `WeatherClient` pubsub type which has a single subscription weatherWarnings for messages published under
topic "weather/warnings". The client expects that the type of messages arriving to `weatherWarnings`
subscription (i.e. published under the defined topic) are of the type `WeatherWarning`.

Pubsub type can define a message in three ways:

- `topic("topic/definition") Type message` to both publish and subscribe a message,
- `publish topic("topic/definition") Type message` to publish a message,
- `subscribe topic("topic/definition") Type message` to subscribe a message.

#### Topic Definition

In the Pub/Sub pattern, it is common to use wildcards for topic definitions in subscriptions.
The wildcards format depends on a particular implementation. Zserio only provides a generic definition
of Pub/Sub clients and doesn't manipulate with the topic definition string. It therefore depends on the
particular Pub/Sub backend whether the wildcards are supported and how.
See the [MQTT standard](https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Topic_Names_and)
as an example of a concrete Pub/Sub pattern specification.

#### Message Types

Message type must be a non-parameterized compound type or `bytes`. Parameterized types are not allowed since
the parameters are not stored in the bit stream. However parameterized types can be still used in the types'
subtree. The `bytes` type can be used when raw data format is needed - e.g. when the message is an image.

### SQL Tables

An SQL table type is a special case of a compound type, where the members of the type correspond to the
columns of a relational table.

Following data type mapping is used when generating SQL CREATE TABLE code:

Zserio type | SQLite type
--- | ---
uint8, uint16, uint32, uint64 | INTEGER
int8, int16, int32, int64 | INTEGER
bit:n (n < 64) | INTEGER
int:n (n <= 64) | INTEGER
float16, float32, float64 | REAL
varuint16, varuint32, varuint64, varuint | INTEGER
varint16, varint32, varint64, varint | INTEGER
bool | INTEGER
string | TEXT
enum | INTEGER
bitmask | INTEGER
struct | BLOB
choice | BLOB
union | BLOB

Then using the following schema:

```
struct Tile
{
    uint8   version;
    uint8   data;
};

sql_table GeoMap
{
    int32   tileId sql "PRIMARY KEY NOT NULL";
    Tile    tile sql "NOT NULL";
};
```

Zserio generates following C++ code:

```cpp
class GeoMap : public ::zserio::AllocatorHolder<::std::allocator<uint8_t>>
{
public:
    struct Row
    {
        using View = ::zserio::View<Row>;

        Row();
        explicit Row(const allocator_type& allocator);

        ::zserio::Optional<::zserio::Int32> tileId;
        ::zserio::Optional<Tile> tile;
    };

    class Reader : public ::zserio::AllocatorHolder<::std::allocator<uint8_t>>
    {
    public:
        // disabled copy constructor and assignment...
        // enabled move constructor and assignment...

        bool hasNext() const noexcept;

        ::zserio::View<Row> next(Row& row);

    private:
        // ...

        ::std::array<bool, 2> m_columnsMapping;
        ::std::unique_ptr<sqlite3_stmt, ::zserio::SqliteFinalizer> m_stmt;
        int m_lastResult;
    };

    GeoMap(::zserio::SqliteConnection& db, ::std::string_view tableName,
            ::std::string_view attachedDbName = {},
            const allocator_type& allocator = {});

    GeoMap(::zserio::SqliteConnection& db, ::std::string_view tableName,
            const allocator_type& allocator);

    // disabled copy constructor and assignment...
    // disabled move constructor and assignment...

    void createTable();
    void deleteTable();

    Reader createReader(::std::string_view condition = {}) const;
    Reader createReader(::zserio::Span<const ::zserio::String> columns,
            ::std::string_view condition = {}) const;

    void write(::zserio::Span<Row> rows, ::zserio::Span<const ::zserio::String> columns = {});
    void update(Row& row, ::std::string_view whereCondition);
    void update(Row& row,
             ::zserio::Span<const ::zserio::String> columns, ::std::string_view whereCondition);

    bool validate(::zserio::IValidationObserver& validationObserver, bool& continueValidation);

    static constexpr ::std::array<::std::string_view, 2> columnNames =
    {{
        "tileId",
        "tile"
    }};

private:
    // ...

    ::zserio::SqliteConnection& m_db;
    ::std::string_view m_name;
    ::std::string_view m_attachedDbName;
};

namespace zserio
{

template<>
class View<GeoMap::Row>
{
public:
    explicit View(const GeoMap::Row& row);

    ::zserio::Optional<::zserio::Int32> tileId();
    ::zserio::Optional<View<Tile>> tile();

    const GeoMap::Row& zserioData() const;

private:
    const GeoMap::Row* m_row;
};

} // namespace zserio
```

In this example

- `GeoMap::Row` is an equivalent of the Data class with direct field access. There is
   also `zserio::View<GeomMap::Row>` with getter-like access.
- `GeoMap` still declares all fields as NULL-able (using `zserio::Optional`). The "NOT NULL" hint in the
   source Zserio file is not parsed it is only used directly when `createTable` is called.
-  `GeoMap::Reader` is an iterator class which iterates query result set.
-  `GeoMap` supports basic SQL operations like `createTable` (CREATE TABLE), `deleteTable` (DROP TABLE),
   `write` (INSERT), `update`(UPDATE) and `createReader` (SELECT).

#### Virtual Tables

It is possible to generate code for SQLite virtual tables using following syntax:

```
sql_table Pages using fts5
{
    string title;
    string body;
};
```

The generated code is the same as in the previous example except it uses `USING fts5` clause in `createTable`.

#### Explicit Parameters

Tables can be constructed with explicit arguments using following syntax:

```
struct Tile(uint8 level, uint8 width)
{
    ...
};

sql_table GeoMap
{
    uint32 tileId;
    uint32 version;
    Tile(explicit level, explicit width) tile;
};
```

> Note the `explicit` keyword is required here so Zserio doesn't complain about missing `level`,
  `width` fields.

This can be useful when f.e. table functions rely on external values not stored in the table.

In this example

- `Tile` structure generates `View<Tile>` specialization with parameters `level`, `width` set from outside
  as usual for parameterized types.
- `GeoMap` now defines additional inner class `IParameterProvider` with following syntax:

   ```cpp
   class IParameterProvider
   {
   public:
        virtual ~IParameterProvider() = default;

        virtual ::zserio::UInt8 level(const ::zserio::View<Row>& currentRow) = 0;
        virtual ::zserio::UInt8 width(const ::zserio::View<Row>& currentRow) = 0;
   };
   ```

* `IParameterProvider` is expected to be derived in the user code and the instance is then passed into
  relevant table operations `createReader`, `write`, `update`.

#### SQLite WITHOUT ROWID Tables

To support the WITHOUT ROWID optimization in SQLite, place `sql_without_rowid` keyword after all table fields:

```
sql_table WithoutRowIdTable
{
    string  word sql "PRIMARY KEY NOT NULL";
    uint32  count;

    sql_without_rowid;
};
```

This will generate `createTable` code with corresponding WITHOUT ROWID clause.

### SQL Databases

To instantiate `sql_tables` one must place them inside the `sql_database` structure as done below.

```
sql_database Markets
{
    GeoMap europe;
    GeoMap america;
};
```

Zserio generates following C++ code:

```cpp
class Markets : public ::zserio::ISqliteDatabase, public ::zserio::AllocatorHolder<::std::allocator<uint8_t>>
{
public:
    using TRelocationMap = ::std::map<::zserio::String, ::zserio::String>;

    explicit Markets(const ::zserio::String& dbFileName,
            const TRelocationMap& tableToDbFileNameRelocationMap = {},
            const allocator_type& allocator = {});

    explicit Markets(const ::zserio::String& dbFileName, const allocator_type& allocator);

    explicit Markets(sqlite3* externalConnection,
            const TRelocationMap& tableToAttachedDbNameRelocationMap = TRelocationMap(),
            const allocator_type& allocator = {});

    explicit Markets(sqlite3* externalConnection, const allocator_type& allocator);

    // disabled copy constructor and assignment...
    // disabled move constructor and assignment...

    :zserio::SqliteConnection& connection() noexcept override;

    GeoMap& getEurope() noexcept;
    GeoMap& getAmerica() noexcept;

    void createSchema() override;
    void createSchema(const ::std::set<::zserio::String>& withoutRowIdTableNamesBlackList);
    void deleteSchema() override;

    void validate(::zserio::IValidationObserver& validationObserver);

    static constexpr::std::string_view databaseName = "Markets";
    static constexpr ::std::array<::std::string_view, 2> tableNames =
    {{
        "europe",
        "america"
    }};

private:
    // ...

    ::zserio::SqliteConnection m_db;
    ::zserio::Vector<::zserio::String> m_attachedDbList;
    TRelocationMap m_tableToAttachedDbNameRelocationMap;

    ::std::shared_ptr<GeoMap> m_europe;
    ::std::shared_ptr<GeoMap> m_america;
};
```

In this example

- Generated `Markets` class is neither Data or View as it contains getters which allow modifying operations.
- Constructors may either open new database connection to given database file or to reuse existing connection.
- Constructors may use rellocation table feature. This allows to specify tables which physically reside in
  different SQL databases. Generated code contains ATTACH DATABASE commands to make them available.
- Provides basic operations `createSchema`, `deleteSchema`, `validate` which may start a transaction and
  execute the operation over all tables included.
- Provides database name and table names as static constexprs.

### Type Information

The type information includes all static information of a Zserio type, which is available in the Zserio schema
(e.g., schema name, whether a field is optional, whether a field is an array, etc.).

The type information is not generated by default. It is generated only when using the command line option
`-withTypeInfoCode`.

The type information can be obtained by calling the following template method:

```
const ITypeInfo& typeInfo = zserio::typeInfo<ZSERIO_OBJECT>();
```

### Reflections

The reflections allow accessing fields of the `Data` abstraction in a generic way.

The reflections are not generated by default. It is generated only by using the command line option
`-withTypeInfoCode`.

The reflection record can be obtained by calling the following template method:

```
zserio::IReflectableDataPtr reflectablePtr = zserio::reflectable(zserioObject);
```

The reflections are generated for the following Zserio types:

- structures
- choices
- unions
- bitmasks
- enums

Because reflections manipulate with the `Data` abstraction only, they do not have access to the Zserio
parameters or functions and do not allow serialization.

### Introspections

The introspections allow read-only access to the `View` abstraction in a generic way.

The introspections are not generated by default. It is generated only by using the command line option
`-withTypeInfoCode`.

The introspection record can be obtained by calling the following template method:

```
zserio::IIntrospectableViewConstPtr introspectablePtr = zserio::introspectable(zserioObjectView);
```

The introspections are generated for the following Zserio types:

- structures
- choices
- unions
- bitmasks
- enums

The introspections do have access to the Zserio parameters and functions and do allow serialization.

### Parsing Info

When a command line option `-withParsingInfoCode` is given, Zserio generates additional
`private` `m_parsingInfo` member inside each `Data` class. `zserio::ParsingInfo` structure contains
`bitPosition` and `bitSize` members which contain information about the position in the input stream
and it is filled during deserialization.

Call `zserio::parsingInfo(const View<Data>&)` to access `ParsingInfo` record. Please be aware that
modifying data won't adjust parsingInfo records. These are only filled during deserialization.