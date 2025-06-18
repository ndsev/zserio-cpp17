package zserio.extension.cpp17;

import java.util.StringJoiner;

import zserio.ast.BitmaskType;
import zserio.ast.BooleanType;
import zserio.ast.BytesType;
import zserio.ast.DynamicBitFieldType;
import zserio.ast.EnumType;
import zserio.ast.ExternType;
import zserio.ast.FloatType;
import zserio.ast.IntegerType;
import zserio.ast.StringType;
import zserio.ast.TemplateArgument;
import zserio.ast.TemplateParameter;
import zserio.ast.TypeInstantiation;
import zserio.ast.TypeReference;
import zserio.ast.ZserioType;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;
import zserio.extension.cpp17.types.NativeDynamicBitFieldType;

public final class NativeTypeInfoTemplateDataCreator
{
    public static NativeTypeInfoTemplateData create(TemplateDataContext context, CppNativeType nativeType,
            TypeInstantiation typeInstantiation) throws ZserioExtensionException
    {
        // use this create method whenever the typeInstantiation is available!

        final String typeFullName =
                createTypeFullName(context, nativeType, typeInstantiation.getTypeReference());
        boolean isDynamicBitField = nativeType instanceof NativeDynamicBitFieldType;
        return create(typeFullName, nativeType, typeInstantiation.getBaseType(), isDynamicBitField);
    }

    public static NativeTypeInfoTemplateData create(TemplateDataContext context, CppNativeType nativeType,
            TypeReference typeReference) throws ZserioExtensionException
    {
        final String typeFullName = createTypeFullName(context, nativeType, typeReference);
        final ZserioType baseType = typeReference.getBaseTypeReference().getType();
        // type instantiation not available, always a dynamic bit field
        final boolean isDynamicBitField = baseType instanceof DynamicBitFieldType;
        return create(typeFullName, nativeType, baseType, isDynamicBitField);
    }

    public static NativeTypeInfoTemplateData create(CppNativeType nativeType, ZserioType baseType)
    {
        final String typeFullName = nativeType.getFullName();
        // type instantiation not available, always a dynamic bit field
        final boolean isDynamicBitField = baseType instanceof DynamicBitFieldType;
        return create(typeFullName, nativeType, baseType, isDynamicBitField);
    }

    private static String createTypeFullName(TemplateDataContext context, CppNativeType nativeType,
            TypeReference typeReference) throws ZserioExtensionException
    {
        final StringJoiner templateArguments = new StringJoiner(", ", "<", ">");
        templateArguments.setEmptyValue("");
        if (!typeReference.getTemplateArguments().isEmpty())
        {
            final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
            for (TemplateArgument templateArgument : typeReference.getTemplateArguments())
            {
                final CppNativeType argumentNativeType =
                        cppNativeMapper.getCppType(templateArgument.getTypeReference());
                final NativeTypeInfoTemplateData templateArgumentTypeInfo =
                        NativeTypeInfoTemplateDataCreator.create(
                                context, argumentNativeType, templateArgument.getTypeReference());
                templateArguments.add(templateArgumentTypeInfo.getTypeFullName());
            }
        }

        return nativeType.getFullName() + templateArguments.toString();
    }

    private static NativeTypeInfoTemplateData create(
            String typeFullName, CppNativeType nativeType, ZserioType baseType, boolean isDynamicBitField)
    {
        // we suppose that zserio enum, bitmask, etc. are mapped to C++ native enum, bitmask, etc.
        // normally, we should consult native type here (in this case we should solve subtypes as well)
        final boolean isInteger = baseType instanceof IntegerType;
        final boolean isFloat = baseType instanceof FloatType;
        final boolean isEnum = baseType instanceof EnumType;
        final boolean isBitmask = baseType instanceof BitmaskType;
        final boolean isBoolean = baseType instanceof BooleanType;
        final boolean isString = baseType instanceof StringType;
        final boolean isExtern = baseType instanceof ExternType;
        final boolean isBytes = baseType instanceof BytesType;
        final boolean isNumeric = isInteger || isFloat || isBoolean;
        final boolean isSimple = isNumeric || isEnum || isBitmask;
        final boolean isTemplateParameter = baseType instanceof TemplateParameter;
        final boolean needsAllocator = !isSimple;

        return new NativeTypeInfoTemplateData(typeFullName, isSimple, isNumeric, isDynamicBitField, isEnum,
                isBitmask, isBoolean, isString, isExtern, isBytes, isTemplateParameter, needsAllocator);
    }
};
