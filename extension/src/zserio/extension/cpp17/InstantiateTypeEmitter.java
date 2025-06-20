package zserio.extension.cpp17;

import zserio.ast.InstantiateType;
import zserio.extension.common.OutputFileManager;
import zserio.extension.common.ZserioExtensionException;

/**
 * Instantiate type emitter.
 */
public final class InstantiateTypeEmitter extends CppDefaultEmitter
{
    public InstantiateTypeEmitter(OutputFileManager outputFileManager, Cpp17ExtensionParameters cppParameters,
            TemplateDataContext context)
    {
        super(outputFileManager, cppParameters, context);
    }

    @Override
    public void beginInstantiateType(InstantiateType instantiateType) throws ZserioExtensionException
    {
        Object templateData = new InstantiateTypeEmitterTemplateData(getTemplateDataContext(), instantiateType);

        processHeaderTemplate(TEMPLATE_HEADER_NAME, templateData, instantiateType);
    }

    private static final String TEMPLATE_HEADER_NAME = "InstantiateType.h.ftl";
}
