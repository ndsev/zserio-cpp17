package zserio.extension.cpp17;

import java.util.ArrayList;
import java.util.List;
import java.util.StringJoiner;

import zserio.ast.CompoundType;
import zserio.ast.DocComment;
import zserio.ast.Field;
import zserio.ast.Function;
import zserio.ast.Parameter;
import zserio.ast.TemplateParameter;
import zserio.extension.common.ZserioExtensionException;

/**
 * FreeMarker template data for compound types.
 */
public class CompoundTypeTemplateData extends UserTypeTemplateData
{
    public CompoundTypeTemplateData(TemplateDataContext context, CompoundType compoundType)
            throws ZserioExtensionException
    {
        super(context, compoundType, compoundType);

        final IncludeCollector includeCollector =
                compoundType.getTemplateParameters().isEmpty() ? this : new HeaderIncludeCollectorAdapter(this);

        final StringJoiner fullNameTemplateParameters = new StringJoiner(", ", "<", ">");
        fullNameTemplateParameters.setEmptyValue("");
        for (TemplateParameter templateParameter : compoundType.getTemplateParameters())
        {
            templateParameters.add(templateParameter.getName());
            fullNameTemplateParameters.add(templateParameter.getName());
        }

        fullName = super.getFullName() + fullNameTemplateParameters.toString();

        usedInPackedArray = context.getPackedTypesCollector().isUsedInPackedArray(compoundType);
        containsOffset = context.getOffsetFieldsCollector().containsOffset(compoundType);

        fieldList = createFieldList(context, compoundType, includeCollector);

        boolean usedAsOffset = false;
        for (CompoundFieldTemplateData field : fieldList)
        {
            if (field.getUsedAsOffset())
            {
                usedAsOffset = true;
                break;
            }
        }
        this.usedAsOffset = usedAsOffset;

        parameterList = createParameterList(context, compoundType, includeCollector);
        functionList = createFunctionList(context, compoundType, includeCollector);

        isPackable = compoundType.isPackable();

        templateInstantiation =
                TemplateInstantiationTemplateData.create(context, compoundType, includeCollector);
    }

    @Override
    public String getFullName()
    {
        return fullName;
    }

    public List<String> getTemplateParameters()
    {
        return templateParameters;
    }

    public boolean getUsedInPackedArray()
    {
        return usedInPackedArray;
    }

    public boolean getContainsOffset()
    {
        return containsOffset;
    }

    public boolean getUsedAsOffset()
    {
        return usedAsOffset;
    }

    public Iterable<CompoundFieldTemplateData> getFieldList()
    {
        return fieldList;
    }

    public Iterable<CompoundParameterTemplateData> getParameterList()
    {
        return parameterList;
    }

    public Iterable<CompoundFunctionTemplateData> getFunctionList()
    {
        return functionList;
    }

    public boolean getIsPackable()
    {
        return isPackable;
    }

    public TemplateInstantiationTemplateData getTemplateInstantiation()
    {
        return templateInstantiation;
    }

    private static List<CompoundFieldTemplateData> createFieldList(TemplateDataContext context,
            CompoundType compoundType, IncludeCollector includeCollector) throws ZserioExtensionException
    {
        final List<Field> fieldTypeList = compoundType.getFields();
        final List<CompoundFieldTemplateData> fieldList =
                new ArrayList<CompoundFieldTemplateData>(fieldTypeList.size());
        for (Field fieldType : fieldTypeList)
        {
            final CompoundFieldTemplateData data =
                    new CompoundFieldTemplateData(context, compoundType, fieldType, includeCollector);

            fieldList.add(data);
        }
        return fieldList;
    }

    static List<CompoundParameterTemplateData> createParameterList(TemplateDataContext context,
            CompoundType compoundType, IncludeCollector includeCollector) throws ZserioExtensionException
    {
        final List<Parameter> parameterTypeList = compoundType.getTypeParameters();
        final List<CompoundParameterTemplateData> parameterList =
                new ArrayList<CompoundParameterTemplateData>(parameterTypeList.size());
        final List<DocComment> compoundDocComments = compoundType.getDocComments();
        for (Parameter parameterType : parameterTypeList)
        {
            final CompoundParameterTemplateData parameter = new CompoundParameterTemplateData(
                    context, parameterType, includeCollector, compoundDocComments);
            parameterList.add(parameter);
        }
        return parameterList;
    }

    private static List<CompoundFunctionTemplateData> createFunctionList(TemplateDataContext context,
            CompoundType compoundType, IncludeCollector includeCollector) throws ZserioExtensionException
    {
        final List<CompoundFunctionTemplateData> functionList = new ArrayList<CompoundFunctionTemplateData>();
        final Iterable<Function> functionTypeList = compoundType.getFunctions();
        for (Function functionType : functionTypeList)
        {
            functionList.add(new CompoundFunctionTemplateData(context, functionType, includeCollector));
        }

        return functionList;
    }

    private final List<String> templateParameters = new ArrayList<String>();
    private final String fullName;

    private final boolean usedInPackedArray;
    private final boolean containsOffset;
    private final boolean usedAsOffset;

    private final List<CompoundFieldTemplateData> fieldList;
    private final List<CompoundParameterTemplateData> parameterList;
    private final List<CompoundFunctionTemplateData> functionList;

    private final boolean isPackable;

    private final TemplateInstantiationTemplateData templateInstantiation;
}
