#include <iostream>
#include <list>

#include "apds9002.hpp"
#include "bh1750.hpp"
#include "max44009.hpp"

using namespace std;
using namespace upm;

int
main()
{
  list<iLight*> lightSensors;

  // Populate list of light sensors
  lightSensors.push_back(new APDS9002(0));
  lightSensors.push_back(new BH1750());
  lightSensors.push_back(new MAX44009(1));

  // Measure luminance level from all 3 individual sensors
  for (auto& sensor : lightSensors) {
    sensor->getLuminance();
  }

  for (auto& sensor : lightSensors) {
    delete sensor;
  }

  return 0;
}
