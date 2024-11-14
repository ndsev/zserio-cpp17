package zserio.extension.cpp17.types;

import zserio.ast.PackageName;

/**
 * Native C++ user type mapping.
 */
public class NativeUserType extends NativeType
{
    public NativeUserType(PackageName packageName, String name, String includeFileName)
    {
        super(packageName, name);
        addUserIncludeFile(includeFileName);
    }
}
