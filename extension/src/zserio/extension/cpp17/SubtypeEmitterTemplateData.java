package zserio.extension.cpp17;

import zserio.ast.Subtype;
import zserio.ast.TypeReference;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;

/**
 * FreeMarker template data for SubtypeEmitter.
 */
public final class SubtypeEmitterTemplateData extends UserTypeTemplateData
{
    public SubtypeEmitterTemplateData(TemplateDataContext context, Subtype subtype)
            throws ZserioExtensionException
    {
        super(context, subtype, subtype);

        final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();

        final TypeReference subtypeTypeReference = subtype.getTypeReference();
        final CppNativeType targetNativeType = cppNativeMapper.getCppType(subtypeTypeReference);
        addHeaderIncludesForType(targetNativeType);

        targetTypeInfo = NativeTypeInfoTemplateDataCreator.create(targetNativeType, subtypeTypeReference);
    }

    public NativeTypeInfoTemplateData getTargetTypeInfo()
    {
        return targetTypeInfo;
    }

    private final NativeTypeInfoTemplateData targetTypeInfo;
}
