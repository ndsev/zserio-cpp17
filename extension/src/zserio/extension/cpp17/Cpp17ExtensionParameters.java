package zserio.extension.cpp17;

import java.util.StringJoiner;

import org.apache.commons.cli.Option;
import org.apache.commons.cli.OptionGroup;
import org.apache.commons.cli.Options;

import zserio.extension.common.ZserioExtensionException;
import zserio.tools.ExtensionParameters;

/**
 * Command line parameters for C++ extension.
 *
 * The class holds all command line parameters passed by core to the C++ extension, which are really
 * used by C++ emitters.
 */
public final class Cpp17ExtensionParameters
{
    public Cpp17ExtensionParameters(ExtensionParameters parameters)
    {
        outputDir = parameters.getCommandLineArg(OptionCpp17);
        withTypeInfoCode = parameters.argumentExists(OptionWithTypeInfoCode);
        withSourcesAmalgamation = parameters.argumentExists(OptionWithSourcesAmalgamation);

        final String cppAllocator = parameters.getCommandLineArg(OptionSetCppAllocator);
        if (cppAllocator == null || cppAllocator.equals(StdAllocator))
            allocatorDefinition = TypesContext.STD_ALLOCATOR;
        else if (cppAllocator.equals(StdPolymorphicAllocator))
            allocatorDefinition = TypesContext.STD_POLYMORPHIC_ALLOCATOR;
        else
            allocatorDefinition = TypesContext.PROPAGATING_POLYMORPHIC_ALLOCATOR;

        final StringJoiner description = new StringJoiner(", ");

        if (withTypeInfoCode)
            description.add("typeInfoCode");
        addAllocatorDescription(description);
        parametersDescription = description.toString();

        zserioVersion = parameters.getZserioVersion();
    }

    public String getOutputDir()
    {
        return outputDir;
    }

    public boolean getWithTypeInfoCode()
    {
        return withTypeInfoCode;
    }

    public boolean getWithSourcesAmalgamation()
    {
        return withSourcesAmalgamation;
    }

    public TypesContext.AllocatorDefinition getAllocatorDefinition()
    {
        return allocatorDefinition;
    }

    public String getParametersDescription()
    {
        return parametersDescription;
    }

    public String getZserioVersion()
    {
        return zserioVersion;
    }

    static void registerOptions(Options options)
    {
        Option option = new Option(OptionCpp17, true, "generate C++17 sources");
        option.setArgName("outputDir");
        option.setRequired(false);
        options.addOption(option);

        option = new Option(OptionSetCppAllocator, true,
                "set the C++ allocator to be used in generated code: std (default), pmr, ppmr");
        option.setArgName("allocator");
        option.setRequired(false);
        options.addOption(option);

        final OptionGroup reflectionGroup = new OptionGroup();
        option = new Option(OptionWithTypeInfoCode, false, "enable type info and reflection code");
        reflectionGroup.addOption(option);
        option =
                new Option(OptionWithoutTypeInfoCode, false, "disable type info and reflection code (default)");
        reflectionGroup.addOption(option);
        reflectionGroup.setRequired(false);
        options.addOptionGroup(reflectionGroup);

        final OptionGroup sourcesAmalgamationGroup = new OptionGroup();
        option = new Option(
                OptionWithSourcesAmalgamation, false, "enable amalgamation of generated C++ sources");
        sourcesAmalgamationGroup.addOption(option);
        option = new Option(OptionWithoutSourcesAmalgamation, false,
                "disable amalgamation of generated C++ sources (default)");
        sourcesAmalgamationGroup.addOption(option);
        sourcesAmalgamationGroup.setRequired(false);
        options.addOptionGroup(sourcesAmalgamationGroup);
    }

    static boolean hasOptionCpp17(ExtensionParameters parameters)
    {
        return parameters.argumentExists(OptionCpp17);
    }

    static void check(ExtensionParameters parameters) throws ZserioExtensionException
    {
        final String cppAllocator = parameters.getCommandLineArg(OptionSetCppAllocator);
        if (cppAllocator != null && !cppAllocator.equals(StdAllocator) &&
                !cppAllocator.equals(StdPolymorphicAllocator) &&
                !cppAllocator.equals(PropagatingPolymorphicAllocator))
        {
            throw new ZserioExtensionException("The specified option '" + OptionSetCppAllocator + "' has "
                    + "unknown allocator '" + cppAllocator + "'!");
        }
    }

    private void addAllocatorDescription(StringJoiner description)
    {
        if (allocatorDefinition == TypesContext.STD_ALLOCATOR)
        {
            description.add(StdAllocator + "Allocator");
        }
        else if (allocatorDefinition == TypesContext.STD_POLYMORPHIC_ALLOCATOR)
        {
            description.add(StdPolymorphicAllocator + "Allocator");
        }
        else if (allocatorDefinition == TypesContext.PROPAGATING_POLYMORPHIC_ALLOCATOR)
        {
            description.add(PropagatingPolymorphicAllocator + "Allocator");
        }
        else
        {
            description.add("customAllocator(" + allocatorDefinition.getAllocatorType() + ", " +
                    allocatorDefinition.getAllocatorSystemInclude() + ")");
        }
    }

    private final static String OptionCpp17 = "cpp17";
    private final static String OptionSetCppAllocator = "setCppAllocator";
    private final static String OptionWithoutTypeInfoCode = "withoutTypeInfoCode";
    private static final String OptionWithTypeInfoCode = "withTypeInfoCode";
    private static final String OptionWithSourcesAmalgamation = "withSourcesAmalgamation";
    private static final String OptionWithoutSourcesAmalgamation = "withoutSourcesAmalgamation";

    private final static String StdAllocator = "std";
    private final static String StdPolymorphicAllocator = "pmr";
    private final static String PropagatingPolymorphicAllocator = "ppmr";

    private final String outputDir;
    private final boolean withTypeInfoCode;
    private final boolean withSourcesAmalgamation;
    private final TypesContext.AllocatorDefinition allocatorDefinition;
    private final String parametersDescription;
    private final String zserioVersion;
}
