package zserio.extension.cpp17;

/**
 * FreeMarker template data with info about types.
 */
public class NativeTypeInfoTemplateData
{
    public NativeTypeInfoTemplateData(String typeFullName, boolean isSimple, boolean isNumeric,
            boolean isDynamicBitField, boolean isEnum, boolean isBitmask, boolean isBoolean, boolean isString,
            boolean isExtern, boolean isBytes, boolean isTemplateParameter, boolean needsAllocator)
    {
        this.typeFullName = typeFullName;
        this.isSimple = isSimple;
        this.isNumeric = isNumeric;
        this.isDynamicBitField = isDynamicBitField;
        this.isEnum = isEnum;
        this.isBitmask = isBitmask;
        this.isBoolean = isBoolean;
        this.isString = isString;
        this.isExtern = isExtern;
        this.isBytes = isBytes;
        this.isTemplateParameter = isTemplateParameter;
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

    public boolean getIsDynamicBitField()
    {
        return isDynamicBitField;
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

    public boolean getIsTemplateParameter()
    {
        return isTemplateParameter;
    }

    public boolean getNeedsAllocator()
    {
        return needsAllocator;
    }

    private final String typeFullName;
    private final boolean isSimple;
    private final boolean isNumeric;
    private final boolean isDynamicBitField;
    private final boolean isEnum;
    private final boolean isBitmask;
    private final boolean isBoolean;
    private final boolean isString;
    private final boolean isExtern;
    private final boolean isBytes;
    private final boolean isTemplateParameter;
    private final boolean needsAllocator;
}
