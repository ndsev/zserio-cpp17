#include "gtest/gtest.h"
#include "templates/instantiate_simple/InstantiateSimple.h"
#include "templates/instantiate_simple/U32.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_simple
{

TEST(InstantiateSimpleTest, writeRead)
{
    InstantiateSimple data;
    data.test = U32{13};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_simple
} // namespace templates
