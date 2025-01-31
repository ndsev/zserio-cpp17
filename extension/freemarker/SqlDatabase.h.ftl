<#include "FileHeader.inc.ftl">
<#include "Sql.inc.ftl">
<#include "DocComment.inc.ftl">
<@file_header generatorDescription/>
<#assign needsParameterProvider = sql_db_needs_parameter_provider(fields)/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <memory>
#include <array>
<@type_includes types.set/>
<@type_includes types.vector/>
<@type_includes types.map/>
<@type_includes types.vector/>
<@type_includes types.string/>
<@type_includes types.uniquePtr/>
#include <zserio/AllocatorHolder.h>
#include <zserio/ISqliteDatabase.h>
#include <zserio/SqliteConnection.h>
#include <zserio/IValidationObserver.h>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>

<#if docComments??>
<@doc_comments docComments/>
</#if>
class ${name} : public ::zserio::ISqliteDatabase, public ::zserio::AllocatorHolder<${types.allocator.default}>
{
public:
<#if needsParameterProvider>
    /** Interface for class which provides all explicit parameter providers of all tables in the database. */
    class IParameterProvider
    {
    public:
        /** Default destructor. */
        virtual ~IParameterProvider() = default;

    <#assign isTheFirst=true>
    <#list fields as field>
        <#if field.hasExplicitParameters>
                <#if !isTheFirst>

                <#else>
                    <#assign isTheFirst=false>
                </#if>
        /**
         * Gets the explicit parameter provider for the table ${field.name}.
         *
         * \return The explicit parameter provider for the table ${field.name}.
         */
        virtual ${field.typeInfo.typeFullName}::IParameterProvider& <@sql_db_table_parameter_provider_getter field/> = 0;
        </#if>
    </#list>
    };

</#if>
    /** Typedef for table relocation map which maps tables to different databases. */
    using TRelocationMap = <@map_type_name types.string.name, types.string.name/>;

    /**
     * Constructor from database file name and table relocation map.
     *
     * The method opens the database of given file name and for each relocated table attaches database to
     * which relocated table should be mapped.
     *
     * \param dbFileName Database file name to use.
     * \param tableToDbFileNameRelocationMap Mapping of relocated table name to database file name.
     * \param allocator Allocator to construct from.
     */
    explicit ${name}(const ${types.string.name}& dbFileName,
            const TRelocationMap& tableToDbFileNameRelocationMap = TRelocationMap(),
            const AllocatorType& allocator = AllocatorType());

    /**
     * Constructor from database file name.
     *
     * The method opens the database of given file name.
     *
     * \param dbFileName Database file name to use.
     * \param allocator Allocator to construct from.
     */
    explicit ${name}(const ${types.string.name}& dbFileName, const AllocatorType& allocator);

    /**
     * Constructor from external connection and table relocation map.
     *
     * The method uses given external connection if database is already open and for each relocated table uses
     * provided already attached database names.
     *
     * \param externalConnection Database connection of already open database.
     * \param tableToAttachedDbNameRelocationMap Mapping of relocated table name to attached database file name.
     * \param allocator Allocator to construct from.
     */
    explicit ${name}(sqlite3* externalConnection,
            const TRelocationMap& tableToAttachedDbNameRelocationMap = TRelocationMap(),
            const AllocatorType& allocator = AllocatorType());

    /**
     * Constructor from external connection.
     *
     * The method uses given external connection if database is already open.
     *
     * \param externalConnection Database connection of already open database.
     * \param allocator Allocator to construct from.
     */
    explicit ${name}(sqlite3* externalConnection, const AllocatorType& allocator);

    /** Default destructor. */
    ~${name}() override;

    /** Disables copy constructor. */
    ${name}(const ${name}&) = delete;
    /** Disables assignment operator. */
    ${name}& operator=(const ${name}&) = delete;

    /** Disables move constructor. */
    ${name}(${name}&&) = delete;
    /** Disables move assignment operator. */
    ${name}& operator=(${name}&&) = delete;

    /**
     * Gets the connection of underlying database.
     *
     * \return Reference to database connection.
     */
    ::zserio::SqliteConnection& connection() noexcept override;

<#list fields as field>
        <#if !field?is_first>

        </#if>
    /**
     * Gets the table ${field.name}.
     *
        <#if field.docComments??>
     * \b Description
     *
     <@doc_comments_inner field.docComments, 1/>
     *
        </#if>
     * \return Reference to the table ${field.name}.
     */
    ${field.typeInfo.typeFullName}& <@sql_db_table_getter_name field/>() noexcept;
</#list>

    /**
     * Creates all tables in the database.
     */
    void createSchema() override;

    /**
     * Creates all tables in the database except of given rowid tables.
     *
     * \param withoutRowIdTableNamesBlackList Set of rowid table names which should not be created.
     */
    void createSchema(const <@set_type_name types.string.name/>& withoutRowIdTableNamesBlackList);

    /**
     * Deletes all tables from the database.
     */
    void deleteSchema() override;

    /**
     * Validates all tables in the database.
     *
     * \param validationObserver Validation observer from which users can obtain validation results.
    <#if needsParameterProvider>
     * \param parameterProvider Provider of explicit parameters for all tables.
    </#if>
     */
    void validate(::zserio::IValidationObserver& validationObserver<#rt>
            <#lt><#if needsParameterProvider>, IParameterProvider& parameterProvider</#if>);

    <#-- cannot be constexpr since constexpr must be defined inline -->
    /**
     * Gets the database name.
     *
     * \return Database name.
     */
    static ::zserio::StringView databaseName() noexcept;

    /**
     * Gets all table names of the database.
     *
     * \return Array of all table names of the database.
     */
    static const ::std::array<::zserio::StringView, ${fields?size}>& tableNames() noexcept;

private:
    void initTables();
    void attachDatabase(::zserio::StringView fileName, ::zserio::StringView attachedDbName);
    void detachDatabases();

<#list fields as field>
    static ::zserio::StringView <@sql_db_table_name_getter field/> noexcept;
</#list>

    ::zserio::SqliteConnection m_db;
    <@vector_type_name types.string.name/> m_attachedDbList;
    TRelocationMap m_tableToAttachedDbNameRelocationMap;

<#list fields as field>
    <@unique_ptr_type_name field.typeInfo.typeFullName/> <@sql_field_member_name field/>;
</#list>
};
<@namespace_end package.path/>

<@include_guard_end package.path, name/>
