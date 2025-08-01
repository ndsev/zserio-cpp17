package zserio.extension.cpp17;

/**
 * C++ extension version information.
 *
 * C++ extension version will be reported to Zserio core and will be stored in MANIFEST.MF.
 *
 * Expected Zserio core version will be used to check if the extension can be safely loaded.
 */
public final class Cpp17ExtensionVersion
{
    /** C++ extension version string. */
    public static final String CPP17_EXTENSION_VERSION_STRING = "0.5.0";

    /** Expected Zserio core version string. */
    public static final String EXPECTED_ZSERIO_VERSION_STRING = "2.17.0";

    /** Zserio BIN version string. */
    public static final String BIN_VERSION_STRING = "1.0";

    /** Zserio JSON version string. */
    public static final String JSON_VERSION_STRING = "1.0";

    /** C++ extension version in integer value to allow version checking in generated code. */
    public static final long CPP17_EXTENSION_VERSION_NUMBER = 5000;
}
