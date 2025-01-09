#include <fstream>
#include <iterator>
#include <string_view>
#include <vector>

#include "gtest/gtest.h"
#include "zserio/BitPositionUtil.h"
#include "zserio/ConstraintException.h"
#include "zserio/SerializeUtil.h"
#include "zserio/TrackingAllocator.h"
#include "zserio/Types.h"

namespace
{

struct SimpleStructure
{
    using AllocatorType = std::allocator<uint8_t>;

    SimpleStructure() noexcept :
            SimpleStructure(AllocatorType{})
    {}

    explicit SimpleStructure(const AllocatorType&) noexcept :
            numberA(),
            numberB(),
            numberC()
    {}

    explicit SimpleStructure(zserio::UInt3 numberA_, zserio::UInt8 numberB_, zserio::UInt7 numberC_) noexcept :
            numberA(numberA_),
            numberB(numberB_),
            numberC(numberC_)
    {}

    zserio::UInt3 numberA;
    zserio::UInt8 numberB;
    zserio::UInt7 numberC;
};

struct SimpleParameterizedStructure
{
    using AllocatorType = std::allocator<uint8_t>;

    SimpleParameterizedStructure() noexcept :
            SimpleParameterizedStructure(AllocatorType{})
    {}

    explicit SimpleParameterizedStructure(const AllocatorType&) noexcept :
            numberA(),
            offsetB(),
            numberB(),
            numberC(),
            numberD()
    {}

    explicit SimpleParameterizedStructure(zserio::UInt3 numberA_, zserio::UInt8 numberB_,
            zserio::UInt7 numberC_, zserio::UInt6 numberD_) noexcept :
            numberA(numberA_),
            offsetB(),
            numberB(numberB_),
            numberC(numberC_),
            numberD(numberD_)
    {}

    zserio::UInt3 numberA;
    mutable zserio::UInt16 offsetB;
    zserio::UInt8 numberB;
    zserio::UInt7 numberC;
    zserio::UInt6 numberD;
};

} // namespace

namespace zserio
{

template <>
class View<SimpleStructure>
{
public:
    explicit View(const SimpleStructure& data) noexcept :
            m_data(data)
    {}

    zserio::UInt3 numberA() const
    {
        return m_data.numberA;
    }
    zserio::UInt8 numberB() const
    {
        return m_data.numberB;
    }
    zserio::UInt7 numberC() const
    {
        return m_data.numberC;
    }

private:
    const SimpleStructure& m_data;
};

namespace detail
{

template <>
void validate(const zserio::View<SimpleStructure>&, std::string_view)
{}

template <>
void write(zserio::BitStreamWriter& writer, const zserio::View<SimpleStructure>& view)
{
    detail::write(writer, view.numberA());
    detail::write(writer, view.numberB());
    detail::write(writer, view.numberC());
}

template <>
View<SimpleStructure> read(zserio::BitStreamReader& reader, SimpleStructure& data)
{
    View<SimpleStructure> view(data);

    detail::read(reader, data.numberA);
    detail::read(reader, data.numberB);
    detail::read(reader, data.numberC);

    return view;
}

template <>
BitSize bitSizeOf(const zserio::View<SimpleStructure>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    endBitPosition += detail::bitSizeOf(view.numberA());
    endBitPosition += detail::bitSizeOf(view.numberB());
    endBitPosition += detail::bitSizeOf(view.numberC());

    return endBitPosition - bitPosition;
}

} // namespace detail

template <>
class View<SimpleParameterizedStructure>
{
public:
    explicit View(const SimpleParameterizedStructure& data, zserio::UInt6 upperLimitD) noexcept :
            m_data(data),
            m_upperLimitD(upperLimitD)
    {}

    zserio::UInt6 upperLimitD() const
    {
        return m_upperLimitD;
    }
    zserio::UInt3 numberA() const
    {
        return m_data.numberA;
    }
    zserio::UInt16 offsetB() const
    {
        return m_data.offsetB;
    }
    zserio::UInt8 numberB() const
    {
        return m_data.numberB;
    }
    zserio::UInt7 numberC() const
    {
        return m_data.numberC;
    }
    zserio::UInt6 numberD() const
    {
        return m_data.numberD;
    }

    const SimpleParameterizedStructure& zserioData() const
    {
        return m_data;
    }

private:
    const SimpleParameterizedStructure& m_data;
    zserio::UInt6 m_upperLimitD;
};

namespace detail
{

template <>
void validate(const zserio::View<SimpleParameterizedStructure>& view, std::string_view fieldName)
{
    (void)fieldName;

    // check constraint
    if (!(view.numberD() <= view.upperLimitD()))
    {
        throw zserio::ConstraintException("validate: Constraint violated!");
    }
}

template <>
void write(zserio::BitStreamWriter& writer, const zserio::View<SimpleParameterizedStructure>& view)
{
    detail::write(writer, view.numberA());
    detail::write(writer, view.offsetB());
    writer.alignTo(8);
    detail::write(writer, view.numberB());
    detail::write(writer, view.numberC());
    detail::write(writer, view.numberD());
}

template <>
View<SimpleParameterizedStructure> read(
        zserio::BitStreamReader& reader, SimpleParameterizedStructure& data, zserio::UInt6 upperLimitD)
{
    View<SimpleParameterizedStructure> view(data, upperLimitD);

    detail::read(reader, data.numberA);
    detail::read(reader, data.offsetB);
    reader.alignTo(8);
    detail::read(reader, data.numberB);
    detail::read(reader, data.numberC);
    detail::read(reader, data.numberD);

    return view;
}

template <>
BitSize bitSizeOf(const zserio::View<SimpleParameterizedStructure>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    endBitPosition += detail::bitSizeOf(view.numberA());
    endBitPosition += detail::bitSizeOf(view.offsetB());
    endBitPosition = alignTo(8, endBitPosition);
    endBitPosition += detail::bitSizeOf(view.numberB());
    endBitPosition += detail::bitSizeOf(view.numberC());
    endBitPosition += detail::bitSizeOf(view.numberD());

    return endBitPosition - bitPosition;
}

template <>
struct OffsetsInitializer<SimpleParameterizedStructure>
{
    static BitSize initialize(const zserio::View<SimpleParameterizedStructure>& view, BitSize bitPosition)
    {
        BitSize endBitPosition = bitPosition;

        endBitPosition += detail::bitSizeOf(view.numberA());
        endBitPosition += detail::bitSizeOf(view.offsetB());
        endBitPosition = alignTo(8, endBitPosition);
        // set offset
        view.zserioData().offsetB = static_cast<uint16_t>(endBitPosition / 8);
        endBitPosition += detail::bitSizeOf(view.numberB());
        endBitPosition += detail::bitSizeOf(view.numberC());
        endBitPosition += detail::bitSizeOf(view.numberD());

        return endBitPosition - bitPosition;
    }
};

} // namespace detail

TEST(SerializeUtilTest, serializeData)
{
    const SimpleStructure simpleStructure{0x07, 0x07, 0x7F};
    const BitBuffer bitBuffer = serialize(simpleStructure);
    ASSERT_EQ(18, bitBuffer.getBitSize());
    ASSERT_EQ(0xE0, bitBuffer.getData()[0]);
    ASSERT_EQ(0xFF, bitBuffer.getData()[1]);
    ASSERT_EQ(0xC0, bitBuffer.getData()[2]);
}

TEST(SerializeUtilTest, serializeView)
{
    const SimpleStructure simpleStructure{0x07, 0x07, 0x7F};
    const View<SimpleStructure> view(simpleStructure);
    const BitBuffer bitBuffer = serialize(view);
    ASSERT_EQ(18, bitBuffer.getBitSize());
    ASSERT_EQ(0xE0, bitBuffer.getData()[0]);
    ASSERT_EQ(0xFF, bitBuffer.getData()[1]);
    ASSERT_EQ(0xC0, bitBuffer.getData()[2]);
}

TEST(SerializeUtilTest, serializeViewWithAllocator)
{
    const TrackingAllocator<uint8_t> allocator;
    const SimpleStructure simpleStructure{0x07, 0x07, 0x7F};
    const View<SimpleStructure> view(simpleStructure);
    const BasicBitBuffer<TrackingAllocator<uint8_t>> bitBuffer = serialize(view, allocator);

    ASSERT_EQ(1, allocator.numAllocs());
    ASSERT_EQ(3, allocator.totalAllocatedSize());
    ASSERT_EQ(18, bitBuffer.getBitSize());
    ASSERT_EQ(0xE0, bitBuffer.getData()[0]);
    ASSERT_EQ(0xFF, bitBuffer.getData()[1]);
    ASSERT_EQ(0xC0, bitBuffer.getData()[2]);
}

TEST(SerializeUtilTest, deserializeData)
{
    const std::array<uint8_t, 3> buffer = {0xE0, 0xFF, 0xC0};
    const BitBuffer bitBuffer(buffer, 18);
    SimpleStructure simpleStructure;
    const View<SimpleStructure> view = deserialize<SimpleStructure>(bitBuffer, simpleStructure);
    ASSERT_EQ(0x07, simpleStructure.numberA);
    ASSERT_EQ(0x07, simpleStructure.numberB);
    ASSERT_EQ(0x7F, simpleStructure.numberC);
    ASSERT_EQ(simpleStructure.numberA, view.numberA());
    ASSERT_EQ(simpleStructure.numberB, view.numberB());
    ASSERT_EQ(simpleStructure.numberC, view.numberC());
}

TEST(SerializeUtilTest, serializeDataToFile)
{
    const SimpleStructure simpleStructure{0x07, 0x07, 0x7F};
    std::string_view fileName("SerializationTest.bin");
    serializeToFile(simpleStructure, fileName);

    std::ifstream file(fileName.data(), std::ios::binary);
    const std::vector<uint8_t> readData(
            (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    ASSERT_EQ(3, readData.size());
    ASSERT_EQ(0xE0, readData[0]);
    ASSERT_EQ(0xFF, readData[1]);
    ASSERT_EQ(0xC0, readData[2]);
}

TEST(SerializeUtilTest, serializeViewToFile)
{
    const SimpleStructure simpleStructure{0x07, 0x07, 0x7F};
    std::string_view fileName("SerializeViewToFile.bin");
    const View<SimpleStructure> view(simpleStructure);
    serializeToFile(simpleStructure, fileName);

    std::ifstream file(fileName.data(), std::ios::binary);
    const std::vector<uint8_t> readData(
            (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    file.close();
    ASSERT_EQ(3, readData.size());
    ASSERT_EQ(0xE0, readData[0]);
    ASSERT_EQ(0xFF, readData[1]);
    ASSERT_EQ(0xC0, readData[2]);
}

TEST(SerializeUtilTest, deserializeDataFromFile)
{
    std::string_view fileName("DeserializeDataFromFile.bin");
    const std::vector<char> data({static_cast<char>(0xE0), static_cast<char>(0xFF), static_cast<char>(0xC0)});
    std::ofstream file;
    file.open(fileName.data(), std::ios::out | std::ios::binary);
    file.write(&data[0], static_cast<std::streamsize>(data.size()));
    file.close();

    SimpleStructure simpleStructure;
    const View<SimpleStructure> view = deserializeFromFile<SimpleStructure>(fileName, simpleStructure);
    ASSERT_EQ(0x07, simpleStructure.numberA);
    ASSERT_EQ(0x07, simpleStructure.numberB);
    ASSERT_EQ(0x7F, simpleStructure.numberC);
    ASSERT_EQ(simpleStructure.numberA, view.numberA());
    ASSERT_EQ(simpleStructure.numberB, view.numberB());
    ASSERT_EQ(simpleStructure.numberC, view.numberC());
}

TEST(SerializeUtilTest, serializeParameterizedData)
{
    const SimpleParameterizedStructure simpleParameterizedStructure{0x07, 0x07, 0x7F, 0x01};
    EXPECT_EQ(45, detail::bitSizeOf(View(simpleParameterizedStructure, static_cast<UInt6>(0x0F))));
    const BitBuffer bitBuffer = serialize(simpleParameterizedStructure, static_cast<UInt6>(0x0F));
    ASSERT_EQ(45, bitBuffer.getBitSize());
    EXPECT_EQ(3, simpleParameterizedStructure.offsetB);
    ASSERT_EQ(0xE0, bitBuffer.getData()[0]);
    ASSERT_EQ(0x00, bitBuffer.getData()[1]);
    ASSERT_EQ(0x60, bitBuffer.getData()[2]);
    ASSERT_EQ(0x07, bitBuffer.getData()[3]);
    ASSERT_EQ(0xFE, bitBuffer.getData()[4]);
    ASSERT_EQ(0x08, bitBuffer.getData()[5]);
}

TEST(SerializeUtilTest, deserializeParameterizedData)
{
    const std::array<uint8_t, 6> buffer = {0xE0, 0x00, 0x60, 0x07, 0xFE, 0x08};
    const BitBuffer bitBuffer(buffer, 45);
    SimpleParameterizedStructure simpleParameterizedStructure;
    const UInt6 upperLimitD = 0x0F;
    const View<SimpleParameterizedStructure> view =
            deserialize(bitBuffer, simpleParameterizedStructure, upperLimitD);

    ASSERT_EQ(0x07, simpleParameterizedStructure.numberA);
    ASSERT_EQ(3, simpleParameterizedStructure.offsetB);
    ASSERT_EQ(0x07, simpleParameterizedStructure.numberB);
    ASSERT_EQ(0x7F, simpleParameterizedStructure.numberC);
    ASSERT_EQ(0x01, simpleParameterizedStructure.numberD);
    ASSERT_EQ(0x0F, view.upperLimitD());
    ASSERT_EQ(simpleParameterizedStructure.numberA, view.numberA());
    ASSERT_EQ(simpleParameterizedStructure.offsetB, view.offsetB());
    ASSERT_EQ(simpleParameterizedStructure.numberB, view.numberB());
    ASSERT_EQ(simpleParameterizedStructure.numberC, view.numberC());
    ASSERT_EQ(simpleParameterizedStructure.numberD, view.numberD());
}

} // namespace zserio
