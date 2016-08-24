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
#include <signal.h>
#include <iostream>

#include "mma7361.hpp"

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

  // Instantiate a MMA7361 sensor on analog pins A0 (X), A1 (Y) A2
  // (Z), selftest pin on D2, sleep pin on D3 nd an analog reference
  // value of 5.0.  The freefall pin and the range pin are unused
  // (-1).
  upm::MMA7361 *sensor = new upm::MMA7361(0, 1, 2, 2, 3, -1, -1, 5.0);

  // 1.5g (true = 6g)
  sensor->setRange(false);

  // Every 10th of a second, update and print values

  while (shouldRun)
    {
      sensor->update();

      float x, y, z;

      sensor->getAcceleration(&x, &y, &z);
      cout << "Acceleration x = "
           << x
           << " y = "
           << y
           << "z = "
           << z
           << endl;

      sensor->getVolts(&x, &y, &z);
      cout << "Volts x = "
           << x
           << " y = "
           << y
           << "z = "
           << z
           << endl;

      cout << endl;

      usleep(100000);
    }

//! [Interesting]

  printf("Exiting...\n");

  delete sensor;

  return 0;
}
