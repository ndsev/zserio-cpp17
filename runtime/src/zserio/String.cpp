#include "zserio/SizeConvertUtil.h"
#include "zserio/String.h"
#include "zserio/Types.h"

namespace zserio
{

namespace detail
{

void validate(std::string_view, std::string_view)
{
    // always validate
}

BitSize bitSizeOf(std::string_view stringValue, BitSize)
{
    const VarSize stringSize = fromCheckedValue<VarSize>(convertSizeToUInt32(stringValue.size()));

    // the string consists of varsize for size followed by the UTF-8 encoded string
    return bitSizeOf(stringSize) + stringSize * 8;
}

BitSize initializeOffsets(std::string_view stringValue, BitSize bitPosition)
{
    return bitSizeOf(stringValue, bitPosition);
}

} // namespace detail

} // namespace zserio
