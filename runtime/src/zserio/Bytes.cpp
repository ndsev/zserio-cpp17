#include "zserio/Bytes.h"
#include "zserio/SizeConvertUtil.h"
#include "zserio/Types.h"

namespace zserio
{

namespace detail
{

void validate(BytesView, std::string_view)
{
    // always validate
}

BitSize bitSizeOf(BytesView bytesValue, BitSize)
{
    const VarSize bytesSize = fromCheckedValue<VarSize>(convertSizeToUInt32(bytesValue.size()));

    // the bytes consists of varsize for size followed by the bytes
    return bitSizeOf(bytesSize) + bytesSize * 8;
}

BitSize initializeOffsets(BytesView bytesValue, BitSize bitPosition)
{
    return bitSizeOf(bytesValue, bitPosition);
}

} // namespace detail

} // namespace zserio
