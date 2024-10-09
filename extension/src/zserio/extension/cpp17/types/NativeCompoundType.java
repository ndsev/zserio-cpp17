package zserio.extension.cpp17.types;

import zserio.ast.PackageName;

/**
 * Native C++ compound type mapping.
 */
public final class NativeCompoundType extends NativeUserType
{
    public NativeCompoundType(PackageName packageName, String name, String includeFileName)
    {
        super(packageName, name, includeFileName, false);
    }
}
