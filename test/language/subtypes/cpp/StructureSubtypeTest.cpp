#include "gtest/gtest.h"
#include "subtypes/structure_subtype/Student.h"
#include "subtypes/structure_subtype/SubtypeStructure.h"
#include "zserio/RebindAlloc.h"

namespace subtypes
{
namespace structure_subtype
{

TEST(StructureSubtypeTest, testSubtype)
{
    SubtypeStructure subtypeStructure;
    const Student student{0xFFFF, "Name"};
    subtypeStructure.student = student;
    const Student readStudent = subtypeStructure.student;

    ASSERT_EQ(readStudent, student);
}

} // namespace structure_subtype
} // namespace subtypes
