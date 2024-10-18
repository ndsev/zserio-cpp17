package zserio.extension.cpp17;

import zserio.ast.TypeInstantiation;
import zserio.extension.common.ExpressionFormatter;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;
import zserio.extension.cpp17.types.NativeIntegralType;

public final class NativeTypeInfoTemplateDataCreator
{
    public static NativeTypeInfoTemplateData create(TemplateDataContext context, CppNativeType nativeType,
            TypeInstantiation typeInstantiation, IncludeCollector includeCollector)
            throws ZserioExtensionException
    {
        if (nativeType instanceof NativeIntegralType)
        {
            return new NativeIntegralTypeInfoTemplateData((NativeIntegralType)nativeType, typeInstantiation,
                    context.getExpressionFormatter(includeCollector));
        }
        else
        {
            return new NativeTypeInfoTemplateData(nativeType, typeInstantiation);
        }
    }

    public static NativeIntegralTypeInfoTemplateData createIntegral(NativeIntegralType nativeIntegralType,
            TypeInstantiation typeInstantiation, ExpressionFormatter expressionFormatter)
            throws ZserioExtensionException
    {
        return new NativeIntegralTypeInfoTemplateData(
                nativeIntegralType, typeInstantiation, expressionFormatter);
    }
};