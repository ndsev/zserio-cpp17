package zserio.extension.cpp17;

import zserio.ast.InstantiateType;
import zserio.ast.TypeReference;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;

/**
 * FreeMarker template data for SubtypeEmitter.
 */
public final class InstantiateTypeEmitterTemplateData extends UserTypeTemplateData
{
    public InstantiateTypeEmitterTemplateData(TemplateDataContext context, InstantiateType instantiateType)
            throws ZserioExtensionException
    {
        super(context, instantiateType, instantiateType);

        final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();

        final TypeReference typeReference = instantiateType.getTypeReference();
        final CppNativeType cppNativeType = cppNativeMapper.getCppType(typeReference);
        IncludeCollector.addHeaderIncludes(cppNativeMapper, cppNativeType, typeReference, this);

        targetTypeInfo = NativeTypeInfoTemplateDataCreator.create(context, cppNativeType, typeReference);
    }

    public NativeTypeInfoTemplateData getTargetTypeInfo()
    {
        return targetTypeInfo;
    }

    private final NativeTypeInfoTemplateData targetTypeInfo;
}
