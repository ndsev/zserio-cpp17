package zserio.extension.cpp17;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;

import zserio.ast.BitmaskType;
import zserio.ast.BitmaskValue;
import zserio.ast.TypeInstantiation;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.NativeIntegralType;

/**
 * FreeMarker template data for BitmaskEmitter.
 */
public final class BitmaskEmitterTemplateData extends UserTypeTemplateData
{
    public BitmaskEmitterTemplateData(TemplateDataContext context, BitmaskType bitmaskType)
            throws ZserioExtensionException
    {
        super(context, bitmaskType, bitmaskType);

        usedInPackedArray = context.getPackedTypesCollector().isUsedInPackedArray(bitmaskType);

        final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();

        final TypeInstantiation bitmaskTypeInstantiation = bitmaskType.getTypeInstantiation();
        final NativeIntegralType nativeBaseType = cppNativeMapper.getCppIntegralType(bitmaskTypeInstantiation);
        addHeaderIncludesForType(nativeBaseType);

        underlyingTypeInfo = NativeTypeInfoTemplateDataCreator.create(
                context, nativeBaseType, bitmaskTypeInstantiation, new HeaderIncludeCollectorAdapter(this));

        final List<BitmaskValue> bitmaskValues = bitmaskType.getValues();
        values = new ArrayList<BitmaskValueData>(bitmaskValues.size());
        for (BitmaskValue bitmaskValue : bitmaskValues)
            values.add(new BitmaskValueData(context, nativeBaseType, bitmaskValue));
    }

    public boolean getUsedInPackedArray()
    {
        return usedInPackedArray;
    }

    public NativeTypeInfoTemplateData getUnderlyingTypeInfo()
    {
        return underlyingTypeInfo;
    }

    public Iterable<BitmaskValueData> getValues()
    {
        return values;
    }

    public static final class BitmaskValueData
    {
        public BitmaskValueData(TemplateDataContext context, NativeIntegralType nativeBaseType,
                BitmaskValue bitmaskValue) throws ZserioExtensionException
        {
            schemaName = bitmaskValue.getName();
            name = bitmaskValue.getName();
            value = nativeBaseType.formatLiteral(bitmaskValue.getValue());
            isZero = bitmaskValue.getValue().equals(BigInteger.ZERO);
            docComments = DocCommentsDataCreator.createData(context, bitmaskValue);
        }

        public String getSchemaName()
        {
            return schemaName;
        }

        public String getName()
        {
            return name;
        }

        public String getValue()
        {
            return value;
        }

        public boolean getIsDeprecated()
        {
            return false;
        }

        public boolean getIsRemoved()
        {
            return false;
        }

        public boolean getIsZero()
        {
            return isZero;
        }

        public DocCommentsTemplateData getDocComments()
        {
            return docComments;
        }

        private final String schemaName;
        private final String name;
        private final String value;
        private final boolean isZero;
        private final DocCommentsTemplateData docComments;
    };

    private final boolean usedInPackedArray;
    private final NativeTypeInfoTemplateData underlyingTypeInfo;
    private final List<BitmaskValueData> values;
}
