package zserio.extension.cpp17;

import zserio.ast.ChoiceType;
import zserio.extension.common.OutputFileManager;
import zserio.extension.common.ZserioExtensionException;

/**
 * Choice emitter.
 */
public final class ChoiceEmitter extends CppDefaultEmitter
{
    public ChoiceEmitter(OutputFileManager outputFileManager, Cpp17ExtensionParameters cppParameters,
            TemplateDataContext context)
    {
        super(outputFileManager, cppParameters, context);
    }

    @Override
    public void beginChoice(ChoiceType choiceType) throws ZserioExtensionException
    {
        final Object templateData = new ChoiceEmitterTemplateData(getTemplateDataContext(), choiceType);

        if (choiceType.getTemplateParameters().isEmpty())
        {
            processHeaderTemplate(FTL_HEADER_NAME, templateData, choiceType);
            processSourceTemplate(FTL_SOURCE_NAME, templateData, choiceType);
        }
        else
        {
            processHeaderTemplate(FTL_TEMPLATE_HEADER_NAME, templateData, choiceType);
        }
    }

    private static final String FTL_HEADER_NAME = "Choice.h.ftl";
    private static final String FTL_SOURCE_NAME = "Choice.cpp.ftl";
    private static final String FTL_TEMPLATE_HEADER_NAME = "ChoiceTemplate.h.ftl";
}
