#include <iostream>
#include <vector>
#include <memory>

#include "apds9002.hpp"
#include "bh1750.hpp"
#include "max44009.hpp"

using namespace std;
using namespace upm;

int
main()
{
  vector<unique_ptr<iLight>> lightSensors;

  // Populate list of light sensors
  lightSensors.push_back(unique_ptr<iLight>(new APDS9002(0)));
  lightSensors.push_back(unique_ptr<iLight>(new BH1750()));
  lightSensors.push_back(unique_ptr<iLight>(new MAX44009(1)));

  // Measure luminance level from all 3 individual sensors
  for (auto& sensor : lightSensors) {
    sensor->getLuminance();
  }

  return 0;
}
