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
        this(name, systemIncludeFile, false);
    }

    public NativeStdType(String name, String systemIncludeFile, boolean isSimple)
    {
        this(STD_PACKAGE_NAME, name, systemIncludeFile, isSimple);
    }

    public NativeStdType(PackageName packageName, String name, String systemIncludeFile)
    {
        this(packageName, name, systemIncludeFile, false);
    }

    public NativeStdType(PackageName packageName, String name, String systemIncludeFile, boolean isSimple)
    {
        super(packageName, name, isSimple);

        addSystemIncludeFile(systemIncludeFile);
    }

    private static final PackageName STD_PACKAGE_NAME = new PackageName.Builder().addId("std").get();
}
