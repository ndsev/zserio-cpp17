#ifndef ZSERIO_CHOICE_TAG_H_INC
#define ZSERIO_CHOICE_TAG_H_INC

namespace zserio
{
namespace detail
{

/**
 * ChoiceTag provides enum Tag definition for Zserio unions and choices.
 *
 * This information is provided via specializations of the ChoiceTag strucure.
 */
template <typename T>
struct ChoiceTag;

} // namespace detail
} // namespace zserio

#endif // ZSERIO_CHOICE_TAG_H_INC
