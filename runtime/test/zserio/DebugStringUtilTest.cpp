#include <array>
#include <sstream>
#include <string_view>

#include "gtest/gtest.h"
#include "test_object/polymorphic_allocator/DebugStringObject.h"
#include "test_object/polymorphic_allocator/DebugStringParamObject.h"
#include "test_object/std_allocator/DebugStringObject.h"
#include "test_object/std_allocator/DebugStringParamObject.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/DebugStringUtil.h"
#include "zserio/ReflectableData.h"
#include "zserio/pmr/PropagatingPolymorphicAllocator.h"

using StdDebugStringObject = test_object::std_allocator::DebugStringObject;
using StdDebugStringParamObject = test_object::std_allocator::DebugStringParamObject;
using PmrDebugStringObject = test_object::polymorphic_allocator::DebugStringObject;
using PmrDebugStringParamObject = test_object::polymorphic_allocator::DebugStringParamObject;

using StdAlloc = std::allocator<uint8_t>;
using PmrAlloc = zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

namespace zserio
{

TEST(DebugStringUtilTest, toJsonStreamDefault)
{
    std::ostringstream stream;
    StdDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream);
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStreamDefaultWithAlloc)
{
    std::ostringstream stream;
    const StdDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream, StdAlloc());
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStreamDefaultWithPolymorphicAlloc)
{
    std::ostringstream stream;
    const PmrDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream, PmrAlloc());
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStreamIndent2)
{
    std::ostringstream stream;
    const StdDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream, 2);
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStreamIndent2WithAlloc)
{
    std::ostringstream stream;
    const StdDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream, 2, StdAlloc());
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStreamIndent2WithPolymorphicAlloc)
{
    std::ostringstream stream;
    const PmrDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream, 2, PmrAlloc());
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStreamFilter)
{
    std::ostringstream stream;
    const StdDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream, DepthWalkFilter(0));
    ASSERT_EQ("{\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStreamFilterWithAlloc)
{
    std::ostringstream stream;
    const StdDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream, DefaultWalkFilter(), StdAlloc());
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStreamFilterWithPolymorphicAlloc)
{
    std::ostringstream stream;
    const PmrDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream, BasicDefaultWalkFilter<PmrAlloc>(), PmrAlloc());
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStreamIndent2Filter)
{
    std::ostringstream stream;
    const StdDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream, 2, DefaultWalkFilter());
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStreamIndent2FilterWithAlloc)
{
    std::ostringstream stream;
    const StdDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream, 2, DepthWalkFilter(0), StdAlloc());
    ASSERT_EQ("{\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStreamIndent2FilterWithPolymorphicAlloc)
{
    std::ostringstream stream;
    const PmrDebugStringObject debugStringObject;
    toJsonStream(debugStringObject, stream, 2, BasicDepthWalkFilter<PmrAlloc>(0), PmrAlloc());
    ASSERT_EQ("{\n}", stream.str());
}

TEST(DebugStringUtilTest, toJsonStringDefault)
{
    const StdDebugStringObject debugStringObject;
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", toJsonString(debugStringObject));
}

TEST(DebugStringUtilTest, toJsonStringDefaultWithAlloc)
{
    const StdDebugStringObject debugStringObject;
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", toJsonString(debugStringObject, StdAlloc()));
}

TEST(DebugStringUtilTest, toJsonStringDefaultWithPolymorphicAlloc)
{
    const PmrDebugStringObject debugStringObject;
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", toJsonString(debugStringObject, PmrAlloc()));
}

TEST(DebugStringUtilTest, toJsonStringIndent2)
{
    const StdDebugStringObject debugStringObject;
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", toJsonString(debugStringObject, 2));
}

TEST(DebugStringUtilTest, toJsonStringIndent2WithAlloc)
{
    const StdDebugStringObject debugStringObject;
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", toJsonString(debugStringObject, 2, StdAlloc()));
}

TEST(DebugStringUtilTest, toJsonStringIndent2WithPolymorphicAlloc)
{
    const PmrDebugStringObject debugStringObject;
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", toJsonString(debugStringObject, 2, PmrAlloc()));
}

TEST(DebugStringUtilTest, toJsonStringFilter)
{
    const StdDebugStringObject debugStringObject;
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", toJsonString(debugStringObject, DefaultWalkFilter()));
}

TEST(DebugStringUtilTest, toJsonStringFilterWithAlloc)
{
    const StdDebugStringObject debugStringObject;
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", toJsonString(debugStringObject, DefaultWalkFilter(), StdAlloc()));
}

TEST(DebugStringUtilTest, toJsonStringFilterWithPolymorphicAlloc)
{
    const PmrDebugStringObject debugStringObject;
    ASSERT_EQ("{\n    \"text\": \"test\"\n}",
            toJsonString(debugStringObject, BasicDefaultWalkFilter<PmrAlloc>(), PmrAlloc()));
}

TEST(DebugStringUtilTest, toJsonStringIndent2Filter)
{
    const StdDebugStringObject debugStringObject;
    ASSERT_EQ("{\n}", toJsonString(debugStringObject, 2, DepthWalkFilter(0)));
}

TEST(DebugStringUtilTest, toJsonStringIndent2FilterWithAlloc)
{
    const StdDebugStringObject debugStringObject;
    ASSERT_EQ(
            "{\n  \"text\": \"test\"\n}", toJsonString(debugStringObject, 2, DefaultWalkFilter(), StdAlloc()));
}

TEST(DebugStringUtilTest, toJsonStringIndent2FilterWithPolymorphicAlloc)
{
    const PmrDebugStringObject debugStringObject;
    ASSERT_EQ("{\n  \"text\": \"test\"\n}",
            toJsonString(debugStringObject, 2, BasicDefaultWalkFilter<PmrAlloc>(), PmrAlloc()));
}

TEST(DebugStringUtilTest, toJsonFileDefault)
{
    const StdDebugStringObject debugStringObject;
    const std::string fileName = "DebugStringUtilTest_toJsonFileDefault.json";
    toJsonFile(debugStringObject, fileName);
    ASSERT_THROW(toJsonFile(debugStringObject, ""), CppRuntimeException);

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", stringStream.str());
}

TEST(DebugStringUtilTest, toJsonFileDefaultWithAlloc)
{
    const StdDebugStringObject debugStringObject;
    const std::string fileName = "DebugStringUtilTest_toJsonFileDefaultWithAlloc.json";
    toJsonFile(debugStringObject, fileName, StdAlloc());

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", stringStream.str());
}

TEST(DebugStringUtilTest, toJsonFileDefaultWithPolymorphicAlloc)
{
    const PmrDebugStringObject debugStringObject;
    const BasicString<RebindAlloc<PmrAlloc, char>> fileName =
            "DebugStringUtilTest_toJsonFileDefaultWithPolymorphicAlloc.json";
    toJsonFile(debugStringObject, fileName, PmrAlloc());

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", stringStream.str());
}

TEST(DebugStringUtilTest, toJsonFileIndent2)
{
    const StdDebugStringObject debugStringObject;
    const std::string fileName = "DebugStringUtilTest_toJsonFileIndent2.json";
    toJsonFile(debugStringObject, fileName, 2);

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", stringStream.str());
}

TEST(DebugStringUtilTest, toJsonFileIndent2WithAlloc)
{
    const StdDebugStringObject debugStringObject;
    const std::string fileName = "DebugStringUtilTest_toJsonFileIndent2WithAlloc.json";
    toJsonFile(debugStringObject, fileName, 2, StdAlloc());

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", stringStream.str());
}

TEST(DebugStringUtilTest, toJsonFileIndent2WithPolymorphicAlloc)
{
    const PmrDebugStringObject debugStringObject;
    const BasicString<RebindAlloc<PmrAlloc, char>> fileName =
            "DebugStringUtilTest_toJsonFileIndent2WithPolymorphicAlloc.json";
    toJsonFile(debugStringObject, fileName, 2, PmrAlloc());

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", stringStream.str());
}

TEST(DebugStringUtilTest, toJsonFileFilter)
{
    const StdDebugStringObject debugStringObject;
    const std::string fileName = "DebugStringUtilTest_toJsonFileFilter.json";
    toJsonFile(debugStringObject, fileName, DefaultWalkFilter());

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", stringStream.str());
}

TEST(DebugStringUtilTest, toJsonFileFilterWithAlloc)
{
    const StdDebugStringObject debugStringObject;
    DefaultWalkFilter defaultWalkFilter;
    const std::string fileName = "DebugStringUtilTest_toJsonFileFilterWithAlloc.json";
    toJsonFile(debugStringObject, fileName, defaultWalkFilter, StdAlloc());

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", stringStream.str());
}

TEST(DebugStringUtilTest, toJsonFileFilterWithPolymorphicAlloc)
{
    const PmrDebugStringObject debugStringObject;
    BasicDefaultWalkFilter<PmrAlloc> defaultWalkFilter;
    const BasicString<RebindAlloc<PmrAlloc, char>> fileName =
            "DebugStringUtilTest_toJsonFileFilterWithPolymorphicAlloc.json";
    toJsonFile(debugStringObject, fileName, defaultWalkFilter, PmrAlloc());

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n    \"text\": \"test\"\n}", stringStream.str());
}

TEST(DebugStringUtilTest, toJsonFileIndent2Filter)
{
    const StdDebugStringObject debugStringObject;
    DepthWalkFilter depthWalkFilter(0);
    const std::string fileName = "DebugStringUtilTest_toJsonFileIndent2Filter.json";
    toJsonFile(debugStringObject, fileName, 2, depthWalkFilter);

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n}", stringStream.str());
}

TEST(DebugStringUtilTest, toJsonFileIndent2FilterWithAlloc)
{
    const StdDebugStringObject debugStringObject;
    DefaultWalkFilter defaultWalkFilter;
    const std::string fileName = "DebugStringUtilTest_toJsonFileIndent2FilterWithAlloc.json";
    toJsonFile(debugStringObject, fileName, 2, defaultWalkFilter, StdAlloc());

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", stringStream.str());
}

TEST(DebugStringUtilTest, toJsonFileIndent2FilterWithPolymorphicAlloc)
{
    const PmrDebugStringObject debugStringObject;
    BasicDefaultWalkFilter<PmrAlloc> defaultWalkFilter;
    const BasicString<RebindAlloc<PmrAlloc, char>> fileName =
            "DebugStringUtilTest_toJsonFileIndent2FilterWithPolymorphicAlloc.json";
    toJsonFile(debugStringObject, fileName, 2, defaultWalkFilter, PmrAlloc());

    std::ifstream stream(fileName.c_str());
    std::stringstream stringStream;
    stringStream << stream.rdbuf();
    ASSERT_EQ("{\n  \"text\": \"test\"\n}", stringStream.str());
}

TEST(DebugStringUtilTest, fromJsonStreamTypeInfo)
{
    std::istringstream stream("{\n  \"text\": \"something\"\n}");
    IReflectableDataPtr reflectable = fromJsonStream(typeInfo<StdDebugStringObject>(), stream);
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonStreamParameterizedTypeInfo)
{
    std::istringstream stream("{\n  \"text\": \"something\"\n}");
    IReflectableDataPtr reflectable = fromJsonStream(typeInfo<StdDebugStringParamObject>(), stream);
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonStreamTypeInfoWithAlloc)
{
    std::istringstream stream("{\n  \"text\": \"something\"\n}");
    IReflectableDataPtr reflectable = fromJsonStream(typeInfo<StdDebugStringObject>(), stream, StdAlloc());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonStreamTypeInfoWithPolymorphicAllocDefault)
{
    std::istringstream stream("{\n  \"text\": \"something\"\n}");
    IBasicReflectableDataPtr<PmrAlloc> reflectable = fromJsonStream(typeInfo<PmrDebugStringObject>(), stream);
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonStreamTypeInfoWithPolymorphicAlloc)
{
    std::istringstream stream("{\n  \"text\": \"something\"\n}");
    IBasicReflectableDataPtr<PmrAlloc> reflectable =
            fromJsonStream(typeInfo<PmrDebugStringObject>(), stream, PmrAlloc());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonStreamType)
{
    std::istringstream stream("{\n  \"text\": \"something\"\n}");
    StdDebugStringObject debugStringObject = fromJsonStream<StdDebugStringObject>(stream);

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonStreamParameterizedType)
{
    std::istringstream stream("{\n  \"text\": \"something\"\n}");
    StdDebugStringParamObject debugStringParamObject = fromJsonStream<StdDebugStringParamObject>(stream);

    ASSERT_EQ("something", debugStringParamObject.text);
}

TEST(DebugStringUtilTest, fromJsonStreamTypeWithAlloc)
{
    std::istringstream stream("{\n  \"text\": \"something\"\n}");
    StdDebugStringObject debugStringObject = fromJsonStream<StdDebugStringObject>(stream, StdAlloc());

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonStreamTypeWithPolymorphicAllocDefault)
{
    std::istringstream stream("{\n  \"text\": \"something\"\n}");
    PmrDebugStringObject debugStringObject = fromJsonStream<PmrDebugStringObject>(stream);

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonStreamTypeWithPolymorphicAlloc)
{
    std::istringstream stream("{\n  \"text\": \"something\"\n}");
    PmrDebugStringObject debugStringObject = fromJsonStream<PmrDebugStringObject>(stream, PmrAlloc());

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonStringTypeInfo)
{
    std::string jsonString("{\n  \"text\": \"something\"\n}");
    IReflectableDataPtr reflectable = fromJsonString(typeInfo<StdDebugStringObject>(), jsonString);
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonStringParameterizedTypeInfo)
{
    std::string jsonString("{\n  \"text\": \"something\"\n}");
    IReflectableDataPtr reflectable = fromJsonString(typeInfo<StdDebugStringParamObject>(), jsonString);
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonStringTypeInfoWithAlloc)
{
    std::string jsonString("{\n  \"text\": \"something\"\n}");
    IReflectableDataPtr reflectable = fromJsonString(typeInfo<StdDebugStringObject>(), jsonString, StdAlloc());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonStringTypeInfoWithPolymorphicAllocDefault)
{
    BasicString<RebindAlloc<PmrAlloc, char>> jsonString("{\n  \"text\": \"something\"\n}");
    IBasicReflectableDataPtr<PmrAlloc> reflectable =
            fromJsonString(typeInfo<PmrDebugStringObject>(), jsonString);
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonStringTypeInfoWithPolymorphicAlloc)
{
    BasicString<RebindAlloc<PmrAlloc, char>> jsonString("{\n  \"text\": \"something\"\n}");
    IBasicReflectableDataPtr<PmrAlloc> reflectable =
            fromJsonString(typeInfo<PmrDebugStringObject>(), jsonString, PmrAlloc());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonStringType)
{
    std::string jsonString("{\n  \"text\": \"something\"\n}");
    StdDebugStringObject debugStringObject = fromJsonString<StdDebugStringObject>(jsonString);

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonStringParameterizedType)
{
    std::string jsonString("{\n  \"text\": \"something\"\n}");
    StdDebugStringParamObject debugStringParamObject = fromJsonString<StdDebugStringParamObject>(jsonString);

    ASSERT_EQ("something", debugStringParamObject.text);
}

TEST(DebugStringUtilTest, fromJsonStringTypeWithAlloc)
{
    std::string jsonString("{\n  \"text\": \"something\"\n}");
    StdDebugStringObject debugStringObject = fromJsonString<StdDebugStringObject>(jsonString, StdAlloc());

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonStringTypeWithPolymorphicAllocDefault)
{
    BasicString<RebindAlloc<PmrAlloc, char>> jsonString("{\n  \"text\": \"something\"\n}");
    PmrDebugStringObject debugStringObject = fromJsonString<PmrDebugStringObject>(jsonString);

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonStringTypeWithPolymorphicAlloc)
{
    BasicString<RebindAlloc<PmrAlloc, char>> jsonString("{\n  \"text\": \"something\"\n}");
    PmrDebugStringObject debugStringObject = fromJsonString<PmrDebugStringObject>(jsonString, PmrAlloc());

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonFileTypeInfo)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileTypeInfo.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    IReflectableDataPtr reflectable = fromJsonFile(typeInfo<StdDebugStringObject>(), fileName);
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());

    ASSERT_THROW(fromJsonFile(typeInfo<StdDebugStringObject>(), ""), CppRuntimeException);
}

TEST(DebugStringUtilTest, fromJsonFileParameterizedTypeInfo)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileParameterizedTypeInfo.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    IReflectableDataPtr reflectable = fromJsonFile(typeInfo<StdDebugStringParamObject>(), fileName);
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonFileTypeInfoWithAlloc)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileTypeInfoWithAlloc.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    IReflectableDataPtr reflectable = fromJsonFile(typeInfo<StdDebugStringObject>(), fileName, StdAlloc());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonFileTypeInfoWithPolymorphicAllocDefault)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileTypeInfoWithPolymorphicAllocDefault.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    IBasicReflectableDataPtr<PmrAlloc> reflectable = fromJsonFile(typeInfo<PmrDebugStringObject>(), fileName);
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonFileTypeInfoWithPolymorphicAlloc)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileTypeInfoWithPolymorphicAlloc.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    IBasicReflectableDataPtr<PmrAlloc> reflectable =
            fromJsonFile(typeInfo<PmrDebugStringObject>(), fileName, PmrAlloc());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonFileParameterizedTypeInfoWithPolymorphicAlloc)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileParameterizedTypeInfoWithPolymorphicAlloc.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    IBasicReflectableDataPtr<PmrAlloc> reflectable =
            fromJsonFile(typeInfo<PmrDebugStringParamObject>(), fileName, PmrAlloc());
    ASSERT_TRUE(reflectable);

    ASSERT_EQ("something", reflectable->getField("text")->getStringView());
}

TEST(DebugStringUtilTest, fromJsonFileType)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileType.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    StdDebugStringObject debugStringObject = fromJsonFile<StdDebugStringObject>(fileName);

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonFileParameterizedType)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileParameterizedType.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    StdDebugStringParamObject debugStringParamObject = fromJsonFile<StdDebugStringParamObject>(fileName);

    ASSERT_EQ("something", debugStringParamObject.text);
}

TEST(DebugStringUtilTest, fromJsonFileTypeWithAlloc)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileTypeWithAlloc.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    StdDebugStringObject debugStringObject = fromJsonFile<StdDebugStringObject>(fileName, StdAlloc());

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonFileTypeWithPolymorphicAllocDefault)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileTypeWithPolymorphicAllocDefault.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    PmrDebugStringObject debugStringObject = fromJsonFile<PmrDebugStringObject>(fileName);

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonFileTypeWithPolymorphicAlloc)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileTypeWithPolymorphicAlloc.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    PmrDebugStringObject debugStringObject = fromJsonFile<PmrDebugStringObject>(fileName, PmrAlloc());

    ASSERT_EQ("something", debugStringObject.text);
}

TEST(DebugStringUtilTest, fromJsonFileParameterizedTypeWithPolymorphicAlloc)
{
    const char* fileName = "DebugStringUtilTest_fromJsonFileParameteriezedTypeWithPolymorphicAlloc.json";
    {
        std::ofstream stream(fileName, std::ofstream::out | std::ofstream::trunc);
        stream << "{\n  \"text\": \"something\"\n}";
    }

    PmrDebugStringParamObject debugStringParamObject =
            fromJsonFile<PmrDebugStringParamObject>(fileName, PmrAlloc());

    ASSERT_EQ("something", debugStringParamObject.text);
}

} // namespace zserio
