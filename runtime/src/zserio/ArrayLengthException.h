#ifndef ZSERIO_ARRAY_LENGTH_EXCEPTION_H_INC
#define ZSERIO_ARRAY_LENGTH_EXCEPTION_H_INC

#include "zserio/CppRuntimeException.h"

namespace zserio
{

/**
 * Exception thrown when a check of the array length fails.
 */
class ArrayLengthException : public CppRuntimeException
{
public:
    using CppRuntimeException::CppRuntimeException;
};

} // namespace zserio

#endif // ifndef ZSERIO_ARRAY_LENGTH_EXCEPTION_H_INC
