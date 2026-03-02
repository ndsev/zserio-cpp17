package zserio.extension.cpp17;

import java.util.Arrays;
import java.util.List;

import zserio.ast.ArrayInstantiation;
import zserio.ast.ChoiceType;
import zserio.ast.CompoundType;
import zserio.ast.Field;
import zserio.ast.SqlTableType;
import zserio.ast.StructureType;
import zserio.ast.TemplateParameter;
import zserio.ast.TypeInstantiation;
import zserio.ast.UnionType;
import zserio.extension.common.DefaultTreeWalker;
import zserio.extension.common.ZserioExtensionException;
import zserio.tools.ZserioToolPrinter;

/**
 * Clash checker for clashing of View type alias, compound name and template parameters.
 *
 * Checks that C++ code generator will not produce any clashes between View type alias, template parameters,
 * compound name Clashes would lead to a C++ compiler error.
 */
class CppViewAliasClashChecker extends DefaultTreeWalker
{
    @Override
    public boolean traverseTemplateInstantiations()
    {
        return false;
    }

    @Override
    public void beginStructure(StructureType structureType) throws ZserioExtensionException
    {
        checkCompound(structureType, "Structure");
    }

    @Override
    public void beginChoice(ChoiceType choiceType) throws ZserioExtensionException
    {
        checkCompound(choiceType, "Choice");
    }

    @Override
    public void beginUnion(UnionType unionType) throws ZserioExtensionException
    {
        checkCompound(unionType, "Union");
    }

    @Override
    public void beginSqlTable(SqlTableType tableType) throws ZserioExtensionException
    {
        checkCompound(tableType, "SqlTable");
    }

    private void checkCompound(CompoundType compoundType, String description) throws ZserioExtensionException
    {
        final String viewAlias = "View";

        // check for a clash with compound name
        if (compoundType.getName().equals(viewAlias))
        {
            // This error may show up for users of other language generators due to -withCrossExtensionCheck.
            // The description is synchronized with ReservedKeywordsClashChecker
            ZserioToolPrinter.printError(compoundType.getLocation(),
                    description + " '" + compoundType.getName() +
                            "' clashes with alias 'View' and may not be used as an identifier!");
            throw new ZserioExtensionException("View alias clash detected!");
        }

        for (TemplateParameter param : compoundType.getTemplateParameters())
        {
            // check for a clash with template parameter
            if (param.getName().equals(viewAlias))
            {
                ZserioToolPrinter.printError(param.getLocation(),
                        "Template parameter '" + param.getName() + "' clashes with the View alias.");
                throw new ZserioExtensionException("View alias clash detected!");
            }
        }
    }
}
