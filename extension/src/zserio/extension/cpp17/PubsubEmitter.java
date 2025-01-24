package zserio.extension.cpp17;

import zserio.ast.PubsubType;
import zserio.extension.common.OutputFileManager;
import zserio.extension.common.ZserioExtensionException;

/**
 * Pubsub emitter.
 */
public final class PubsubEmitter extends CppDefaultEmitter
{
    public PubsubEmitter(OutputFileManager outputFileManager, Cpp17ExtensionParameters cppParameters,
            TemplateDataContext context)
    {
        super(outputFileManager, cppParameters, context);
    }

    @Override
    public void beginPubsub(PubsubType pubsubType) throws ZserioExtensionException
    {
        final Object templateData = new PubsubEmitterTemplateData(getTemplateDataContext(), pubsubType);
        processSourceTemplate(TEMPLATE_SOURCE_NAME, templateData, pubsubType);
        processHeaderTemplate(TEMPLATE_HEADER_NAME, templateData, pubsubType);
    }

    private static final String TEMPLATE_SOURCE_NAME = "Pubsub.cpp.ftl";
    private static final String TEMPLATE_HEADER_NAME = "Pubsub.h.ftl";
}
