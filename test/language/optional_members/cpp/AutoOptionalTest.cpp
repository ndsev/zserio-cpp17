#include "gtest/gtest.h"
#include "optional_members/auto_optional/Container.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"

namespace optional_members
{
namespace auto_optional
{

using AllocatorType = Container::AllocatorType;

class AutoOptionalDataTest : public ::testing::Test
{
protected:
    static constexpr zserio::Int32 NON_OPTIONAL_INT_VALUE = static_cast<int32_t>(0xDEADDEAD);
    static constexpr zserio::Int32 AUTO_OPTIONAL_INT_VALUE = static_cast<int32_t>(0xBEEFBEEF);
};

class AutoOptionalViewTest : public AutoOptionalDataTest
{
protected:
    void checkContainerInBitStream(zserio::BitStreamReader& reader, int32_t nonOptionalIntValue,
            bool hasOptionalIntValue, int32_t optionalIntValue)
    {
        ASSERT_EQ(nonOptionalIntValue, reader.readSignedBits32(32));
        if (hasOptionalIntValue)
        {
            ASSERT_TRUE(reader.readBool());
            ASSERT_EQ(optionalIntValue, reader.readSignedBits32(32));
        }
        else
        {
            ASSERT_FALSE(reader.readBool());
        }
        reader.setBitPosition(0);
    }

    static constexpr size_t CONTAINER_BIT_SIZE_WITHOUT_OPTIONAL = 32 + 1;
    static constexpr size_t CONTAINER_BIT_SIZE_WITH_OPTIONAL = 32 + 1 + 32;

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

TEST_F(AutoOptionalDataTest, emptyConstructor)
{
    {
        const Container container;
        ASSERT_FALSE(container.autoOptionalInt);
    }
    {
        const Container container = {};
        ASSERT_FALSE(container.autoOptionalInt);
    }
    {
        const Container container{AllocatorType()};
        ASSERT_FALSE(container.autoOptionalInt);
    }
}

TEST_F(AutoOptionalDataTest, fieldConstructor)
{
    const Container containerWithOptional(NON_OPTIONAL_INT_VALUE, AUTO_OPTIONAL_INT_VALUE);
    ASSERT_TRUE(containerWithOptional.autoOptionalInt);
    ASSERT_EQ(AUTO_OPTIONAL_INT_VALUE, *containerWithOptional.autoOptionalInt);

    const Container containerWithoutOptional(NON_OPTIONAL_INT_VALUE, {});
    ASSERT_FALSE(containerWithoutOptional.autoOptionalInt);

    const Container containerWithDefaultOptional({}, {});
    ASSERT_EQ(0, containerWithDefaultOptional.nonOptionalInt);
    ASSERT_FALSE(containerWithDefaultOptional.autoOptionalInt);
}

TEST_F(AutoOptionalDataTest, resetAutoOptionalInt)
{
    Container container;
    container.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    ASSERT_TRUE(container.autoOptionalInt);

    container.autoOptionalInt = {};
    ASSERT_FALSE(container.autoOptionalInt);

    container.autoOptionalInt = std::nullopt;
    ASSERT_FALSE(container.autoOptionalInt);
}

TEST_F(AutoOptionalDataTest, operatorEquality)
{
    Container container1;
    Container container2;

    container1.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    container1.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    container2.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    ASSERT_FALSE(container1 == container2);

    container2.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    ASSERT_TRUE(container1 == container2);

    container1.autoOptionalInt = {};
    ASSERT_FALSE(container1 == container2);
}

TEST_F(AutoOptionalDataTest, operatorLessThan)
{
    Container container1;
    Container container2;
    ASSERT_FALSE(container1 < container2);
    ASSERT_FALSE(container2 < container1);

    container1.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    container1.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    container2.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    ASSERT_FALSE(container1 < container2);
    ASSERT_TRUE(container2 < container1);

    container2.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    ASSERT_FALSE(container1 < container2);
    ASSERT_FALSE(container2 < container1);

    container1.autoOptionalInt = {};
    ASSERT_TRUE(container1 < container2);
    ASSERT_FALSE(container2 < container1);
}

TEST_F(AutoOptionalDataTest, stdHash)
{
    std::hash<Container> hasher;

    Container container1;
    Container container2;

    container1.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    container1.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    container2.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    ASSERT_NE(hasher(container1), hasher(container2));

    container2.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    ASSERT_EQ(hasher(container1), hasher(container2));

    container1.autoOptionalInt = {};
    ASSERT_NE(hasher(container1), hasher(container2));

    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(3735937536, hasher(container1));
    ASSERT_EQ(3994118383, hasher(container2));
}

TEST_F(AutoOptionalViewTest, operatorEquality)
{
    Container container1;
    Container container2;

    zserio::View view1(container1);
    zserio::View view2(container2);

    container1.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    container1.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    container2.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    ASSERT_FALSE(view1 == view2);

    container2.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    ASSERT_TRUE(view1 == view2);

    container1.autoOptionalInt = {};
    ASSERT_FALSE(view1 == view2);
}

TEST_F(AutoOptionalViewTest, operatorLessThan)
{
    Container container1;
    Container container2;

    zserio::View view1(container1);
    zserio::View view2(container2);

    ASSERT_FALSE(view1 < view2);
    ASSERT_FALSE(view2 < view1);

    container1.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    container1.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    container2.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    ASSERT_FALSE(view1 < view2);
    ASSERT_TRUE(view2 < view1);

    container2.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    ASSERT_FALSE(view1 < view2);
    ASSERT_FALSE(view2 < view1);

    container1.autoOptionalInt = {};
    ASSERT_TRUE(view1 < view2);
    ASSERT_FALSE(view2 < view1);
}

TEST_F(AutoOptionalViewTest, stdHash)
{
    std::hash<zserio::View<Container>> hasher;

    Container container1;
    Container container2;

    zserio::View view1(container1);
    zserio::View view2(container2);

    container1.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    container1.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    container2.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    ASSERT_NE(hasher(view1), hasher(view2));

    container2.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    ASSERT_EQ(hasher(view1), hasher(view2));

    container1.autoOptionalInt = {};
    ASSERT_NE(hasher(view1), hasher(view2));

    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(3735937536, hasher(view1));
    ASSERT_EQ(3994118383, hasher(view2));
}

TEST_F(AutoOptionalViewTest, bitSizeOf)
{
    Container container;
    zserio::View view(container);

    container.nonOptionalInt = NON_OPTIONAL_INT_VALUE;
    ASSERT_EQ(CONTAINER_BIT_SIZE_WITHOUT_OPTIONAL, zserio::detail::bitSizeOf(view, 0));

    container.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;
    ASSERT_EQ(CONTAINER_BIT_SIZE_WITH_OPTIONAL, zserio::detail::bitSizeOf(view, 0));
}

TEST_F(AutoOptionalViewTest, writeRead)
{
    Container container;
    zserio::View view(container);
    container.nonOptionalInt = NON_OPTIONAL_INT_VALUE;

    zserio::BitStreamWriter writerNonOptional(bitBuffer);
    zserio::detail::write(writerNonOptional, view);

    zserio::BitStreamReader readerNonOptional(
            writerNonOptional.getWriteBuffer(), writerNonOptional.getBitPosition(), zserio::BitsTag());
    checkContainerInBitStream(readerNonOptional, NON_OPTIONAL_INT_VALUE, false, 0);
    Container readContainerNonOptional;
    zserio::View readViewNonOptional = zserio::detail::read(readerNonOptional, readContainerNonOptional);
    ASSERT_EQ(NON_OPTIONAL_INT_VALUE, readViewNonOptional.nonOptionalInt());
    ASSERT_FALSE(readViewNonOptional.autoOptionalInt());

    container.autoOptionalInt = AUTO_OPTIONAL_INT_VALUE;

    zserio::BitStreamWriter writerOptional(bitBuffer);
    zserio::detail::write(writerOptional, view);

    zserio::BitStreamReader readerOptional(
            writerOptional.getWriteBuffer(), writerOptional.getBitPosition(), zserio::BitsTag());
    checkContainerInBitStream(readerOptional, NON_OPTIONAL_INT_VALUE, true, AUTO_OPTIONAL_INT_VALUE);
    Container readContainerOptional;
    zserio::View readViewOptional = zserio::detail::read(readerOptional, readContainerOptional);
    ASSERT_EQ(NON_OPTIONAL_INT_VALUE, readViewOptional.nonOptionalInt());
    ASSERT_TRUE(readViewOptional.autoOptionalInt());
    ASSERT_EQ(AUTO_OPTIONAL_INT_VALUE, readViewOptional.autoOptionalInt());
}

} // namespace auto_optional
} // namespace optional_members
