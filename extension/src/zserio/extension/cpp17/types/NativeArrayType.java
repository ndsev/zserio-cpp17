package zserio.extension.cpp17.types;

/**
 * Native C++ array type mapping.
 */
public final class NativeArrayType extends NativeZserioType
{
    public NativeArrayType(CppNativeType elementType, NativeAllocType nativeVectorType)
    {
        super("Array");
        this.elementType = elementType;

        addSystemIncludeFile("zserio/Array.h");
        addIncludeFiles(nativeVectorType);
    }

    public CppNativeType getElementType()
    {
        return elementType;
    }

    private final CppNativeType elementType;
}
