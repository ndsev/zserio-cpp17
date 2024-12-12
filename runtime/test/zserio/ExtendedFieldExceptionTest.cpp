#include "gtest/gtest.h"
#include "zserio/ExtendedFieldException.h"

namespace zserio
{

TEST(ExtendedFieldExceptionTest, correctTypeAfterAppend)
{
    ASSERT_THROW(
            {
                throw ExtendedFieldException()
                        << "Test that appending using operator<< persists the exception type!";
            },
            ExtendedFieldException);
}

} // namespace zserio
