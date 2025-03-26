#include "gtest/gtest.h"
#include "test_object/std_allocator/WalkerBitmask.h"
#include "test_object/std_allocator/WalkerChoice.h"
#include "test_object/std_allocator/WalkerNested.h"
#include "test_object/std_allocator/WalkerObject.h"
#include "test_object/std_allocator/WalkerUnion.h"
#include "zserio/Array.h"
#include "zserio/ArrayTraits.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/Optional.h"
#include "zserio/ReflectableData.h"
#include "zserio/Traits.h"
#include "zserio/TypeInfo.h"
#include "zserio/Walker.h"

using test_object::std_allocator::WalkerBitmask;
using test_object::std_allocator::WalkerChoice;
using test_object::std_allocator::WalkerNested;
using test_object::std_allocator::WalkerObject;
using test_object::std_allocator::WalkerUnion;

namespace zserio
{

namespace
{

WalkerObject createWalkerObject(uint32_t identifier = 13, bool createNested = true)
{
    std::vector<WalkerUnion> unionArray;
    unionArray.resize(3);
    unionArray[0].emplace<WalkerUnion::ChoiceTag::CHOICE_text>("1");
    unionArray[1].emplace<WalkerUnion::ChoiceTag::CHOICE_value>(2);
    unionArray[2].emplace<WalkerUnion::ChoiceTag::CHOICE_nestedArray>(
            std::vector<WalkerNested>{{WalkerNested{"nestedArray"}}});
    const uint8_t choiceSelector = 0;
    WalkerChoice choiceField;
    if (createNested)
    {
        return WalkerObject(identifier, WalkerNested("nested"), "test", std::move(unionArray), std::nullopt,
                choiceSelector, choiceField);
    }
    else
    {
        return WalkerObject(identifier, std::nullopt, "test", std::move(unionArray), std::nullopt,
                choiceSelector, choiceField);
    }
}

class TestWalkObserver : public IWalkObserver
{
public:
    using CapturesMap = std::map<std::string_view, std::vector<IReflectableDataConstPtr>>;

    TestWalkObserver()
    {
        // initialize empty captures
        m_captures["beginRoot"];
        m_captures["endRoot"];
        m_captures["beginArray"];
        m_captures["endArray"];
        m_captures["beginCompound"];
        m_captures["endCompound"];
        m_captures["visitValue"];
    }

    void beginRoot(const IReflectableDataConstPtr& compound) override
    {
        m_captures["beginRoot"].push_back(compound);
    }

    void endRoot(const IReflectableDataConstPtr& compound) override
    {
        m_captures["endRoot"].push_back(compound);
    }

    void beginArray(const IReflectableDataConstPtr& array, const FieldInfo&) override
    {
        m_captures["beginArray"].push_back(array);
    }

    void endArray(const IReflectableDataConstPtr& array, const FieldInfo&) override
    {
        m_captures["endArray"].push_back(array);
    }

    void beginCompound(const IReflectableDataConstPtr& compound, const FieldInfo&, size_t) override
    {
        m_captures["beginCompound"].push_back(compound);
    }

    void endCompound(const IReflectableDataConstPtr& compound, const FieldInfo&, size_t) override
    {
        m_captures["endCompound"].push_back(compound);
    }

    void visitValue(const IReflectableDataConstPtr& value, const FieldInfo&, size_t) override
    {
        m_captures["visitValue"].push_back(value);
    }

    const std::vector<IReflectableDataConstPtr>& getCaptures(std::string_view captureName) const
    {
        return m_captures.find(captureName)->second;
    }

private:
    CapturesMap m_captures;
};

class TestWalkFilter : public IWalkFilter
{
public:
    TestWalkFilter& beforeArray(bool beforeArray)
    {
        m_beforeArray = beforeArray;
        return *this;
    }
    TestWalkFilter& afterArray(bool afterArray)
    {
        m_afterArray = afterArray;
        return *this;
    }
    TestWalkFilter& onlyFirstElement(bool onlyFirstElement)
    {
        m_onlyFirstElement = onlyFirstElement;
        return *this;
    }
    TestWalkFilter& beforeCompound(bool beforeCompound)
    {
        m_beforeCompound = beforeCompound;
        return *this;
    }
    TestWalkFilter& afterCompound(bool afterCompound)
    {
        m_afterCompound = afterCompound;
        return *this;
    }
    TestWalkFilter& beforeValue(bool beforeValue)
    {
        m_beforeValue = beforeValue;
        return *this;
    }
    TestWalkFilter& afterValue(bool afterValue)
    {
        m_afterValue = afterValue;
        return *this;
    }

    bool beforeArray(const IReflectableDataConstPtr&, const FieldInfo&) override
    {
        m_isFirstElement = true;
        return m_beforeArray;
    }

    bool afterArray(const IReflectableDataConstPtr&, const FieldInfo&) override
    {
        m_isFirstElement = false;
        return m_afterArray;
    }

    bool beforeCompound(const IReflectableDataConstPtr&, const FieldInfo&, size_t) override
    {
        return m_beforeCompound;
    }

    bool afterCompound(const IReflectableDataConstPtr&, const FieldInfo&, size_t) override
    {
        bool goToNext = !(m_onlyFirstElement && m_isFirstElement);
        m_isFirstElement = false;
        return goToNext && m_afterCompound;
    }

    bool beforeValue(const IReflectableDataConstPtr&, const FieldInfo&, size_t) override
    {
        return m_beforeValue;
    }

    bool afterValue(const IReflectableDataConstPtr&, const FieldInfo&, size_t) override
    {
        return m_afterValue;
    }

private:
    bool m_beforeArray = true;
    bool m_afterArray = true;
    bool m_onlyFirstElement = false;
    bool m_beforeCompound = true;
    bool m_afterCompound = true;
    bool m_beforeValue = true;
    bool m_afterValue = true;
    bool m_isFirstElement = false;
};

} // namespace

TEST(WalkerTest, walkNull)
{
    DefaultWalkObserver defaultObserver;
    Walker walker(defaultObserver);
    ASSERT_THROW(walker.walk(nullptr), CppRuntimeException);
}

TEST(WalkerTest, walkNonCompound)
{
    DefaultWalkObserver defaultObserver;
    Walker walker(defaultObserver);
    WalkerBitmask walkerBitmask;

    ASSERT_THROW(walker.walk(reflectable(walkerBitmask)), CppRuntimeException);
}

TEST(WalkerTest, walk)
{
    TestWalkObserver observer;
    DefaultWalkFilter defaultFilter;
    Walker walker(observer, defaultFilter);
    WalkerObject walkerObject = createWalkerObject();
    walker.walk(reflectable(walkerObject));

    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("beginRoot").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("endRoot").at(0)->getTypeInfo().getSchemaName());

    ASSERT_EQ(2, observer.getCaptures("beginArray").size());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginArray").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("beginArray").at(1)->getTypeInfo().getSchemaName());

    ASSERT_EQ(2, observer.getCaptures("endArray").size());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("endArray").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endArray").at(1)->getTypeInfo().getSchemaName());

    ASSERT_EQ(6, observer.getCaptures("beginCompound").size());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("beginCompound").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginCompound").at(1)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginCompound").at(2)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginCompound").at(3)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("beginCompound").at(4)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerChoice",
            observer.getCaptures("beginCompound").at(5)->getTypeInfo().getSchemaName());

    ASSERT_EQ(6, observer.getCaptures("endCompound").size());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("endCompound").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endCompound").at(1)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endCompound").at(2)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("endCompound").at(3)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endCompound").at(4)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerChoice",
            observer.getCaptures("endCompound").at(5)->getTypeInfo().getSchemaName());

    ASSERT_EQ(8, observer.getCaptures("visitValue").size());
    ASSERT_EQ(13, observer.getCaptures("visitValue").at(0)->toUInt());
    ASSERT_EQ("nested", observer.getCaptures("visitValue").at(1)->toString());
    ASSERT_EQ("test", observer.getCaptures("visitValue").at(2)->toString());
    ASSERT_EQ("1", observer.getCaptures("visitValue").at(3)->toString());
    ASSERT_EQ(2, observer.getCaptures("visitValue").at(4)->toUInt());
    ASSERT_EQ("nestedArray", observer.getCaptures("visitValue").at(5)->toString());
    ASSERT_EQ(nullptr, observer.getCaptures("visitValue").at(6));
    ASSERT_EQ(0, observer.getCaptures("visitValue").at(7)->toUInt());
}

TEST(WalkerTest, walkWrongUnionField)
{
    // use case: union field has wrong field name in type info (this cannot happen in generated code,
    //           therefore we need some wrong reflectable here)
    using allocator_type = ::std::allocator<uint8_t>;

    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<allocator_type>
    {
    public:
        explicit Reflectable(WalkerUnion&, const allocator_type& allocator) :
                ::zserio::ReflectableDataAllocatorHolderBase<allocator_type>(typeInfo<WalkerUnion>(), allocator)
        {}

        ::std::string_view getChoice() const override
        {
            return "wrong"; // this would be "value" in generated code
        }
    };

    TestWalkObserver observer;
    DefaultWalkFilter defaultFilter;
    Walker walker(observer, defaultFilter);

    WalkerUnion walkerUnion;
    walkerUnion.emplace<WalkerUnion::ChoiceTag::CHOICE_value>(0);
    walker.walk(std::allocate_shared<Reflectable>(allocator_type(), walkerUnion, allocator_type()));

    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginRoot").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endRoot").at(0)->getTypeInfo().getSchemaName());

    ASSERT_EQ(0, observer.getCaptures("beginArray").size());
    ASSERT_EQ(0, observer.getCaptures("endArray").size());

    ASSERT_EQ(0, observer.getCaptures("beginCompound").size());
    ASSERT_EQ(0, observer.getCaptures("endCompound").size());

    ASSERT_EQ(0, observer.getCaptures("visitValue").size());
}

TEST(WalkerTest, walkWrongOptionalCondition)
{
    // use case: optional condition states that the optional is used, but it is not set!
    TestWalkObserver observer;
    DefaultWalkFilter defaultFilter;
    Walker walker(observer, defaultFilter);
    WalkerObject walkerObject = createWalkerObject(13, false);
    walker.walk(reflectable(walkerObject));

    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("beginRoot").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("endRoot").at(0)->getTypeInfo().getSchemaName());

    ASSERT_EQ(2, observer.getCaptures("beginArray").size());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginArray").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("beginArray").at(1)->getTypeInfo().getSchemaName());

    ASSERT_EQ(2, observer.getCaptures("endArray").size());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("endArray").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endArray").at(1)->getTypeInfo().getSchemaName());

    ASSERT_EQ(5, observer.getCaptures("beginCompound").size());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginCompound").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginCompound").at(1)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginCompound").at(2)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("beginCompound").at(3)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerChoice",
            observer.getCaptures("beginCompound").at(4)->getTypeInfo().getSchemaName());

    ASSERT_EQ(5, observer.getCaptures("endCompound").size());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endCompound").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endCompound").at(1)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("endCompound").at(2)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endCompound").at(3)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerChoice",
            observer.getCaptures("endCompound").at(4)->getTypeInfo().getSchemaName());

    ASSERT_EQ(8, observer.getCaptures("visitValue").size());
    ASSERT_EQ(13, observer.getCaptures("visitValue").at(0)->toUInt());
    ASSERT_EQ(nullptr, observer.getCaptures("visitValue").at(1));
    ASSERT_EQ("test", observer.getCaptures("visitValue").at(2)->toString());
    ASSERT_EQ("1", observer.getCaptures("visitValue").at(3)->toString());
    ASSERT_EQ(2, observer.getCaptures("visitValue").at(4)->toUInt());
    ASSERT_EQ("nestedArray", observer.getCaptures("visitValue").at(5)->toString());
    ASSERT_EQ(nullptr, observer.getCaptures("visitValue").at(6));
    ASSERT_EQ(0, observer.getCaptures("visitValue").at(7)->toUInt());
}

TEST(WalkerTest, walkSkipCompound)
{
    TestWalkObserver observer;
    TestWalkFilter filter;
    filter.beforeCompound(false);
    Walker walker(observer, filter);
    WalkerObject walkerObject = createWalkerObject();
    walker.walk(reflectable(walkerObject));

    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("beginRoot").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("endRoot").at(0)->getTypeInfo().getSchemaName());

    ASSERT_EQ(1, observer.getCaptures("beginArray").size());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginArray").at(0)->getTypeInfo().getSchemaName());

    ASSERT_EQ(1, observer.getCaptures("endArray").size());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endArray").at(0)->getTypeInfo().getSchemaName());

    ASSERT_TRUE(observer.getCaptures("beginCompound").empty());
    ASSERT_TRUE(observer.getCaptures("endCompound").empty());

    ASSERT_EQ(4, observer.getCaptures("visitValue").size());
    ASSERT_EQ(13, observer.getCaptures("visitValue").at(0)->toUInt());
    ASSERT_EQ("test", observer.getCaptures("visitValue").at(1)->toString());
    ASSERT_EQ(nullptr, observer.getCaptures("visitValue").at(2));
    ASSERT_EQ(0, observer.getCaptures("visitValue").at(3)->toUInt());
}

TEST(WalkerTest, walkSkipSiblings)
{
    TestWalkObserver observer;
    TestWalkFilter filter;
    filter.afterValue(false);
    Walker walker(observer, filter);
    WalkerObject walkerObject = createWalkerObject();
    walker.walk(reflectable(walkerObject));

    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("beginRoot").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("endRoot").at(0)->getTypeInfo().getSchemaName());

    ASSERT_TRUE(observer.getCaptures("beginArray").empty());
    ASSERT_TRUE(observer.getCaptures("endArray").empty());

    ASSERT_TRUE(observer.getCaptures("beginCompound").empty());
    ASSERT_TRUE(observer.getCaptures("endCompound").empty());

    ASSERT_EQ(1, observer.getCaptures("visitValue").size());
    ASSERT_EQ(13, observer.getCaptures("visitValue").at(0)->toUInt());
}

TEST(WalkerTest, walkSkipAfterNested)
{
    TestWalkObserver observer;
    TestWalkFilter filter;
    filter.afterCompound(false);
    Walker walker(observer, filter);
    WalkerObject walkerObject = createWalkerObject();
    walker.walk(reflectable(walkerObject));

    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("beginRoot").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("endRoot").at(0)->getTypeInfo().getSchemaName());

    ASSERT_TRUE(observer.getCaptures("beginArray").empty());
    ASSERT_TRUE(observer.getCaptures("endArray").empty());

    ASSERT_EQ(1, observer.getCaptures("beginCompound").size());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("beginCompound").at(0)->getTypeInfo().getSchemaName());

    ASSERT_EQ(1, observer.getCaptures("endCompound").size());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("endCompound").at(0)->getTypeInfo().getSchemaName());

    ASSERT_EQ(2, observer.getCaptures("visitValue").size());
    ASSERT_EQ(13, observer.getCaptures("visitValue").at(0)->toUInt());
    ASSERT_EQ("nested", observer.getCaptures("visitValue").at(1)->toString());
}

TEST(WalkerTest, walkOnlyFirstElement)
{
    TestWalkObserver observer;
    TestWalkFilter filter;
    filter.onlyFirstElement(true);
    Walker walker(observer, filter);
    WalkerObject walkerObject = createWalkerObject();
    walker.walk(reflectable(walkerObject));

    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("beginRoot").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerObject",
            observer.getCaptures("endRoot").at(0)->getTypeInfo().getSchemaName());

    ASSERT_EQ(1, observer.getCaptures("beginArray").size());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginArray").at(0)->getTypeInfo().getSchemaName());

    ASSERT_EQ(1, observer.getCaptures("endArray").size());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endArray").at(0)->getTypeInfo().getSchemaName());

    ASSERT_EQ(3, observer.getCaptures("beginCompound").size());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("beginCompound").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("beginCompound").at(1)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerChoice",
            observer.getCaptures("beginCompound").at(2)->getTypeInfo().getSchemaName());

    ASSERT_EQ(3, observer.getCaptures("endCompound").size());
    ASSERT_EQ("test_object.std_allocator.WalkerNested",
            observer.getCaptures("endCompound").at(0)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerUnion",
            observer.getCaptures("endCompound").at(1)->getTypeInfo().getSchemaName());
    ASSERT_EQ("test_object.std_allocator.WalkerChoice",
            observer.getCaptures("endCompound").at(2)->getTypeInfo().getSchemaName());

    ASSERT_EQ(6, observer.getCaptures("visitValue").size());
    ASSERT_EQ(13, observer.getCaptures("visitValue").at(0)->toUInt());
    ASSERT_EQ("nested", observer.getCaptures("visitValue").at(1)->toString());
    ASSERT_EQ("test", observer.getCaptures("visitValue").at(2)->toString());
    ASSERT_EQ("1", observer.getCaptures("visitValue").at(3)->toString());
    ASSERT_EQ(nullptr, observer.getCaptures("visitValue").at(4));
    ASSERT_EQ(0, observer.getCaptures("visitValue").at(5)->toUInt());
}

TEST(DefaultWalkObserverTest, allMethods)
{
    DefaultWalkObserver defaultObserver;
    IWalkObserver& walkObserver = defaultObserver;
    IReflectableDataPtr walkerReflectable = nullptr;
    const FieldInfo& walkerFieldInfo = typeInfo<WalkerObject>().getFields()[0];

    ASSERT_NO_THROW(walkObserver.beginRoot(walkerReflectable));
    ASSERT_NO_THROW(walkObserver.endRoot(walkerReflectable));
    ASSERT_NO_THROW(walkObserver.beginArray(walkerReflectable, walkerFieldInfo));
    ASSERT_NO_THROW(walkObserver.endArray(walkerReflectable, walkerFieldInfo));
    ASSERT_NO_THROW(walkObserver.beginCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_NO_THROW(walkObserver.endCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_NO_THROW(walkObserver.visitValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
}

TEST(DefaultWalkFilterTest, allMethods)
{
    DefaultWalkFilter defaultFilter;
    IWalkFilter& walkFilter = defaultFilter;
    IReflectableDataPtr walkerReflectable = nullptr;
    const FieldInfo& walkerFieldInfo = typeInfo<WalkerObject>().getFields()[0];

    ASSERT_TRUE(walkFilter.beforeArray(walkerReflectable, walkerFieldInfo));
    ASSERT_TRUE(walkFilter.afterArray(walkerReflectable, walkerFieldInfo));
    ASSERT_TRUE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
}

TEST(DepthFilterTest, depth0)
{
    DepthWalkFilter depthWalkFilter(0);
    IWalkFilter& walkFilter = depthWalkFilter;
    IReflectableDataPtr walkerReflectable = nullptr;
    const FieldInfo& walkerFieldInfo = typeInfo<WalkerObject>().getFields()[0];

    ASSERT_FALSE(walkFilter.beforeArray(walkerReflectable, walkerFieldInfo)); // 0
    ASSERT_TRUE(walkFilter.afterArray(walkerReflectable, walkerFieldInfo)); // 0

    ASSERT_FALSE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 0
    ASSERT_TRUE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 0

    ASSERT_FALSE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 0
    ASSERT_TRUE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 0
}

TEST(DepthFilterTest, depth1)
{
    DepthWalkFilter depthWalkFilter(1);
    IWalkFilter& walkFilter = depthWalkFilter;
    IReflectableDataPtr walkerReflectable = nullptr;
    const FieldInfo& walkerFieldInfo = typeInfo<WalkerObject>().getFields()[0];

    ASSERT_TRUE(walkFilter.beforeArray(walkerReflectable, walkerFieldInfo)); // 0
    ASSERT_FALSE(walkFilter.beforeArray(walkerReflectable, walkerFieldInfo)); // 1
    ASSERT_TRUE(walkFilter.afterArray(walkerReflectable, walkerFieldInfo)); // 1
    ASSERT_FALSE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 1
    ASSERT_TRUE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 1
    ASSERT_FALSE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 1
    ASSERT_TRUE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 1
    ASSERT_TRUE(walkFilter.afterArray(walkerReflectable, walkerFieldInfo)); // 0

    ASSERT_TRUE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 0
    ASSERT_FALSE(walkFilter.beforeArray(walkerReflectable, walkerFieldInfo)); // 1
    ASSERT_TRUE(walkFilter.afterArray(walkerReflectable, walkerFieldInfo)); // 1
    ASSERT_FALSE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 1
    ASSERT_TRUE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 1
    ASSERT_FALSE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 1
    ASSERT_TRUE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 1
    ASSERT_TRUE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 0

    ASSERT_TRUE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 0
    ASSERT_TRUE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT)); // 0
}

TEST(RegexWalkFilterTest, regexAllMatch)
{
    RegexWalkFilter regexWalkFilter(".*");
    IWalkFilter& walkFilter = regexWalkFilter;
    IReflectableDataPtr walkerReflectable = nullptr;
    const FieldInfo& walkerFieldInfo = typeInfo<WalkerObject>().getFields()[0];
    const FieldInfo& walkerArrayFieldInfo = typeInfo<WalkerObject>().getFields()[3];

    ASSERT_TRUE(walkFilter.beforeArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_TRUE(walkFilter.afterArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_TRUE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
}

TEST(RegexWalkFilterTest, regexPrefixMatch)
{
    RegexWalkFilter regexWalkFilter("nested\\..*");
    IWalkFilter& walkFilter = regexWalkFilter;
    WalkerObject walkerObject = createWalkerObject();
    IReflectableDataConstPtr walkerReflectable = reflectable(walkerObject);

    const FieldInfo& identifierFieldInfo = typeInfo<WalkerObject>().getFields()[0];
    IReflectableDataConstPtr identifierReflectable = walkerReflectable->getField("identifier");
    ASSERT_FALSE(walkFilter.beforeValue(identifierReflectable, identifierFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterValue(identifierReflectable, identifierFieldInfo, WALKER_NOT_ELEMENT));

    const FieldInfo& nestedFieldInfo = typeInfo<WalkerObject>().getFields()[1];
    IReflectableDataConstPtr nestedReflectable = walkerReflectable->getField("nested");
    ASSERT_TRUE(walkFilter.beforeCompound(nestedReflectable, nestedFieldInfo, WALKER_NOT_ELEMENT));
    const FieldInfo& textFieldInfo = nestedFieldInfo.typeInfo.getFields()[0];
    IReflectableDataConstPtr textReflectable = nestedReflectable->getField("text");
    ASSERT_TRUE(walkFilter.beforeValue(textReflectable, textFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterValue(textReflectable, textFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterCompound(nestedReflectable, nestedFieldInfo, WALKER_NOT_ELEMENT));

    // ignore text

    const FieldInfo& unionArrayFieldInfo = typeInfo<WalkerObject>().getFields()[3];
    IReflectableDataConstPtr unionArrayReflectable = walkerReflectable->getField("unionArray");
    ASSERT_FALSE(walkFilter.beforeArray(unionArrayReflectable, unionArrayFieldInfo));
    ASSERT_TRUE(walkFilter.afterArray(unionArrayReflectable, unionArrayFieldInfo));
}

TEST(RegexWalkFilterTest, regexArrayMatch)
{
    RegexWalkFilter regexWalkFilter("unionArray\\[\\d+\\]\\.nes.*");
    IWalkFilter& walkFilter = regexWalkFilter;
    WalkerObject walkerObject = createWalkerObject();
    IReflectableDataConstPtr walkerReflectable = reflectable(walkerObject);

    const FieldInfo& unionArrayFieldInfo = typeInfo<WalkerObject>().getFields()[3];
    IReflectableDataConstPtr unionArrayReflectable = walkerReflectable->getField("unionArray");
    ASSERT_TRUE(walkFilter.beforeArray(unionArrayReflectable, unionArrayFieldInfo));

    ASSERT_FALSE(walkFilter.beforeCompound(unionArrayReflectable->at(0), unionArrayFieldInfo, 0));
    ASSERT_TRUE(walkFilter.afterCompound(unionArrayReflectable->at(0), unionArrayFieldInfo, 0));

    ASSERT_FALSE(walkFilter.beforeCompound(unionArrayReflectable->at(1), unionArrayFieldInfo, 1));
    ASSERT_TRUE(walkFilter.afterCompound(unionArrayReflectable->at(1), unionArrayFieldInfo, 1));

    ASSERT_TRUE(walkFilter.beforeCompound(unionArrayReflectable->at(2), unionArrayFieldInfo, 2));
    ASSERT_TRUE(walkFilter.afterCompound(unionArrayReflectable->at(2), unionArrayFieldInfo, 2));

    ASSERT_TRUE(walkFilter.afterArray(unionArrayReflectable, unionArrayFieldInfo));
}

TEST(RegexWalkFilterTest, regexArrayNoMatch)
{
    RegexWalkFilter regexWalkFilter("^unionArray\\[\\d*\\]\\.te.*");
    IWalkFilter& walkFilter = regexWalkFilter;

    std::vector<WalkerUnion> unionArray;
    unionArray.resize(1);
    unionArray[0].emplace<WalkerUnion::ChoiceTag::CHOICE_nestedArray>(
            std::vector<WalkerNested>{{WalkerNested{"nestedArray"}}});
    const uint8_t choiceSelector = 8;
    WalkerChoice choiceField;
    choiceField.emplace<WalkerChoice::ChoiceTag::CHOICE_value8>(0xAB);
    WalkerObject walkerObject(13, WalkerNested("nested"), "test", std::move(unionArray), std::nullopt,
            choiceSelector, choiceField);
    IReflectableDataConstPtr walkerReflectable = reflectable(walkerObject);

    const FieldInfo& unionArrayFieldInfo = typeInfo<WalkerObject>().getFields()[3];
    IReflectableDataConstPtr unionArrayReflectable = walkerReflectable->getField("unionArray");
    ASSERT_FALSE(walkFilter.beforeArray(unionArrayReflectable, unionArrayFieldInfo));
    ASSERT_TRUE(walkFilter.afterArray(unionArrayReflectable, unionArrayFieldInfo));
}

TEST(RegexWalkFilterTest, regexNullCompoundMatch)
{
    RegexWalkFilter regexWalkFilter("nested");
    IWalkFilter& walkFilter = regexWalkFilter;

    WalkerObject walkerObject = createWalkerObject(0, false);
    IReflectableDataConstPtr walkerReflectable = reflectable(walkerObject);

    const FieldInfo& nestedFieldInfo = typeInfo<WalkerObject>().getFields()[1];
    IReflectableDataConstPtr nestedReflectable = walkerReflectable->getField("nested");
    ASSERT_EQ(nullptr, nestedReflectable);
    // note that the null compounds are processed as values!
    ASSERT_TRUE(walkFilter.beforeValue(nestedReflectable, nestedFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterValue(nestedReflectable, nestedFieldInfo, WALKER_NOT_ELEMENT));
}

TEST(RegexWalkFilterTest, regexNullCompoundNoMatch)
{
    RegexWalkFilter regexWalkFilter("^nested\\.text$");
    IWalkFilter& walkFilter = regexWalkFilter;

    WalkerObject walkerObject = createWalkerObject(0, false);
    IReflectableDataConstPtr walkerReflectable = reflectable(walkerObject);

    const FieldInfo& nestedFieldInfo = typeInfo<WalkerObject>().getFields()[1];
    IReflectableDataConstPtr nestedReflectable = walkerReflectable->getField("nested");
    ASSERT_EQ(nullptr, nestedReflectable);
    // note that the null compounds are processed as values!
    ASSERT_FALSE(walkFilter.beforeValue(nestedReflectable, nestedFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterValue(nestedReflectable, nestedFieldInfo, WALKER_NOT_ELEMENT));
}

TEST(RegexWalkFilterTest, regexNullArrayMatch)
{
    RegexWalkFilter regexWalkFilter("optionalUnionArray");
    IWalkFilter& walkFilter = regexWalkFilter;

    WalkerObject walkerObject = createWalkerObject();
    IReflectableDataConstPtr walkerReflectable = reflectable(walkerObject);

    const FieldInfo& optionalUnionArrayFieldInfo = typeInfo<WalkerObject>().getFields()[4];
    IReflectableDataConstPtr optionalUnionArrayReflectable = walkerReflectable->getField("optionalUnionArray");
    ASSERT_EQ(nullptr, optionalUnionArrayReflectable);
    // note that the null arrays are processed as values!
    ASSERT_TRUE(walkFilter.beforeValue(
            optionalUnionArrayReflectable, optionalUnionArrayFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterValue(
            optionalUnionArrayReflectable, optionalUnionArrayFieldInfo, WALKER_NOT_ELEMENT));
}

TEST(RegexWalkFilterTest, regexNullArrayNoMatch)
{
    RegexWalkFilter regexWalkFilter("^optionalUnionArray\\.\\[\\d+\\]\\.nestedArray.*");
    IWalkFilter& walkFilter = regexWalkFilter;

    WalkerObject walkerObject = createWalkerObject();
    IReflectableDataConstPtr walkerReflectable = reflectable(walkerObject);

    const FieldInfo& optionalUnionArrayFieldInfo = typeInfo<WalkerObject>().getFields()[4];
    IReflectableDataConstPtr optionalUnionArrayReflectable = walkerReflectable->getField("optionalUnionArray");
    ASSERT_EQ(nullptr, optionalUnionArrayReflectable);
    // note that the null arrays are processed as values!
    ASSERT_FALSE(walkFilter.beforeValue(
            optionalUnionArrayReflectable, optionalUnionArrayFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterValue(
            optionalUnionArrayReflectable, optionalUnionArrayFieldInfo, WALKER_NOT_ELEMENT));
}

TEST(ArrayLengthWalkFilterTest, length0)
{
    ArrayLengthWalkFilter arrayLengthWalkFilter(0);
    IWalkFilter& walkFilter = arrayLengthWalkFilter;
    IReflectableDataConstPtr walkerReflectable = nullptr;
    const FieldInfo& walkerFieldInfo = typeInfo<WalkerObject>().getFields()[0];
    const FieldInfo& walkerArrayFieldInfo = typeInfo<WalkerObject>().getFields()[3];

    ASSERT_TRUE(walkFilter.beforeArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_FALSE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, 0));
    ASSERT_FALSE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, 0));
    ASSERT_FALSE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, 1));
    ASSERT_FALSE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, 1));
    ASSERT_TRUE(walkFilter.afterArray(walkerReflectable, walkerArrayFieldInfo));

    ASSERT_TRUE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.beforeArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_FALSE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, 0));
    ASSERT_FALSE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, 0));
    ASSERT_TRUE(walkFilter.afterArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_TRUE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
}

TEST(AndWalkFilterTest, empty)
{
    AndWalkFilter andWalkFilter({});
    IWalkFilter& walkFilter = andWalkFilter;
    IReflectableDataConstPtr walkerReflectable = nullptr;
    const FieldInfo& walkerFieldInfo = typeInfo<WalkerObject>().getFields()[0];
    const FieldInfo& walkerArrayFieldInfo = typeInfo<WalkerObject>().getFields()[3];

    ASSERT_TRUE(walkFilter.beforeArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_TRUE(walkFilter.afterArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_TRUE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
}

TEST(AndWalkFilterTest, trueTrue)
{
    TestWalkFilter trueFilter1;
    TestWalkFilter trueFilter2;
    AndWalkFilter andWalkFilter({std::ref<IWalkFilter>(trueFilter1), std::ref<IWalkFilter>(trueFilter2)});
    IWalkFilter& walkFilter = andWalkFilter;
    IReflectableDataConstPtr walkerReflectable = nullptr;
    const FieldInfo& walkerFieldInfo = typeInfo<WalkerObject>().getFields()[0];
    const FieldInfo& walkerArrayFieldInfo = typeInfo<WalkerObject>().getFields()[3];

    ASSERT_TRUE(walkFilter.beforeArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_TRUE(walkFilter.afterArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_TRUE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_TRUE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
}

TEST(AndWalkFilterTest, falseFalse)
{
    TestWalkFilter falseFilter1;
    falseFilter1.beforeArray(false);
    falseFilter1.afterArray(false);
    falseFilter1.beforeCompound(false);
    falseFilter1.afterCompound(false);
    falseFilter1.beforeValue(false);
    falseFilter1.afterValue(false);
    TestWalkFilter falseFilter2;
    falseFilter2.beforeArray(false);
    falseFilter2.afterArray(false);
    falseFilter2.beforeCompound(false);
    falseFilter2.afterCompound(false);
    falseFilter2.beforeValue(false);
    falseFilter2.afterValue(false);
    AndWalkFilter andWalkFilter({std::ref<IWalkFilter>(falseFilter1), std::ref<IWalkFilter>(falseFilter2)});
    IWalkFilter& walkFilter = andWalkFilter;
    IReflectableDataPtr walkerReflectable = nullptr;
    const FieldInfo& walkerFieldInfo = typeInfo<WalkerObject>().getFields()[0];
    const FieldInfo& walkerArrayFieldInfo = typeInfo<WalkerObject>().getFields()[3];

    ASSERT_FALSE(walkFilter.beforeArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_FALSE(walkFilter.afterArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_FALSE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_FALSE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_FALSE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_FALSE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
}

TEST(AndWalkFilterTest, trueFalse)
{
    TestWalkFilter trueFilter;
    TestWalkFilter falseFilter;
    falseFilter.beforeArray(false);
    falseFilter.afterArray(false);
    falseFilter.beforeCompound(false);
    falseFilter.afterCompound(false);
    falseFilter.beforeValue(false);
    falseFilter.afterValue(false);
    AndWalkFilter andWalkFilter({std::ref<IWalkFilter>(trueFilter), std::ref<IWalkFilter>(falseFilter)});
    IWalkFilter& walkFilter = andWalkFilter;
    IReflectableDataPtr walkerReflectable = nullptr;
    const FieldInfo& walkerFieldInfo = typeInfo<WalkerObject>().getFields()[0];
    const FieldInfo& walkerArrayFieldInfo = typeInfo<WalkerObject>().getFields()[3];

    ASSERT_FALSE(walkFilter.beforeArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_FALSE(walkFilter.afterArray(walkerReflectable, walkerArrayFieldInfo));
    ASSERT_FALSE(walkFilter.beforeCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_FALSE(walkFilter.afterCompound(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_FALSE(walkFilter.beforeValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
    ASSERT_FALSE(walkFilter.afterValue(walkerReflectable, walkerFieldInfo, WALKER_NOT_ELEMENT));
}

} // namespace zserio
