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
#include "urm37.h"

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

  // Instantiate a URM37 sensor on analog pin A0, reset pin on D2,
  // trigger pin on D3 with an analog reference voltage of 5.0
  upm::URM37 *sensor = new upm::URM37(0, 2, 3, 5.0);

  // Every half a second, sample the URM37 and output the measured
  // distance in cm.

  while (shouldRun)
    {
      cout << "Detected distance (cm): " << sensor->getDistance() << endl;
      usleep(500000);
    }

//! [Interesting]

  cout << "Exiting" << endl;

  delete sensor;
  return 0;
}
