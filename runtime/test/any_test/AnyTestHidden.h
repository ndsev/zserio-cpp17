#pragma once

#include <typeinfo>
#include <zserio/Any.h>

namespace AnyTestHiddenLib
{

// Empty structure without a key function, so its RTTI is emitted in every translation unit which uses it.
// Generated zserio code uses the same pattern (e.g. empty structures used as flags in choices).
struct EmptyStruct
{};

zserio::Any createAnyEmptyStruct();

bool isEmptyStruct(const zserio::Any& any);

// Holds a structure named AnonymousStruct from an anonymous namespace in this library.
zserio::Any createAnyAnonymousStruct();

#if defined(_MSC_VER) || defined(__GXX_RTTI)
// The type_info of the structure held by createAnyAnonymousStruct().
const std::type_info& getAnonymousStructTypeInfo();
#endif

} // namespace AnyTestHiddenLib
