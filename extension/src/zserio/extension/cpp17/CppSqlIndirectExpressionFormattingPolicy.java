package zserio.extension.cpp17;

import zserio.ast.Field;
import zserio.extension.common.ZserioExtensionException;

/**
 * A special formatting policy to be used in generated helper classes (e.g. factories) which need indirect
 * references.
 */
public final class CppSqlIndirectExpressionFormattingPolicy extends CppIndirectExpressionFormattingPolicy
{
    public CppSqlIndirectExpressionFormattingPolicy(
            CppNativeMapper cppNativeMapper, IncludeCollector includeCollector, String compoundTypeAccessPrefix)
    {
        super(cppNativeMapper, includeCollector, compoundTypeAccessPrefix);
    }

    @Override
    protected void formatFieldAccessor(StringBuilder result, boolean isMostLeftId, Field field,
            boolean isSetter) throws ZserioExtensionException
    {
        super.formatFieldAccessor(result, isMostLeftId, field, isSetter);

        if (isMostLeftId)
        {
            // row fields are always optional
            result.append(CPP_GETTER_OPTIONAL_VALUE);
        }
    }
}
