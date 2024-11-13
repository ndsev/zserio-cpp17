#ifndef ZSERIO_CHOICE_CASE_EXCEPTION_H_INC
#define ZSERIO_CHOICE_CASE_EXCEPTION_H_INC

#include "zserio/CppRuntimeException.h"

namespace zserio
{

/**
 * Exception thrown when a check of the choice case fails.
 */
class ChoiceCaseException : public CppRuntimeException
{
public:
    using CppRuntimeException::CppRuntimeException;
};

} // namespace zserio

#endif // ifndef ZSERIO_CHOICE_CASE_EXCEPTION_H_INC
