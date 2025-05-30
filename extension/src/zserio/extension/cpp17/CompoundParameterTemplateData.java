package zserio.extension.cpp17;

import java.util.ArrayList;
import java.util.List;

import zserio.ast.DocComment;
import zserio.ast.Parameter;
import zserio.ast.TypeReference;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;

/**
 * FreeMarker template data for compound parameters.
 */
public final class CompoundParameterTemplateData
{
    public CompoundParameterTemplateData(TemplateDataContext context, Parameter parameter,
            IncludeCollector includeCollector, List<DocComment> compoundDocComments)
            throws ZserioExtensionException
    {
        final TypeReference parameterTypeReference = parameter.getTypeReference();
        final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
        final CppNativeType cppNativeType = cppNativeMapper.getCppType(parameterTypeReference);
        includeCollector.addHeaderIncludesForType(cppNativeType);

        name = parameter.getName();
        typeInfo = NativeTypeInfoTemplateDataCreator.create(context, cppNativeType, parameterTypeReference);
        getterName = AccessorNameFormatter.getGetterName(parameter);

        final List<DocComment> paramDocComments = new ArrayList<DocComment>();
        for (DocComment compoundDocComment : compoundDocComments)
        {
            final DocComment paramDocComment = compoundDocComment.findParamDoc(name);
            if (paramDocComment != null)
                paramDocComments.add(paramDocComment);
        }
        docComments = DocCommentsDataCreator.createData(context, paramDocComments);
    }

    public String getName()
    {
        return name;
    }

    public NativeTypeInfoTemplateData getTypeInfo()
    {
        return typeInfo;
    }

    public String getGetterName()
    {
        return getterName;
    }

    public DocCommentsTemplateData getDocComments()
    {
        return docComments;
    }

    private final String name;
    private final NativeTypeInfoTemplateData typeInfo;
    private final String getterName;
    private final DocCommentsTemplateData docComments;
}
