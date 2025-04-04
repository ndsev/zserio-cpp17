#include "gtest/gtest.h"
#include "structure_types/structure_inner_classes_clashing/ArrayType_array.h"
#include "structure_types/structure_inner_classes_clashing/OffsetChecker_array.h"
#include "structure_types/structure_inner_classes_clashing/OffsetInitializer_array.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace structure_types
{
namespace structure_inner_classes_clashing
{

using allocator_type = ArrayType_array::allocator_type;

template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

TEST(StructureInnerClassesClashingTest, writeReadArrayTypeArray)
{
    const VectorType<zserio::UInt32> array{10, 20, 30, 40, 50};
    ArrayType_array data(array);

    test_utils::writeReadTest(data);
}

TEST(StructureInnerClassesClashingTest, writeReadOffsetCheckerArray)
{
    const VectorType<zserio::UInt32> array{10, 20, 30, 40, 50};
    OffsetChecker_array data(array, {});
    data.offsets.resize(data.array.size());

    test_utils::writeReadTest(data);
}

TEST(StructureInnerClassesClashingTest, writeReadOffsetInitializerArray)
{
    const VectorType<zserio::UInt32> array{10, 20, 30, 40, 50};
    OffsetInitializer_array data(array, {});
    data.offsets.resize(data.array.size());

    test_utils::writeReadTest(data);
}

} // namespace structure_inner_classes_clashing
} // namespace structure_types
