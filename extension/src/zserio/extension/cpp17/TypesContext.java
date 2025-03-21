package zserio.extension.cpp17;

import zserio.ast.PackageName;

/**
 * Keeps information about configurable types.
 *
 * Currently only types which depends on allocators are configurable.
 */
public final class TypesContext
{
    public TypesContext(AllocatorDefinition allocator)
    {
        this.allocator = allocator;

        if (allocator.equals(STD_ALLOCATOR))
        {
            any = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "Any", false, false, "zserio/Any.h");
            bitBuffer = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "BitBuffer", false, false, "zserio/BitBuffer.h");
            bitBufferView = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "BitBufferView", false, false, "zserio/BitBuffer.h");
            bytes = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "Bytes", false, false, "zserio/Bytes.h");
            string = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "String", false, false, "zserio/String.h");
            vector = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "Vector", true, false, "zserio/Vector.h");
            map = new NativeTypeDefinition(STD_PACKAGE_NAME, "map", true, false, "map");
            set = new NativeTypeDefinition(STD_PACKAGE_NAME, "set", true, false, "set");
            optional =
                    new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "Optional", true, false, "zserio/Optional.h");
            variant = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "Variant", true, false, "zserio/Variant.h");
            uniquePtr = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "UniquePtr", true, false, "zserio/UniquePtr.h");
            service = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "IService", false, false, "zserio/IService.h");
            serviceClient = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "IServiceClient", false, false, "zserio/IService.h");
            serviceDataPtr = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "IServiceDataPtr", false, false, "zserio/IService.h");
            objectServiceData = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "ObjectServiceData", false, false, "zserio/IService.h");
            rawServiceDataHolder = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "RawServiceDataHolder", false, false, "zserio/IService.h");
            rawServiceDataView = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "RawServiceDataView", false, false, "zserio/IService.h");
            typeInfo = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "ITypeInfo", false, false, "zserio/ITypeInfo.h");
            reflectablePtr = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "IReflectableDataPtr", false, false, "zserio/IReflectableData.h");
            reflectableConstPtr = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "IReflectableDataConstPtr", false, false, "zserio/IReflectableData.h");
        }
        else if (allocator.equals(PROPAGATING_POLYMORPHIC_ALLOCATOR))
        {
            any = new NativeTypeDefinition(ZSERIO_PMR_PACKAGE_NAME, "Any", false, false, "zserio/pmr/Any.h");
            bitBuffer = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "BitBuffer", false, false, "zserio/pmr/BitBuffer.h");
            bitBufferView = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "BitBufferView", false, false, "zserio/pmr/BitBuffer.h");
            bytes = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "Bytes", false, false, "zserio/pmr/Bytes.h");
            string = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "String", false, false, "zserio/pmr/String.h");
            vector = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "Vector", true, false, "zserio/pmr/Vector.h");
            map = new NativeTypeDefinition(ZSERIO_PMR_PACKAGE_NAME, "Map", true, false, "zserio/pmr/Map.h");
            set = new NativeTypeDefinition(ZSERIO_PMR_PACKAGE_NAME, "Set", true, false, "zserio/pmr/Set.h");
            optional = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "Optional", true, false, "zserio/pmr/Optional.h");
            variant = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "Variant", true, false, "zserio/pmr/Variant.h");
            uniquePtr = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "UniquePtr", true, false, "zserio/pmr/UniquePtr.h");
            service = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "IService", false, false, "zserio/pmr/IService.h");
            serviceClient = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "IServiceClient", false, false, "zserio/pmr/IService.h");
            serviceDataPtr = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "IServiceDataPtr", false, false, "zserio/pmr/IService.h");
            objectServiceData = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "ObjectServiceData", false, false, "zserio/pmr/IService.h");
            rawServiceDataHolder = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "RawServiceDataHolder", false, false, "zserio/pmr/IService.h");
            rawServiceDataView = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "RawServiceDataView", false, false, "zserio/pmr/IService.h");
            typeInfo = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "ITypeInfo", false, false, "zserio/pmr/ITypeInfo.h");
            reflectablePtr = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "IReflectableDataPtr", false, false, "zserio/pmr/IReflectableData.h");
            reflectableConstPtr = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "IReflectableDataConstPtr",
                    false, false, "zserio/pmr/IReflectableData.h");
        }
        else
        {
            any = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "BasicAny", true, true, "zserio/Any.h");
            bitBuffer = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "BasicBitBuffer", true, true, "zserio/BitBuffer.h");
            bitBufferView = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "BasicBitBufferView", true, true, "zserio/BitBuffer.h");
            bytes = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "BasicBytes", true, true, "zserio/Bytes.h");
            string =
                    new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "BasicString", true, true, "zserio/String.h");
            vector = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "Vector", true, true, "zserio/Vector.h");
            map = new NativeTypeDefinition(STD_PACKAGE_NAME, "map", true, true, "map");
            set = new NativeTypeDefinition(STD_PACKAGE_NAME, "set", true, true, "set");
            optional = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "BasicOptional", true, true, "zserio/Optional.h");
            variant = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "BasicVariant", true, true, "zserio/Variant.h");
            uniquePtr = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "UniquePtr", true, true, "zserio/UniquePtr.h");
            service = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "IBasicService", true, true, "zserio/IService.h");
            serviceClient = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "IBasicServiceClient", true, true, "zserio/IService.h");
            serviceDataPtr = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "IBasicServiceDataPtr", true, true, "zserio/IService.h");
            objectServiceData = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "BasicObjectServiceData", true, true, "zserio/IService.h");
            rawServiceDataHolder = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "BasicRawServiceDataHolder", true, true, "zserio/IService.h");
            rawServiceDataView = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "BasicRawServiceDataView", true, true, "zserio/IService.h");
            typeInfo = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "ITypeInfo", true, true, "zserio/ITypeInfo.h");
            reflectablePtr = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "IReflectablePtr", true, true, "zserio/IReflectableData.h");
            reflectableConstPtr = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "IReflectableConstPtr", true, true, "zserio/IReflectableData.h");
        }
    }

    public AllocatorDefinition getAllocatorDefinition()
    {
        return allocator;
    }

    public NativeTypeDefinition getAny()
    {
        return any;
    }

    public NativeTypeDefinition getBitBuffer()
    {
        return bitBuffer;
    }

    public NativeTypeDefinition getBitBufferView()
    {
        return bitBufferView;
    }

    public NativeTypeDefinition getBytes()
    {
        return bytes;
    }

    public NativeTypeDefinition getString()
    {
        return string;
    }

    public NativeTypeDefinition getVector()
    {
        return vector;
    }

    public NativeTypeDefinition getMap()
    {
        return map;
    }

    public NativeTypeDefinition getSet()
    {
        return set;
    }

    public NativeTypeDefinition getOptional()
    {
        return optional;
    }

    public NativeTypeDefinition getVariant()
    {
        return variant;
    }

    public NativeTypeDefinition getUniquePtr()
    {
        return uniquePtr;
    }

    public NativeTypeDefinition getService()
    {
        return service;
    }

    public NativeTypeDefinition getServiceClient()
    {
        return serviceClient;
    }

    public NativeTypeDefinition getServiceDataPtr()
    {
        return serviceDataPtr;
    }

    public NativeTypeDefinition getObjectServiceData()
    {
        return objectServiceData;
    }

    public NativeTypeDefinition getRawServiceDataHolder()
    {
        return rawServiceDataHolder;
    }

    public NativeTypeDefinition getRawServiceDataView()
    {
        return rawServiceDataView;
    }

    public NativeTypeDefinition getTypeInfo()
    {
        return typeInfo;
    }

    public NativeTypeDefinition getReflectablePtr()
    {
        return reflectablePtr;
    }

    public NativeTypeDefinition getReflectableConstPtr()
    {
        return reflectableConstPtr;
    }

    public static final class AllocatorDefinition
    {
        public AllocatorDefinition(String allocatorType, String allocatorSystemInclude)
        {
            this(allocatorType, allocatorSystemInclude, "uint8_t");
        }

        public AllocatorDefinition(
                String allocatorType, String allocatorSystemInclude, String allocatorDefaultType)
        {
            this.allocatorType = allocatorType;
            this.allocatorSystemInclude = allocatorSystemInclude;
            this.allocatorDefaultType = allocatorDefaultType;
        }

        public String getAllocatorType()
        {
            return allocatorType;
        }

        public String getAllocatorSystemInclude()
        {
            return allocatorSystemInclude;
        }

        public String getAllocatorDefaultType()
        {
            return allocatorDefaultType;
        }

        private final String allocatorType;
        private final String allocatorSystemInclude;
        private final String allocatorDefaultType;
    }

    public static final class NativeTypeDefinition
    {
        public NativeTypeDefinition(PackageName pkg, String name, boolean isTemplate,
                boolean needsAllocatorArgument, String systemInclude)
        {
            this.pkg = pkg;
            this.name = name;
            this.isTemplate = isTemplate;
            this.needsAllocatorArgument = needsAllocatorArgument;
            this.systemInclude = systemInclude;
        }

        public PackageName getPackage()
        {
            return pkg;
        }

        public String getName()
        {
            return name;
        }

        public String getSystemInclude()
        {
            return systemInclude;
        }

        public boolean isTemplate()
        {
            return isTemplate;
        }

        public boolean needsAllocatorArgument()
        {
            return needsAllocatorArgument;
        }

        private final PackageName pkg;
        private final String name;
        private final String systemInclude;
        private final boolean isTemplate;
        private final boolean needsAllocatorArgument;
    }

    private final AllocatorDefinition allocator;
    private final NativeTypeDefinition any;
    private final NativeTypeDefinition bitBuffer;
    private final NativeTypeDefinition bitBufferView;
    private final NativeTypeDefinition bytes;
    private final NativeTypeDefinition string;
    private final NativeTypeDefinition vector;
    private final NativeTypeDefinition map;
    private final NativeTypeDefinition set;
    private final NativeTypeDefinition optional;
    private final NativeTypeDefinition variant;
    private final NativeTypeDefinition uniquePtr;

    private final NativeTypeDefinition service;
    private final NativeTypeDefinition serviceClient;
    private final NativeTypeDefinition serviceDataPtr;
    private final NativeTypeDefinition objectServiceData;
    private final NativeTypeDefinition rawServiceDataHolder;
    private final NativeTypeDefinition rawServiceDataView;

    private final NativeTypeDefinition typeInfo;
    private final NativeTypeDefinition reflectablePtr;
    private final NativeTypeDefinition reflectableConstPtr;

    public static final AllocatorDefinition PROPAGATING_POLYMORPHIC_ALLOCATOR = new AllocatorDefinition(
            "::zserio::pmr::PropagatingPolymorphicAllocator", "zserio/pmr/PropagatingPolymorphicAllocator.h");
    public static final AllocatorDefinition STD_ALLOCATOR =
            new AllocatorDefinition("::std::allocator", "memory");

    private static final PackageName STD_PACKAGE_NAME = new PackageName.Builder().addId("std").get();
    private static final PackageName ZSERIO_PACKAGE_NAME = new PackageName.Builder().addId("zserio").get();
    private static final PackageName ZSERIO_PMR_PACKAGE_NAME =
            new PackageName.Builder().addId("zserio").addId("pmr").get();
}
