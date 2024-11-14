package zserio.extension.cpp17.types;

import zserio.extension.cpp17.CppFullNameFormatter;
import zserio.extension.cpp17.CppLiteralFormatter;

/**
 * Native C++ StringView type mapping.
 */
public final class NativeStringViewType extends NativeStdType
{
    public NativeStringViewType()
    {
        super("string_view", "string_view");
    }

    public String formatLiteral(String value)
    {
        return CppFullNameFormatter.getFullName(getPackageName(), "string_view") + "{" +
                CppLiteralFormatter.formatStringLiteral(value) + "}";
    }
}
