package zserio.extension.cpp17.types;

/**
 * Native C++ integral type mapping.
 */
public final class NativeDynamicBitFieldType extends NativeIntegralType
{
    public NativeDynamicBitFieldType(String zserioTypeName, int numBits, boolean isSigned)
    {
        super(zserioTypeName, numBits, isSigned);
    }
}
