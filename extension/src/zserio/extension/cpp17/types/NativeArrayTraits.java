package zserio.extension.cpp17.types;

import java.util.Collections;

/**
 * Native C++ array traits mapping.
 */
public class NativeArrayTraits extends NativeZserioType
{
    public NativeArrayTraits(String name)
    {
        super(name, Collections.singleton("zserio/ArrayTraits.h"), null);
    }
};
