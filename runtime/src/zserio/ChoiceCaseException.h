#ifndef ZSERIO_CHOICE_CASE_EXCEPTION_H_INC
#define ZSERIO_CHOICE_CASE_EXCEPTION_H_INC

#include "zserio/ValidationException.h"

namespace zserio
{

/**
 * Exception thrown when a check of the choice case fails.
 */
class ChoiceCaseException : public ValidationException
{
public:
    using ValidationException::ValidationException;
};

} // namespace zserio

#endif // ifndef ZSERIO_CHOICE_CASE_EXCEPTION_H_INC
