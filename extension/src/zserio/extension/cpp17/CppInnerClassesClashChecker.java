package zserio.extension.cpp17;

import java.util.Arrays;
import java.util.List;

import zserio.ast.BitmaskType;
import zserio.ast.ChoiceType;
import zserio.ast.CompoundType;
import zserio.ast.SqlTableType;
import zserio.ast.StructureType;
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
final class CppInnerClassesClashChecker extends DefaultTreeWalker
{
    @Override
    public boolean traverseTemplateInstantiations()
    {
        // we need to check class names with their inner classes names
        return true;
    }

    @Override
    public void beginBitmask(BitmaskType bitmaskType) throws ZserioExtensionException
    {
        final String className = bitmaskType.getName();
        if (BITMASK_INNER_CLASS_NAME.equals(className))
        {
            ZserioToolPrinter.printError(bitmaskType.getLocation(),
                    "Class name '" + className + "' generated for bitmask clashes with "
                            + "its inner class '" + className + "' generated in C++ code.");
            throw new ZserioExtensionException("Class name clash detected!");
        }
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
    public void beginSqlTable(SqlTableType sqlTableType) throws ZserioExtensionException
    {
        final String className = sqlTableType.getName();
        if (SQL_TABLE_INNER_CLASSES_NAMES.contains(className))
        {
            ZserioToolPrinter.printError(sqlTableType.getLocation(),
                    "Class name '" + className + "' generated for SQL table clashes with "
                            + "its inner class '" + className + "' generated in C++ code.");
            throw new ZserioExtensionException("Class name clash detected!");
        }
    }

    private void checkCompound(CompoundType compoundType) throws ZserioExtensionException
    {
        final String className = compoundType.getName();
        if (COMPOUND_INNER_TYPEDEF_NAME.equals(className))
        {
            ZserioToolPrinter.printError(compoundType.getLocation(),
                    "Class name '" + className + "' generated for compound clashes with "
                            + "it's inner typedef '" + className + "' generated in C++ code.");
            throw new ZserioExtensionException("Class name clash detected!");
        }
    }

    private static final String BITMASK_INNER_CLASS_NAME = "Values";
    private static final String COMPOUND_INNER_TYPEDEF_NAME = "AllocatorType";
    private static final List<String> SQL_TABLE_INNER_CLASSES_NAMES =
            Arrays.asList("IParameterProvider", "Reader", "Row");
}
