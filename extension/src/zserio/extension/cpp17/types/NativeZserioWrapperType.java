package zserio.extension.cpp17.types;

/**
 * Native C++ runtime type mapping.
 */
public class NativeZserioWrapperType extends NativeZserioType
{
    public NativeZserioWrapperType(String name, String nativeTypeName)
    {
        super(name);
        this.nativeTypeName = nativeTypeName;
        addSystemIncludeFile(ZSERIO_TYPES_INCLUDE);
    }

    String getNativeTypeName()
    {
        return nativeTypeName;
    }

    private static final String ZSERIO_TYPES_INCLUDE = "zserio/Types.h";

    private final String nativeTypeName;
}
