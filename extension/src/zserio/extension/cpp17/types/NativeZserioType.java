package zserio.extension.cpp17.types;

import zserio.ast.PackageName;

/**
 * Native C++ runtime type mapping.
 */
public class NativeZserioType extends NativeType
{
    public NativeZserioType(String name, boolean isSimple)
    {
        super(ZSERIO_PACKAGE_NAME, name, isSimple);
    }

    private static final PackageName ZSERIO_PACKAGE_NAME = new PackageName.Builder().addId("zserio").get();
}
