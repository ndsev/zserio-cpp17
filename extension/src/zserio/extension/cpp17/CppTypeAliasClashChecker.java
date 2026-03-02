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
 * Clash checker for clashing of template parameters, compound name and various type aliases.
 *
 * Checks that C++ code generator will not produce any clashes between template parameters, compound name and
 * type aliases. Clashes would lead to a C++ compiler error.
 */
class CppTypeAliasClashChecker extends DefaultTreeWalker
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

    @Override
    public void beginSqlTable(SqlTableType tableType) throws ZserioExtensionException
    {
        checkCompound(tableType);
    }

    private void checkCompound(CompoundType compoundType) throws ZserioExtensionException
    {
        for (zserio.ast.Field field : compoundType.getFields())
        {
            final TypeInstantiation typeInst = field.getTypeInstantiation();
            if (!(typeInst instanceof ArrayInstantiation))
                continue;
            String aliasName = AccessorNameFormatter.getTypeAliasName(field);
            // check for a clash with compound name
            if (aliasName.equals(compoundType.getName()))
            {
                ZserioToolPrinter.printError(field.getLocation(),
                        "Type alias for a field '" + field.getName() + "' clashes with the compound name '" +
                                compoundType.getName() + "'.");
                throw new ZserioExtensionException("Type alias clash detected!");
            }
            // check for a clash between field type aliases
            for (zserio.ast.Field field2 : compoundType.getFields())
            {
                if (field2 == field)
                    continue;
                final TypeInstantiation typeInst2 = field2.getTypeInstantiation();
                if (!(typeInst2 instanceof ArrayInstantiation))
                    continue;
                String aliasName2 = AccessorNameFormatter.getTypeAliasName(field2);
                if (aliasName.equals(aliasName2))
                {
                    ZserioToolPrinter.printError(field.getLocation(),
                            "Type alias for field '" + field.getName() +
                                    "' clashes with type alias for another field '" + field2.getName() +
                                    "'.");
                    throw new ZserioExtensionException("Type alias clash detected!");
                }
            }
        }
    }
}
