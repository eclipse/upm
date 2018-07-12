#include <iostream>
#include <list>

#include "apds9002.hpp"
#include "bh1750.hpp"
#include "max44009.hpp"

int
main()
{
    std::list<upm::iLight*> lightSensors;

    // Populate list of light sensors
    lightSensors.push_back(new upm::APDS9002(0));
    lightSensors.push_back(new upm::BH1750());
    lightSensors.push_back(new upm::MAX44009(1));

    // Measure luminance level from all 3 individual sensors
    for (auto& sensor : lightSensors) {
        sensor->getLuminance();
    }

    for (auto& sensor : lightSensors) {
        delete sensor;
    }

    return 0;
}
