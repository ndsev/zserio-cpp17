#ifndef ZSERIO_INTROSPECTABLE_H_INC
#define ZSERIO_INTROSPECTABLE_H_INC

#include "zserio/CppRuntimeException.h"
#include "zserio/IIntrospectable.h"
#include "zserio/TypeInfo.h"

namespace zserio
{

template <typename ALLOC>
class IntrospectableBase : public IBasicIntrospectable<ALLOC>
{
    /**
     * Constructor.
     *
     * \param typeInfo Type info of the reflected object.
     */
    explicit IntrospectableBase(const IBasicTypeInfo<ALLOC>& typeInfo);

    /** Destructor. */
    ~IntrospectableBase() override = 0;

    // TODO[Mi-L@]: Common inspectable interface (same in reflectable)
    const IBasicTypeInfo<ALLOC>& getTypeInfo() const override;
    bool isArray() const override;

    size_t initializeOffsets(size_t bitPosition) override;
    size_t initializeOffsets() override;
    size_t bitSizeOf(size_t bitPosition) const override;
    size_t bitSizeOf() const override;
    void write(BitStreamWriter& writer) const override;

    IBasicIntrospectableConstPtr<ALLOC> getField(std::string_view name) const override;
    IBasicIntrospectableConstPtr<ALLOC> getParameter(std::string_view name) const override;
    IBasicIntrospectableConstPtr<ALLOC> callFunction(std::string_view name) const override;

    std::string_view getChoice() const override; // TODO[Mi-L@]: Will be needed somehow also in reflectable?

    IBasicIntrospectableConstPtr<ALLOC> find(std::string_view path) const override;
    IBasicIntrospectableConstPtr<ALLOC> operator[](std::string_view path) const override;

private:
    const IBasicTypeInfo<ALLOC>& m_typeInfo;
};

// implementation of base classes methods

template <typename ALLOC>
IntrospectableBase<ALLOC>::IntrospectableBase(const IBasicTypeInfo<ALLOC>& typeInfo) :
        m_typeInfo(typeInfo)
{}

template <typename ALLOC>
IntrospectableBase<ALLOC>::~IntrospectableBase() = default;

template <typename ALLOC>
const IBasicTypeInfo<ALLOC>& IntrospectableBase<ALLOC>::getTypeInfo() const
{
    return m_typeInfo;
}

template <typename ALLOC>
bool IntrospectableBase<ALLOC>::isArray() const
{
    return false;
}

template <typename ALLOC>
size_t IntrospectableBase<ALLOC>::initializeOffsets(size_t)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not a compound type!";
}

template <typename ALLOC>
size_t IntrospectableBase<ALLOC>::initializeOffsets()
{
    return initializeOffsets(0);
}

template <typename ALLOC>
size_t IntrospectableBase<ALLOC>::bitSizeOf(size_t) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not implemented!";
}

template <typename ALLOC>
size_t IntrospectableBase<ALLOC>::bitSizeOf() const
{
    return bitSizeOf(0);
}

template <typename ALLOC>
void IntrospectableBase<ALLOC>::write(BitStreamWriter&) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not implemented!";
}

template <typename ALLOC>
IBasicIntrospectableConstPtr<ALLOC> IntrospectableBase<ALLOC>::getParameter(std::string_view) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' has no parameters to get!";
}

template <typename ALLOC>
IBasicIntrospectableConstPtr<ALLOC> IntrospectableBase<ALLOC>::callFunction(std::string_view) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' has no functions to call!";
}

template <typename ALLOC>
std::string_view IntrospectableBase<ALLOC>::getChoice() const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is neither choice nor union!";
}

template <typename ALLOC>
IBasicIntrospectableConstPtr<ALLOC> IntrospectableBase<ALLOC>::find(std::string_view path) const
{
    return detail::getFromObject(*this, path, 0);
}

template <typename ALLOC>
IBasicIntrospectableConstPtr<ALLOC> IntrospectableBase<ALLOC>::operator[](std::string_view path) const
{
    return find(path);
}

} // namespace zserio

#endif // ZSERIO_INTROSPECTABLE_H_INC
