[![](https://github.com/ndsev/zserio/actions/workflows/build_linux.yml/badge.svg)](https://github.com/ndsev/zserio-cpp17/actions/workflows/build_linux.yml)
[![](https://github.com/ndsev/zserio/actions/workflows/build_windows.yml/badge.svg)](https://github.com/ndsev/zserio-cpp17/actions/workflows/build_windows.yml)
[![runtime - doc](https://img.shields.io/badge/runtime-doc-2ea44f?logo=c%2B%2B)](https://ndsev.github.io/zserio-cpp17/doc/runtime/latest)
[![](https://img.shields.io/endpoint?url=https://ndsev.github.io/zserio-cpp17/doc/runtime/latest/coverage/clang/coverage_github_badge.json)](https://ndsev.github.io/zserio-cpp17/doc/runtime/latest/coverage/clang)
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
  This new abstraction naturally solves tge implementation of Parameterized Types without the need for
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

## Current State

The [Design Document](doc/Cpp17Design.md) acts as the primary source of information about the C++17 generator
design.

The generator fully supports the Zserio language and has the following limitations:

- Templates are generated as instantiations without usage of native C++ templates
- No support for generic programming, including JSON export and import

### How to Get the latest C++17 Generator

Download the latest Zserio bundle jar (together with Zserio runtime library) from the GitHub action artifacts
using the following steps:

- Go to the [Actions](https://github.com/ndsev/zserio-cpp17/actions) page
- Click on the latest Linux workflow
- Scroll down to the Artifacts
- Download `zserio-java8` artifact for Zserio bundle jar
- Alternatively, download `zserio-runtime-cpp` artifact for Zserio runtime library

### How to Run the latest C++17 Generator

Run the Zserio C++17 generator using the following steps:

- Unzip `zserio-java8` to get `zserio.jar` binary
- Run the command `java -jar zserio.jar schema_name.zs -cpp17 output_directory_name`
