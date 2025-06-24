#include "gtest/gtest.h"
#include "templates/instantiate_vs_default/InstantiateVsDefault.h"
#include "templates/instantiate_vs_default/TStr.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_vs_default
{

TEST(InstantiateVsDefaultTest, writeRead)
{
    InstantiateVsDefault data;
    data.test32 = pkg::Test<zserio::UInt32>{13};
    data.testStr = TStr{"test"};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_vs_default
} // namespace templates
