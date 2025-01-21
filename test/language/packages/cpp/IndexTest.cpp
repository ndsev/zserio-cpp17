#include "gtest/gtest.h"
#include "index_workaround/index/Test.h"
#include "test_utils/TestUtility.h"

namespace index_workaround
{

class IndexTest : public ::testing::Test
{
protected:
    static constexpr uint32_t ARRAY_SIZE = 10;
};

TEST_F(IndexTest, writeRead)
{
    ::index_workaround::index::Test data;
    data.indexes.resize(ARRAY_SIZE);
    data.indexesForParameterized.resize(ARRAY_SIZE);
    for (uint32_t i = 0; i < ARRAY_SIZE; ++i)
    {
        data.array.emplace_back(i);
        data.parameterizedArray.emplace_back(i);
    }

    test_utils::writeReadTest(data);
}

} // namespace index_workaround
