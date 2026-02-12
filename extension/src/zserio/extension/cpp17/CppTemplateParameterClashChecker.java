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
        for (zserio.ast.Field field : compoundType.getFields())
        {
            final TypeInstantiation typeInst = field.getTypeInstantiation();
            if (typeInst instanceof ArrayInstantiation)
            {
                String usingName = AccessorNameFormatter.getTypeAliasName(field);
                for (TemplateParameter tmpParam : compoundType.getTemplateParameters())
                {
                    if (tmpParam.getName().equals(usingName))
                    {
                        ZserioToolPrinter.printError(tmpParam.getLocation(),
                                "Template parameter '" + tmpParam.getName() +
                                        "' clashes with generated type alias for field '" + field.getName() +
                                        "'.");
                        throw new ZserioExtensionException("Template parameter clash detected!");
                    }
                }
            }
        }
    }
}
