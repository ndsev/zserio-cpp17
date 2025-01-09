package zserio.extension.cpp17;

import zserio.ast.Subtype;
import zserio.extension.common.OutputFileManager;
import zserio.extension.common.PackedTypesCollector;
import zserio.extension.common.ZserioExtensionException;

/**
 * Subtype emitter.
 */
public final class SubtypeEmitter extends CppDefaultEmitter
{
    public SubtypeEmitter(OutputFileManager outputFileManager, Cpp17ExtensionParameters cppParameters,
            TemplateDataContext context)
    {
        super(outputFileManager, cppParameters, context);
    }

    @Override
    public void beginSubtype(Subtype subtype) throws ZserioExtensionException
    {
        Object templateData = new SubtypeEmitterTemplateData(getTemplateDataContext(), subtype);

        processHeaderTemplate(TEMPLATE_HEADER_NAME, templateData, subtype);
    }

    private static final String TEMPLATE_HEADER_NAME = "Subtype.h.ftl";
}
