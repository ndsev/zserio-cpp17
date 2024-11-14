package zserio.extension.cpp17.types;

/**
 * Native C++ runtime type mapping.
 */
public class NativeNumericWrapperType extends NativeZserioType
{
    public NativeNumericWrapperType(String name, String nativeTypeName)
    {
        super(name);
        this.nativeTypeName = nativeTypeName;
        addSystemIncludeFile("zserio/Types.h");
    }

    String getNativeTypeName()
    {
        return nativeTypeName;
    }

    private final String nativeTypeName;
}
