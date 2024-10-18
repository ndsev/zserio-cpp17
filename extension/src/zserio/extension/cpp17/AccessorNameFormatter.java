package zserio.extension.cpp17;

import java.util.Locale;

import zserio.ast.EnumItem;
import zserio.ast.Field;
import zserio.ast.Function;
import zserio.ast.Parameter;

/**
 * Accessor name formatter.
 *
 * Provides names of accessors (e.g. property names) as they will be generated in C++.
 */
public final class AccessorNameFormatter
{
    public static String getGetterName(Field field)
    {
        return getAccessorName(GETTER_NAME_PREFIX, field.getName());
    }

    public static String getGetterName(Parameter param)
    {
        return getAccessorName(GETTER_NAME_PREFIX, param.getName());
    }

    public static String getFunctionName(Function function)
    {
        return getAccessorName(FUNCTION_NAME_PREFIX, function.getName());
    }

    public static String getEnumeratorName(EnumItem enumItem)
    {
        return enumItem.isRemoved() ? REMOVED_ENUMERATOR_PREFIX + enumItem.getName() : enumItem.getName();
    }

    private static String getAccessorName(String accessorNamePrefix, String memberName)
    {
        return getAccessorName(accessorNamePrefix, memberName, "");
    }

    private static String getAccessorName(
            String accessorNamePrefix, String memberName, String accessorNameSuffix)
    {
        StringBuilder accessorName = new StringBuilder(accessorNamePrefix);
        if (!memberName.isEmpty())
        {
            if (accessorName.length() != 0)
            {
                final String firstMemberNameChar = String.valueOf(memberName.charAt(0));
                final String restMemberNameChars = memberName.substring(1, memberName.length());
                accessorName.append(firstMemberNameChar.toUpperCase(Locale.ENGLISH));
                accessorName.append(restMemberNameChars);
            }
            else
            {
                accessorName.append(memberName);
            }
        }
        if (!accessorNameSuffix.isEmpty())
            accessorName.append(accessorNameSuffix);

        return accessorName.toString();
    }

    private final static String GETTER_NAME_PREFIX = "";
    private final static String FUNCTION_NAME_PREFIX = "";
    private final static String REMOVED_ENUMERATOR_PREFIX = "ZSERIO_REMOVED_";
}
