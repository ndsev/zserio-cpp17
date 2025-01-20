#include <string>

#include "gtest/gtest.h"
#include "member_access/access_within_type/Message.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"
#include "zserio/StringConvertUtil.h"

namespace member_access
{
namespace access_within_type
{

using AllocatorType = Message::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class AccessWithinTypeTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer, uint16_t numSentences)
    {
        writer.writeUnsignedBits32(VERSION_VALUE, 16);
        writer.writeUnsignedBits32(numSentences, 16);

        const uint16_t numStrings = numSentences;
        for (uint16_t i = 0; i < numStrings; ++i)
        {
            writer.writeString(getSentence(i));
        }
    }

    static void fillData(Message& message, uint16_t numSentences, bool wrongArrayLength)
    {
        Header& header = message.header;
        header.version = VERSION_VALUE;
        header.numSentences = numSentences;

        const uint16_t numStrings = (wrongArrayLength) ? static_cast<uint16_t>(numSentences - 1) : numSentences;
        VectorType<StringType>& sentences = message.sentences;
        sentences.reserve(numStrings);
        for (uint16_t i = 0; i < numStrings; ++i)
        {
            sentences.push_back(getSentence(i));
        }
    }

    static StringType getSentence(uint16_t index)
    {
        return StringType("This is sentence #") + zserio::toString<AllocatorType>(index);
    }

    static constexpr uint16_t VERSION_VALUE = 0xAB;
};

TEST_F(AccessWithinTypeTest, read)
{
    const uint16_t numSentences = 10;
    const bool wrongArrayLength = false;
    Message data;
    fillData(data, numSentences, wrongArrayLength);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, numSentences), data);
}

TEST_F(AccessWithinTypeTest, write)
{
    const uint16_t numSentences = 13;
    const bool wrongArrayLength = false;
    Message data;
    fillData(data, numSentences, wrongArrayLength);

    test_utils::writeReadTest(data);
}

TEST_F(AccessWithinTypeTest, writeWrongArrayLength)
{
    const uint16_t numSentences = 13;
    const bool wrongArrayLength = true;
    Message data;
    fillData(data, numSentences, wrongArrayLength);

    ASSERT_THROW(zserio::detail::validate(zserio::View(data)), zserio::CppRuntimeException);
    ASSERT_THROW(test_utils::writeReadTest(data), zserio::CppRuntimeException);
}

} // namespace access_within_type
} // namespace member_access
