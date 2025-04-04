<#include "FileHeader.inc.ftl">
<#include "DocComment.inc.ftl">
<#include "Pubsub.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <memory>
#include <zserio/AllocatorHolder.h>
#include <zserio/IPubsub.h>
#include <zserio/PubsubException.h>
<#if withTypeInfoCode>
<@type_includes types.typeInfo/>
</#if>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>

<#if docComments??>
<@doc_comments docComments/>
</#if>
class ${name} : public ::zserio::AllocatorHolder<${types.allocator.default}>
{
public:
    /**
     * Constructor from the Pub/Sub client backend.
     *
     * \param pubsub Interface for Pub/Sub client backend.
     * \param allocator Allocator to construct from.
     */
    explicit ${name}(::zserio::IPubsub& pubsub, const allocator_type& allocator = {});

    /** Default destructor. */
    ~${name}() = default;

    /** Disables copy constructor. */
    ${name}(const ${name}&) = delete;
    /** Disables assignment operator. */
    ${name}& operator=(const ${name}&) = delete;

    /** Default move constructor. */
    ${name}(${name}&&) = default;
    /** Disables move assignment operator. */
    ${name}& operator=(${name}&&) = delete;
<#if hasSubscribing>

    /**
     * Interface for callback used for Pub/Sub subscription.
     */
    template <typename ZSERIO_MESSAGE>
    class ${name}Callback
    {
    public:
        /** Default destructor. */
        virtual ~${name}Callback() = default;

        /**
         * Function call operator overload.
         *
         * \param topic Topic definition to be used for Pub/Sub subcription.
         * \param message Message to be used for Pub/Sub subcription.
         */
        virtual void operator()(::std::string_view topic, const ZSERIO_MESSAGE& message) = 0;
    };
</#if>
<#list messageList as message>
    <#if message.isPublished>

    /**
     * Publishes given message as a topic '${message.name}'.
     *
            <#if message.docComments??>
     * \b Description
     *
     <@doc_comments_inner message.docComments, 1/>
     *
            </#if>
     * \param message Message to publish.
     * \param context Context specific for a particular Pub/Sub implementation.
     */
    void publish${message.name?cap_first}(<@pubsub_arg_type_name message.typeInfo/> message, void* context = nullptr);
    </#if>
    <#if message.isSubscribed>

    /**
     * Subscribes a topic '${message.name}'.
     *
        <#if message.docComments??>
     * \b Description
     *
     <@doc_comments_inner message.docComments, 1/>
     *
        </#if>
     * \param callback Callback to be called when a message with the specified topic arrives.
     * \param context Context specific for a particular Pub/Sub implementation.
     *
     * \return Subscription ID.
     * \throw PubsubException when subscribing fails.
     */
    ::zserio::IPubsub::SubscriptionId subscribe${message.name?cap_first}(
            const ::std::shared_ptr<${name}Callback<<@pubsub_type_name message.typeInfo/>>>& callback,
            void* context = nullptr);
    </#if>
</#list>
<#if hasSubscribing>

    /**
     * Unsubscribes the subscription with the given ID.
     *
     * \param id ID of the subscription to be unsubscribed.
     *
     * \throw PubsubException when unsubscribing fails.
     */
    void unsubscribe(::zserio::IPubsub::SubscriptionId id);
</#if>

private:
<#if hasPublishing && has_published_object(messageList)>
    template <typename ZSERIO_MESSAGE>
    void publish(ZSERIO_MESSAGE& message, ::std::string_view topic, void* context);

</#if>
    ::zserio::IPubsub& m_pubsub;
};
<@namespace_end package.path/>
<#if withTypeInfoCode>
<@namespace_begin ["zserio", "detail"]/>

template <>
struct TypeInfo<${fullName}, ${types.allocator.default}>
{
    static const ${types.typeInfo.name}& get();
};
<@namespace_end ["zserio", "detail"]/>
</#if>

<@include_guard_end package.path, name/>
