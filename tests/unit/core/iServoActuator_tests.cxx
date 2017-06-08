#include "gtest/gtest.h"
#include "iUpmObject.hpp"
#include "iServoActuator.hpp"
#include "external/json/json.hpp"
#include <vector>
#include <cstdlib>

/* Use the string literal operator from nlohmann */
using namespace nlohmann;

/* Mock ServoTest class */
class ES08A : public virtual upm::iServoActuator
{
public:
    std::map<std::string, float> sinks;

    ES08A()
    {
        sinks["axis0"] = 0.0;
        sinks["axis1"] = 0.0;

        /* Add a second command for testing */
        AddCommand("axis1", "degrees");
    }

    void AngleForCommands(std::map<std::string, float> commands)
    {
        for(std::map<std::string, float>::const_iterator it = commands.begin(); it != commands.end(); ++it)
        {
#ifndef NDEBUG
                std::cout << "Received set angle command for: " << it->first << " value: " << it->second << std::endl;
#endif

            std::map<std::string, float>::iterator sit = sinks.find(it->first);

            if (sit == sinks.end())
                throw std::invalid_argument("Servo axis: " + it->first + " not found");

            sit->second = it->second;
        }
    }
};

/* Interfaces test fixture */
class iServoActuator_unit : public ::testing::Test
{
protected:
    iServoActuator_unit() {}
    virtual ~iServoActuator_unit() {}
    virtual void SetUp()
    {
        /* Initialize from servo.json */
        sref.initFromJsonLibDesc("../../../src/servo/servo.json");
    }
    virtual void TearDown() {}

    ES08A sref;
};

TEST_F(iServoActuator_unit, test_iServoActuator_basic)
{
    ASSERT_EQ(2, sref.Commands().size());
}

TEST_F(iServoActuator_unit, test_iServoActuator_serialize_command)
{
    upm::ActuatorCommand<float> acmd;
    acmd.command = "axis0";
    acmd.unit = "degrees";
    acmd.value = 30.0;
    json jcmd = acmd;

    ASSERT_EQ("{\"command\":\"axis0\",\"unit\":\"degrees\",\"value\":30.0}", jcmd.dump());
}

TEST_F(iServoActuator_unit, test_iServoActuator_deserialize_command)
{
    json jcmd = "{\"command\":\"axis0\",\"unit\":\"degrees\",\"value\":30.0}"_json;
    upm::ActuatorCommand<float> acmd = jcmd;
}

TEST_F(iServoActuator_unit, test_iServoActuator_run_command)
{
    sref.AngleFromJson("{\"command\":\"axis0\",\"unit\":\"radians\",\"value\":3.14159}");

    ASSERT_FLOAT_EQ(3.14159, sref.sinks["axis0"]);
}

TEST_F(iServoActuator_unit, test_iServoActuator_run_command_invalid_command)
{
    ASSERT_THROW(sref.AngleFromJson("{\"command\":\"axis_doesn't_exist\",\"unit\":\"radians\",\"value\":3.14159}"),
                 std::invalid_argument);
}

TEST_F(iServoActuator_unit, test_iServoActuator_run_command_fail_deserialization)
{
    ASSERT_THROW(sref.AngleFromJson("This is not }{ a valid JSON string"),
                 std::invalid_argument);
}

TEST_F(iServoActuator_unit, test_iServoActuator_run_commands)
{
    sref.AngleFromJson("[{\"command\":\"axis0\",\"unit\":\"degrees\",\"value\":30.0},{\"command\":\"axis1\",\"unit\":\"radians\",\"value\":3.1}]");
}
