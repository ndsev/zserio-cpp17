#include "expressions/lengthof_operator/LengthOfFunctions.h"
#include "expressions/lengthof_operator/STR_CONSTANT.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SerializeUtil.h"

namespace expressions
{
namespace lengthof_operator
{

using AllocatorType = LengthOfFunctions::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
using BytesType = zserio::BasicBytes<zserio::RebindAlloc<AllocatorType, uint8_t>>;

TEST(LengthOfOperatorTest, getLengthOfStrConstant)
{
    LengthOfFunctions data;
    ASSERT_EQ(11, STR_CONSTANT.size()); // check that it's length in bytes (UTF-8)
    ASSERT_EQ(STR_CONSTANT.size(), zserio::View(data).getLengthOfStrConstant());
}

TEST(LengthOfOperatorTest, getLengthOfLiteral)
{
    LengthOfFunctions data;
    ASSERT_EQ(10, std::string_view("€literal").size()); // check that it's length in bytes (UTF-8)
    ASSERT_EQ(std::string_view("€literal").size(), zserio::View(data).getLengthOfLiteral());
}

TEST(LengthOfOperatorTest, getLengthOfString)
{
    auto strField = StringType("€test");
    LengthOfFunctions data;
    data.strField = strField;
    ASSERT_EQ(7, strField.size()); // check that it's length in bytes (UTF-8)
    ASSERT_EQ(strField.size(), zserio::View(data).getLengthOfString());
}

TEST(LengthOfOperatorTest, getLengthOfBytes)
{
    auto bytesField = BytesType{{0x00, 0x01, 0x02}};
    LengthOfFunctions data;
    data.bytesField = bytesField;
    ASSERT_EQ(bytesField.size(), zserio::View(data).getLengthOfBytes());
}

TEST(LengthOfOperatorTest, getLengthOfFixedArray)
{
    LengthOfFunctions data;
    const zserio::UInt8 fixedArrayLength = 10;
    data.fixedArray.resize(fixedArrayLength);
    ASSERT_EQ(fixedArrayLength, zserio::View(data).getLengthOfFixedArray());
}

TEST(LengthOfOperatorTest, getLengthOfVariableArray)
{
    LengthOfFunctions data;
    const zserio::UInt8 variableArrayLength = 11;
    VectorType<zserio::UInt8> variableArray(variableArrayLength);
    data.numElements = variableArrayLength;
    data.variableArray = variableArray;
    ASSERT_EQ(variableArrayLength, zserio::View(data).getLengthOfVariableArray());
}

TEST(LengthOfOperatorTest, getLengthOfFirstStrInArray)
{
    auto strArray = VectorType<StringType>{"€", "$"};
    LengthOfFunctions data;
    data.strArray = strArray;
    ASSERT_EQ(3, strArray.at(0).size()); // check that it's length in bytes (UTF-8)
    ASSERT_EQ(strArray.at(0).size(), zserio::View(data).getLengthOfFirstStrInArray());
}

TEST(LengthOfOperatorTest, getLengthOfFirstBytesInArray)
{
    auto bytesArray = VectorType<BytesType>{{0x00, 0x01}, {}};
    LengthOfFunctions data;
    data.bytesArray = bytesArray;
    ASSERT_EQ(bytesArray.at(0).size(), zserio::View(data).getLengthOfFirstBytesInArray());
}

TEST(LengthOfOperatorTest, literalLengthFieldDefault)
{
    LengthOfFunctions data;
    ASSERT_EQ(10, std::string_view("€literal").size()); // check that it's length in bytes (UTF-8)
    ASSERT_EQ(std::string_view("€literal").size(), data.literalLengthField);
}

TEST(LengthOfOperatorTest, writeRead)
{
    LengthOfFunctions data;
    data.fixedArray = VectorType<::zserio::UInt8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    data.numElements = 3;
    data.variableArray = VectorType<::zserio::UInt8>{0x03, 0x02, 0x01};
    data.strField = "longer than constant";
    data.bytesField = BytesType{{0x00, 0x01, 0x02}};
    data.strArray = VectorType<StringType>{"text1", "text2", "text3", "text4"};
    data.bytesArray = VectorType<BytesType>{{0x01, 0x02}, {0x03, 0x04}};

    test_utils::writeReadTest(data);
}

} // namespace lengthof_operator
} // namespace expressions
