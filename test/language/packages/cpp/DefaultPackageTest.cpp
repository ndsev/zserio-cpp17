#include "gtest/gtest.h"

#include "DefaultPackageStructure.h"

TEST(DefaultPackageTest, defaultPackageStructure)
{
    // just test that DefaultPackageStructure is available in global namespace
    DefaultPackageStructure data;

    data.value = 10;
    default_package_import::top::TopStructure topStructure;
    topStructure.type = 1;
    topStructure.data = 1234;
    data.topStructure = topStructure;
    Child child;
    child.value = 0xdeadbeef;
    data.childStructure = child;

    zserio::View view(data, 4);

    ASSERT_EQ(10, view.value());
    ASSERT_EQ(1, view.topStructure().type());
    ASSERT_EQ(1234, view.topStructure().data());
    ASSERT_EQ(0xdeadbeef, view.childStructure().value());
}
