#include <fstream>
#include <string>

#include "gtest/gtest.h"

namespace test_utils
{

namespace detail
{

inline bool isStringInFilePresent(std::string_view fileName, std::string_view str)
{
    std::ifstream file(std::string(fileName).data());
    bool isPresent = false;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.find(str) != std::string::npos)
        {
            isPresent = true;
            break;
        }
    }
    file.close();

    return isPresent;
}

inline void assertMethodPresent(std::string_view path, std::string_view typeName, const char* declaration,
        const char* definition, const char* srcFile, int srcLine)
{
    const std::string filePath = std::string(path) + std::string(typeName);
    if (declaration != nullptr)
    {
        ASSERT_TRUE(isStringInFilePresent(filePath + ".h", declaration))
                << "Method declaration '" << declaration << "' is not present in '" << typeName << "'! "
                << srcFile << ":" << srcLine;
    }
    if (definition != nullptr)
    {
        ASSERT_TRUE(isStringInFilePresent(filePath + ".cpp", definition))
                << "Method definition '" << definition << "' is not present in '" << typeName << "'! "
                << srcFile << ":" << srcLine;
    }
}

inline void assertMethodNotPresent(std::string_view path, std::string_view typeName, const char* declaration,
        const char* definition, const char* srcFile, int srcLine)
{
    const std::string filePath = std::string(path) + std::string(typeName);
    if (declaration != nullptr)
    {
        ASSERT_FALSE(isStringInFilePresent(filePath + ".h", declaration))
                << "Method declaration '" << declaration << "' is present in '" << typeName << "'! " << srcFile
                << ":" << srcLine;
    }
    if (definition != nullptr)
    {
        ASSERT_FALSE(isStringInFilePresent(filePath + ".cpp", definition))
                << "Method definition '" << definition << "' is present'" << typeName << "'! " << srcFile << ":"
                << srcLine;
    }
}

inline void assertStringInFilePresent(
        std::string_view filePath, std::string_view str, const char* srcFile, int srcLine)
{
    ASSERT_TRUE(isStringInFilePresent(filePath, str))
            << "String '" << str << "' is not present in file '" << filePath << "'! " << srcFile << ":"
            << srcLine;
}

inline void assertStringInFileNotPresent(
        std::string_view filePath, std::string_view str, const char* srcFile, int srcLine)
{
    ASSERT_FALSE(isStringInFilePresent(filePath, str))
            << "String '" << str << "' is present in file '" << filePath << "'! " << srcFile << ":" << srcLine;
}

} // namespace detail

} // namespace test_utils

#define ASSERT_METHOD_PRESENT(path, typeName, declaration, definition)                                         \
    test_utils::detail::assertMethodPresent(path, typeName, declaration, definition, __FILE__, __LINE__)

#define ASSERT_METHOD_NOT_PRESENT(path, typeName, declaration, definition)                                     \
    test_utils::detail::assertMethodNotPresent(path, typeName, declaration, definition, __FILE__, __LINE__)

#define ASSERT_STRING_IN_FILE_PRESENT(filePath, str)                                                           \
    test_utils::detail::assertStringInFilePresent(filePath, str, __FILE__, __LINE__)

#define ASSERT_STRING_IN_FILE_NOT_PRESENT(filePath, str)                                                       \
    test_utils::detail::assertStringInFileNotPresent(filePath, str, __FILE__, __LINE__)
