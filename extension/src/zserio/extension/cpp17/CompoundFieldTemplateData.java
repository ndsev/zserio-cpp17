package zserio.extension.cpp17;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
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
        IncludeCollector.addHeaderIncludes(
                cppNativeMapper, fieldNativeType, fieldTypeInstantiation.getTypeReference(), includeCollector);

        name = field.getName();
        getterName = AccessorNameFormatter.getGetterName(field);

        usedAsOffset = context.getOffsetFieldsCollector().isUsedAsOffset(field);

        isExtended = field.isExtended();
        if (isExtended)
        {
            includeCollector.addHeaderSystemIncludes(Arrays.asList("zserio/Extended.h"));
            includeCollector.addCppSystemIncludes(Arrays.asList("zserio/ExtendedFieldException.h"));
        }
        isPackable = field.isPackable();

        offset = createOffset(context, field, includeCollector);
        alignmentValue = createAlignmentValue(context, field, includeCollector);
        initializer = createInitializer(context, field, includeCollector);
        optional = createOptional(context, field, fieldBaseType, parentType, includeCollector);
        constraint = createConstraint(context, field, includeCollector);

        typeInfo = createTypeInfo(context, fieldNativeType, fieldTypeInstantiation, includeCollector);
        parameterized = createParameterized(context, fieldTypeInstantiation, includeCollector);
        compound = createCompound(context, fieldTypeInstantiation, includeCollector);
        dynamicBitFieldLength = createDynamicBitFieldLength(context, fieldTypeInstantiation, includeCollector);
        integerRange = createIntegerRange(context, fieldTypeInstantiation, includeCollector);
        array = createArray(context, fieldNativeType, fieldTypeInstantiation, parentType, includeCollector);
        docComments = DocCommentsDataCreator.createData(context, field);

        typeAliasName = array != null ? AccessorNameFormatter.getTypeAliasName(field) : "";
    }

    public DynamicBitLength getDynamicBitFieldLength()
    {
        return dynamicBitFieldLength;
    }

    public String getName()
    {
        return name;
    }

    public String getTypeAliasName()
    {
        return typeAliasName;
    }

    public String getGetterName()
    {
        return getterName;
    }

    public boolean getUsedAsOffset()
    {
        return usedAsOffset;
    }

    public boolean getIsExtended()
    {
        return isExtended;
    }

    public boolean getIsPackable()
    {
        return isPackable;
    }

    public Offset getOffset()
    {
        return offset;
    }

    public String getAlignmentValue()
    {
        return alignmentValue;
    }

    public String getInitializer()
    {
        return initializer;
    }

    public Optional getOptional()
    {
        return optional;
    }

    public Constraint getConstraint()
    {
        return constraint;
    }

    public NativeTypeInfoTemplateData getTypeInfo()
    {
        return typeInfo;
    }

    public Parameterized getParameterized()
    {
        return parameterized;
    }

    public Compound getCompound()
    {
        return compound;
    }

    public Array getArray()
    {
        return array;
    }

    public IntegerRange getIntegerRange()
    {
        return integerRange;
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
            if (optionalClauseExpression == null)
            {
                clause = null;
                viewIndirectClause = null;
                lhsIndirectClause = null;
                rhsIndirectClause = null;
            }
            else
            {
                includeCollector.addCppSystemIncludes(Arrays.asList("zserio/MissedOptionalException.h"));
                includeCollector.addCppSystemIncludes(Arrays.asList("zserio/UnexpectedOptionalException.h"));
                final ExpressionFormatter cppExpressionFormatter =
                        context.getExpressionFormatter(includeCollector);
                clause = cppExpressionFormatter.formatGetter(optionalClauseExpression);
                final ExpressionFormatter viewIndirectExpressionFormatter =
                        context.getIndirectExpressionFormatter(includeCollector, "view");
                viewIndirectClause = viewIndirectExpressionFormatter.formatGetter(optionalClauseExpression);
                final ExpressionFormatter lhsIndirectExpressionFormatter =
                        context.getIndirectExpressionFormatter(includeCollector, "lhs");
                lhsIndirectClause = lhsIndirectExpressionFormatter.formatGetter(optionalClauseExpression);
                final ExpressionFormatter rhsIndirectExpressionFormatter =
                        context.getIndirectExpressionFormatter(includeCollector, "rhs");
                rhsIndirectClause = rhsIndirectExpressionFormatter.formatGetter(optionalClauseExpression);
            }
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

    public static final class Constraint
    {
        public Constraint(Expression constraintExpression, ExpressionFormatter cppExpressionFormatter,
                ExpressionFormatter viewIndirectExpressionFormatter) throws ZserioExtensionException
        {
            expression = cppExpressionFormatter.formatGetter(constraintExpression);
            viewIndirectExpression = viewIndirectExpressionFormatter.formatGetter(constraintExpression);
        }

        public String getExpression()
        {
            return expression;
        }

        public String getViewIndirectExpression()
        {
            return viewIndirectExpression;
        }

        private final String expression;
        private final String viewIndirectExpression;
    }

    public static final class Offset
    {
        public Offset(TemplateDataContext context, Expression offsetExpression,
                IncludeCollector includeCollector) throws ZserioExtensionException
        {
            final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
            getter = cppExpressionFormatter.formatGetter(offsetExpression);
            final ExpressionFormatter cppViewIndirectExpressionFormatter =
                    context.getIndirectExpressionFormatter(includeCollector, "view");
            viewIndirectSetter = cppViewIndirectExpressionFormatter.formatSetter(offsetExpression);
            final ExpressionFormatter cppOwnerIndirectExpressionFormatter =
                    context.getIndirectExpressionFormatter(includeCollector, "m_owner");
            ownerIndirectSetter = cppOwnerIndirectExpressionFormatter.formatSetter(offsetExpression);
            final ZserioType offsetExprZserioType = offsetExpression.getExprZserioType();
            if (offsetExprZserioType != null)
            {
                final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
                final CppNativeType nativeType = cppNativeMapper.getCppType(offsetExprZserioType);
                typeInfo = NativeTypeInfoTemplateDataCreator.create(nativeType, offsetExprZserioType);
            }
            else
            {
                typeInfo = null;
            }
            containsIndex = offsetExpression.containsIndex();
        }

        public String getGetter()
        {
            return getter;
        }

        public String getViewIndirectSetter()
        {
            return viewIndirectSetter;
        }

        public String getOwnerIndirectSetter()
        {
            return ownerIndirectSetter;
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
        private final String viewIndirectSetter;
        private final String ownerIndirectSetter;
        private final NativeTypeInfoTemplateData typeInfo;
        private final boolean containsIndex;
    }

    public static final class Parameterized
    {
        public Parameterized(TemplateDataContext context,
                ParameterizedTypeInstantiation parameterizedTypeInstantiation,
                IncludeCollector includeCollector) throws ZserioExtensionException
        {
            boolean needsOwner = false;
            boolean needsIndex = false;
            final ExpressionFormatter expressionFormatter = context.getExpressionFormatter(includeCollector);
            final ExpressionFormatter ownerIndirectExpressionFormatter =
                    context.getIndirectExpressionFormatter(includeCollector, "owner");
            final ExpressionFormatter viewIndirectExpressionFormatter =
                    context.getIndirectExpressionFormatter(includeCollector, "view");
            for (Expression argumentExpression : parameterizedTypeInstantiation.getTypeArguments())
            {
                needsIndex |= argumentExpression.containsIndex();
                needsOwner |= argumentExpression.requiresOwnerContext();
                arguments.add(expressionFormatter.formatGetter(argumentExpression));
                ownerIndirectArguments.add(ownerIndirectExpressionFormatter.formatGetter(argumentExpression));
                viewIndirectArguments.add(viewIndirectExpressionFormatter.formatGetter(argumentExpression));
            }
            this.needsIndex = needsIndex;
            this.needsOwner = needsOwner;
        }

        public boolean getNeedsIndex()
        {
            return needsIndex;
        }

        public boolean getNeedsOwner()
        {
            return needsOwner;
        }

        public List<String> getArguments()
        {
            return arguments;
        }

        public List<String> getOwnerIndirectArguments()
        {
            return ownerIndirectArguments;
        }

        public List<String> getViewIndirectArguments()
        {
            return viewIndirectArguments;
        }

        private final boolean needsIndex;
        private final boolean needsOwner;
        private final List<String> arguments = new ArrayList<String>();
        private final List<String> ownerIndirectArguments = new ArrayList<String>();
        private final List<String> viewIndirectArguments = new ArrayList<String>();
    }

    public static final class Compound
    {
        public Compound(TemplateDataContext context,
                ParameterizedTypeInstantiation parameterizedTypeInstantiation,
                IncludeCollector includeCollector) throws ZserioExtensionException
        {
            this(context, (CompoundType)parameterizedTypeInstantiation.getBaseType(), includeCollector);

            for (InstantiatedParameter param : parameterizedTypeInstantiation.getInstantiatedParameters())
            {
                instantiatedParameters.add(new InstantiatedParameterData(context, param, includeCollector));
            }
        }

        public Compound(TemplateDataContext context, CompoundType baseType, IncludeCollector includeCollector)
                throws ZserioExtensionException
        {
            instantiatedParameters = new ArrayList<InstantiatedParameterData>();
            parameters = CompoundTypeTemplateData.createParameterList(context, baseType, includeCollector);
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
                final ExpressionFormatter viewIndirectExpressionFormatter =
                        context.getIndirectExpressionFormatter(includeCollector, "view");
                final ExpressionFormatter ownerIndirectExpressionFormatter =
                        context.getIndirectExpressionFormatter(includeCollector, "owner");
                expression = cppExpressionFormatter.formatGetter(argumentExpression);
                viewIndirectExpression = viewIndirectExpressionFormatter.formatGetter(argumentExpression);
                ownerIndirectExpression = ownerIndirectExpressionFormatter.formatGetter(argumentExpression);
                needsOwner = argumentExpression.requiresOwnerContext();
                needsIndex = argumentExpression.containsIndex();
                final TypeReference parameterTypeReference =
                        instantiatedParameter.getParameter().getTypeReference();
                final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
                final CppNativeType cppNativeType = cppNativeMapper.getCppType(parameterTypeReference);
                typeInfo = NativeTypeInfoTemplateDataCreator.create(
                        context, cppNativeType, parameterTypeReference);
            }

            public String getExpression()
            {
                return expression;
            }

            public String getViewIndirectExpression()
            {
                return viewIndirectExpression;
            }

            public String getOwnerIndirectExpression()
            {
                return ownerIndirectExpression;
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
            private final String ownerIndirectExpression;
            private final boolean needsOwner;
            private final boolean needsIndex;
            private final NativeTypeInfoTemplateData typeInfo;
        }

        private final List<InstantiatedParameterData> instantiatedParameters;
        private final List<CompoundParameterTemplateData> parameters;
    }

    public static final class DynamicBitLength
    {
        public DynamicBitLength(TemplateDataContext context,
                DynamicBitFieldInstantiation dynamicBitFieldInstantiation, IncludeCollector includeCollector)
                throws ZserioExtensionException
        {
            final Expression lengthExpression = dynamicBitFieldInstantiation.getLengthExpression();

            final ExpressionFormatter expressionFormatter = context.getExpressionFormatter(includeCollector);
            final ExpressionFormatter viewIndirectExpressionFormatter =
                    context.getIndirectExpressionFormatter(includeCollector, "view");
            final ExpressionFormatter ownerIndirectExpressionFormatter =
                    context.getIndirectExpressionFormatter(includeCollector, "owner");
            expression = expressionFormatter.formatGetter(lengthExpression);
            viewIndirectExpression = viewIndirectExpressionFormatter.formatGetter(lengthExpression);
            ownerIndirectExpression = ownerIndirectExpressionFormatter.formatGetter(lengthExpression);
            needsOwner = lengthExpression.requiresOwnerContext();
            needsIndex = lengthExpression.containsIndex();
        }

        public String getExpression()
        {
            return expression;
        }

        public String getViewIndirectExpression()
        {
            return viewIndirectExpression;
        }

        public String getOwnerIndirectExpression()
        {
            return ownerIndirectExpression;
        }

        public boolean getNeedsOwner()
        {
            return needsOwner;
        }

        public boolean getNeedsIndex()
        {
            return needsIndex;
        }

        private final String expression;
        private final String viewIndirectExpression;
        private final String ownerIndirectExpression;
        private final boolean needsOwner;
        private final boolean needsIndex;
    }

    public static final class IntegerRange
    {
        public IntegerRange(boolean checkLowerBound, String lowerBound, String upperBound,
                NativeTypeInfoTemplateData typeInfo, String bitFieldLength) throws ZserioExtensionException
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

        public NativeTypeInfoTemplateData getTypeInfo()
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
        private final NativeTypeInfoTemplateData typeInfo;
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
            IncludeCollector.addHeaderIncludes(cppNativeMapper, elementNativeType,
                    elementTypeInstantiation.getTypeReference(), includeCollector);
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

    private static Optional createOptional(TemplateDataContext context, Field field, ZserioType baseFieldType,
            CompoundType parentType, IncludeCollector includeCollector) throws ZserioExtensionException
    {
        if (!field.isOptional())
            return null;

        final boolean isRecursive = baseFieldType == parentType;
        return new Optional(context, field, isRecursive, includeCollector);
    }

    private static Constraint createConstraint(TemplateDataContext context, Field field,
            IncludeCollector includeCollector) throws ZserioExtensionException
    {
        final Expression constraintExpression = field.getConstraintExpr();
        if (constraintExpression == null)
            return null;

        includeCollector.addCppSystemIncludes(Arrays.asList("zserio/ConstraintException.h"));
        final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
        final ExpressionFormatter viewIndirectExpressionFormatter =
                context.getIndirectExpressionFormatter(includeCollector, "view");
        return new Constraint(constraintExpression, cppExpressionFormatter, viewIndirectExpressionFormatter);
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
                    context, elementNativeType, elementTypeInstantiation);
        }
        else
        {
            return NativeTypeInfoTemplateDataCreator.create(context, nativeType, typeInstantiation);
        }
    }

    private static Parameterized createParameterized(
            TemplateDataContext context, TypeInstantiation typeInstantiation, IncludeCollector includeCollector)
            throws ZserioExtensionException
    {
        if (typeInstantiation instanceof ArrayInstantiation)
        {
            final TypeInstantiation elementTypeInstantiation =
                    ((ArrayInstantiation)typeInstantiation).getElementTypeInstantiation();
            return createParameterizedImpl(context, elementTypeInstantiation, includeCollector);
        }
        else
        {
            return createParameterizedImpl(context, typeInstantiation, includeCollector);
        }
    }

    private static Parameterized createParameterizedImpl(
            TemplateDataContext context, TypeInstantiation typeInstantiation, IncludeCollector includeCollector)
            throws ZserioExtensionException
    {
        if (typeInstantiation instanceof ParameterizedTypeInstantiation)
        {
            return new Parameterized(
                    context, (ParameterizedTypeInstantiation)typeInstantiation, includeCollector);
        }
        else
        {
            return null;
        }
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
        if (typeInstantiation.getBaseType() instanceof CompoundType)
        {
            if (typeInstantiation instanceof ParameterizedTypeInstantiation)
            {
                return new Compound(
                        context, (ParameterizedTypeInstantiation)typeInstantiation, includeCollector);
            }
            else
            {
                return new Compound(context, (CompoundType)typeInstantiation.getBaseType(), includeCollector);
            }
        }
        else
        {
            return null;
        }
    }

    private static DynamicBitLength createDynamicBitFieldLength(
            TemplateDataContext context, TypeInstantiation typeInstantiation, IncludeCollector includeCollector)
            throws ZserioExtensionException
    {
        if (typeInstantiation instanceof ArrayInstantiation)
        {
            final TypeInstantiation elementTypeInstantiation =
                    ((ArrayInstantiation)typeInstantiation).getElementTypeInstantiation();
            return createDynamicBitLengthImpl(context, elementTypeInstantiation, includeCollector);
        }
        else
        {
            return createDynamicBitLengthImpl(context, typeInstantiation, includeCollector);
        }
    }

    private static DynamicBitLength createDynamicBitLengthImpl(
            TemplateDataContext context, TypeInstantiation typeInstantiation, IncludeCollector includeCollector)
            throws ZserioExtensionException
    {
        if (typeInstantiation instanceof DynamicBitFieldInstantiation)
        {
            final DynamicBitFieldInstantiation dynamicBitFieldInstantiation =
                    ((DynamicBitFieldInstantiation)typeInstantiation);
            if (dynamicBitFieldInstantiation.getLengthExpression().getIntegerValue() == null)
            {
                // only for dynamic bit fields which have unknown bit length
                return new DynamicBitLength(context, dynamicBitFieldInstantiation, includeCollector);
            }
        }
        return null;
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

        // TODO[TEMPLATES]: not evaluated for templates
        if (zserioLowerBound == null || zserioUpperBound == null)
            return null;

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
        final NativeTypeInfoTemplateData typeInfo =
                NativeTypeInfoTemplateDataCreator.create(context, nativeType, typeInstantiation);

        return new IntegerRange(checkLowerBound, lowerBound, upperBound, typeInfo, bitFieldLength);
    }

    private final String name;
    private final String typeAliasName;
    private final String getterName;
    private final boolean usedAsOffset;
    private final boolean isExtended;
    private final boolean isPackable;
    private final Offset offset;
    private final String alignmentValue;
    private final String initializer;
    private final Optional optional;
    private final Constraint constraint;
    private final NativeTypeInfoTemplateData typeInfo;
    private final Parameterized parameterized;
    private final Compound compound;
    private final DynamicBitLength dynamicBitFieldLength;
    private final Array array;
    private final IntegerRange integerRange;
    private final DocCommentsTemplateData docComments;
}
