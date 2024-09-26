package zserio.extension.cpp17;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.TreeSet;

import zserio.ast.PackageName;
import zserio.extension.cpp17.types.CppNativeType;

/**
 * Base class for all C++ template data for FreeMarker.
 */
public abstract class CppTemplateData implements IncludeCollector
{
    public CppTemplateData(TemplateDataContext context)
    {
        generatorDescription = context.getGeneratorDescription();
        generatorVersion = new GeneratorVersionTemplateData(
                context.getGeneratorVersionString(), context.getGeneratorVersionNumber());

        withReflectionCode = context.getWithReflectionCode();

        headerSystemIncludes = new TreeSet<String>();
        headerUserIncludes = new TreeSet<String>();
        cppUserIncludes = new TreeSet<String>();
        cppSystemIncludes = new TreeSet<String>();
    }

    public String getGeneratorDescription()
    {
        return generatorDescription;
    }

    public GeneratorVersionTemplateData getGeneratorVersion()
    {
        return generatorVersion;
    }

    public boolean getWithReflectionCode()
    {
        return withReflectionCode;
    }

    public Iterable<String> getHeaderSystemIncludes()
    {
        return headerSystemIncludes;
    }

    public Iterable<String> getHeaderUserIncludes()
    {
        return headerUserIncludes;
    }

    public Iterable<String> getCppUserIncludes()
    {
        return cppUserIncludes;
    }

    public Iterable<String> getCppSystemIncludes()
    {
        return cppSystemIncludes;
    }

    @Override
    public void addHeaderIncludesForType(CppNativeType nativeType)
    {
        addHeaderSystemIncludes(nativeType.getSystemIncludeFiles());
        addHeaderUserIncludes(nativeType.getUserIncludeFiles());
    }

    @Override
    public void addHeaderSystemIncludes(Collection<String> systemIncludes)
    {
        headerSystemIncludes.addAll(systemIncludes);
    }

    @Override
    public void addHeaderUserIncludes(Collection<String> userIncludes)
    {
        headerUserIncludes.addAll(userIncludes);
    }

    @Override
    public void addCppIncludesForType(CppNativeType nativeType)
    {
        cppSystemIncludes.addAll(nativeType.getSystemIncludeFiles());
        cppUserIncludes.addAll(nativeType.getUserIncludeFiles());
    }

    @Override
    public void addCppSystemIncludes(Collection<String> systemIncludes)
    {
        cppSystemIncludes.addAll(systemIncludes);
    }

    @Override
    public void addCppUserIncludes(Collection<String> userIncludes)
    {
        cppUserIncludes.addAll(userIncludes);
    }

    public static final class PackageTemplateData
    {
        public PackageTemplateData(CppNativeType type)
        {
            this(type.getPackageName());
        }

        public PackageTemplateData(PackageName packageName)
        {
            this.packageName = packageName;
        }

        public String getName()
        {
            return CppFullNameFormatter.getFullName(packageName);
        }

        public Iterable<String> getPath()
        {
            return packageName.getIdList();
        }

        private final PackageName packageName;
    }

    public static final class GeneratorVersionTemplateData
    {
        public GeneratorVersionTemplateData(String versionString, long versionNumber)
        {
            this.versionString = versionString;
            this.versionNumber = Long.toString(versionNumber);
        }

        public String getVersionString()
        {
            return versionString;
        }

        public String getVersionNumber()
        {
            return versionNumber;
        }

        private final String versionString;
        private final String versionNumber;
    }

    private final String generatorDescription;
    private final GeneratorVersionTemplateData generatorVersion;

    private final boolean withReflectionCode;

    private final TreeSet<String> headerSystemIncludes;
    private final TreeSet<String> headerUserIncludes;
    private final TreeSet<String> cppUserIncludes;
    private final TreeSet<String> cppSystemIncludes;
}
