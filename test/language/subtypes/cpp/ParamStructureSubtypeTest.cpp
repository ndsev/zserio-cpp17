#include "gtest/gtest.h"
#include "subtypes/param_structure_subtype/ParameterizedSubtypeStruct.h"
#include "test_utils/Assertions.h"
#include "zserio/RebindAlloc.h"

namespace subtypes
{
namespace param_structure_subtype
{

using AllocatorType = ParameterizedSubtypeStruct::AllocatorType;

template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

TEST(ParamStructureSubtypeTest, testSubtype)
{
    // just check that Another/ParameterizedSubtype is defined and that it's same as the ParameterizedStruct
    ParameterizedSubtypeStruct data;
    ParameterizedSubtype& parameterizedSubtype = data.parameterizedSubtype;
    ParameterizedStruct& parameterizedStruct = data.parameterizedSubtype;
    VectorType<AnotherParameterizedSubtype>& anotherParameterizedSubtypeArray =
            data.anotherParameterizedSubtypeArray;
    VectorType<ParameterizedStruct>& parameterizedStructArray = data.anotherParameterizedSubtypeArray;
    ASSERT_EQ(&parameterizedSubtype, &parameterizedStruct);
    ASSERT_EQ(&anotherParameterizedSubtypeArray, &parameterizedStructArray);

    // ensure that include to the subtype is present and that the subtype is used
    ASSERT_STRING_IN_FILE_PRESENT(
            "language/subtypes/gen/subtypes/param_structure_subtype/ParameterizedSubtypeStruct.h",
            "#include <subtypes/param_structure_subtype/ParameterizedSubtype.h>");

    ASSERT_STRING_IN_FILE_PRESENT(
            "language/subtypes/gen/subtypes/param_structure_subtype/ParameterizedSubtypeStruct.h",
            "::subtypes::param_structure_subtype::ParameterizedSubtype parameterizedSubtype");
    ASSERT_STRING_IN_FILE_PRESENT(
            "language/subtypes/gen/subtypes/param_structure_subtype/ParameterizedSubtypeStruct.h",
            "::zserio::Vector<::subtypes::param_structure_subtype::AnotherParameterizedSubtype> "
            "anotherParameterizedSubtypeArray");

    ASSERT_STRING_IN_FILE_PRESENT(
            "language/subtypes/gen/subtypes/param_structure_subtype/ParameterizedSubtypeStruct.h",
            "View<::subtypes::param_structure_subtype::ParameterizedSubtype> parameterizedSubtype() const");
    ASSERT_STRING_IN_FILE_PRESENT(
            "language/subtypes/gen/subtypes/param_structure_subtype/ParameterizedSubtypeStruct.h",
            "Array<::zserio::Vector<::subtypes::param_structure_subtype::AnotherParameterizedSubtype>, "
            "ArrayType::AUTO, ::zserio::ArrayStorage::IMMUTABLE, "
            "ZserioAnotherParameterizedSubtypeArrayArrayTraits> anotherParameterizedSubtypeArray");
}

} // namespace param_structure_subtype
} // namespace subtypes