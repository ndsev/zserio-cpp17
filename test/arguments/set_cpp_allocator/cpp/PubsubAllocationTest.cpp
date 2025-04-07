#include <memory>
#include <string_view>

#include "gtest/gtest.h"
#include "pubsub_allocation/GreetingPubsub.h"
#include "test_utils/MemoryResources.h"
#include "test_utils/TestPubsub.h"

using namespace std::literals;
using namespace test_utils;

namespace pubsub_allocation
{

using allocator_type = GreetingPubsub::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;

class PubsubAllocationTest : public ::testing::Test
{
public:
    PubsubAllocationTest() :
            m_invalidMemoryResource(),
            m_invalidMemoryResourceSetter(m_invalidMemoryResource),
            m_memoryResource("Memory resource"),
            m_allocator(&m_memoryResource),
            m_testPubsub(new TestPubsubImpl(m_allocator)),
            greetingPubsub(*m_testPubsub, m_allocator)
    {}

    ~PubsubAllocationTest() override
    {
        m_testPubsub.reset();
        EXPECT_EQ(m_memoryResource.getNumDeallocations(), m_memoryResource.getNumAllocations());
    }

    PubsubAllocationTest(const PubsubAllocationTest&) = delete;
    PubsubAllocationTest& operator=(const PubsubAllocationTest&) = delete;

    PubsubAllocationTest(PubsubAllocationTest&&) = delete;
    PubsubAllocationTest& operator=(PubsubAllocationTest&&) = delete;

    const allocator_type& getAllocator()
    {
        return m_allocator;
    }

protected:
    struct CountingContext
    {
        size_t publishCount = 0;
        size_t subscribeCount = 0;
    };

    class TestPubsubImpl : public TestPubsub<allocator_type>
    {
    public:
        explicit TestPubsubImpl(const allocator_type& allocator) :
                TestPubsub(allocator)
        {}

    protected:
        void processPublishContext(void* context) override
        {
            if (context == nullptr)
            {
                return;
            }

            static_cast<CountingContext*>(context)->publishCount++;
        }

        void processSubscribeContext(void* context) override
        {
            if (context == nullptr)
            {
                return;
            }

            static_cast<CountingContext*>(context)->subscribeCount++;
        }
    };

    class NameCallback : public GreetingPubsub::GreetingPubsubCallback<Name>
    {
    public:
        using allocator_type = pubsub_allocation::allocator_type;

        explicit NameCallback(GreetingPubsub& pubsub, const allocator_type& allocator) :
                m_greetingPubsub(pubsub),
                m_allocator(allocator)
        {}

        void operator()(std::string_view topic, const Name& name) override
        {
            ASSERT_EQ("pubsub_allocation/name_to_use_for_greeting"sv, topic);
            Greeting greeting{prepareGreeting(name.name), m_allocator};
            m_greetingPubsub.publishGreeting(greeting);
        }

    private:
        StringType prepareGreeting(const StringType& name)
        {
            // use string longer than 32B to catch string allocation on most platforms
            StringType greeting{"Hello my dear ", m_allocator};
            greeting += name;
            greeting += "! I hope you are well!";
            return greeting;
        }

        GreetingPubsub& m_greetingPubsub;
        allocator_type m_allocator;
    };

    struct GreetingCallback : public GreetingPubsub::GreetingPubsubCallback<Greeting>
    {
        using allocator_type = pubsub_allocation::allocator_type;

        explicit GreetingCallback(const allocator_type& allocator) :
                greeting(allocator)
        {}

        void operator()(std::string_view topic, const Greeting& providedGreeting) override
        {
            ASSERT_EQ("pubsub_allocation/greeting_generated_for_name"sv, topic);
            greeting = providedGreeting.greeting;
        }

        StringType greeting;
    };

private:
    InvalidMemoryResource m_invalidMemoryResource;
    MemoryResourceScopedSetter m_invalidMemoryResourceSetter;
    TestMemoryResource<4 * 1024> m_memoryResource;
    allocator_type m_allocator;
    std::unique_ptr<TestPubsubImpl> m_testPubsub;

protected: // must be behind m_allocator
    GreetingPubsub greetingPubsub;
};

TEST_F(PubsubAllocationTest, sendGreetingPubsub)
{
    auto idName =
            greetingPubsub.subscribeName(std::allocate_shared<NameCallback>(getAllocator(), greetingPubsub));

    std::shared_ptr<GreetingCallback> greetingCallback = std::allocate_shared<GreetingCallback>(getAllocator());
    auto idGreeting = greetingPubsub.subscribeGreeting(greetingCallback);

    Name name{StringType("Zserio with long name which is longer than 32B", getAllocator()), getAllocator()};
    greetingPubsub.publishName(name);
    ASSERT_EQ("Hello my dear Zserio with long name which is longer than 32B! I hope you are well!",
            greetingCallback->greeting);

    greetingPubsub.unsubscribe(idName);
    greetingPubsub.unsubscribe(idGreeting);
}

TEST_F(PubsubAllocationTest, sendGreetingPubsubWithContext)
{
    CountingContext countingContext;

    auto idName = greetingPubsub.subscribeName(
            std::allocate_shared<NameCallback>(getAllocator(), greetingPubsub), &countingContext);
    ASSERT_EQ(1, countingContext.subscribeCount);

    std::shared_ptr<GreetingCallback> greetingCallback = std::allocate_shared<GreetingCallback>(getAllocator());
    auto idGreeting = greetingPubsub.subscribeGreeting(greetingCallback, &countingContext);
    ASSERT_EQ(2, countingContext.subscribeCount);

    Name name{StringType("Zserio with long name which is longer than 32B", getAllocator()), getAllocator()};
    greetingPubsub.publishName(name, &countingContext);
    ASSERT_EQ(1, countingContext.publishCount);
    ASSERT_EQ("Hello my dear Zserio with long name which is longer than 32B! I hope you are well!",
            greetingCallback->greeting);

    greetingPubsub.unsubscribe(idName);
    greetingPubsub.unsubscribe(idGreeting);
}

} // namespace pubsub_allocation
