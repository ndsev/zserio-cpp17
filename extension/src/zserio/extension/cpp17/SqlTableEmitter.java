package zserio.extension.cpp17;

import zserio.ast.SqlTableType;
import zserio.extension.common.OutputFileManager;
import zserio.extension.common.ZserioExtensionException;

/**
 * SQL table emitter.
 */
public final class SqlTableEmitter extends CppDefaultEmitter
{
    public SqlTableEmitter(OutputFileManager outputFileManager, Cpp17ExtensionParameters cppParameters,
            TemplateDataContext context)
    {
        super(outputFileManager, cppParameters, context);
    }

    @Override
    public void beginSqlTable(SqlTableType sqlTableType) throws ZserioExtensionException
    {
        final Object templateData = new SqlTableEmitterTemplateData(getTemplateDataContext(), sqlTableType);
        processHeaderTemplate(TABLE_TEMPLATE_HEADER_NAME, templateData, sqlTableType);
        processSourceTemplate(TABLE_TEMPLATE_SOURCE_NAME, templateData, sqlTableType);
    }

    private static final String TABLE_TEMPLATE_SOURCE_NAME = "SqlTable.cpp.ftl";
    private static final String TABLE_TEMPLATE_HEADER_NAME = "SqlTable.h.ftl";
}
