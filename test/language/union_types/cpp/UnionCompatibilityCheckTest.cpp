#include <string_view>

#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "union_types/union_compatibility_check/UnionCompatibilityCheckVersion1.h"
#include "union_types/union_compatibility_check/UnionCompatibilityCheckVersion2.h"

namespace union_types
{
namespace union_compatibility_check
{

using AllocatorType = UnionCompatibilityCheckVersion1::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class UnionCompatibilityCheckTest : public ::testing::Test
{
protected:
    template <typename T, typename ARRAY>
    void fillUnionCompatibilityCheck(T& data, const ARRAY& array)
    {
        data.array = array;
        data.packedArray = array;
    }

    template <typename UNION>
    UNION createUnion(uint32_t index)
    {
        UNION data;
        if (index % 2 == 0)
        {
            data.template emplace<UNION::Tag::coordXY>(CoordXY{10 * index, 20 * index});
        }
        else
        {
            data.template emplace<UNION::Tag::text>("text" + zserio::toString<AllocatorType>(index));
        }

        return data;
    }

    template <typename UNION>
    VectorType<UNION> createArrayVersion1()
    {
        return VectorType<UNION>{
                createUnion<UNION>(0), createUnion<UNION>(1), createUnion<UNION>(2), createUnion<UNION>(3)};
    }

    UnionVersion2 createUnionXYZ(uint32_t index)
    {
        UnionVersion2 data;
        data.template emplace<UnionVersion2::Tag::coordXYZ>(CoordXYZ{10 * index, 20 * index, 1.1 * index});

        return data;
    }

    VectorType<UnionVersion2> createArrayVersion2()
    {
        return VectorType<UnionVersion2>{createUnion<UnionVersion2>(0), createUnion<UnionVersion2>(1),
                createUnion<UnionVersion2>(2), createUnion<UnionVersion2>(3), createUnionXYZ(4),
                createUnionXYZ(5), createUnionXYZ(6)};
    }

    template <typename T_READ, typename T_WRITE>
    T_READ writeRead(T_WRITE& data)
    {
        zserio::View view(data);
        const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);

        auto bitBuffer = zserio::serialize(view);
        const zserio::BitSize serializedBitSize = static_cast<zserio::BitSize>(bitBuffer.getBitSize());
        // this must be EXPECT_EQ and not ASSERT_EQ because ASSERT_EQ returns void
        EXPECT_EQ(bitSize, serializedBitSize);

        T_READ readData;
        zserio::deserialize(bitBuffer, readData);

        return readData;
    }

    template <typename T_READ, typename T_WRITE>
    T_READ writeReadFile(T_WRITE& data, std::string_view fileName)
    {
        zserio::View view(data);
        zserio::serializeToFile(view, fileName);

        T_READ readData;
        zserio::deserializeFromFile(fileName, readData);

        return readData;
    }

    static constexpr std::string_view BLOB_NAME_BASE = "language/union_types/union_compatibility_check_";
};

TEST_F(UnionCompatibilityCheckTest, writeVersion1ReadVersion1)
{
    UnionCompatibilityCheckVersion1 data1;
    fillUnionCompatibilityCheck(data1, createArrayVersion1<UnionVersion1>());

    test_utils::writeReadTest(data1);
}

TEST_F(UnionCompatibilityCheckTest, writeVersion1ReadVersion1File)
{
    UnionCompatibilityCheckVersion1 data1;
    fillUnionCompatibilityCheck(data1, createArrayVersion1<UnionVersion1>());

    test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "version1_version1.blob", data1);
}

TEST_F(UnionCompatibilityCheckTest, writeVersion1ReadVersion2)
{
    UnionCompatibilityCheckVersion1 data1;
    fillUnionCompatibilityCheck(data1, createArrayVersion1<UnionVersion1>());

    auto readData2 = writeRead<UnionCompatibilityCheckVersion2>(data1);
    const auto expectedArrayVersion2 = createArrayVersion1<UnionVersion2>();
    ASSERT_EQ(expectedArrayVersion2, readData2.array);
    ASSERT_EQ(expectedArrayVersion2, readData2.packedArray);
}

TEST_F(UnionCompatibilityCheckTest, writeVersion1ReadVersion2File)
{
    UnionCompatibilityCheckVersion1 data1;
    fillUnionCompatibilityCheck(data1, createArrayVersion1<UnionVersion1>());

    auto readData2 = writeReadFile<UnionCompatibilityCheckVersion2>(
            data1, std::string(BLOB_NAME_BASE) + "version1_version2.blob");
    const auto expectedArrayVersion2 = createArrayVersion1<UnionVersion2>();
    ASSERT_EQ(expectedArrayVersion2, readData2.array);
    ASSERT_EQ(expectedArrayVersion2, readData2.packedArray);
}

TEST_F(UnionCompatibilityCheckTest, writeVersion2ReadVersion1)
{
    UnionCompatibilityCheckVersion2 data2;
    fillUnionCompatibilityCheck(data2, createArrayVersion1<UnionVersion2>());

    auto readData1 = writeRead<UnionCompatibilityCheckVersion1>(data2);
    const auto expectedArrayVersion1 = createArrayVersion1<UnionVersion1>();
    ASSERT_EQ(expectedArrayVersion1, readData1.array);
    ASSERT_EQ(expectedArrayVersion1, readData1.packedArray);
}

TEST_F(UnionCompatibilityCheckTest, writeVersion2ReadVersion1File)
{
    UnionCompatibilityCheckVersion2 data2;
    fillUnionCompatibilityCheck(data2, createArrayVersion1<UnionVersion2>());

    auto readData1 = writeReadFile<UnionCompatibilityCheckVersion1>(
            data2, std::string(BLOB_NAME_BASE) + "version2_version1.blob");
    const auto expectedArrayVersion1 = createArrayVersion1<UnionVersion1>();
    ASSERT_EQ(expectedArrayVersion1, readData1.array);
    ASSERT_EQ(expectedArrayVersion1, readData1.packedArray);
}

TEST_F(UnionCompatibilityCheckTest, writeVersion2ReadVersion2)
{
    UnionCompatibilityCheckVersion2 data2;
    fillUnionCompatibilityCheck(data2, createArrayVersion2());

    test_utils::writeReadTest(data2);
}

TEST_F(UnionCompatibilityCheckTest, writeVersion2ReadVersion2File)
{
    UnionCompatibilityCheckVersion2 data2;
    fillUnionCompatibilityCheck(data2, createArrayVersion2());

    test_utils::writeReadFileTest(std::string(BLOB_NAME_BASE) + "version2_version2.blob", data2);
}

} // namespace union_compatibility_check
} // namespace union_types
