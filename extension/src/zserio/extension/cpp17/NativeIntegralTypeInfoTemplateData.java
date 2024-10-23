package zserio.extension.cpp17;

import zserio.ast.DynamicBitFieldInstantiation;
import zserio.ast.TypeInstantiation;
import zserio.extension.common.ExpressionFormatter;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.NativeDynamicBitFieldType;
import zserio.extension.cpp17.types.NativeIntegralType;

/**
 * FreeMarker template data with info about integral types.
 */
public final class NativeIntegralTypeInfoTemplateData extends NativeTypeInfoTemplateData
{
    public NativeIntegralTypeInfoTemplateData(NativeIntegralType nativeBaseType,
            TypeInstantiation typeInstantiation, ExpressionFormatter expressionFormatter)
            throws ZserioExtensionException
    {
        super(nativeBaseType, typeInstantiation);

        final StringBuilder typeTemplateArgBuilder = new StringBuilder();
        if (nativeBaseType instanceof NativeDynamicBitFieldType &&
                typeInstantiation instanceof DynamicBitFieldInstantiation)
        {
            typeTemplateArgBuilder.append('<');
            final DynamicBitFieldInstantiation dynamicBitFieldInstantiation =
                    (DynamicBitFieldInstantiation)typeInstantiation;
            if (dynamicBitFieldInstantiation.getLengthExpression().getIntegerValue() != null)
            {
                typeTemplateArgBuilder.append(
                        expressionFormatter.formatGetter(dynamicBitFieldInstantiation.getLengthExpression()));
            }
            typeTemplateArgBuilder.append('>');
        }
        typeTemplateArg = typeTemplateArgBuilder.toString();

        typeNumBits = nativeBaseType.getNumBits();
        isSigned = nativeBaseType.isSigned();
    }

    @Override
    public String getTypeFullName()
    {
        return super.getTypeFullName() + typeTemplateArg;
    }

    public int getTypeNumBits()
    {
        return typeNumBits;
    }

    public boolean getIsSigned()
    {
        return isSigned;
    }

    private final String typeTemplateArg;
    private final int typeNumBits;
    private final boolean isSigned;
}
