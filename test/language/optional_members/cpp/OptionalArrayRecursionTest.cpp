#include <cstring>

#include "gtest/gtest.h"
#include "optional_members/optional_array_recursion/Employee.h"
#include "optional_members/optional_array_recursion/Title.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace optional_members
{
namespace optional_array_recursion
{

using AllocatorType = Employee::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class OptionalArrayRecursionTest : public ::testing::Test
{
protected:
    void fillEmployee(Employee& employee, std::string_view name, uint16_t salary, Title title)
    {
        employee.name = StringType(name);
        employee.salary = salary;
        employee.title = title;
    }

    void fillTeamLead(Employee& teamLead)
    {
        fillEmployee(teamLead, EMPLOYEE_TEAM_LEAD_NAME, EMPLOYEE_TEAM_LEAD_SALARY, Title::TEAM_LEAD);

        VectorType<Employee> teamMembers;
        Employee teamMember1;
        fillEmployee(teamMember1, EMPLOYEE_DEVELOPER1_NAME, EMPLOYEE_DEVELOPER1_SALARY, Title::DEVELOPER);
        teamMembers.push_back(teamMember1);

        Employee teamMember2;
        fillEmployee(teamMember2, EMPLOYEE_DEVELOPER2_NAME, EMPLOYEE_DEVELOPER2_SALARY, Title::DEVELOPER);
        teamMembers.push_back(teamMember2);

        teamLead.teamMembers = teamMembers;
    }

    static void writeEmployee(
            zserio::BitStreamWriter& writer, std::string_view name, uint16_t salary, Title title)
    {
        writer.writeString(StringType(name));
        writer.writeUnsignedBits32(salary, 16);
        writer.writeUnsignedBits32(zserio::enumToValue(title), 8);
    }

    static void writeTeamLead(zserio::BitStreamWriter& writer)
    {
        writeEmployee(writer, EMPLOYEE_TEAM_LEAD_NAME, EMPLOYEE_TEAM_LEAD_SALARY, Title::TEAM_LEAD);
        writer.writeVarUInt64(NUM_DEVELOPERS);
        writeEmployee(writer, EMPLOYEE_DEVELOPER1_NAME, EMPLOYEE_DEVELOPER1_SALARY, Title::DEVELOPER);
        writeEmployee(writer, EMPLOYEE_DEVELOPER2_NAME, EMPLOYEE_DEVELOPER2_SALARY, Title::DEVELOPER);
    }

    static constexpr std::string_view BLOB_NAME_BASE = "language/optional_members/optional_array_recursion_";

    static constexpr std::string_view EMPTY_EMPLOYEE_NAME = "";
    static constexpr uint16_t EMPTY_EMPLOYEE_SALARY = 0;

    static constexpr std::string_view EMPLOYEE_TEAM_LEAD_NAME = "Nico";
    static constexpr uint16_t EMPLOYEE_TEAM_LEAD_SALARY = 2000;

    static constexpr std::string_view EMPLOYEE_DEVELOPER1_NAME = "Mike";
    static constexpr uint16_t EMPLOYEE_DEVELOPER1_SALARY = 1000;

    static constexpr std::string_view EMPLOYEE_DEVELOPER2_NAME = "Luke";
    static constexpr uint16_t EMPLOYEE_DEVELOPER2_SALARY = 1800;

    static constexpr size_t NUM_DEVELOPERS = 2;
    static constexpr size_t EMPTY_EMPLOYEE_BIT_SIZE = 32;
    static constexpr size_t TEAM_LEAD_BIT_SIZE = EMPTY_EMPLOYEE_BIT_SIZE * 3 + 8 +
            OptionalArrayRecursionTest::EMPLOYEE_TEAM_LEAD_NAME.size() * 8 +
            OptionalArrayRecursionTest::EMPLOYEE_DEVELOPER1_NAME.size() * 8 +
            OptionalArrayRecursionTest::EMPLOYEE_DEVELOPER2_NAME.size() * 8;
};

TEST_F(OptionalArrayRecursionTest, bitSizeOf)
{
    Employee data(StringType(EMPTY_EMPLOYEE_NAME), EMPTY_EMPLOYEE_SALARY, Title::DEVELOPER, std::nullopt);
    zserio::View view(data);
    ASSERT_EQ(EMPTY_EMPLOYEE_BIT_SIZE, zserio::detail::bitSizeOf(view));

    fillTeamLead(data);
    ASSERT_EQ(TEAM_LEAD_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(OptionalArrayRecursionTest, comparisionOperators)
{
    Employee teamLead;
    fillTeamLead(teamLead);
    Employee equalTeamLead;
    fillTeamLead(equalTeamLead);
    Employee employee(StringType(EMPTY_EMPLOYEE_NAME), EMPTY_EMPLOYEE_SALARY, Title::DEVELOPER, std::nullopt);

    test_utils::comparisonOperatorsTest(teamLead, equalTeamLead, employee);

    zserio::View view(teamLead);
    zserio::View equalView(equalTeamLead);
    zserio::View lessThanView(employee);

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(OptionalArrayRecursionTest, writeReadEmployee)
{
    Employee employee;
    fillEmployee(employee, EMPLOYEE_DEVELOPER1_NAME, EMPLOYEE_DEVELOPER1_SALARY, Title::DEVELOPER);

    test_utils::writeReadTest(employee);
}

TEST_F(OptionalArrayRecursionTest, writeReadTeamLead)
{
    Employee teamLead;
    fillTeamLead(teamLead);

    test_utils::writeReadTest(teamLead);
}

TEST_F(OptionalArrayRecursionTest, writeReadFileEmployee)
{
    Employee employee;
    fillEmployee(employee, EMPLOYEE_DEVELOPER1_NAME, EMPLOYEE_DEVELOPER1_SALARY, Title::DEVELOPER);
    const StringType fileName = StringType(BLOB_NAME_BASE) + "employee.blob";

    test_utils::writeReadFileTest(fileName, employee);
}

TEST_F(OptionalArrayRecursionTest, writeReadFileTeamLead)
{
    Employee teamLead;
    fillTeamLead(teamLead);
    const StringType fileName = StringType(BLOB_NAME_BASE) + "team_lead.blob";

    test_utils::writeReadFileTest(fileName, teamLead);
}

TEST_F(OptionalArrayRecursionTest, readEmployee)
{
    Employee employee;
    fillEmployee(employee, EMPLOYEE_DEVELOPER1_NAME, EMPLOYEE_DEVELOPER1_SALARY, Title::DEVELOPER);

    test_utils::readTest(std::bind(writeEmployee, std::placeholders::_1, EMPLOYEE_DEVELOPER1_NAME,
                                 EMPLOYEE_DEVELOPER1_SALARY, Title::DEVELOPER),
            employee);
}

TEST_F(OptionalArrayRecursionTest, readTeamLead)
{
    Employee teamLead;
    fillTeamLead(teamLead);

    test_utils::readTest(writeTeamLead, teamLead);
}

TEST_F(OptionalArrayRecursionTest, stdHash)
{
    Employee teamLead;
    fillTeamLead(teamLead);
    const size_t teamLeadHash = 1117021751;
    Employee equalTeamLead;
    fillTeamLead(equalTeamLead);
    Employee employee(StringType(EMPTY_EMPLOYEE_NAME), EMPTY_EMPLOYEE_SALARY, Title::DEVELOPER, std::nullopt);
    const size_t employeeHash = 32338;

    test_utils::hashTest(teamLead, teamLeadHash, equalTeamLead, employee, employeeHash);

    zserio::View view(teamLead);
    const size_t viewHash = 1117021751;
    zserio::View equalView(equalTeamLead);
    zserio::View diffView(employee);
    const size_t diffViewHash = 32338;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace optional_array_recursion
} // namespace optional_members
