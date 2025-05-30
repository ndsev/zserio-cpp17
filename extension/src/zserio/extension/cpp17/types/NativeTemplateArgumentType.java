package zserio.extension.cpp17.types;

import zserio.ast.PackageName;

/**
 * Native C++ template argument type mapping.
 */
public class NativeTemplateArgumentType extends NativeType
{
    public NativeTemplateArgumentType(String name)
    {
        super(PackageName.EMPTY, name);
    }
}
