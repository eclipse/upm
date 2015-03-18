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
#include <signal.h>
#include <iostream>
#include "grovemd.h"

using namespace std;

int main(int argc, char **argv)
{
  //! [Interesting]
  // Instantiate an I2C Grove Motor Driver on I2C bus 0

  upm::GroveMD *motors = new upm::GroveMD(GROVEMD_I2C_BUS, 
                                          GROVEMD_DEFAULT_I2C_ADDR);

  // set direction to CW and set speed to 50%
  cout << "Spin M1 and M2 at half speed for 3 seconds" << endl;
  motors->setMotorDirections(upm::GroveMD::DIR_CW, upm::GroveMD::DIR_CW);
  motors->setMotorSpeeds(127, 127);
  
  sleep(3);
  // counter clockwise
  cout << "Reversing M1 and M2 for 3 seconds" << endl;
  motors->setMotorDirections(upm::GroveMD::DIR_CCW, upm::GroveMD::DIR_CCW);
  sleep(3);

  //! [Interesting]

  cout << "Stopping motors" << endl;
  motors->setMotorSpeeds(0, 0);

  cout << "Exiting..." << endl;

  delete motors;
  return 0;
}
