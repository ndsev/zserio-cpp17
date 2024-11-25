package zserio.extension.cpp17;

import java.util.ArrayList;
import java.util.List;

import zserio.ast.ChoiceCase;
import zserio.ast.ChoiceCaseExpression;
import zserio.ast.ChoiceDefault;
import zserio.ast.ChoiceType;
import zserio.ast.Expression;
import zserio.ast.Field;
import zserio.extension.common.ExpressionFormatter;
import zserio.extension.common.ZserioExtensionException;

/**
 * FreeMarker template data for ChoiceEmitter.
 */
public final class ChoiceEmitterTemplateData extends CompoundTypeTemplateData
{
    public ChoiceEmitterTemplateData(TemplateDataContext context, ChoiceType choiceType)
            throws ZserioExtensionException
    {
        super(context, choiceType);

        final Expression expression = choiceType.getSelectorExpression();
        final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(this);
        selectorExpression = cppExpressionFormatter.formatGetter(expression);
        final ExpressionFormatter cppLhsIndirectExpressionFormatter =
                context.getIndirectExpressionFormatter(this, "lhs");
        lhsIndirectSelectorExpression = cppLhsIndirectExpressionFormatter.formatGetter(expression);
        final ExpressionFormatter cppViewIndirectExpressionFormatter =
                context.getIndirectExpressionFormatter(this, "view");
        viewIndirectSelectorExpression = cppViewIndirectExpressionFormatter.formatGetter(expression);
        canUseNativeSwitch = expression.getExprType() != Expression.ExpressionType.BOOLEAN &&
                expression.getExprType() != Expression.ExpressionType.BITMASK;

        caseMemberList = new ArrayList<CaseMember>();
        final Iterable<ChoiceCase> choiceCaseTypes = choiceType.getChoiceCases();
        for (ChoiceCase choiceCaseType : choiceCaseTypes)
        {
            caseMemberList.add(new CaseMember(context, choiceType, choiceCaseType, this));
        }

        final ChoiceDefault choiceDefaultType = choiceType.getChoiceDefault();
        if (choiceDefaultType != null)
        {
            defaultMember = new DefaultMember(context, choiceType, choiceDefaultType, this);
        }
        else
        {
            defaultMember = null;
        }

        isDefaultUnreachable = choiceType.isChoiceDefaultUnreachable();
    }

    public String getSelectorExpression()
    {
        return selectorExpression;
    }

    public String getLhsIndirectSelectorExpression()
    {
        return lhsIndirectSelectorExpression;
    }

    public String getViewIndirectSelectorExpression()
    {
        return viewIndirectSelectorExpression;
    }

    public boolean getCanUseNativeSwitch()
    {
        return canUseNativeSwitch;
    }

    public Iterable<CaseMember> getCaseMemberList()
    {
        return caseMemberList;
    }

    public DefaultMember getDefaultMember()
    {
        return defaultMember;
    }

    public boolean getIsDefaultUnreachable()
    {
        return isDefaultUnreachable;
    }

    public static final class CaseMember
    {
        public CaseMember(TemplateDataContext context, ChoiceType choiceType, ChoiceCase choiceCaseType,
                IncludeCollector includeCollector) throws ZserioExtensionException
        {
            final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);

            expressionList = new ArrayList<String>();
            final Iterable<ChoiceCaseExpression> caseExpressions = choiceCaseType.getExpressions();
            for (ChoiceCaseExpression caseExpression : caseExpressions)
                expressionList.add(cppExpressionFormatter.formatGetter(caseExpression.getExpression()));

            final Field fieldType = choiceCaseType.getField();
            field = (fieldType != null)
                    ? new CompoundFieldTemplateData(context, choiceType, fieldType, includeCollector)
                    : null;
        }

        public List<String> getExpressionList()
        {
            return expressionList;
        }

        public CompoundFieldTemplateData getField()
        {
            return field;
        }

        private final List<String> expressionList;
        private final CompoundFieldTemplateData field;
    }

    public static final class DefaultMember
    {
        public DefaultMember(TemplateDataContext context, ChoiceType choiceType,
                ChoiceDefault choiceDefaultType, IncludeCollector includeCollector)
                throws ZserioExtensionException
        {
            final Field fieldType = choiceDefaultType.getField();
            field = (fieldType != null)
                    ? new CompoundFieldTemplateData(context, choiceType, fieldType, includeCollector)
                    : null;
        }

        public CompoundFieldTemplateData getField()
        {
            return field;
        }

        private final CompoundFieldTemplateData field;
    }

    private final String selectorExpression;
    private final String lhsIndirectSelectorExpression;
    private final String viewIndirectSelectorExpression;
    private final boolean canUseNativeSwitch;
    private final List<CaseMember> caseMemberList;
    private final DefaultMember defaultMember;
    private final boolean isDefaultUnreachable;
}
