package zserio.extension.cpp17.types;

/**
 * Native C++ array traits mapping.
 */
public class NativeArrayTraits extends NativeZserioType
{
    public NativeArrayTraits(String name)
    {
        super(name);
        addSystemIncludeFile("zserio/ArrayTraits.h");
    }
};
