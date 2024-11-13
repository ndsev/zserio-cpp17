#ifndef ZSERIO_UNION_CASE_EXCEPTION_H_INC
#define ZSERIO_UNION_CASE_EXCEPTION_H_INC

#include "zserio/CppRuntimeException.h"

namespace zserio
{

/**
 * Exception thrown when a check of the union case fails.
 */
class UnionCaseException : public CppRuntimeException
{
public:
    using CppRuntimeException::CppRuntimeException;
};

} // namespace zserio

#endif // ifndef ZSERIO_UNION_CASE_EXCEPTION_H_INC
