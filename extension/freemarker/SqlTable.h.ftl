<#include "FileHeader.inc.ftl">
<#include "Sql.inc.ftl">
<#include "DocComment.inc.ftl">
<@file_header generatorDescription/>

<@include_guard_begin package.path, name/>

<@runtime_version_check generatorVersion/>

#include <memory>
#include <sqlite3.h>
#include <string_view>
<@type_includes types.optional/>
<@type_includes types.vector/>
<@type_includes types.string/>
#include <zserio/AllocatorHolder.h>
#include <zserio/IValidationObserver.h>
#include <zserio/Span.h>
#include <zserio/SqliteConnection.h>
#include <zserio/SqliteFinalizer.h>
#include <zserio/ValidationSqliteUtil.h>
<@system_includes headerSystemIncludes/>
<@user_includes headerUserIncludes/>
<@namespace_begin package.path/>

<#assign needsParameterProvider=explicitParameters?has_content/>
<#assign hasNonVirtualField=sql_table_has_non_virtual_field(fieldList)/>
<#if docComments??>
<@doc_comments docComments/>
</#if>
class ${name} : public ::zserio::AllocatorHolder<${types.allocator.default}>
{
public:
<#if needsParameterProvider>
    struct Row;

    /** Interface for class which provides all explicit parameters of the table. */
    class IParameterProvider
    {
    public:
        /** Default destructor. */
        virtual ~IParameterProvider() = default;

    <#list explicitParameters as parameter>
            <#if !parameter?is_first>

            </#if>
        /**
         * Gets the explicit parameter ${parameter.expression}.
         *
         * /param currentRow Current row of the table which can be used during parameter calculation.
         *
         * /return The the explicit parameter ${parameter.expression}.
         */
        virtual <@sql_parameter_provider_return_type parameter/> <@sql_parameter_provider_getter_name parameter/>(<#rt>
                <#lt>const ::zserio::View<Row>& currentRow) = 0;
    </#list>
    };

</#if>
    /** Class which describes one row in the table. Contains only data. See View<Row> for View. */
    struct Row
    {
        Row();
        explicit Row(const AllocatorType& allocator);

<#list fieldList as field>
        <@optional_type_name field.typeInfo.typeFullName/> <@sql_row_member_name field/>;
</#list>
    };

    /** Reader abstraction which is used for reading rows from the table. */
    class Reader : public ::zserio::AllocatorHolder<${types.allocator.default}>
    {
    public:
        /** Default destructor. */
        ~Reader() = default;

        /** Disables copy constructor. */
        Reader(const Reader&) = delete;
        /** Disables assignment operator. */
        Reader& operator=(const Reader&) = delete;

        /** Disables move constructor. */
        Reader(Reader&&) = default;
        /** Disables move assignment operator. */
        Reader& operator=(Reader&&) = delete;

        /**
         * Checks if there is next row available for reading.
         *
         * \return True if row for reading is available, otherwise false.
         */
        bool hasNext() const noexcept;

        /**
         * Reads next row from the table.
         *
         * \return Read row.
         */
        ::zserio::View<Row> next(Row& row);

    private:
        explicit Reader(::zserio::SqliteConnection& db, <#rt>
                <#lt><#if needsParameterProvider>IParameterProvider& parameterProvider, </#if><#rt>
                <#lt>const ::std::array<bool, ${fieldList?size}>& columnsMapping,
                ::std::string_view sqlQuery, const AllocatorType& allocator);
        friend class ${name};

        void makeStep();

<#if needsParameterProvider>
        IParameterProvider& m_parameterProvider;
</#if>
        ::std::array<bool, ${fieldList?size}> m_columnsMapping;
        ::std::unique_ptr<sqlite3_stmt, ::zserio::SqliteFinalizer> m_stmt;
        int m_lastResult;
    };

    /**
     * Constructor from database connection, table name and attached database name.
     *
     * \param db Database connection where the table is located.
     * \param tableName Table name.
     * \param attachedDbName Name of the attached database where table has been relocated.
     * \param allocator Allocator to construct from.
     */
    ${name}(::zserio::SqliteConnection& db, ::std::string_view tableName,
            ::std::string_view attachedDbName = ::std::string_view(),
            const AllocatorType& allocator = AllocatorType());

    /**
     * Constructor from database connection and table name.
     *
     * \param db Database connection where the table is located.
     * \param tableName Table name.
     * \param allocator Allocator to construct from.
     */
    ${name}(::zserio::SqliteConnection& db, ::std::string_view tableName, const AllocatorType& allocator);

    /** Default destructor. */
    ~${name}() = default;

    /** Disables copy constructor. */
    ${name}(const ${name}&) = delete;
    /** Disables assignment operator. */
    ${name}& operator=(const ${name}&) = delete;

    /** Disables move constructor. */
    ${name}(${name}&&) = delete;
    /** Disables move assignment operator. */
    ${name}& operator=(${name}&&) = delete;

    /**
     * Creates the table using database connection given by constructor.
     */
    void createTable();
<#if sql_table_has_non_virtual_field(fieldList) && isWithoutRowId>

    /**
     * Creates the table as ordinary row id using database connection given by constructor.
     *
     * The method creates the table as ordinary row id even if it is specified as without row id in schema.
     */
    void createOrdinaryRowIdTable();
</#if>

    /**
     * Deletes the table using database connection given by constructor.
     */
    void deleteTable();

    /**
     * Creates the table reader for given SQL condition.
     *
        <#if needsParameterProvider>
     * \param parameterProvider Explicit parameter provider to be used during reading.
        </#if>
     * \param columns Column names to use in select statement. If omitted or empty, all columns are used.
     * \param condition SQL condition to use.
     *
     * \return Created table reader.
     */
    /** \{ */
    Reader createReader(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if><#rt>
            <#lt>::std::string_view condition = ::std::string_view()) const;

    Reader createReader(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if><#rt>
            <#lt>::zserio::Span<const ${types.string.name}> columns,
            ::std::string_view condition = ::std::string_view()) const;
    /** \} */

    /**
     * Writes rows to the table.
     *
<#if needsParameterProvider>
     * \param parameterProvider Explicit parameter provider to be used during reading.
</#if>
     * \param rows Table rows to write.
     * \param columns Column names to write. If omitted or empty, all columns are used.
     */
    void write(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if><#rt>
            <#lt>::zserio::Span<Row> rows, ::zserio::Span<const ${types.string.name}> columns = {});

    /**
     * Updates row of the table.
     *
<#if needsParameterProvider>
     * \param parameterProvider Explicit parameter provider to be used during reading.
</#if>
     * \param row Table row to update.
     * \param columns Column names to update. If omitted or empty, all columns are used.
     * \param whereCondition SQL where condition to use.
     */
    /** \{ */
    void update(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if>Row& row, <#rt>
            <#lt>::std::string_view whereCondition);

    void update(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if>Row& row,
             ::zserio::Span<const ${types.string.name}> columns, ::std::string_view whereCondition);
    /** \} */

    /**
     * Validates the table.
     *
     * Validation consists of the following validation tests:
     *
     * 1. Table schema validation.
     *
     *    Table schema is read from database and checked according to the schema. It's checked if number of
     *    columns are correct and if each column has expected type and expected 'isNotNull' and 'isPrimaryKey'
     *    flags.
     *
     * 2. Validation of column values.
     *
     *    Each blob or integer value stored in table is read from database and checked. Blobs are read from
     *    the bit stream and written again. Then read bit stream is binary compared with the written stream.
     *    Integer values are checked according to their boundaries specified in schema.
     *
     * \param validationObserver Validation observer from which users can obtain validation results.
<#if needsParameterProvider>
     * \param parameterProvider Explicit parameter provider to be used during reading.
</#if>
     * \param continueValidation True if validation shall continue to validate next tables.
     */
    bool validate(::zserio::IValidationObserver& validationObserver<#rt>
            <#lt><#if needsParameterProvider>, IParameterProvider& parameterProvider</#if>, bool& continueValidation);

    static constexpr ::std::array<::std::string_view, ${fieldList?size}> columnNames =
    {{
<#list fieldList as field>
        "${field.name}"<#sep>,</#sep>
</#list>
    }};

private:
    bool validateSchema(::zserio::IValidationObserver& validationObserver);
<#list fieldList as field>
    bool validateColumn${field.name?cap_first}(::zserio::IValidationObserver& validationObserver,
            ::zserio::ValidationSqliteUtil<${types.allocator.default}>::TableSchema& tableSchema,
            bool& continueValidation);
</#list>
<#if hasNonVirtualField>

    <#list fieldList as field>
    bool validateType${field.name?cap_first}(::zserio::IValidationObserver& validationObserver,
            sqlite3_stmt* statement, bool& continueValidation);
    </#list>

    <#list fieldList as field>
        <#if field.sqlTypeData.isBlob>
    bool validateBlob${field.name?cap_first}(::zserio::IValidationObserver& validationObserver,
            sqlite3_stmt* statement, Row& row<#if needsParameterProvider>, IParameterProvider& parameterProvider</#if>,
            bool& continueValidation);
        <#else>
    bool validateField${field.name?cap_first}(::zserio::IValidationObserver& validationObserver,
            sqlite3_stmt* statement, Row& row<#if needsParameterProvider>, IParameterProvider& parameterProvider</#if>,
            bool& continueValidation);
        </#if>
    </#list>

    <@vector_type_name types.string.name/> getRowKeyValuesHolder(sqlite3_stmt* statement);
    <@vector_type_name "::std::string_view"/> getRowKeyValues(
            const <@vector_type_name types.string.name/>& rowKeyValuesHolder);
</#if>

    void writeRow(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if>Row& row,
            const ::std::array<bool, ${fieldList?size}>& columnsMapping, sqlite3_stmt& statement);

    void appendCreateTableToQuery(${types.string.name}& sqlQuery) const;
    void appendTableNameToQuery(${types.string.name}& sqlQuery) const;

    ::zserio::SqliteConnection& m_db;
    ::std::string_view m_name;
    ::std::string_view m_attachedDbName;
};
<@namespace_end package.path/>
<@namespace_begin ["zserio"]/>

template<>
class View<${fullName}::Row>
{
public:
    explicit View(const ${fullName}::Row& row<#if needsParameterProvider>, ${fullName}::IParameterProvider& parameterProvider</#if>);
<#list fieldList>

    <#items as field>
    <@sql_row_view_field_type_name field/> ${field.getterName}();
    </#items>

</#list>
    const ${fullName}::Row& zserioData() const;

private:
    const ${fullName}::Row* m_row;
<#if needsParameterProvider>
    ${fullName}::IParameterProvider& m_parameterProvider;
</#if>
};
<@namespace_end ["zserio"]/>

<@include_guard_end package.path, name/>
