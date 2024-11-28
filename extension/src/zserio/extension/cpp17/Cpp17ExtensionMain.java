package zserio.extension.cpp17;

/**
 * The main entry of the Zserio C++17 extension.
 *
 * This just prints the usage.
 */
public final class Cpp17ExtensionMain
{
    public static void main(String[] args)
    {
        System.out.println(
                "C++17 Generator version " + Cpp17ExtensionVersion.CPP17_EXTENSION_VERSION_STRING + "\n");
        System.out.println("This generator cannot be run standalone and is designed as an extension to the " +
                           "Zserio version " +
                Cpp17ExtensionVersion.EXPECTED_ZSERIO_VERSION_STRING + ".\n");
        System.out.println("How to run it using Zserio bundle:");
        System.out.println("  java -cp zserio.jar:zserio_cpp17.jar zserio.tools.ZserioTool\n");
        System.out.println("How to run it using Zserio core:");
        System.out.println("  java -cp zserio_core.jar:zserio_cpp17.jar zserio.tools.ZserioTool");
    }
}
