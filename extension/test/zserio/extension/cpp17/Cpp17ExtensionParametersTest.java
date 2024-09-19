package zserio.extension.cpp17;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.fail;

import java.util.ArrayList;
import java.util.List;

import org.junit.jupiter.api.Test;

import zserio.extension.common.ZserioExtensionException;
import zserio.tools.ExtensionParameters;

public class Cpp17ExtensionParametersTest
{
    @Test
    public void checkUnknownSetCppAllocator()
    {
        final String setCppAllocator = "unknown";
        final boolean withReflectionCode = false;
        final TestExtensionParameters extensionParameters =
                new TestExtensionParameters(setCppAllocator, withReflectionCode);
        assertThrows(ZserioExtensionException.class, () -> Cpp17ExtensionParameters.check(extensionParameters));
    }

    @Test
    public void checkStdSetCppAllocator()
    {
        final String setCppAllocator = "std";
        final boolean withReflectionCode = false;
        final TestExtensionParameters extensionParameters =
                new TestExtensionParameters(setCppAllocator, withReflectionCode);
        assertDoesNotThrow(() -> Cpp17ExtensionParameters.check(extensionParameters));
    }

    @Test
    public void checkPolymorphicSetCppAllocator()
    {
        final String setCppAllocator = "polymorphic";
        final boolean withReflectionCode = false;
        final TestExtensionParameters extensionParameters =
                new TestExtensionParameters(setCppAllocator, withReflectionCode);
        assertDoesNotThrow(() -> Cpp17ExtensionParameters.check(extensionParameters));
    }

    @Test
    public void checkWithReflectionCode()
    {
        final String setCppAllocator = "std";
        final boolean withReflectionCode = true;
        final TestExtensionParameters extensionParameters =
                new TestExtensionParameters(setCppAllocator, withReflectionCode);
        assertDoesNotThrow(() -> Cpp17ExtensionParameters.check(extensionParameters));
    }

    private static class TestExtensionParameters implements ExtensionParameters
    {
        public TestExtensionParameters(String setCppAllocator, boolean withReflectionCode)
        {
            this.setCppAllocator = setCppAllocator;
            this.withReflectionCode = withReflectionCode;
        }

        @Override
        public boolean argumentExists(String argumentName)
        {
            if (argumentName.equals("withReflectionCode"))
                return withReflectionCode;

            fail("TestExtensionParameters: argumentExists failure!");

            return false;
        }

        @Override
        public String getCommandLineArg(String argumentName)
        {
            if (argumentName.equals("setCppAllocator"))
                return setCppAllocator;

            fail("TestExtensionParameters: getCommandLineArg failure!");

            return null;
        }

        @Override
        public String getFileName()
        {
            return null;
        }

        @Override
        public String getPathName()
        {
            return null;
        }

        @Override
        public boolean getWithRangeCheckCode()
        {
            return false;
        }

        @Override
        public boolean getWithPubsubCode()
        {
            return false;
        }

        @Override
        public boolean getWithServiceCode()
        {
            return false;
        }

        @Override
        public boolean getWithSqlCode()
        {
            return false;
        }

        @Override
        public boolean getWithTypeInfoCode()
        {
            return false;
        }

        @Override
        public boolean getWithValidationCode()
        {
            return false;
        }

        @Override
        public boolean getWithWriterCode()
        {
            return false;
        }

        @Override
        public boolean getWithCodeComments()
        {
            return false;
        }

        @Override
        public List<String> getTopLevelPackageNameIds()
        {
            return new ArrayList<String>();
        }

        @Override
        public boolean getIgnoreTimestamps()
        {
            return false;
        }

        @Override
        public long getLastModifiedTime()
        {
            return 0;
        }

        @Override
        public String getZserioVersion()
        {
            return null;
        }

        private final String setCppAllocator;
        private final boolean withReflectionCode;
    }
}
