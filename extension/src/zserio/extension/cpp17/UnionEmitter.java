package zserio.extension.cpp17;

import zserio.ast.UnionType;
import zserio.extension.common.OutputFileManager;
import zserio.extension.common.ZserioExtensionException;

/**
 * Union emitter.
 */
public final class UnionEmitter extends CppDefaultEmitter
{
    public UnionEmitter(OutputFileManager outputFileManager, Cpp17ExtensionParameters cppParameters,
            TemplateDataContext context)
    {
        super(outputFileManager, cppParameters, context);
    }

    @Override
    public void beginUnion(UnionType unionType) throws ZserioExtensionException
    {
        final Object templateData = new UnionEmitterTemplateData(getTemplateDataContext(), unionType);
        if (unionType.getTemplateParameters().isEmpty())
        {
            processHeaderTemplate(FTL_HEADER_NAME, templateData, unionType);
            processSourceTemplate(FTL_SOURCE_NAME, templateData, unionType);
        }
        else
        {
            processHeaderTemplate(FTL_TEMPLATE_HEADER_NAME, templateData, unionType);
        }
    }

    private static final String FTL_HEADER_NAME = "Union.h.ftl";
    private static final String FTL_SOURCE_NAME = "Union.cpp.ftl";
    private static final String FTL_TEMPLATE_HEADER_NAME = "UnionTemplate.h.ftl";
}
