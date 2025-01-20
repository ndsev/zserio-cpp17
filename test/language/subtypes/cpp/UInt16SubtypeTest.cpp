#include "gtest/gtest.h"
#include "subtypes/uint16_subtype/TestStructure.h"

namespace subtypes
{
namespace uint16_subtype
{

TEST(UInt16SubtypeTest, testSubtype)
{
    const Identifier identifier = 0xFFFF;
    TestStructure data;
    data.identifier = identifier;
    const Identifier readIdentifier = data.identifier;
    ASSERT_EQ(identifier, readIdentifier);
}

} // namespace uint16_subtype
} // namespace subtypes
