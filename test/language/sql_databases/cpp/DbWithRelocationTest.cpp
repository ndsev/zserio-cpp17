#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "sql_databases/db_with_relocation/AmericaDb.h"
#include "sql_databases/db_with_relocation/EuropeDb.h"
#include "test_utils/SqlUtility.h"
#include "zserio/RebindAlloc.h"
#include "zserio/StringConvertUtil.h"

namespace sql_databases
{
namespace db_with_relocation
{

using allocator_type = EuropeDb::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class DbWithRelocationTest : public ::testing::Test
{
public:
    DbWithRelocationTest() :
            m_europeDbFileName("language/sql_databases/db_with_relocation_test_europe.sqlite"),
            m_americaDbFileName("language/sql_databases/db_with_relocation_test_america.sqlite"),
            m_relocatedSlovakiaTableName("slovakia"),
            m_relocatedCzechiaTableName("czechia")
    {
        std::remove(m_europeDbFileName.c_str());
        std::remove(m_americaDbFileName.c_str());

        m_europeDb.reset(new EuropeDb(m_europeDbFileName));
        m_europeDb->createSchema();

        AmericaDb::TRelocationMap tableToDbFileNameRelocationMap;
        tableToDbFileNameRelocationMap.insert(std::make_pair(m_relocatedSlovakiaTableName, m_europeDbFileName));
        tableToDbFileNameRelocationMap.insert(std::make_pair(m_relocatedCzechiaTableName, m_europeDbFileName));
        m_americaDb.reset(new AmericaDb(m_americaDbFileName, tableToDbFileNameRelocationMap));
        m_americaDb->createSchema();

        m_attachedDatabasesNames.insert("main");
        m_attachedDatabasesNames.insert("AmericaDb_" + m_relocatedSlovakiaTableName);
        m_attachedDatabasesNames.insert("AmericaDb_" + m_relocatedCzechiaTableName);
    }

protected:
    const StringType m_europeDbFileName;
    const StringType m_americaDbFileName;
    const StringType m_relocatedSlovakiaTableName;
    const StringType m_relocatedCzechiaTableName;

    std::unique_ptr<EuropeDb> m_europeDb;
    std::unique_ptr<AmericaDb> m_americaDb;

    std::set<StringType> m_attachedDatabasesNames;
};

TEST_F(DbWithRelocationTest, tableGetters)
{
    CountryMapTable& germanyTable = m_europeDb->getGermany();
    CountryMapTable& usaTable = m_americaDb->getUsa();
    CountryMapTable& canadaTable = m_americaDb->getCanada();
    CountryMapTable& slovakiaTable = m_americaDb->getSlovakia();
    CountryMapTable& czechiaTable = m_americaDb->getCzechia();

    ASSERT_TRUE(&germanyTable != &usaTable);
    ASSERT_TRUE(&usaTable != &canadaTable);
    ASSERT_TRUE(&canadaTable != &slovakiaTable);
    ASSERT_TRUE(&slovakiaTable != &czechiaTable);
}

TEST_F(DbWithRelocationTest, relocatedSlovakiaTable)
{
    ASSERT_FALSE(test_utils::isTableInDb(*m_americaDb, m_relocatedSlovakiaTableName));
    ASSERT_TRUE(test_utils::isTableInDb(*m_europeDb, m_relocatedSlovakiaTableName));

    // write to relocated table
    int32_t updateTileId = 1;
    VectorType<CountryMapTable::Row> writtenRows(1);
    CountryMapTable::Row& row = writtenRows.back();
    row.tileId = updateTileId;
    Tile writtenTile;
    writtenTile.version = 'a';
    writtenTile.data = 'A';
    row.tile = writtenTile;
    CountryMapTable& relocatedTable = m_americaDb->getSlovakia();
    relocatedTable.write(writtenRows);

    // update it
    CountryMapTable::Row updateRow;
    updateRow.tileId = updateTileId;
    Tile updatedTile;
    updatedTile.version = 'b';
    updatedTile.data = 'B';
    updateRow.tile = updatedTile;
    const StringType updateCondition = StringType("tileId=") + zserio::toString<allocator_type>(updateTileId);
    relocatedTable.update(updateRow, updateCondition);

    // read it back
    VectorType<CountryMapTable::Row> readRows;
    CountryMapTable::Reader reader = relocatedTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    ASSERT_EQ(1, readRows.size());
    ASSERT_EQ(updateRow.tileId, readRows.front().tileId);
    ASSERT_EQ(updateRow.tile, readRows.front().tile);
}

TEST_F(DbWithRelocationTest, relocatedCzechiaTable)
{
    ASSERT_FALSE(test_utils::isTableInDb(*m_americaDb, m_relocatedCzechiaTableName));
    ASSERT_TRUE(test_utils::isTableInDb(*m_europeDb, m_relocatedCzechiaTableName));

    // write to relocated table
    int32_t updateTileId = 1;
    VectorType<CountryMapTable::Row> writtenRows(1);
    CountryMapTable::Row& row = writtenRows.back();
    row.tileId = updateTileId;
    Tile writtenTile;
    writtenTile.version = 'c';
    writtenTile.data = 'C';
    row.tile = writtenTile;
    CountryMapTable& relocatedTable = m_americaDb->getCzechia();
    relocatedTable.write(writtenRows);

    // update it
    CountryMapTable::Row updateRow;
    updateRow.tileId = updateTileId;
    Tile updatedTile;
    updatedTile.version = 'd';
    updatedTile.data = 'D';
    updateRow.tile = updatedTile;
    const StringType updateCondition = StringType("tileId=") + zserio::toString<allocator_type>(updateTileId);
    relocatedTable.update(updateRow, updateCondition);

    // read it back
    VectorType<CountryMapTable::Row> readRows;
    CountryMapTable::Reader reader = relocatedTable.createReader();
    while (reader.hasNext())
    {
        reader.next(readRows.emplace_back());
    }

    ASSERT_EQ(1, readRows.size());
    ASSERT_EQ(updateRow.tileId, readRows.front().tileId);
    ASSERT_EQ(updateRow.tile, readRows.front().tile);
}

TEST_F(DbWithRelocationTest, attachedDatabases)
{
    std::unique_ptr<sqlite3_stmt, zserio::SqliteFinalizer> statement(
            m_americaDb->connection().prepareStatement("PRAGMA database_list"));

    while (sqlite3_step(statement.get()) == SQLITE_ROW)
    {
        const char* databaseName = reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 1));
        ASSERT_EQ(1, m_attachedDatabasesNames.count(databaseName));
        m_attachedDatabasesNames.erase(databaseName);
    }

    ASSERT_EQ(1, m_attachedDatabasesNames.size());
    ASSERT_EQ(0, m_attachedDatabasesNames.count("main"));
}

} // namespace db_with_relocation
} // namespace sql_databases
