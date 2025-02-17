#include "gtest/gtest.h"
#include "sql_allocation/SqlAllocationDb.h"
#include "test_utils/MemoryResources.h"
#include "test_utils/ValidationObservers.h"

using namespace test_utils;

namespace sql_allocation
{

using AllocatorType = SqlAllocationDb::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;
using BitBufferType = zserio::BasicBitBuffer<zserio::RebindAlloc<AllocatorType, uint8_t>>;

class SqlAllocationTest : public ::testing::Test
{
public:
    SqlAllocationTest() :
            m_invalidMemoryResource(),
            m_invalidMemoryResourceSetter(m_invalidMemoryResource),
            m_memoryResource("Memory resource"),
            m_allocator(&m_memoryResource)
    {
        std::remove(DB_FILE_NAME);

        m_database.reset(new SqlAllocationDb(StringType(DB_FILE_NAME, m_allocator), m_allocator));
        m_database->createSchema();
    }

    ~SqlAllocationTest() override
    {
        m_database.reset();
        EXPECT_EQ(m_memoryResource.getNumDeallocations(), m_memoryResource.getNumAllocations());
    }

    SqlAllocationTest(const SqlAllocationTest&) = delete;
    SqlAllocationTest& operator=(const SqlAllocationTest&) = delete;

    SqlAllocationTest(SqlAllocationTest&&) = delete;
    SqlAllocationTest& operator=(SqlAllocationTest&&) = delete;

protected:
    class SqlAllocationTableParameterProvider : public SqlAllocationTable::IParameterProvider
    {
    public:
        zserio::View<DataBlob> dataBlob(const zserio::View<SqlAllocationTable::Row>&) override
        {
            return zserio::View(m_dataBlob);
        }

    private:
        DataBlob m_dataBlob = DataBlob{10, MAGIC};
    };

    class SqlAllocationDbParameterProvider : public SqlAllocationDb::IParameterProvider
    {
    public:
        SqlAllocationTableParameterProvider& getAllocationTableParameterProvider() override
        {
            return m_sqlAllocationTableParameterProvider;
        }

    private:
        SqlAllocationTableParameterProvider m_sqlAllocationTableParameterProvider;
    };

    const AllocatorType& getAllocator() const
    {
        return m_allocator;
    }

    void fillRows(VectorType<SqlAllocationTable::Row>& rows,
            SqlAllocationTableParameterProvider& parameterProvider, const AllocatorType& allocator)
    {
        rows.reserve(NUM_ROWS);
        for (uint32_t i = 0; i < NUM_ROWS; ++i)
        {
            fillRow(rows.emplace_back(allocator), parameterProvider, allocator, i);
        }
    }

    void fillRow(SqlAllocationTable::Row& row, SqlAllocationTableParameterProvider& parameterProvider,
            const AllocatorType& allocator, uint32_t id, uint32_t len = 0)
    {
        zserio::View rowView(row, parameterProvider);

        row.idWithVeryLongNameAndYetLongerName = id;
        row.textWithVeryLongNameAndYetLongerName =
                StringType("This is constant string longer than 32 bytes (", allocator) +
                zserio::toString(id, allocator) + ")";
        row.dataBlobWithVeryLongNameAndYetLongerName = DataBlob{len == 0 ? id + 1 : len, MAGIC};
        row.parameterizedBlobWithVeryLongNameAndYetLongerName =
                createParameterizedBlob(*row.dataBlobWithVeryLongNameAndYetLongerName, allocator);
        row.parameterizedBlobExplicitWithVeryLongNameAndYetLongerName =
                createParameterizedBlob(parameterProvider.dataBlob(rowView).zserioData(), allocator);
        row.colorWithVeryLongNameAndYetLongerName = id != RED_ROW_ID ? Color::GREEN : Color::RED;
        row.roleWithVeryLongNameAndYetLongerName = Role::Values::MEMBER;
    }

    ParameterizedBlob createParameterizedBlob(const DataBlob& dataBlob, const AllocatorType& allocator)
    {
        ParameterizedBlob parameterizedBlob{allocator};
        for (uint32_t i = 0; i < dataBlob.len; ++i)
        {
            parameterizedBlob.arr.push_back(i);
        }

        return parameterizedBlob;
    }

    static void checkRow(SqlAllocationTableParameterProvider& parameterProvider,
            const SqlAllocationTable::Row& row1, const SqlAllocationTable::Row& row2)
    {
        zserio::View rowView1(row1, parameterProvider);
        zserio::View rowView2(row2, parameterProvider);

        ASSERT_EQ(rowView1.idWithVeryLongNameAndYetLongerName(), rowView2.idWithVeryLongNameAndYetLongerName());
        ASSERT_EQ(rowView1.textWithVeryLongNameAndYetLongerName(),
                rowView2.textWithVeryLongNameAndYetLongerName());
        ASSERT_EQ(rowView1.dataBlobWithVeryLongNameAndYetLongerName(),
                rowView2.dataBlobWithVeryLongNameAndYetLongerName());
        ASSERT_EQ(rowView1.parameterizedBlobWithVeryLongNameAndYetLongerName(),
                rowView2.parameterizedBlobWithVeryLongNameAndYetLongerName());
        ASSERT_EQ(rowView1.parameterizedBlobExplicitWithVeryLongNameAndYetLongerName(),
                rowView2.parameterizedBlobExplicitWithVeryLongNameAndYetLongerName());
        ASSERT_EQ(rowView1.colorWithVeryLongNameAndYetLongerName(),
                rowView2.colorWithVeryLongNameAndYetLongerName());
        ASSERT_EQ(rowView1.roleWithVeryLongNameAndYetLongerName(),
                rowView2.roleWithVeryLongNameAndYetLongerName());
    }

    static void checkRows(SqlAllocationTableParameterProvider& parameterProvider,
            const VectorType<SqlAllocationTable::Row>& rows1, const VectorType<SqlAllocationTable::Row>& rows2)
    {
        ASSERT_EQ(rows1.size(), rows2.size());
        for (size_t i = 0; i < rows1.size(); ++i)
        {
            checkRow(parameterProvider, rows1.at(i), rows2.at(i));
        }
    }

private:
    InvalidMemoryResource m_invalidMemoryResource;
    MemoryResourceScopedSetter m_invalidMemoryResourceSetter;
    TestMemoryResource<12 * 1024> m_memoryResource;
    AllocatorType m_allocator;

protected:
    std::unique_ptr<SqlAllocationDb> m_database; // must be behind the m_memoryResource
    static constexpr uint32_t NUM_ROWS = 3;
    static constexpr uint32_t RED_ROW_ID = 1;
    static constexpr uint32_t MAGIC = 1;
    static constexpr uint32_t WRONG_MAGIC = 0;

private:
    static constexpr const char* DB_FILE_NAME = "arguments/set_cpp_allocator/sql_allocation_test.sqlite";
};

TEST_F(SqlAllocationTest, readWithoutCondition)
{
    SqlAllocationTableParameterProvider parameterProvider;
    VectorType<SqlAllocationTable::Row> writtenRows(getAllocator());
    fillRows(writtenRows, parameterProvider, getAllocator());

    SqlAllocationTable& sqlAllocationTable = m_database->getAllocationTable();
    sqlAllocationTable.write(parameterProvider, writtenRows);

    SqlAllocationTable::Reader reader = sqlAllocationTable.createReader(parameterProvider);

    VectorType<SqlAllocationTable::Row> readRows{getAllocator()};
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back(getAllocator()));
    }
    checkRows(parameterProvider, writtenRows, readRows);
}

TEST_F(SqlAllocationTest, readWithCondition)
{
    SqlAllocationTableParameterProvider parameterProvider;
    VectorType<SqlAllocationTable::Row> writtenRows(getAllocator());
    fillRows(writtenRows, parameterProvider, getAllocator());

    SqlAllocationTable& sqlAllocationTable = m_database->getAllocationTable();
    sqlAllocationTable.write(parameterProvider, writtenRows);

    const StringType condition = StringType("colorWithVeryLongNameAndYetLongerName=", getAllocator()) +
            zserio::toString(zserio::enumToValue(Color::RED), getAllocator());

    SqlAllocationTable::Reader reader = sqlAllocationTable.createReader(parameterProvider, condition);
    ASSERT_TRUE(reader.hasNext());
    SqlAllocationTable::Row readRow(getAllocator());
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());

    checkRow(parameterProvider, writtenRows[RED_ROW_ID], readRow);
}

TEST_F(SqlAllocationTest, update)
{
    SqlAllocationTableParameterProvider parameterProvider;
    VectorType<SqlAllocationTable::Row> writtenRows(getAllocator());
    fillRows(writtenRows, parameterProvider, getAllocator());

    SqlAllocationTable& sqlAllocationTable = m_database->getAllocationTable();
    sqlAllocationTable.write(parameterProvider, writtenRows);

    const uint32_t updateRowId = 2;
    SqlAllocationTable::Row updateRow(getAllocator());
    fillRow(updateRow, parameterProvider, getAllocator(), updateRowId, 4);
    const StringType updateCondition = StringType("idWithVeryLongNameAndYetLongerName=", getAllocator()) +
            zserio::toString(updateRowId, getAllocator());
    sqlAllocationTable.update(parameterProvider, updateRow, updateCondition);

    SqlAllocationTable::Reader reader = sqlAllocationTable.createReader(parameterProvider, updateCondition);
    ASSERT_TRUE(reader.hasNext());
    SqlAllocationTable::Row readRow(getAllocator());
    reader.next(readRow);
    ASSERT_FALSE(reader.hasNext());

    checkRow(parameterProvider, updateRow, readRow);
}

TEST_F(SqlAllocationTest, validateValidDatabase)
{
    SqlAllocationDbParameterProvider dbParameterProvider;
    VectorType<SqlAllocationTable::Row> writtenRows(getAllocator());
    fillRows(writtenRows, dbParameterProvider.getAllocationTableParameterProvider(), getAllocator());

    SqlAllocationTable& sqlAllocationTable = m_database->getAllocationTable();
    sqlAllocationTable.write(dbParameterProvider.getAllocationTableParameterProvider(), writtenRows);

    CountingValidationObserver validationObserver;
    m_database->validate(validationObserver, dbParameterProvider);

    ASSERT_EQ(1, validationObserver.getNumberOfValidatedTables());
    ASSERT_EQ(NUM_ROWS, validationObserver.getNumberOfValidatedRows());
    ASSERT_EQ(0, validationObserver.getNumberOfErrors());
}

TEST_F(SqlAllocationTest, validateInvalidSchema)
{
    zserio::SqliteConnection& connection = m_database->connection();

    const bool wasTransactionStarted = connection.startTransaction();
    connection.executeUpdate("DROP TABLE allocationTable");
    connection.executeUpdate(
            "CREATE TABLE allocationTable("
            "idWithVeryLongNameAndYetLongerName INTEGER NOT NULL, " // shall be PK
            "textWithVeryLongNameAndYetLongerName TEXT, " // shall be NOT NULL
            "dataBlobWithVeryLongNameAndYetLongerName TEXT NOT NULL, " // wrong type, shall be BLOB
            "parameterizedBlobWithVeryLongNameAndYetLongerName BLOB NOT NULL, " // shall not be NOT NULL
            "parameterizedBlobExplicitWithVeryLongNameAndYetLongerName BLOB, "
            "colorWithVeryLongNameAndYetLongerName INTEGER PRIMARY KEY, " // shall not be PK
            // missing roleWithVeryLongNameAndYetLongerName
            "superfluousColumnWithVeryLongNameAndYetLongerName INTEGER)" // superfluous
    );
    connection.endTransaction(wasTransactionStarted);

    SqlAllocationDbParameterProvider dbParameterProvider;
    CountingValidationObserver validationObserver;
    m_database->validate(validationObserver, dbParameterProvider);

    ASSERT_EQ(1, validationObserver.getNumberOfValidatedTables());
    ASSERT_EQ(7, validationObserver.getNumberOfErrors());
}

TEST_F(SqlAllocationTest, validateInvalidField)
{
    SqlAllocationDbParameterProvider dbParameterProvider;
    VectorType<SqlAllocationTable::Row> writtenRows(getAllocator());
    fillRows(writtenRows, dbParameterProvider.getAllocationTableParameterProvider(), getAllocator());

    SqlAllocationTable& sqlAllocationTable = m_database->getAllocationTable();
    sqlAllocationTable.write(dbParameterProvider.getAllocationTableParameterProvider(), writtenRows);

    zserio::SqliteConnection& connection = m_database->connection();
    const bool wasTransactionStarted = connection.startTransaction();
    connection.executeUpdate(
            "UPDATE allocationTable SET colorWithVeryLongNameAndYetLongerName = 13 "
            "WHERE idWithVeryLongNameAndYetLongerName = 0");
    connection.executeUpdate(
            "UPDATE allocationTable SET roleWithVeryLongNameAndYetLongerName = 100000 "
            "WHERE idWithVeryLongNameAndYetLongerName = 1");
    {
        std::unique_ptr<sqlite3_stmt, zserio::SqliteFinalizer> statement(connection.prepareStatement(
                "UPDATE allocationTable SET dataBlobWithVeryLongNameAndYetLongerName = ? "
                "WHERE idWithVeryLongNameAndYetLongerName = 2"));
        BitBufferType bitBuffer{64, getAllocator()}; // for data blob 2 * uint32
        zserio::BitStreamWriter writer(bitBuffer);
        writer.writeUnsignedBits32(2 + 1, 32); // len = id + 1
        writer.writeUnsignedBits32(WRONG_MAGIC, 32); // magic
        sqlite3_bind_blob(statement.get(), 1, bitBuffer.getBuffer(), static_cast<int>(bitBuffer.getByteSize()),
                SQLITE_TRANSIENT);
        ASSERT_EQ(SQLITE_DONE, sqlite3_step(statement.get()));
    }
    connection.endTransaction(wasTransactionStarted);

    CountingValidationObserver validationObserver;
    m_database->validate(validationObserver, dbParameterProvider);

    ASSERT_EQ(1, validationObserver.getNumberOfValidatedTables());
    ASSERT_EQ(NUM_ROWS, validationObserver.getNumberOfValidatedRows());
    ASSERT_EQ(3, validationObserver.getNumberOfErrors());
}

} // namespace sql_allocation
