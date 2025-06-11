package zserio.extension.cpp17;

import zserio.ast.StructureType;
import zserio.extension.common.OutputFileManager;
import zserio.extension.common.ZserioExtensionException;

/**
 * Structure emitter.
 */
public final class StructureEmitter extends CppDefaultEmitter
{
    public StructureEmitter(OutputFileManager outputFileManager, Cpp17ExtensionParameters cppParameters,
            TemplateDataContext context)
    {
        super(outputFileManager, cppParameters, context);
    }

    @Override
    public void beginStructure(StructureType structureType) throws ZserioExtensionException
    {
        final Object templateData = new StructureEmitterTemplateData(getTemplateDataContext(), structureType);
        if (structureType.getTemplateParameters().isEmpty())
        {
            processHeaderTemplate(FTL_HEADER_NAME, templateData, structureType);
            processSourceTemplate(FTL_SOURCE_NAME, templateData, structureType);
        }
        else
        {
            processHeaderTemplate(FTL_TEMPLATE_HEADER_NAME, templateData, structureType);
        }
    }

    private static final String FTL_HEADER_NAME = "Structure.h.ftl";
    private static final String FTL_SOURCE_NAME = "Structure.cpp.ftl";
    private static final String FTL_TEMPLATE_HEADER_NAME = "StructureTemplate.h.ftl";
}
