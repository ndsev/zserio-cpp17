package zserio.extension.cpp17;

import zserio.ast.ServiceType;
import zserio.extension.common.OutputFileManager;
import zserio.extension.common.ZserioExtensionException;

/**
 * Service emitter.
 */
public final class ServiceEmitter extends CppDefaultEmitter
{
    public ServiceEmitter(OutputFileManager outputFileManager, Cpp17ExtensionParameters cppParameters,
            TemplateDataContext context)
    {
        super(outputFileManager, cppParameters, context);
    }

    @Override
    public void beginService(ServiceType serviceType) throws ZserioExtensionException
    {
        final Object templateData = new ServiceEmitterTemplateData(getTemplateDataContext(), serviceType);
        processSourceTemplate(TEMPLATE_SOURCE_NAME, templateData, serviceType);
        processHeaderTemplate(TEMPLATE_HEADER_NAME, templateData, serviceType);
    }

    private static final String TEMPLATE_SOURCE_NAME = "Service.cpp.ftl";
    private static final String TEMPLATE_HEADER_NAME = "Service.h.ftl";
}
