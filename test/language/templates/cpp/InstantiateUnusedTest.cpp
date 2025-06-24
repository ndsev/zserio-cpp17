#include "gtest/gtest.h"
#include "templates/instantiate_unused/U32.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_unused
{

TEST(InstantiateUnusedTest, writeRead)
{
    U32 data{13}; // check that unused template is instantiated via the instantiate command

    test_utils::writeReadTest(data);
}

} // namespace instantiate_unused
} // namespace templates
