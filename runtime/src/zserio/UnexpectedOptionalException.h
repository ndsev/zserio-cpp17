#ifndef ZSERIO_UNEXPECTED_OPTIONAL_EXCEPTION_H_INC
#define ZSERIO_UNEXPECTED_OPTIONAL_EXCEPTION_H_INC

#include "zserio/ValidationException.h"

namespace zserio
{

/**
 * Exception thrown when a check of the optional field fails.
 *
 * Check of the optional field fails if it is set but it should not be present according to the if-clause.
 */
class UnexpectedOptionalException : public ValidationException
{
public:
    using ValidationException::ValidationException;
};

} // namespace zserio

#endif // ifndef ZSERIO_UNEXPECTED_OPTIONAL_EXCEPTION_H_INC
