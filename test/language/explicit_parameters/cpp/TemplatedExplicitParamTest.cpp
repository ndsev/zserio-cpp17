#include <memory>
#include <string>

#include "explicit_parameters/templated_explicit_param/TestDb.h"
#include "gtest/gtest.h"
#include "test_utils/SqlUtility.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/RebindAlloc.h"

namespace explicit_parameters
{
namespace templated_explicit_param
{

using AllocatorType = TestDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class TemplatedExplicitParamTest : public ::testing::Test
{
public:
    TemplatedExplicitParamTest()
    {
        const StringType dbFileName("language/explicit_parameters/templated_explicit_param_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new TestDb(dbFileName);
        m_database->createSchema();
    }

    ~TemplatedExplicitParamTest() override
    {
        delete m_database;
    }

    TemplatedExplicitParamTest(const TemplatedExplicitParamTest&) = delete;
    TemplatedExplicitParamTest& operator=(const TemplatedExplicitParamTest&) = delete;

    TemplatedExplicitParamTest(TemplatedExplicitParamTest&&) = delete;
    TemplatedExplicitParamTest& operator=(TemplatedExplicitParamTest&&) = delete;

protected:
    template <typename T>
    class ParameterProvider : public TestTableWithExplicit<T>::IParameterProvider
    {
    public:
        ::zserio::view_type_t<::std::tuple_element_t<0, typename ::zserio::detail::ObjectTraits<T>::Parameters>>
        len(const ::zserio::View<typename TestTableWithExplicit<T>::Row>& currentRow) override
        {
            return *currentRow.zserioData().id + 1;
        }
    };

    template <typename T>
    static void fillRows(
            ParameterProvider<T>& provider, VectorType<typename TestTableWithExplicit<T>::Row>& rows)
    {
        rows.clear();
        for (size_t i = 0; i < NUM_ROWS; ++i)
        {
            typename TestTableWithExplicit<T>::Row row;
            zserio::View<typename TestTableWithExplicit<T>::Row> rowView(row, provider);
            row.id = static_cast<uint32_t>(i);
            row.parameterizedColumn.emplace(VectorType<StringType>(provider.len(rowView), "test"));
            rows.push_back(std::move(row));
        }
    }
    template <typename T>
    static void checkRows(const VectorType<typename TestTableWithExplicit<T>::Row>& rows1,
            const VectorType<typename TestTableWithExplicit<T>::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            const auto& row1 = rows1[i];
            const auto& row2 = rows2[i];
            ASSERT_EQ(row1.id, row2.id);
            ASSERT_EQ(row1.parameterizedColumn, row2.parameterizedColumn);
        }
    }
    template <typename T>
    void writeReadTable(TestTableWithExplicit<T>& table)
    {
        ParameterProvider<T> provider;

        VectorType<typename TestTableWithExplicit<T>::Row> rows;
        fillRows<T>(provider, rows);
        table.write(provider, rows);

        VectorType<typename TestTableWithExplicit<T>::Row> readRows;
        auto reader = table.createReader(provider);
        while (reader.hasNext())
        {
            reader.next(readRows.emplace_back());
        }
        checkRows<T>(rows, readRows);
    }

    static constexpr size_t NUM_ROWS = 5;

    TestDb* m_database;
};

TEST_F(TemplatedExplicitParamTest, writeRead)
{
    writeReadTable(m_database->getExplicitParameterizedTable());
}

} // namespace templated_explicit_param
} // namespace explicit_parameters
