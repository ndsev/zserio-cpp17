#ifndef ZSERIO_CONSTRAINT_EXCEPTION_H_INC
#define ZSERIO_CONSTRAINT_EXCEPTION_H_INC

#include "zserio/ValidationException.h"

namespace zserio
{

/**
 * Exception thrown when a constraint check fails.
 */
class ConstraintException : public ValidationException
{
public:
    using ValidationException::ValidationException;
};

} // namespace zserio

#endif // ifndef ZSERIO_CONSTRAINT_EXCEPTION_H_INC
