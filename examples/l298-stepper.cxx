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
#include "l298.h"

using namespace std;

int main ()
{
  //! [Interesting]

  // Instantiate a Stepper motor on a L298 Dual H-Bridge.

  // This was tested with the NEMA-17 12V, 350mA, with 200 steps per rev.
  upm::L298* l298 = new upm::L298(200, 3, 4, 7, 8, 9);

  l298->setSpeed(10);            // 10 RPMs
  l298->setDirection(upm::L298::DIR_CW);
  l298->enable(true);
  cout << "Rotating 1 full revolution at 10 RPM speed." << endl;
  // move 200 steps, a full rev
  l298->stepperSteps(200);
  cout << "Sleeping for 2 seconds..." << endl;
  sleep(2);
  cout << "Rotating 1/2 revolution in opposite direction at 10 RPM speed." 
       << endl;
  l298->setDirection(upm::L298::DIR_CCW);
  l298->stepperSteps(100);
  // release
  l298->enable(false);

  //! [Interesting]

  cout << "Exiting..." << endl;

  delete l298;
  return 0;
}
