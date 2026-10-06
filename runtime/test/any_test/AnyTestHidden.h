#pragma once

#include <zserio/Any.h>

namespace AnyTestHiddenLib
{

// Empty structure without a key function, so its RTTI is emitted in every translation unit which uses it.
// Generated zserio code uses the same pattern (e.g. empty structures used as flags in choices).
struct EmptyStruct
{};

zserio::Any createAnyEmptyStruct();

bool isEmptyStruct(const zserio::Any& any);

} // namespace AnyTestHiddenLib
