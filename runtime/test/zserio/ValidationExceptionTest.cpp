#include "gtest/gtest.h"
#include "zserio/ValidationException.h"

namespace zserio
{

TEST(ValidationExceptionTest, correctTypeAfterAppend)
{
    ASSERT_THROW(
            {
                throw ValidationException()
                        << "Test that appending using operator<< persists the exception type!";
            },
            ValidationException);
}

} // namespace zserio
