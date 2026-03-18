package zserio.extension.cpp17;

import zserio.ast.StructureType;
import zserio.extension.common.ZserioExtensionException;

/**
 * FreeMarker template data for StructureEmitter.
 */
public final class StructureEmitterTemplateData extends CompoundTypeTemplateData
{
    private StructureEmitterTemplateData(TemplateDataContext context, StructureType structureType)
            throws ZserioExtensionException
    {
        super(context, structureType);
    }

    public static StructureEmitterTemplateData create(TemplateDataContext context, StructureType structureType)
            throws ZserioExtensionException
    {
        final StructureEmitterTemplateData self = new StructureEmitterTemplateData(context, structureType);
        self.init(context, structureType);
        return self;
    }
}
