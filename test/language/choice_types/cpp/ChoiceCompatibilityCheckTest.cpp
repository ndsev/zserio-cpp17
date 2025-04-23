#include "choice_types/choice_compatibility_check/ChoiceCompatibilityCheckVersion1.h"
#include "choice_types/choice_compatibility_check/ChoiceCompatibilityCheckVersion2.h"
#include "gtest/gtest.h"
#include "zserio/SerializeUtil.h"

namespace choice_types
{
namespace choice_compatibility_check
{

using AllocatorType = ChoiceCompatibilityCheckVersion1::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

namespace
{
template <typename HOLDER>
struct VersionTraits
{};

template <>
struct VersionTraits<HolderVersion1>
{
    using Holder = HolderVersion1;
    using Choice = ChoiceVersion1;
    using Enum = EnumVersion1;
};

template <>
struct VersionTraits<HolderVersion2>
{
    using Holder = HolderVersion2;
    using Choice = ChoiceVersion2;
    using Enum = EnumVersion2;
};
} // namespace

class ChoiceCompatibilityCheckTest : public ::testing::Test
{
protected:
    template <typename T, typename ARRAY>
    static void fillData(T& data, const ARRAY& array)
    {
        data.array = array;
        data.packedArray = array;
    }

    template <typename T_READ, typename T_WRITE>
    static T_READ writeRead(T_WRITE& data)
    {
        auto bitBuffer = zserio::serialize(data);

        T_READ readData;
        zserio::deserialize(bitBuffer, readData);
        return readData;
    }

    template <typename T_READ, typename T_WRITE>
    T_READ writeReadFile(T_WRITE& data, const char* variant)
    {
        const std::string fileName = std::string(BLOB_NAME_BASE) + variant + ".blob";
        zserio::serializeToFile(data, fileName);

        T_READ readData;
        zserio::deserializeFromFile(fileName, readData);
        return readData;
    }

    template <typename HOLDER, typename ENUM>
    static HOLDER createHolder(ENUM selector, uint32_t index)
    {
        using Choice = typename VersionTraits<HOLDER>::Choice;

        Choice choice;
        if (selector == ENUM::COORD_XY)
        {
            choice.template emplace<Choice::Tag::coordXY>(CoordXY{10 * index, 20 * index});
        }
        else
        {
            choice.template emplace<Choice::Tag::text>("text" + zserio::toString<AllocatorType>(index));
        }

        HOLDER holder(selector, std::move(choice));
        return holder;
    }

    template <typename HOLDER>
    static VectorType<HOLDER> createArrayVersion1()
    {
        using Enum = typename VersionTraits<HOLDER>::Enum;

        auto holder = createHolder<HOLDER>(Enum::COORD_XY, 0);
        (void)holder;

        return VectorType<HOLDER>{createHolder<HOLDER>(Enum::COORD_XY, 0), createHolder<HOLDER>(Enum::TEXT, 1),
                createHolder<HOLDER>(Enum::COORD_XY, 2), createHolder<HOLDER>(Enum::TEXT, 3)};
    }

    HolderVersion2 createHolderCoordXYZ(uint32_t index)
    {
        ChoiceVersion2 choice;
        choice.emplace<ChoiceVersion2::Tag::coordXYZ>(CoordXYZ{10 * index, 20 * index, 1.1 * index});

        HolderVersion2 holderVersion2(EnumVersion2::COORD_XYZ, std::move(choice));
        return holderVersion2;
    }

    VectorType<HolderVersion2> createArrayVersion2()
    {
        auto array = createArrayVersion1<HolderVersion2>();
        array.push_back(createHolderCoordXYZ(4));
        array.push_back(createHolderCoordXYZ(5));
        array.push_back(createHolderCoordXYZ(6));
        return array;
    }

private:
    static constexpr std::string_view BLOB_NAME_BASE = "language/choice_types/choice_compatibility_check_";
};

TEST_F(ChoiceCompatibilityCheckTest, writeVersion1ReadVersion1)
{
    ChoiceCompatibilityCheckVersion1 dataVersion1;
    fillData(dataVersion1, createArrayVersion1<HolderVersion1>());
    zserio::View viewVersion1(dataVersion1);

    writeRead<ChoiceCompatibilityCheckVersion1>(dataVersion1);

    auto readDataVersion1 = writeRead<ChoiceCompatibilityCheckVersion1>(dataVersion1);
    ASSERT_EQ(dataVersion1, readDataVersion1);
    ASSERT_EQ(viewVersion1, zserio::View(readDataVersion1));
}

TEST_F(ChoiceCompatibilityCheckTest, writeVersion1ReadVersion2)
{
    ChoiceCompatibilityCheckVersion1 dataVersion1;
    fillData(dataVersion1, createArrayVersion1<HolderVersion1>());

    auto readDataVersion2 = writeRead<ChoiceCompatibilityCheckVersion2>(dataVersion1);

    ChoiceCompatibilityCheckVersion2 expectedDataVersion2;
    fillData(expectedDataVersion2, createArrayVersion1<HolderVersion2>());
    zserio::View expectedViewVersion2(expectedDataVersion2);
    ASSERT_EQ(expectedDataVersion2, readDataVersion2);
    ASSERT_EQ(expectedViewVersion2, zserio::View(readDataVersion2));
}

TEST_F(ChoiceCompatibilityCheckTest, writeVersion2ReadVersion1)
{
    ChoiceCompatibilityCheckVersion2 dataVersion2;
    fillData(dataVersion2, createArrayVersion1<HolderVersion2>());

    auto readDataVersion1 = writeRead<ChoiceCompatibilityCheckVersion1>(dataVersion2);

    ChoiceCompatibilityCheckVersion1 expectedDataVersion1;
    fillData(expectedDataVersion1, createArrayVersion1<HolderVersion1>());
    zserio::View expectedViewVersion1(expectedDataVersion1);
    ASSERT_EQ(expectedDataVersion1, readDataVersion1);
    ASSERT_EQ(expectedViewVersion1, zserio::View(readDataVersion1));
}

TEST_F(ChoiceCompatibilityCheckTest, writeVersion2ReadVersion2)
{
    ChoiceCompatibilityCheckVersion2 dataVersion2;
    fillData(dataVersion2, createArrayVersion2());
    zserio::View viewVersion2(dataVersion2);

    auto readDataVersion2 = writeRead<ChoiceCompatibilityCheckVersion2>(dataVersion2);
    ASSERT_EQ(dataVersion2, readDataVersion2);
    ASSERT_EQ(viewVersion2, zserio::View(readDataVersion2));
}

TEST_F(ChoiceCompatibilityCheckTest, writeVersion1ReadVersion1File)
{
    ChoiceCompatibilityCheckVersion1 dataVersion1;
    fillData(dataVersion1, createArrayVersion1<HolderVersion1>());
    zserio::View viewVersion1(dataVersion1);

    writeRead<ChoiceCompatibilityCheckVersion1>(dataVersion1);

    auto readDataVersion1 = writeReadFile<ChoiceCompatibilityCheckVersion1>(dataVersion1, "version1_version1");
    ASSERT_EQ(dataVersion1, readDataVersion1);
    ASSERT_EQ(viewVersion1, zserio::View(readDataVersion1));
}

TEST_F(ChoiceCompatibilityCheckTest, writeVersion1ReadVersion2File)
{
    ChoiceCompatibilityCheckVersion1 dataVersion1;
    fillData(dataVersion1, createArrayVersion1<HolderVersion1>());

    auto readDataVersion2 = writeReadFile<ChoiceCompatibilityCheckVersion2>(dataVersion1, "version1_version2");

    ChoiceCompatibilityCheckVersion2 expectedDataVersion2;
    fillData(expectedDataVersion2, createArrayVersion1<HolderVersion2>());
    zserio::View expectedViewVersion2(expectedDataVersion2);
    ASSERT_EQ(expectedDataVersion2, readDataVersion2);
    ASSERT_EQ(expectedViewVersion2, zserio::View(readDataVersion2));
}

TEST_F(ChoiceCompatibilityCheckTest, writeVersion2ReadVersion1File)
{
    ChoiceCompatibilityCheckVersion2 dataVersion2;
    fillData(dataVersion2, createArrayVersion1<HolderVersion2>());

    auto readDataVersion1 = writeReadFile<ChoiceCompatibilityCheckVersion1>(dataVersion2, "version2_version1");

    ChoiceCompatibilityCheckVersion1 expectedDataVersion1;
    fillData(expectedDataVersion1, createArrayVersion1<HolderVersion1>());
    zserio::View expectedViewVersion1(expectedDataVersion1);
    ASSERT_EQ(expectedDataVersion1, readDataVersion1);
    ASSERT_EQ(expectedViewVersion1, zserio::View(readDataVersion1));
}

TEST_F(ChoiceCompatibilityCheckTest, writeVersion2ReadVersion2File)
{
    ChoiceCompatibilityCheckVersion2 dataVersion2;
    fillData(dataVersion2, createArrayVersion2());
    zserio::View viewVersion2(dataVersion2);

    auto readDataVersion2 = writeReadFile<ChoiceCompatibilityCheckVersion2>(dataVersion2, "version2_version2");
    ASSERT_EQ(dataVersion2, readDataVersion2);
    ASSERT_EQ(viewVersion2, zserio::View(readDataVersion2));
}

} // namespace choice_compatibility_check
} // namespace choice_types
