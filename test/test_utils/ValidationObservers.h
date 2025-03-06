#ifndef TEST_UTILS_VALIDATION_OBSERVERS_H_INC
#define TEST_UTILS_VALIDATION_OBSERVERS_H_INC

#include <algorithm>
#include <sstream>
#include <string_view>

#include "zserio/IValidationObserver.h"
#include "zserio/StringConvertUtil.h"

namespace test_utils
{

class CountingValidationObserver : public zserio::IValidationObserver
{
public:
    void beginDatabase(size_t numberOfTables) override
    {
        m_numberOfTables += numberOfTables;
    }

    void endDatabase(size_t numberOfValidatedTables) override
    {
        m_numberOfValidatedTables += numberOfValidatedTables;
    }

    bool beginTable(std::string_view, size_t numberOfRows) override
    {
        m_numberOfRows += numberOfRows;
        return true;
    }

    bool endTable(std::string_view, size_t numberOfValidatedRows) override
    {
        m_numberOfValidatedRows += numberOfValidatedRows;
        return true;
    }

    bool reportError(std::string_view, std::string_view, zserio::Span<const std::string_view>, ErrorType,
            std::string_view) override
    {
        m_numberOfErrors++;
        return true;
    }

    size_t getNumberOfTables() const
    {
        return m_numberOfTables;
    }
    size_t getNumberOfValidatedTables() const
    {
        return m_numberOfValidatedTables;
    }
    size_t getNumberOfRows() const
    {
        return m_numberOfRows;
    }
    size_t getNumberOfValidatedRows() const
    {
        return m_numberOfValidatedRows;
    }
    size_t getNumberOfErrors() const
    {
        return m_numberOfErrors;
    }

private:
    size_t m_numberOfTables = 0;
    size_t m_numberOfValidatedTables = 0;
    size_t m_numberOfRows = 0;
    size_t m_numberOfValidatedRows = 0;
    size_t m_numberOfErrors = 0;
};

class ValidationObserver : public zserio::IValidationObserver
{
public:
    using RowCounts = std::tuple<size_t, size_t>;

    struct Error
    {
        Error(std::string_view tableNameView, std::string_view fieldNameView,
                zserio::Span<const std::string_view> primaryKeyValuesSpan, ErrorType error,
                std::string_view messageView) :
                tableName(zserio::toString(tableNameView)),
                fieldName(zserio::toString(fieldNameView)),
                errorType(error),
                message(zserio::toString(messageView))
        {
            std::transform(primaryKeyValuesSpan.begin(), primaryKeyValuesSpan.end(),
                    std::back_inserter(this->primaryKeyValues), [](std::string_view msg) -> std::string {
                        return ::zserio::toString(msg);
                    });
        }

        std::string tableName;
        std::string fieldName;
        std::vector<std::string> primaryKeyValues;
        ErrorType errorType;
        std::string message;
    };

    void beginDatabase(size_t numberOfTables) override
    {
        m_numberOfTables = numberOfTables;
    }

    void endDatabase(size_t numberOfValidatedTables) override
    {
        m_numberOfValidatedTables = numberOfValidatedTables;
    }

    bool beginTable(std::string_view tableName, size_t numberOfRows) override
    {
        m_rowCountsMap[zserio::toString(tableName)] = std::make_tuple(numberOfRows, static_cast<size_t>(0));
        return true;
    }

    bool endTable(std::string_view tableName, size_t numberOfValidatedRows) override
    {
        std::get<1>(m_rowCountsMap[zserio::toString(tableName)]) = numberOfValidatedRows;
        return true;
    }

    bool reportError(std::string_view tableName, std::string_view fieldName,
            zserio::Span<const std::string_view> primaryKeyValues, ErrorType errorType,
            std::string_view message) override
    {
        m_errors.emplace_back(tableName, fieldName, primaryKeyValues, errorType, message);
        return true;
    }

    size_t getNumberOfTables() const
    {
        return m_numberOfTables;
    }

    size_t getNumberOfValidatedTables() const
    {
        return m_numberOfValidatedTables;
    }

    size_t getNumberOfTableRows(std::string_view tableName) const
    {
        return std::get<0>(getTableRowCounts(tableName));
    }

    size_t getNumberOfValidatedTableRows(std::string_view tableName) const
    {
        return std::get<1>(getTableRowCounts(tableName));
    }

    const std::vector<Error>& getErrors() const
    {
        return m_errors;
    }

    std::string getErrorsString() const
    {
        std::ostringstream out;
        for (const auto& error : m_errors)
        {
            out << error.tableName << ", " << error.fieldName << ", [";
            for (auto keyValueIt = error.primaryKeyValues.begin(); keyValueIt != error.primaryKeyValues.end();
                    ++keyValueIt)
            {
                out << (keyValueIt != error.primaryKeyValues.begin() ? ", " : "") << *keyValueIt;
            }
            out << "], " << error.errorType << ", " << error.message << "\n";
        }

        return out.str();
    }

private:
    RowCounts getTableRowCounts(std::string_view tableName) const
    {
        auto search = m_rowCountsMap.find(zserio::toString(tableName));

        return (search != m_rowCountsMap.end()) ? search->second : std::make_tuple<size_t, size_t>(0, 0);
    }

    size_t m_numberOfTables = 0;
    size_t m_numberOfValidatedTables = 0;
    std::map<std::string, RowCounts> m_rowCountsMap;
    std::vector<Error> m_errors;
};

} // namespace test_utils

#endif // TEST_UTILS_VALIDATION_OBSERVERS_H_INC
