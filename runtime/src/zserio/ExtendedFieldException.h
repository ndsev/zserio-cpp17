#ifndef ZSERIO_EXTENDED_FIELD_EXCEPTION_H_INC
#define ZSERIO_EXTENDED_FIELD_EXCEPTION_H_INC

#include "zserio/ValidationException.h"

namespace zserio
{

/**
 * Exception thrown when a check of an extended field fails.
 */
class ExtendedFieldException : public ValidationException
{
public:
    using ValidationException::ValidationException;
};

} // namespace zserio

#endif // ifndef ZSERIO_EXTENDED_FIELD_EXCEPTION_H_INC
