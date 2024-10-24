#include "zserio/Bytes.h"
#include "zserio/SizeConvertUtil.h"
#include "zserio/Types.h"

namespace zserio
{

namespace detail
{

BitSize bitSizeOf(Span<const uint8_t> bytesValue)
{
    const VarSize bytesSize = fromCheckedValue<VarSize>(convertSizeToUInt32(bytesValue.size()));

    // the bytes consists of varsize for size followed by the bytes
    return bitSizeOf(bytesSize) + bytesSize * 8;
}

} // namespace detail

} // namespace zserio
