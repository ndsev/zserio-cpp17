#include <map>
#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "sql_types/SqlTypesDb.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SqliteFinalizer.h"

namespace sql_types
{

using AllocatorType = SqlTypesDb::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

class SqlTypesTest : public ::testing::Test
{
public:
    SqlTypesTest()
    {
        const StringType dbFileName("language/sql_types/sql_types_test.sqlite");
        std::remove(dbFileName.c_str());

        m_database = new sql_types::SqlTypesDb(dbFileName);
        m_database->createSchema();
    }

    ~SqlTypesTest() override
    {
        delete m_database;
    }

    SqlTypesTest(const SqlTypesTest&) = delete;
    SqlTypesTest& operator=(const SqlTypesTest&) = delete;

    SqlTypesTest(SqlTypesTest&&) = delete;
    SqlTypesTest& operator=(SqlTypesTest&&) = delete;

protected:
    bool getSqlColumnTypes(std::map<StringType, StringType>& sqlColumnTypes)
    {
        // prepare SQL query
        StringType checkTableName = "sqlTypesTable";
        StringType sqlQuery = "PRAGMA table_info(" + checkTableName + ")";

        // get table info
        std::unique_ptr<sqlite3_stmt, zserio::SqliteFinalizer> statement(
                m_database->connection().prepareStatement(sqlQuery));

        int result = SQLITE_OK;
        while ((result = sqlite3_step(statement.get())) != SQLITE_DONE)
        {
            if (result != SQLITE_ROW)
            {
                return false;
            }

            const unsigned char* columnName = sqlite3_column_text(statement.get(), 1);
            const unsigned char* columnType = sqlite3_column_text(statement.get(), 2);
            if (columnName == nullptr || columnType == nullptr)
            {
                return false;
            }

            sqlColumnTypes[StringType(reinterpret_cast<const char*>(columnName))] =
                    StringType(reinterpret_cast<const char*>(columnType));
        }

        return true;
    }

    sql_types::SqlTypesDb* m_database;
};

TEST_F(SqlTypesTest, unsignedIntegerTypes)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("uint8Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("uint16Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("uint32Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("uint64Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);
}

TEST_F(SqlTypesTest, signedIntegerTypes)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("int8Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("int16Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("int32Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("int64Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);
}

TEST_F(SqlTypesTest, unsignedBitfieldTypes)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("bitfield8Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("variableBitfieldType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);
}

TEST_F(SqlTypesTest, signedBitfieldTypes)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("intfield8Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("variableIntfieldType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);
}

TEST_F(SqlTypesTest, float16Type)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("float16Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("REAL", it->second);
}

TEST_F(SqlTypesTest, float32Type)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("float32Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("REAL", it->second);
}

TEST_F(SqlTypesTest, float64Type)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("float64Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("REAL", it->second);
}

TEST_F(SqlTypesTest, variableUnsignedIntegerTypes)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("varuint16Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("varuint32Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("varuint64Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("varuintType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("varsizeType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);
}

TEST_F(SqlTypesTest, variableSignedIntegerTypes)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("varint16Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("varint32Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("varint64Type");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);

    it = sqlColumnTypes.find("varintType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);
}

TEST_F(SqlTypesTest, boolType)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("boolType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);
}

TEST_F(SqlTypesTest, stringTypes)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("stringType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("TEXT", it->second);
}

TEST_F(SqlTypesTest, enumType)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("enumType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);
}

TEST_F(SqlTypesTest, bitmaskType)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("bitmaskType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("INTEGER", it->second);
}

TEST_F(SqlTypesTest, structureType)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("structureType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("BLOB", it->second);
}

TEST_F(SqlTypesTest, choiceType)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("choiceType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("BLOB", it->second);
}

TEST_F(SqlTypesTest, unionType)
{
    std::map<StringType, StringType> sqlColumnTypes;
    ASSERT_TRUE(getSqlColumnTypes(sqlColumnTypes));

    std::map<StringType, StringType>::const_iterator it = sqlColumnTypes.find("unionType");
    ASSERT_TRUE(it != sqlColumnTypes.end());
    ASSERT_EQ("BLOB", it->second);
}

} // namespace sql_types
