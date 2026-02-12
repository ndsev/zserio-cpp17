package zserio.extension.cpp17;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.cli.Options;

import zserio.ast.Root;
import zserio.extension.common.CompatibilityChecker;
import zserio.extension.common.OutputFileManager;
import zserio.extension.common.PackedTypesCollector;
import zserio.extension.common.ReservedKeywordsClashChecker;
import zserio.extension.common.ZserioExtensionException;
import zserio.tools.Extension;
import zserio.tools.ExtensionParameters;

/**
 * The extension which generates C++17 API sources.
 */
public final class Cpp17Extension implements Extension
{
    @Override
    public String getName()
    {
        return "C++17 Generator";
    }

    @Override
    public String getExtensionVersion()
    {
        return Cpp17ExtensionVersion.CPP17_EXTENSION_VERSION_STRING + " (BIN " +
                Cpp17ExtensionVersion.BIN_VERSION_STRING + ", JSON " +
                Cpp17ExtensionVersion.JSON_VERSION_STRING + ")";
    }

    @Override
    public String getZserioVersion()
    {
        return Cpp17ExtensionVersion.EXPECTED_ZSERIO_VERSION_STRING;
    }

    @Override
    public void registerOptions(Options options)
    {
        Cpp17ExtensionParameters.registerOptions(options);
    }

    @Override
    public boolean isEnabled(ExtensionParameters parameters)
    {
        return Cpp17ExtensionParameters.hasOptionCpp17(parameters);
    }

    @Override
    public void check(Root rootNode, ExtensionParameters parameters) throws ZserioExtensionException
    {
        Cpp17ExtensionParameters.check(parameters);

        final CompatibilityChecker compatibilityChecker = new CompatibilityChecker();
        rootNode.walk(compatibilityChecker);

        final ReservedKeywordsClashChecker cppKeywordsClashChecker =
                new ReservedKeywordsClashChecker("C++", CPP_KEYWORDS);
        rootNode.walk(cppKeywordsClashChecker);

        final CppInnerClassesClashChecker innerClassesClashChecker = new CppInnerClassesClashChecker();
        rootNode.walk(innerClassesClashChecker);

        final CppTemplateParameterClashChecker tmpParamClashChecker = new CppTemplateParameterClashChecker();
        rootNode.walk(tmpParamClashChecker);
    }

    @Override
    public void process(Root rootNode, ExtensionParameters parameters) throws ZserioExtensionException
    {
        final OutputFileManager outputFileManager = new OutputFileManager(parameters);
        final Cpp17ExtensionParameters cppParameters = new Cpp17ExtensionParameters(parameters);

        // collect which types are used within a packed array
        final PackedTypesCollector packedTypesCollector = new PackedTypesCollector();
        rootNode.accept(packedTypesCollector);

        // collect which fields are used in offset expressions
        final OffsetFieldsCollector offsetFieldsCollector = new OffsetFieldsCollector();
        rootNode.accept(offsetFieldsCollector);

        final TemplateDataContext context =
                new TemplateDataContext(cppParameters, packedTypesCollector, offsetFieldsCollector);

        final List<CppDefaultEmitter> emitters = new ArrayList<CppDefaultEmitter>();
        emitters.add(new BitmaskEmitter(outputFileManager, cppParameters, context));
        emitters.add(new ConstEmitter(outputFileManager, cppParameters, context));
        emitters.add(new ChoiceEmitter(outputFileManager, cppParameters, context));
        emitters.add(new EnumerationEmitter(outputFileManager, cppParameters, context));
        emitters.add(new InstantiateTypeEmitter(outputFileManager, cppParameters, context));
        emitters.add(new ServiceEmitter(outputFileManager, cppParameters, context));
        emitters.add(new StructureEmitter(outputFileManager, cppParameters, context));
        emitters.add(new SubtypeEmitter(outputFileManager, cppParameters, context));
        emitters.add(new SqlDatabaseEmitter(outputFileManager, cppParameters, context));
        emitters.add(new SqlTableEmitter(outputFileManager, cppParameters, context));
        emitters.add(new PubsubEmitter(outputFileManager, cppParameters, context));
        emitters.add(new UnionEmitter(outputFileManager, cppParameters, context));

        // emit C++ code
        for (CppDefaultEmitter emitter : emitters)
            rootNode.walk(emitter);

        outputFileManager.printReport();
    }

    private static final String[] CPP_KEYWORDS = new String[] {
            "alignas",
            "alignof",
            "and",
            "and_eq",
            "asm",
            "auto",
            "bitand",
            "bitor",
            "bool",
            "break",
            "case",
            "catch",
            "char",
            "char8_t",
            "char16_t",
            "char32_t",
            "class",
            "compl",
            "concept",
            "const",
            "consteval",
            "constexpr",
            "constinit",
            "const_cast",
            "continue",
            "co_await",
            "co_return",
            "co_yield",
            "decltype",
            "default",
            "delete",
            "do",
            "double",
            "dynamic_cast",
            "else",
            "enum",
            "explicit",
            "export",
            "extern",
            "false",
            "float",
            "for",
            "friend",
            "goto",
            "if",
            "inline",
            "int",
            "long",
            "mutable",
            "namespace",
            "new",
            "noexcept",
            "not",
            "not_eq",
            "nullptr",
            "operator",
            "or",
            "or_eq",
            "private",
            "protected",
            "public",
            "register",
            "reinterpret_cast",
            "requires",
            "return",
            "short",
            "signed",
            "sizeof",
            "static",
            "static_assert",
            "static_cast",
            "struct",
            "switch",
            "template",
            "this",
            "thread_local",
            "throw",
            "true",
            "try",
            "typedef",
            "typeid",
            "typename",
            "union",
            "unsigned",
            "using",
            "virtual",
            "void",
            "volatile",
            "wchar_t",
            "while",
            "xor",
            "xor_eq",
    };
}
