#include "gtest/gtest.h"
#include "zserio/UnionCaseException.h"

namespace zserio
{

TEST(UnionCaseExceptionTest, correctTypeAfterAppend)
{
    ASSERT_THROW(
            {
                throw UnionCaseException()
                        << "Test that appending using operator<< persists the exception type!";
            },
            UnionCaseException);
}

} // namespace zserio
