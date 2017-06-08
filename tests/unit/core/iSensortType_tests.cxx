#include "gtest/gtest.h"
#include "iUpmObject.hpp"
#include "iSensorType.hpp"
#include "iTemperatureSensor.hpp"
#include "external/json/json.hpp"
#include <vector>


/* Interfaces test fixture */
class iSensorType_unit : public ::testing::Test, public virtual upm::iSensorType
{
protected:
    /* One-time setup logic if needed */
    iSensorType_unit()
    {
        this->initFromJsonLibDesc("./sensortype_example.json");
    }

    /* One-time tear-down logic if needed */
    virtual ~iSensorType_unit() {}

    /* Per-test setup logic if needed */
    virtual void SetUp() {}

    /* Per-test tear-down logic if needed */
    virtual void TearDown() {}

    //TestSensorClass& obj;
};


TEST_F(iSensorType_unit, test_sources_deserialize)
{
    /* Deserialize the JSON to a map of sources */
    std::map<std::string, upm::SensorSource<float>> sources =
            LibraryJson()["Sensor Class"]["TestSensorClass"]["Sources"];

    ASSERT_EQ(2, sources.size());

    ASSERT_TRUE(sources.find("temperature") != sources.end());
    ASSERT_TRUE(sources.find("humidity-relative") != sources.end());
    ASSERT_EQ("C", sources["temperature"].unit);
    ASSERT_EQ(-17.0, sources["temperature"].min);
    ASSERT_EQ(49.0, sources["temperature"].max);
    ASSERT_EQ(1.0, sources["temperature"].accuracy);
}


TEST_F(iSensorType_unit, test_iTemperatureSensor)
{
    /* Mock RHUSB class */
    class RHUSB : public virtual upm::iTemperatureSensor
    {
        std::map<std::string, float> TemperatureForSources(std::vector<std::string> sources)
        {
            std::map<std::string, float> fake_temps;

            if (std::find(sources.begin(), sources.end(), "temperature") != sources.end())
                fake_temps["temperature"] = 25.5;

            return fake_temps;
        }
    };

    RHUSB ts;
    ts.initFromJsonLibDesc("../../../src/rhusb/rhusb.json");

    ASSERT_EQ(2, ts.Sources().size());
    ASSERT_EQ(25.5, ts.TemperatureForSource("temperature"));
}
