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
#include "pca9685.h"

using namespace std;

int main(int argc, char **argv)
{
//! [Interesting]
  // Instantiate an PCA9685 on I2C bus 0

  upm::PCA9685 *leds = new upm::PCA9685(PCA9685_I2C_BUS, 
                                        PCA9685_DEFAULT_I2C_ADDR);

  // put device to sleep
  leds->setModeSleep(true);

  // setup a period of 50Hz
  leds->setPrescaleFromHz(50);
  
  // wake device up
  leds->setModeSleep(false);

  // Setup a 50% duty cycle -- on time at 0, off time at 2048 (4096 / 2)
  // Set for all channels

  leds->ledOnTime(PCA9685_ALL_LED, 0);
  leds->ledOffTime(PCA9685_ALL_LED, 2048);

  // but, turn channel 3 full off and channel 4 full on

  cout << "Turning channel 3 off, and channel 4 on." << endl;
  cout << "All other channels will be PWM'd at a 50% duty cycle." << endl;

  leds->ledFullOff(3, true);
  leds->ledFullOn(4, true);

  // now, just sleep for 5 seconds, reset channels 3 and 4, and exit.
  cout << "Sleeping for 5 seconds..." << endl;

  sleep(5);

  cout << "Exiting..." << endl;

  // clear the bits we set earlier
  leds->ledFullOff(3, false);
  leds->ledFullOn(4, false);

//! [Interesting]

  delete leds;
  return 0;
}
