package zserio.extension.cpp17.types;

/**
 * Native C++ integral type mapping.
 */
public final class NativeFixedBitFieldType extends NativeIntegralType
{
    public NativeFixedBitFieldType(int numBits, boolean isSigned, String length)
    {
        super(getName(numBits, isSigned, length), numBits, isSigned);
    }

    private static String getName(int numBits, boolean isSigned, String length)
    {
        final StringBuilder name = new StringBuilder();
        if (!isSigned)
            name.append('U');
        name.append("Int");
        name.append('<');
        name.append(length);
        name.append('>');

        return name.toString();
    }
}
