<#include "FileHeader.inc.ftl">
<#include "Sql.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/CppRuntimeException.h>
#include <zserio/SerializeUtil.h>
#include <zserio/SqliteException.h>
<#assign hasNonVirtualField=sql_table_has_non_virtual_field(fields)/>
<#if hasNonVirtualField>
#include <algorithm>
#include <zserio/BitFieldUtil.h>
</#if>
<@type_includes types.bitBuffer/>
<@system_includes cppSystemIncludes/>

<@user_include package.path, "${name}.h"/>
<@user_includes cppUserIncludes, false/>
<@namespace_begin package.path/>

namespace
{

::std::array<bool, ${fields?size}> createColumnsMapping(::zserio::Span<const ::std::string> columns)
{
    if (columns.empty())
    {
        static constexpr ::std::array<bool, ${fields?size}> allColumns = {<#rt>
                <#lt><#list fields as field>true<#sep>, </#sep></#list>};
        return allColumns;
    }

    ::std::array<bool, ${fields?size}> columnsMapping = {};
    for (const ::std::string& columnName : columns)
    {
        const auto it = ::std::find(${name}::columnNames.begin(), ${name}::columnNames.end(), columnName);
        if (it == ${name}::columnNames.end())
        {
            throw ::zserio::SqliteException("Column name '") << columnName
                    << "' doesn't exist in '${name}'!";
        }
        columnsMapping[static_cast<size_t>(it - ${name}::columnNames.begin())] = true;
    }

    return columnsMapping;
}

void appendColumnsToQuery(${types.string.name}& sqlQuery, ::std::array<bool, ${fields?size}> columnsMapping)
{
    bool isFirst = true;
    for (size_t i = 0; i < columnsMapping.size(); ++i)
    {
        if (columnsMapping[i])
        {
            if (isFirst)
            {
                isFirst = false;
            }
            else
            {
                sqlQuery += ", ";
            }
            sqlQuery += ${name}::columnNames[i];
        }
    }
}

void appendWriteParametersToQuery(
        ${types.string.name}& sqlQuery, ::std::array<bool, ${fields?size}> columnsMapping)
{
    bool isFirst = true;
    for (bool columnUsed : columnsMapping)
    {
        if (columnUsed)
        {
            if (isFirst)
            {
                isFirst = false;
            }
            else
            {
                sqlQuery += ", ";
            }
            sqlQuery += "?";
        }
    }
}

void appendUpdateParametersToQuery(
        ${types.string.name}& sqlQuery, ::std::array<bool, ${fields?size}> columnsMapping)
{
    bool isFirst = true;
    for (size_t i = 0; i < columnsMapping.size(); ++i)
    {
        if (columnsMapping[i])
        {
            if (isFirst)
            {
                isFirst = false;
            }
            else
            {
                sqlQuery += ", ";
            }
            sqlQuery += ${name}::columnNames[i];
            sqlQuery += "=?";
        }
    }
}

} // namespace

<#assign needsParameterProvider=explicitParameters?has_content/>
<#assign hasPrimaryKeyField=false/>
<#list fields as field>
    <#if field.isPrimaryKey>
        <#assign hasPrimaryKeyField=true/>
        <#break>
    </#if>
</#list>
${name}::${name}(::zserio::SqliteConnection& db, ::std::string_view tableName,
        ::std::string_view attachedDbName, const AllocatorType& allocator) :
        ::zserio::AllocatorHolder<AllocatorType>(allocator),
        m_db(db), m_name(tableName), m_attachedDbName(attachedDbName)
{
}

${name}::${name}(::zserio::SqliteConnection& db, ::std::string_view tableName,
        const AllocatorType& allocator) :
        ${name}(db, tableName, ::std::string_view(), allocator)
{
}

void ${name}::createTable()
{
    ${types.string.name} sqlQuery(get_allocator_ref());
    appendCreateTableToQuery(sqlQuery);
<#if hasNonVirtualField && isWithoutRowId>
    sqlQuery += " WITHOUT ROWID";
</#if>
    m_db.executeUpdate(sqlQuery);
}

<#if hasNonVirtualField && isWithoutRowId>
void ${name}::createOrdinaryRowIdTable()
{
    ${types.string.name} sqlQuery(get_allocator_ref());
    appendCreateTableToQuery(sqlQuery);
    m_db.executeUpdate(sqlQuery);
}

</#if>
void ${name}::deleteTable()
{
    ${types.string.name} sqlQuery(get_allocator_ref());
    sqlQuery += "DROP TABLE ";
    appendTableNameToQuery(sqlQuery);
    m_db.executeUpdate(sqlQuery);
}

${name}::Reader ${name}::createReader(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if><#rt>
        <#lt>::std::string_view condition) const
{
    return createReader(<#if needsParameterProvider>parameterProvider, </#if>{}, condition);
}

${name}::Reader ${name}::createReader(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if><#rt>
        <#lt>::zserio::Span<const ::std::string> columns,
        ::std::string_view condition) const
{
    const ::std::array<bool, ${fields?size}> columnsMapping = createColumnsMapping(columns);

    ${types.string.name} sqlQuery(get_allocator_ref());
    sqlQuery += "SELECT ";
    appendColumnsToQuery(sqlQuery, columnsMapping);
    sqlQuery += " FROM ";
    appendTableNameToQuery(sqlQuery);
    if (!condition.empty())
    {
        sqlQuery += " WHERE ";
        sqlQuery += condition;
    }

    return Reader(m_db, <#if needsParameterProvider>parameterProvider, </#if>columnsMapping, <#rt>
            <#lt>sqlQuery, get_allocator_ref());
}

${name}::Reader::Reader(::zserio::SqliteConnection& db, <#rt>
        <#if needsParameterProvider>IParameterProvider& parameterProvider, </#if><#t>
        <#lt>::std::array<bool, ${fields?size}> columnsMapping,
        ::std::string_view sqlQuery, const AllocatorType& allocator) :
        ::zserio::AllocatorHolder<AllocatorType>(allocator),
<#if needsParameterProvider>
        m_parameterProvider(parameterProvider),
</#if>
        m_columnsMapping(columnsMapping),
        m_stmt(db.prepareStatement(sqlQuery))
{
    makeStep();
}

bool ${name}::Reader::hasNext() const noexcept
{
    return m_lastResult == SQLITE_ROW;
}

<#macro view_parameters field parameterProviderVarName rowVarName="rowView">
    <#if field.typeParameters?has_content>
        <#list field.typeParameters as parameter>
            , <#t>
            <#if parameter.isExplicit>
                ${parameterProviderVarName}.<@sql_parameter_provider_getter_name parameter/>(${rowVarName})<#t>
            <#else>
                <#if parameter.typeInfo.isNumeric>
                ${parameter.typeInfo.typeFullName}(static_cast<${parameter.typeInfo.typeFullName}::ValueType>(<#t>
                </#if>
                ${parameter.expression}<#t>
                <#if parameter.typeInfo.isNumeric>
                ))<#t>
                </#if>
            </#if>
        </#list>
    <#elseif field.typeInfo.isDynamicBitField>
        , static_cast<uint8_t>(${field.dynamicBitFieldLength.expression})<#t>
    </#if>
</#macro>
::zserio::View<${name}::Row> ${name}::Reader::next(${name}::Row& row)
{
    if (!hasNext())
    {
        throw ::zserio::SqliteException("${name}::Reader::next: next row is not available: ") <<
                ::zserio::SqliteErrorCode(m_lastResult);
    }

    ::zserio::View rowView(row<#if needsParameterProvider>, m_parameterProvider</#if>);
    int index = 0;
<#list fields as field>
    // field ${field.name}
    if (m_columnsMapping[${field?index}])
    {
        if (sqlite3_column_type(m_stmt.get(), index) != SQLITE_NULL)
        {
    <#if field.sqlTypeData.isBlob>
            const void* blobDataPtr = sqlite3_column_blob(m_stmt.get(), index);
            const int blobDataLength = sqlite3_column_bytes(m_stmt.get(), index);
            ::zserio::Span<const uint8_t> blobData(static_cast<const uint8_t*>(blobDataPtr),
                    static_cast<size_t>(blobDataLength));
            row.<@sql_row_member_name field/>.emplace(get_allocator_ref());
            zserio::deserializeFromBytes(blobData, *row.<@sql_row_member_name field/><#rt>
                    <#lt><@view_parameters field, "m_parameterProvider"/>);
    <#elseif field.sqlTypeData.isInteger>
            const int64_t intValue = sqlite3_column_int64(m_stmt.get(), index);
        <#if field.typeInfo.isEnum>
            row.<@sql_row_member_name field/> = ::zserio::valueToEnum<${field.typeInfo.typeFullName}>(<#rt>
                    static_cast<${field.underlyingTypeInfo.typeFullName}::ValueType>(intValue));
        <#elseif field.typeInfo.isBitmask>
            row.<@sql_row_member_name field/> = ${field.typeInfo.typeFullName}(
                    static_cast<${field.underlyingTypeInfo.typeFullName}::ValueType>(intValue));
        <#elseif field.typeInfo.isBoolean>
            row.<@sql_row_member_name field/> = intValue != 0;
        <#else>
            row.<@sql_row_member_name field/>= static_cast<${field.typeInfo.typeFullName}::ValueType>(intValue);
        </#if>
    <#elseif field.sqlTypeData.isReal>
            const double doubleValue = sqlite3_column_double(m_stmt.get(), index);
            row.<@sql_row_member_name field/> = static_cast<${field.typeInfo.typeFullName}::ValueType>(doubleValue);
    <#else>
            const unsigned char* textValue = sqlite3_column_text(m_stmt.get(), index);
            row.<@sql_row_member_name field/> = ${field.typeInfo.typeFullName}(
                    reinterpret_cast<const char*>(textValue), get_allocator_ref());
    </#if>
        }
        ++index;
    }
</#list>

    makeStep();

    return rowView;
}

void ${name}::Reader::makeStep()
{
    m_lastResult = sqlite3_step(m_stmt.get());
    if (m_lastResult != SQLITE_ROW && m_lastResult != SQLITE_DONE)
    {
        throw ::zserio::SqliteException("${name}::Read: sqlite3_step() failed: ") <<
                ::zserio::SqliteErrorCode(m_lastResult);
    }
}

void ${name}::write(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if><#rt>
        <#lt>::zserio::Span<Row> rows,
        ::zserio::Span<const ::std::string> columns)
{
    // assemble sql query
    const ::std::array<bool, ${fields?size}> columnsMapping = createColumnsMapping(columns);
    ${types.string.name} sqlQuery(get_allocator_ref());
    sqlQuery += "INSERT INTO ";
    appendTableNameToQuery(sqlQuery);
    sqlQuery += "(";
    appendColumnsToQuery(sqlQuery, columnsMapping);
    sqlQuery += ") VALUES (";
    appendWriteParametersToQuery(sqlQuery, columnsMapping);
    sqlQuery += ");";

    // write rows
    const bool wasTransactionStarted = m_db.startTransaction();
    ::std::unique_ptr<sqlite3_stmt, ::zserio::SqliteFinalizer> statement(m_db.prepareStatement(sqlQuery));

    for (Row& row : rows)
    {
        writeRow(<#if needsParameterProvider>parameterProvider, </#if>row, columnsMapping, *statement);
        int result = sqlite3_step(statement.get());
        if (result != SQLITE_DONE)
        {
            throw ::zserio::SqliteException("Write: sqlite3_step() failed: ") <<
                    ::zserio::SqliteErrorCode(result);
        }

        result = sqlite3_clear_bindings(statement.get());
        if (result != SQLITE_OK)
        {
            throw ::zserio::SqliteException("Write: sqlite3_clear_bindings() failed: ") <<
                    ::zserio::SqliteErrorCode(result);
        }

        result = sqlite3_reset(statement.get());
        if (result != SQLITE_OK)
        {
            throw ::zserio::SqliteException("Write: sqlite3_reset() failed: ") <<
                    ::zserio::SqliteErrorCode(result);
        }
    }

    m_db.endTransaction(wasTransactionStarted);
}

void ${name}::update(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if>Row& row, <#rt>
        <#lt>::std::string_view whereCondition)
{
    update(<#if needsParameterProvider>parameterProvider, </#if>row, {}, whereCondition);
}

void ${name}::update(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if>Row& row,
        ::zserio::Span<const ::std::string> columns, ::std::string_view whereCondition)
{
    // assemble sql query
    const ::std::array<bool, ${fields?size}> columnsMapping = createColumnsMapping(columns);
    ${types.string.name} sqlQuery(get_allocator_ref());
    sqlQuery += "UPDATE ";
    appendTableNameToQuery(sqlQuery);
    sqlQuery += " SET ";
    appendUpdateParametersToQuery(sqlQuery, columnsMapping);
    sqlQuery += " WHERE ";
    sqlQuery += whereCondition;

    // update row
    ::std::unique_ptr<sqlite3_stmt, ::zserio::SqliteFinalizer> statement(m_db.prepareStatement(sqlQuery));
    writeRow(<#if needsParameterProvider>parameterProvider, </#if>row, columnsMapping, *statement);
    const int result = sqlite3_step(statement.get());
    if (result != SQLITE_DONE)
    {
        throw ::zserio::SqliteException("Update: sqlite3_step() failed: ") << ::zserio::SqliteErrorCode(result);
    }
}

bool ${name}::validate(::zserio::IValidationObserver& validationObserver<#rt>
        <#lt><#if needsParameterProvider>, IParameterProvider& parameterProvider</#if>, bool& continueValidation)
{
    const size_t numberOfRows = <#if hasNonVirtualField>::zserio::ValidationSqliteUtil<${types.allocator.default}>::getNumberOfTableRows(
            m_db, m_attachedDbName, m_name, get_allocator_ref());<#else>0;</#if>
    continueValidation = true;
    if (!validationObserver.beginTable(m_name, numberOfRows))
    {
        return false;
    }

    size_t numberOfValidatedRows = 0;
<#if hasNonVirtualField>
    if (validateSchema(validationObserver))
    {
        ${types.string.name} sqlQuery{get_allocator_ref()};
        sqlQuery += "SELECT ";
    <#list fields as field>
        sqlQuery += "${field.name}<#if field?has_next || !hasPrimaryKeyField>, </#if>";
    </#list>
    <#if !hasPrimaryKeyField><#-- use rowid instead of primary key in getRowKeyValuesHolder -->
        sqlQuery += "rowid";
    </#if>
        sqlQuery += " FROM ";
        appendTableNameToQuery(sqlQuery);
        ::std::unique_ptr<sqlite3_stmt, ::zserio::SqliteFinalizer> statement(m_db.prepareStatement(sqlQuery));
        int result = SQLITE_OK;
        bool continueTableValidation = true;
        while ((result = sqlite3_step(statement.get())) == SQLITE_ROW && continueTableValidation)
        {
            ++numberOfValidatedRows;

    <#list fields as field>
            if (!validateType${field.name?cap_first}(validationObserver, statement.get(), continueValidation))
            {
                continue;
            }
    </#list>

            Row row;
    <#list fields as field>
        <#if field.sqlTypeData.isBlob>
            if (!validateBlob${field.name?cap_first}(validationObserver, statement.get(), row<#rt>
                    <#lt><#if needsParameterProvider>, parameterProvider</#if>, continueTableValidation))
            {
                continue;
            }
        <#else>
            if (!validateField${field.name?cap_first}(validationObserver, statement.get(), row<#rt>
                    <#lt><#if needsParameterProvider>, parameterProvider</#if>, continueTableValidation))
            {
                continue;
            }
        </#if>
    </#list>
        }
        if (result != SQLITE_DONE && (continueTableValidation || result != SQLITE_ROW))
        {
            throw ::zserio::SqliteException("Validate: sqlite3_step() failed: ") <<
                    ::zserio::SqliteErrorCode(result);
        }
    }
<#else>
    <#-- If the table has only virtual fields, skip everything except of schema checking. -->
    (void)validateSchema(validationObserver);
</#if>

    continueValidation = validationObserver.endTable(m_name, numberOfValidatedRows);

    return true;
}

bool ${name}::validateSchema(::zserio::IValidationObserver& validationObserver)
{
    ::zserio::ValidationSqliteUtil<${types.allocator.default}>::TableSchema tableSchema(
            get_allocator_ref());
    ::zserio::ValidationSqliteUtil<${types.allocator.default}>::getTableSchema(
            m_db, m_attachedDbName, m_name, tableSchema, get_allocator_ref());

    bool result = true;
    bool continueValidation = true;
<#list fields as field>

    if (<#if !field?is_first>continueValidation && </#if>!validateColumn${field.name?cap_first}(
            validationObserver, tableSchema, continueValidation))
    {
        result = false;
    }
</#list>

    if (!tableSchema.empty())
    {
        for (auto it = tableSchema.begin(); it != tableSchema.end() && continueValidation; ++it)
        {
            const auto& columnName = it->first;
            const auto& columnType = it->second.type;
            ${types.string.name} errorMessage = ${types.string.name}(
                    "superfluous column ", get_allocator_ref());
            errorMessage += m_name;
            errorMessage += ".";
            errorMessage += columnName;
            errorMessage += " of type ";
            errorMessage += columnType;
            errorMessage += " encountered";
            continueValidation = validationObserver.reportError(m_name, columnName,
                    ::zserio::Span<::std::string_view>(), ::zserio::IValidationObserver::COLUMN_SUPERFLUOUS,
                    errorMessage);
        }
        result = false;
    }

    return result;
}
<#list fields as field>

bool ${name}::validateColumn${field.name?cap_first}(::zserio::IValidationObserver& validationObserver,
        ::zserio::ValidationSqliteUtil<${types.allocator.default}>::TableSchema& tableSchema,
        bool& continueValidation)
{
    auto search = tableSchema.find(${types.string.name}("${field.name}", get_allocator_ref()));
    <#-- if column is virtual, it can be hidden -->
    if (search == tableSchema.end()<#if field.isVirtual> &&
            !::zserio::ValidationSqliteUtil<${types.allocator.default}>::isColumnInTable(
                    m_db, m_attachedDbName, m_name, "${field.name}", get_allocator_ref())</#if>)
    {
        continueValidation = validationObserver.reportError(m_name, "${field.name}",
                ::zserio::Span<const ::std::string_view>(), ::zserio::IValidationObserver::COLUMN_MISSING,
                "column ${name}.${field.name} is missing");
        return false;
    }

        <#-- SQLite does not maintain column properties for virtual tables columns or for virtual columns -->
    <#if !virtualTableUsing?? && !field.isVirtual>
    const auto column = search->second;<#-- copy the column since it will be erased from schema -->
    (void)tableSchema.erase(search);

    if (column.type != "${field.sqlTypeData.name}")
    {
        ${types.string.name} errorMessage = ${types.string.name}(
                "column ${name}.${field.name} has type '", get_allocator_ref());
        errorMessage += column.type;
        errorMessage += "' but '${field.sqlTypeData.name}' is expected";
        continueValidation = validationObserver.reportError(m_name, "${field.name}",
                ::zserio::Span<const ::std::string_view>(),
                ::zserio::IValidationObserver::INVALID_COLUMN_TYPE, errorMessage);
        return false;
    }

    if (<#if field.isNotNull>!</#if>column.isNotNull)
    {
        continueValidation = validationObserver.reportError(m_name, "${field.name}",
                ::zserio::Span<::std::string_view>(), ::zserio::IValidationObserver::INVALID_COLUMN_CONSTRAINT,
                "column ${name}.${field.name} is <#if !field.isNotNull>NOT </#if>NULL-able, "
                "but the column is expected to be <#if field.isNotNull>NOT </#if>NULL-able");
        return false;
    }

    if (<#if field.isPrimaryKey>!</#if>column.isPrimaryKey)
    {
        continueValidation = validationObserver.reportError(m_name, "${field.name}",
                ::zserio::Span<::std::string_view>(), ::zserio::IValidationObserver::INVALID_COLUMN_CONSTRAINT,
                "column ${name}.${field.name} is <#if field.isPrimaryKey>not </#if>primary key, "
                "but the column is expected <#if !field.isPrimaryKey>not </#if>to be primary key");
        return false;
    }
    <#else>
    if (search != tableSchema.end())
    {
        (void)tableSchema.erase(search);
    }
    </#if>

    return true;
}
</#list>
<#if hasNonVirtualField>
    <#list fields as field>

<#macro sqlite_type_field field>
    <#if field.sqlTypeData.isInteger>
        SQLITE_INTEGER<#t>
    <#elseif field.sqlTypeData.isReal>
        SQLITE_FLOAT<#t>
    <#elseif field.sqlTypeData.isBlob>
        SQLITE_BLOB<#t>
    <#else>
        SQLITE_TEXT<#t>
    </#if>
</#macro>
bool ${name}::validateType${field.name?cap_first}(::zserio::IValidationObserver& validationObserver,
        sqlite3_stmt* statement, bool& continueValidation)
{
    const int type = sqlite3_column_type(statement, ${field?index});
    if (type == SQLITE_NULL)
    {
        return true;
    }

    if (type != <@sqlite_type_field field/>)
    {
        const auto rowKeyValuesHolder = getRowKeyValuesHolder(statement);
        ${types.string.name} errorMessage = ${types.string.name}(
                "Column ${name}.${field.name} type check failed (", get_allocator_ref());
        errorMessage += ::zserio::ValidationSqliteUtil<${types.allocator.default}>::sqliteColumnTypeName(type);
        errorMessage += " doesn't match to ";
        errorMessage += ::zserio::ValidationSqliteUtil<${types.allocator.default}>::sqliteColumnTypeName(<@sqlite_type_field field/>);
        errorMessage += ")!";
        continueValidation = validationObserver.reportError(m_name, "${field.name}",
                getRowKeyValues(rowKeyValuesHolder), ::zserio::IValidationObserver::INVALID_COLUMN_TYPE,
                errorMessage);
        return false;
    }

    return true;
}
    </#list>
    <#list fields as field>

        <#if field.sqlTypeData.isBlob>
bool ${name}::validateBlob${field.name?cap_first}(::zserio::IValidationObserver& validationObserver,
        sqlite3_stmt* statement, Row& row<#rt>
        <#lt><#if needsParameterProvider>, IParameterProvider& parameterProvider</#if>, bool& continueValidation)
{
    const void* blobDataPtr = sqlite3_column_blob(statement, ${field?index});
    if (blobDataPtr == nullptr)
    {
        return true;
    }

    ::zserio::View rowView(row<#if needsParameterProvider>, parameterProvider</#if>);
    try
    {
        const int blobDataLength = sqlite3_column_bytes(statement, ${field?index});
        zserio::BitStreamReader reader(static_cast<const uint8_t*>(blobDataPtr),
                static_cast<size_t>(blobDataLength));
        row.<@sql_row_member_name field/>.emplace(get_allocator_ref());
        auto blobView = zserio::detail::read(reader, *row.<@sql_row_member_name field/><#rt>
                <#lt><@view_parameters field, "parameterProvider"/>);
        auto bitBuffer = ::zserio::serialize(blobView);
        if (reader.getBitPosition() != bitBuffer.getBitSize())
        {
            const auto rowKeyValuesHolder = getRowKeyValuesHolder(statement);
            ${types.string.name} errorMessage = ${types.string.name}(
                    "Blob binary compare failed because of length (", get_allocator_ref());
            errorMessage += ::zserio::toString(reader.getBitPosition(), get_allocator_ref());
            errorMessage += " != ";
            errorMessage += ::zserio::toString(bitBuffer.getBitSize(), get_allocator_ref());
            errorMessage += ")";
            continueValidation = validationObserver.reportError(m_name,
                    "${field.name}", getRowKeyValues(rowKeyValuesHolder),
                    ::zserio::IValidationObserver::BLOB_COMPARE_FAILED, errorMessage);
            return false;
        }
    }
    catch (const ::zserio::CppRuntimeException& exception)
    {
        const auto rowKeyValuesHolder = getRowKeyValuesHolder(statement);
        continueValidation = validationObserver.reportError(m_name, "${field.name}",
                getRowKeyValues(rowKeyValuesHolder), ::zserio::IValidationObserver::BLOB_PARSE_FAILED,
                exception.what());
        return false;
    }

    return true;
}
        <#else>
<#macro range_check_field name field types indent>
    <#local I>${""?left_pad(indent * 4)}</#local>
${I}if (<#if field.sqlRangeCheckData.checkLowerBound>rangeCheckValue < lowerBound || </#if>rangeCheckValue > upperBound)
${I}{
${I}    const auto rowKeyValuesHolder = getRowKeyValuesHolder(statement);
${I}    ${types.string.name} errorMessage = ${types.string.name}("Value ", get_allocator_ref());
${I}    errorMessage += ::zserio::toString(rangeCheckValue, get_allocator_ref());
${I}    errorMessage += " of ${name}.${field.name} exceeds the range of ";
${I}    errorMessage += ::zserio::toString(lowerBound, get_allocator_ref());
${I}    errorMessage += "..";
${I}    errorMessage += ::zserio::toString(upperBound, get_allocator_ref());
${I}    continueValidation = validationObserver.reportError(m_name, "${field.name}",
${I}            getRowKeyValues(rowKeyValuesHolder),
${I}            ::zserio::IValidationObserver::VALUE_OUT_OF_RANGE, errorMessage);
${I}    return false;
${I}}
</#macro>
bool ${name}::validateField${field.name?cap_first}(::zserio::IValidationObserver&<#rt>
        <#lt><#if field.sqlRangeCheckData?? || field.typeInfo.isEnum> validationObserver</#if>,
        sqlite3_stmt* statement, Row& row<#if needsParameterProvider>, IParameterProvider&<#rt>
        <#lt><#if field.sqlRangeCheckData?? && field.sqlRangeCheckData.bitFieldLength??> parameterProvider</#if></#if>,
        bool&<#if field.sqlRangeCheckData?? || field.typeInfo.isEnum> continueValidation</#if>)
{
    if (sqlite3_column_type(statement, ${field?index}) == SQLITE_NULL)
    {
        return true;
    }

                <#if field.sqlTypeData.isInteger>
    const int64_t intValue = sqlite3_column_int64(statement, ${field?index});
                    <#if field.sqlRangeCheckData??>
    // range check
    const ${field.sqlRangeCheckData.typeInfo.typeFullName} rangeCheckValue = static_cast<${field.sqlRangeCheckData.typeInfo.typeFullName}::ValueType>(intValue);
                        <#if field.sqlRangeCheckData.bitFieldLength??>
    try
    {
        ::zserio::View rowView(row<#if needsParameterProvider>, parameterProvider</#if>);
        const ::zserio::BitSize bitFieldLength = static_cast<::zserio::BitSize>(${field.sqlRangeCheckData.bitFieldLength});
        const ${field.sqlRangeCheckData.typeInfo.typeFullName} lowerBound = static_cast<${field.sqlRangeCheckData.typeInfo.typeFullName}::ValueType>(
                ::zserio::NumericLimits<${field.typeInfo.typeFullName}>::min(bitFieldLength));
        const ${field.sqlRangeCheckData.typeInfo.typeFullName} upperBound = static_cast<${field.sqlRangeCheckData.typeInfo.typeFullName}::ValueType>(
                ::zserio::NumericLimits<${field.typeInfo.typeFullName}>::max(bitFieldLength));
        <@range_check_field name, field, types, 2/>
    }
    catch (const ::zserio::CppRuntimeException& exception)
    {
        const auto rowKeyValuesHolder = getRowKeyValuesHolder(statement);
        continueValidation = validationObserver.reportError(m_name, "${field.name}",
                getRowKeyValues(rowKeyValuesHolder),
                ::zserio::IValidationObserver::INVALID_VALUE,
                exception.what());
        return false;
    }
                        <#else>
    const ${field.sqlRangeCheckData.typeInfo.typeFullName} lowerBound = ${field.sqlRangeCheckData.lowerBound};
    const ${field.sqlRangeCheckData.typeInfo.typeFullName} upperBound = ${field.sqlRangeCheckData.upperBound};
    <@range_check_field name, field, types, 1/>
                        </#if>

                    </#if>
                    <#if field.typeInfo.isEnum>
    try
    {
        row.<@sql_row_member_name field/> = ::zserio::valueToEnum<${field.typeInfo.typeFullName}>(<#rt>
                <#lt>static_cast<${field.underlyingTypeInfo.typeFullName}::ValueType>(intValue));
    }
    catch (const ::zserio::CppRuntimeException&)
    {
        const auto rowKeyValuesHolder = getRowKeyValuesHolder(statement);
        ${types.string.name} errorMessage = ${types.string.name}("Enumeration value ", get_allocator_ref());
        errorMessage += ::zserio::toString(intValue, get_allocator_ref());
        errorMessage += " of ${name}.${field.name} is not valid!";
        continueValidation = validationObserver.reportError(m_name, "${field.name}",
                getRowKeyValues(rowKeyValuesHolder),
                ::zserio::IValidationObserver::INVALID_VALUE, errorMessage);
        return false;
    }
                    <#elseif field.typeInfo.isBitmask>
    const ${field.typeInfo.typeFullName} bitmaskValue = ${field.typeInfo.typeFullName}(static_cast<${field.underlyingTypeInfo.typeFullName}::ValueType>(intValue));
    row.<@sql_row_member_name field/> = bitmaskValue;
                    <#elseif field.typeInfo.isBoolean>
    row.<@sql_row_member_name field/> = intValue != 0;
                    <#else>
    row.<@sql_row_member_name field/> = static_cast<${field.typeInfo.typeFullName}::ValueType>(intValue);
                    </#if>
                <#elseif field.sqlTypeData.isReal>
    const double doubleValue = sqlite3_column_double(statement, ${field?index});
    row.<@sql_row_member_name field/> = static_cast<${field.typeInfo.typeFullName}::ValueType>(doubleValue);
                <#else>
    const unsigned char* textValue = sqlite3_column_text(statement, ${field?index});
    row.<@sql_row_member_name field/> = ${field.typeInfo.typeFullName}(
            reinterpret_cast<const char*>(textValue), get_allocator_ref());
                </#if>

    return true;
}
        </#if>
    </#list>

    <#assign needsStatementArgument=false/>
    <#if hasPrimaryKeyField>
        <#list fields as field>
            <#if field.isPrimaryKey>
                <#if !field.sqlTypeData.isBlob>
                    <#assign needsStatementArgument=true/>
                    <#break>
                </#if>
             </#if>
        </#list>
    <#else>
        <#assign needsStatementArgument=true/>
    </#if>
<@vector_type_name types.string.name/> ${name}::getRowKeyValuesHolder(sqlite3_stmt*<#if needsStatementArgument> statement</#if>)
{
    <@vector_type_name types.string.name/> rowKeyValuesHolder{get_allocator_ref()};

    <#if hasPrimaryKeyField>
        <#list fields as field>
            <#if field.isPrimaryKey>
                <#if field.sqlTypeData.isBlob>
    rowKeyValuesHolder.emplace_back("BLOB");
                <#else>
    const unsigned char* strValue${field.name?cap_first} = sqlite3_column_text(statement, ${field?index});
    rowKeyValuesHolder.emplace_back(reinterpret_cast<const char*>(strValue${field.name?cap_first}));
                </#if>
            </#if>
        </#list>
    <#else>
    const unsigned char* strValueRowId = sqlite3_column_text(statement, ${fields?size});
    rowKeyValuesHolder.emplace_back(reinterpret_cast<const char*>(strValueRowId));
    </#if>

    return rowKeyValuesHolder;
}

<@vector_type_name "::std::string_view"/> ${name}::getRowKeyValues(
        const <@vector_type_name types.string.name/>& rowKeyValuesHolder)
{
    <@vector_type_name "::std::string_view"/> rowKeyValues{get_allocator_ref()};
    (void)::std::transform(rowKeyValuesHolder.begin(), rowKeyValuesHolder.end(),
            ::std::back_inserter(rowKeyValues),
            [](const ${types.string.name}& message) -> ::std::string_view { return message; });
    return rowKeyValues;
}
</#if>

void ${name}::writeRow(<#if needsParameterProvider>IParameterProvider& parameterProvider, </#if>Row& row,
        ::std::array<bool, ${fields?size}> columnsMapping, sqlite3_stmt& statement)
{
    int result = SQLITE_ERROR;

    zserio::View rowView(row<#if needsParameterProvider>, parameterProvider</#if>);
    int index = 1;
<#list fields as field>
    // field ${field.name}
    if (columnsMapping[${field?index}])
    {
        if (!row.<@sql_row_member_name field/>)
        {
            result = sqlite3_bind_null(&statement, index);
        }
        else
        {
    <#if field.sqlTypeData.isBlob>
            const ${field.typeInfo.typeFullName}& blob = *row.<@sql_row_member_name field/>;
            auto bitBuffer = zserio::serialize(blob, get_allocator_ref()<@view_parameters field, "parameterProvider"/>);
            result = sqlite3_bind_blob(&statement, index, bitBuffer.getBuffer(),
                    static_cast<int>(bitBuffer.getByteSize()), SQLITE_TRANSIENT);
    <#elseif field.sqlTypeData.isInteger>
            const int64_t intValue = static_cast<int64_t>(row.<@sql_row_member_name field/><#if field.typeInfo.isBitmask>->getValue()<#else>.value()</#if>);
            result = sqlite3_bind_int64(&statement, index, intValue);
    <#elseif field.sqlTypeData.isReal>
            const ${field.typeInfo.typeFullName} realValue = *row.<@sql_row_member_name field/>;
            result = sqlite3_bind_double(&statement, index, static_cast<double>(realValue));
    <#else>
            const ${field.typeInfo.typeFullName}& stringValue = *row.<@sql_row_member_name field/>;
            result = sqlite3_bind_text(&statement, index, stringValue.c_str(), -1, SQLITE_TRANSIENT);
    </#if>
        }
        if (result != SQLITE_OK)
        {
            throw ::zserio::SqliteException("${name}::WriteRow: sqlite3_bind() for field ${field.name} failed: ")
                    << ::zserio::SqliteErrorCode(result);
        }
        ++index;
    }
</#list>
}

void ${name}::appendCreateTableToQuery(${types.string.name}& sqlQuery) const
{
    sqlQuery += "CREATE <#if virtualTableUsing??>VIRTUAL </#if>TABLE ";
    appendTableNameToQuery(sqlQuery);
<#if virtualTableUsing??>
    sqlQuery += " USING ${virtualTableUsing}";
</#if>
<#if hasNonVirtualField || sqlConstraint??>
    sqlQuery += '(';
    <#assign firstNonVirtualField=true/>
    <#list fields as field>
        <#if !field.isVirtual>
    sqlQuery += "<#if !firstNonVirtualField>, </#if>${field.name}<#if needsTypesInSchema> ${field.sqlTypeData.name}</#if>";
            <#if field.sqlConstraint??>
    sqlQuery += ' ';
    sqlQuery += ${field.sqlConstraint};
            </#if>
            <#assign firstNonVirtualField=false/>
        </#if>
    </#list>
    <#if hasNonVirtualField && sqlConstraint??>
    sqlQuery += ", ";
    </#if>
    <#if sqlConstraint??>
    sqlQuery += ${sqlConstraint};
    </#if>
    sqlQuery += ')';
</#if>
}

void ${name}::appendTableNameToQuery(${types.string.name}& sqlQuery) const
{
    if (!m_attachedDbName.empty())
    {
        sqlQuery += m_attachedDbName;
        sqlQuery += '.';
    }
    sqlQuery += m_name;
}
<@namespace_end package.path/>
<@namespace_begin ["zserio"]/>

View<${fullName}::Row>::View(const ${fullName}::Row& row<#if needsParameterProvider>, ${fullName}::IParameterProvider& parameterProvider</#if>) :
        m_row(&row)<#if needsParameterProvider>,
        m_parameterProvider(parameterProvider)</#if>
{}
<#list fields as field>

<@sql_row_view_field_type_name field/> View<${fullName}::Row>::${field.getterName}()
{
    if (!m_row-><@sql_row_member_name field/>)
    {
        return <@sql_row_view_field_type_name field/>(std::nullopt, m_row-><@sql_row_member_name field/>.get_allocator());
    }

    <#if field.requiresOwnerContext || field.hasExplicitParameters || field.typeInfo.isDynamicBitField>
    View<${fullName}::Row>& rowView = *this;
    </#if>
    return <@sql_row_view_field_type_name field/>(std::in_place, m_row-><@sql_row_member_name field/>.get_allocator(),
            *m_row-><@sql_row_member_name field/><@view_parameters field, "m_parameterProvider", "rowView"/>);
}
</#list>

const ${fullName}::Row& View<${fullName}::Row>::zserioData() const
{
    return *m_row;
}
<@namespace_end ["zserio"]/>
