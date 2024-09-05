#ifndef ZSERIO_OUT_OF_RANGE_EXCEPTION_H_INC
#define ZSERIO_OUT_OF_RANGE_EXCEPTION_H_INC

#include "zserio/CppRuntimeException.h"

namespace zserio
{

/**
 * Exception thrown when a value is out of range.
 */
class OutOfRangeException : public CppRuntimeException
{
public:
    using CppRuntimeException::CppRuntimeException;
};

} // namespace zserio

#endif // ifndef ZSERIO_OUT_OF_RANGE_EXCEPTION_H_INC
