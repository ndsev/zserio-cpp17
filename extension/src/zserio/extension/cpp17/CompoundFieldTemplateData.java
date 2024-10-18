package zserio.extension.cpp17;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;

import zserio.ast.ArrayInstantiation;
import zserio.ast.CompoundType;
import zserio.ast.DynamicBitFieldInstantiation;
import zserio.ast.Expression;
import zserio.ast.Field;
import zserio.ast.IntegerType;
import zserio.ast.ParameterizedTypeInstantiation;
import zserio.ast.ParameterizedTypeInstantiation.InstantiatedParameter;
import zserio.ast.TypeInstantiation;
import zserio.ast.TypeReference;
import zserio.ast.ZserioType;
import zserio.extension.common.ExpressionFormatter;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;
import zserio.extension.cpp17.types.NativeArrayType;
import zserio.extension.cpp17.types.NativeIntegralType;

/**
 * FreeMarker template data for compound fields.
 */
public final class CompoundFieldTemplateData
{
    public CompoundFieldTemplateData(TemplateDataContext context, CompoundType parentType, Field field,
            IncludeCollector includeCollector) throws ZserioExtensionException
    {
        final TypeInstantiation fieldTypeInstantiation = field.getTypeInstantiation();
        final ZserioType fieldBaseType = fieldTypeInstantiation.getBaseType();

        final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
        final CppNativeType fieldNativeType = cppNativeMapper.getCppType(fieldTypeInstantiation);
        includeCollector.addHeaderIncludesForType(fieldNativeType);

        name = field.getName();
        getterName = AccessorNameFormatter.getGetterName(field);

        isExtended = field.isExtended();
        isPackable = field.isPackable();

        alignmentValue = createAlignmentValue(context, field, includeCollector);
        initializer = createInitializer(context, field, includeCollector);
        constraint = createConstraint(context, field, includeCollector);
        offset = createOffset(context, field, includeCollector);

        optional = createOptional(context, field, fieldBaseType, parentType, includeCollector);
        typeInfo = createTypeInfo(context, fieldNativeType, fieldTypeInstantiation, includeCollector);
        compound = createCompound(context, fieldTypeInstantiation, includeCollector);
        integerRange = createIntegerRange(context, fieldTypeInstantiation, includeCollector);
        array = createArray(context, fieldNativeType, fieldTypeInstantiation, parentType, includeCollector);
        docComments = DocCommentsDataCreator.createData(context, field);
    }

    public Optional getOptional()
    {
        return optional;
    }

    public Compound getCompound()
    {
        return compound;
    }

    public String getName()
    {
        return name;
    }

    public NativeTypeInfoTemplateData getTypeInfo()
    {
        return typeInfo;
    }

    public String getGetterName()
    {
        return getterName;
    }

    public boolean getIsExtended()
    {
        return isExtended;
    }

    public boolean getIsPackable()
    {
        return isPackable;
    }

    public IntegerRange getIntegerRange()
    {
        return integerRange;
    }

    public String getAlignmentValue()
    {
        return alignmentValue;
    }

    public String getInitializer()
    {
        return initializer;
    }

    public Constraint getConstraint()
    {
        return constraint;
    }

    public Offset getOffset()
    {
        return offset;
    }

    public Array getArray()
    {
        return array;
    }

    public DocCommentsTemplateData getDocComments()
    {
        return docComments;
    }

    public static final class Optional
    {
        public Optional(TemplateDataContext context, Field field, boolean isRecursive,
                IncludeCollector includeCollector) throws ZserioExtensionException
        {
            this.isRecursive = isRecursive;
            final Expression optionalClauseExpression = field.getOptionalClauseExpr();
            final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
            final ExpressionFormatter viewIndirectExpressionFormatter =
                    context.getIndirectExpressionFormatter(includeCollector, "view");
            final ExpressionFormatter lhsIndirectExpressionFormatter =
                    context.getIndirectExpressionFormatter(includeCollector, "lhs");
            final ExpressionFormatter rhsIndirectExpressionFormatter =
                    context.getIndirectExpressionFormatter(includeCollector, "rhs");
            clause = (optionalClauseExpression == null)
                    ? null
                    : cppExpressionFormatter.formatGetter(optionalClauseExpression);
            viewIndirectClause = (optionalClauseExpression == null)
                    ? null
                    : viewIndirectExpressionFormatter.formatGetter(optionalClauseExpression);
            lhsIndirectClause = (optionalClauseExpression == null)
                    ? null
                    : lhsIndirectExpressionFormatter.formatGetter(optionalClauseExpression);
            rhsIndirectClause = (optionalClauseExpression == null)
                    ? null
                    : rhsIndirectExpressionFormatter.formatGetter(optionalClauseExpression);
        }

        public boolean getIsRecursive()
        {
            return isRecursive;
        }

        public String getClause()
        {
            return clause;
        }

        public String getViewIndirectClause()
        {
            return viewIndirectClause;
        }

        public String getlhsIndirectClause()
        {
            return lhsIndirectClause;
        }

        public String getRhsIndirectClause()
        {
            return rhsIndirectClause;
        }

        private final boolean isRecursive;
        private final String clause;
        private final String viewIndirectClause;
        private final String lhsIndirectClause;
        private final String rhsIndirectClause;
    }

    public static final class Compound
    {
        public Compound(TemplateDataContext context,
                ParameterizedTypeInstantiation parameterizedTypeInstantiation,
                IncludeCollector includeCollector) throws ZserioExtensionException
        {
            this(context, parameterizedTypeInstantiation.getBaseType(), includeCollector);

            for (InstantiatedParameter param : parameterizedTypeInstantiation.getInstantiatedParameters())
            {
                instantiatedParameters.add(new InstantiatedParameterData(context, param, includeCollector));
            }
        }

        public Compound(TemplateDataContext context, CompoundType compoundType,
                IncludeCollector includeCollector) throws ZserioExtensionException
        {
            instantiatedParameters = new ArrayList<InstantiatedParameterData>();
            parameters = CompoundTypeTemplateData.createParameterList(context, compoundType, includeCollector);
        }

        public Iterable<InstantiatedParameterData> getInstantiatedParameters()
        {
            return instantiatedParameters;
        }

        public Iterable<CompoundParameterTemplateData> getParameters()
        {
            return parameters;
        }

        public static final class InstantiatedParameterData
        {
            public InstantiatedParameterData(TemplateDataContext context,
                    InstantiatedParameter instantiatedParameter, IncludeCollector includeCollector)
                    throws ZserioExtensionException
            {
                final Expression argumentExpression = instantiatedParameter.getArgumentExpression();
                final ExpressionFormatter cppExpressionFormatter =
                        context.getExpressionFormatter(includeCollector);
                expression = cppExpressionFormatter.formatGetter(argumentExpression);
                final ExpressionFormatter viewIndirectExpressionFormatter =
                        context.getIndirectExpressionFormatter(includeCollector, "view");
                viewIndirectExpression = viewIndirectExpressionFormatter.formatGetter(argumentExpression);
                needsOwner = argumentExpression.requiresOwnerContext();
                needsIndex = argumentExpression.containsIndex();
                final TypeReference parameterTypeReference =
                        instantiatedParameter.getParameter().getTypeReference();
                final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
                final CppNativeType cppNativeType = cppNativeMapper.getCppType(parameterTypeReference);
                typeInfo = new NativeTypeInfoTemplateData(cppNativeType, parameterTypeReference);
            }

            public String getExpression()
            {
                return expression;
            }

            public String getViewIndirectExpression()
            {
                return viewIndirectExpression;
            }

            public boolean getNeedsOwner()
            {
                return needsOwner;
            }

            public boolean getNeedsIndex()
            {
                return needsIndex;
            }

            public NativeTypeInfoTemplateData getTypeInfo()
            {
                return typeInfo;
            }

            private final String expression;
            private final String viewIndirectExpression;
            private final boolean needsOwner;
            private final boolean needsIndex;
            private final NativeTypeInfoTemplateData typeInfo;
        }

        private final List<InstantiatedParameterData> instantiatedParameters;
        private final List<CompoundParameterTemplateData> parameters;
    }

    public static final class Constraint
    {
        public Constraint(Expression constraintExpression, ExpressionFormatter cppExpressionFormatter,
                ExpressionFormatter cppConstraintExpressionFormatter) throws ZserioExtensionException
        {
            writeConstraint = cppExpressionFormatter.formatGetter(constraintExpression);
            readConstraint = cppConstraintExpressionFormatter.formatGetter(constraintExpression);
        }

        public String getWriteConstraint()
        {
            return writeConstraint;
        }

        public String getReadConstraint()
        {
            return readConstraint;
        }

        private final String writeConstraint;
        private final String readConstraint;
    }

    public static final class Offset
    {
        public Offset(TemplateDataContext context, Expression offsetExpression,
                IncludeCollector includeCollector) throws ZserioExtensionException
        {
            final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
            getter = cppExpressionFormatter.formatGetter(offsetExpression);
            final ExpressionFormatter cppOwnerIndirectExpressionFormatter =
                    context.getIndirectExpressionFormatter(includeCollector, "owner");
            indirectGetter = cppOwnerIndirectExpressionFormatter.formatGetter(offsetExpression);
            setter = cppExpressionFormatter.formatSetter(offsetExpression);
            indirectSetter = cppOwnerIndirectExpressionFormatter.formatSetter(offsetExpression);
            final ZserioType offsetExprZserioType = offsetExpression.getExprZserioType();
            final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
            final CppNativeType nativeType = cppNativeMapper.getCppType(offsetExprZserioType);
            typeInfo = new NativeTypeInfoTemplateData(nativeType, offsetExprZserioType);
            containsIndex = offsetExpression.containsIndex();
        }

        public String getGetter()
        {
            return getter;
        }

        public String getIndirectGetter()
        {
            return indirectGetter;
        }

        public String getSetter()
        {
            return setter;
        }

        public String getIndirectSetter()
        {
            return indirectSetter;
        }

        public NativeTypeInfoTemplateData getTypeInfo()
        {
            return typeInfo;
        }

        public boolean getContainsIndex()
        {
            return containsIndex;
        }

        private final String getter;
        private final String indirectGetter;
        private final String setter;
        private final String indirectSetter;
        private final NativeTypeInfoTemplateData typeInfo;
        private final boolean containsIndex;
    }

    public static final class IntegerRange
    {
        public IntegerRange(boolean checkLowerBound, String lowerBound, String upperBound,
                NativeIntegralTypeInfoTemplateData typeInfo, String bitFieldLength)
                throws ZserioExtensionException
        {
            this.checkLowerBound = checkLowerBound;
            this.lowerBound = lowerBound;
            this.upperBound = upperBound;
            this.typeInfo = typeInfo;
            this.bitFieldLength = bitFieldLength;
        }

        public boolean getCheckLowerBound()
        {
            return checkLowerBound;
        }

        public String getLowerBound()
        {
            return lowerBound;
        }

        public String getUpperBound()
        {
            return upperBound;
        }

        public NativeIntegralTypeInfoTemplateData getTypeInfo()
        {
            return typeInfo;
        }

        public String getBitFieldLength()
        {
            return bitFieldLength;
        }

        private final boolean checkLowerBound;
        private final String lowerBound;
        private final String upperBound;
        private final NativeIntegralTypeInfoTemplateData typeInfo;
        private final String bitFieldLength;
    }

    public static final class Array
    {
        public Array(TemplateDataContext context, NativeArrayType nativeType,
                ArrayInstantiation arrayInstantiation, CompoundType parentType,
                IncludeCollector includeCollector) throws ZserioExtensionException
        {
            final TypeInstantiation elementTypeInstantiation = arrayInstantiation.getElementTypeInstantiation();

            isImplicit = arrayInstantiation.isImplicit();
            isPacked = arrayInstantiation.isPacked();
            final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
            length = createLength(arrayInstantiation, cppExpressionFormatter);
            final ExpressionFormatter viewIndirectExpressionFormatter =
                    context.getIndirectExpressionFormatter(includeCollector, "view");
            viewIndirectLength = createLength(arrayInstantiation, viewIndirectExpressionFormatter);
            isRecursive = elementTypeInstantiation.getBaseType() == parentType;
            // TODO[Mi-L@]: Is this right place where to add the element's include?
            final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
            final CppNativeType elementNativeType = cppNativeMapper.getCppType(elementTypeInstantiation);
            includeCollector.addHeaderIncludesForType(elementNativeType);
            elementUsedInPackedArray = context.getPackedTypesCollector().isUsedInPackedArray(
                    elementTypeInstantiation.getBaseType());
        }

        public boolean getIsImplicit()
        {
            return isImplicit;
        }

        public boolean getIsPacked()
        {
            return isPacked;
        }

        public String getLength()
        {
            return length;
        }

        public String getViewIndirectLength()
        {
            return viewIndirectLength;
        }

        public boolean getIsRecursive()
        {
            return isRecursive;
        }

        public boolean getElementUsedInPackedArray()
        {
            return elementUsedInPackedArray;
        }

        private static String createLength(ArrayInstantiation arrayInstantiation,
                ExpressionFormatter cppExpressionFormatter) throws ZserioExtensionException
        {
            final Expression lengthExpression = arrayInstantiation.getLengthExpression();
            if (lengthExpression == null)
                return null;

            return cppExpressionFormatter.formatGetter(lengthExpression);
        }

        private final boolean isImplicit;
        private final boolean isPacked;
        private final String length;
        private final String viewIndirectLength;
        private final boolean isRecursive;
        private final boolean elementUsedInPackedArray;
    }

    private static String getDynamicBitFieldLength(TypeInstantiation instantiation,
            ExpressionFormatter cppExpressionFormatter) throws ZserioExtensionException
    {
        if (!(instantiation instanceof DynamicBitFieldInstantiation))
            return null;

        final DynamicBitFieldInstantiation dynamicBitFieldInstantiation =
                (DynamicBitFieldInstantiation)instantiation;
        return cppExpressionFormatter.formatGetter(dynamicBitFieldInstantiation.getLengthExpression());
    }

    private static String createAlignmentValue(TemplateDataContext context, Field field,
            IncludeCollector includeCollector) throws ZserioExtensionException
    {
        final Expression alignmentExpression = field.getAlignmentExpr();
        if (alignmentExpression == null)
            return null;

        final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
        return cppExpressionFormatter.formatGetter(alignmentExpression);
    }

    private static String createInitializer(TemplateDataContext context, Field field,
            IncludeCollector includeCollector) throws ZserioExtensionException
    {
        final Expression initializerExpression = field.getInitializerExpr();
        if (initializerExpression == null)
            return null;

        final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
        return cppExpressionFormatter.formatGetter(initializerExpression);
    }

    private static Constraint createConstraint(TemplateDataContext context, Field field,
            IncludeCollector includeCollector) throws ZserioExtensionException
    {
        final Expression constraintExpression = field.getConstraintExpr();
        if (constraintExpression == null)
            return null;

        final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
        final CppConstraintExpressionFormattingPolicy expressionFormattingPolicy =
                new CppConstraintExpressionFormattingPolicy(cppNativeMapper, includeCollector, field);
        final ExpressionFormatter cppConstaintExpressionFormatter =
                new ExpressionFormatter(expressionFormattingPolicy);

        final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
        return new Constraint(constraintExpression, cppExpressionFormatter, cppConstaintExpressionFormatter);
    }

    private static Offset createOffset(TemplateDataContext context, Field field,
            IncludeCollector includeCollector) throws ZserioExtensionException
    {
        final Expression offsetExpression = field.getOffsetExpr();
        if (offsetExpression == null)
            return null;

        return new Offset(context, offsetExpression, includeCollector);
    }

    public static NativeTypeInfoTemplateData createTypeInfo(TemplateDataContext context,
            CppNativeType nativeType, TypeInstantiation typeInstantiation, IncludeCollector includeCollector)
            throws ZserioExtensionException
    {
        if (typeInstantiation instanceof ArrayInstantiation)
        {
            final TypeInstantiation elementTypeInstantiation =
                    ((ArrayInstantiation)typeInstantiation).getElementTypeInstantiation();
            final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
            final CppNativeType elementNativeType = cppNativeMapper.getCppType(elementTypeInstantiation);

            return NativeTypeInfoTemplateDataCreator.create(
                    context, elementNativeType, elementTypeInstantiation, includeCollector);
        }
        else
        {
            return NativeTypeInfoTemplateDataCreator.create(
                    context, nativeType, typeInstantiation, includeCollector);
        }
    }

    private static Optional createOptional(TemplateDataContext context, Field field, ZserioType baseFieldType,
            CompoundType parentType, IncludeCollector includeCollector) throws ZserioExtensionException
    {
        if (!field.isOptional())
            return null;

        final boolean isRecursive = baseFieldType == parentType;
        return new Optional(context, field, isRecursive, includeCollector);
    }

    private static Compound createCompound(TemplateDataContext context, TypeInstantiation typeInstantiation,
            IncludeCollector includeCollector) throws ZserioExtensionException
    {
        if (typeInstantiation instanceof ArrayInstantiation)
        {
            final TypeInstantiation elementTypeInstantiation =
                    ((ArrayInstantiation)typeInstantiation).getElementTypeInstantiation();
            return createCompoundImpl(context, elementTypeInstantiation, includeCollector);
        }
        else
        {
            return createCompoundImpl(context, typeInstantiation, includeCollector);
        }
    }

    private static Compound createCompoundImpl(TemplateDataContext context, TypeInstantiation typeInstantiation,
            IncludeCollector includeCollector) throws ZserioExtensionException
    {
        if (typeInstantiation instanceof ParameterizedTypeInstantiation)
        {
            return new Compound(context, (ParameterizedTypeInstantiation)typeInstantiation, includeCollector);
        }
        if (typeInstantiation.getBaseType() instanceof CompoundType)
        {
            return new Compound(context, (CompoundType)typeInstantiation.getBaseType(), includeCollector);
        }
        else
        {
            return null;
        }
    }

    private static Array createArray(TemplateDataContext context, CppNativeType cppNativeType,
            TypeInstantiation typeInstantiation, CompoundType parentType, IncludeCollector includeCollector)
            throws ZserioExtensionException
    {
        if (!(typeInstantiation instanceof ArrayInstantiation))
            return null;

        if (!(cppNativeType instanceof NativeArrayType))
        {
            throw new ZserioExtensionException("Inconsistent instantiation '" +
                    typeInstantiation.getClass().getName() + "' and native type '" +
                    cppNativeType.getClass().getName() + "'!");
        }

        return new Array(context, (NativeArrayType)cppNativeType, (ArrayInstantiation)typeInstantiation,
                parentType, includeCollector);
    }

    private static IntegerRange createIntegerRange(
            TemplateDataContext context, TypeInstantiation typeInstantiation, IncludeCollector includeCollector)
            throws ZserioExtensionException
    {
        if (!(typeInstantiation.getBaseType() instanceof IntegerType))
            return null;

        final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
        final String bitFieldLength = getDynamicBitFieldLength(typeInstantiation, cppExpressionFormatter);
        final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
        final NativeIntegralType nativeType = cppNativeMapper.getCppIntegralType(typeInstantiation);
        final IntegerType typeToCheck = (IntegerType)typeInstantiation.getBaseType();

        final BigInteger zserioLowerBound = typeToCheck.getLowerBound(typeInstantiation);
        final BigInteger zserioUpperBound = typeToCheck.getUpperBound(typeInstantiation);
        boolean checkLowerBound = true;
        boolean checkUpperBound = true;

        if (bitFieldLength == null)
        {
            final BigInteger nativeLowerBound = nativeType.getLowerBound();
            checkLowerBound = nativeLowerBound.compareTo(zserioLowerBound) < 0;

            final BigInteger nativeUpperBound = nativeType.getUpperBound();
            checkUpperBound = nativeUpperBound.compareTo(zserioUpperBound) > 0;
            final boolean hasFullRange = !checkLowerBound && !checkUpperBound;
            if (hasFullRange)
                return null;
        }

        final String lowerBound = nativeType.formatLiteral(zserioLowerBound);
        final String upperBound = nativeType.formatLiteral(zserioUpperBound);
        final NativeIntegralTypeInfoTemplateData typeInfo = NativeTypeInfoTemplateDataCreator.createIntegral(
                nativeType, typeInstantiation, cppExpressionFormatter);

        return new IntegerRange(checkLowerBound, lowerBound, upperBound, typeInfo, bitFieldLength);
    }

    private final Optional optional;
    private final String name;
    private final String getterName;
    private final boolean isExtended;
    private final boolean isPackable;
    private final String alignmentValue;
    private final String initializer;
    private final Constraint constraint;
    private final Offset offset;
    private final NativeTypeInfoTemplateData typeInfo;
    private final Compound compound;
    private final Array array;
    private final IntegerRange integerRange;
    private final DocCommentsTemplateData docComments;
}
