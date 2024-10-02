package zserio.extension.cpp17.types;

import java.util.SortedSet;

import zserio.ast.PackageName;

/**
 * C++ native type interface.
 */
public interface CppNativeType
{
    public String getFullName();

    public String getName();

    public PackageName getPackageName();

    public boolean isSimple();

    public SortedSet<String> getSystemIncludeFiles();

    public SortedSet<String> getUserIncludeFiles();
}
