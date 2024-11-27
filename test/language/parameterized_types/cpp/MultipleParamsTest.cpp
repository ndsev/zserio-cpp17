#include <array>
#include <limits>
#include <string>
#include <string_view>

#include "gtest/gtest.h"
#include "parameterized_types/multiple_params/TestStructure.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace parameterized_types
{
namespace multiple_params
{

using AllocatorType = TestStructure::AllocatorType;

template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

class MultipleParamsTest : public ::testing::Test
{
protected:
    static constexpr std::string_view BLOB_NAME = "language/parameterized_types/multiple_params.blob";
};

TEST_F(MultipleParamsTest, arrayLength)
{
    const TestStructure data;
    zserio::View view(data, 1, 1);
    ASSERT_EQ(1, view.arrayLength());
}

TEST_F(MultipleParamsTest, bitFieldLength)
{
    const TestStructure data;
    zserio::View view(data, 1, 1);
    ASSERT_EQ(1, view.bitFieldLength());
}

TEST_F(MultipleParamsTest, array5)
{
    TestStructure data;
    const VectorType<zserio::UInt5> array5 = {1, 10, 15};
    data.array5 = array5;
    zserio::View view(data, static_cast<uint32_t>(array5.size()), 1);
    zserio::Array<zserio::Vector<zserio::UInt5>, zserio::ArrayType::NORMAL> expectedArray5View(
            array5, array5.size());
    ASSERT_EQ(expectedArray5View, view.array5());
}

TEST_F(MultipleParamsTest, array13)
{
    TestStructure data;
    const VectorType<Data13> array13 = {
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
    };
    data.array13 = array13;
    zserio::View view(data, static_cast<uint32_t>(array13.size()), 1);
    zserio::Array<zserio::Vector<Data13>, zserio::ArrayType::NORMAL,
            zserio::View<TestStructure>::ZserioArray13ArrayTraits>
            expectedArray13View(array13, view, array13.size());
    ASSERT_EQ(expectedArray13View, view.array13());
}

TEST_F(MultipleParamsTest, field17)
{
    TestStructure data;
    const zserio::Int17 field17 = 65535;
    data.field17 = field17;
    zserio::View view(data, 1, 1);
    ASSERT_EQ(field17, view.field17());
}

TEST_F(MultipleParamsTest, dynBitField)
{
    TestStructure data;
    const zserio::DynInt16<> dynBitField = 16383;
    data.dynBitField = dynBitField;
    zserio::View view(data, 1, 15);
    ASSERT_EQ(dynBitField, view.dynBitField());
}

TEST_F(MultipleParamsTest, comparisonOperators)
{
    const VectorType<zserio::UInt5> array5 = {1, 10, 15};
    const VectorType<Data13> array13 = {
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
    };
    const zserio::Int17 field17 = 65535;
    const zserio::DynInt16<> dynBitField = 16383;
    const TestStructure data{array5, array13, field17, dynBitField};
    const TestStructure equalData{array5, array13, field17, dynBitField};
    const VectorType<zserio::UInt5> lessThenArray5 = {1, 10, 14};
    const TestStructure lessThenData{lessThenArray5, array13, field17, dynBitField};
    test_utils::comparisonOperatorsTest(data, equalData, lessThenData);

    zserio::View view(data, static_cast<uint32_t>(array5.size()), 15);
    zserio::View equalView(equalData, static_cast<uint32_t>(array5.size()), 15);
    zserio::View lessThenView(lessThenData, static_cast<uint32_t>(array5.size()), 15);
    test_utils::comparisonOperatorsTest(view, equalView, lessThenView);
}

TEST_F(MultipleParamsTest, validate)
{
    {
        // parameter arrayLength is out of range
        const TestStructure data;
        zserio::View view(data, std::numeric_limits<uint32_t>::max(), 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // parameter bitFieldLength is out of range
        const TestStructure data;
        zserio::View view(data, 1, 16);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }

    const VectorType<zserio::UInt5> array5 = {1, 10, 15};
    const VectorType<Data13> array13 = {
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
            Data13{VectorType<zserio::Int13>{1, 10, 15}},
    };
    const zserio::Int17 field17 = 65535;
    const zserio::DynInt16<> dynBitField = 16383;
    {
        // field array5 is out of range
        const VectorType<zserio::UInt5> wrongArray5 = {1, 10, 255};
        const TestStructure data{wrongArray5, array13, field17, dynBitField};
        zserio::View view(data, static_cast<uint32_t>(array5.size()), 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // field array5 has wrong length
        const TestStructure data{array5, array13, field17, dynBitField};
        zserio::View view(data, 1, 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::ArrayLengthException);
    }
    {
        // field array13 is out of range
        const VectorType<Data13> wrongArray13 = {
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 15}},
                Data13{VectorType<zserio::Int13>{1, 10, 32767}},
        };
        const TestStructure data{array5, wrongArray13, field17, dynBitField};
        zserio::View view(data, static_cast<uint32_t>(array5.size()), 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // field array13 has wrong length
        const TestStructure data{array5, array13, field17, dynBitField};
        zserio::View view(data, 1, 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::ArrayLengthException);
    }
    {
        // field field17 is out of range
        const zserio::Int17 wrongField17 = 131072;
        const TestStructure data{array5, array13, wrongField17, dynBitField};
        zserio::View view(data, static_cast<uint32_t>(array5.size()), 1);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
    {
        // field dynBitField is out of range
        const zserio::DynInt16<> wrongDynBitField = 32767;
        const TestStructure data{array5, array13, field17, wrongDynBitField};
        zserio::View view(data, static_cast<uint32_t>(array5.size()), 15);
        ASSERT_THROW(zserio::detail::validate(view), zserio::OutOfRangeException);
    }
}

TEST_F(MultipleParamsTest, writeRead)
{
    TestStructure data{
            {1, 10, 15},
            {
                    Data13{VectorType<zserio::Int13>{1, 10, 15}},
                    Data13{VectorType<zserio::Int13>{1, 10, 15}},
                    Data13{VectorType<zserio::Int13>{1, 10, 15}},
            },
            65535,
            16383,
    };
    test_utils::writeReadTest(data, zserio::VarSize{3}, zserio::UInt4{15});
}

TEST_F(MultipleParamsTest, writeReadFile)
{
    TestStructure data{
            {1, 10, 15},
            {
                    Data13{VectorType<zserio::Int13>{1, 10, 15}},
                    Data13{VectorType<zserio::Int13>{1, 10, 15}},
                    Data13{VectorType<zserio::Int13>{1, 10, 15}},
            },
            65535,
            16383,
    };
    test_utils::writeReadFileTest(BLOB_NAME, data, zserio::VarSize{3}, zserio::UInt4{15});
}

} // namespace multiple_params
} // namespace parameterized_types
