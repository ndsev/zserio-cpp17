package zserio.extension.cpp17;

import java.util.ArrayList;
import java.util.List;

import zserio.ast.TemplateParameter;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;

/**
 * FreeMarker template data for compound parameters.
 */
public final class CompoundTemplateParameterTemplateData
{
    public CompoundTemplateParameterTemplateData(TemplateDataContext context, TemplateParameter parameter)
    {
        name = parameter.getName();
        typeAliasName = AccessorNameFormatter.getTypeAliasName(parameter);
    }

    public String getName()
    {
        return name;
    }

    public String getTypeAliasName()
    {
        return typeAliasName;
    }

    private final String name;
    private final String typeAliasName;
}
