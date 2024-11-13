#include "gtest/gtest.h"
#include "zserio/MissedOptionalException.h"

namespace zserio
{

TEST(MissedOptionalExceptionTest, correctTypeAfterAppend)
{
    ASSERT_THROW(
            {
                throw MissedOptionalException()
                        << "Test that appending using operator<< persists the exception type!";
            },
            MissedOptionalException);
}

} // namespace zserio
