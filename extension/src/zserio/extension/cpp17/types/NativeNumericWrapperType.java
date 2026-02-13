package zserio.extension.cpp17.types;

import java.util.Collections;

/**
 * Native C++ runtime type mapping.
 */
public class NativeNumericWrapperType extends NativeZserioType
{
    public NativeNumericWrapperType(String name, String nativeTypeName)
    {
        super(name, Collections.singleton("zserio/Types.h"), null);
        this.nativeTypeName = nativeTypeName;
    }

    String getNativeTypeName()
    {
        return nativeTypeName;
    }

    private final String nativeTypeName;
}
