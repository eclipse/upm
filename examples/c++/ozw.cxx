/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include "ozw.hpp"

using namespace std;

int main(int argc, char **argv)
{
//! [Interesting]

  string defaultDev = "/dev/ttyACM0";

  // if an argument was specified, use it as the device instead
  if (argc > 1)
    defaultDev = string(argv[1]);

  // Instantiate an OZW instance
  upm::OZW *sensor = new upm::OZW();

  // The first thing to do is create options, then lock them when done.
  sensor->optionsCreate();
  sensor->optionsLock();

  // Next, initialize it.
  cout << "Initializing, this may take awhile depending on your ZWave network" 
       << endl;

  if (!sensor->init(defaultDev))
    {
      cerr << "Init failed." << endl;
      return 0;
    }
  cout << "Initialization complete" << endl;

  cout << "Dumping nodes..." << endl;

  sensor->dumpNodes();

  // The following is example output of dumpNodes:
  //
  // Dumping nodes...
  // Node 1: Z-Stick Gen5
  // Node 2: Smart Switch 6
  //    Index:  0, Type: bool, Label: Switch, Value: False 
  //    Index:  2, Type: float, Label: Energy, Value: 1.190 kWh
  //    Index:  3, Type: float, Label: Previous Reading, Value: 1.190 kWh
  //    Index:  4, Type: int32, Label: Interval, Value: 1521 seconds
  //    Index:  5, Type: float, Label: Power, Value: 0.000 W
  //    Index:  6, Type: float, Label: Voltage, Value: 121.256 V
  //    Index:  7, Type: float, Label: Current, Value: 0.000 A
  //    Index:  8, Type: bool, Label: Exporting, Value: False 
  //    Index: 45, Type: list, Label: Day, Value: Friday 
  //    Index: 46, Type: byte, Label: Hour, Value: 5 
  //    Index: 47, Type: byte, Label: Minute, Value: 53 
  // Node 3: Multi Sensor
  //    Index:  0, Type: bool, Label: Sensor, Value: True 
  //    Index:  1, Type: float, Label: Temperature, Value: 72.8 F
  //    Index:  2, Type: float, Label: Luminance, Value: 4 lux
  //    Index:  3, Type: float, Label: Relative Humidity, Value: 22 %
  //    Index: 17, Type: byte, Label: Battery Level, Value: 98 %
  // Node 5: Minimote
  // Node 6: Smart Energy Switch
  //    Index:  0, Type: bool, Label: Switch, Value: False 
  //    Index:  2, Type: float, Label: Power, Value: 0.000 W
  //    Index:  3, Type: float, Label: Energy, Value: 1.609 kWh
  //    Index:  4, Type: float, Label: Previous Reading, Value: 1.609 kWh
  //    Index:  5, Type: int32, Label: Interval, Value: 1521 seconds
  //    Index:  6, Type: float, Label: Power, Value: 0.000 W
  //    Index:  7, Type: float, Label: Previous Reading, Value: 1.609 W
  //    Index:  8, Type: int32, Label: Interval, Value: 1521 seconds
  //    Index:  9, Type: bool, Label: Exporting, Value: False 
  // Node 7: Smart Energy Switch
  //    Index:  0, Type: bool, Label: Switch, Value: False 
  //    Index:  2, Type: float, Label: Power, Value: 0.000 W
  //    Index:  3, Type: float, Label: Energy, Value: 0.000 kWh
  //    Index:  4, Type: float, Label: Previous Reading, Value: 0.000 kWh
  //    Index:  5, Type: int32, Label: Interval, Value: 1521 seconds
  //    Index:  6, Type: float, Label: Power, Value: 0.000 W
  //    Index:  7, Type: float, Label: Previous Reading, Value: 0.000 W
  //    Index:  8, Type: int32, Label: Interval, Value: 1521 seconds
  //    Index:  9, Type: bool, Label: Exporting, Value: False 
  //
  // So, with the above in mind:
  // 
  // 1. Query the temperature on node 3 and print it out (as a
  // string), along with the units of measure:
  //
  // cout << "Temperature: " << sensor->getValueAsString(3, 1) 
  //      << " " << sensor->getValueUnits(3, 1) << endl;
  //
  // 2. query the same temperature as a float:
  // 
  // float temperature = sensor->getValueAsFloat(3, 1);
  //
  // 3. Turn on the light plugged into the switch on Node 7, wait 5
  // seconds, then turn it back off again:
  // 
  // cout << "Turning ON node 7" << endl;
  // sensor->setValueAsBool(7, 0, true);
  //
  // cout << "Sleeping for 5 seconds" << endl;
  // sleep(5);
  //
  // cout << "Turning OFF node 7" << endl;
  // sensor->setValueAsBool(7, 0, false);


//! [Interesting]


  cout << "Exiting..." << endl;
  
  delete sensor;
  
  return 0;
}
