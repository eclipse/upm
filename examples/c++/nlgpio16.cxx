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
#include <stdio.h>
#include "nlgpio16.hpp"

using namespace std;
using namespace upm;

int main(int argc, char **argv)
{
  // Instantiate a NLGPIO16 Module on the default UART (/dev/ttyACM0)
  upm::NLGPIO16* sensor = new upm::NLGPIO16();

  // get the Version
  cout << "Device Version: " << sensor->getVersion() << endl;
  // read the gpio at pin 3
  cout << "GPIO 3 Value: " << sensor->gpioRead(3) << endl;
  // read the analog voltage at pin 5
  cout << "Analog 5 Voltage: " << sensor->analogReadVolts(5) << endl;
  // set the gpio at pin 14 to HIGH
  sensor->gpioSet(14);

  delete sensor;
  return 0;
}

//! [Interesting]
