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
#include <signal.h>
#include "mhz16.h"

using namespace std;

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main (int argc, char **argv)
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate a MHZ16 serial CO2 sensor on uart 0.
  // This example was tested on the Grove CO2 sensor module.

  upm::MHZ16* co2 = new upm::MHZ16(0);
  
  // make sure port is initialized properly.  9600 baud is the default.
  if (!co2->setupTty(B9600))
    {
      cerr << "Failed to setup tty port parameters" << endl;
      return 1;
    }
  
  cout << "Make sure that the sensor has had at least 3 minutes to warm up" 
       << endl;
  cout << "or you will not get valid results." << endl;
  cout << "The temperature reported is not the ambient temperature, " << endl;
  cout << "but rather the temperature of the sensor elements." << endl;

  sleep(1);

  int gas;
  int temp;

  while (shouldRun)
    {
      co2->getData(&gas, &temp);
      cout << "CO2 concentration: " << gas << " PPM, "
           << "Temperature (in C): " << temp <<endl;

      sleep(2);
    }

//! [Interesting]

  cout << "Exiting..." << endl;

  delete co2;
  return 0;
}
