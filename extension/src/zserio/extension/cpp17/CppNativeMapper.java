package zserio.extension.cpp17;

import zserio.ast.ArrayInstantiation;
import zserio.ast.AstNode;
import zserio.ast.BitmaskType;
import zserio.ast.BooleanType;
import zserio.ast.BytesType;
import zserio.ast.ChoiceType;
import zserio.ast.CompoundType;
import zserio.ast.Constant;
import zserio.ast.DynamicBitFieldInstantiation;
import zserio.ast.DynamicBitFieldType;
import zserio.ast.EnumType;
import zserio.ast.ExternType;
import zserio.ast.FixedBitFieldType;
import zserio.ast.FloatType;
import zserio.ast.InstantiateType;
import zserio.ast.PackageName;
import zserio.ast.PubsubType;
import zserio.ast.ServiceType;
import zserio.ast.SqlDatabaseType;
import zserio.ast.SqlTableType;
import zserio.ast.StdIntegerType;
import zserio.ast.StringType;
import zserio.ast.StructureType;
import zserio.ast.Subtype;
import zserio.ast.TypeInstantiation;
import zserio.ast.TypeReference;
import zserio.ast.UnionType;
import zserio.ast.VarIntegerType;
import zserio.ast.ZserioAstDefaultVisitor;
import zserio.ast.ZserioType;
import zserio.extension.common.ZserioExtensionException;
import zserio.extension.cpp17.symbols.CppNativeSymbol;
import zserio.extension.cpp17.types.CppNativeType;
import zserio.extension.cpp17.types.NativeAllocType;
import zserio.extension.cpp17.types.NativeArrayType;
import zserio.extension.cpp17.types.NativeCompoundType;
import zserio.extension.cpp17.types.NativeDynamicBitFieldType;
import zserio.extension.cpp17.types.NativeIntegralType;
import zserio.extension.cpp17.types.NativeNumericWrapperType;
import zserio.extension.cpp17.types.NativeStringViewType;
import zserio.extension.cpp17.types.NativeUserType;

/**
 * C++ native mapper.
 *
 * Provides mapping of types and symbols from Zserio package symbols to C++ native types and symbols.
 */
public final class CppNativeMapper
{
    public CppNativeMapper(TypesContext typesContext)
    {
        final TypesContext.AllocatorDefinition allocatorDefinition = typesContext.getAllocatorDefinition();

        stringViewType = new NativeStringViewType();
        anyType = new NativeAllocType(typesContext.getAny(), allocatorDefinition, "uint8_t");
        bitBufferType = new NativeAllocType(typesContext.getBitBuffer(), allocatorDefinition, "uint8_t");
        bitBufferViewType =
                new NativeAllocType(typesContext.getBitBufferView(), allocatorDefinition, "uint8_t");
        bytesType = new NativeAllocType(typesContext.getBytes(), allocatorDefinition);
        stringType = new NativeAllocType(typesContext.getString(), allocatorDefinition, "char");
        vectorType = new NativeAllocType(typesContext.getVector(), allocatorDefinition);

        mapType = new NativeAllocType(typesContext.getMap(), allocatorDefinition);
        setType = new NativeAllocType(typesContext.getSet(), allocatorDefinition);

        optionalType = new NativeAllocType(typesContext.getOptional(), allocatorDefinition);
        variantType = new NativeAllocType(typesContext.getVariant(), allocatorDefinition);
        uniquePtrType = new NativeAllocType(typesContext.getUniquePtr(), allocatorDefinition);

        serviceType = new NativeAllocType(typesContext.getService(), allocatorDefinition, "uint8_t");
        serviceClientType =
                new NativeAllocType(typesContext.getServiceClient(), allocatorDefinition, "uint8_t");
        serviceDataPtrType =
                new NativeAllocType(typesContext.getServiceDataPtr(), allocatorDefinition, "uint8_t");
        objectServiceDataType =
                new NativeAllocType(typesContext.getObjectServiceData(), allocatorDefinition, "uint8_t");
        rawServiceDataHolderType =
                new NativeAllocType(typesContext.getRawServiceDataHolder(), allocatorDefinition, "uint8_t");
        rawServiceDataViewType =
                new NativeAllocType(typesContext.getRawServiceDataView(), allocatorDefinition, "uint8_t");

        typeInfoType = new NativeAllocType(typesContext.getTypeInfo(), allocatorDefinition, "uint8_t");
        reflectablePtrType =
                new NativeAllocType(typesContext.getReflectablePtr(), allocatorDefinition, "uint8_t");
        reflectableConstPtrType =
                new NativeAllocType(typesContext.getReflectableConstPtr(), allocatorDefinition, "uint8_t");
    }

    public CppNativeSymbol getCppSymbol(AstNode symbol) throws ZserioExtensionException
    {
        if (symbol instanceof Constant)
        {
            final Constant constant = (Constant)symbol;
            final PackageName packageName = constant.getPackage().getPackageName();
            final String name = constant.getName();
            final String includeFileName = getIncludePath(packageName, name);
            return new CppNativeSymbol(packageName, name, includeFileName);
        }
        else
            throw new ZserioExtensionException(
                    "Unhandled symbol '" + symbol.getClass().getName() + "' in CppNativeMapper!");
    }

    public CppNativeType getCppType(TypeInstantiation typeInstantiation) throws ZserioExtensionException
    {
        if (typeInstantiation instanceof ArrayInstantiation)
            return mapArray((ArrayInstantiation)typeInstantiation);
        else if (typeInstantiation instanceof DynamicBitFieldInstantiation)
            return mapDynamicBitFieldInstantiation((DynamicBitFieldInstantiation)typeInstantiation);

        // don't resolve subtypes so that the subtype name (C++ typedef) will be used
        return getCppType(typeInstantiation.getType());
    }

    public CppNativeType getCppType(TypeReference typeReference) throws ZserioExtensionException
    {
        // don't resolve subtypes so that the subtype name (C++ typedef) will be used
        return getCppType(typeReference.getType());
    }

    public CppNativeType getCppType(ZserioType type) throws ZserioExtensionException
    {
        final TypeMapperVisitor visitor = new TypeMapperVisitor();
        type.accept(visitor);

        final CppNativeType nativeType = visitor.getCppType();
        if (nativeType == null)
            throw new ZserioExtensionException(
                    "Unhandled type '" + type.getClass().getName() + "' in CppNativeMapper!");

        return nativeType;
    }

    public NativeIntegralType getCppIntegralType(TypeInstantiation typeInstantiation)
            throws ZserioExtensionException
    {
        CppNativeType nativeType = null;
        if (typeInstantiation instanceof DynamicBitFieldInstantiation)
        {
            nativeType = mapDynamicBitFieldInstantiation((DynamicBitFieldInstantiation)typeInstantiation);
        }
        else
        {
            // use the base type to get the integral type (i.e. to resolve subtype)
            nativeType = getCppType(typeInstantiation.getBaseType());
        }

        if (!(nativeType instanceof NativeIntegralType))
        {
            throw new ZserioExtensionException("Unhandled integral type '" +
                    typeInstantiation.getBaseType().getName() + "' in CppNativeMapper!");
        }

        return (NativeIntegralType)nativeType;
    }

    public NativeStringViewType getStringViewType()
    {
        return stringViewType;
    }

    public NativeAllocType getAnyType()
    {
        return anyType;
    }

    public NativeAllocType getBitBufferType()
    {
        return bitBufferType;
    }

    public NativeAllocType getBitBufferViewType()
    {
        return bitBufferViewType;
    }

    public NativeAllocType getBytesType()
    {
        return bytesType;
    }

    public NativeAllocType getStringType()
    {
        return stringType;
    }

    public NativeAllocType getVectorType()
    {
        return vectorType;
    }

    public NativeAllocType getMapType()
    {
        return mapType;
    }

    public NativeAllocType getSetType()
    {
        return setType;
    }

    public NativeAllocType getOptionalType()
    {
        return optionalType;
    }

    public NativeAllocType getVariantType()
    {
        return variantType;
    }

    public NativeAllocType getUniquePtrType()
    {
        return uniquePtrType;
    }

    public NativeAllocType getServiceType()
    {
        return serviceType;
    }

    public NativeAllocType getServiceClientType()
    {
        return serviceClientType;
    }

    public NativeAllocType getServiceDataPtrType()
    {
        return serviceDataPtrType;
    }

    public NativeAllocType getObjectServiceDataType()
    {
        return objectServiceDataType;
    }

    public NativeAllocType getRawServiceDataHolderType()
    {
        return rawServiceDataHolderType;
    }

    public NativeAllocType getRawServiceDataViewType()
    {
        return rawServiceDataViewType;
    }

    public NativeAllocType getTypeInfoType()
    {
        return typeInfoType;
    }

    public NativeAllocType getReflectablePtrType()
    {
        return reflectablePtrType;
    }

    public NativeAllocType getReflectableConstPtrType()
    {
        return reflectableConstPtrType;
    }

    public NativeIntegralType getUInt64Type()
    {
        return uint64Type;
    }

    public NativeIntegralType getInt64Type()
    {
        return int64Type;
    }

    private CppNativeType mapArray(ArrayInstantiation instantiation) throws ZserioExtensionException
    {
        final TypeInstantiation elementInstantiation = instantiation.getElementTypeInstantiation();

        final CppNativeType nativeType = getCppType(elementInstantiation);
        return new NativeArrayType(nativeType, vectorType);
    }

    private static CppNativeType mapDynamicBitFieldInstantiation(DynamicBitFieldInstantiation instantiation)
            throws ZserioExtensionException
    {
        final boolean isSigned = instantiation.getBaseType().isSigned();
        final int maxBitSize = instantiation.getMaxBitSize();

        return mapDynamicBitFieldType(isSigned, maxBitSize);
    }

    private static CppNativeType mapBitFieldType(boolean isSigned, int numBits)
    {
        return isSigned ? mapSignedBitFieldType(numBits) : mapUnsignedBitFieldType(numBits);
    }

    private static CppNativeType mapSignedBitFieldType(int numBits)
    {
        switch (numBits)
        {
        case 1:
            return int1Type;
        case 2:
            return int2Type;
        case 3:
            return int3Type;
        case 4:
            return int4Type;
        case 5:
            return int5Type;
        case 6:
            return int6Type;
        case 7:
            return int7Type;
        case 8:
            return int8Type;
        case 9:
            return int9Type;
        case 10:
            return int10Type;
        case 11:
            return int11Type;
        case 12:
            return int12Type;
        case 13:
            return int13Type;
        case 14:
            return int14Type;
        case 15:
            return int15Type;
        case 16:
            return int16Type;
        case 17:
            return int17Type;
        case 18:
            return int18Type;
        case 19:
            return int19Type;
        case 20:
            return int20Type;
        case 21:
            return int21Type;
        case 22:
            return int22Type;
        case 23:
            return int23Type;
        case 24:
            return int24Type;
        case 25:
            return int25Type;
        case 26:
            return int26Type;
        case 27:
            return int27Type;
        case 28:
            return int28Type;
        case 29:
            return int29Type;
        case 30:
            return int30Type;
        case 31:
            return int31Type;
        case 32:
            return int32Type;
        case 33:
            return int33Type;
        case 34:
            return int34Type;
        case 35:
            return int35Type;
        case 36:
            return int36Type;
        case 37:
            return int37Type;
        case 38:
            return int38Type;
        case 39:
            return int39Type;
        case 40:
            return int40Type;
        case 41:
            return int41Type;
        case 42:
            return int42Type;
        case 43:
            return int43Type;
        case 44:
            return int44Type;
        case 45:
            return int45Type;
        case 46:
            return int46Type;
        case 47:
            return int47Type;
        case 48:
            return int48Type;
        case 49:
            return int49Type;
        case 50:
            return int50Type;
        case 51:
            return int51Type;
        case 52:
            return int52Type;
        case 53:
            return int53Type;
        case 54:
            return int54Type;
        case 55:
            return int55Type;
        case 56:
            return int56Type;
        case 57:
            return int57Type;
        case 58:
            return int58Type;
        case 59:
            return int59Type;
        case 60:
            return int60Type;
        case 61:
            return int61Type;
        case 62:
            return int62Type;
        case 63:
            return int63Type;
        case 64:
            return int64Type;

        default:
            // invalid
            return null;
        }
    }

    private static CppNativeType mapUnsignedBitFieldType(int numBits)
    {
        switch (numBits)
        {
        case 1:
            return uint1Type;
        case 2:
            return uint2Type;
        case 3:
            return uint3Type;
        case 4:
            return uint4Type;
        case 5:
            return uint5Type;
        case 6:
            return uint6Type;
        case 7:
            return uint7Type;
        case 8:
            return uint8Type;
        case 9:
            return uint9Type;
        case 10:
            return uint10Type;
        case 11:
            return uint11Type;
        case 12:
            return uint12Type;
        case 13:
            return uint13Type;
        case 14:
            return uint14Type;
        case 15:
            return uint15Type;
        case 16:
            return uint16Type;
        case 17:
            return uint17Type;
        case 18:
            return uint18Type;
        case 19:
            return uint19Type;
        case 20:
            return uint20Type;
        case 21:
            return uint21Type;
        case 22:
            return uint22Type;
        case 23:
            return uint23Type;
        case 24:
            return uint24Type;
        case 25:
            return uint25Type;
        case 26:
            return uint26Type;
        case 27:
            return uint27Type;
        case 28:
            return uint28Type;
        case 29:
            return uint29Type;
        case 30:
            return uint30Type;
        case 31:
            return uint31Type;
        case 32:
            return uint32Type;
        case 33:
            return uint33Type;
        case 34:
            return uint34Type;
        case 35:
            return uint35Type;
        case 36:
            return uint36Type;
        case 37:
            return uint37Type;
        case 38:
            return uint38Type;
        case 39:
            return uint39Type;
        case 40:
            return uint40Type;
        case 41:
            return uint41Type;
        case 42:
            return uint42Type;
        case 43:
            return uint43Type;
        case 44:
            return uint44Type;
        case 45:
            return uint45Type;
        case 46:
            return uint46Type;
        case 47:
            return uint47Type;
        case 48:
            return uint48Type;
        case 49:
            return uint49Type;
        case 50:
            return uint50Type;
        case 51:
            return uint51Type;
        case 52:
            return uint52Type;
        case 53:
            return uint53Type;
        case 54:
            return uint54Type;
        case 55:
            return uint55Type;
        case 56:
            return uint56Type;
        case 57:
            return uint57Type;
        case 58:
            return uint58Type;
        case 59:
            return uint59Type;
        case 60:
            return uint60Type;
        case 61:
            return uint61Type;
        case 62:
            return uint62Type;
        case 63:
            return uint63Type;
        case 64:
            return uint64Type;

        default:
            // invalid
            return null;
        }
    }

    private static CppNativeType mapDynamicBitFieldType(boolean isSigned, int maxBitSize)
    {
        if (maxBitSize <= 8)
            return isSigned ? dynInt8Type : dynUInt8Type;
        else if (maxBitSize <= 16)
            return isSigned ? dynInt16Type : dynUInt16Type;
        else if (maxBitSize <= 32)
            return isSigned ? dynInt32Type : dynUInt32Type;
        else
            return isSigned ? dynInt64Type : dynUInt64Type;
    }

    private String getIncludePathRoot(PackageName packageName)
    {
        if (packageName.isEmpty())
            return "";

        return packageName.toString(INCLUDE_DIR_SEPARATOR) + INCLUDE_DIR_SEPARATOR;
    }

    private String getIncludePath(PackageName packageName, String name)
    {
        return getIncludePathRoot(packageName) + name + HEADER_SUFFIX;
    }

    private class TypeMapperVisitor extends ZserioAstDefaultVisitor
    {
        public CppNativeType getCppType()
        {
            return cppType;
        }

        @Override
        public void visitStdIntegerType(StdIntegerType type)
        {
            final int numBits = type.getBitSize();
            final boolean isSigned = type.isSigned();
            if (numBits <= 8)
                cppType = isSigned ? int8Type : uint8Type;
            else if (numBits <= 16)
                cppType = isSigned ? int16Type : uint16Type;
            else if (numBits <= 32)
                cppType = isSigned ? int32Type : uint32Type;
            else
                cppType = isSigned ? int64Type : uint64Type;
        }

        @Override
        public void visitVarIntegerType(VarIntegerType type)
        {
            switch (type.getMaxBitSize())
            {
            case 16:
                cppType = (type.isSigned()) ? varInt16Type : varUInt16Type;
                break;

            case 32:
                cppType = (type.isSigned()) ? varInt32Type : varUInt32Type;
                break;

            case 40:
                if (!type.isSigned())
                    cppType = varSizeType;
                break;

            case 64:
                cppType = (type.isSigned()) ? varInt64Type : varUInt64Type;
                break;

            case 72:
                cppType = (type.isSigned()) ? varIntType : varUIntType;
                break;

            default:
                // not supported
                break;
            }
        }

        @Override
        public void visitBooleanType(BooleanType type)
        {
            cppType = booleanType;
        }

        @Override
        public void visitBytesType(BytesType type)
        {
            cppType = bytesType;
        }

        @Override
        public void visitChoiceType(ChoiceType type)
        {
            mapCompoundType(type);
        }

        @Override
        public void visitEnumType(EnumType type)
        {
            final PackageName packageName = type.getPackage().getPackageName();
            final String name = type.getName();
            final String includeFileName = getIncludePath(packageName, name);
            cppType = new NativeUserType(packageName, name, includeFileName);
        }

        @Override
        public void visitBitmaskType(BitmaskType type)
        {
            final PackageName packageName = type.getPackage().getPackageName();
            final String name = type.getName();
            final String includeFileName = getIncludePath(packageName, name);
            cppType = new NativeUserType(packageName, name, includeFileName);
        }

        @Override
        public void visitFloatType(FloatType type)
        {
            switch (type.getBitSize())
            {
            case 16:
                cppType = float16Type;
                break;

            case 32:
                cppType = float32Type;
                break;

            case 64:
                cppType = float64Type;
                break;

            default:
                break;
            }
        }

        @Override
        public void visitExternType(ExternType type)
        {
            cppType = bitBufferType;
        }

        @Override
        public void visitServiceType(ServiceType type)
        {
            final PackageName packageName = type.getPackage().getPackageName();
            final String name = type.getName();
            final String includeFileName = getIncludePath(packageName, name);
            cppType = new NativeUserType(packageName, name, includeFileName);
        }

        @Override
        public void visitPubsubType(PubsubType type)
        {
            final PackageName packageName = type.getPackage().getPackageName();
            final String name = type.getName();
            final String includeFileName = getIncludePath(packageName, name);
            cppType = new NativeUserType(packageName, name, includeFileName);
        }

        @Override
        public void visitSqlDatabaseType(SqlDatabaseType type)
        {
            final PackageName packageName = type.getPackage().getPackageName();
            final String name = type.getName();
            final String includeFileName = getIncludePath(packageName, name);
            cppType = new NativeUserType(packageName, name, includeFileName);
        }

        @Override
        public void visitSqlTableType(SqlTableType type)
        {
            final PackageName packageName = type.getPackage().getPackageName();
            final String name = type.getName();
            final String includeFileName = getIncludePath(packageName, name);
            cppType = new NativeUserType(packageName, name, includeFileName);
        }

        @Override
        public void visitStringType(StringType type)
        {
            cppType = stringType;
        }

        @Override
        public void visitStructureType(StructureType type)
        {
            mapCompoundType(type);
        }

        @Override
        public void visitSubtype(Subtype type)
        {
            mapAliasType(type, type.getTypeReference());
        }

        @Override
        public void visitUnionType(UnionType type)
        {
            mapCompoundType(type);
        }

        @Override
        public void visitInstantiateType(InstantiateType type)
        {
            mapAliasType(type, type.getTypeReference());
        }

        @Override
        public void visitFixedBitFieldType(FixedBitFieldType type)
        {
            cppType = CppNativeMapper.mapBitFieldType(type.isSigned(), type.getBitSize());
        }

        @Override
        public void visitDynamicBitFieldType(DynamicBitFieldType type)
        {
            // this is only for reference to dynamic bit field type (e.g. when used as compound parameter)
            cppType = CppNativeMapper.mapDynamicBitFieldType(type.isSigned(), DynamicBitFieldType.MAX_BIT_SIZE);
        }

        private void mapCompoundType(CompoundType type)
        {
            final PackageName packageName = type.getPackage().getPackageName();
            final String name = type.getName();
            final String includeFileName = getIncludePath(packageName, name);
            cppType = new NativeCompoundType(packageName, name, includeFileName);
        }

        private void mapAliasType(ZserioType aliasType, TypeReference referencedType)
        {
            final PackageName packageName = aliasType.getPackage().getPackageName();
            final String name = aliasType.getName();
            final String includeFileName = getIncludePath(packageName, name);
            cppType = new NativeUserType(packageName, name, includeFileName);
        }

        private CppNativeType cppType = null;
    }

    private final static String INCLUDE_DIR_SEPARATOR = "/";
    private final static String HEADER_SUFFIX = ".h";

    private final NativeStringViewType stringViewType;
    private final NativeAllocType anyType;
    private final NativeAllocType bitBufferType;
    private final NativeAllocType bitBufferViewType;
    private final NativeAllocType bytesType;
    private final NativeAllocType stringType;
    private final NativeAllocType vectorType;
    private final NativeAllocType mapType;
    private final NativeAllocType setType;
    private final NativeAllocType optionalType;
    private final NativeAllocType variantType;
    private final NativeAllocType uniquePtrType;

    private final NativeAllocType serviceType;
    private final NativeAllocType serviceClientType;
    private final NativeAllocType serviceDataPtrType;
    private final NativeAllocType objectServiceDataType;
    private final NativeAllocType rawServiceDataHolderType;
    private final NativeAllocType rawServiceDataViewType;

    private final NativeAllocType typeInfoType;
    private final NativeAllocType reflectablePtrType;
    private final NativeAllocType reflectableConstPtrType;

    private final static NativeNumericWrapperType booleanType = new NativeNumericWrapperType("Bool", "bool");
    private final static NativeNumericWrapperType float16Type =
            new NativeNumericWrapperType("Float16", "float");
    private final static NativeNumericWrapperType float32Type =
            new NativeNumericWrapperType("Float32", "float");
    private final static NativeNumericWrapperType float64Type =
            new NativeNumericWrapperType("Float64", "double");

    private final static NativeIntegralType int1Type = new NativeIntegralType(1, true);
    private final static NativeIntegralType int2Type = new NativeIntegralType(2, true);
    private final static NativeIntegralType int3Type = new NativeIntegralType(3, true);
    private final static NativeIntegralType int4Type = new NativeIntegralType(4, true);
    private final static NativeIntegralType int5Type = new NativeIntegralType(5, true);
    private final static NativeIntegralType int6Type = new NativeIntegralType(6, true);
    private final static NativeIntegralType int7Type = new NativeIntegralType(7, true);
    private final static NativeIntegralType int8Type = new NativeIntegralType(8, true);
    private final static NativeIntegralType int9Type = new NativeIntegralType(9, true);
    private final static NativeIntegralType int10Type = new NativeIntegralType(10, true);
    private final static NativeIntegralType int11Type = new NativeIntegralType(11, true);
    private final static NativeIntegralType int12Type = new NativeIntegralType(12, true);
    private final static NativeIntegralType int13Type = new NativeIntegralType(13, true);
    private final static NativeIntegralType int14Type = new NativeIntegralType(14, true);
    private final static NativeIntegralType int15Type = new NativeIntegralType(15, true);
    private final static NativeIntegralType int16Type = new NativeIntegralType(16, true);
    private final static NativeIntegralType int17Type = new NativeIntegralType(17, true);
    private final static NativeIntegralType int18Type = new NativeIntegralType(18, true);
    private final static NativeIntegralType int19Type = new NativeIntegralType(19, true);
    private final static NativeIntegralType int20Type = new NativeIntegralType(20, true);
    private final static NativeIntegralType int21Type = new NativeIntegralType(21, true);
    private final static NativeIntegralType int22Type = new NativeIntegralType(22, true);
    private final static NativeIntegralType int23Type = new NativeIntegralType(23, true);
    private final static NativeIntegralType int24Type = new NativeIntegralType(24, true);
    private final static NativeIntegralType int25Type = new NativeIntegralType(25, true);
    private final static NativeIntegralType int26Type = new NativeIntegralType(26, true);
    private final static NativeIntegralType int27Type = new NativeIntegralType(27, true);
    private final static NativeIntegralType int28Type = new NativeIntegralType(28, true);
    private final static NativeIntegralType int29Type = new NativeIntegralType(29, true);
    private final static NativeIntegralType int30Type = new NativeIntegralType(30, true);
    private final static NativeIntegralType int31Type = new NativeIntegralType(31, true);
    private final static NativeIntegralType int32Type = new NativeIntegralType(32, true);
    private final static NativeIntegralType int33Type = new NativeIntegralType(33, true);
    private final static NativeIntegralType int34Type = new NativeIntegralType(34, true);
    private final static NativeIntegralType int35Type = new NativeIntegralType(35, true);
    private final static NativeIntegralType int36Type = new NativeIntegralType(36, true);
    private final static NativeIntegralType int37Type = new NativeIntegralType(37, true);
    private final static NativeIntegralType int38Type = new NativeIntegralType(38, true);
    private final static NativeIntegralType int39Type = new NativeIntegralType(39, true);
    private final static NativeIntegralType int40Type = new NativeIntegralType(40, true);
    private final static NativeIntegralType int41Type = new NativeIntegralType(41, true);
    private final static NativeIntegralType int42Type = new NativeIntegralType(42, true);
    private final static NativeIntegralType int43Type = new NativeIntegralType(43, true);
    private final static NativeIntegralType int44Type = new NativeIntegralType(44, true);
    private final static NativeIntegralType int45Type = new NativeIntegralType(45, true);
    private final static NativeIntegralType int46Type = new NativeIntegralType(46, true);
    private final static NativeIntegralType int47Type = new NativeIntegralType(47, true);
    private final static NativeIntegralType int48Type = new NativeIntegralType(48, true);
    private final static NativeIntegralType int49Type = new NativeIntegralType(49, true);
    private final static NativeIntegralType int50Type = new NativeIntegralType(50, true);
    private final static NativeIntegralType int51Type = new NativeIntegralType(51, true);
    private final static NativeIntegralType int52Type = new NativeIntegralType(52, true);
    private final static NativeIntegralType int53Type = new NativeIntegralType(53, true);
    private final static NativeIntegralType int54Type = new NativeIntegralType(54, true);
    private final static NativeIntegralType int55Type = new NativeIntegralType(55, true);
    private final static NativeIntegralType int56Type = new NativeIntegralType(56, true);
    private final static NativeIntegralType int57Type = new NativeIntegralType(57, true);
    private final static NativeIntegralType int58Type = new NativeIntegralType(58, true);
    private final static NativeIntegralType int59Type = new NativeIntegralType(59, true);
    private final static NativeIntegralType int60Type = new NativeIntegralType(60, true);
    private final static NativeIntegralType int61Type = new NativeIntegralType(61, true);
    private final static NativeIntegralType int62Type = new NativeIntegralType(62, true);
    private final static NativeIntegralType int63Type = new NativeIntegralType(63, true);
    private final static NativeIntegralType int64Type = new NativeIntegralType(64, true);

    private final static NativeIntegralType uint1Type = new NativeIntegralType(1, false);
    private final static NativeIntegralType uint2Type = new NativeIntegralType(2, false);
    private final static NativeIntegralType uint3Type = new NativeIntegralType(3, false);
    private final static NativeIntegralType uint4Type = new NativeIntegralType(4, false);
    private final static NativeIntegralType uint5Type = new NativeIntegralType(5, false);
    private final static NativeIntegralType uint6Type = new NativeIntegralType(6, false);
    private final static NativeIntegralType uint7Type = new NativeIntegralType(7, false);
    private final static NativeIntegralType uint8Type = new NativeIntegralType(8, false);
    private final static NativeIntegralType uint9Type = new NativeIntegralType(9, false);
    private final static NativeIntegralType uint10Type = new NativeIntegralType(10, false);
    private final static NativeIntegralType uint11Type = new NativeIntegralType(11, false);
    private final static NativeIntegralType uint12Type = new NativeIntegralType(12, false);
    private final static NativeIntegralType uint13Type = new NativeIntegralType(13, false);
    private final static NativeIntegralType uint14Type = new NativeIntegralType(14, false);
    private final static NativeIntegralType uint15Type = new NativeIntegralType(15, false);
    private final static NativeIntegralType uint16Type = new NativeIntegralType(16, false);
    private final static NativeIntegralType uint17Type = new NativeIntegralType(17, false);
    private final static NativeIntegralType uint18Type = new NativeIntegralType(18, false);
    private final static NativeIntegralType uint19Type = new NativeIntegralType(19, false);
    private final static NativeIntegralType uint20Type = new NativeIntegralType(20, false);
    private final static NativeIntegralType uint21Type = new NativeIntegralType(21, false);
    private final static NativeIntegralType uint22Type = new NativeIntegralType(22, false);
    private final static NativeIntegralType uint23Type = new NativeIntegralType(23, false);
    private final static NativeIntegralType uint24Type = new NativeIntegralType(24, false);
    private final static NativeIntegralType uint25Type = new NativeIntegralType(25, false);
    private final static NativeIntegralType uint26Type = new NativeIntegralType(26, false);
    private final static NativeIntegralType uint27Type = new NativeIntegralType(27, false);
    private final static NativeIntegralType uint28Type = new NativeIntegralType(28, false);
    private final static NativeIntegralType uint29Type = new NativeIntegralType(29, false);
    private final static NativeIntegralType uint30Type = new NativeIntegralType(30, false);
    private final static NativeIntegralType uint31Type = new NativeIntegralType(31, false);
    private final static NativeIntegralType uint32Type = new NativeIntegralType(32, false);
    private final static NativeIntegralType uint33Type = new NativeIntegralType(33, false);
    private final static NativeIntegralType uint34Type = new NativeIntegralType(34, false);
    private final static NativeIntegralType uint35Type = new NativeIntegralType(35, false);
    private final static NativeIntegralType uint36Type = new NativeIntegralType(36, false);
    private final static NativeIntegralType uint37Type = new NativeIntegralType(37, false);
    private final static NativeIntegralType uint38Type = new NativeIntegralType(38, false);
    private final static NativeIntegralType uint39Type = new NativeIntegralType(39, false);
    private final static NativeIntegralType uint40Type = new NativeIntegralType(40, false);
    private final static NativeIntegralType uint41Type = new NativeIntegralType(41, false);
    private final static NativeIntegralType uint42Type = new NativeIntegralType(42, false);
    private final static NativeIntegralType uint43Type = new NativeIntegralType(43, false);
    private final static NativeIntegralType uint44Type = new NativeIntegralType(44, false);
    private final static NativeIntegralType uint45Type = new NativeIntegralType(45, false);
    private final static NativeIntegralType uint46Type = new NativeIntegralType(46, false);
    private final static NativeIntegralType uint47Type = new NativeIntegralType(47, false);
    private final static NativeIntegralType uint48Type = new NativeIntegralType(48, false);
    private final static NativeIntegralType uint49Type = new NativeIntegralType(49, false);
    private final static NativeIntegralType uint50Type = new NativeIntegralType(50, false);
    private final static NativeIntegralType uint51Type = new NativeIntegralType(51, false);
    private final static NativeIntegralType uint52Type = new NativeIntegralType(52, false);
    private final static NativeIntegralType uint53Type = new NativeIntegralType(53, false);
    private final static NativeIntegralType uint54Type = new NativeIntegralType(54, false);
    private final static NativeIntegralType uint55Type = new NativeIntegralType(55, false);
    private final static NativeIntegralType uint56Type = new NativeIntegralType(56, false);
    private final static NativeIntegralType uint57Type = new NativeIntegralType(57, false);
    private final static NativeIntegralType uint58Type = new NativeIntegralType(58, false);
    private final static NativeIntegralType uint59Type = new NativeIntegralType(59, false);
    private final static NativeIntegralType uint60Type = new NativeIntegralType(60, false);
    private final static NativeIntegralType uint61Type = new NativeIntegralType(61, false);
    private final static NativeIntegralType uint62Type = new NativeIntegralType(62, false);
    private final static NativeIntegralType uint63Type = new NativeIntegralType(63, false);
    private final static NativeIntegralType uint64Type = new NativeIntegralType(64, false);

    private final static NativeDynamicBitFieldType dynInt8Type =
            new NativeDynamicBitFieldType("DynInt8", 8, true);
    private final static NativeDynamicBitFieldType dynInt16Type =
            new NativeDynamicBitFieldType("DynInt16", 16, true);
    private final static NativeDynamicBitFieldType dynInt32Type =
            new NativeDynamicBitFieldType("DynInt32", 32, true);
    private final static NativeDynamicBitFieldType dynInt64Type =
            new NativeDynamicBitFieldType("DynInt64", 64, true);

    private final static NativeDynamicBitFieldType dynUInt8Type =
            new NativeDynamicBitFieldType("DynUInt8", 8, false);
    private final static NativeDynamicBitFieldType dynUInt16Type =
            new NativeDynamicBitFieldType("DynUInt16", 16, false);
    private final static NativeDynamicBitFieldType dynUInt32Type =
            new NativeDynamicBitFieldType("DynUInt32", 32, false);
    private final static NativeDynamicBitFieldType dynUInt64Type =
            new NativeDynamicBitFieldType("DynUInt64", 64, false);

    private final static NativeIntegralType varInt16Type = new NativeIntegralType("VarInt16", 16, true);
    private final static NativeIntegralType varInt32Type = new NativeIntegralType("VarInt32", 32, true);
    private final static NativeIntegralType varInt64Type = new NativeIntegralType("VarInt64", 64, true);
    private final static NativeIntegralType varIntType = new NativeIntegralType("VarInt", 64, true);

    private final static NativeIntegralType varUInt16Type = new NativeIntegralType("VarUInt16", 16, false);
    private final static NativeIntegralType varUInt32Type = new NativeIntegralType("VarUInt32", 32, false);
    private final static NativeIntegralType varUInt64Type = new NativeIntegralType("VarUInt64", 64, false);
    private final static NativeIntegralType varUIntType = new NativeIntegralType("VarUInt", 64, false);

    private final static NativeIntegralType varSizeType = new NativeIntegralType("VarSize", 32, false);
}
