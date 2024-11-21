#ifndef ZSERIO_ARRAY_LENGTH_EXCEPTION_H_INC
#define ZSERIO_ARRAY_LENGTH_EXCEPTION_H_INC

#include "zserio/ValidationException.h"

namespace zserio
{

/**
 * Exception thrown when a check of the array length fails.
 */
class ArrayLengthException : public ValidationException
{
public:
    using ValidationException::ValidationException;
};

} // namespace zserio

#endif // ifndef ZSERIO_ARRAY_LENGTH_EXCEPTION_H_INC
