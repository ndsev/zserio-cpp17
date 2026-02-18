package zserio.extension.cpp17;

import java.util.Locale;

import zserio.ast.EnumItem;
import zserio.ast.Field;
import zserio.ast.Function;
import zserio.ast.Parameter;
import zserio.ast.TemplateParameter;

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

    public static String getTypeAliasName(Field field)
    {
        return pascalCase(field.getName()) + "Type";
    }

    public static String getTypeAliasName(TemplateParameter param)
    {
        return pascalCase(param.getName()) + "Type";
    }

    private static String uncapFirst(String name)
    {
        final StringBuilder nameFirstUncap = new StringBuilder();
        nameFirstUncap.append(String.valueOf(name.charAt(0)).toLowerCase(Locale.ENGLISH));
        nameFirstUncap.append(name.substring(1));

        return nameFirstUncap.toString();
    }

    // myField1A -> MyField1A
    // GEOMETRY_T = GeometryT
    private static String pascalCase(String name)
    {
        final StringBuilder sbu = new StringBuilder();
        boolean upper = true;
        for (int i = 0; i < name.length(); ++i)
        {
            char c = name.charAt(i);
            if (c == '_')
            {
                upper = true;
            }
            else if (Character.isDigit(c))
            {
                sbu.append(c);
                upper = true;
            }
            else if (upper)
            {
                sbu.append(String.valueOf(c).toUpperCase(Locale.ENGLISH));
                upper = false;
            }
            else
            {
                sbu.append(String.valueOf(c).toLowerCase(Locale.ENGLISH));
            }
            if (Character.isLowerCase(c) && i + 1 < name.length() && Character.isUpperCase(name.charAt(i + 1)))
                upper = true;
        }
        return sbu.toString();
    }

    private final static String REMOVED_ENUMERATOR_PREFIX = "ZSERIO_REMOVED_";
}
