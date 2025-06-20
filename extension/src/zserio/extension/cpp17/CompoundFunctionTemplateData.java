package zserio.extension.cpp17;

import zserio.ast.Function;
import zserio.ast.StringType;
import zserio.ast.TypeReference;
import zserio.ast.ZserioType;
import zserio.extension.common.ExpressionFormatter;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;
import zserio.extension.cpp17.types.NativeStringViewType;

/**
 * FreeMarker template data for compound functions.
 */
public final class CompoundFunctionTemplateData
{
    public CompoundFunctionTemplateData(TemplateDataContext context, Function function,
            IncludeCollector includeCollector) throws ZserioExtensionException
    {
        final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
        final TypeReference returnTypeReference = function.getReturnTypeReference();
        final ZserioType returnBaseType = returnTypeReference.getBaseTypeReference().getType();
        if (returnBaseType instanceof StringType)
        {
            // we have to return strings as string_view because we are not able to find out whether
            // the expression always leads to some field or whether it contains any string constant
            // (the expression can contain e.g. another function call, constant, etc.)
            final NativeStringViewType nativeStringViewType = cppNativeMapper.getStringViewType();
            includeCollector.addHeaderIncludesForType(nativeStringViewType);
            returnTypeInfo = NativeTypeInfoTemplateDataCreator.create(
                    context, nativeStringViewType, returnTypeReference);
        }
        else
        {
            final CppNativeType returnNativeType = cppNativeMapper.getCppType(returnTypeReference);
            IncludeCollector.addHeaderIncludes(
                    cppNativeMapper, returnNativeType, returnTypeReference, includeCollector);
            returnTypeInfo =
                    NativeTypeInfoTemplateDataCreator.create(context, returnNativeType, returnTypeReference);
        }

        final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
        resultExpression = cppExpressionFormatter.formatGetter(function.getResultExpression());
        schemaName = function.getName();
        name = AccessorNameFormatter.getFunctionName(function);

        docComments = DocCommentsDataCreator.createData(context, function);
    }

    public NativeTypeInfoTemplateData getReturnTypeInfo()
    {
        return returnTypeInfo;
    }

    public String getResultExpression()
    {
        return resultExpression;
    }

    public String getSchemaName()
    {
        return schemaName;
    }

    public String getName()
    {
        return name;
    }

    public DocCommentsTemplateData getDocComments()
    {
        return docComments;
    }

    private final NativeTypeInfoTemplateData returnTypeInfo;
    private final String resultExpression;
    private final String schemaName;
    private final String name;
    private final DocCommentsTemplateData docComments;
}
