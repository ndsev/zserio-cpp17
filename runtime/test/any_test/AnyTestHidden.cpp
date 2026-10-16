#include "AnyTestHidden.h"

namespace AnyTestHiddenLib
{

namespace
{

struct AnonymousStruct
{
    int value = 42;
};

} // namespace

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

zserio::Any createAnyAnonymousStruct()
{
    zserio::Any any;
    any.set(AnonymousStruct());
    return any;
}

} // namespace AnyTestHiddenLib
