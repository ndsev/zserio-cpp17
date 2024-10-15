package zserio.extension.cpp17;

import zserio.ast.ChoiceType;
import zserio.extension.common.ZserioExtensionException;

/**
 * FreeMarker template data for UnionEmitter.
 */
public final class ChoiceEmitterTemplateData extends CompoundTypeTemplateData
{
    public ChoiceEmitterTemplateData(TemplateDataContext context, ChoiceType choiceType)
            throws ZserioExtensionException
    {
        super(context, choiceType);
    }
}
