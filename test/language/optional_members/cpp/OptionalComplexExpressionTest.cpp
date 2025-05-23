#include "gtest/gtest.h"
#include "optional_members/optional_complex_expression/Container.h"
#include "test_utils/TestUtility.h"
#include "zserio/MissedOptionalException.h"
#include "zserio/UnexpectedOptionalException.h"

namespace optional_members
{
namespace optional_complex_expression
{

class OptionalComplexExpressionTest : public ::testing::Test
{
protected:
    static constexpr zserio::BitSize CONTAINER_WITHOUT_OPTIONAL_BIT_SIZE = 5;
    static constexpr zserio::BitSize CONTAINER_WITH_OPTIONAL_BIT_SIZE = 9;
    static constexpr zserio::UInt4 EXTRA_DATA_VALUE = 0x0F;
};

TEST_F(OptionalComplexExpressionTest, withoutOptional)
{
    Container data{Permission::Values::READ, {}};
    ASSERT_EQ(CONTAINER_WITHOUT_OPTIONAL_BIT_SIZE, zserio::detail::bitSizeOf(zserio::View(data)));

    test_utils::writeReadTest(data);
}

TEST_F(OptionalComplexExpressionTest, withOptional)
{
    Container data{Permission::Values::WRITE, EXTRA_DATA_VALUE};
    ASSERT_EQ(CONTAINER_WITH_OPTIONAL_BIT_SIZE, zserio::detail::bitSizeOf(zserio::View(data)));

    test_utils::writeReadTest(data);
}

TEST_F(OptionalComplexExpressionTest, validateMissedOptional)
{
    Container data;
    data.permission = Permission::Values::WRITE;
    zserio::View view(data);
    ASSERT_THROW(zserio::detail::validate(view), zserio::MissedOptionalException);
}

TEST_F(OptionalComplexExpressionTest, validateUnexpectedOptional)
{
    Container data;
    data.permission = Permission::Values::NONE;
    data.extraData = EXTRA_DATA_VALUE;
    zserio::View view(data);
    ASSERT_THROW(zserio::detail::validate(view), zserio::UnexpectedOptionalException);
}

} // namespace optional_complex_expression
} // namespace optional_members
