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
        return uncapFirst(field.getName());
    }

    public static String getGetterName(Parameter param)
    {
        return uncapFirst(param.getName());
    }

    public static String getFunctionName(Function function)
    {
        return uncapFirst(function.getName());
    }

    public static String getEnumeratorName(EnumItem enumItem)
    {
        return enumItem.isRemoved() ? REMOVED_ENUMERATOR_PREFIX + enumItem.getName() : enumItem.getName();
    }

    private static String uncapFirst(String name)
    {
        final StringBuilder nameFirstUncap = new StringBuilder();
        nameFirstUncap.append(String.valueOf(name.charAt(0)).toLowerCase(Locale.ENGLISH));
        nameFirstUncap.append(name.substring(1));

        return nameFirstUncap.toString();
    }

    private final static String REMOVED_ENUMERATOR_PREFIX = "ZSERIO_REMOVED_";
}
