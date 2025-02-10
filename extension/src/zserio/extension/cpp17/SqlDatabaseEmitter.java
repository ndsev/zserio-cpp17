package zserio.extension.cpp17;

import zserio.ast.SqlDatabaseType;
import zserio.extension.common.OutputFileManager;
import zserio.extension.common.ZserioExtensionException;

/**
 * SQL database emitter.
 */
public final class SqlDatabaseEmitter extends CppDefaultEmitter
{
    public SqlDatabaseEmitter(OutputFileManager outputFileManager, Cpp17ExtensionParameters cppParameters,
            TemplateDataContext context)
    {
        super(outputFileManager, cppParameters, context);
    }

    @Override
    public void beginSqlDatabase(SqlDatabaseType sqlDatabaseType) throws ZserioExtensionException
    {
        final Object templateData =
                new SqlDatabaseEmitterTemplateData(getTemplateDataContext(), sqlDatabaseType);

        processHeaderTemplate(TEMPLATE_HEADER_NAME, templateData, sqlDatabaseType);
        processSourceTemplate(TEMPLATE_SOURCE_NAME, templateData, sqlDatabaseType);
    }

    private static final String TEMPLATE_SOURCE_NAME = "SqlDatabase.cpp.ftl";
    private static final String TEMPLATE_HEADER_NAME = "SqlDatabase.h.ftl";
}
