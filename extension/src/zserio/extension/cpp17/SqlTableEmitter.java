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
        if (sqlTableType.getTemplateParameters().isEmpty())
        {
            processHeaderTemplate(FTL_HEADER_NAME, templateData, sqlTableType);
            processSourceTemplate(FTL_SOURCE_NAME, templateData, sqlTableType);
        }
        else
        {
            processHeaderTemplate(FTL_TEMPLATE_HEADER_NAME, templateData, sqlTableType);
        }
    }

    private static final String FTL_HEADER_NAME = "SqlTable.h.ftl";
    private static final String FTL_SOURCE_NAME = "SqlTable.cpp.ftl";
    private static final String FTL_TEMPLATE_HEADER_NAME = "SqlTableTemplate.h.ftl";
}
