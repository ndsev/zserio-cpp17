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

This repository currently contains only a [Design Document](doc/Cpp17Design.md) and a [PoC](poc/)
of the generated code using a [schema sample](poc/test.zs).
