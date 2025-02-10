#ifndef TEST_UTILS_SQL_UTILITY_H_INC
#define TEST_UTILS_SQL_UTILITY_H_INC

#include <algorithm>
#include <string>
#include <string_view>
#include <zserio/SqliteFinalizer.h>

namespace test_utils
{

inline bool isTableInDb(zserio::ISqliteDatabase& database, std::string_view tableName)
{
    std::string sqlQuery = "SELECT name FROM sqlite_master WHERE type='table' AND name='";
    sqlQuery += tableName;
    sqlQuery += "'";
    std::unique_ptr<sqlite3_stmt, zserio::SqliteFinalizer> statement(
            database.connection().prepareStatement(sqlQuery));

    const int result = sqlite3_step(statement.get());
    if (result == SQLITE_DONE || result != SQLITE_ROW)
    {
        return false;
    }

    const unsigned char* readTableName = sqlite3_column_text(statement.get(), 0);
    return (readTableName != nullptr && tableName == reinterpret_cast<const char*>(readTableName));
}

inline bool isVirtualColumnInTable(
        zserio::ISqliteDatabase& database, std::string_view tableName, std::string_view virtualColumnName)
{
    std::string sqlQuery = "PRAGMA table_info(";
    sqlQuery += tableName;
    sqlQuery += ")";
    std::unique_ptr<sqlite3_stmt, zserio::SqliteFinalizer> statement(
            database.connection().prepareStatement(sqlQuery));

    bool isFound = false;
    while (!isFound)
    {
        int result = sqlite3_step(statement.get());
        if (result == SQLITE_DONE || result != SQLITE_ROW)
        {
            break;
        }

        const unsigned char* readColumnName = sqlite3_column_text(statement.get(), 1);
        if (readColumnName != nullptr && virtualColumnName == reinterpret_cast<const char*>(readColumnName))
        {
            isFound = true;
        }
    }

    return isFound;
}

} // namespace test_utils

#endif // TEST_UTILS_SQL_UTILITY_H_INC
