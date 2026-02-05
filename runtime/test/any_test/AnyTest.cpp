#include <zserio/Types.h>

#include "AnyTest.h"

namespace AnyTestLib
{

zserio::Any createAnyStdInt()
{
    zserio::Any any;
    any.set(11);
    return any;
}

zserio::Any createAnyInt32()
{
    zserio::Any any;
    any.set(zserio::Int32(11));
    return any;
}

zserio::Any createAnyString()
{
    zserio::Any any;
    any.set(zserio::String("kuk"));
    return any;
}

} // namespace AnyTestLib
