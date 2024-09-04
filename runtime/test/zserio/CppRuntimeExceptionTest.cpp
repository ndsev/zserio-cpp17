#include <algorithm>
#include <array>
#include <limits>

#include "gtest/gtest.h"
#include "zserio/CppRuntimeException.h"

namespace zserio
{

TEST(CppRuntimeExceptionTest, emptyConstructor)
{
    CppRuntimeException exception;
    ASSERT_EQ(std::string(), exception.what());
}

TEST(CppRuntimeExceptionTest, cStringConstructor)
{
    CppRuntimeException noDescriptionException;
    ASSERT_EQ(std::string(), noDescriptionException.what());

    CppRuntimeException emptyDescriptionException("");
    ASSERT_EQ(std::string(), emptyDescriptionException.what());

    const std::string testMessage = "this is the test message";
    CppRuntimeException exception(testMessage.c_str());
    ASSERT_EQ(testMessage, exception.what());
}

TEST(CppRuntimeExceptionTest, appendCString)
{
    std::string testMessage = "1234567890123456";
    const std::string appendix = "1234567890123456";
    CppRuntimeException exception = CppRuntimeException(testMessage.c_str()) << appendix.c_str();
    testMessage += appendix;
    ASSERT_EQ(testMessage, exception.what());

    const size_t exceptionBufferSize = 512;
    const size_t maxLen = exceptionBufferSize - 1;
    for (int i = 0; i < 100; ++i)
    {
        exception = exception << appendix.c_str();
        testMessage += appendix;
        const size_t len = std::min(testMessage.size(), maxLen);
        ASSERT_EQ(testMessage.substr(0, len), exception.what());
    }
}

TEST(CppRuntimeExceptionTest, appendBool)
{
    std::string testMessage = "test true: ";
    CppRuntimeException exception = CppRuntimeException(testMessage.c_str()) << true;
    testMessage += "true";
    ASSERT_EQ(testMessage, exception.what());

    exception << ", and false: " << false;
    testMessage += ", and false: false";
    ASSERT_EQ(testMessage, exception.what());
}

TEST(CppRuntimeExceptionTest, appendFloat)
{
    const float value = 123.456F;
    CppRuntimeException exception = CppRuntimeException() << value;
    ASSERT_EQ(std::string("123.456"), exception.what());

    const float valueWithoutFloatingPart = 123.0F;
    exception = CppRuntimeException() << valueWithoutFloatingPart;
    ASSERT_EQ(std::string("123.0"), exception.what());

    const float valueInfinity = std::numeric_limits<float>::infinity();
    exception = CppRuntimeException() << valueInfinity;
    ASSERT_EQ(std::string("+Inf"), exception.what());
}

TEST(CppRuntimeExceptionTest, appendDouble)
{
    const double value = 123.456;
    CppRuntimeException exception = CppRuntimeException() << value;
    ASSERT_EQ(std::string("123.456"), exception.what());
}

TEST(CppRuntimeExceptionTest, appendStringView)
{
    using namespace std::literals;
    CppRuntimeException exception = CppRuntimeException() << "test"sv;
    ASSERT_STREQ("test", exception.what());
}

TEST(CppRuntimeExceptionTest, appendInt)
{
    const int value = 42;
    CppRuntimeException exception = CppRuntimeException() << value;
    ASSERT_EQ(std::to_string(value), exception.what());
}

TEST(CppRuntimeExceptionTest, appendString)
{
    CppRuntimeException exception = CppRuntimeException() << std::string("test");
    ASSERT_STREQ("test", exception.what());
}

TEST(CppRuntimeExceptionTest, appendVector)
{
    CppRuntimeException exception = CppRuntimeException() << std::vector<int>{{1, 2, 3}};
    ASSERT_STREQ("vector([...], 3)", exception.what());
}

} // namespace zserio
