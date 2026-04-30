[![](https://github.com/ndsev/zserio-cpp17/actions/workflows/build_linux.yml/badge.svg)](https://github.com/ndsev/zserio-cpp17/actions/workflows/build_linux.yml)
[![](https://github.com/ndsev/zserio-cpp17/actions/workflows/build_windows.yml/badge.svg)](https://github.com/ndsev/zserio-cpp17/actions/workflows/build_windows.yml)
[![runtime - doc](https://img.shields.io/badge/runtime-doc-2ea44f?logo=c%2B%2B)](https://ndsev.github.io/zserio-cpp17/doc/runtime/latest)
[![](https://img.shields.io/endpoint?url=https://ndsev.github.io/zserio-cpp17/doc/runtime/latest/coverage/clang-18/coverage_github_badge.json)](https://ndsev.github.io/zserio-cpp17/doc/runtime/latest/coverage/clang-18)
[![](https://sonarcloud.io/api/project_badges/measure?project=ndsev_zserio-cpp17&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=ndsev_zserio-cpp17)

# C++17 Extension

This extension implements a C++17 generator for [Zserio](https://zserio.org).

## Motivation

[Zserio](https://zserio.org) already provides a [C++ generator](https://zserio.org/compiler/extensions/cpp/)
which uses the C++11 standard.

The user experience with the current [C++ generator](https://zserio.org/compiler/extensions/cpp/) has led
to new ideas and enhancements that could improve the usability of the generated code. Additionally,
as time flies, users have requested support for the more modern C++17 standard. However, such changes
to the generated code would break backward compatibility with existing user applications, which is undesirable.
Therefore, this brand new C++17 generator with an incompatible API has been developed.

## Advantages

The following two main features of a C++17 generator offer significant advantages over the current
[C++ generator](https://zserio.org/compiler/extensions/cpp/):

- Implementation of the Parameterized Types

  New C++17 generator implements Zserio Structures, Choices, and Unions using a new Data View abstraction.
  This new abstraction naturally solves the implementation of Parameterized Types without the need for
  two-phase initialization or custom copy and move constructors.

- Implementation of the Templates

  New C++17 generator implements Zserio templates using C++ native templates. To distinguish which
  Zserio native type is used as a template argument (e.g. Zserio `bit:5` and `bit:7` types are stored in the
  same C++ `uint8_t` type), this approach also involves implementing dedicated C++ Zserio types for all Zserio
  built-in types within the C++ runtime library.

## Supported Platforms

Zserio C++17 generator supports the following platforms:

- 64-bit Linux
- 32-bit Linux
- 64-bit Windows

## Supported Compilers

Zserio C++17 generator supports the following C++ compilers:

- g++ 11.4.0
- clang 14.0.0
- clang 18.1.3
- MinGW 11.2.0
- MSVC 2022

Although newer C++ compilers are not tested, they should work as well as long as they are backward compatible.

## Documentation

The [Design Document](doc/Cpp17Design.md) acts as the primary source of information about the C++17 generator design.

The [Command Line Parameters](#command-line-parameters) section lists command line parameters related to the C++17 generator.

The [Generated Code Gotchas](#generated-code-gotchas) section outlines the counter-intuitive behaviors in the
generated code that might frequently trick people into making mistakes.

The [Out of Memory Allocation Errors](#out-of-memory-allocation-errors) section talks about checks for invalid
array size values.

The [How to Use the Development Build](#how-to-use-the-development-build) section outlines the procedure for
using the latest C++17 generator via the current development build.

### Command Line Parameters

```
java -jar zserio.jar
    [-cpp17 <output directory>]
    [-setCppAllocator <allocator>]
    [-setTopLevelPackage <package>]
    [-src <source directory>]
    [-withSourcesAmalgamation|-withoutSourcesAmalgamation]
    [-withTypeInfoCode|-withoutTypeInfoCode]
    <input file>
```

**`-cpp17`**

Zserio will generate C++17 API into a given output directory.

**`-setCppAllocator`**

Sets the C++ allocator type to be used in generated code. Possible values: `std` (default), `pmr`, `ppmr`.

`std` stands for `std::allocator` class implemented in standard C++ library.

`pmr` stands for `std::pmr::polymorphic_allocator` class implemented in standard C++ library.

`ppmr` stands for `zserio::pmr::PropagatingPolymorphicAllocator` class implemented in
Zserio C++ runtime library.

**`-setTopLevelPackage`**

Sets the top level package for generated Java sources and top level namespace for generated C++ sources.

> Parameter `-setTopLevelPackage appl.Zserio` forces all generated Java sources to be in the package
> `appl.Zserio` and all generated C++ sources to be in the namespace `appl::Zserio`.

**`-src`**

Defines the root directory for the input file and all imported packages. If this option is missing, the default
value is the current working directory. Currently, only one source directory can be specified. A list of
directories as in the Java `CLASSPATH` is not supported.

> If the source path is `C:\zserio` and the input file is `com\acme\foo.zs`, Zserio will try parsing
> `C:\zserio\com\acme\foo.zs`. If `foo.zs` contains the declaration `import com.acme.bar.*`, Zserio will
> try parsing `C:\zserio\com\acme\bar.zs`.

**`-withSourcesAmalgamation|-withoutSourcesAmalgamation`**

Enables/disables amalgamation of generated C++ sources. By default the code for each zserio object is placed in one header and one source file. When amalgamation is enabled,
C++ sources will be automatically amalgamated to speed up C++ compilation time. C++ sources generated in
different subdirectories will be amalgamated separately. Thus, if amalgamation is enabled, each generated
subdirectory will contain only one C++ source module.

**`-withTypeInfoCode|-withoutTypeInfoCode`**

Enables/disables generation of type information code. By default is disabled.

### Generated Code Gotchas

#### View Class Declaration

As written in the [Design Document](doc/Cpp17Design.md), C++17 separates generated classes into `Data` and
`View` parts.

Data Class:

* Contains only fields (the raw data).
* Not lightweight (owns the data); typically passed by reference.
* Named directly after the Zserio entity.

View Class:

* Read-only and lightweight; designed to be passed by value.
* Stores a const reference to the `Data` class, accessible via zserioData().
* Contains getters, inner helper classes, and convenient type aliases.
* Acts as a specialization of the `zserio::View<T>` class; for convenience, it can also be accessed
  via the `Data::View` alias.

The I/O methods are put in the `ObjectTraits` specialization. Both classes implement comparison / relational
operators and specialize `std::hash`.

In most cases accessing the View through `zserio::View<Data>` or `Data::View` is equivalent. But in case
you write a generic code with `Data` class template deduction prefer to use `zserio::View` specialization.

Example:

```c++
// preferred way
template <typename Data>
void printNameWithDeduction(zserio::View<Data> view)
{
    std::cout << view.name() << std::endl;
}

// alternate way
template <typename Data>
void printNameWithoutDeduction(typename Data::View view)
{
    std::cout << view.name() << std::endl;
}

// now according to C++ rules one can call printName as below
MyGeneratedStruct data;
MyGeneratedStruct::View view(data);

printNameWithDeduction(view); // template parameter deduction works
printNameWithoutDeduction<MyGeneratedStruct>(view); // needs to spell the type to work
```

### Out of Memory Allocation Errors

Zserio tries to prevent out of memory allocation errors which could happen when stored array sizes contain
invalid values. For array elements with fixed bitsize the array size is compared against remaining buffer
bit size. Then for all kinds of arrays the initial array allocations is checked against maximum allowed value
`maxArrayPrealloc`. This value can be configured in the
[`zserio::BitStreamReader`](https://ndsev.github.io/zserio-cpp17/doc/runtime/latest/classzserio_1_1BitStreamReader.html)
constructor or passed directly to
[`zserio::deserialize()`](https://ndsev.github.io/zserio-cpp17/doc/runtime/latest/SerializeUtil_8h.html) functions.

Please note that even with this check C++17 Extension cannot always guarantee data validity if the incoming stream is
corrupted. We recommend using checksums at the application level to defend against invalid data input.

> [!CAUTION]
The `maxArrayPrealloc` is set to 128KB by default to ensure protection in all cases. If stored array elements exceed
this limit the array will be reallocated but this can have an effect on the overall performance, so please don't forget
to choose a suitable limit for your application!

### How to Use the Development Build

Download the latest Zserio bundle jar (together with Zserio runtime library) from the GitHub action artifacts
using the following steps:

- Go to the [Actions](https://github.com/ndsev/zserio-cpp17/actions) page
- Click on the latest Linux workflow
- Scroll down to the Artifacts
- Download `zserio-java8` artifact for Zserio bundle jar
- Alternatively, download `zserio-runtime-cpp` artifact for Zserio runtime library

Run the Zserio C++17 generator using the following steps:

- Unzip `zserio-java8` to get `zserio.jar` binary
- Run the command `java -jar zserio.jar schema_name.zs -cpp17 output_directory_name`
