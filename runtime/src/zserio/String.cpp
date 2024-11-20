#include "zserio/SizeConvertUtil.h"
#include "zserio/String.h"
#include "zserio/Types.h"

namespace zserio
{

namespace detail
{

void validate(StringView, std::string_view)
{
    // always validate
}

BitSize bitSizeOf(StringView stringValue, BitSize)
{
    const VarSize stringSize = fromCheckedValue<VarSize>(convertSizeToUInt32(stringValue.size()));

    // the string consists of varsize for size followed by the UTF-8 encoded string
    return bitSizeOf(stringSize) + stringSize * 8;
}

} // namespace detail

} // namespace zserio
