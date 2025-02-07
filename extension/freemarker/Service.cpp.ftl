<#include "FileHeader.inc.ftl">
<#include "Service.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/SerializeUtil.h>
<@type_includes types.bitBuffer/>
<@system_includes cppSystemIncludes/>

<@user_include package.path, "${name}.h"/>
<@user_includes cppUserIncludes, false/>
<@namespace_begin package.path/>
<@namespace_begin [name]/>

Service::Service(const AllocatorType& allocator) :
        ::zserio::AllocatorHolder<${types.allocator.default}>(allocator)
{}

${types.serviceDataPtr.name} Service::callMethod(
        ::std::string_view methodName, ::zserio::Span<const uint8_t> requestData, void* context)
{
<#list methodList as method>
    if (methodName == methodNames()[${method?index}])
    {
        return ${method.name}Method(requestData, context);
    }
</#list>
    throw ::zserio::ServiceException("${serviceFullName}: Method '") << methodName << "' does not exist!";
}

::std::string_view Service::serviceFullName() noexcept
{
    static const ::std::string_view serviceFullName = "${serviceFullName}";
    return serviceFullName;
}

const ::std::array<::std::string_view, ${methodList?size}>& Service::methodNames() noexcept
{
    static constexpr ::std::array<::std::string_view, ${methodList?size}> names =
    {
<#list methodList as method>
        "${method.name}"<#if method?has_next>,</#if>
</#list>
    };

    return names;
}
<#list methodList as method>

${types.serviceDataPtr.name} Service::${method.name}Method(
        ::zserio::Span<const uint8_t> requestData, void* context)
{
    <#if !method.requestTypeInfo.isBytes>
    ${method.requestTypeInfo.typeFullName} request(get_allocator_ref());
    ::zserio::deserializeFromBytes(requestData, request);

    </#if>
    <#if method.responseTypeInfo.isBytes>
    return ::std::allocate_shared<${types.rawServiceDataHolder.name}>(get_allocator_ref(),
            ${method.name}Impl(request<#if method.requestTypeInfo.isBytes>Data</#if>, context));
    <#else>
    class ResponseData : public ${types.serviceDataPtr.name}::element_type
    {
    public:
        ResponseData(${method.responseTypeInfo.typeFullName}&& response, const AllocatorType& allocator) :
                m_serviceData(response, allocator)
        {}

        ::zserio::Span<const uint8_t> getData() const override
        {
            return m_serviceData.getData();
        }

    private:
        ${types.objectServiceData.name} m_serviceData;
    };

    return ::std::allocate_shared<ResponseData>(get_allocator_ref(),
            ${method.name}Impl(request<#if method.requestTypeInfo.isBytes>Data</#if>, context), <#rt>
            <#lt>get_allocator_ref());
    </#if>
}
</#list>

Client::Client(${types.serviceClient.name}& service, const AllocatorType& allocator) :
        ::zserio::AllocatorHolder<${types.allocator.default}>(allocator),
        m_service(service)
{
}
<#list methodList as method>

${method.responseTypeInfo.typeFullName} Client::${method.name}Method(<#rt>
        <#lt><@service_arg_type_name method.requestTypeInfo/> request, void* context)
{
    <#if method.requestTypeInfo.isBytes>
    const ${types.rawServiceDataView.name} requestData(request);
    <#else>
    const ${types.objectServiceData.name} requestData(request, get_allocator_ref());
    </#if>

    auto responseData = m_service.callMethod("${method.name}", requestData, context);
    <#if method.responseTypeInfo.isBytes>
    return responseData;
    <#else>

    ${method.responseTypeInfo.typeFullName} response(get_allocator_ref());
    ::zserio::deserializeFromBytes(responseData, response);
    return response;
    </#if>
}
</#list>
<@namespace_end [name]/>
<@namespace_end package.path/>
