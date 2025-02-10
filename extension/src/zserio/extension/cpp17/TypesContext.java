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
            uniquePtr = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "unique_ptr", true, false, "zserio/UniquePtr.h");
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
        }
        else if (allocator.equals(PROPAGATING_POLYMORPHIC_ALLOCATOR))
        {
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
            map = new NativeTypeDefinition(ZSERIO_PMR_PACKAGE_NAME, "map", true, false, "zserio/pmr/Map.h");
            set = new NativeTypeDefinition(ZSERIO_PMR_PACKAGE_NAME, "set", true, false, "zserio/pmr/Set.h");
            optional = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "Optional", true, false, "zserio/pmr/Optional.h");
            uniquePtr = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "unique_ptr", true, false, "zserio/pmr/UniquePtr.h");
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
        }
        else
        {
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
            uniquePtr = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "unique_ptr", true, true, "zserio/UniquePtr.h");
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
        }
    }

    public AllocatorDefinition getAllocatorDefinition()
    {
        return allocator;
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
    private final NativeTypeDefinition bitBuffer;
    private final NativeTypeDefinition bitBufferView;
    private final NativeTypeDefinition bytes;
    private final NativeTypeDefinition string;
    private final NativeTypeDefinition vector;
    private final NativeTypeDefinition map;
    private final NativeTypeDefinition set;
    private final NativeTypeDefinition optional;
    private final NativeTypeDefinition uniquePtr;

    private final NativeTypeDefinition service;
    private final NativeTypeDefinition serviceClient;
    private final NativeTypeDefinition serviceDataPtr;
    private final NativeTypeDefinition objectServiceData;
    private final NativeTypeDefinition rawServiceDataHolder;
    private final NativeTypeDefinition rawServiceDataView;

    public static final AllocatorDefinition PROPAGATING_POLYMORPHIC_ALLOCATOR = new AllocatorDefinition(
            "::zserio::pmr::PropagatingPolymorphicAllocator", "zserio/pmr/PolymorphicAllocator.h", "");
    public static final AllocatorDefinition STD_ALLOCATOR =
            new AllocatorDefinition("::std::allocator", "memory");

    private static final PackageName STD_PACKAGE_NAME = new PackageName.Builder().addId("std").get();
    private static final PackageName ZSERIO_PACKAGE_NAME = new PackageName.Builder().addId("zserio").get();
    private static final PackageName ZSERIO_PMR_PACKAGE_NAME =
            new PackageName.Builder().addId("zserio").addId("pmr").get();
}
