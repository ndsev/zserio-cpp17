#include <fstream>
#include <regex>
#include <string>

#include "gtest/gtest.h"

namespace test_utils
{

namespace detail
{

inline bool isStringPresentInFile(std::string_view fileName, std::string_view str)
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

inline bool matchRegexInFile(std::string_view fileName, std::string_view regexStr)
{
    std::regex regex(std::string{regexStr});
    std::ifstream file(std::string(fileName).data());
    bool isPresent = false;
    std::string line;
    while (std::getline(file, line))
    {
        if (std::regex_search(line, regex))
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
        ASSERT_TRUE(isStringPresentInFile(filePath + ".h", declaration))
                << "Method declaration '" << declaration << "' is not present in '" << typeName << "'! "
                << srcFile << ":" << srcLine;
    }
    if (definition != nullptr)
    {
        ASSERT_TRUE(isStringPresentInFile(filePath + ".cpp", definition))
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
        ASSERT_FALSE(isStringPresentInFile(filePath + ".h", declaration))
                << "Method declaration '" << declaration << "' is present in '" << typeName << "'! " << srcFile
                << ":" << srcLine;
    }
    if (definition != nullptr)
    {
        ASSERT_FALSE(isStringPresentInFile(filePath + ".cpp", definition))
                << "Method definition '" << definition << "' is present in '" << typeName << "'! " << srcFile
                << ":" << srcLine;
    }
}

inline void assertStringInFilePresent(
        std::string_view filePath, std::string_view str, const char* srcFile, int srcLine)
{
    ASSERT_TRUE(isStringPresentInFile(filePath, str))
            << "String '" << str << "' is not present in file '" << filePath << "'! " << srcFile << ":"
            << srcLine;
}

inline void assertStringInFileNotPresent(
        std::string_view filePath, std::string_view str, const char* srcFile, int srcLine)
{
    ASSERT_FALSE(isStringPresentInFile(filePath, str))
            << "String '" << str << "' is present in file '" << filePath << "'! " << srcFile << ":" << srcLine;
}

inline void assertMethodRegexMatch(std::string_view path, std::string_view typeName,
        const char* declarationRegex, const char* definitionRegex, const char* srcFile, int srcLine)
{
    const std::string filePath = std::string(path) + std::string(typeName);
    if (declarationRegex != nullptr)
    {
        ASSERT_TRUE(matchRegexInFile(filePath + ".h", declarationRegex))
                << "Method declaration '" << declarationRegex << "' does not match in '" << typeName << "'! "
                << srcFile << ":" << srcLine;
    }
    if (definitionRegex != nullptr)
    {
        ASSERT_TRUE(matchRegexInFile(filePath + ".cpp", definitionRegex))
                << "Method definition '" << definitionRegex << "' does not match in '" << typeName << "'! "
                << srcFile << ":" << srcLine;
    }
}

inline void assertMethodRegexNoMatch(std::string_view path, std::string_view typeName,
        const char* declarationRegex, const char* definitionRegex, const char* srcFile, int srcLine)
{
    const std::string filePath = std::string(path) + std::string(typeName);
    if (declarationRegex != nullptr)
    {
        ASSERT_FALSE(matchRegexInFile(filePath + ".h", declarationRegex))
                << "Method declaration '" << declarationRegex << "' matches in '" << typeName << "'! "
                << srcFile << ":" << srcLine;
    }
    if (definitionRegex != nullptr)
    {
        ASSERT_FALSE(matchRegexInFile(filePath + ".cpp", definitionRegex))
                << "Method definition '" << definitionRegex << "' matches in '" << typeName << "'! " << srcFile
                << ":" << srcLine;
    }
}

inline void assertRegexMatchInFile(
        std::string_view filePath, std::string_view regexStr, const char* srcFile, int srcLine)
{
    ASSERT_TRUE(matchRegexInFile(filePath, regexStr))
            << "Regex '" << regexStr << "' doesn't match in file '" << filePath << "'! " << srcFile << ":"
            << srcLine;
}

inline void assertRegexNoMatchInFile(
        std::string_view filePath, std::string_view regexStr, const char* srcFile, int srcLine)
{
    ASSERT_FALSE(matchRegexInFile(filePath, regexStr))
            << "Regex '" << regexStr << "' matches in file '" << filePath << "'! " << srcFile << ":" << srcLine;
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

#define ASSERT_METHOD_REGEX_MATCH(path, typeName, declarationRegex, definitionRegex)                           \
    test_utils::detail::assertMethodRegexMatch(                                                                \
            path, typeName, declarationRegex, definitionRegex, __FILE__, __LINE__)

#define ASSERT_METHOD_REGEX_NO_MATCH(path, typeName, declarationRegex, definitionRegex)                        \
    test_utils::detail::assertMethodRegexNoMatch(                                                              \
            path, typeName, declarationRegex, definitionRegex, __FILE__, __LINE__)

#define ASSERT_REGEX_MATCH_IN_FILE(filePath, regexStr)                                                         \
    test_utils::detail::assertRegexMatchInFile(filePath, regexStr, __FILE__, __LINE__)

#define ASSERT_REGEX_NO_MATCH_IN_FILE(filePath, regexStr)                                                      \
    test_utils::detail::assertRegexNoMatchInFile(filePath, regexStr, __FILE__, __LINE__)
