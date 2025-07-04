package zserio.extension.cpp17;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;
import java.util.SortedSet;
import java.util.StringJoiner;
import java.util.TreeSet;

import zserio.ast.BitmaskType;
import zserio.ast.DynamicBitFieldInstantiation;
import zserio.ast.EnumType;
import zserio.ast.Expression;
import zserio.ast.Field;
import zserio.ast.IntegerType;
import zserio.ast.Parameter;
import zserio.ast.ParameterizedTypeInstantiation;
import zserio.ast.ParameterizedTypeInstantiation.InstantiatedParameter;
import zserio.ast.SqlConstraint;
import zserio.ast.SqlTableType;
import zserio.ast.TemplateParameter;
import zserio.ast.TypeInstantiation;
import zserio.ast.TypeReference;
import zserio.ast.ZserioType;
import zserio.extension.common.ExpressionFormatter;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.common.sql.SqlNativeTypeMapper;
import zserio.extension.common.sql.types.NativeBlobType;
import zserio.extension.common.sql.types.NativeIntegerType;
import zserio.extension.common.sql.types.NativeRealType;
import zserio.extension.common.sql.types.NativeTemplateType;
import zserio.extension.common.sql.types.SqlNativeType;
import zserio.extension.cpp17.SqlTableEmitterTemplateData.FieldTemplateData.DynamicBitLength;
import zserio.extension.cpp17.SqlTableEmitterTemplateData.FieldTemplateData.SqlRangeCheckData;
import zserio.extension.cpp17.types.CppNativeType;
import zserio.extension.cpp17.types.NativeIntegralType;
import zserio.tools.HashUtil;

/**
 * FreeMarker template data for SqlTableEmitter.
 */
public final class SqlTableEmitterTemplateData extends UserTypeTemplateData
{
    public SqlTableEmitterTemplateData(TemplateDataContext context, SqlTableType tableType)
            throws ZserioExtensionException
    {
        super(context, tableType, tableType);

        final IncludeCollector includeCollector =
                tableType.getTemplateParameters().isEmpty() ? this : new HeaderIncludeCollectorAdapter(this);

        final StringJoiner fullNameTemplateParameters = new StringJoiner(", ", "<", ">");
        fullNameTemplateParameters.setEmptyValue("");
        for (TemplateParameter templateParameter : tableType.getTemplateParameters())
        {
            templateParameters.add(templateParameter.getName());
            fullNameTemplateParameters.add(templateParameter.getName());
        }

        fullName = super.getFullName() + fullNameTemplateParameters.toString();

        final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
        sqlConstraint = createSqlConstraint(tableType.getSqlConstraint(), cppExpressionFormatter);
        virtualTableUsing = tableType.getVirtualTableUsingString();
        needsTypesInSchema = tableType.needsTypesInSchema();
        isWithoutRowId = tableType.isWithoutRowId();

        final List<Field> tableFields = tableType.getFields();
        fieldList = new ArrayList<FieldTemplateData>(tableFields.size());
        explicitParameters = new TreeSet<ExplicitParameterTemplateData>();
        boolean hasImplicitParameters = false;
        boolean requiresOwnerContext = false;
        for (Field tableField : tableFields)
        {
            final FieldTemplateData field =
                    new FieldTemplateData(context, tableType, tableField, includeCollector);
            fieldList.add(field);

            if (field.getHasImplicitParameters())
                hasImplicitParameters = true;

            int index = 0;
            for (FieldTemplateData.ParameterTemplateData parameterTemplateData : field.getTypeParameters())
            {
                if (parameterTemplateData.getIsExplicit())
                {
                    final String expression = parameterTemplateData.getExpression();
                    final NativeTypeInfoTemplateData typeInfo = parameterTemplateData.getTypeInfo();
                    explicitParameters.add(
                            new ExplicitParameterTemplateData(expression, typeInfo, field, index));
                }
                else
                {
                    if (parameterTemplateData.getRequiresOwnerContext())
                        requiresOwnerContext = true;
                }
                ++index;
            }

            if (field.getParameterized() != null)
            {
                index = 0;
                for (FieldTemplateData.Parameterized.Argument argument :
                        field.getParameterized().getArguments())
                {
                    if (argument.getIsExplicit())
                    {
                        explicitParameters.add(new ExplicitParameterTemplateData(
                                argument.getExpression(), null, field, index));
                    }
                    ++index;
                }
            }
        }
        this.hasImplicitParameters = hasImplicitParameters;
        this.requiresOwnerContext = requiresOwnerContext;

        templateInstantiation = TemplateInstantiationTemplateData.create(
                context, tableType, templateParameters, includeCollector);
    }

    @Override
    public String getFullName()
    {
        return fullName;
    }

    public List<String> getTemplateParameters()
    {
        return templateParameters;
    }

    public Iterable<FieldTemplateData> getFieldList()
    {
        return fieldList;
    }

    public Iterable<ExplicitParameterTemplateData> getExplicitParameters()
    {
        return explicitParameters;
    }

    public boolean getHasImplicitParameters()
    {
        return hasImplicitParameters;
    }

    public boolean getRequiresOwnerContext()
    {
        return requiresOwnerContext;
    }

    public String getSqlConstraint()
    {
        return sqlConstraint;
    }

    public String getVirtualTableUsing()
    {
        return virtualTableUsing;
    }

    public boolean getNeedsTypesInSchema()
    {
        return needsTypesInSchema;
    }

    public boolean getIsWithoutRowId()
    {
        return isWithoutRowId;
    }

    public TemplateInstantiationTemplateData getTemplateInstantiation()
    {
        return templateInstantiation;
    }

    public static final class ExplicitParameterTemplateData implements Comparable<ExplicitParameterTemplateData>
    {
        public ExplicitParameterTemplateData(
                String expression, NativeTypeInfoTemplateData typeInfo, FieldTemplateData field, int index)
        {
            this.expression = expression;
            this.typeInfo = typeInfo;
            this.field = field;
            this.index = index;
        }

        public String getExpression()
        {
            return expression;
        }

        public NativeTypeInfoTemplateData getTypeInfo()
        {
            return typeInfo;
        }

        public FieldTemplateData getField()
        {
            return field;
        }

        public int getIndex()
        {
            return index;
        }

        @Override
        public int compareTo(ExplicitParameterTemplateData other)
        {
            return expression.compareTo(other.expression);
        }

        @Override
        public boolean equals(Object other)
        {
            if (this == other)
                return true;

            if (other instanceof ExplicitParameterTemplateData)
            {
                return compareTo((ExplicitParameterTemplateData)other) == 0;
            }

            return false;
        }

        @Override
        public int hashCode()
        {
            int hash = HashUtil.HASH_SEED;
            hash = HashUtil.hash(hash, expression);
            return hash;
        }

        private final String expression;
        private final NativeTypeInfoTemplateData typeInfo;
        private final FieldTemplateData field;
        private final int index;
    }

    public static final class FieldTemplateData
    {
        public FieldTemplateData(TemplateDataContext context, SqlTableType table, Field field,
                IncludeCollector includeCollector) throws ZserioExtensionException
        {
            final TypeInstantiation fieldTypeInstantiation = field.getTypeInstantiation();
            final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
            final CppNativeType nativeFieldType = cppNativeMapper.getCppType(fieldTypeInstantiation);
            includeCollector.addHeaderIncludesForType(nativeFieldType);

            name = field.getName();

            typeInfo =
                    NativeTypeInfoTemplateDataCreator.create(context, nativeFieldType, fieldTypeInstantiation);

            final SqlConstraint fieldSqlConstraint = field.getSqlConstraint();
            final ExpressionFormatter cppExpressionFormatter = context.getExpressionFormatter(includeCollector);
            sqlConstraint = createSqlConstraint(fieldSqlConstraint, cppExpressionFormatter);
            isNotNull = !SqlConstraint.isNullAllowed(fieldSqlConstraint);
            isPrimaryKey = table.isFieldPrimaryKey(field);
            isVirtual = field.isVirtual();

            getterName = AccessorNameFormatter.getGetterName(field);

            boolean hasImplicitParameters = false;
            boolean hasExplicitParameters = false;
            boolean requiresOwnerContext = false;
            parameterized = createParameterized(context, fieldTypeInstantiation, includeCollector);
            if (parameterized != null)
            {
                hasExplicitParameters = parameterized.getHasExplicitParameter();
                hasImplicitParameters = parameterized.getHasImplicitParameter();
                requiresOwnerContext = parameterized.getNeedsOwner();
            }

            typeParameters = new ArrayList<ParameterTemplateData>();
            final ExpressionFormatter cppRowIndirectExpressionFormatter =
                    context.getSqlIndirectExpressionFormatter(includeCollector, "rowView");
            if (fieldTypeInstantiation instanceof ParameterizedTypeInstantiation)
            {
                final ParameterizedTypeInstantiation parameterizedInstantiation =
                        (ParameterizedTypeInstantiation)fieldTypeInstantiation;
                for (InstantiatedParameter parameter : parameterizedInstantiation.getInstantiatedParameters())
                {
                    final ParameterTemplateData parameterTemplateData = new ParameterTemplateData(context,
                            cppNativeMapper, cppRowIndirectExpressionFormatter, table, field, parameter,
                            includeCollector);
                    typeParameters.add(parameterTemplateData);
                    if (parameterTemplateData.getIsExplicit())
                        hasExplicitParameters = true;
                    else
                    {
                        hasImplicitParameters = true;
                        requiresOwnerContext = parameterTemplateData.requiresOwnerContext;
                    }
                }
            }
            this.hasImplicitParameters = hasImplicitParameters;
            this.hasExplicitParameters = hasExplicitParameters;
            this.requiresOwnerContext = requiresOwnerContext;

            dynamicBitFieldLength =
                    createDynamicBitLength(fieldTypeInstantiation, cppRowIndirectExpressionFormatter);

            final ZserioType fieldBaseType = fieldTypeInstantiation.getBaseType();
            underlyingTypeInfo =
                    createUnderlyingTypeInfo(context, cppNativeMapper, fieldBaseType, includeCollector);
            final SqlNativeTypeMapper sqlNativeTypeMapper = new SqlNativeTypeMapper();
            sqlTypeData = new SqlTypeTemplateData(sqlNativeTypeMapper, field);
            sqlRangeCheckData = createRangeCheckData(context, cppNativeMapper, fieldBaseType,
                    cppRowIndirectExpressionFormatter, fieldTypeInstantiation);

            docComments = DocCommentsDataCreator.createData(context, field);
        }

        public String getName()
        {
            return name;
        }

        public NativeTypeInfoTemplateData getTypeInfo()
        {
            return typeInfo;
        }

        public String getSqlConstraint()
        {
            return sqlConstraint;
        }

        public boolean getIsNotNull()
        {
            return isNotNull;
        }

        public boolean getIsPrimaryKey()
        {
            return isPrimaryKey;
        }

        public boolean getIsVirtual()
        {
            return isVirtual;
        }

        public String getGetterName()
        {
            return getterName;
        }

        public Parameterized getParameterized()
        {
            return parameterized;
        }

        public Iterable<ParameterTemplateData> getTypeParameters()
        {
            return typeParameters;
        }

        public boolean getHasImplicitParameters()
        {
            return hasImplicitParameters;
        }

        public boolean getHasExplicitParameters()
        {
            return hasExplicitParameters;
        }

        public boolean getRequiresOwnerContext()
        {
            return requiresOwnerContext;
        }

        public DynamicBitLength getDynamicBitFieldLength()
        {
            return dynamicBitFieldLength;
        }

        public NativeTypeInfoTemplateData getUnderlyingTypeInfo()
        {
            return underlyingTypeInfo;
        }

        public SqlTypeTemplateData getSqlTypeData()
        {
            return sqlTypeData;
        }

        public SqlRangeCheckData getSqlRangeCheckData()
        {
            return sqlRangeCheckData;
        }

        public DocCommentsTemplateData getDocComments()
        {
            return docComments;
        }

        public static final class ParameterTemplateData
        {
            public ParameterTemplateData(TemplateDataContext context, CppNativeMapper cppNativeMapper,
                    ExpressionFormatter cppRowIndirectExpressionFormatter, SqlTableType tableType, Field field,
                    InstantiatedParameter instantiatedParameter, IncludeCollector includeCollector)
                    throws ZserioExtensionException
            {
                final Parameter parameter = instantiatedParameter.getParameter();
                final TypeReference parameterTypeReference = parameter.getTypeReference();
                final CppNativeType parameterNativeType = cppNativeMapper.getCppType(parameterTypeReference);
                includeCollector.addHeaderIncludesForType(parameterNativeType);

                name = parameter.getName();
                typeInfo = NativeTypeInfoTemplateDataCreator.create(
                        context, parameterNativeType, parameterTypeReference);
                final Expression argumentExpression = instantiatedParameter.getArgumentExpression();
                isExplicit = argumentExpression.isExplicitVariable();
                expression = cppRowIndirectExpressionFormatter.formatGetter(argumentExpression);
                requiresOwnerContext = argumentExpression.requiresOwnerContext();
                getterName = AccessorNameFormatter.getGetterName(parameter);
            }

            public String getName()
            {
                return name;
            }

            public NativeTypeInfoTemplateData getTypeInfo()
            {
                return typeInfo;
            }

            public boolean getIsExplicit()
            {
                return isExplicit;
            }

            public String getExpression()
            {
                return expression;
            }

            public boolean getRequiresOwnerContext()
            {
                return requiresOwnerContext;
            }

            public String getGetterName()
            {
                return getterName;
            }

            private final String name;
            private final NativeTypeInfoTemplateData typeInfo;
            private final boolean isExplicit;
            private final String expression;
            private final boolean requiresOwnerContext;
            private final String getterName;
        }

        public static final class DynamicBitLength
        {
            public DynamicBitLength(DynamicBitFieldInstantiation dynamicBitFieldInstantiation,
                    ExpressionFormatter cppRowIndirectExpressionFormatter) throws ZserioExtensionException
            {
                final Expression lengthExpression = dynamicBitFieldInstantiation.getLengthExpression();

                expression = cppRowIndirectExpressionFormatter.formatGetter(lengthExpression);
                needsOwner = lengthExpression.requiresOwnerContext();
                needsIndex = lengthExpression.containsIndex();
            }

            public String getExpression()
            {
                return expression;
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
            private final boolean needsOwner;
            private final boolean needsIndex;
        }

        public static final class SqlTypeTemplateData
        {
            public SqlTypeTemplateData(SqlNativeTypeMapper sqlNativeTypeMapper, Field field)
                    throws ZserioExtensionException
            {
                final SqlNativeType sqlNativeType =
                        sqlNativeTypeMapper.getSqlType(field.getTypeInstantiation());
                name = sqlNativeType.getFullName();
                isBlob = sqlNativeType instanceof NativeBlobType;
                isInteger = sqlNativeType instanceof NativeIntegerType;
                isReal = sqlNativeType instanceof NativeRealType;
                isTemplate = sqlNativeType instanceof NativeTemplateType;
            }

            public String getName()
            {
                return name;
            }

            public boolean getIsBlob()
            {
                return isBlob;
            }

            public boolean getIsInteger()
            {
                return isInteger;
            }

            public boolean getIsReal()
            {
                return isReal;
            }

            public boolean getIsTemplate()
            {
                return isTemplate;
            }

            private final String name;
            private final boolean isBlob;
            private final boolean isInteger;
            private final boolean isReal;
            private final boolean isTemplate;
        }

        public static final class SqlRangeCheckData
        {
            public SqlRangeCheckData(boolean checkLowerBound, String lowerBound, String upperBound,
                    NativeTypeInfoTemplateData typeInfo, DynamicBitLength dynamicBitFieldLength)
                    throws ZserioExtensionException
            {
                this.checkLowerBound = checkLowerBound;
                this.lowerBound = lowerBound;
                this.upperBound = upperBound;
                this.typeInfo = typeInfo;
                this.bitFieldLength =
                        dynamicBitFieldLength != null ? dynamicBitFieldLength.getExpression() : null;
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

        public static final class Parameterized
        {
            public Parameterized(TemplateDataContext context,
                    ParameterizedTypeInstantiation parameterizedTypeInstantiation,
                    IncludeCollector includeCollector) throws ZserioExtensionException
            {
                boolean needsOwner = false;
                boolean needsIndex = false;
                boolean hasExplicitParameter = false;
                boolean hasImplicitParameter = true;
                final ExpressionFormatter rowViewIndirectExpressionFormatter =
                        context.getSqlIndirectExpressionFormatter(includeCollector, "rowView");
                for (Expression argumentExpression : parameterizedTypeInstantiation.getTypeArguments())
                {
                    final Argument argument = new Argument(argumentExpression,
                            rowViewIndirectExpressionFormatter.formatGetter(argumentExpression));
                    needsIndex |= argument.getNeedsIndex();
                    needsOwner |= argument.getNeedsOwner();
                    hasExplicitParameter = argument.getIsExplicit();
                    arguments.add(argument);
                }
                this.needsIndex = needsIndex;
                this.needsOwner = needsOwner;
                this.hasExplicitParameter = hasExplicitParameter;
                this.hasImplicitParameter = hasImplicitParameter;
            }

            public boolean getNeedsIndex()
            {
                return needsIndex;
            }

            public boolean getNeedsOwner()
            {
                return needsOwner;
            }

            public boolean getHasExplicitParameter()
            {
                return hasExplicitParameter;
            }

            public boolean getHasImplicitParameter()
            {
                return hasImplicitParameter;
            }

            public List<Argument> getArguments()
            {
                return arguments;
            }

            public static class Argument
            {
                public Argument(Expression argumentExpression, String rowViewIndirectExpression)
                {
                    needsIndex = argumentExpression.containsIndex();
                    needsOwner = argumentExpression.requiresOwnerContext();
                    isExplicit = argumentExpression.isExplicitVariable();
                    expression = rowViewIndirectExpression;
                }

                public boolean getNeedsIndex()
                {
                    return needsIndex;
                }

                public boolean getNeedsOwner()
                {
                    return needsOwner;
                }

                public boolean getIsExplicit()
                {
                    return isExplicit;
                }

                public String getExpression()
                {
                    return expression;
                }

                private final boolean needsIndex;
                private final boolean needsOwner;
                private final boolean isExplicit;
                private final String expression;
            }

            private final boolean needsIndex;
            private final boolean needsOwner;
            private final boolean hasExplicitParameter;
            private final boolean hasImplicitParameter;
            private final List<Argument> arguments = new ArrayList<Argument>();
        }

        private static NativeTypeInfoTemplateData createUnderlyingTypeInfo(TemplateDataContext context,
                CppNativeMapper cppNativeMapper, ZserioType fieldBaseType, IncludeCollector includeCollector)
                throws ZserioExtensionException
        {
            TypeInstantiation baseTypeInstantiation;
            if (fieldBaseType instanceof EnumType)
            {
                baseTypeInstantiation = ((EnumType)fieldBaseType).getTypeInstantiation();
            }
            else if (fieldBaseType instanceof BitmaskType)
            {
                baseTypeInstantiation = ((BitmaskType)fieldBaseType).getTypeInstantiation();
            }
            else
            {
                return null;
            }

            final CppNativeType nativeBaseType = cppNativeMapper.getCppType(baseTypeInstantiation);
            includeCollector.addCppIncludesForType(nativeBaseType);

            return NativeTypeInfoTemplateDataCreator.create(context, nativeBaseType, baseTypeInstantiation);
        }

        private static Parameterized createParameterized(TemplateDataContext context,
                TypeInstantiation typeInstantiation, IncludeCollector includeCollector)
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

        private final String name;
        private final NativeTypeInfoTemplateData typeInfo;
        private final String sqlConstraint;
        private final boolean isNotNull;
        private final boolean isPrimaryKey;
        private final boolean isVirtual;
        private final String getterName;
        private final Parameterized parameterized;
        private final List<ParameterTemplateData> typeParameters;
        private final boolean hasImplicitParameters;
        private final boolean hasExplicitParameters;
        private final boolean requiresOwnerContext;
        private final DynamicBitLength dynamicBitFieldLength;
        private final NativeTypeInfoTemplateData underlyingTypeInfo;
        private final SqlTypeTemplateData sqlTypeData;
        private final SqlRangeCheckData sqlRangeCheckData;
        private final DocCommentsTemplateData docComments;
    }

    private static DynamicBitLength createDynamicBitLength(TypeInstantiation typeInstantiation,
            ExpressionFormatter cppRowIndirectExpressionFormatter) throws ZserioExtensionException
    {
        if (typeInstantiation instanceof DynamicBitFieldInstantiation)
        {
            final DynamicBitFieldInstantiation dynamicBitFieldInstantiation =
                    ((DynamicBitFieldInstantiation)typeInstantiation);
            if (dynamicBitFieldInstantiation.getLengthExpression().getIntegerValue() == null)
            {
                // only for dynamic bit fields which have unknown bit length
                return new DynamicBitLength(dynamicBitFieldInstantiation, cppRowIndirectExpressionFormatter);
            }
        }
        return null;
    }

    private static String createSqlConstraint(SqlConstraint sqlConstraint,
            ExpressionFormatter cppExpressionFormatter) throws ZserioExtensionException
    {
        if (sqlConstraint == null)
            return null;

        return cppExpressionFormatter.formatGetter(sqlConstraint.getConstraintExpr());
    }

    private static SqlRangeCheckData createRangeCheckData(TemplateDataContext context,
            CppNativeMapper cppNativeMapper, ZserioType fieldBaseType,
            ExpressionFormatter cppRowIndirectExpressionFormatter, TypeInstantiation typeInstantiation)
            throws ZserioExtensionException
    {
        TypeInstantiation baseTypeInstantiation = typeInstantiation;
        if (fieldBaseType instanceof EnumType)
        {
            baseTypeInstantiation = ((EnumType)fieldBaseType).getTypeInstantiation();
        }
        else if (fieldBaseType instanceof BitmaskType)
        {
            baseTypeInstantiation = ((BitmaskType)fieldBaseType).getTypeInstantiation();
        }

        // in SQL, don't do range check non-integer types
        if (!(baseTypeInstantiation.getBaseType() instanceof IntegerType))
            return null;

        final DynamicBitLength dynamicBitFieldLength =
                createDynamicBitLength(baseTypeInstantiation, cppRowIndirectExpressionFormatter);
        final NativeIntegralType nativeType = cppNativeMapper.getCppIntegralType(baseTypeInstantiation);
        final boolean isSigned = nativeType.isSigned();
        final IntegerType typeToCheck = (IntegerType)baseTypeInstantiation.getBaseType();

        final BigInteger zserioLowerBound = typeToCheck.getLowerBound(baseTypeInstantiation);
        final BigInteger zserioUpperBound = typeToCheck.getUpperBound(baseTypeInstantiation);
        boolean checkLowerBound = true;
        boolean checkUpperBound = true;
        // since we use sqlite_column_int64, it has no sense to test 64-bit types
        final NativeIntegralType sqlNativeType =
                (isSigned) ? cppNativeMapper.getInt64Type() : cppNativeMapper.getUInt64Type();
        if (dynamicBitFieldLength == null)
        {
            final BigInteger nativeLowerBound = sqlNativeType.getLowerBound();
            checkLowerBound = nativeLowerBound.compareTo(zserioLowerBound) < 0;

            final BigInteger nativeUpperBound = sqlNativeType.getUpperBound();
            checkUpperBound = nativeUpperBound.compareTo(zserioUpperBound) > 0;
            final boolean hasFullRange = !checkLowerBound && !checkUpperBound;
            if (hasFullRange)
                return null;
        }

        final String lowerBound = nativeType.formatLiteral(zserioLowerBound);
        final String upperBound = nativeType.formatLiteral(zserioUpperBound);
        final NativeTypeInfoTemplateData typeInfo =
                NativeTypeInfoTemplateDataCreator.create(context, sqlNativeType, baseTypeInstantiation);

        return new SqlRangeCheckData(checkLowerBound, lowerBound, upperBound, typeInfo, dynamicBitFieldLength);
    }

    private final List<String> templateParameters = new ArrayList<String>();
    private final String fullName;

    private final List<FieldTemplateData> fieldList;
    private final SortedSet<ExplicitParameterTemplateData> explicitParameters;
    private final boolean hasImplicitParameters;
    private final boolean requiresOwnerContext;
    private final String sqlConstraint;
    private final String virtualTableUsing;
    private final boolean needsTypesInSchema;
    private final boolean isWithoutRowId;
    private final TemplateInstantiationTemplateData templateInstantiation;
}
