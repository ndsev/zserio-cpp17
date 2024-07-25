#ifndef ZSERIO_WALKER_H_INC
#define ZSERIO_WALKER_H_INC

#include <algorithm>
#include <functional>
#include <regex>

#include "zserio/IReflectable.h"
#include "zserio/ITypeInfo.h"
#include "zserio/IWalkFilter.h"
#include "zserio/IWalkObserver.h"
#include "zserio/String.h"
#include "zserio/StringConvertUtil.h"
#include "zserio/TypeInfoUtil.h"
#include "zserio/Vector.h"
#include "zserio/WalkerConst.h"

namespace zserio
{

template <typename ALLOC>
class BasicDefaultWalkFilter;

/**
 * Walker through zserio objects, based on generated type info (see -withTypeInfoCode) and
 * reflectable interface (see -withReflectionCode).
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BasicWalker
{
public:
    /**
     * Constructor using default walk filter.
     *
     * \param walkObserver Observer to use during walking.
     */
    explicit BasicWalker(IBasicWalkObserver<ALLOC>& walkObserver);

    /**
     * Constructor.
     *
     * \param walkObserver Observer to use during walking.
     * \param walkFilter Walk filter to use.
     */
    BasicWalker(IBasicWalkObserver<ALLOC>& walkObserver, IBasicWalkFilter<ALLOC>& walkFilter);

    /**
     * Method generated by default.
     */
    ~BasicWalker() = default;

    /**
     * Copying and moving is disallowed!
     * \{
     */
    BasicWalker(const BasicWalker& other) = delete;
    BasicWalker& operator=(const BasicWalker& other) = delete;

    BasicWalker(BasicWalker&& other) = delete;
    BasicWalker& operator=(BasicWalker&& other) = delete;
    /**
     * \}
     */

    /**
     * Walks given reflectable zserio compound object.
     *
     * \param compound Zserio compound object to walk.
     */
    void walk(const IBasicReflectableConstPtr<ALLOC>& compound);

private:
    void walkFields(const IBasicReflectableConstPtr<ALLOC>& compound, const IBasicTypeInfo<ALLOC>& typeInfo);
    bool walkField(const IBasicReflectableConstPtr<ALLOC>& reflectable, const BasicFieldInfo<ALLOC>& fieldInfo);
    bool walkFieldValue(const IBasicReflectableConstPtr<ALLOC>& reflectable,
            const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex = WALKER_NOT_ELEMENT);

    IBasicWalkObserver<ALLOC>& m_walkObserver;
    BasicDefaultWalkFilter<ALLOC> m_defaultWalkFilter;
    IBasicWalkFilter<ALLOC>& m_walkFilter;
};

/**
 * Default walk observer which just does nothing.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BasicDefaultWalkObserver : public IBasicWalkObserver<ALLOC>
{
public:
    /**
     * Method generated by default.
     * \{
     */
    BasicDefaultWalkObserver() = default;
    ~BasicDefaultWalkObserver() override = default;
    /**
     * \}
     */

    /**
     * Copying and moving is disallowed!
     * \{
     */
    BasicDefaultWalkObserver(const BasicDefaultWalkObserver& other) = delete;
    BasicDefaultWalkObserver& operator=(const BasicDefaultWalkObserver& other) = delete;

    BasicDefaultWalkObserver(BasicDefaultWalkObserver&& other) = delete;
    BasicDefaultWalkObserver& operator=(BasicDefaultWalkObserver&& other) = delete;
    /**
     * \}
     */

    void beginRoot(const IBasicReflectableConstPtr<ALLOC>&) override
    {}
    void endRoot(const IBasicReflectableConstPtr<ALLOC>&) override
    {}

    void beginArray(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&) override
    {}
    void endArray(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&) override
    {}

    void beginCompound(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t) override
    {}
    void endCompound(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t) override
    {}

    void visitValue(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t) override
    {}
};

/**
 * Default walk filter which filters nothing.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BasicDefaultWalkFilter : public IBasicWalkFilter<ALLOC>
{
public:
    /**
     * Method generated by default.
     * \{
     */
    BasicDefaultWalkFilter() = default;
    ~BasicDefaultWalkFilter() override = default;
    /**
     * \}
     */

    /**
     * Copying and moving is disallowed!
     * \{
     */
    BasicDefaultWalkFilter(const BasicDefaultWalkFilter& other) = delete;
    BasicDefaultWalkFilter& operator=(const BasicDefaultWalkFilter& other) = delete;

    BasicDefaultWalkFilter(BasicDefaultWalkFilter&& other) = delete;
    BasicDefaultWalkFilter& operator=(BasicDefaultWalkFilter&& other) = delete;
    /**
     * \}
     */

    bool beforeArray(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&) override
    {
        return true;
    }

    bool afterArray(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&) override
    {
        return true;
    }

    bool beforeCompound(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t) override
    {
        return true;
    }

    bool afterCompound(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t) override
    {
        return true;
    }

    bool beforeValue(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t) override
    {
        return true;
    }

    bool afterValue(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t) override
    {
        return true;
    }
};

/**
 * Walk filter which allows to walk only to the given maximum depth.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BasicDepthWalkFilter : public IBasicWalkFilter<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param maxDepth Maximum depth to walk to.
     */
    explicit BasicDepthWalkFilter(size_t maxDepth);

    /**
     * Method generated by default.
     */
    ~BasicDepthWalkFilter() override = default;

    /**
     * Copying is disallowed!
     * \{
     */
    BasicDepthWalkFilter(const BasicDepthWalkFilter& other) = delete;
    BasicDepthWalkFilter& operator=(const BasicDepthWalkFilter& other) = delete;

    BasicDepthWalkFilter(BasicDepthWalkFilter&& other) = delete;
    BasicDepthWalkFilter& operator=(BasicDepthWalkFilter&& other) = delete;
    /**
     * \}
     */

    bool beforeArray(
            const IBasicReflectableConstPtr<ALLOC>& array, const BasicFieldInfo<ALLOC>& fieldInfo) override;
    bool afterArray(
            const IBasicReflectableConstPtr<ALLOC>& array, const BasicFieldInfo<ALLOC>& fieldInfo) override;

    bool beforeCompound(const IBasicReflectableConstPtr<ALLOC>& compound,
            const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex) override;
    bool afterCompound(const IBasicReflectableConstPtr<ALLOC>& compound, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;

    bool beforeValue(const IBasicReflectableConstPtr<ALLOC>& value, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;
    bool afterValue(const IBasicReflectableConstPtr<ALLOC>& value, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;

private:
    bool enterDepthLevel();
    bool leaveDepthLevel();

    size_t m_maxDepth;
    size_t m_depth;
};

/**
 * Walk filter which allows to walk only paths matching the given regex.
 *
 * The path is constructed from field names within the root object, thus the root object itself
 * is not part of the path.
 *
 * Array elements have the index appended to the path so that e.g. "compound.arrayField[0]" will match
 * only the first element in the array "arrayField".
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BasicRegexWalkFilter : public IBasicWalkFilter<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param pathRegex Path regex to use for filtering.
     */
    explicit BasicRegexWalkFilter(const char* pathRegex, const ALLOC& allocator = ALLOC());

    /**
     * Method generated by default.
     */
    ~BasicRegexWalkFilter() override = default;

    /**
     * Copying is disallowed!
     * \{
     */
    BasicRegexWalkFilter(const BasicRegexWalkFilter& other) = delete;
    BasicRegexWalkFilter& operator=(const BasicRegexWalkFilter& other) = delete;

    BasicRegexWalkFilter(BasicRegexWalkFilter&& other) = delete;
    BasicRegexWalkFilter& operator=(BasicRegexWalkFilter&& other) = delete;
    /**
     * \}
     */

    bool beforeArray(
            const IBasicReflectableConstPtr<ALLOC>& array, const BasicFieldInfo<ALLOC>& fieldInfo) override;
    bool afterArray(
            const IBasicReflectableConstPtr<ALLOC>& array, const BasicFieldInfo<ALLOC>& fieldInfo) override;

    bool beforeCompound(const IBasicReflectableConstPtr<ALLOC>& compound,
            const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex) override;
    bool afterCompound(const IBasicReflectableConstPtr<ALLOC>& compound, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;

    bool beforeValue(const IBasicReflectableConstPtr<ALLOC>& value, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;
    bool afterValue(const IBasicReflectableConstPtr<ALLOC>& value, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;

private:
    void appendPath(const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex);
    void popPath(const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex);
    string<ALLOC> getCurrentPath() const;
    bool matchSubtree(
            const IBasicReflectableConstPtr<ALLOC>& value, const BasicFieldInfo<ALLOC>& fieldInfo) const;

    vector<string<ALLOC>, ALLOC> m_currentPath;
    std::regex m_pathRegex;
    ALLOC m_allocator; // TODO[Mi-L@]: Check how std::regex_match allocates when results are omitted!
};

/**
 * Walk filter which allows to walk only to the given maximum array length.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BasicArrayLengthWalkFilter : public IBasicWalkFilter<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param maxArrayLength Maximum array length to walk to.
     */
    explicit BasicArrayLengthWalkFilter(size_t maxArrayLength);

    /**
     * Method generated by default.
     */
    ~BasicArrayLengthWalkFilter() override = default;

    /**
     * Copying and moving is disallowed!
     * \{
     */
    BasicArrayLengthWalkFilter(const BasicArrayLengthWalkFilter& other) = delete;
    BasicArrayLengthWalkFilter& operator=(const BasicArrayLengthWalkFilter& other) = delete;

    BasicArrayLengthWalkFilter(BasicArrayLengthWalkFilter&& other) = delete;
    BasicArrayLengthWalkFilter& operator=(BasicArrayLengthWalkFilter&& other) = delete;
    /**
     * \}
     */

    bool beforeArray(
            const IBasicReflectableConstPtr<ALLOC>& array, const BasicFieldInfo<ALLOC>& fieldInfo) override;
    bool afterArray(
            const IBasicReflectableConstPtr<ALLOC>& array, const BasicFieldInfo<ALLOC>& fieldInfo) override;

    bool beforeCompound(const IBasicReflectableConstPtr<ALLOC>& compound,
            const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex) override;
    bool afterCompound(const IBasicReflectableConstPtr<ALLOC>& compound, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;

    bool beforeValue(const IBasicReflectableConstPtr<ALLOC>& value, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;
    bool afterValue(const IBasicReflectableConstPtr<ALLOC>& value, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;

private:
    bool filterArrayElement(size_t elementIndex);

    size_t m_maxArrayLength;
};

/**
 * Walk filter which implements composition of particular filters.
 *
 * The filters are called sequentially and logical and is applied on theirs results.
 * Note that all filters are always called.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BasicAndWalkFilter : public IBasicWalkFilter<ALLOC>
{
public:
    using WalkFilterRef = std::reference_wrapper<IBasicWalkFilter<ALLOC>>;
    using WalkFilters = vector<WalkFilterRef, ALLOC>;

    /**
     * Constructor.
     *
     * \param walkFilters List of filters to use in composition.
     */
    explicit BasicAndWalkFilter(const WalkFilters& walkFilters);

    /**
     * Method generated by default.
     */
    ~BasicAndWalkFilter() override = default;

    /**
     * Copying and moving is disallowed!
     * \{
     */
    BasicAndWalkFilter(const BasicAndWalkFilter& other) = delete;
    BasicAndWalkFilter& operator=(const BasicAndWalkFilter& other) = delete;

    BasicAndWalkFilter(BasicAndWalkFilter&& other) = delete;
    BasicAndWalkFilter& operator=(BasicAndWalkFilter&& other) = delete;
    /**
     * \}
     */

    bool beforeArray(
            const IBasicReflectableConstPtr<ALLOC>& array, const BasicFieldInfo<ALLOC>& fieldInfo) override;
    bool afterArray(
            const IBasicReflectableConstPtr<ALLOC>& array, const BasicFieldInfo<ALLOC>& fieldInfo) override;

    bool beforeCompound(const IBasicReflectableConstPtr<ALLOC>& compound,
            const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex) override;
    bool afterCompound(const IBasicReflectableConstPtr<ALLOC>& compound, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;

    bool beforeValue(const IBasicReflectableConstPtr<ALLOC>& value, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;
    bool afterValue(const IBasicReflectableConstPtr<ALLOC>& value, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override;

private:
    template <typename FILTER_FUNC, typename... ARGS>
    bool applyFilters(FILTER_FUNC filterFunc, ARGS... args)
    {
        bool result = true;
        for (IBasicWalkFilter<ALLOC>& walkFilter : m_walkFilters)
            result &= (walkFilter.*filterFunc)(args...);
        return result;
    }

    WalkFilters m_walkFilters;
};

/** Typedefs to walker related classes provided for convenience - using default std::allocator<uint8_t>. */
/** \{ */
using Walker = BasicWalker<>;
using DefaultWalkObserver = BasicDefaultWalkObserver<>;
using DefaultWalkFilter = BasicDefaultWalkFilter<>;
using DepthWalkFilter = BasicDepthWalkFilter<>;
using RegexWalkFilter = BasicRegexWalkFilter<>;
using ArrayLengthWalkFilter = BasicArrayLengthWalkFilter<>;
using AndWalkFilter = BasicAndWalkFilter<>;
/** \} */

template <typename ALLOC>
BasicWalker<ALLOC>::BasicWalker(IBasicWalkObserver<ALLOC>& walkObserver) :
        m_walkObserver(walkObserver),
        m_walkFilter(m_defaultWalkFilter)
{}

template <typename ALLOC>
BasicWalker<ALLOC>::BasicWalker(IBasicWalkObserver<ALLOC>& walkObserver, IBasicWalkFilter<ALLOC>& walkFilter) :
        m_walkObserver(walkObserver),
        m_walkFilter(walkFilter)
{}

template <typename ALLOC>
void BasicWalker<ALLOC>::walk(const IBasicReflectableConstPtr<ALLOC>& compound)
{
    if (!compound)
        throw CppRuntimeException("Walker: Root object cannot be NULL!");

    const IBasicTypeInfo<ALLOC>& typeInfo = compound->getTypeInfo();
    if (!TypeInfoUtil::isCompound(typeInfo.getSchemaType()))
    {
        throw CppRuntimeException("Walker: Root object '")
                << typeInfo.getSchemaName() << "' is not a compound type!";
    }

    m_walkObserver.beginRoot(compound);
    walkFields(compound, typeInfo);
    m_walkObserver.endRoot(compound);
}

template <typename ALLOC>
void BasicWalker<ALLOC>::walkFields(
        const IBasicReflectableConstPtr<ALLOC>& compound, const IBasicTypeInfo<ALLOC>& typeInfo)
{
    if (TypeInfoUtil::hasChoice(typeInfo.getSchemaType()))
    {
        StringView compoundChoice = compound->getChoice();
        if (!compoundChoice.empty())
        {
            Span<const BasicFieldInfo<ALLOC>> fields = typeInfo.getFields();
            auto fieldsIt = std::find_if(
                    fields.begin(), fields.end(), [compoundChoice](const BasicFieldInfo<ALLOC>& fieldInfo) {
                        return fieldInfo.schemaName == compoundChoice;
                    });
            if (fieldsIt != fields.end())
            {
                walkField(compound->getField(compoundChoice), *fieldsIt);
            }
        }
        // else uninitialized or empty branch
    }
    else
    {
        for (const BasicFieldInfo<ALLOC>& fieldInfo : typeInfo.getFields())
        {
            if (!walkField(compound->getField(fieldInfo.schemaName), fieldInfo))
                break;
        }
    }
}

template <typename ALLOC>
bool BasicWalker<ALLOC>::walkField(
        const IBasicReflectableConstPtr<ALLOC>& reflectable, const BasicFieldInfo<ALLOC>& fieldInfo)
{
    if (reflectable && fieldInfo.isArray)
    {
        if (m_walkFilter.beforeArray(reflectable, fieldInfo))
        {
            m_walkObserver.beginArray(reflectable, fieldInfo);
            for (size_t i = 0; i < reflectable->size(); ++i)
            {
                if (!walkFieldValue(reflectable->at(i), fieldInfo, i))
                    break;
            }
            m_walkObserver.endArray(reflectable, fieldInfo);
        }
        return m_walkFilter.afterArray(reflectable, fieldInfo);
    }
    else
    {
        return walkFieldValue(reflectable, fieldInfo);
    }
}

template <typename ALLOC>
bool BasicWalker<ALLOC>::walkFieldValue(const IBasicReflectableConstPtr<ALLOC>& reflectable,
        const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
{
    const IBasicTypeInfo<ALLOC>& typeInfo = fieldInfo.typeInfo;
    if (reflectable && TypeInfoUtil::isCompound(typeInfo.getSchemaType()))
    {
        if (m_walkFilter.beforeCompound(reflectable, fieldInfo, elementIndex))
        {
            m_walkObserver.beginCompound(reflectable, fieldInfo, elementIndex);
            walkFields(reflectable, typeInfo);
            m_walkObserver.endCompound(reflectable, fieldInfo, elementIndex);
        }
        return m_walkFilter.afterCompound(reflectable, fieldInfo, elementIndex);
    }
    else
    {
        if (m_walkFilter.beforeValue(reflectable, fieldInfo, elementIndex))
            m_walkObserver.visitValue(reflectable, fieldInfo, elementIndex);
        return m_walkFilter.afterValue(reflectable, fieldInfo, elementIndex);
    }
}

template <typename ALLOC>
BasicDepthWalkFilter<ALLOC>::BasicDepthWalkFilter(size_t maxDepth) :
        m_maxDepth(maxDepth),
        m_depth(1)
{}

template <typename ALLOC>
bool BasicDepthWalkFilter<ALLOC>::beforeArray(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&)
{
    return enterDepthLevel();
}

template <typename ALLOC>
bool BasicDepthWalkFilter<ALLOC>::afterArray(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&)
{
    return leaveDepthLevel();
}

template <typename ALLOC>
bool BasicDepthWalkFilter<ALLOC>::beforeCompound(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t)
{
    return enterDepthLevel();
}

template <typename ALLOC>
bool BasicDepthWalkFilter<ALLOC>::afterCompound(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t)
{
    return leaveDepthLevel();
}

template <typename ALLOC>
bool BasicDepthWalkFilter<ALLOC>::beforeValue(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t)
{
    return m_depth <= m_maxDepth;
}

template <typename ALLOC>
bool BasicDepthWalkFilter<ALLOC>::afterValue(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t)
{
    return true;
}

template <typename ALLOC>
bool BasicDepthWalkFilter<ALLOC>::enterDepthLevel()
{
    const bool enter = (m_depth <= m_maxDepth);
    m_depth += 1;
    return enter;
}

template <typename ALLOC>
bool BasicDepthWalkFilter<ALLOC>::leaveDepthLevel()
{
    m_depth -= 1;
    return true;
}

namespace detail
{

template <typename ALLOC>
string<ALLOC> getCurrentPathImpl(const vector<string<ALLOC>, ALLOC>& currentPath, const ALLOC& allocator)
{
    string<ALLOC> currentPathStr(allocator);
    for (auto it = currentPath.begin(); it != currentPath.end(); ++it)
    {
        if (!currentPathStr.empty())
            currentPathStr += ".";
        currentPathStr += *it;
    }
    return currentPathStr;
}

template <typename ALLOC>
void appendPathImpl(vector<string<ALLOC>, ALLOC>& currentPath, const BasicFieldInfo<ALLOC>& fieldInfo,
        size_t elementIndex, const ALLOC& allocator)
{
    if (elementIndex == WALKER_NOT_ELEMENT)
    {
        currentPath.emplace_back(fieldInfo.schemaName.data(), fieldInfo.schemaName.size(), allocator);
    }
    else
    {
        currentPath.back() =
                toString(fieldInfo.schemaName, allocator) + "[" + toString(elementIndex, allocator) + "]";
    }
}

template <typename ALLOC>
void popPathImpl(vector<string<ALLOC>, ALLOC>& currentPath, const BasicFieldInfo<ALLOC>& fieldInfo,
        size_t elementIndex, const ALLOC& allocator)
{
    if (elementIndex == WALKER_NOT_ELEMENT)
        currentPath.pop_back();
    else
        currentPath.back() = toString(fieldInfo.schemaName, allocator);
}

template <typename ALLOC>
class SubtreeRegexWalkFilter : public IBasicWalkFilter<ALLOC>
{
public:
    SubtreeRegexWalkFilter(const vector<string<ALLOC>, ALLOC>& currentPath, const std::regex& pathRegex,
            const ALLOC& allocator) :
            m_currentPath(currentPath),
            m_pathRegex(pathRegex),
            m_allocator(allocator)
    {}

    bool matches()
    {
        return m_matches;
    }

    bool beforeArray(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>& fieldInfo) override
    {
        m_currentPath.emplace_back(fieldInfo.schemaName.data(), fieldInfo.schemaName.size());
        m_matches = std::regex_match(getCurrentPath(), m_pathRegex);

        // terminate when the match is already found (note that array is never null here)
        return !m_matches;
    }

    bool afterArray(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&) override
    {
        m_currentPath.pop_back();
        return !m_matches; // terminate when the match is already found
    }

    bool beforeCompound(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override
    {
        appendPath(fieldInfo, elementIndex);
        m_matches = std::regex_match(getCurrentPath(), m_pathRegex);

        // terminate when the match is already found (note that compound is never null here)
        return !m_matches;
    }

    bool afterCompound(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override
    {
        popPath(fieldInfo, elementIndex);
        return !m_matches; // terminate when the match is already found
    }

    bool beforeValue(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override
    {
        appendPath(fieldInfo, elementIndex);
        m_matches = std::regex_match(getCurrentPath(), m_pathRegex);

        return !m_matches; // terminate when the match is already found
    }

    bool afterValue(const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>& fieldInfo,
            size_t elementIndex) override
    {
        popPath(fieldInfo, elementIndex);
        return !m_matches; // terminate when the match is already found
    }

private:
    string<ALLOC> getCurrentPath() const
    {
        return detail::getCurrentPathImpl(m_currentPath, m_allocator);
    }

    void appendPath(const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
    {
        detail::appendPathImpl(m_currentPath, fieldInfo, elementIndex, m_allocator);
    }

    void popPath(const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
    {
        detail::popPathImpl(m_currentPath, fieldInfo, elementIndex, m_allocator);
    }

    vector<string<ALLOC>, ALLOC> m_currentPath;
    std::regex m_pathRegex;
    ALLOC m_allocator;
    bool m_matches = false;
};

} // namespace detail

template <typename ALLOC>
BasicRegexWalkFilter<ALLOC>::BasicRegexWalkFilter(const char* pathRegex, const ALLOC& allocator) :
        m_pathRegex(pathRegex),
        m_allocator(allocator)
{}

template <typename ALLOC>
bool BasicRegexWalkFilter<ALLOC>::beforeArray(
        const IBasicReflectableConstPtr<ALLOC>& array, const BasicFieldInfo<ALLOC>& fieldInfo)
{
    m_currentPath.emplace_back(fieldInfo.schemaName.data(), fieldInfo.schemaName.size(), m_allocator);

    if (std::regex_match(getCurrentPath(), m_pathRegex))
        return true; // the array itself matches

    for (size_t i = 0; i < array->size(); ++i)
    {
        m_currentPath.back() =
                toString(fieldInfo.schemaName, m_allocator) + "[" + toString(i, m_allocator) + "]";

        if (matchSubtree(array->at(i), fieldInfo))
            return true;
    }

    m_currentPath.back() = toString(fieldInfo.schemaName, m_allocator);

    return false;
}

template <typename ALLOC>
bool BasicRegexWalkFilter<ALLOC>::afterArray(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&)
{
    m_currentPath.pop_back();
    return true;
}

template <typename ALLOC>
bool BasicRegexWalkFilter<ALLOC>::beforeCompound(const IBasicReflectableConstPtr<ALLOC>& compound,
        const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
{
    appendPath(fieldInfo, elementIndex);
    if (std::regex_match(getCurrentPath(), m_pathRegex))
        return true; // the compound itself matches

    return matchSubtree(compound, fieldInfo);
}

template <typename ALLOC>
bool BasicRegexWalkFilter<ALLOC>::afterCompound(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
{
    popPath(fieldInfo, elementIndex);
    return true;
}

template <typename ALLOC>
bool BasicRegexWalkFilter<ALLOC>::beforeValue(const IBasicReflectableConstPtr<ALLOC>& value,
        const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
{
    appendPath(fieldInfo, elementIndex);
    return matchSubtree(value, fieldInfo);
}

template <typename ALLOC>
bool BasicRegexWalkFilter<ALLOC>::afterValue(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
{
    popPath(fieldInfo, elementIndex);
    return true;
}

template <typename ALLOC>
void BasicRegexWalkFilter<ALLOC>::appendPath(const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
{
    detail::appendPathImpl(m_currentPath, fieldInfo, elementIndex, m_allocator);
}

template <typename ALLOC>
void BasicRegexWalkFilter<ALLOC>::popPath(const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
{
    detail::popPathImpl(m_currentPath, fieldInfo, elementIndex, m_allocator);
}

template <typename ALLOC>
string<ALLOC> BasicRegexWalkFilter<ALLOC>::getCurrentPath() const
{
    return detail::getCurrentPathImpl(m_currentPath, m_allocator);
}

template <typename ALLOC>
bool BasicRegexWalkFilter<ALLOC>::matchSubtree(
        const IBasicReflectableConstPtr<ALLOC>& value, const BasicFieldInfo<ALLOC>& fieldInfo) const
{
    if (value != nullptr && TypeInfoUtil::isCompound(fieldInfo.typeInfo.getSchemaType()))
    {
        // is a not null compound, try to find match within its subtree
        BasicDefaultWalkObserver<ALLOC> defaultObserver;
        detail::SubtreeRegexWalkFilter<ALLOC> subtreeFilter(m_currentPath, m_pathRegex, m_allocator);
        BasicWalker<ALLOC> walker(defaultObserver, subtreeFilter);
        walker.walk(value);
        return subtreeFilter.matches();
    }
    else
    {
        // try to match a simple value or null compound
        return std::regex_match(getCurrentPath(), m_pathRegex);
    }
}

template <typename ALLOC>
BasicArrayLengthWalkFilter<ALLOC>::BasicArrayLengthWalkFilter(size_t maxArrayLength) :
        m_maxArrayLength(maxArrayLength)
{}

template <typename ALLOC>
bool BasicArrayLengthWalkFilter<ALLOC>::beforeArray(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&)
{
    return true;
}

template <typename ALLOC>
bool BasicArrayLengthWalkFilter<ALLOC>::afterArray(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&)
{
    return true;
}

template <typename ALLOC>
bool BasicArrayLengthWalkFilter<ALLOC>::beforeCompound(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t elementIndex)
{
    return filterArrayElement(elementIndex);
}

template <typename ALLOC>
bool BasicArrayLengthWalkFilter<ALLOC>::afterCompound(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t elementIndex)
{
    return filterArrayElement(elementIndex);
}

template <typename ALLOC>
bool BasicArrayLengthWalkFilter<ALLOC>::beforeValue(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t elementIndex)
{
    return filterArrayElement(elementIndex);
}

template <typename ALLOC>
bool BasicArrayLengthWalkFilter<ALLOC>::afterValue(
        const IBasicReflectableConstPtr<ALLOC>&, const BasicFieldInfo<ALLOC>&, size_t elementIndex)
{
    return filterArrayElement(elementIndex);
}

template <typename ALLOC>
bool BasicArrayLengthWalkFilter<ALLOC>::filterArrayElement(size_t elementIndex)
{
    return elementIndex == WALKER_NOT_ELEMENT ? true : elementIndex < m_maxArrayLength;
}

template <typename ALLOC>
BasicAndWalkFilter<ALLOC>::BasicAndWalkFilter(const WalkFilters& walkFilters) :
        m_walkFilters(walkFilters)
{}

template <typename ALLOC>
bool BasicAndWalkFilter<ALLOC>::beforeArray(
        const IBasicReflectableConstPtr<ALLOC>& array, const BasicFieldInfo<ALLOC>& fieldInfo)
{
    return applyFilters(&IBasicWalkFilter<ALLOC>::beforeArray, array, fieldInfo);
}

template <typename ALLOC>
bool BasicAndWalkFilter<ALLOC>::afterArray(
        const IBasicReflectableConstPtr<ALLOC>& array, const BasicFieldInfo<ALLOC>& fieldInfo)
{
    return applyFilters(&IBasicWalkFilter<ALLOC>::afterArray, array, fieldInfo);
}

template <typename ALLOC>
bool BasicAndWalkFilter<ALLOC>::beforeCompound(const IBasicReflectableConstPtr<ALLOC>& compound,
        const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
{
    return applyFilters(&IBasicWalkFilter<ALLOC>::beforeCompound, compound, fieldInfo, elementIndex);
}

template <typename ALLOC>
bool BasicAndWalkFilter<ALLOC>::afterCompound(const IBasicReflectableConstPtr<ALLOC>& compound,
        const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
{
    return applyFilters(&IBasicWalkFilter<ALLOC>::afterCompound, compound, fieldInfo, elementIndex);
}

template <typename ALLOC>
bool BasicAndWalkFilter<ALLOC>::beforeValue(const IBasicReflectableConstPtr<ALLOC>& value,
        const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
{
    return applyFilters(&IBasicWalkFilter<ALLOC>::beforeValue, value, fieldInfo, elementIndex);
}

template <typename ALLOC>
bool BasicAndWalkFilter<ALLOC>::afterValue(const IBasicReflectableConstPtr<ALLOC>& value,
        const BasicFieldInfo<ALLOC>& fieldInfo, size_t elementIndex)
{
    return applyFilters(&IBasicWalkFilter<ALLOC>::afterValue, value, fieldInfo, elementIndex);
}

} // namespace zserio

#endif // ZSERIO_WALKER_H_INC
