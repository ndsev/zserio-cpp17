#include "zserio/SizeConvertUtil.h"
#include "zserio/String.h"
#include "zserio/Types.h"

namespace zserio
{

namespace detail
{

BitSize bitSizeOf(std::string_view stringValue, BitSize)
{
    const VarSize stringSize = fromCheckedValue<VarSize>(convertSizeToUInt32(stringValue.size()));

    // the string consists of varsize for size followed by the UTF-8 encoded string
    return bitSizeOf(stringSize) + stringSize * 8;
}

} // namespace detail

} // namespace zserio
