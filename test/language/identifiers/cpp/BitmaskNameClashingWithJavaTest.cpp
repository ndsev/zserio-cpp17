#include "gtest/gtest.h"
#include "identifiers/bitmask_name_clashing_with_java/BitmaskNameClashingWithJava.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"

namespace identifiers
{
namespace bitmask_name_clashing_with_java
{

class BitmaskNameClashingWithJavaTest : public ::testing::Test
{
protected:
    static constexpr size_t BIT_SIZE = 8;
};

TEST_F(BitmaskNameClashingWithJavaTest, emptyConstructor)
{
    {
        BitmaskNameClashingWithJava data;
        ASSERT_EQ(0, data.stringField.getValue());
    }
    {
        BitmaskNameClashingWithJava data = {};
        ASSERT_EQ(0, data.stringField.getValue());
    }
    {
        BitmaskNameClashingWithJava data = {};
        zserio::View view(data);
        ASSERT_EQ(0, view.stringField().getValue());
    }
}

TEST_F(BitmaskNameClashingWithJavaTest, bitSizeOf)
{
    BitmaskNameClashingWithJava data{String::Values::WRITE};
    zserio::View view(data);
    ASSERT_EQ(BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(BitmaskNameClashingWithJavaTest, toStringMethod)
{
    BitmaskNameClashingWithJava data{String::Values::READ};
    zserio::View view(data);
    ASSERT_EQ(std::string{"1[READ]"}, view.stringField().toString().c_str());
}

} // namespace bitmask_name_clashing_with_java
} // namespace identifiers
