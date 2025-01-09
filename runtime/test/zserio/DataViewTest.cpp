#include "gtest/gtest.h"
#include "zserio/Array.h"
#include "zserio/DataView.h"

namespace zserio
{

struct TestParam
{
    VarSize len;
};

template <>
class View<TestParam>
{
public:
    explicit View(const TestParam& data) :
            m_data(data)
    {}

    VarSize len() const
    {
        return m_data.len;
    }

private:
    const TestParam& m_data;
};

struct TestObject
{
    Vector<UInt32> array;
};

template <>
class View<TestObject>
{
public:
    View(const TestObject& data, const View<TestParam>& param) :
            m_data(data),
            m_param(param)
    {}

    Array<Vector<UInt32>, ArrayType::AUTO> array() const
    {
        return Array<Vector<UInt32>, ArrayType::AUTO>(m_data.array);
    }

    View<TestParam> param() const
    {
        return m_param;
    }

protected:
    View(const TestObject& data, const View& other) :
            m_data(data),
            m_param(other.param())
    {}

private:
    const TestObject& m_data;
    View<TestParam> m_param;
};

namespace detail
{

template <>
void validate(const zserio::View<TestObject>& view, std::string_view fieldName)
{
    if (view.param().len() != view.array().size())
    {
        throw CppRuntimeException("Validation error (") << fieldName << ")!";
    }
}

template <>
BitSize bitSizeOf(const zserio::View<TestObject>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    endBitPosition = bitSizeOf(view.array());

    return endBitPosition - bitPosition;
}

} // namespace detail

TEST(DataViewTest, dataConstructor)
{
    TestParam param{3};
    TestObject data{Vector<UInt32>{{0, 1, 2}}};
    DataView dataView(data, View(param));

    // TODO[Mi-L@]: How to check that the raw array data doesn't remain on the same address?

    ASSERT_EQ(3, dataView.param().len());
    ASSERT_EQ(3, dataView.array().size());
    ASSERT_EQ(0, dataView.array().at(0));

    // validation error
    TestParam wrongParam{4};
    ASSERT_THROW(DataView(data, View(wrongParam)), CppRuntimeException);
}

TEST(DataViewTest, dataMoveConstructor)
{
    TestParam param{3};
    TestObject data{Vector<UInt32>{{0, 1, 2}}};
    DataView dataView(std::move(data), View(param));

    // TODO[Mi-L@]: How to check that the raw array data remain on the same address?

    ASSERT_EQ(3, dataView.param().len());
    ASSERT_EQ(3, dataView.array().size());
    ASSERT_EQ(0, dataView.array().at(0));

    // validation error
    TestParam wrongParam{4};
    TestObject otherData{Vector<UInt32>{{0, 1, 2}}};
    ASSERT_THROW(DataView(std::move(otherData), View(wrongParam)), CppRuntimeException);
}

TEST(DataViewTest, copyConstructor)
{
    TestParam param{3};
    TestObject data{Vector<UInt32>{{0, 1, 2}}};
    DataView dataView(data, View(param));

    DataView copied(dataView);
    ASSERT_EQ(3, copied.param().len());
    ASSERT_EQ(0, copied.array().at(0));
}

TEST(DataViewTest, moveConstructor)
{
    TestParam param{3};
    TestObject data{Vector<UInt32>{{0, 1, 2}}};
    DataView dataView(data, View(param));

    DataView copied(std::move(dataView));
    ASSERT_EQ(3, copied.param().len());
    ASSERT_EQ(0, copied.array().at(0));
}

} // namespace zserio
