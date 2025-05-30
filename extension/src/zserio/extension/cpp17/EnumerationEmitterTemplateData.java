package zserio.extension.cpp17;

import java.util.ArrayList;
import java.util.List;

import zserio.ast.EnumItem;
import zserio.ast.EnumType;
import zserio.ast.TypeInstantiation;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.types.CppNativeType;
import zserio.extension.cpp17.types.NativeIntegralType;

/**
 * FreeMarker template data for EnumerationEmitter.
 */
public final class EnumerationEmitterTemplateData extends UserTypeTemplateData
{
    public EnumerationEmitterTemplateData(TemplateDataContext context, EnumType enumType)
            throws ZserioExtensionException
    {
        super(context, enumType, enumType);

        usedInPackedArray = context.getPackedTypesCollector().isUsedInPackedArray(enumType);

        final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
        final CppNativeType nativeEnumType = cppNativeMapper.getCppType(enumType);

        final TypeInstantiation enumTypeInstantiation = enumType.getTypeInstantiation();
        final NativeIntegralType nativeType = cppNativeMapper.getCppIntegralType(enumTypeInstantiation);
        addHeaderIncludesForType(nativeType);

        underlyingTypeInfo =
                NativeTypeInfoTemplateDataCreator.create(context, nativeType, enumTypeInstantiation);

        isSigned = nativeType.isSigned();
        nativeNumBits = NativeIntegralType.getNativeNumbits(nativeType.getNumBits());

        final List<EnumItem> enumItems = enumType.getItems();
        items = new ArrayList<EnumItemData>(enumItems.size());
        for (EnumItem enumItem : enumItems)
            items.add(new EnumItemData(context, nativeType, nativeEnumType, enumItem));
    }

    public boolean getUsedInPackedArray()
    {
        return usedInPackedArray;
    }

    public NativeTypeInfoTemplateData getUnderlyingTypeInfo()
    {
        return underlyingTypeInfo;
    }

    public boolean getIsSigned()
    {
        return isSigned;
    }

    public int getNativeNumBits()
    {
        return nativeNumBits;
    }

    public Iterable<EnumItemData> getItems()
    {
        return items;
    }

    public static final class EnumItemData
    {
        public EnumItemData(TemplateDataContext context, NativeIntegralType nativeType,
                CppNativeType nativeEnumType, EnumItem enumItem) throws ZserioExtensionException
        {
            schemaName = enumItem.getName();
            name = AccessorNameFormatter.getEnumeratorName(enumItem);
            fullName = CppFullNameFormatter.getFullName(
                    nativeEnumType.getPackageName(), nativeEnumType.getName(), name);
            value = nativeType.formatLiteral(enumItem.getValue());
            isDeprecated = enumItem.isDeprecated();
            isRemoved = enumItem.isRemoved();
            docComments = DocCommentsDataCreator.createData(context, enumItem);
        }

        public String getSchemaName()
        {
            return schemaName;
        }

        public String getName()
        {
            return name;
        }

        public String getFullName()
        {
            return fullName;
        }

        public String getValue()
        {
            return value;
        }

        public boolean getIsDeprecated()
        {
            return isDeprecated;
        }

        public boolean getIsRemoved()
        {
            return isRemoved;
        }

        public DocCommentsTemplateData getDocComments()
        {
            return docComments;
        }

        private final String schemaName;
        private final String name;
        private final String fullName;
        private final String value;
        private final boolean isDeprecated;
        private final boolean isRemoved;
        private final DocCommentsTemplateData docComments;
    };

    private final boolean usedInPackedArray;
    private final NativeTypeInfoTemplateData underlyingTypeInfo;
    private final boolean isSigned;
    private final int nativeNumBits;
    private final List<EnumItemData> items;
}
