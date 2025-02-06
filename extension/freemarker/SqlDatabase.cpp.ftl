<#include "FileHeader.inc.ftl">
<#include "Sql.inc.ftl">
<@file_header generatorDescription/>
<#assign needsParameterProvider = sql_db_needs_parameter_provider(fields)/>

#include <zserio/SqliteException.h>
#include <zserio/ValidationSqliteUtil.h>

<@user_include package.path, "${name}.h"/>
<@namespace_begin package.path/>

<#assign hasWithoutRowIdTable=sql_db_has_without_rowid_table(fields)/>
${name}::${name}(const ${types.string.name}& dbFileName, const TRelocationMap& tableToDbFileNameRelocationMap,
        const AllocatorType& allocator) :
        ::zserio::AllocatorHolder<AllocatorType>(allocator),
        m_tableToAttachedDbNameRelocationMap(allocator)
{
    sqlite3 *internalConnection = nullptr;
    const int sqliteOpenMode = SQLITE_OPEN_URI | SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE;
    const int sqliteResult = sqlite3_open_v2(dbFileName.c_str(), &internalConnection, sqliteOpenMode, nullptr);
    m_db.reset(internalConnection, ::zserio::SqliteConnection::INTERNAL_CONNECTION);
    if (sqliteResult != SQLITE_OK)
    {
        throw ::zserio::SqliteException("${name}::${name}: can't open DB ") << dbFileName.c_str() << ": " <<
                ::zserio::SqliteErrorCode(sqliteResult);
    }

    <@map_type_name types.string.name types.string.name/> dbFileNameToAttachedDbNameMap(get_allocator_ref());
    for (const auto& relocation : tableToDbFileNameRelocationMap)
    {
        const ${types.string.name}& tableName = relocation.first;
        const ${types.string.name}& fileName = relocation.second;
        auto attachedDbIt = dbFileNameToAttachedDbNameMap.find(fileName);
        if (attachedDbIt == dbFileNameToAttachedDbNameMap.end())
        {
            ${types.string.name} attachedDbName =
                    ${types.string.name}(databaseName, get_allocator_ref()) + "_" + tableName;
            attachDatabase(fileName, attachedDbName);
            attachedDbIt = dbFileNameToAttachedDbNameMap.emplace(fileName, ::std::move(attachedDbName)).first;
        }
        auto emplaceResult = m_tableToAttachedDbNameRelocationMap.emplace(
                ${types.string.name}(tableName, get_allocator_ref()),
                ${types.string.name}(attachedDbIt->second, get_allocator_ref()));
        if (!emplaceResult.second)
        {
            throw ::zserio::SqliteException("${name}::${name}: can't insert ") << tableName <<
                    " into Database Relocation Map!";
        }
    }

    initTables();
}

${name}::${name}(const ${types.string.name}& dbFileName, const AllocatorType& allocator) :
        ${name}(dbFileName, TRelocationMap(allocator), allocator)
{}

${name}::${name}(sqlite3* externalConnection, const TRelocationMap& tableToAttachedDbNameRelocationMap,
        const AllocatorType& allocator) :
        ::zserio::AllocatorHolder<AllocatorType>(allocator),
        m_tableToAttachedDbNameRelocationMap(tableToAttachedDbNameRelocationMap, allocator)
{
    m_db.reset(externalConnection, ::zserio::SqliteConnection::EXTERNAL_CONNECTION);
    initTables();
}

${name}::${name}(sqlite3* externalConnection, const AllocatorType& allocator) :
        ${name}(externalConnection, TRelocationMap(allocator), allocator)
{}

${name}::~${name}()
{
    detachDatabases();
}

::zserio::SqliteConnection& ${name}::connection() noexcept
{
    return m_db;
}
<#list fields as field>

${field.typeInfo.typeFullName}& ${name}::<@sql_db_table_getter_name field/>() noexcept
{
    return *<@sql_field_member_name field/>;
}
</#list>

void ${name}::createSchema()
{
    const bool wasTransactionStarted = m_db.startTransaction();

<#list fields as field>
    <@sql_field_member_name field/>->createTable();
</#list>

    m_db.endTransaction(wasTransactionStarted);
}

void ${name}::createSchema(const <@set_type_name types.string.name/>&<#if hasWithoutRowIdTable> withoutRowIdTableNamesBlackList</#if>)
{
<#if hasWithoutRowIdTable>
    const bool wasTransactionStarted = m_db.startTransaction();

    <#list fields as field>
        <#if field.isWithoutRowIdTable>
    if (withoutRowIdTableNamesBlackList.find(${types.string.name}(
            tableNames[${field?index}], get_allocator_ref())) != withoutRowIdTableNamesBlackList.end())
    {
        <@sql_field_member_name field/>->createOrdinaryRowIdTable();
    }
    else
    {
        <@sql_field_member_name field/>->createTable();
    }
        <#else>
    <@sql_field_member_name field/>->createTable();
        </#if>
    </#list>

    m_db.endTransaction(wasTransactionStarted);
<#else>
    createSchema();
</#if>
}

void ${name}::deleteSchema()
{
    const bool wasTransactionStarted = m_db.startTransaction();

<#list fields as field>
    <@sql_field_member_name field/>->deleteTable();
</#list>

    m_db.endTransaction(wasTransactionStarted);
}

void ${name}::validate(::zserio::IValidationObserver& validationObserver<#rt>
        <#lt><#if needsParameterProvider>, IParameterProvider& parameterProvider</#if>)
{
    validationObserver.beginDatabase(${fields?size});
    bool continueValidation = true;
    size_t numberOfValidatedTables = 0;

<#list fields as field>
    if (<#if !field?is_first>continueValidation && </#if><@sql_field_member_name field/>->validate(validationObserver<#rt>
    <#if field.hasExplicitParameters>
            <#lt>,
            parameterProvider.<@sql_db_table_parameter_provider_getter field/><#rt>
    </#if>
        <#lt>, continueValidation))
    {
        ++numberOfValidatedTables;
    }
</#list>

    validationObserver.endDatabase(numberOfValidatedTables);
}

void ${name}::initTables()
{
    static const ::std::string_view EMPTY_STR = ::std::string_view();
<#list fields as field>
    <#if field?is_first>
    auto relocationIt = m_tableToAttachedDbNameRelocationMap.find(
            ${types.string.name}(tableNames[${field?index}], get_allocator_ref()));
    <#else>
    relocationIt = m_tableToAttachedDbNameRelocationMap.find(
            ${types.string.name}(tableNames[${field?index}], get_allocator_ref()));
    </#if>
    <@sql_field_member_name field/> = ::zserio::allocate_unique<${field.typeInfo.typeFullName}>(
            get_allocator_ref(), this->m_db, tableNames[${field?index}],
            relocationIt != m_tableToAttachedDbNameRelocationMap.end() ? relocationIt->second : EMPTY_STR,
            get_allocator_ref());
    <#if field?has_next>

    </#if>
</#list>
}

void ${name}::attachDatabase(::std::string_view fileName, const ${types.string.name}& attachedDbName)
{
    ${types.string.name} sqlQuery(get_allocator_ref());
    sqlQuery += "ATTACH DATABASE '";
    sqlQuery += fileName;
    sqlQuery += "' AS ";
    sqlQuery += attachedDbName;

    m_db.executeUpdate(sqlQuery);

    m_attachedDbList.push_back(attachedDbName);
}

void ${name}::detachDatabases()
{
    for (const auto& attachedDb : m_attachedDbList)
    {
        try
        {
            ${types.string.name} sqlQuery(get_allocator_ref());
            sqlQuery += "DETACH DATABASE ";
            sqlQuery += attachedDb;
            m_db.executeUpdate(sqlQuery);
        }
        catch (const ::zserio::SqliteException&)
        {
            // ignore since we have no logging sub-system and we need to prevent exception in SQLDatabase destructor
        }
    }
    m_attachedDbList.clear();
}
<@namespace_end package.path/>
