package zserio.extension.cpp17;

import zserio.ast.BitmaskType;
import zserio.ast.BooleanType;
import zserio.ast.BytesType;
import zserio.ast.DynamicBitFieldInstantiation;
import zserio.ast.DynamicBitFieldType;
import zserio.ast.EnumType;
import zserio.ast.ExternType;
import zserio.ast.StringType;
import zserio.ast.TypeInstantiation;
import zserio.ast.TypeReference;
import zserio.ast.ZserioType;
import zserio.extension.common.ExpressionFormatter;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;
import zserio.extension.cpp17.types.NativeDynamicBitFieldType;
import zserio.extension.cpp17.types.NativeIntegralType;
import zserio.extension.cpp17.types.NativeNumericWrapperType;

public final class NativeTypeInfoTemplateDataCreator
{
    public static NativeTypeInfoTemplateData create(TemplateDataContext context, CppNativeType nativeBaseType,
            TypeInstantiation typeInstantiation, IncludeCollector includeCollector)
            throws ZserioExtensionException
    {
        final ZserioType baseType = typeInstantiation.getBaseType();

        boolean isDynamicBitField = false;
        final StringBuilder typeTemplateArgBuilder = new StringBuilder();
        if (nativeBaseType instanceof NativeIntegralType)
        {
            final ExpressionFormatter expressionFormatter = context.getExpressionFormatter(includeCollector);
            if (nativeBaseType instanceof NativeDynamicBitFieldType &&
                    typeInstantiation instanceof DynamicBitFieldInstantiation)
            {
                typeTemplateArgBuilder.append('<');
                final DynamicBitFieldInstantiation dynamicBitFieldInstantiation =
                        (DynamicBitFieldInstantiation)typeInstantiation;
                if (dynamicBitFieldInstantiation.getLengthExpression().getIntegerValue() != null)
                {
                    typeTemplateArgBuilder.append(expressionFormatter.formatGetter(
                            dynamicBitFieldInstantiation.getLengthExpression()));
                }
                else
                {
                    isDynamicBitField = true;
                }
                typeTemplateArgBuilder.append('>');
            }
        }

        final String typeFullName = nativeBaseType.getFullName() + typeTemplateArgBuilder.toString();
        return create(typeFullName, nativeBaseType, baseType, isDynamicBitField);
    }

    public static NativeTypeInfoTemplateData create(CppNativeType nativeBaseType, TypeReference typeReference)
    {
        final ZserioType baseType = typeReference.getBaseTypeReference().getType();
        return create(nativeBaseType, baseType);
    }

    public static NativeTypeInfoTemplateData create(CppNativeType nativeBaseType, ZserioType baseType)
    {
        boolean isDynamicBitLength = false;
        final StringBuilder typeTemplateArgBuilder = new StringBuilder();
        if (baseType instanceof DynamicBitFieldType)
        {
            // when no type instantiation is available, we always get dynamic bit field (w/o the known length)
            typeTemplateArgBuilder.append("<>");
            isDynamicBitLength = true;
        }

        final String typeFullName = nativeBaseType.getFullName() + typeTemplateArgBuilder.toString();
        return create(typeFullName, nativeBaseType, baseType, isDynamicBitLength);
    }

    private static NativeTypeInfoTemplateData create(
            String typeFullName, CppNativeType nativeBaseType, ZserioType baseType, boolean isDynamicBitField)
    {
        final boolean isSimple = nativeBaseType.isSimple();
        final boolean isNumeric = nativeBaseType instanceof NativeNumericWrapperType;

        // we suppose that zserio enum, bitmask, etc. are mapped to C++ native enum, bitmask, etc.
        // normally, we should consult native type here (in this case we should solve subtypes as well)
        final boolean isEnum = baseType instanceof EnumType;
        final boolean isBitmask = baseType instanceof BitmaskType;
        final boolean isBoolean = baseType instanceof BooleanType;
        final boolean isString = baseType instanceof StringType;
        final boolean isExtern = baseType instanceof ExternType;
        final boolean isBytes = baseType instanceof BytesType;
        final boolean needsAllocator = isString || isExtern || isBytes || !isSimple;

        return new NativeTypeInfoTemplateData(typeFullName, isSimple, isNumeric, isDynamicBitField, isEnum,
                isBitmask, isBoolean, isString, isExtern, isBytes, needsAllocator);
    }
};
