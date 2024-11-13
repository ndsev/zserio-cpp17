#include "gtest/gtest.h"
#include "zserio/ArrayLengthException.h"

namespace zserio
{

TEST(ArrayLengthExceptionTest, correctTypeAfterAppend)
{
    ASSERT_THROW(
            {
                throw ArrayLengthException()
                        << "Test that appending using operator<< persists the exception type!";
            },
            ArrayLengthException);
}

} // namespace zserio
