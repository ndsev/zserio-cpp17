package zserio.extension.cpp17;

import zserio.ast.BitmaskType;
import zserio.ast.BooleanType;
import zserio.ast.BytesType;
import zserio.ast.DynamicBitFieldInstantiation;
import zserio.ast.DynamicBitFieldType;
import zserio.ast.EnumType;
import zserio.ast.Expression;
import zserio.ast.ExternType;
import zserio.ast.FloatType;
import zserio.ast.IntegerType;
import zserio.ast.StringType;
import zserio.ast.TypeInstantiation;
import zserio.ast.TypeReference;
import zserio.ast.ZserioType;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;
import zserio.extension.cpp17.types.NativeDynamicBitFieldType;

public final class NativeTypeInfoTemplateDataCreator
{
    public static NativeTypeInfoTemplateData create(
            CppNativeType nativeType, TypeInstantiation typeInstantiation) throws ZserioExtensionException
    {
        final ZserioType baseType = typeInstantiation.getBaseType();

        boolean isDynamicBitField = nativeType instanceof NativeDynamicBitFieldType;

        final String typeFullName = nativeType.getFullName();
        return create(typeFullName, nativeType, baseType, isDynamicBitField);
    }

    public static NativeTypeInfoTemplateData create(CppNativeType nativeType, TypeReference typeReference)
    {
        final ZserioType baseType = typeReference.getBaseTypeReference().getType();
        return create(nativeType, baseType);
    }

    public static NativeTypeInfoTemplateData create(CppNativeType nativeType, ZserioType baseType)
    {
        boolean isDynamicBitLength = false;
        if (baseType instanceof DynamicBitFieldType)
        {
            // when no type instantiation is available, we always get dynamic bit field (w/o the known length)
            isDynamicBitLength = true;
        }

        final String typeFullName = nativeType.getFullName();
        return create(typeFullName, nativeType, baseType, isDynamicBitLength);
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
        final boolean needsAllocator = !isSimple;

        return new NativeTypeInfoTemplateData(typeFullName, isSimple, isNumeric, isDynamicBitField, isEnum,
                isBitmask, isBoolean, isString, isExtern, isBytes, needsAllocator);
    }
};
