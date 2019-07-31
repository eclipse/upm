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

#include "adafruitms1438.hpp"
#include "upm_utilities.h"

using namespace std;
using namespace upm;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate an Adafruit MS 1438 on I2C bus 0

    upm::AdafruitMS1438 ms(ADAFRUITMS1438_I2C_BUS, ADAFRUITMS1438_DEFAULT_I2C_ADDR);

    // Setup for use with a DC motor connected to the M3 port

    // set a PWM period of 50Hz
    ms.setPWMPeriod(50);

    // disable first, to be safe
    ms.disableMotor(AdafruitMS1438::MOTOR_M3);

    // set speed at 50%
    ms.setMotorSpeed(AdafruitMS1438::MOTOR_M3, 50);
    ms.setMotorDirection(AdafruitMS1438::MOTOR_M3, AdafruitMS1438::DIR_CW);

    cout << "Spin M3 at half speed for 3 seconds, then reverse for 3 seconds." << endl;

    ms.enableMotor(AdafruitMS1438::MOTOR_M3);

    upm_delay(3);

    cout << "Reversing M3" << endl;
    ms.setMotorDirection(AdafruitMS1438::MOTOR_M3, AdafruitMS1438::DIR_CCW);

    upm_delay(3);

    cout << "Stopping M3" << endl;
    ms.disableMotor(AdafruitMS1438::MOTOR_M3);

    cout << "Exiting" << endl;

    //! [Interesting]

    return 0;
}
