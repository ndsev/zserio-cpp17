package zserio.extension.cpp17;

import zserio.extension.common.ExpressionFormatter;
import zserio.extension.common.ExpressionFormattingPolicy;
import zserio.extension.common.PackedTypesCollector;

/**
 * FreeMarker template data context for all emitters.
 *
 * Holds all parameters used by FreeMarker template data.
 */
final class TemplateDataContext
{
    public TemplateDataContext(
            Cpp17ExtensionParameters cppParameters, PackedTypesCollector packedTypesCollector)
    {
        this.packedTypesCollector = packedTypesCollector;

        cppNativeMapper = new CppNativeMapper();

        withReflectionCode = cppParameters.getWithReflectionCode();

        generatorDescription = "/**\n"
                + " * Automatically generated by Zserio C++17 generator version " +
                Cpp17ExtensionVersion.CPP17_EXTENSION_VERSION_STRING + " using Zserio core " +
                cppParameters.getZserioVersion() + ".\n"
                + " * Generator setup: " + cppParameters.getParametersDescription() + ".\n"
                + " */";
        generatorVersionString = Cpp17ExtensionVersion.CPP17_EXTENSION_VERSION_STRING;
        generatorVersionNumber = Cpp17ExtensionVersion.CPP17_EXTENSION_VERSION_NUMBER;
    }

    public PackedTypesCollector getPackedTypesCollector()
    {
        return packedTypesCollector;
    }

    public CppNativeMapper getCppNativeMapper()
    {
        return cppNativeMapper;
    }

    public ExpressionFormatter getExpressionFormatter(IncludeCollector includeCollector)
    {
        final ExpressionFormattingPolicy expressionFormattingPolicy =
                new CppExpressionFormattingPolicy(cppNativeMapper, includeCollector);

        return new ExpressionFormatter(expressionFormattingPolicy);
    }

    public ExpressionFormatter getIndirectExpressionFormatter(
            IncludeCollector includeCollector, String compoundTypeAccessPrefix)
    {
        final ExpressionFormattingPolicy expressionFormattingPolicy = new CppIndirectExpressionFormattingPolicy(
                cppNativeMapper, includeCollector, compoundTypeAccessPrefix);

        return new ExpressionFormatter(expressionFormattingPolicy);
    }

    public boolean getWithReflectionCode()
    {
        return withReflectionCode;
    }

    public String getGeneratorDescription()
    {
        return generatorDescription;
    }

    public String getGeneratorVersionString()
    {
        return generatorVersionString;
    }

    public long getGeneratorVersionNumber()
    {
        return generatorVersionNumber;
    }

    private final PackedTypesCollector packedTypesCollector;

    private final CppNativeMapper cppNativeMapper;

    private final boolean withReflectionCode;
    private final String generatorDescription;
    private final String generatorVersionString;
    private final long generatorVersionNumber;
}
