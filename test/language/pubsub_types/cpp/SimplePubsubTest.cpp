#include "gtest/gtest.h"
#include "pubsub_types/simple_pubsub/SimplePubsub.h"
#include "pubsub_types/simple_pubsub/SimplePubsubClient.h"
#include "pubsub_types/simple_pubsub/SimplePubsubProvider.h"
#include "test_utils/TestPubsub.h"
#include "zserio/PubsubException.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SerializeUtil.h"

using namespace std::literals;

namespace pubsub_types
{
namespace simple_pubsub
{

using AllocatorType = SimplePubsub::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
using BitBufferType = zserio::BasicBitBuffer<AllocatorType>;

class SimplePubsubTest : public ::testing::Test
{
public:
    SimplePubsubTest() :
            simplePubsubProvider(pubsub),
            simplePubsubClient(pubsub),
            simplePubsub(createPubsub())
    {}

protected:
    class TestPubsubImpl : public test_utils::TestPubsub<AllocatorType>
    {
    public:
        struct Context
        {
            bool seenByPubsub = false;
        };

    protected:
        void processPublishContext(void* context) override
        {
            if (context == nullptr)
            {
                return;
            }

            static_cast<Context*>(context)->seenByPubsub = true;
        }

        void processSubscribeContext(void* context) override
        {
            if (context == nullptr)
            {
                return;
            }

            static_cast<Context*>(context)->seenByPubsub = true;
        }
    };

private:
    TestPubsubImpl pubsub;

protected:
    SimplePubsubProvider simplePubsubProvider;
    SimplePubsubClient simplePubsubClient;
    SimplePubsub simplePubsub;

private:
    // check that move constructor works correctly
    SimplePubsub createPubsub()
    {
        // cannot be const because we need to move-construct it
        SimplePubsub createdSimplePubsub(pubsub);
        return createdSimplePubsub;
    }
};

TEST_F(SimplePubsubTest, powerOfTwoClientAndProvider)
{
    struct RequestCallback : public SimplePubsubProvider::SimplePubsubProviderCallback<Int32Value>
    {
        explicit RequestCallback(SimplePubsubProvider& provider) :
                simplePubsubProvider(provider)
        {}

        void operator()(std::string_view topic, const Int32Value& value) override
        {
            ASSERT_EQ("simple_pubsub/request"sv, topic);
            const uint64_t absValue =
                    value.value > 0 ? static_cast<uint64_t>(value.value) : static_cast<uint64_t>(-value.value);
            UInt64Value result{absValue * absValue};
            simplePubsubProvider.publishPowerOfTwo(result);
        }

        SimplePubsubProvider& simplePubsubProvider;
    };

    simplePubsubProvider.subscribeRequest(
            std::allocate_shared<RequestCallback>(AllocatorType(), simplePubsubProvider));

    struct PowerOfTwoCallback : public SimplePubsubClient::SimplePubsubClientCallback<UInt64Value>
    {
        void operator()(std::string_view topic, const UInt64Value& value) override
        {
            ASSERT_EQ("simple_pubsub/power_of_two"sv, topic);
            result = value.value;
        }

        uint64_t result = 0;
    };

    std::shared_ptr<PowerOfTwoCallback> powerOfTwoCallback =
            std::allocate_shared<PowerOfTwoCallback>(AllocatorType());
    simplePubsubClient.subscribePowerOfTwo(powerOfTwoCallback);

    Int32Value request{13};
    simplePubsubClient.publishRequest(request);
    ASSERT_EQ(169, powerOfTwoCallback->result);

    request.value = -13;
    simplePubsubClient.publishRequest(request);
    ASSERT_EQ(169, powerOfTwoCallback->result);

    request.value = 2;
    simplePubsubClient.publishRequest(request);
    ASSERT_EQ(4, powerOfTwoCallback->result);

    request.value = -2;
    simplePubsubClient.publishRequest(request);
    ASSERT_EQ(4, powerOfTwoCallback->result);
}

TEST_F(SimplePubsubTest, powerOfTwoSimplePubsub)
{
    struct RequestCallback : public SimplePubsub::SimplePubsubCallback<Int32Value>
    {
        explicit RequestCallback(SimplePubsub& pubsub) :
                simplePubsub(pubsub)
        {}

        void operator()(std::string_view topic, const Int32Value& value) override
        {
            ASSERT_EQ("simple_pubsub/request"sv, topic);
            const uint64_t absValue =
                    value.value > 0 ? static_cast<uint64_t>(value.value) : static_cast<uint64_t>(-value.value);
            UInt64Value result{absValue * absValue};
            simplePubsub.publishPowerOfTwo(result);
        }

        SimplePubsub& simplePubsub;
    };

    simplePubsub.subscribeRequest(std::allocate_shared<RequestCallback>(AllocatorType(), simplePubsub));

    struct PowerOfTwoCallback : public SimplePubsub::SimplePubsubCallback<UInt64Value>
    {
        void operator()(std::string_view topic, const UInt64Value& value) override
        {
            ASSERT_EQ("simple_pubsub/power_of_two"sv, topic);
            result = value.value;
        }

        uint64_t result = 0;
    };

    std::shared_ptr<PowerOfTwoCallback> powerOfTwoCallback =
            std::allocate_shared<PowerOfTwoCallback>(AllocatorType());
    simplePubsub.subscribePowerOfTwo(powerOfTwoCallback);

    Int32Value request{13};
    simplePubsub.publishRequest(request);
    ASSERT_EQ(169, powerOfTwoCallback->result);

    request.value = -13;
    simplePubsub.publishRequest(request);
    ASSERT_EQ(169, powerOfTwoCallback->result);

    request.value = 2;
    simplePubsub.publishRequest(request);
    ASSERT_EQ(4, powerOfTwoCallback->result);

    request.value = -2;
    simplePubsub.publishRequest(request);
    ASSERT_EQ(4, powerOfTwoCallback->result);
}

TEST_F(SimplePubsubTest, powerOfTwoRawClientAndProvider)
{
    struct RequestRawCallback
            : public SimplePubsubProvider::SimplePubsubProviderCallback<zserio::Span<const uint8_t>>
    {
        explicit RequestRawCallback(SimplePubsubProvider& provider) :
                simplePubsubProvider(provider)
        {}

        void operator()(std::string_view topic, const zserio::Span<const uint8_t>& valueData) override
        {
            ASSERT_EQ("simple_pubsub/request_raw"sv, topic);
            Int32Value value;
            zserio::deserializeFromBytes(valueData, value);
            const uint64_t absValue =
                    value.value > 0 ? static_cast<uint64_t>(value.value) : static_cast<uint64_t>(-value.value);
            UInt64Value result{absValue * absValue};
            const BitBufferType resultBitBuffer = zserio::serialize(result);
            simplePubsubProvider.publishPowerOfTwoRaw(
                    ::zserio::Span<const uint8_t>{resultBitBuffer.getBuffer(), resultBitBuffer.getByteSize()});
        }

        SimplePubsubProvider& simplePubsubProvider;
    };

    simplePubsubProvider.subscribeRequestRaw(
            std::allocate_shared<RequestRawCallback>(AllocatorType(), simplePubsubProvider));

    struct PowerOfTwoRawCallback
            : public SimplePubsubClient::SimplePubsubClientCallback<zserio::Span<const uint8_t>>
    {
        void operator()(std::string_view topic, const zserio::Span<const uint8_t>& valueData) override
        {
            ASSERT_EQ("simple_pubsub/power_of_two_raw"sv, topic);
            UInt64Value value;
            zserio::deserializeFromBytes(valueData, value);
            result = value.value;
        }

        uint64_t result = 0;
    };

    std::shared_ptr<PowerOfTwoRawCallback> powerOfTwoRawCallback =
            std::allocate_shared<PowerOfTwoRawCallback>(AllocatorType());
    simplePubsubClient.subscribePowerOfTwoRaw(powerOfTwoRawCallback);

    Int32Value request{13};
    const BitBufferType requestBitBuffer = zserio::serialize(request);
    simplePubsubClient.publishRequestRaw(
            zserio::Span<const uint8_t>(requestBitBuffer.getBuffer(), requestBitBuffer.getByteSize()));
    ASSERT_EQ(169, powerOfTwoRawCallback->result);
}

TEST_F(SimplePubsubTest, powerOfTwoRawSimplePubsub)
{
    struct RequestRawCallback : public SimplePubsub::SimplePubsubCallback<zserio::Span<const uint8_t>>
    {
        explicit RequestRawCallback(SimplePubsub& pubsub) :
                simplePubsub(pubsub)
        {}

        void operator()(std::string_view topic, const zserio::Span<const uint8_t>& valueData) override
        {
            ASSERT_EQ("simple_pubsub/request_raw"sv, topic);
            Int32Value value;
            zserio::deserializeFromBytes(valueData, value);
            const uint64_t absValue =
                    value.value > 0 ? static_cast<uint64_t>(value.value) : static_cast<uint64_t>(-value.value);
            UInt64Value result{absValue * absValue};
            const BitBufferType resultBitBuffer = zserio::serialize(result);
            simplePubsub.publishPowerOfTwoRaw(
                    zserio::Span<const uint8_t>{resultBitBuffer.getBuffer(), resultBitBuffer.getByteSize()});
        }

        SimplePubsub& simplePubsub;
    };

    simplePubsub.subscribeRequestRaw(std::allocate_shared<RequestRawCallback>(AllocatorType(), simplePubsub));

    struct PowerOfTwoRawCallback : public SimplePubsub::SimplePubsubCallback<zserio::Span<const uint8_t>>
    {
        void operator()(std::string_view topic, const zserio::Span<const uint8_t>& valueData) override
        {
            ASSERT_EQ("simple_pubsub/power_of_two_raw"sv, topic);
            UInt64Value value;
            zserio::deserializeFromBytes(valueData, value);
            result = value.value;
        }

        uint64_t result = 0;
    };

    std::shared_ptr<PowerOfTwoRawCallback> powerOfTwoRawCallback =
            std::allocate_shared<PowerOfTwoRawCallback>(AllocatorType());
    simplePubsub.subscribePowerOfTwoRaw(powerOfTwoRawCallback);

    Int32Value request{13};
    const BitBufferType requestBitBuffer = zserio::serialize(request);
    simplePubsub.publishRequestRaw(
            zserio::Span<const uint8_t>(requestBitBuffer.getBuffer(), requestBitBuffer.getByteSize()));
    ASSERT_EQ(169, powerOfTwoRawCallback->result);
}

TEST_F(SimplePubsubTest, publishRequestWithContext)
{
    TestPubsubImpl::Context context;
    ASSERT_FALSE(context.seenByPubsub);
    Int32Value request{42};
    simplePubsub.publishRequest(request, &context);
    ASSERT_TRUE(context.seenByPubsub);
}

TEST_F(SimplePubsubTest, subscribeRequestWithContext)
{
    TestPubsubImpl::Context context;
    ASSERT_FALSE(context.seenByPubsub);

    struct RequestCallback : public SimplePubsub::SimplePubsubCallback<Int32Value>
    {
        void operator()(std::string_view, const Int32Value&) override
        {}
    };

    simplePubsub.subscribeRequest(std::allocate_shared<RequestCallback>(AllocatorType()), &context);
    ASSERT_TRUE(context.seenByPubsub);
}

TEST_F(SimplePubsubTest, unsubscribe)
{
    struct RequestCallback : public SimplePubsub::SimplePubsubCallback<Int32Value>
    {
        explicit RequestCallback(SimplePubsub& pubsub) :
                simplePubsub(pubsub)
        {}

        void operator()(std::string_view topic, const Int32Value& value) override
        {
            ASSERT_EQ("simple_pubsub/request"sv, topic);
            const uint64_t absValue =
                    value.value > 0 ? static_cast<uint64_t>(value.value) : static_cast<uint64_t>(-value.value);
            UInt64Value result{absValue * absValue};
            simplePubsub.publishPowerOfTwo(result);
        }

        SimplePubsub& simplePubsub;
    };

    auto id0 =
            simplePubsub.subscribeRequest(std::allocate_shared<RequestCallback>(AllocatorType(), simplePubsub));

    struct PowerOfTwoCallback : public SimplePubsub::SimplePubsubCallback<UInt64Value>
    {
        void operator()(std::string_view topic, const UInt64Value& value) override
        {
            ASSERT_EQ("simple_pubsub/power_of_two"sv, topic);
            result = value.value;
        }

        uint64_t result = 0;
    };

    std::shared_ptr<PowerOfTwoCallback> powerOfTwoCallback1 =
            std::allocate_shared<PowerOfTwoCallback>(AllocatorType());
    auto id1 = simplePubsub.subscribePowerOfTwo(powerOfTwoCallback1);

    std::shared_ptr<PowerOfTwoCallback> powerOfTwoCallback2 =
            std::allocate_shared<PowerOfTwoCallback>(AllocatorType());
    auto id2 = simplePubsub.subscribePowerOfTwo(powerOfTwoCallback2);

    Int32Value request{13};
    simplePubsub.publishRequest(request);
    ASSERT_EQ(169, powerOfTwoCallback1->result);
    ASSERT_EQ(169, powerOfTwoCallback2->result);

    simplePubsub.unsubscribe(id1);
    request.value = 2;
    simplePubsub.publishRequest(request);
    ASSERT_EQ(169, powerOfTwoCallback1->result); // shall not be changed!
    ASSERT_EQ(4, powerOfTwoCallback2->result);

    simplePubsub.unsubscribe(id0); // unsubscribe publisher
    request.value = 3;
    simplePubsub.publishRequest(request);
    ASSERT_EQ(169, powerOfTwoCallback1->result); // shall not be changed!
    ASSERT_EQ(4, powerOfTwoCallback2->result); // shall not be changed!

    simplePubsub.unsubscribe(id2);
}

TEST_F(SimplePubsubTest, unsubscribeInvalid)
{
    ASSERT_THROW(simplePubsub.unsubscribe(0), zserio::PubsubException);
}

} // namespace simple_pubsub
} // namespace pubsub_types
