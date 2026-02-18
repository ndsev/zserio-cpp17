package zserio.extension.cpp17;

import java.util.Arrays;
import java.util.List;

import zserio.ast.ArrayInstantiation;
import zserio.ast.ChoiceType;
import zserio.ast.CompoundType;
import zserio.ast.Field;
import zserio.ast.StructureType;
import zserio.ast.TemplateParameter;
import zserio.ast.TypeInstantiation;
import zserio.ast.UnionType;
import zserio.extension.common.DefaultTreeWalker;
import zserio.extension.common.ZserioExtensionException;
import zserio.tools.ZserioToolPrinter;

/**
 * Clash checker for clashing of classes generated for zserio types with theirs inner classes.
 *
 * Checks that C++ code generator will not produce any clashes between generated classes and inner classes.
 * Note that C++ doesn't allow an inner class to have same name as its outer class.
 */
final class CppTemplateParameterClashChecker extends DefaultTreeWalker
{
    @Override
    public boolean traverseTemplateInstantiations()
    {
        return false;
    }

    @Override
    public void beginStructure(StructureType structureType) throws ZserioExtensionException
    {
        checkCompound(structureType);
    }

    @Override
    public void beginChoice(ChoiceType choiceType) throws ZserioExtensionException
    {
        checkCompound(choiceType);
    }

    @Override
    public void beginUnion(UnionType unionType) throws ZserioExtensionException
    {
        checkCompound(unionType);
    }

    private void checkCompound(CompoundType compoundType) throws ZserioExtensionException
    {
        for (TemplateParameter param : compoundType.getTemplateParameters())
        {
            // check for a clash with its alias - impossible when getTypeAliasName adds Type suffix
            String paramAlias = AccessorNameFormatter.getTypeAliasName(param);
            if (paramAlias.equals(param.getName()))
            {
                ZserioToolPrinter.printError(param.getLocation(),
                        "Type alias for a template parameter '" + param.getName() +
                                "' clashes with itself. Use uppercase convention or longer parameter names.");
                throw new ZserioExtensionException("Template parameter clash detected!");
            }
            // check for a clash with compound name
            if (paramAlias.equals(compoundType.getName()))
            {
                ZserioToolPrinter.printError(param.getLocation(),
                        "Type alias for a template parameter '" + param.getName() +
                                "' clashes with the compound name '" + compoundType.getName() + "'.");
                throw new ZserioExtensionException("Template parameter clash detected!");
            }
            // check for a clash with other template parameters
            for (TemplateParameter otherParam : compoundType.getTemplateParameters())
            {
                if (otherParam == param)
                    continue;
                if (AccessorNameFormatter.getTypeAliasName(otherParam).equals(paramAlias))
                {
                    ZserioToolPrinter.printError(param.getLocation(),
                            "Type alias for a template parameter '" + param.getName() +
                                    "' clashes with type alias for another template parameter '" +
                                    otherParam.getName() + "'.");
                    throw new ZserioExtensionException("Template parameter clash detected!");
                }
            }
            // check for a clash with field type aliases
            for (zserio.ast.Field field : compoundType.getFields())
            {
                final TypeInstantiation typeInst = field.getTypeInstantiation();
                if (typeInst instanceof ArrayInstantiation)
                {
                    String aliasName = AccessorNameFormatter.getTypeAliasName(field);
                    if (param.getName().equals(aliasName))
                    {
                        ZserioToolPrinter.printError(param.getLocation(),
                                "Template parameter '" + param.getName() +
                                        "' clashes with type alias for field '" + field.getName() + "'.");
                        throw new ZserioExtensionException("Template parameter clash detected!");
                    }
                }
            }
        }
    }
}
