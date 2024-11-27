#include <array>
#include <vector>

#include "array_types/packed_auto_array_empty_compounds/PackedAutoArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace array_types
{
namespace packed_auto_array_empty_compounds
{

class PackedAutoArrayEmptyCompoundsTest : public ::testing::Test
{
protected:
    PackedAutoArray createData()
    {
        PackedAutoArray data({{EmptyStruct(), EmptyStruct(), EmptyStruct()}},
                {{EmptyUnion(), EmptyUnion(), EmptyUnion()}}, {{EmptyChoice(), EmptyChoice(), EmptyChoice()}},
                {{Main(), Main(), Main()}});

        data.mainArray.at(0).param = 0;
        data.mainArray.at(1).param = 1;
        data.mainArray.at(2).param = 2;
        return data;
    }

    static constexpr std::string_view BLOB_NAME = "language/array_types/packed_auto_array_empty_compounds.blob";
};

TEST_F(PackedAutoArrayEmptyCompoundsTest, writeReadFile)
{
    auto data = createData();
    test_utils::writeReadFileTest(BLOB_NAME, data);
}

} // namespace packed_auto_array_empty_compounds
} // namespace array_types
