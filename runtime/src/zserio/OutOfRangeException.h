#ifndef ZSERIO_OUT_OF_RANGE_EXCEPTION_H_INC
#define ZSERIO_OUT_OF_RANGE_EXCEPTION_H_INC

#include "zserio/ValidationException.h"

namespace zserio
{

/**
 * Exception thrown when a value is out of range.
 */
class OutOfRangeException : public ValidationException
{
public:
    using ValidationException::ValidationException;
};

} // namespace zserio

#endif // ifndef ZSERIO_OUT_OF_RANGE_EXCEPTION_H_INC
