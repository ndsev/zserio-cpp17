#include <cassert>
#include <set>

#include "RouteSegment.h"
#include "pmr/RouteSegment.h"
#include "zserio/DataView.h"
#include "zserio/SerializeUtil.h"

void TestOperatorLess();
void TestSerialize(RouteSegment& data);
void TestSerialize(pmr::RouteSegment& data);
void TestSerializeDataView(const RouteSegment& data);

int main()
{
    TestOperatorLess();

    {
        // set test data
        RouteSegment route;
        route.hasEndOffset = true;
        route.shift = 3;
        LinePositionOffset2D lpos;
        lpos.linePosition = 10;
        lpos.numBits = 16;
        lpos.offset = { 100, 110 };
        route.endPositionWithOffset = lpos;
        route.myArray = { 9, 8, 7 };
        route.container.numElements = 1;
        route.container.array.push_back(lpos);
        route.packedContainer.push_back({ 1, { lpos }, "a" });
        route.packedContainer.push_back({ 1, { lpos }, "b" });
        route.packedContainer.push_back({ 1, { lpos }, "c" });
        route.myChoice.objectChoice = ::zserio::Int16(-3);
        route.myUnion.objectChoice = ::zserio::String("haha");

        TestSerialize(route);
        TestSerializeDataView(route);
    }

    {
        // set test data
        pmr::RouteSegment route;
        route.hasEndOffset = true;
        route.shift = 3;
        pmr::LinePositionOffset2D lpos;
        lpos.linePosition = 10;
        lpos.numBits = 16;
        lpos.offset = { 100, 110 };
        route.endPositionWithOffset = lpos;
        route.myArray = { 9, 8, 7 };
        route.container.numElements = 1;
        route.container.array.push_back(lpos);
        route.packedContainer.push_back({ 1, { lpos }, "a" });
        route.packedContainer.push_back({ 1, { lpos }, "b" });
        route.packedContainer.push_back({ 1, { lpos }, "c" });
        route.myChoice.objectChoice = ::zserio::Int16(-3);
        route.myUnion.objectChoice = ::zserio::String("haha");

        TestSerialize(route);
    }
}

void TestOperatorLess()
{
    // test template operator< is reachable
    // this was not the case with PositionContainer<LinePositionOffset2D>::View
    ::std::set<PositionContainer<LinePositionOffset2D, ::zserio::Int32>> sortedData;
    ::std::set<::zserio::View<PositionContainer<LinePositionOffset2D, ::zserio::Int32>>> sortedViews;

    // test hash
    ::std::hash<RouteSegment>()(RouteSegment());
    ::std::hash<pmr::RouteSegment>()(pmr::RouteSegment());
}

void TestSerialize(RouteSegment& data)
{
    // data are not validated, serialization will validate data and initialize offsets
    auto bitBuffer = ::zserio::serialize(data);

    // deserialization will fill the data and return a view to the data
    RouteSegment readData;
    const ::zserio::View<RouteSegment> readView = ::zserio::deserialize(bitBuffer, readData);
    assert(data == readData);
}

void TestSerialize(pmr::RouteSegment& data)
{
    // data are not validated, serialization will validate data and initialize offsets
    auto bitBuffer = ::zserio::serialize(data);

    // deserialization will fill the data and return a view to the data
    pmr::RouteSegment readData;
    const ::zserio::View<pmr::RouteSegment> readView = ::zserio::deserialize(bitBuffer, readData);
    assert(data == readData);
}

void TestSerializeDataView(const RouteSegment& data)
{
    {
        const ::zserio::DataView<RouteSegment> dataView(data);
        // we have already validated data and set offsets, serialization does not validate anything
        auto bitBuffer = ::zserio::serialize(dataView);
        const ::zserio::DataView<RouteSegment> readDataView = ::zserio::deserialize<RouteSegment>(bitBuffer);
        assert(readDataView == dataView);

        // make a data copy and modify data
        RouteSegment newData = readDataView.data();
        newData.shift = 4;
        const ::zserio::DataView<RouteSegment> newDataView(newData);
        bitBuffer = ::zserio::serialize(newDataView);
        const ::zserio::DataView<RouteSegment> readNewDataView = ::zserio::deserialize<RouteSegment>(bitBuffer);
        assert(readNewDataView == newDataView);
    }

    {
        const RouteSegment dataCopy = data;
        const ::zserio::DataView<RouteSegment> dataView(::std::move(dataCopy));
        // we have already validated data and set offsets, serialization does not validate anything
        auto bitBuffer = ::zserio::serialize(dataView);
        const ::zserio::DataView<RouteSegment> readDataView = ::zserio::deserialize<RouteSegment>(bitBuffer);
        assert(readDataView == dataView);

        // make a data copy and modify data
        RouteSegment newData = readDataView.data();
        newData.shift = 4;
        const ::zserio::DataView<RouteSegment> newDataView(::std::move(newData));
        bitBuffer = ::zserio::serialize(newDataView);
        const ::zserio::DataView<RouteSegment> readNewDataView = ::zserio::deserialize<RouteSegment>(bitBuffer);
        assert(readNewDataView == newDataView);
    }
}
