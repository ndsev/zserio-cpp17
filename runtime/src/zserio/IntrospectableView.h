#ifndef ZSERIO_INTROSPECTABLE_VIEW_H_INC
#define ZSERIO_INTROSPECTABLE_VIEW_H_INC

#include "zserio/IIntrospectableView.h"

#include "IntrospectableDataBase.h"

namespace zserio
{

/**
 * Base class for all introspectable view implementations.
 */
template <typename ALLOC>
class IntrospectableViewBase : public IntrospectableDataBase<IBasicIntrospectableView<ALLOC>, ALLOC>
{
public:
    /** Shared pointer to the constant introspectable interface. */
    using ConstPtr = IBasicIntrospectableView<ALLOC>::ConstPtr;

    /**
     * Constructor.
     *
     * \param typeInfo Type info of the object.
     */
    explicit IntrospectableViewBase(const IBasicTypeInfo<ALLOC>& typeInfo);

    /** Destructor. */
    ~IntrospectableViewBase() override = 0;

    /**
     * Copying and moving is disallowed!
     * \{
     */
    IntrospectableViewBase(const IntrospectableViewBase&) = delete;
    IntrospectableViewBase& operator=(const IntrospectableViewBase&) = delete;

    IntrospectableViewBase(const IntrospectableViewBase&&) = delete;
    IntrospectableViewBase& operator=(const IntrospectableViewBase&&) = delete;
    /**
     * \}
     */

    ConstPtr getParameter(std::string_view name) const override;
    ConstPtr callFunction(std::string_view name) const override;

    size_t initializeOffsets(size_t bitPosition) override;
    size_t initializeOffsets() override;
    size_t bitSizeOf(size_t bitPosition) const override;
    size_t bitSizeOf() const override;
    void write(BitStreamWriter& writer) const override;

private:
    ConstPtr getFieldFromObject(const IBasicIntrospectableView<ALLOC>& object, std::string_view name) const;
    ConstPtr getParameterFromObject(const IBasicIntrospectableView<ALLOC>& object, std::string_view name) const;
    ConstPtr callFunctionInObject(const IBasicIntrospectableView<ALLOC>& object, std::string_view name) const;
    ConstPtr getFromObject(
            const IBasicIntrospectableView<ALLOC>& object, std::string_view path, size_t pos) const;
};

// implementation of base classes methods

template <typename ALLOC>
IntrospectableViewBase<ALLOC>::IntrospectableViewBase(const IBasicTypeInfo<ALLOC>& typeInfo) :
        IntrospectableDataBase<IBasicIntrospectableView, ALLOC>(typeInfo)
{}

template <typename ALLOC>
IntrospectableViewBase<ALLOC>::~IntrospectableViewBase() = default;

template <typename ALLOC>
IntrospectableViewBase<ALLOC>::ConstPtr IntrospectableViewBase<ALLOC>::find(std::string_view path) const
{
    return detail::getFromObject(*this, path, 0);
}

template <typename ALLOC>
IntrospectableViewBase<ALLOC>::ConstPtr IntrospectableViewBase<ALLOC>::getParameter(std::string_view) const
{
    throw CppRuntimeException("Type '")
            << IntrospectableViewBase<ALLOC>::getTypeInfo().getSchemaName() << "' has no parameters to get!";
}

template <typename ALLOC>
IntrospectableViewBase<ALLOC>::ConstPtr IntrospectableViewBase<ALLOC>::callFunction(std::string_view) const
{
    throw CppRuntimeException("Type '")
            << IntrospectableViewBase<ALLOC>::getTypeInfo().getSchemaName() << "' has no functions to call!";
}

template <typename ALLOC>
size_t IntrospectableViewBase<ALLOC>::initializeOffsets(size_t)
{
    throw CppRuntimeException("Type '")
            << IntrospectableViewBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not a compound type!";
}

template <typename ALLOC>
size_t IntrospectableViewBase<ALLOC>::initializeOffsets()
{
    return initializeOffsets(0);
}

template <typename ALLOC>
size_t IntrospectableViewBase<ALLOC>::bitSizeOf(size_t) const
{
    throw CppRuntimeException("Type '")
            << IntrospectableViewBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not implemented!";
}

template <typename ALLOC>
size_t IntrospectableViewBase<ALLOC>::bitSizeOf() const
{
    return bitSizeOf(0);
}

template <typename ALLOC>
void IntrospectableViewBase<ALLOC>::write(BitStreamWriter&) const
{
    throw CppRuntimeException("Type '")
            << IntrospectableViewBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not implemented!";
}

template <typename ALLOC>
IntrospectableViewBase<ALLOC>::ConstPtr IntrospectableViewBase<ALLOC>::getFieldFromObject(
        const IBasicIntrospectableView<ALLOC>& object, std::string_view name)
{
    const auto& typeInfo = object.getTypeInfo();
    if (TypeInfoUtil::isCompound(typeInfo.getSchemaType()))
    {
        const auto& fields = typeInfo.getFields();
        auto fieldsIt =
                std::find_if(fields.begin(), fields.end(), [name](const BasicFieldInfo<ALLOC>& fieldInfo) {
                    return fieldInfo.schemaName == name;
                });
        if (fieldsIt != fields.end())
        {
            return object.getField(name);
        }
    }

    return nullptr;
}

template <typename ALLOC>
IntrospectableViewBase<ALLOC>::ConstPtr IntrospectableViewBase<ALLOC>::getParameterFromObject(
        const IBasicIntrospectableView<ALLOC>& object, std::string_view name)
{
    const auto& typeInfo = object.getTypeInfo();
    if (TypeInfoUtil::isCompound(typeInfo.getSchemaType()))
    {
        const auto& parameters = typeInfo.getParameters();
        auto parametersIt = std::find_if(
                parameters.begin(), parameters.end(), [name](const BasicParameterInfo<ALLOC>& parameterInfo) {
                    return parameterInfo.schemaName == name;
                });
        if (parametersIt != parameters.end())
        {
            return object.getParameter(name);
        }
    }

    return nullptr;
}

template <typename ALLOC>
IntrospectableViewBase<ALLOC>::ConstPtr IntrospectableViewBase<ALLOC>::callFunctionInObject(
        const IBasicIntrospectableView<ALLOC>& object, std::string_view name)
{
    const auto& typeInfo = object.getTypeInfo();
    if (TypeInfoUtil::isCompound(typeInfo.getSchemaType()))
    {
        const auto& functions = typeInfo.getFunctions();
        auto functionsIt = std::find_if(
                functions.begin(), functions.end(), [name](const BasicFunctionInfo<ALLOC>& functionInfo) {
                    return functionInfo.schemaName == name;
                });
        if (functionsIt != functions.end())
        {
            return object.callFunction(name);
        }
    }

    return nullptr;
}

template <typename ALLOC>
IntrospectableViewBase<ALLOC>::ConstPtr IntrospectableViewBase<ALLOC>::getFromObject(
        const IBasicIntrospectableView<ALLOC>& object, std::string_view path, size_t pos)
{
    try
    {
        const size_t dotPos = path.find('.', pos);
        const bool isLast = dotPos == std::string_view::npos;
        const std::string_view name =
                path.substr(pos, dotPos == std::string_view::npos ? std::string_view::npos : dotPos - pos);

        auto field = getFieldFromObject(object, name);
        if (field)
        {
            return isLast ? std::move(field) : getFromObject(*field, path, dotPos + 1);
        }

        auto parameter = getParameterFromObject(object, name);
        if (parameter)
        {
            return isLast ? std::move(parameter) : getFromObject(*parameter, path, dotPos + 1);
        }

        auto functionResult = callFunctionInObject(object, name);
        if (functionResult)
        {
            return isLast ? std::move(functionResult) : getFromObject(*functionResult, path, dotPos + 1);
        }
    }
    catch (const CppRuntimeException&)
    {}

    return nullptr;
}

} // namespace zserio

#endif // ZSERIO_INTROSPECTABLE_VIEW_H_INC
