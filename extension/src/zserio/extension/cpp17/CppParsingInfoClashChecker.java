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
 * Clash checker for clashing of ParsingInfo field and template friend parameter in compounds.
 *
 * Checks that C++ code generator will not produce any clashes with ParsingInfo generated members
 * because it would lead to a C++ compilation error.
 */
class CppParsingInfoClashChecker extends DefaultTreeWalker
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

    private void checkCompound(CompoundType compoundType, String description) throws ZserioExtensionException
    {
        for (Field field : compoundType.getFields())
        {
            if (field.getName().equals("m_parsingInfo"))
            {
                ZserioToolPrinter.printError(field.getLocation(),
                        "Field '" + field.getName() +
                                "' clashes with generated m_parsingInfo field (-withParsingInfoCode).");
                throw new ZserioExtensionException("Field name clash detected!");
            }
        }

        // no need to test for template parameter clash. Template parameter uses 'zserio' prefix
        // which is banned everywhere.
    }
}
