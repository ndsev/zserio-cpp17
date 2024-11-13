#include "gtest/gtest.h"
#include "zserio/ChoiceCaseException.h"

namespace zserio
{

TEST(ChoiceCaseExceptionTest, correctTypeAfterAppend)
{
    ASSERT_THROW(
            {
                throw ChoiceCaseException()
                        << "Test that appending using operator<< persists the exception type!";
            },
            ChoiceCaseException);
}

} // namespace zserio
