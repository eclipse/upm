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
#include "uln200xa.h"

using namespace std;

int main ()
{
  //! [Interesting]

  // Instantiate a Stepper motor on a ULN200XA Dual H-Bridge.

  // This was tested with the Grove Gear Stepper Motor with Driver

  // Wire the pins so that I1 is pin D8, I2 is pin D9, I3 is pin D10 and
  // I4 is pin D11
  upm::ULN200XA* uln200xa = new upm::ULN200XA(4096, 8, 9, 10, 11);

  uln200xa->setSpeed(5);
  uln200xa->setDirection(upm::ULN200XA::DIR_CW);
  cout << "Rotating 1 revolution clockwise." << endl;
  uln200xa->stepperSteps(4096);
  cout << "Sleeping for 2 seconds..." << endl;
  sleep(2);
  cout << "Rotating 1/2 revolution counter clockwise." << endl;
  uln200xa->setDirection(upm::ULN200XA::DIR_CCW);
  uln200xa->stepperSteps(2048);

  // turn off the power
  uln200xa->release();

  //! [Interesting]
  cout << "Exiting..." << endl;

  delete uln200xa;
  return 0;
}

