#ifndef ZSERIO_VECTOR_H_INC
#define ZSERIO_VECTOR_H_INC

#include <vector>

namespace zserio
{

/**
 * Typedef to std::vector provided for convenience - using std::allocator.
 */
template <typename T, typename ALLOC = std::allocator<T>>
using Vector = std::vector<T, ALLOC>;

} // namespace zserio

#endif // ZSERIO_VECTOR_H_INC
