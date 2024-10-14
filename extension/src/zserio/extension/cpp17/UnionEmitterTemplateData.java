package zserio.extension.cpp17;

import zserio.ast.UnionType;
import zserio.extension.common.ZserioExtensionException;

/**
 * FreeMarker template data for UnionEmitter.
 */
public final class UnionEmitterTemplateData extends CompoundTypeTemplateData
{
    public UnionEmitterTemplateData(TemplateDataContext context, UnionType unionType)
            throws ZserioExtensionException
    {
        super(context, unionType);
    }
}
