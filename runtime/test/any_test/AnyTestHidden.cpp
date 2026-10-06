#include "AnyTestHidden.h"

namespace AnyTestHiddenLib
{

zserio::Any createAnyEmptyStruct()
{
    zserio::Any any;
    any.set(EmptyStruct());
    return any;
}

bool isEmptyStruct(const zserio::Any& any)
{
    return any.isType<EmptyStruct>();
}

} // namespace AnyTestHiddenLib
