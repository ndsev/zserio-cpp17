package zserio.extension.cpp17;

import java.util.Collection;

import zserio.ast.TemplateArgument;
import zserio.ast.TypeReference;
import zserio.extension.common.ZserioExtensionException;
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

    static void addHeaderIncludes(CppNativeMapper cppNativeMapper, CppNativeType nativeType,
            TypeReference typeReference, IncludeCollector includeCollector) throws ZserioExtensionException
    {
        includeCollector.addHeaderIncludesForType(nativeType);
        for (TemplateArgument templateArgument : typeReference.getTemplateArguments())
        {
            final TypeReference argumentTypeReference = templateArgument.getTypeReference();
            final CppNativeType argumentNativeType = cppNativeMapper.getCppType(argumentTypeReference);
            addHeaderIncludes(cppNativeMapper, argumentNativeType, argumentTypeReference, includeCollector);
        }
    }
}
