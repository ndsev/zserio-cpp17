#include "gtest/gtest.h"
#include "reader/Test.h"
#include "test_utils/WriteReadTest.h"

namespace reader
{

class ReaderTest : public ::testing::Test
{
protected:
    static constexpr uint32_t ARRAY_SIZE = 10;
};

TEST_F(ReaderTest, writeRead)
{
    ::reader::Test data;
    data.indexes.resize(ARRAY_SIZE);
    data.indexesForParameterized.resize(ARRAY_SIZE);
    for (uint32_t i = 0; i < ARRAY_SIZE; ++i)
    {
        data.array.emplace_back(i);
        data.parameterizedArray.emplace_back(i);
    }

    test_utils::writeReadTest(data);
}

} // namespace reader
