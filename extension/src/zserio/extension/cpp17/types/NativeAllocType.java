package zserio.extension.cpp17.types;

import zserio.ast.PackageName;
import zserio.extension.cpp17.TypesContext;

/**
 * Native C++ runtime type mapping for types which depend on allocators.
 */
public class NativeAllocType extends NativeType
{
    public NativeAllocType(TypesContext.NativeTypeDefinition nativeTypeDefinition,
            TypesContext.AllocatorDefinition allocatorDefinition, NativeIntegralType allocatorTemplateArg)
    {
        this(nativeTypeDefinition, allocatorDefinition, nativeTypeDefinition.getPackage(),
                getName(nativeTypeDefinition, allocatorDefinition, allocatorTemplateArg.getFullName()));
    }

    public NativeAllocType(TypesContext.NativeTypeDefinition nativeTypeDefinition,
            TypesContext.AllocatorDefinition allocatorDefinition, String allocatorTemplateArgName)
    {
        this(nativeTypeDefinition, allocatorDefinition, nativeTypeDefinition.getPackage(),
                getName(nativeTypeDefinition, allocatorDefinition, allocatorTemplateArgName));
    }

    public NativeAllocType(TypesContext.NativeTypeDefinition nativeTypeDefinition,
            TypesContext.AllocatorDefinition allocatorDefinition)
    {
        this(nativeTypeDefinition, allocatorDefinition, nativeTypeDefinition.getPackage(),
                nativeTypeDefinition.getName());
    }

    public boolean needsAllocatorArgument()
    {
        return needsAllocatorArgument;
    }

    protected static String getName(TypesContext.NativeTypeDefinition nativeTypeDefinition,
            TypesContext.AllocatorDefinition allocatorDefinition, String allocatorTemplateArgName)
    {
        String name = nativeTypeDefinition.getName();
        if (nativeTypeDefinition.isTemplate())
        {
            name += "<" +
                    getAllocatorArgument(nativeTypeDefinition, allocatorDefinition, allocatorTemplateArgName) +
                    ">";
        }

        return name;
    }

    private static String getAllocatorArgument(TypesContext.NativeTypeDefinition nativeTypeDefinition,
            TypesContext.AllocatorDefinition allocatorDefinition, String allocatorTemplateArgName)
    {
        if (!nativeTypeDefinition.needsAllocatorArgument())
            return "";

        String allocatorArgument = allocatorDefinition.getAllocatorType();
        if (!allocatorTemplateArgName.isEmpty())
            allocatorArgument += "<" + allocatorTemplateArgName + ">";

        return allocatorArgument;
    }

    private NativeAllocType(TypesContext.NativeTypeDefinition nativeTypeDefinition,
            TypesContext.AllocatorDefinition allocatorDefinition, PackageName packageName, String name)
    {
        super(packageName, name);

        needsAllocatorArgument = nativeTypeDefinition.needsAllocatorArgument();
        if (needsAllocatorArgument)
            addSystemIncludeFile(allocatorDefinition.getAllocatorSystemInclude());
        addSystemIncludeFile(nativeTypeDefinition.getSystemInclude());
    }

    private final boolean needsAllocatorArgument;
}
