#include "gtest/gtest.h"
#include "zserio/OutOfRangeException.h"

namespace zserio
{

TEST(OutOfRangeExceptionTest, correctTypeAfterAppend)
{
    ASSERT_THROW(
            {
                throw OutOfRangeException()
                        << "Test that appending using operator<< persists the exception type!";
            },
            OutOfRangeException);
}

} // namespace zserio
