<#include "FileHeader.inc.ftl">
<#include "DocComment.inc.ftl">
<#include "Service.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <array>
#include <zserio/AllocatorHolder.h>
<@type_includes types.service/>
#include <zserio/ServiceException.h>
<#if withTypeInfoCode>
<@type_includes types.typeInfo/>
</#if>
#include <zserio/Types.h>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>
<@namespace_begin [name]/>

/**
 * Service part of the service ${name}.
 *
    <#if docComments??>
 * \b Description
 *
 <@doc_comments_inner docComments/>
    </#if>
 */
class Service :
        public ${types.service.name},
        public ::zserio::AllocatorHolder<${types.allocator.default}>
{
public:
    /**
     * Default constructor.
     *
     * \param allocator Allocator to construct from.
     */
    explicit Service(const AllocatorType& allocator = AllocatorType());

    /** Default destructor. */
    ~Service() override = default;

    /** Disables copy constructor. */
    Service(const Service&) = delete;
    /** Disables assignment operator. */
    Service& operator=(const Service&) = delete;

    /** Default move constructor. */
    Service(Service&&) = default;
    /** Disables move assignment operator. */
    Service& operator=(Service&&) = delete;

    /**
     * Calls method with the given name synchronously.
     *
     * \param methodName Name of the service method to call.
     * \param requestData Request data to be passed to the method.
     * \param context Context specific for particular service.
     *
     * \return Created response data.
     *
     * \throw ServiceException if the call fails.
     */
    ${types.serviceDataPtr.name} callMethod(
            ::std::string_view methodName, ::zserio::Span<const uint8_t> requestData,
            void* context) override;

    /**
     * Gets the service full qualified name.
     *
     * \return Service name together with its package name.
     */
    static ::std::string_view serviceFullName() noexcept;

    /**
     * Gets all method names of the service.
     *
     * \return Array of all method names of the service.
     */
    static const ::std::array<::std::string_view, ${methodList?size}>& methodNames() noexcept;

private:
<#if methodList?has_content>
<#list methodList as method>
    virtual ${method.responseTypeInfo.typeFullName} ${method.name}Impl(<#rt>
            <#lt><@service_arg_type_name method.requestTypeInfo/> request, void* context) = 0;
</#list>

<#list methodList as method>
    ${types.serviceDataPtr.name} ${method.name}Method(
            ::zserio::Span<const uint8_t> requestData, void* context);
</#list>
</#if>
};

/**
 * Client part of the service ${name}.
 *
    <#if docComments??>
 * \b Description
 *
 <@doc_comments_inner docComments/>
    </#if>
 */
class Client : public ::zserio::AllocatorHolder<${types.allocator.default}>
{
public:
    /**
     * Constructor from the service client backend.
     *
     * \param service Interface for service client backend.
     * \param allocator Allocator to construct from.
     */
    explicit Client(${types.serviceClient.name}& service, const AllocatorType& allocator = AllocatorType());

    /** Default destructor. */
    ~Client() = default;

    /** Disables copy constructor. */
    Client(const Client&) = delete;
    /** Disables assignment operator. */
    Client& operator=(const Client&) = delete;

    /** Default move constructor. */
    Client(Client&&) = default;
    /** Disables move assignment operator. */
    Client& operator=(Client&&) = delete;
<#list methodList as method>

    /**
     * Calls method ${method.name}.
     *
    <#if method.docComments??>
     * \b Description
     *
     <@doc_comments_inner method.docComments, 1/>
     *
    </#if>
     * \param request Request to be passed to the method.
     * \param context Context specific for particular service.
     *
     * \return Response returned from the method.
     */
    ${method.responseTypeInfo.typeFullName} ${method.name}Method(<#rt>
            <#lt><@service_arg_type_name method.requestTypeInfo/> request, void* context = nullptr);
</#list>

private:
    ${types.serviceClient.name}& m_service;
};
<@namespace_end [name]/>
<@namespace_end package.path/>
<#if withTypeInfoCode>
<@namespace_begin ["zserio", "detail"]/>

template <>
struct TypeInfo<${fullName}::Service, ${types.allocator.default}>
{
    static const ${types.typeInfo.name}& get();
};

template <>
struct TypeInfo<${fullName}::Client, ${types.allocator.default}> :
        TypeInfo<${fullName}::Service, ${types.allocator.default}>
{};
<@namespace_end ["zserio", "detail"]/>
</#if>

<@include_guard_end package.path, name/>
