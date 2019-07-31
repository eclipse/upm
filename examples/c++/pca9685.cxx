/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "pca9685.hpp"
#include "upm_utilities.h"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate an PCA9685 on I2C bus 0

    upm::PCA9685 leds(PCA9685_I2C_BUS, PCA9685_DEFAULT_I2C_ADDR);

    // put device to upm_delay
    leds.setModeSleep(true);

    // setup a period of 50Hz
    leds.setPrescaleFromHz(50);

    // wake device up
    leds.setModeSleep(false);

    // Setup a 50% duty cycle -- on time at 0, off time at 2048 (4096 / 2)
    // Set for all channels

    leds.ledOnTime(PCA9685_ALL_LED, 0);
    leds.ledOffTime(PCA9685_ALL_LED, 2048);

    // but, turn channel 3 full off and channel 4 full on

    cout << "Turning channel 3 off, and channel 4 on." << endl;
    cout << "All other channels will be PWM'd at a 50% duty cycle." << endl;

    leds.ledFullOff(3, true);
    leds.ledFullOn(4, true);

    // now, just upm_delay for 5 seconds, reset channels 3 and 4, and exit.
    cout << "Sleeping for 5 seconds..." << endl;

    upm_delay(5);

    cout << "Exiting..." << endl;

    // clear the bits we set earlier
    leds.ledFullOff(3, false);
    leds.ledFullOn(4, false);

    //! [Interesting]

    return 0;
}
