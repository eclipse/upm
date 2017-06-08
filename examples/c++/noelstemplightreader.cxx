#include <iostream>
#include <unistd.h>

#include "core/iUpmObject.hpp"
#include "noelstemplightreader.hpp"

void printMap(upm::NoelsTempLightReader &sensor, std::map<std::string, float> &data)
{
    if (data.empty())
        std::cout << "Empty map" << std::endl;
    for (std::map<std::string, float>::const_iterator it = data.begin();
            it != data.end(); ++it)
    {
        std::cout << "label: " << it->first << ", value: " << it->second
            << ", unit: " << sensor.Unit(it->first) << std::endl;
    }
    std::cout << std::endl;
}

int main ()
{
    upm::NoelsTempLightReader sensor;
    std::cout << "iUpmObject JsonDefinition..." << std::endl << ((upm::iUpmObject&)sensor).JsonDefinition() << std::endl << std::endl;
    std::cout << "iSensorType JsonDefinition..." << std::endl << ((upm::iSensorType&)sensor).JsonDefinition() << std::endl << std::endl;
    std::cout << "iMraa JsonDefinition..." << std::endl << ((upm::iMraa&)sensor).JsonDefinition() << std::endl << std::endl;
    std::cout << "NoelsTempLightReader JsonDefinition..." << std::endl << sensor.JsonDefinition() << std::endl << std::endl;

    std::cout << "Read all light values..." << std::endl;
    std::map<std::string, float> values = sensor.LightAll();
    printMap(sensor, values);

    std::cout << "Read a single light value for light0..." << std::endl;
    std::cout << "Single value = " << sensor.LightForSource("light0") << std::endl << std::endl;

    try
    {
        std::cout << "Read a single light value for lightX (doesn't exist)..." << std::endl;
        sensor.LightForSource("lightX");
    }
    catch (const std::exception& e) { std::cout << e.what() << std::endl << std::endl;}

    std::cout << "Read a light value for lightX (doesn't exist)..." << std::endl;
    values = sensor.LightForSources(std::vector<std::string>({"lightX"}));
    printMap(sensor, values);

    std::cout << "Read all values as JsonDefinition..." << std::endl << sensor.JsonValues() << std::endl;

    return 0;
}
