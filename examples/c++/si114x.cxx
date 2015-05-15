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
#include "si114x.h"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}


int main ()
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate a SI114x UV Sensor on I2C bus 0
  upm::SI114X* uvi = new upm::SI114X(0);
  
  // First initialize it
  uvi->initialize();

  cout << "UV Index Scale:" << endl;
  cout << "---------------" << endl;
  cout << "11+        Extreme" << endl;
  cout << "8-10       Very High" << endl;
  cout << "6-7        High" << endl;
  cout << "3-5        Moderate" << endl;
  cout << "0-2        Low" << endl;
  cout << endl;

  // update every second and print the currently measured UV Index
  while (shouldRun)
    {
      // update current value(s)
      uvi->update();

      // print detected value
      cout << "UV Index: " << uvi->getUVIndex() << endl;

      sleep(1);
    }
//! [Interesting]

  cout << "Exiting..." << endl;

  delete uvi;
  return 0;
}
