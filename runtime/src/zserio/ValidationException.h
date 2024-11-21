#ifndef ZSERIO_VALIDATION_EXCEPTION_H_INC
#define ZSERIO_VALIDATION_EXCEPTION_H_INC

#include "zserio/CppRuntimeException.h"

namespace zserio
{

/**
 * Exception thrown when a validation fails.
 */
class ValidationException : public CppRuntimeException
{
public:
    using CppRuntimeException::CppRuntimeException;
};

} // namespace zserio

#endif // ifndef ZSERIO_VALIDATION_EXCEPTION_H_INC
