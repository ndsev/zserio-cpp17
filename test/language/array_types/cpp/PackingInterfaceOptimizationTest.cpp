#include "array_types/packing_interface_optimization/PackingInterfaceOptimization.h"
#include "gtest/gtest.h"
#include "test_utils/Assertions.h"
#include "test_utils/TestUtility.h"

namespace array_types
{
namespace packing_interface_optimization
{

using AllocatorType = PackingInterfaceOptimization::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, AllocatorType>;

class PackingInterfaceOptimizationTest : public ::testing::Test
{
protected:
    void fillUnpackedColors(VectorType<UnpackedColorStruct>& unpackedColors)
    {
        unpackedColors.emplace_back(true, UnpackedColorChoice{});
        unpackedColors.back().colorChoice.emplace<UnpackedColorChoice::ChoiceTag::CHOICE_colorName>("yellow");

        unpackedColors.emplace_back(false, UnpackedColorChoice{});
        unpackedColors.back().colorChoice.emplace<UnpackedColorChoice::ChoiceTag::CHOICE_colorUnion>(
                UnpackedColorUnion(zserio::in_place_index<UnpackedColorUnion::ChoiceTag::CHOICE_colorBitmask>,
                        UnpackedColorBitmask::Values::GREEN | UnpackedColorBitmask::Values::RED));

        unpackedColors.emplace_back(false, UnpackedColorChoice{});
        unpackedColors.back().colorChoice.emplace<UnpackedColorChoice::ChoiceTag::CHOICE_colorUnion>(
                UnpackedColorUnion(zserio::in_place_index<UnpackedColorUnion::ChoiceTag::CHOICE_colorEnum>,
                        UnpackedColorEnum::BLUE));
    }

    void fillMixedColors(VectorType<MixedColorStruct>& mixedColors)
    {
        mixedColors.emplace_back(true, MixedColorChoice{});
        mixedColors.back().colorChoice.emplace<MixedColorChoice::ChoiceTag::CHOICE_colorName>("purple");

        mixedColors.emplace_back(false, MixedColorChoice{});
        mixedColors.back().colorChoice.emplace<MixedColorChoice::ChoiceTag::CHOICE_colorUnion>(MixedColorUnion(
                zserio::in_place_index<MixedColorUnion::ChoiceTag::CHOICE_colorBitmask>,
                MixedColorBitmask::Values::BLUE | MixedColorBitmask::Values::GREEN));

        mixedColors.emplace_back(false, MixedColorChoice{});
        mixedColors.back().colorChoice.emplace<MixedColorChoice::ChoiceTag::CHOICE_colorUnion>(MixedColorUnion(
                zserio::in_place_index<MixedColorUnion::ChoiceTag::CHOICE_colorEnum>, MixedColorEnum::RED));
    }

    void fillPackedColors(VectorType<PackedColorStruct>& packedColors)
    {
        packedColors.emplace_back(true, PackedColorChoice{});
        packedColors.back().colorChoice.emplace<PackedColorChoice::ChoiceTag::CHOICE_colorName>("grey");

        packedColors.emplace_back(false, PackedColorChoice{});
        packedColors.back().colorChoice.emplace<PackedColorChoice::ChoiceTag::CHOICE_colorUnion>(
                PackedColorUnion(zserio::in_place_index<PackedColorUnion::ChoiceTag::CHOICE_colorBitmask>,
                        PackedColorBitmask::Values::BLUE | PackedColorBitmask::Values::RED));

        packedColors.emplace_back(false, PackedColorChoice{});
        packedColors.back().colorChoice.emplace<PackedColorChoice::ChoiceTag::CHOICE_colorUnion>(
                PackedColorUnion(zserio::in_place_index<PackedColorUnion::ChoiceTag::CHOICE_colorEnum>,
                        PackedColorEnum::GREEN));
    }

    void fillUnpackedColorsHolder(UnpackedColorsHolder& unpackedColorsHolder)
    {
        fillUnpackedColors(unpackedColorsHolder.unpackedColors);
        fillMixedColors(unpackedColorsHolder.mixedColors);
    }

    void fillPackedColorsHolder(PackedColorsHolder& packedColorsHolder)
    {
        fillMixedColors(packedColorsHolder.mixedColors);
        fillPackedColors(packedColorsHolder.packedColors);
    }

    void assertPackingInterfaceMethodsPresent(const std::string& typeName)
    {
        ASSERT_METHOD_PRESENT(
                PATH, typeName, "void initContext(PackingContext<", "initContext(PackingContext<");
        ASSERT_METHOD_PRESENT(
                PATH, typeName, "BitSize bitSizeOf(PackingContext<", "BitSize bitSizeOf(PackingContext<");
        ASSERT_METHOD_PRESENT(PATH, typeName, "void write(PackingContext<", "void write(PackingContext<");
        ASSERT_METHOD_PRESENT(PATH, typeName, "void read(PackingContext<", "void read(PackingContext<");
    }

    void assertPackingInterfaceMethodsNotPresent(const std::string& typeName)
    {
        ASSERT_METHOD_NOT_PRESENT(
                PATH, typeName, "void initContext(PackingContext<", "void initContext(PackingContext<");
        ASSERT_METHOD_NOT_PRESENT(
                PATH, typeName, "BitSize bitSizeOf(PackingContext<", "BitSize bitSizeOf(PackingContext<");
        ASSERT_METHOD_NOT_PRESENT(PATH, typeName, "void write(PackingContext<", "void write(PackingContext<");
        ASSERT_METHOD_NOT_PRESENT(PATH, typeName, "void read(PackingContext<", "void read(PackingContext<");
    }

    static constexpr std::string_view BLOB_NAME = "language/array_types/packing_interface_optimization.blob";
    static constexpr std::string_view PATH =
            "language/array_types/gen/array_types/packing_interface_optimization/";
};

TEST_F(PackingInterfaceOptimizationTest, writeReadFile)
{
    PackingInterfaceOptimization data;
    fillUnpackedColorsHolder(data.unpackedColorsHolder);
    fillPackedColorsHolder(data.packedColorsHolder);

    test_utils::writeReadFileTest(BLOB_NAME, data);
}

TEST_F(PackingInterfaceOptimizationTest, packingInterfaceOptimizationMethods)
{
    assertPackingInterfaceMethodsNotPresent("PackingInterfaceOptimization");
    const std::string path = std::string(PATH) + "PackingInterfaceOptimization.h";
    ASSERT_STRING_IN_FILE_NOT_PRESENT(path, "struct PackingContext<");
}

TEST_F(PackingInterfaceOptimizationTest, unpackedColorsHolderMethods)
{
    assertPackingInterfaceMethodsNotPresent("UnpackedColorsHolder");
    const std::string path = std::string(PATH) + "UnpackedColorsHolder.h";
    ASSERT_STRING_IN_FILE_NOT_PRESENT(path, "struct PackingContext<");
}

TEST_F(PackingInterfaceOptimizationTest, packedColorsHolderMethods)
{
    assertPackingInterfaceMethodsNotPresent("PackedColorsHolder");
    const std::string path = std::string(PATH) + "PackedColorsHolder.h";
    ASSERT_STRING_IN_FILE_NOT_PRESENT(path, "struct PackingContext<");
}

TEST_F(PackingInterfaceOptimizationTest, unpackedColorStructMethods)
{
    assertPackingInterfaceMethodsNotPresent("UnpackedColorStruct");
    const std::string path = std::string(PATH) + "UnpackedColorStruct.h";
    ASSERT_STRING_IN_FILE_NOT_PRESENT(path, "struct PackingContext<");
}

TEST_F(PackingInterfaceOptimizationTest, unpackedColorChoiceMethods)
{
    assertPackingInterfaceMethodsNotPresent("UnpackedColorChoice");
    const std::string path = std::string(PATH) + "UnpackedColorChoice.h";
    ASSERT_STRING_IN_FILE_NOT_PRESENT(path, "struct PackingContext<");
}

TEST_F(PackingInterfaceOptimizationTest, unpackedColorUnionMethods)
{
    assertPackingInterfaceMethodsNotPresent("UnpackedColorUnion");
    const std::string path = std::string(PATH) + "UnpackedColorUnion.h";
    ASSERT_STRING_IN_FILE_NOT_PRESENT(path, "struct PackingContext<");
}

TEST_F(PackingInterfaceOptimizationTest, mixedColorStructMethods)
{
    assertPackingInterfaceMethodsPresent("MixedColorStruct");
    const std::string path = std::string(PATH) + "MixedColorStruct.h";
    ASSERT_STRING_IN_FILE_PRESENT(path, "struct PackingContext<");
}

TEST_F(PackingInterfaceOptimizationTest, mixedColorChoiceMethods)
{
    assertPackingInterfaceMethodsPresent("MixedColorChoice");
    const std::string path = std::string(PATH) + "MixedColorChoice.h";
    ASSERT_STRING_IN_FILE_PRESENT(path, "struct PackingContext<");
}

TEST_F(PackingInterfaceOptimizationTest, mixedColorUnionMethods)
{
    assertPackingInterfaceMethodsPresent("MixedColorUnion");
    const std::string path = std::string(PATH) + "MixedColorUnion.h";
    ASSERT_STRING_IN_FILE_PRESENT(path, "struct PackingContext<");
}

TEST_F(PackingInterfaceOptimizationTest, packedColorStructMethods)
{
    assertPackingInterfaceMethodsPresent("PackedColorStruct");
    const std::string path = std::string(PATH) + "PackedColorStruct.h";
    ASSERT_STRING_IN_FILE_PRESENT(path, "struct PackingContext<");
}

TEST_F(PackingInterfaceOptimizationTest, packedColorChoiceMethods)
{
    assertPackingInterfaceMethodsPresent("PackedColorChoice");
    const std::string path = std::string(PATH) + "PackedColorChoice.h";
    ASSERT_STRING_IN_FILE_PRESENT(path, "struct PackingContext<");
}

TEST_F(PackingInterfaceOptimizationTest, packedColorUnionMethods)
{
    assertPackingInterfaceMethodsPresent("PackedColorUnion");
    const std::string path = std::string(PATH) + "PackedColorUnion.h";
    ASSERT_STRING_IN_FILE_PRESENT(path, "struct PackingContext<");
}

} // namespace packing_interface_optimization
} // namespace array_types
