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

#include "hka5.hpp"

using namespace std;

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main()
{
  signal(SIGINT, sig_handler);

//! [Interesting]

  // Instantiate a HKA5 sensor on uart 0.  We don't use the set or
  // reset pins, so we pass -1 for them.
  upm::HKA5 *sensor = new upm::HKA5(0, -1, -1);

  // update once every 2 seconds and output data
  while (shouldRun)
    {
      sensor->update();

      cout << "PM 1  : "
           << sensor->getPM1()
           << " ug/m3"
           << endl;

      cout << "PM 2.5: "
           << sensor->getPM2_5()
           << " ug/m3"
           << endl;

      cout << "PM 10 : "
           << sensor->getPM10()
           << " ug/m3"
           << endl;

      cout << endl;

      sleep(2);
    }

  if (shouldRun)
    cerr << "Timed out" << endl;

//! [Interesting]

  cout << "Exiting" << endl;

  delete sensor;

  return 0;
}
