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

  for (auto sensor : tempSensors) {
    cout << sensor->getTemperature() << endl;
  }

  for (iTemperature* sensor : tempSensors) {
    delete sensor;
  }

  tempSensors.clear();

  return 0;
}
