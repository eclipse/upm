/*
 * Author: Stan Gifford <stan@gifford.id.au>
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


/**
 * Description
 * Demo program for Adafruit 16 channel servo shield/controller
 * Physical setup for tests is a single servo attached to one channel.
 * Note - when 3 or more GWS servos attached results unpredictable.
 * Adafruit do recommend a Cap be installed on the board which should alleviate the issue.
 * I (and Adafruit) are unable to give any Capacitor sizing data.
 */

#include <iostream>
#include "adafruitss.h"
#include <unistd.h>

using namespace std;

int main() {

int n;

//! [Interesting]
  upm::adafruitss* servos = new upm::adafruitss(6,0x40);

  for (;;)
  {
    cout << "Setting all to 0" << endl;
    for (n = 0; n < 16; n++)
        servos->servo(n, 1, 0); // GWS Mini Servo = Type 1.
    usleep(1000000); // Wait 1 second

    cout << "Setting all to 45" << endl;
    for (n = 0; n < 16; n++)
        servos->servo(n, 1, 45);
    usleep(1000000); // Wait 1 second

    cout << "Setting all to 90" << endl;
    for (n = 0; n < 16; n++)
        servos->servo(n, 1, 90);
    usleep(1000000); // Wait 1 second

    cout << "Setting all to 135" << endl;
    for (n = 0; n < 16; n++)
        servos->servo(n, 1, 135);
    usleep(1000000); // Wait 1 second

    cout << "Setting all to 180" << endl;
    for (n = 0; n < 16; n++)
        servos->servo(n, 1, 160);
    usleep(2000000); // Wait 1 second
  }
//! [Interesting]
  return 0;
}
