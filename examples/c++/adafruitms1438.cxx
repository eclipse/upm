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
#include <signal.h>
#include <iostream>
#include "adafruitms1438.h"

using namespace std;
using namespace upm;

int main(int argc, char **argv)
{
//! [Interesting]
  // Instantiate an Adafruit MS 1438 on I2C bus 0

  upm::AdafruitMS1438 *ms = 
    new upm::AdafruitMS1438(ADAFRUITMS1438_I2C_BUS, 
                            ADAFRUITMS1438_DEFAULT_I2C_ADDR);

  // Setup for use with a DC motor connected to the M3 port

  // set a PWM period of 50Hz
  ms->setPWMPeriod(50);

  // disable first, to be safe
  ms->disableMotor(AdafruitMS1438::MOTOR_M3);

  // set speed at 50%
  ms->setMotorSpeed(AdafruitMS1438::MOTOR_M3, 50);
  ms->setMotorDirection(AdafruitMS1438::MOTOR_M3, AdafruitMS1438::DIR_CW);

  cout << "Spin M3 at half speed for 3 seconds, then reverse for 3 seconds."
       << endl;

  ms->enableMotor(AdafruitMS1438::MOTOR_M3);

  sleep(3);

  cout << "Reversing M3" << endl;
  ms->setMotorDirection(AdafruitMS1438::MOTOR_M3, AdafruitMS1438::DIR_CCW);

  sleep(3);

  cout << "Stopping M3" << endl;
  ms->disableMotor(AdafruitMS1438::MOTOR_M3);

  cout << "Exiting" << endl;

//! [Interesting]

  delete ms;
  return 0;
}
