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

## Current State

The [Design Document](doc/Cpp17Design.md) acts as the primary source of information about the C++17 generator
design and is updated regularly.

Current state (15.11.2024):

- The generator fully supports the following Zserio types:
  - Enumeration types
  - Bitmask types
  - Constants
  - Structure types
  - Choice types
  - Union types
- Offsets are not supported, fatal error is reported during generation
- Extended members are not supported
- Templates are generated as instantiations without usage of native C++ templates
- Services, Pubsubs, SQL databases and SQL tables are silently ignored
- DataView abstraction is not supported

Current state of the generated sources can be investigated at PoC
[here](https://github.com/ndsev/zserio-cpp17/tree/master/poc/gen/cpp17).

### How to Get the C++17 Generator

Download the latest Zserio bundle jar (together with Zserio runtime library) from the GitHub action artifacts
using the following steps:

- Go to the [Actions](https://github.com/ndsev/zserio-cpp17/actions) page
- Click on the latest Linux workflow
- Scroll down to the Artifacts
- Download `zserio-java8` artifact for Zserio bundle jar
- Alternatively, download `zserio-runtime-cpp` artifact for Zserio runtime library

### How to Run the C++17 Generator

Run the Zserio C++17 generator using the following steps:

- Unzip `zserio-java8` to get `zserio.jar` binary
- Run the command `java -jar zserio.jar schema_name.zs -cpp17 output_directory_name`
