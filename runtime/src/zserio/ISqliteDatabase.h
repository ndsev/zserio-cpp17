#ifndef ZSERIO_ISQLITE_DATABASE_H_INC
#define ZSERIO_ISQLITE_DATABASE_H_INC

#include "zserio/SqliteConnection.h"

namespace zserio
{

/** Writer interface for generated databases. */
class ISqliteDatabase
{
public:
    /**
     * Destructor.
     */
    virtual ~ISqliteDatabase() = default;

    /**
     * Returns current database connection.
     *
     * \return SQLite database connection.
     */
    virtual SqliteConnection& connection() noexcept = 0;

    /**
     * Creates database schema.
     */
    virtual void createSchema() = 0;

    /**
     * Deletes database schema.
     */
    virtual void deleteSchema() = 0;
};

} // namespace zserio

#endif // ifndef ZSERIO_ISQLITE_DATABASE_H_INC
