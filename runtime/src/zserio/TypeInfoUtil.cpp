#include "zserio/TypeInfoUtil.h"

namespace zserio
{

bool TypeInfoUtil::isCompound(SchemaType schemaType)
{
    switch (schemaType)
    {
    case SchemaType::STRUCT:
    case SchemaType::CHOICE:
    case SchemaType::UNION:
        return true;
    default:
        return false;
    }
}

bool TypeInfoUtil::isCompound(CppType cppType)
{
    switch (cppType)
    {
    case CppType::STRUCT:
    case CppType::CHOICE:
    case CppType::UNION:
        return true;
    default:
        return false;
    }
}

bool TypeInfoUtil::hasChoice(SchemaType schemaType)
{
    switch (schemaType)
    {
    case SchemaType::CHOICE:
    case SchemaType::UNION:
        return true;
    default:
        return false;
    }
}

bool TypeInfoUtil::hasChoice(CppType cppType)
{
    switch (cppType)
    {
    case CppType::CHOICE:
    case CppType::UNION:
        return true;
    default:
        return false;
    }
}

bool TypeInfoUtil::isFixedSize(SchemaType schemaType)
{
    switch (schemaType)
    {
    case SchemaType::BOOL:
    case SchemaType::INT1:
    case SchemaType::INT2:
    case SchemaType::INT3:
    case SchemaType::INT4:
    case SchemaType::INT5:
    case SchemaType::INT6:
    case SchemaType::INT7:
    case SchemaType::INT8:
    case SchemaType::INT9:
    case SchemaType::INT10:
    case SchemaType::INT11:
    case SchemaType::INT12:
    case SchemaType::INT13:
    case SchemaType::INT14:
    case SchemaType::INT15:
    case SchemaType::INT16:
    case SchemaType::INT17:
    case SchemaType::INT18:
    case SchemaType::INT19:
    case SchemaType::INT20:
    case SchemaType::INT21:
    case SchemaType::INT22:
    case SchemaType::INT23:
    case SchemaType::INT24:
    case SchemaType::INT25:
    case SchemaType::INT26:
    case SchemaType::INT27:
    case SchemaType::INT28:
    case SchemaType::INT29:
    case SchemaType::INT30:
    case SchemaType::INT31:
    case SchemaType::INT32:
    case SchemaType::INT33:
    case SchemaType::INT34:
    case SchemaType::INT35:
    case SchemaType::INT36:
    case SchemaType::INT37:
    case SchemaType::INT38:
    case SchemaType::INT39:
    case SchemaType::INT40:
    case SchemaType::INT41:
    case SchemaType::INT42:
    case SchemaType::INT43:
    case SchemaType::INT44:
    case SchemaType::INT45:
    case SchemaType::INT46:
    case SchemaType::INT47:
    case SchemaType::INT48:
    case SchemaType::INT49:
    case SchemaType::INT50:
    case SchemaType::INT51:
    case SchemaType::INT52:
    case SchemaType::INT53:
    case SchemaType::INT54:
    case SchemaType::INT55:
    case SchemaType::INT56:
    case SchemaType::INT57:
    case SchemaType::INT58:
    case SchemaType::INT59:
    case SchemaType::INT60:
    case SchemaType::INT61:
    case SchemaType::INT62:
    case SchemaType::INT63:
    case SchemaType::INT64:
    case SchemaType::UINT1:
    case SchemaType::UINT2:
    case SchemaType::UINT3:
    case SchemaType::UINT4:
    case SchemaType::UINT5:
    case SchemaType::UINT6:
    case SchemaType::UINT7:
    case SchemaType::UINT8:
    case SchemaType::UINT9:
    case SchemaType::UINT10:
    case SchemaType::UINT11:
    case SchemaType::UINT12:
    case SchemaType::UINT13:
    case SchemaType::UINT14:
    case SchemaType::UINT15:
    case SchemaType::UINT16:
    case SchemaType::UINT17:
    case SchemaType::UINT18:
    case SchemaType::UINT19:
    case SchemaType::UINT20:
    case SchemaType::UINT21:
    case SchemaType::UINT22:
    case SchemaType::UINT23:
    case SchemaType::UINT24:
    case SchemaType::UINT25:
    case SchemaType::UINT26:
    case SchemaType::UINT27:
    case SchemaType::UINT28:
    case SchemaType::UINT29:
    case SchemaType::UINT30:
    case SchemaType::UINT31:
    case SchemaType::UINT32:
    case SchemaType::UINT33:
    case SchemaType::UINT34:
    case SchemaType::UINT35:
    case SchemaType::UINT36:
    case SchemaType::UINT37:
    case SchemaType::UINT38:
    case SchemaType::UINT39:
    case SchemaType::UINT40:
    case SchemaType::UINT41:
    case SchemaType::UINT42:
    case SchemaType::UINT43:
    case SchemaType::UINT44:
    case SchemaType::UINT45:
    case SchemaType::UINT46:
    case SchemaType::UINT47:
    case SchemaType::UINT48:
    case SchemaType::UINT49:
    case SchemaType::UINT50:
    case SchemaType::UINT51:
    case SchemaType::UINT52:
    case SchemaType::UINT53:
    case SchemaType::UINT54:
    case SchemaType::UINT55:
    case SchemaType::UINT56:
    case SchemaType::UINT57:
    case SchemaType::UINT58:
    case SchemaType::UINT59:
    case SchemaType::UINT60:
    case SchemaType::UINT61:
    case SchemaType::UINT62:
    case SchemaType::UINT63:
    case SchemaType::UINT64:
    case SchemaType::FLOAT16:
    case SchemaType::FLOAT32:
    case SchemaType::FLOAT64:
        return true;
    default:
        return false;
    }
}

bool TypeInfoUtil::isFixedSize(CppType cppType)
{
    switch (cppType)
    {
    case CppType::BOOL:
    case CppType::INT8:
    case CppType::INT16:
    case CppType::INT32:
    case CppType::INT64:
    case CppType::UINT8:
    case CppType::UINT16:
    case CppType::UINT32:
    case CppType::UINT64:
    case CppType::FLOAT:
    case CppType::DOUBLE:
        return true;
    default:
        return false;
    }
}

bool TypeInfoUtil::isIntegral(SchemaType schemaType)
{
    switch (schemaType)
    {
    case SchemaType::BOOL:
    case SchemaType::INT1:
    case SchemaType::INT2:
    case SchemaType::INT3:
    case SchemaType::INT4:
    case SchemaType::INT5:
    case SchemaType::INT6:
    case SchemaType::INT7:
    case SchemaType::INT8:
    case SchemaType::INT9:
    case SchemaType::INT10:
    case SchemaType::INT11:
    case SchemaType::INT12:
    case SchemaType::INT13:
    case SchemaType::INT14:
    case SchemaType::INT15:
    case SchemaType::INT16:
    case SchemaType::INT17:
    case SchemaType::INT18:
    case SchemaType::INT19:
    case SchemaType::INT20:
    case SchemaType::INT21:
    case SchemaType::INT22:
    case SchemaType::INT23:
    case SchemaType::INT24:
    case SchemaType::INT25:
    case SchemaType::INT26:
    case SchemaType::INT27:
    case SchemaType::INT28:
    case SchemaType::INT29:
    case SchemaType::INT30:
    case SchemaType::INT31:
    case SchemaType::INT32:
    case SchemaType::INT33:
    case SchemaType::INT34:
    case SchemaType::INT35:
    case SchemaType::INT36:
    case SchemaType::INT37:
    case SchemaType::INT38:
    case SchemaType::INT39:
    case SchemaType::INT40:
    case SchemaType::INT41:
    case SchemaType::INT42:
    case SchemaType::INT43:
    case SchemaType::INT44:
    case SchemaType::INT45:
    case SchemaType::INT46:
    case SchemaType::INT47:
    case SchemaType::INT48:
    case SchemaType::INT49:
    case SchemaType::INT50:
    case SchemaType::INT51:
    case SchemaType::INT52:
    case SchemaType::INT53:
    case SchemaType::INT54:
    case SchemaType::INT55:
    case SchemaType::INT56:
    case SchemaType::INT57:
    case SchemaType::INT58:
    case SchemaType::INT59:
    case SchemaType::INT60:
    case SchemaType::INT61:
    case SchemaType::INT62:
    case SchemaType::INT63:
    case SchemaType::INT64:
    case SchemaType::UINT1:
    case SchemaType::UINT2:
    case SchemaType::UINT3:
    case SchemaType::UINT4:
    case SchemaType::UINT5:
    case SchemaType::UINT6:
    case SchemaType::UINT7:
    case SchemaType::UINT8:
    case SchemaType::UINT9:
    case SchemaType::UINT10:
    case SchemaType::UINT11:
    case SchemaType::UINT12:
    case SchemaType::UINT13:
    case SchemaType::UINT14:
    case SchemaType::UINT15:
    case SchemaType::UINT16:
    case SchemaType::UINT17:
    case SchemaType::UINT18:
    case SchemaType::UINT19:
    case SchemaType::UINT20:
    case SchemaType::UINT21:
    case SchemaType::UINT22:
    case SchemaType::UINT23:
    case SchemaType::UINT24:
    case SchemaType::UINT25:
    case SchemaType::UINT26:
    case SchemaType::UINT27:
    case SchemaType::UINT28:
    case SchemaType::UINT29:
    case SchemaType::UINT30:
    case SchemaType::UINT31:
    case SchemaType::UINT32:
    case SchemaType::UINT33:
    case SchemaType::UINT34:
    case SchemaType::UINT35:
    case SchemaType::UINT36:
    case SchemaType::UINT37:
    case SchemaType::UINT38:
    case SchemaType::UINT39:
    case SchemaType::UINT40:
    case SchemaType::UINT41:
    case SchemaType::UINT42:
    case SchemaType::UINT43:
    case SchemaType::UINT44:
    case SchemaType::UINT45:
    case SchemaType::UINT46:
    case SchemaType::UINT47:
    case SchemaType::UINT48:
    case SchemaType::UINT49:
    case SchemaType::UINT50:
    case SchemaType::UINT51:
    case SchemaType::UINT52:
    case SchemaType::UINT53:
    case SchemaType::UINT54:
    case SchemaType::UINT55:
    case SchemaType::UINT56:
    case SchemaType::UINT57:
    case SchemaType::UINT58:
    case SchemaType::UINT59:
    case SchemaType::UINT60:
    case SchemaType::UINT61:
    case SchemaType::UINT62:
    case SchemaType::UINT63:
    case SchemaType::UINT64:
    case SchemaType::VARINT16:
    case SchemaType::VARINT32:
    case SchemaType::VARINT64:
    case SchemaType::VARINT:
    case SchemaType::VARUINT16:
    case SchemaType::VARUINT32:
    case SchemaType::VARUINT64:
    case SchemaType::VARUINT:
    case SchemaType::VARSIZE:
    case SchemaType::DYNAMIC_SIGNED_BITFIELD:
    case SchemaType::DYNAMIC_UNSIGNED_BITFIELD:
        return true;
    default:
        return false;
    }
}

bool TypeInfoUtil::isIntegral(CppType cppType)
{
    switch (cppType)
    {
    case CppType::BOOL:
    case CppType::INT8:
    case CppType::INT16:
    case CppType::INT32:
    case CppType::INT64:
    case CppType::UINT8:
    case CppType::UINT16:
    case CppType::UINT32:
    case CppType::UINT64:
        return true;
    default:
        return false;
    }
}

bool TypeInfoUtil::isSigned(SchemaType schemaType)
{
    switch (schemaType)
    {
    case SchemaType::INT1:
    case SchemaType::INT2:
    case SchemaType::INT3:
    case SchemaType::INT4:
    case SchemaType::INT5:
    case SchemaType::INT6:
    case SchemaType::INT7:
    case SchemaType::INT8:
    case SchemaType::INT9:
    case SchemaType::INT10:
    case SchemaType::INT11:
    case SchemaType::INT12:
    case SchemaType::INT13:
    case SchemaType::INT14:
    case SchemaType::INT15:
    case SchemaType::INT16:
    case SchemaType::INT17:
    case SchemaType::INT18:
    case SchemaType::INT19:
    case SchemaType::INT20:
    case SchemaType::INT21:
    case SchemaType::INT22:
    case SchemaType::INT23:
    case SchemaType::INT24:
    case SchemaType::INT25:
    case SchemaType::INT26:
    case SchemaType::INT27:
    case SchemaType::INT28:
    case SchemaType::INT29:
    case SchemaType::INT30:
    case SchemaType::INT31:
    case SchemaType::INT32:
    case SchemaType::INT33:
    case SchemaType::INT34:
    case SchemaType::INT35:
    case SchemaType::INT36:
    case SchemaType::INT37:
    case SchemaType::INT38:
    case SchemaType::INT39:
    case SchemaType::INT40:
    case SchemaType::INT41:
    case SchemaType::INT42:
    case SchemaType::INT43:
    case SchemaType::INT44:
    case SchemaType::INT45:
    case SchemaType::INT46:
    case SchemaType::INT47:
    case SchemaType::INT48:
    case SchemaType::INT49:
    case SchemaType::INT50:
    case SchemaType::INT51:
    case SchemaType::INT52:
    case SchemaType::INT53:
    case SchemaType::INT54:
    case SchemaType::INT55:
    case SchemaType::INT56:
    case SchemaType::INT57:
    case SchemaType::INT58:
    case SchemaType::INT59:
    case SchemaType::INT60:
    case SchemaType::INT61:
    case SchemaType::INT62:
    case SchemaType::INT63:
    case SchemaType::INT64:
    case SchemaType::VARINT16:
    case SchemaType::VARINT32:
    case SchemaType::VARINT64:
    case SchemaType::VARINT:
    case SchemaType::DYNAMIC_SIGNED_BITFIELD:
    case SchemaType::FLOAT16:
    case SchemaType::FLOAT32:
    case SchemaType::FLOAT64:
        return true;
    default:
        return false;
    }
}

bool TypeInfoUtil::isSigned(CppType cppType)
{
    switch (cppType)
    {
    case CppType::INT8:
    case CppType::INT16:
    case CppType::INT32:
    case CppType::INT64:
    case CppType::FLOAT:
    case CppType::DOUBLE:
        return true;
    default:
        return false;
    }
}

bool TypeInfoUtil::isFloatingPoint(SchemaType schemaType)
{
    switch (schemaType)
    {
    case SchemaType::FLOAT16:
    case SchemaType::FLOAT32:
    case SchemaType::FLOAT64:
        return true;
    default:
        return false;
    }
}

bool TypeInfoUtil::isFloatingPoint(CppType cppType)
{
    switch (cppType)
    {
    case CppType::FLOAT:
    case CppType::DOUBLE:
        return true;
    default:
        return false;
    }
}

} // namespace zserio
