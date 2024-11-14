package zserio.extension.cpp17.types;

import zserio.ast.PackageName;

/**
 * Native C++ runtime type mapping.
 */
public class NativeStdType extends NativeType
{
    public NativeStdType(PackageName packageName, String name)
    {
        super(packageName, name);
    }

    public NativeStdType(String name, String systemIncludeFile)
    {
        this(STD_PACKAGE_NAME, name, systemIncludeFile);
    }

    public NativeStdType(PackageName packageName, String name, String systemIncludeFile)
    {
        super(packageName, name);

        addSystemIncludeFile(systemIncludeFile);
    }

    private static final PackageName STD_PACKAGE_NAME = new PackageName.Builder().addId("std").get();
}
