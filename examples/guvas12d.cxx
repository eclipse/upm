/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include "guvas12d.h"

using namespace std;

bool shouldRun = true;

// analog voltage, usually 3.3 or 5.0
#define GUVAS12D_AREF   5.0
#define SAMPLES_PER_QUERY 1024

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main()
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // The was tested with the Grove UV Sensor module.
  // It has a sensing range from between 200-400nm.  It's strongest
  // response is around 320-360nm.

  // Instantiate a GUVAS12D on analog pin A0
  upm::GUVAS12D *volts = new upm::GUVAS12D(0);
  
  // The higher the voltage the more intense the UV radiation.

  while (shouldRun)
    {
      cout << "AREF: " << GUVAS12D_AREF 
           << ", Voltage value (higher means more UV): " 
           << volts->value(GUVAS12D_AREF, SAMPLES_PER_QUERY) << endl;
      
      sleep(1);
    }
//! [Interesting]

  cout << "Exiting" << endl;

  delete volts;
  return 0;
}
