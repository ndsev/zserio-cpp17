#include <fstream>
#include <string>

#include "gtest/gtest.h"
#include "test_utils/WriteReadTest.h"

#include "AmalgamationStructure.h"

namespace default_package_amalgamation
{

class DefaultPackageAmalgamationTest : public ::testing::Test
{
protected:
    bool isFilePresent(const char* fileName)
    {
        std::ifstream file(fileName);
        const bool isPresent = file.good();
        file.close();

        return isPresent;
    }

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

TEST_F(DefaultPackageAmalgamationTest, checkDefaultPackage)
{
    ASSERT_TRUE(isFilePresent(
            "arguments/with_sources_amalgamation/gen_default_package/DefaultPackageAmalgamation.cpp"));
    ASSERT_TRUE(
            isFilePresent("arguments/with_sources_amalgamation/gen_default_package/AmalgamationStructure.h"));

    ASSERT_FALSE(
            isFilePresent("arguments/with_sources_amalgamation/gen_default_package/AmalgamationStructure.cpp"));
}

TEST_F(DefaultPackageAmalgamationTest, readWrite)
{
    AmalgamationStructure structure{4, 0x0f};
    test_utils::writeReadTest(structure);
}

} // namespace default_package_amalgamation
