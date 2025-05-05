#include <fstream>

#include "gif/GifFile.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SerializeUtil.h"

namespace gif
{

using AllocatorType = GifFile::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;
using BitBufferType = zserio::BasicBitBuffer<AllocatorType>;

class GifTest : public ::testing::Test
{
protected:
    bool readFileToBuffer(const StringType& fileName, VectorType<uint8_t>& buffer)
    {
        std::ifstream inputStream(fileName.c_str(), std::ios::binary);
        if (!inputStream)
        {
            return false;
        }

        inputStream.seekg(0, inputStream.end);
        const size_t fileSize = static_cast<size_t>(inputStream.tellg());
        inputStream.seekg(0, inputStream.beg);
        buffer.resize(fileSize);
        inputStream.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
        const bool result = static_cast<bool>((inputStream));
        inputStream.close();

        return result;
    }

    void convertUInt8ArrayToString(const VectorType<zserio::UInt8>& array, StringType& outputString)
    {
        for (zserio::UInt8 element : array)
        {
            outputString.append(1, static_cast<char>(element));
        }
    }
};

TEST_F(GifTest, OnePixGif)
{
    const StringType onePixGifFileName("others/gif/data/1pix.gif");
    VectorType<uint8_t> buffer;
    ASSERT_TRUE(readFileToBuffer(onePixGifFileName, buffer));
    GifFile data;
    zserio::deserialize(BitBufferType(buffer), data);

    StringType fileFormat;
    convertUInt8ArrayToString(data.signature.format, fileFormat);
    const StringType expectedGifFileFormat("GIF");
    ASSERT_EQ(expectedGifFileFormat, fileFormat);

    StringType fileVersion;
    convertUInt8ArrayToString(data.signature.version, fileVersion);
    const StringType expectedGifFileVersion("89a");
    ASSERT_EQ(expectedGifFileVersion, fileVersion);

    const screen_descriptor::ScreenDescriptor& screenDescriptor = data.screen;
    const uint16_t expectedGifScreenWidth = 256;
    ASSERT_EQ(expectedGifScreenWidth, screenDescriptor.width);

    const uint16_t expectedGifScreenHeight = 256;
    ASSERT_EQ(expectedGifScreenHeight, screenDescriptor.height);

    const uint8_t expectedGifScreenBgColor = 255;
    ASSERT_EQ(expectedGifScreenBgColor, screenDescriptor.bgColor);

    const uint8_t expectedScreenBitsOfColorResolution = 7;
    ASSERT_EQ(expectedScreenBitsOfColorResolution, screenDescriptor.bitsOfColorResolution);

    const uint8_t expectedScreenBitsPerPixel = 7;
    ASSERT_EQ(expectedScreenBitsPerPixel, screenDescriptor.bitsPerPixel);
}

} // namespace gif
