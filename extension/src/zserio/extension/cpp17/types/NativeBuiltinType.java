package zserio.extension.cpp17.types;

import zserio.ast.PackageName;

/**
 * Native C++ built-in type mapping.
 */
public class NativeBuiltinType extends NativeType implements CppNativeArrayableType
{
    public NativeBuiltinType(String builtinTypeName, NativeArrayTraits arrayTraits)
    {
        super(PackageName.EMPTY, builtinTypeName);

        this.arrayTraits = arrayTraits;
        addIncludeFiles(arrayTraits);
    }

    @Override
    public NativeArrayTraits getArrayTraits()
    {
        return arrayTraits;
    }

    private NativeArrayTraits arrayTraits;
}
