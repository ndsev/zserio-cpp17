#include "gtest/gtest.h"
#include "service_allocation/GreetingService.h"
#include "test_utils/LocalServiceClient.h"
#include "test_utils/MemoryResources.h"

using namespace test_utils;

namespace service_allocation
{

using AllocatorType = GreetingService::Client::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
using LocalServiceClient = test_utils::LocalServiceClient<AllocatorType>;

class ServiceAllocationTest : public ::testing::Test
{
public:
    ServiceAllocationTest() :
            m_invalidMemoryResource(),
            m_invalidMemoryResourceSetter(m_invalidMemoryResource),
            m_memoryResource("Memory resource"),
            m_allocator(&m_memoryResource),
            greetingService(m_allocator),
            localServiceClient(greetingService, m_allocator),
            greetingClient(localServiceClient, m_allocator)
    {}

    ~ServiceAllocationTest() override
    {
        EXPECT_EQ(m_memoryResource.getNumDeallocations(), m_memoryResource.getNumAllocations());
    }

    ServiceAllocationTest(const ServiceAllocationTest&) = delete;
    ServiceAllocationTest& operator=(const ServiceAllocationTest&) = delete;

    ServiceAllocationTest(ServiceAllocationTest&&) = delete;
    ServiceAllocationTest& operator=(ServiceAllocationTest&&) = delete;

    const AllocatorType& getAllocator()
    {
        return m_allocator;
    }

protected:
    struct CountingContext
    {
        size_t greetingCount = 0;
    };

    class GreetingServiceImpl : public GreetingService::Service
    {
    public:
        explicit GreetingServiceImpl(const allocator_type& allocator) :
                GreetingService::Service(allocator)
        {}

        Greeting sendGreetingImpl(const Name& name, void* context) override
        {
            if (context != nullptr)
            {
                static_cast<CountingContext*>(context)->greetingCount++;
            }

            return Greeting(prepareGreeting(name.name), get_allocator_ref());
        }

    private:
        StringType prepareGreeting(const StringType& name)
        {
            // use string longer than 32B to catch string allocation on most platforms
            StringType greeting{"Hello my dear ", get_allocator_ref()};
            greeting += name;
            greeting += "! I hope you are well!";
            return greeting;
        }
    };

private:
    InvalidMemoryResource m_invalidMemoryResource;
    MemoryResourceScopedSetter m_invalidMemoryResourceSetter;
    TestMemoryResource<> m_memoryResource;
    AllocatorType m_allocator;

protected: // must be behind m_allocator
    GreetingServiceImpl greetingService;
    LocalServiceClient localServiceClient;
    GreetingService::Client greetingClient;
};

TEST_F(ServiceAllocationTest, sendGreeting)
{
    Name name{StringType("Zserio with long name which is longer than 32B", getAllocator()), getAllocator()};
    const Greeting greeting = greetingClient.sendGreetingMethod(name);
    ASSERT_EQ("Hello my dear Zserio with long name which is longer than 32B! I hope you are well!",
            greeting.greeting);
}

TEST_F(ServiceAllocationTest, sendGreetingWithContext)
{
    CountingContext context;

    Name name{StringType("Zserio with long name which is longer than 32B", getAllocator()), getAllocator()};
    const Greeting greeting = greetingClient.sendGreetingMethod(name, &context);
    ASSERT_EQ("Hello my dear Zserio with long name which is longer than 32B! I hope you are well!",
            greeting.greeting);
    ASSERT_EQ(1, context.greetingCount);
}

} // namespace service_allocation
