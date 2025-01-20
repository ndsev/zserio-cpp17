#include "expressions/modulo_operator/ModuloFunction.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace modulo_operator
{

TEST(ModuloOperatorTest, isValueDivBy4)
{
    const ModuloFunction data;
    zserio::View view(data);

    ASSERT_TRUE(view.isValueDivBy4());
}

} // namespace modulo_operator
} // namespace expressions
