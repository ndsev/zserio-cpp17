package zserio.extension.cpp17;

import java.util.Collection;

import zserio.extension.cpp17.types.CppNativeType;

/**
 * Interface for include collectors.
 */
public interface IncludeCollector
{
    void addHeaderIncludesForType(CppNativeType nativeType);
    void addHeaderSystemIncludes(Collection<String> systemIncludes);
    void addHeaderUserIncludes(Collection<String> userIncludes);

    void addCppIncludesForType(CppNativeType nativeType);
    void addCppSystemIncludes(Collection<String> systemIncludes);
    void addCppUserIncludes(Collection<String> userIncludes);
}
