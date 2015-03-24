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

  // Setup for use with a stepper motor connected to the M1 & M2 ports

  // set a PWM period of 50Hz

  // disable first, to be safe
  ms->disableStepper(AdafruitMS1438::STEPMOTOR_M12);

  // configure for a NEMA-17, 200 steps per revolution
  ms->stepConfig(AdafruitMS1438::STEPMOTOR_M12, 200);

  // set speed at 10 RPM's
  ms->setStepperSpeed(AdafruitMS1438::STEPMOTOR_M12, 10);
  ms->setStepperDirection(AdafruitMS1438::STEPMOTOR_M12, 
                          AdafruitMS1438::DIR_CW);

  // enable
  cout << "Enabling..." << endl;
  ms->enableStepper(AdafruitMS1438::STEPMOTOR_M12);

  cout << "Rotating 1 full revolution at 10 RPM speed." << endl;
  ms->stepperSteps(AdafruitMS1438::STEPMOTOR_M12, 200);

  cout << "Sleeping for 2 seconds..." << endl;
  sleep(2);
  cout << "Rotating 1/2 revolution in opposite direction at 10 RPM speed." 
       << endl;

  ms->setStepperDirection(AdafruitMS1438::STEPMOTOR_M12, 
                          AdafruitMS1438::DIR_CCW);
  ms->stepperSteps(AdafruitMS1438::STEPMOTOR_M12, 100);

  cout << "Disabling..." << endl;
  ms->disableStepper(AdafruitMS1438::STEPMOTOR_M12);

  cout << "Exiting" << endl;

//! [Interesting]

  delete ms;
  return 0;
}
