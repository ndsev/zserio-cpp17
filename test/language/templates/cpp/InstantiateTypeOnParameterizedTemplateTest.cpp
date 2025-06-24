#include "gtest/gtest.h"
#include "templates/instantiate_type_on_parameterized_template/InstantiateTypeOnParameterizedTemplate.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace instantiate_type_on_parameterized_template
{

using AllocatorType = InstantiateTypeOnParameterizedTemplate::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

TEST(InstantiateTypeOnParameterizedTemplateTest, writeRead)
{
    InstantiateTypeOnParameterizedTemplate data{2, TestP{Parameterized{VectorType<zserio::UInt32>{13, 42}}}};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_type_on_parameterized_template
} // namespace templates
