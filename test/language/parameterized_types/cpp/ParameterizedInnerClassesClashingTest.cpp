#include "gtest/gtest.h"
#include "parameterized_types/parameterized_inner_classes_clashing/ElementChildrenInitializer_array.h"
#include "parameterized_types/parameterized_inner_classes_clashing/ElementFactory_array.h"
#include "parameterized_types/parameterized_inner_classes_clashing/ElementInitializer_array.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace parameterized_inner_classes_clashing
{

using AllocatorType = ElementFactory_array::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class ParameterizedInnerClassesClashingTest : public ::testing::Test
{
protected:
};

TEST_F(ParameterizedInnerClassesClashingTest, writeReadElementFactory)
{
    ElementFactory_array data{100, VectorType<Compound>{{Compound{13}, Compound{42}}}};

    test_utils::writeReadTest(data);
}

TEST_F(ParameterizedInnerClassesClashingTest, writeReadElementInitializer)
{
    ElementInitializer_array data{100, VectorType<Compound>{{Compound{13}, Compound{42}}}};

    test_utils::writeReadTest(data);
}

TEST_F(ParameterizedInnerClassesClashingTest, writeReadElementChildrenInitializer)
{
    const uint32_t param = 100;
    ElementChildrenInitializer_array data{
            VectorType<Parent>{{Parent{param, Compound{13}}, Parent{param, Compound{42}}}}};

    test_utils::writeReadTest(data);
}

} // namespace parameterized_inner_classes_clashing
} // namespace parameterized_types
