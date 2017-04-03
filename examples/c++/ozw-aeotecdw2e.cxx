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

#include "aeotecdw2e.hpp"

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

  string defaultDev = "/dev/ttyACM0";

  // if an argument was specified, use it as the device instead
  if (argc > 1)
    defaultDev = string(argv[1]);

  // Instantiate an Aeotec Door/Window 2nd Edition sensor instance, on
  // device node 10.  You will almost certainly need to change this to
  // reflect your own network.  Use the ozwdump example to see what
  // nodes are available.
  upm::AeotecDW2E *sensor = new upm::AeotecDW2E(10);

  // The first thing to do is create options, then lock them when done.
  sensor->optionsCreate();
  sensor->optionsLock();

  // Next, initialize it.
  cout << "Initializing, this may take awhile depending on your ZWave network"
       << endl;

  sensor->init(defaultDev);
  cout << "Initialization complete" << endl;

  cout << "Querying data..." << endl;
  while (shouldRun)
    {
      if (sensor->isDeviceAvailable())
        {
          cout << "Alarm status: "
               << sensor->isAlarmTripped()
               << endl;

          cout << "Tamper Switch status: "
               << sensor->isTamperTripped()
               << endl;

          cout << "Battery Level: "
               << sensor->getBatteryLevel()
               << "%"
               << endl;
          cout << endl;
        }
      else
        {
          cout << "Device has not yet responded to probe."
               << endl;

          cout << "Try waking it, or wait until it wakes itself if configured"
               << " to do so."
               << endl;
          cout << endl;
        }

      sleep(1);
    }

  cout << "Exiting..." << endl;

  delete sensor;

  return 0;
}
