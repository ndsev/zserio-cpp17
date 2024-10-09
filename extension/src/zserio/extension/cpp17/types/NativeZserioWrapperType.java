package zserio.extension.cpp17.types;

/**
 * Native C++ runtime type mapping.
 */
public class NativeZserioWrapperType extends NativeZserioType
{
    public NativeZserioWrapperType(String name, String nativeTypeName)
    {
        super(name, true);
        this.nativeTypeName = nativeTypeName;
        addSystemIncludeFile("zserio/Types.h");
    }

    String getNativeTypeName()
    {
        return nativeTypeName;
    }

    private final String nativeTypeName;
}
