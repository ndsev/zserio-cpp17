package zserio.extension.cpp17;

/**
 * A special formatting policy to be used in generated helper classes (e.g. factories) which need indirect
 * references.
 */
public class CppIndirectExpressionFormattingPolicy extends CppExpressionFormattingPolicy
{
    public CppIndirectExpressionFormattingPolicy(
            CppNativeMapper cppNativeMapper, IncludeCollector includeCollector, String compoundTypeAccessPrefix)
    {
        super(cppNativeMapper, includeCollector);
        this.compoundTypeAccessPrefix = compoundTypeAccessPrefix;
    }

    @Override
    protected String getAccessPrefixForCompoundType()
    {
        return compoundTypeAccessPrefix;
    }

    private final String compoundTypeAccessPrefix;
}
