package zserio.extension.cpp17;

import zserio.ast.Constant;
import zserio.ast.StringType;
import zserio.ast.TypeInstantiation;
import zserio.extension.common.ExpressionFormatter;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.symbols.CppNativeSymbol;
import zserio.extension.cpp17.types.CppNativeType;
import zserio.extension.cpp17.types.NativeStringViewType;

/**
 * FreeMarker template data for ConstEmitter.
 */
public final class ConstEmitterTemplateData extends CppTemplateData
{
    public ConstEmitterTemplateData(TemplateDataContext context, Constant constant)
            throws ZserioExtensionException
    {
        super(context);

        final CppNativeMapper cppNativeMapper = context.getCppNativeMapper();
        final ExpressionFormatter cppExpressionFormatter =
                context.getExpressionFormatter(new HeaderIncludeCollectorAdapter(this));

        final CppNativeSymbol constantNativeSymbol = cppNativeMapper.getCppSymbol(constant);
        packageData = new PackageTemplateData(constantNativeSymbol.getPackageName());
        name = constantNativeSymbol.getName();

        final TypeInstantiation constantTypeInstantation = constant.getTypeInstantiation();
        if (constantTypeInstantation.getBaseType() instanceof StringType)
        {
            final NativeStringViewType nativeStringViewType = cppNativeMapper.getStringViewType();
            addHeaderIncludesForType(nativeStringViewType);
            typeInfo = NativeTypeInfoTemplateDataCreator.create(
                    context, nativeStringViewType, constantTypeInstantation.getTypeReference());
        }
        else
        {
            final CppNativeType nativeTargetType = cppNativeMapper.getCppType(constantTypeInstantation);
            addHeaderIncludesForType(nativeTargetType);

            typeInfo = NativeTypeInfoTemplateDataCreator.create(
                    context, nativeTargetType, constantTypeInstantation);
        }

        value = cppExpressionFormatter.formatGetter(constant.getValueExpression());
        docComments = DocCommentsDataCreator.createData(context, constant);
    }

    public PackageTemplateData getPackage()
    {
        return packageData;
    }

    public String getName()
    {
        return name;
    }

    public NativeTypeInfoTemplateData getTypeInfo()
    {
        return typeInfo;
    }

    public String getValue()
    {
        return value;
    }

    public DocCommentsTemplateData getDocComments()
    {
        return docComments;
    }

    private final PackageTemplateData packageData;
    private final String name;
    private final NativeTypeInfoTemplateData typeInfo;
    private final String value;
    private final DocCommentsTemplateData docComments;
}
