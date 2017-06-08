#include "gtest/gtest.h"
#include "iUpmObject.hpp"
#include "iActuatorType.hpp"
#include "iServoActuator.hpp"
#include "external/json/json.hpp"
#include <vector>

/* Interfaces test fixture */
class iActuatorType_unit : public ::testing::Test, public virtual upm::iActuatorType
{
protected:
    /* One-time setup logic if needed */
    iActuatorType_unit()
    {
        this->initFromJsonLibDesc("./actuatortype_example.json");
    }

    /* One-time tear-down logic if needed */
    virtual ~iActuatorType_unit() {}

    /* Per-test setup logic if needed */
    virtual void SetUp() {}

    /* Per-test tear-down logic if needed */
    virtual void TearDown() {}
};


TEST_F(iActuatorType_unit, test_commands_deserialize)
{
    /* Deserialize the JSON to a map of commands */
    std::map<std::string, upm::ActuatorSink<float>> commands =
            LibraryJson()["Sensor Class"]["ServoTest"]["Commands"];

    ASSERT_EQ(1, commands.size());

    ASSERT_TRUE(commands.find("axis0") != commands.end());
    ASSERT_EQ("degrees", commands["axis0"].unit);
    ASSERT_EQ(0.0, commands["axis0"].min);
    ASSERT_EQ(180.0, commands["axis0"].max);
}
