package zserio.extension.cpp17;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.TreeSet;

import zserio.ast.PackageName;
import zserio.extension.cpp17.types.CppNativeType;
import zserio.extension.cpp17.types.NativeAllocType;

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

        withTypeInfoCode = context.getWithTypeInfoCode();

        headerSystemIncludes = new TreeSet<String>();
        headerUserIncludes = new TreeSet<String>();
        cppUserIncludes = new TreeSet<String>();
        cppSystemIncludes = new TreeSet<String>();

        types = new TypesTemplateData(context.getTypesContext(), context.getCppNativeMapper());
    }

    public String getGeneratorDescription()
    {
        return generatorDescription;
    }

    public GeneratorVersionTemplateData getGeneratorVersion()
    {
        return generatorVersion;
    }

    public boolean getWithTypeInfoCode()
    {
        return withTypeInfoCode;
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

    public TypesTemplateData getTypes()
    {
        return types;
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

    public static final class TypesTemplateData
    {
        public TypesTemplateData(TypesContext typesContext, CppNativeMapper nativeMapper)
        {
            allocator = new AllocatorTemplateData(typesContext);
            any = new TypeTemplateData(
                    nativeMapper.getAnyType(), nativeMapper.getAnyType().needsAllocatorArgument());
            bitBuffer = new TypeTemplateData(
                    nativeMapper.getBitBufferType(), nativeMapper.getBitBufferType().needsAllocatorArgument());
            bitBufferView = new TypeTemplateData(nativeMapper.getBitBufferViewType(),
                    nativeMapper.getBitBufferViewType().needsAllocatorArgument());
            bytes = new TypeTemplateData(
                    nativeMapper.getBytesType(), nativeMapper.getBytesType().needsAllocatorArgument());
            string = new TypeTemplateData(
                    nativeMapper.getStringType(), nativeMapper.getStringType().needsAllocatorArgument());
            vector = new TypeTemplateData(
                    nativeMapper.getVectorType(), nativeMapper.getVectorType().needsAllocatorArgument());
            map = new TypeTemplateData(
                    nativeMapper.getMapType(), nativeMapper.getMapType().needsAllocatorArgument());
            set = new TypeTemplateData(
                    nativeMapper.getSetType(), nativeMapper.getSetType().needsAllocatorArgument());
            optional = new TypeTemplateData(
                    nativeMapper.getOptionalType(), nativeMapper.getOptionalType().needsAllocatorArgument());
            variant = new TypeTemplateData(
                    nativeMapper.getVariantType(), nativeMapper.getVariantType().needsAllocatorArgument());
            uniquePtr = new TypeTemplateData(
                    nativeMapper.getUniquePtrType(), nativeMapper.getUniquePtrType().needsAllocatorArgument());
            service = new TypeTemplateData(nativeMapper.getServiceType());
            serviceClient = new TypeTemplateData(nativeMapper.getServiceClientType());
            serviceDataPtr = new TypeTemplateData(nativeMapper.getServiceDataPtrType());
            introspectableServiceData = new TypeTemplateData(nativeMapper.getIntrospectableServiceDataType());
            objectServiceData = new TypeTemplateData(nativeMapper.getObjectServiceDataType());
            rawServiceDataHolder = new TypeTemplateData(nativeMapper.getRawServiceDataHolderType());
            rawServiceDataView = new TypeTemplateData(nativeMapper.getRawServiceDataViewType());
            typeInfo = new TypeTemplateData(nativeMapper.getTypeInfoType());
            reflectablePtr = new TypeTemplateData(nativeMapper.getReflectablePtrType());
            reflectableConstPtr = new TypeTemplateData(nativeMapper.getReflectableConstPtrType());
            introspectableConstPtr = new TypeTemplateData(nativeMapper.getIntrospectableConstPtrType());
        }

        public AllocatorTemplateData getAllocator()
        {
            return allocator;
        }

        public TypeTemplateData getAny()
        {
            return any;
        }

        public TypeTemplateData getBitBuffer()
        {
            return bitBuffer;
        }

        public TypeTemplateData getBitBufferView()
        {
            return bitBufferView;
        }

        public TypeTemplateData getBytes()
        {
            return bytes;
        }

        public TypeTemplateData getString()
        {
            return string;
        }

        public TypeTemplateData getVector()
        {
            return vector;
        }

        public TypeTemplateData getMap()
        {
            return map;
        }

        public TypeTemplateData getSet()
        {
            return set;
        }

        public TypeTemplateData getOptional()
        {
            return optional;
        }

        public TypeTemplateData getVariant()
        {
            return variant;
        }

        public TypeTemplateData getUniquePtr()
        {
            return uniquePtr;
        }

        public TypeTemplateData getService()
        {
            return service;
        }

        public TypeTemplateData getServiceClient()
        {
            return serviceClient;
        }

        public TypeTemplateData getServiceDataPtr()
        {
            return serviceDataPtr;
        }

        public TypeTemplateData getIntrospectableServiceData()
        {
            return introspectableServiceData;
        }

        public TypeTemplateData getObjectServiceData()
        {
            return objectServiceData;
        }

        public TypeTemplateData getRawServiceDataHolder()
        {
            return rawServiceDataHolder;
        }

        public TypeTemplateData getRawServiceDataView()
        {
            return rawServiceDataView;
        }

        public TypeTemplateData getTypeInfo()
        {
            return typeInfo;
        }

        public TypeTemplateData getReflectablePtr()
        {
            return reflectablePtr;
        }

        public TypeTemplateData getReflectableConstPtr()
        {
            return reflectableConstPtr;
        }

        public TypeTemplateData getIntrospectableConstPtr()
        {
            return introspectableConstPtr;
        }

        public static final class AllocatorTemplateData
        {
            public AllocatorTemplateData(TypesContext typesContext)
            {
                name = typesContext.getAllocatorDefinition().getAllocatorType();
                defaultType = typesContext.getAllocatorDefinition().getAllocatorDefaultType();
                systemIncludes.add(typesContext.getAllocatorDefinition().getAllocatorSystemInclude());
            }

            public String getName()
            {
                return name;
            }

            public String getDefaultType()
            {
                return defaultType;
            }

            public String getDefault()
            {
                return name + "<" + defaultType + ">";
            }

            public Iterable<String> getSystemIncludes()
            {
                return systemIncludes;
            }

            public Iterable<String> getUserIncludes()
            {
                return userIncludes;
            }

            private final String name;
            private final String defaultType;
            private final List<String> systemIncludes = new ArrayList<String>();
            private final List<String> userIncludes = new ArrayList<String>();
        }

        public static class TypeTemplateData
        {
            public TypeTemplateData(CppNativeType type)
            {
                this(type, false);
            }

            public TypeTemplateData(NativeAllocType type)
            {
                this(type, type.needsAllocatorArgument());
            }

            public String getName()
            {
                return name;
            }

            public boolean getNeedsAllocatorArgument()
            {
                return needsAllocatorArgument;
            }

            public Iterable<String> getSystemIncludes()
            {
                return systemIncludes;
            }

            public Iterable<String> getUserIncludes()
            {
                return userIncludes;
            }

            private TypeTemplateData(CppNativeType type, boolean needsAllocatorArgument)
            {
                name = type.getFullName();
                this.needsAllocatorArgument = needsAllocatorArgument;
                systemIncludes = type.getSystemIncludeFiles();
                userIncludes = type.getUserIncludeFiles();
            }

            private final String name;
            private final boolean needsAllocatorArgument;
            private final Iterable<String> systemIncludes;
            private final Iterable<String> userIncludes;
        }

        private final AllocatorTemplateData allocator;
        private final TypeTemplateData any;
        private final TypeTemplateData bitBuffer;
        private final TypeTemplateData bitBufferView;
        private final TypeTemplateData bytes;
        private final TypeTemplateData string;
        private final TypeTemplateData vector;
        private final TypeTemplateData map;
        private final TypeTemplateData set;
        private final TypeTemplateData optional;
        private final TypeTemplateData variant;
        private final TypeTemplateData uniquePtr;
        private final TypeTemplateData service;
        private final TypeTemplateData serviceClient;
        private final TypeTemplateData serviceDataPtr;
        private final TypeTemplateData introspectableServiceData;
        private final TypeTemplateData objectServiceData;
        private final TypeTemplateData rawServiceDataHolder;
        private final TypeTemplateData rawServiceDataView;
        private final TypeTemplateData typeInfo;
        private final TypeTemplateData reflectablePtr;
        private final TypeTemplateData reflectableConstPtr;
        private final TypeTemplateData introspectableConstPtr;
    }

    private final String generatorDescription;
    private final GeneratorVersionTemplateData generatorVersion;

    private final boolean withTypeInfoCode;

    private final TreeSet<String> headerSystemIncludes;
    private final TreeSet<String> headerUserIncludes;
    private final TreeSet<String> cppUserIncludes;
    private final TreeSet<String> cppSystemIncludes;
    private final TypesTemplateData types;
}
