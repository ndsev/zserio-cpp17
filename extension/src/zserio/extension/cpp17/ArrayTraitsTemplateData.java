package zserio.extension.cpp17;

import zserio.extension.cpp17.types.NativeArrayTraits;

/**
 * FreeMarker template data for array traits.
 */
public final class ArrayTraitsTemplateData
{
    public ArrayTraitsTemplateData(NativeArrayTraits nativeTraits)
    {
        this.nativeTraits = nativeTraits;
    }

    public String getName()
    {
        return nativeTraits.getFullName();
    }

    private final NativeArrayTraits nativeTraits;
}
