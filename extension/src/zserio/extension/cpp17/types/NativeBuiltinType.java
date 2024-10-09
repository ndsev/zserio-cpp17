package zserio.extension.cpp17.types;

import zserio.ast.PackageName;

/**
 * Native C++ built-in type mapping.
 */
public class NativeBuiltinType extends NativeType
{
    public NativeBuiltinType(String builtinTypeName)
    {
        super(PackageName.EMPTY, builtinTypeName);
    }
}
