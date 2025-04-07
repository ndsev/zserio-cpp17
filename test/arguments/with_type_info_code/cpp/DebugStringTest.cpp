#include <fstream>

#include "gtest/gtest.h"
#include "zserio/DebugStringUtil.h"
#include "zserio/JsonWriter.h"
#include "zserio/RebindAlloc.h"
#include "zserio/ReflectableUtil.h"
#include "zserio/SerializeUtil.h"
#include "zserio/Walker.h"

#include "WithTypeInfoCodeCreator.h"

using namespace std::literals;

namespace with_type_info_code
{

using AllocatorType = WithTypeInfoCode::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

using BitBuffer = zserio::BasicBitBuffer<AllocatorType>;

using Walker = zserio::BasicWalker<AllocatorType>;
using JsonWriter = zserio::BasicJsonWriter<AllocatorType>;
using IWalkFilter = zserio::IBasicWalkFilter<AllocatorType>;
using ArrayLengthWalkFilter = zserio::BasicArrayLengthWalkFilter<AllocatorType>;
using DepthWalkFilter = zserio::BasicDepthWalkFilter<AllocatorType>;
using RegexWalkFilter = zserio::BasicRegexWalkFilter<AllocatorType>;
using AndWalkFilter = zserio::BasicAndWalkFilter<AllocatorType>;
using IReflectableDataPtr = zserio::IBasicReflectableDataPtr<AllocatorType>;
using JsonReader = zserio::BasicJsonReader<AllocatorType>;

class DebugStringTest : public ::testing::Test
{
protected:
    StringType getJsonNameWithArrayLengthFilter(size_t arrayLength)
    {
        return StringType("arguments/with_type_info_code/with_type_info_code_array_length_") +
                zserio::toString<AllocatorType>(arrayLength) + ".json";
    }

    void checkReflectable(const WithTypeInfoCode& withTypeInfoCode, const IReflectableDataPtr& readReflectable)
    {
        // check using objects
        const WithTypeInfoCode& readWithTypeInfoCode =
                zserio::ReflectableUtil::getValue<WithTypeInfoCode, AllocatorType>(readReflectable);
        ASSERT_EQ(withTypeInfoCode, readWithTypeInfoCode);

        // check using reflectables
        auto origReflectable = zserio::reflectable(withTypeInfoCode);
        ASSERT_TRUE(zserio::ReflectableUtil::equal<AllocatorType>(origReflectable, readReflectable));
    }

    void checkWithTypeInfoCodeArrayLength(const IReflectableDataPtr& reflectable, size_t maxArrayLength)
    {
        ASSERT_EQ(zserio::typeInfo<WithTypeInfoCode>().getSchemaName(),
                reflectable->getTypeInfo().getSchemaName());

        ASSERT_LE(reflectable->find("complexStruct.array")->size(), maxArrayLength);
        ASSERT_LE(reflectable->find("complexStruct.arrayWithLen")->size(), maxArrayLength);
        auto paramStructArrayReflectable = reflectable->find("complexStruct.paramStructArray");
        ASSERT_LE(paramStructArrayReflectable->size(), maxArrayLength);
        for (size_t i = 0; i < paramStructArrayReflectable->size(); ++i)
        {
            ASSERT_LE(paramStructArrayReflectable->at(i)->getField("array")->size(), maxArrayLength);
        }
        ASSERT_LE(reflectable->find("complexStruct.dynamicBitFieldArray")->size(), maxArrayLength);

        ASSERT_LE(reflectable->find("parameterizedStruct.array")->size(), maxArrayLength);
        ASSERT_LE(reflectable->find("templatedParameterizedStruct.array")->size(), maxArrayLength);
        ASSERT_LE(reflectable->find("externArray")->size(), maxArrayLength);
        ASSERT_LE(reflectable->find("bytesArray")->size(), maxArrayLength);
        ASSERT_LE(reflectable->find("implicitArray")->size(), maxArrayLength);
    }

    void checkWithTypeInfoCodeDepth0(const IReflectableDataPtr& reflectable)
    {
        ASSERT_EQ(zserio::typeInfo<WithTypeInfoCode>().getSchemaName(),
                reflectable->getTypeInfo().getSchemaName());

        // in C++ all fields are default constructed,
        // so just check that optionals are not set and arrays are empty
        ASSERT_EQ(0, reflectable->find("complexStruct.array")->size());
        ASSERT_FALSE(reflectable->find("complexStruct.arrayWithLen"));
        ASSERT_FALSE(reflectable->find("complexStruct.paramStructArray"));
        ASSERT_EQ(0, reflectable->find("complexStruct.dynamicBitFieldArray")->size());
        ASSERT_FALSE(reflectable->find("complexStruct.optionalEnum"));
        ASSERT_FALSE(reflectable->find("complexStruct.optionalBitmask"));
        ASSERT_FALSE(reflectable->find("complexStruct.optionalExtern"));
        ASSERT_FALSE(reflectable->find("complexStruct.optionalBytes"));
        ASSERT_EQ(0, reflectable->find("parameterizedStruct.array")->size());
        ASSERT_FALSE(reflectable->find("recursiveStruct.fieldRecursion"));
        ASSERT_EQ(0, reflectable->find("recursiveStruct.arrayRecursion")->size());
        ASSERT_EQ(0, reflectable->find("templatedParameterizedStruct.array")->size());
        ASSERT_EQ(0, reflectable->find("externArray")->size());
        ASSERT_EQ(0, reflectable->find("bytesArray")->size());
        ASSERT_EQ(0, reflectable->find("implicitArray")->size());
    }

    void checkWithTypeInfoCodeDepth1ArrayLength0(const IReflectableDataPtr& reflectable)
    {
        ASSERT_EQ(zserio::typeInfo<WithTypeInfoCode>().getSchemaName(),
                reflectable->getTypeInfo().getSchemaName());

        // in C++ all fields are default constructed,
        // so just check that optionals are not set and arrays are empty
        ASSERT_EQ(0, reflectable->find("complexStruct.array")->size());
        ASSERT_FALSE(reflectable->find("complexStruct.arrayWithLen"));
        ASSERT_FALSE(reflectable->find("complexStruct.paramStructArray"));
        ASSERT_EQ(0, reflectable->find("complexStruct.dynamicBitFieldArray")->size());
        ASSERT_FALSE(reflectable->find("complexStruct.optionalEnum"));
        ASSERT_FALSE(reflectable->find("complexStruct.optionalBitmask"));
        ASSERT_FALSE(reflectable->find("complexStruct.optionalExtern"));
        ASSERT_FALSE(reflectable->find("complexStruct.optionalBytes"));
        ASSERT_EQ(0, reflectable->find("parameterizedStruct.array")->size());
        ASSERT_FALSE(reflectable->find("recursiveStruct.fieldRecursion"));
        ASSERT_EQ(0, reflectable->find("recursiveStruct.arrayRecursion")->size());
        ASSERT_EQ(0, reflectable->find("templatedParameterizedStruct.array")->size());
        ASSERT_EQ(0, reflectable->find("externArray")->size());
        ASSERT_EQ(0, reflectable->find("bytesArray")->size());
        ASSERT_EQ(0, reflectable->find("implicitArray")->size());
    }

    void checkWithTypeInfoCodeRegex(const IReflectableDataPtr& reflectable)
    {
        ASSERT_EQ(zserio::typeInfo<WithTypeInfoCode>().getSchemaName(),
                reflectable->getTypeInfo().getSchemaName());

        ASSERT_NE(0, reflectable->find("simpleStruct.fieldOffset")->getUInt32());
        ASSERT_NE(0, reflectable->find("complexStruct.simpleStruct.fieldOffset")->getUInt32());
        ASSERT_NE(0, reflectable->find("complexStruct.anotherSimpleStruct.fieldOffset")->getUInt32());
    }

    static constexpr std::string_view JSON_NAME_WITH_OPTIONALS =
            "arguments/with_type_info_code/with_type_info_code_optionals.json";
    static constexpr std::string_view JSON_NAME_WITHOUT_OPTIONALS =
            "arguments/with_type_info_code/with_type_info_code.json";
    static constexpr std::string_view JSON_NAME_WITH_DEPTH0_FILTER =
            "arguments/with_type_info_code/with_type_info_code_depth0.json";
    static constexpr std::string_view JSON_NAME_WITH_DEPTH5_FILTER =
            "arguments/with_type_info_code/with_type_info_code_depth5.json";
    static constexpr std::string_view JSON_NAME_WITH_DEPTH1_ARRAY_LENGTH0_FILTER =
            "arguments/with_type_info_code/with_type_info_code_depth1_array_length0.json";
    static constexpr std::string_view JSON_NAME_WITH_REGEX_FILTER =
            "arguments/with_type_info_code/with_type_info_code_regex.json";

    static constexpr uint8_t JSON_INDENT = 4;
};

TEST_F(DebugStringTest, jsonWriterWithOptionals)
{
    // non-constant reflectable
    WithTypeInfoCode withTypeInfoCode;
    const bool createOptionals = true;
    fillWithTypeInfoCode(withTypeInfoCode, createOptionals);
    zserio::detail::initializeOffsets(zserio::View(withTypeInfoCode), 0);

    {
        zserio::toJsonFile(withTypeInfoCode, JSON_NAME_WITH_OPTIONALS);

        auto reflectablePtr = zserio::fromJsonFile(
                zserio::typeInfo<WithTypeInfoCode>(), JSON_NAME_WITH_OPTIONALS, AllocatorType());
        ASSERT_TRUE(reflectablePtr);
        checkReflectable(withTypeInfoCode, reflectablePtr);
    }
    {
        // constant reflectable
        const WithTypeInfoCode& withTypeInfoCodeConst = withTypeInfoCode;
        zserio::toJsonFile(withTypeInfoCodeConst, JSON_NAME_WITH_OPTIONALS);

        WithTypeInfoCode readWithTypeInfoCode =
                zserio::fromJsonFile<WithTypeInfoCode>(JSON_NAME_WITH_OPTIONALS, AllocatorType());

        ASSERT_EQ(withTypeInfoCode, readWithTypeInfoCode);
    }
}

TEST_F(DebugStringTest, jsonWriterWithoutOptionals)
{
    WithTypeInfoCode withTypeInfoCode;
    const bool createOptionals = false;
    fillWithTypeInfoCode(withTypeInfoCode, createOptionals);
    zserio::detail::initializeOffsets(zserio::View(withTypeInfoCode), 0);

    {
        zserio::toJsonFile(withTypeInfoCode, JSON_NAME_WITHOUT_OPTIONALS);

        WithTypeInfoCode readWithTypeInfoCode =
                zserio::fromJsonFile<WithTypeInfoCode>(JSON_NAME_WITHOUT_OPTIONALS, AllocatorType());

        ASSERT_EQ(withTypeInfoCode, readWithTypeInfoCode);
    }
    {
        // constant reflectable
        const WithTypeInfoCode& withTypeInfoCodeConst = withTypeInfoCode;
        zserio::toJsonFile(withTypeInfoCodeConst, JSON_NAME_WITHOUT_OPTIONALS);

        auto reflectablePtr = zserio::fromJsonFile(
                zserio::typeInfo<WithTypeInfoCode>(), JSON_NAME_WITHOUT_OPTIONALS, AllocatorType());
        ASSERT_TRUE(reflectablePtr);
        checkReflectable(withTypeInfoCode, reflectablePtr);
    }
}

TEST_F(DebugStringTest, jsonWriterWithArrayLengthFilter)
{
    WithTypeInfoCode withTypeInfoCode;
    fillWithTypeInfoCode(withTypeInfoCode);
    zserio::detail::initializeOffsets(zserio::View(withTypeInfoCode), 0);

    for (size_t i = 0; i < 11; ++i)
    {
        const StringType jsonFileName = getJsonNameWithArrayLengthFilter(i);

        {
            {
                std::ofstream jsonFile(jsonFileName.c_str(), std::ios::out | std::ios::trunc);
                JsonWriter jsonWriter(jsonFile, JSON_INDENT);
                ArrayLengthWalkFilter walkFilter(i);
                Walker walker(jsonWriter, walkFilter);
                walker.walk(zserio::reflectable(withTypeInfoCode));
            }
            {
                std::ifstream jsonFile(jsonFileName.c_str());
                JsonReader jsonReader(jsonFile);
                auto reflectablePtr = jsonReader.read(zserio::typeInfo<WithTypeInfoCode>());
                ASSERT_TRUE(reflectablePtr);
                checkWithTypeInfoCodeArrayLength(reflectablePtr, i);
            }
        }
        {
            {
                // constant reflectable
                const WithTypeInfoCode& withTypeInfoCodeConst = withTypeInfoCode;
                std::ofstream jsonFile(jsonFileName.c_str(), std::ios::out | std::ios::trunc);
                JsonWriter jsonWriter(jsonFile, JSON_INDENT);
                ArrayLengthWalkFilter walkFilter(i);
                Walker walker(jsonWriter, walkFilter);
                walker.walk(zserio::reflectable(withTypeInfoCodeConst));
            }
            {
                std::ifstream jsonFile(jsonFileName.c_str());
                JsonReader jsonReader(jsonFile);
                auto reflectablePtr = jsonReader.read(zserio::typeInfo<WithTypeInfoCode>());
                ASSERT_TRUE(reflectablePtr);
                checkWithTypeInfoCodeArrayLength(reflectablePtr, i);
            }
        }
    } // namespace with_type_info_code
}

TEST_F(DebugStringTest, jsonWriterWithDepth0Filter)
{
    WithTypeInfoCode withTypeInfoCode;
    fillWithTypeInfoCode(withTypeInfoCode);
    zserio::detail::initializeOffsets(zserio::View(withTypeInfoCode), 0);

    {
        {
            std::ofstream jsonFile(JSON_NAME_WITH_DEPTH0_FILTER.data(), std::ios::out | std::ios::trunc);
            JsonWriter jsonWriter(jsonFile, JSON_INDENT);
            DepthWalkFilter walkFilter(0);
            Walker walker(jsonWriter, walkFilter);
            walker.walk(zserio::reflectable(withTypeInfoCode));
        }
        {
            std::ifstream jsonFile(JSON_NAME_WITH_DEPTH0_FILTER.data());
            JsonReader jsonReader(jsonFile);
            auto reflectablePtr = jsonReader.read(zserio::typeInfo<WithTypeInfoCode>());
            ASSERT_TRUE(reflectablePtr);
            checkWithTypeInfoCodeDepth0(reflectablePtr);
        }
    }
    {
        // constant reflectable
        {
            const WithTypeInfoCode& withTypeInfoCodeConst = withTypeInfoCode;
            std::ofstream jsonFile(JSON_NAME_WITH_DEPTH0_FILTER.data(), std::ios::out | std::ios::trunc);
            JsonWriter jsonWriter(jsonFile, JSON_INDENT);
            DepthWalkFilter walkFilter(0);
            Walker walker(jsonWriter, walkFilter);
            walker.walk(zserio::reflectable(withTypeInfoCodeConst));
        }
        {
            std::ifstream jsonFile(JSON_NAME_WITH_DEPTH0_FILTER.data());
            JsonReader jsonReader(jsonFile);
            auto reflectablePtr = jsonReader.read(zserio::typeInfo<WithTypeInfoCode>());
            ASSERT_TRUE(reflectablePtr);
            checkWithTypeInfoCodeDepth0(reflectablePtr);
        }
    }
}

TEST_F(DebugStringTest, jsonWriterWithDepth1ArrayLength0Filter)
{
    WithTypeInfoCode withTypeInfoCode;
    fillWithTypeInfoCode(withTypeInfoCode);
    zserio::detail::initializeOffsets(zserio::View(withTypeInfoCode), 0);

    {
        {
            std::ofstream jsonFile(
                    JSON_NAME_WITH_DEPTH1_ARRAY_LENGTH0_FILTER.data(), std::ios::out | std::ios::trunc);
            JsonWriter jsonWriter(jsonFile, JSON_INDENT);
            DepthWalkFilter depthFilter(1);
            ArrayLengthWalkFilter arrayLengthFilter(0);
            AndWalkFilter walkFilter(
                    {std::ref<IWalkFilter>(depthFilter), std::ref<IWalkFilter>(arrayLengthFilter)});
            Walker walker(jsonWriter, walkFilter);
            walker.walk(zserio::reflectable(withTypeInfoCode));
        }
        {
            std::ifstream jsonFile(JSON_NAME_WITH_DEPTH1_ARRAY_LENGTH0_FILTER.data());
            JsonReader jsonReader(jsonFile);
            auto reflectablePtr = jsonReader.read(zserio::typeInfo<WithTypeInfoCode>());
            ASSERT_TRUE(reflectablePtr);
            checkWithTypeInfoCodeDepth1ArrayLength0(reflectablePtr);
        }
    }
    {
        {
            // constant reflectable
            const WithTypeInfoCode& withTypeInfoCodeConst = withTypeInfoCode;
            std::ofstream jsonFile(
                    JSON_NAME_WITH_DEPTH1_ARRAY_LENGTH0_FILTER.data(), std::ios::out | std::ios::trunc);
            JsonWriter jsonWriter(jsonFile, JSON_INDENT);
            DepthWalkFilter depthFilter(1);
            ArrayLengthWalkFilter arrayLengthFilter(0);
            AndWalkFilter walkFilter(
                    {std::ref<IWalkFilter>(depthFilter), std::ref<IWalkFilter>(arrayLengthFilter)});
            Walker walker(jsonWriter, walkFilter);
            walker.walk(zserio::reflectable(withTypeInfoCodeConst));
        }
        {
            std::ifstream jsonFile(JSON_NAME_WITH_DEPTH1_ARRAY_LENGTH0_FILTER.data());
            JsonReader jsonReader(jsonFile);
            auto reflectablePtr = jsonReader.read(zserio::typeInfo<WithTypeInfoCode>());
            ASSERT_TRUE(reflectablePtr);
            checkWithTypeInfoCodeDepth1ArrayLength0(reflectablePtr);
        }
    }
}

TEST_F(DebugStringTest, jsonWriterWithDepth5Filter)
{
    WithTypeInfoCode withTypeInfoCode;
    fillWithTypeInfoCode(withTypeInfoCode);
    zserio::detail::initializeOffsets(zserio::View(withTypeInfoCode), 0);

    {
        {
            std::ofstream jsonFile(JSON_NAME_WITH_DEPTH5_FILTER.data(), std::ios::out | std::ios::trunc);
            JsonWriter jsonWriter(jsonFile, JSON_INDENT);
            DepthWalkFilter walkFilter(5);
            Walker walker(jsonWriter, walkFilter);
            walker.walk(zserio::reflectable(withTypeInfoCode));
        }
        {
            std::ifstream jsonFile(JSON_NAME_WITH_DEPTH5_FILTER.data());
            JsonReader jsonReader(jsonFile);
            auto reflectablePtr = jsonReader.read(zserio::typeInfo<WithTypeInfoCode>());
            ASSERT_TRUE(reflectablePtr);
            checkReflectable(withTypeInfoCode, reflectablePtr);
        }
    }
    {
        {
            // constant reflectable
            const WithTypeInfoCode& withTypeInfoCodeConst = withTypeInfoCode;
            std::ofstream jsonFile(JSON_NAME_WITH_DEPTH5_FILTER.data(), std::ios::out | std::ios::trunc);
            JsonWriter jsonWriter(jsonFile, JSON_INDENT);
            DepthWalkFilter walkFilter(5);
            Walker walker(jsonWriter, walkFilter);
            walker.walk(zserio::reflectable(withTypeInfoCodeConst));
        }
        {
            std::ifstream jsonFile(JSON_NAME_WITH_DEPTH5_FILTER.data());
            JsonReader jsonReader(jsonFile);
            auto reflectablePtr = jsonReader.read(zserio::typeInfo<WithTypeInfoCode>());
            ASSERT_TRUE(reflectablePtr);
            checkReflectable(withTypeInfoCode, reflectablePtr);
        }
    }
}

TEST_F(DebugStringTest, jsonWriterWithRegexFilter)
{
    WithTypeInfoCode withTypeInfoCode;
    const bool createOptionals = false;
    fillWithTypeInfoCode(withTypeInfoCode, createOptionals);
    zserio::detail::initializeOffsets(zserio::View(withTypeInfoCode), 0);

    {
        {
            std::ofstream jsonFile(JSON_NAME_WITH_REGEX_FILTER.data(), std::ios::out | std::ios::trunc);
            JsonWriter jsonWriter(jsonFile, JSON_INDENT);
            RegexWalkFilter walkFilter(".*fieldOffset");
            Walker walker(jsonWriter, walkFilter);
            walker.walk(zserio::reflectable(withTypeInfoCode));
        }
        {
            std::ifstream jsonFile(JSON_NAME_WITH_REGEX_FILTER.data());
            JsonReader jsonReader(jsonFile);
            auto reflectablePtr = jsonReader.read(zserio::typeInfo<WithTypeInfoCode>());
            ASSERT_TRUE(reflectablePtr);
            checkWithTypeInfoCodeRegex(reflectablePtr);
        }
    }
    {
        {
            // constant reflectable
            const WithTypeInfoCode& withTypeInfoCodeConst = withTypeInfoCode;
            std::ofstream jsonFile(JSON_NAME_WITH_REGEX_FILTER.data(), std::ios::out | std::ios::trunc);
            JsonWriter jsonWriter(jsonFile, JSON_INDENT);
            RegexWalkFilter walkFilter(".*fieldOffset");
            Walker walker(jsonWriter, walkFilter);
            walker.walk(zserio::reflectable(withTypeInfoCodeConst));
        }
        {
            std::ifstream jsonFile(JSON_NAME_WITH_REGEX_FILTER.data());
            JsonReader jsonReader(jsonFile);
            auto reflectablePtr = jsonReader.read(zserio::typeInfo<WithTypeInfoCode>());
            ASSERT_TRUE(reflectablePtr);
            checkWithTypeInfoCodeRegex(reflectablePtr);
        }
    }
}

} // namespace with_type_info_code
