package zserio.extension.cpp17;

/**
 * FreeMarker template data with info about types.
 */
public class NativeTypeInfoTemplateData
{
    public NativeTypeInfoTemplateData(String typeFullName, boolean isSimple, boolean isNumeric,
            boolean isDynamic, boolean isEnum, boolean isBitmask, boolean isBoolean, boolean isString,
            boolean isExtern, boolean isBytes, boolean needsAllocator)
    {
        this.typeFullName = typeFullName;
        this.isSimple = isSimple;
        this.isNumeric = isNumeric;
        this.isDynamic = isDynamic;
        this.isEnum = isEnum;
        this.isBitmask = isBitmask;
        this.isBoolean = isBoolean;
        this.isString = isString;
        this.isExtern = isExtern;
        this.isBytes = isBytes;
        this.needsAllocator = needsAllocator;
    }

    public String getTypeFullName()
    {
        return typeFullName;
    }

    public boolean getIsSimple()
    {
        return isSimple;
    }

    public boolean getIsNumeric()
    {
        return isNumeric;
    }

    public boolean getIsDynamic()
    {
        return isDynamic;
    }

    public boolean getIsEnum()
    {
        return isEnum;
    }

    public boolean getIsBitmask()
    {
        return isBitmask;
    }

    public boolean getIsBoolean()
    {
        return isBoolean;
    }

    public boolean getIsString()
    {
        return isString;
    }

    public boolean getIsExtern()
    {
        return isExtern;
    }

    public boolean getIsBytes()
    {
        return isBytes;
    }

    public boolean getNeedsAllocator()
    {
        return needsAllocator;
    }

    private final String typeFullName;
    private final boolean isSimple;
    private final boolean isNumeric;
    private final boolean isDynamic;
    private final boolean isEnum;
    private final boolean isBitmask;
    private final boolean isBoolean;
    private final boolean isString;
    private final boolean isExtern;
    private final boolean isBytes;
    private final boolean needsAllocator;
}
