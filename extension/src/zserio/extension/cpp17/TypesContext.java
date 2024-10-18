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
            bytes = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "Bytes", false, false, "zserio/Bytes.h");
            string = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "string", false, false, "zserio/String.h");
            vector = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "vector", true, false, "zserio/Vector.h");
        }
        else if (allocator.equals(PROPAGATING_POLYMORPHIC_ALLOCATOR))
        {
            bitBuffer = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "BitBuffer", false, false, "zserio/pmr/BitBuffer.h");
            bytes = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "Bytes", false, false, "zserio/pmr/Bytes.h");
            string = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "string", false, false, "zserio/pmr/String.h");
            vector = new NativeTypeDefinition(
                    ZSERIO_PMR_PACKAGE_NAME, "vector", true, false, "zserio/pmr/Vector.h");
        }
        else
        {
            bitBuffer = new NativeTypeDefinition(
                    ZSERIO_PACKAGE_NAME, "BasicBitBuffer", true, true, "zserio/BitBuffer.h");
            bytes = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "BasicBytes", true, true, "zserio/Bytes.h");
            string = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "string", true, true, "zserio/String.h");
            vector = new NativeTypeDefinition(ZSERIO_PACKAGE_NAME, "vector", true, true, "zserio/Vector.h");
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
    private final NativeTypeDefinition bytes;
    private final NativeTypeDefinition string;
    private final NativeTypeDefinition vector;

    public static final AllocatorDefinition PROPAGATING_POLYMORPHIC_ALLOCATOR = new AllocatorDefinition(
            "::zserio::pmr::PropagatingPolymorphicAllocator", "zserio/pmr/PolymorphicAllocator.h", "");
    public static final AllocatorDefinition STD_ALLOCATOR =
            new AllocatorDefinition("::std::allocator", "memory");

    private static final PackageName ZSERIO_PACKAGE_NAME = new PackageName.Builder().addId("zserio").get();
    private static final PackageName ZSERIO_PMR_PACKAGE_NAME =
            new PackageName.Builder().addId("zserio").addId("pmr").get();
}
