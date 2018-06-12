#include <iostream>
#include <vector>

#include "lm35.hpp"
#include "abp.hpp"

using namespace std;
using namespace upm;

int
main()
{
  vector<iTemperature*> tempSensors {new LM35(0), new ABP(0, ABP_DEFAULT_ADDRESS)};

  for (auto& sensor : tempSensors) {
    float celsiusTemp = sensor->getTemperature();
    cout << "Temperature in Celsius degrees: " << celsiusTemp << endl;
    cout << "Temperature in Kelvin: " << iTemperature::convertCelsiusTo(celsiusTemp, TemperatureUnit::KELVIN);
  }

  for (auto& sensor : tempSensors) {
    delete sensor;
  }

  return 0;
}
