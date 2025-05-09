#include "gtest/gtest.h"
#include "optional_members/optional_array/TestStruct.h"
#include "zserio/MissedOptionalException.h"
#include "zserio/RebindAlloc.h"
#include "zserio/UnexpectedOptionalException.h"

// just test setters and getters
namespace optional_members
{
namespace optional_array
{

using AllocatorType = TestStruct::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

TEST(OptionalArrayTest, data8)
{
    TestStruct data;
    data.hasData8 = true;
    data.data8 = VectorType<Data8>(4);
    zserio::View view(data);

    ASSERT_EQ(4, view.data8()->size());
    ASSERT_FALSE(view.data16());
}

TEST(OptionalArrayTest, autoData8)
{
    TestStruct data;
    data.autoData8 = VectorType<Data8>(4);
    zserio::View view(data);

    ASSERT_TRUE(view.autoData8());
    ASSERT_EQ(4, view.autoData8()->size());
}

TEST(OptionalArrayTest, data16)
{
    TestStruct data;
    data.hasData8 = false;
    data.data16 = VectorType<zserio::Int16>(4);
    zserio::View view(data);

    ASSERT_EQ(4, view.data16()->size());
    ASSERT_FALSE(view.data8());
}

TEST(OptionalArrayTest, autoData16)
{
    TestStruct data;
    data.autoData16 = VectorType<zserio::Int16>(4);
    zserio::View view(data);

    ASSERT_TRUE(view.autoData16());
    ASSERT_EQ(4, view.autoData16()->size());
}

TEST(OptionalArrayTest, validateMissedOptional)
{
    TestStruct data;
    data.hasData8 = true;
    zserio::View view(data);
    ASSERT_THROW(zserio::detail::validate(view), zserio::MissedOptionalException);
}

TEST(OptionalArrayTest, validateUnexpectedOptional)
{
    TestStruct data;
    data.data16 = VectorType<zserio::Int16>(4);
    data.data8 = VectorType<Data8>(4);
    zserio::View view(data);
    ASSERT_THROW(zserio::detail::validate(view), zserio::UnexpectedOptionalException);
}

} // namespace optional_array
} // namespace optional_members
