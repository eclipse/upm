/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <unistd.h>
#include <iostream>
#include <signal.h>

#include "h803x.hpp"

using namespace std;

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);

//! [Interesting]

  string defaultDev = "/dev/ttyUSB0";

  // if an argument was specified, use it as the device instead
  if (argc > 1)
    defaultDev = string(argv[1]);

  cout << "Using device " << defaultDev << endl;
  cout << "Initializing..." << endl;

  // Instantiate an H803X instance, using MODBUS slave address 1, and
  // default comm parameters (9600, 8, N, 2)
  upm::H803X *sensor = new upm::H803X(defaultDev, 1);

  // output the Slave ID string
  cout << "Slave ID: " << sensor->getSlaveID() << endl;
  cout << endl;

  // update and print available values every second
  while (shouldRun)
    {
      // update our values from the sensor
      sensor->update();

      // H8035 / H8036
      cout << "Consumption (kWh): " << sensor->getConsumption() << endl;
      cout << "Real Power (kW): " << sensor->getRealPower() << endl;

      if (sensor->isH8036())
        {
          // The H8036 has much more data available...

          cout << "Reactive Power (kVAR): " << sensor->getReactivePower() 
               << endl;
          cout << "Apparent Power (kVA): " << sensor->getApparentPower() 
               << endl;
          cout << "Power Factor: " << sensor->getPowerFactor() 
               << endl;
          cout << "Volts Line to Line: " << sensor->getVoltsLineToLine() 
               << endl;
          cout << "Volts Line to Neutral: " << sensor->getVoltsLineToNeutral() 
               << endl;

          cout << "Current: " << sensor->getCurrent() 
               << endl;

          cout << "Real Power Phase A (kW): " << sensor->getRealPowerPhaseA() 
               << endl;
          cout << "Real Power Phase B (kW): " << sensor->getRealPowerPhaseB() 
               << endl;
          cout << "Real Power Phase C (kW): " << sensor->getRealPowerPhaseC() 
               << endl;

          cout << "Power Factor Phase A: " << sensor->getPowerFactorPhaseA() 
               << endl;
          cout << "Power Factor Phase B: " << sensor->getPowerFactorPhaseB() 
               << endl;
          cout << "Power Factor Phase C: " << sensor->getPowerFactorPhaseC() 
               << endl;

          cout << "Volts Phase A to B: " << sensor->getVoltsPhaseAToB() 
               << endl;
          cout << "Volts Phase B to C: " << sensor->getVoltsPhaseBToC() 
               << endl;
          cout << "Volts Phase A to C: " << sensor->getVoltsPhaseAToC() 
               << endl;
          cout << "Volts Phase A to Neutral: "
               << sensor->getVoltsPhaseAToNeutral() 
               << endl;
          cout << "Volts Phase B to Neutral: "
               << sensor->getVoltsPhaseBToNeutral() 
               << endl;
          cout << "Volts Phase C to Neutral: "
               << sensor->getVoltsPhaseCToNeutral() 
               << endl;

          cout << "Current Phase A: " << sensor->getCurrentPhaseA() 
               << endl;
          cout << "Current Phase B: " << sensor->getCurrentPhaseB() 
               << endl;
          cout << "Current Phase C: " << sensor->getCurrentPhaseC() 
               << endl;

          cout << "Avg Real Power (kW): " << sensor->getAvgRealPower() 
               << endl;
          cout << "Min Real Power (kW): " << sensor->getMinRealPower() 
               << endl;
          cout << "Max Real Power (kW): " << sensor->getMaxRealPower() 
               << endl;
        }

      cout << endl;

      sleep(2);
    }

  cout << "Exiting..." << endl;

  delete sensor;

//! [Interesting]

  return 0;
}
