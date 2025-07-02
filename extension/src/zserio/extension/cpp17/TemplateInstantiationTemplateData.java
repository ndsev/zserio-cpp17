package zserio.extension.cpp17;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import zserio.ast.TemplateArgument;
import zserio.ast.TypeReference;
import zserio.ast.ZserioTemplatableType;
import zserio.ast.ZserioTypeUtil;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;

/**
 * FreeMarker template data for template instantiations.
 */
public final class TemplateInstantiationTemplateData
{
    public TemplateInstantiationTemplateData(TemplateDataContext context, ZserioTemplatableType template,
            List<String> templateParameters, List<NativeTypeInfoTemplateData> templateArgumentTypeInfos,
            IncludeCollector includeCollector) throws ZserioExtensionException
    {
        templateName = ZserioTypeUtil.getFullName(template);
        this.templateArgumentTypeInfos =templateArgumentTypeInfos;
        this.templateParameters = templateParameters;
    }

    public String getTemplateName()
    {
        return templateName;
    }

    public List<String> getTemplateParameters()
    {
        return templateParameters;
    }

    public Iterable<NativeTypeInfoTemplateData> getTemplateArgumentTypeInfos()
    {
        return templateArgumentTypeInfos;
    }

    static TemplateInstantiationTemplateData create(
            TemplateDataContext context, ZserioTemplatableType templatable, List<String> templateParameters,
            IncludeCollector includeCollector) throws ZserioExtensionException
    {
        if (!templatable.getTemplateParameters().isEmpty())
        {
            return new TemplateInstantiationTemplateData(
                    context, templatable, templateParameters, null, includeCollector);
        }
        else if (templatable.getTemplate() != null)
        {
            final Iterator<TypeReference> instantiationReferenceIterator =
                    templatable.getInstantiationReferenceStack().iterator();
            if (!instantiationReferenceIterator.hasNext())
            {
                return null; // should not occur
            }

            final List<NativeTypeInfoTemplateData> templateArgumentTypeInfos =
                    new ArrayList<NativeTypeInfoTemplateData>();
            final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
            for (TemplateArgument templateArgument :
                    instantiationReferenceIterator.next().getTemplateArguments())
            {
                final CppNativeType argumentNativeType =
                        cppNativeMapper.getCppType(templateArgument.getTypeReference());
                templateArgumentTypeInfos.add(NativeTypeInfoTemplateDataCreator.create(
                        context, argumentNativeType, templateArgument.getTypeReference()));
            }

            return new TemplateInstantiationTemplateData(context, templatable.getTemplate(),
                    null, templateArgumentTypeInfos, includeCollector);
        }

        return null;
    }

    private final String templateName;
    private final List<String> templateParameters;
    private final List<NativeTypeInfoTemplateData> templateArgumentTypeInfos;
}
