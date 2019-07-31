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

    // Setup for use with a stepper motor connected to the M1 & M2 ports

    // set a PWM period of 50Hz

    // disable first, to be safe
    ms.disableStepper(AdafruitMS1438::STEPMOTOR_M12);

    // configure for a NEMA-17, 200 steps per revolution
    ms.stepConfig(AdafruitMS1438::STEPMOTOR_M12, 200);

    // set speed at 10 RPM's
    ms.setStepperSpeed(AdafruitMS1438::STEPMOTOR_M12, 10);
    ms.setStepperDirection(AdafruitMS1438::STEPMOTOR_M12, AdafruitMS1438::DIR_CW);

    // enable
    cout << "Enabling..." << endl;
    ms.enableStepper(AdafruitMS1438::STEPMOTOR_M12);

    cout << "Rotating 1 full revolution at 10 RPM speed." << endl;
    ms.stepperSteps(AdafruitMS1438::STEPMOTOR_M12, 200);

    cout << "Sleeping for 2 seconds..." << endl;
    upm_delay(2);
    cout << "Rotating 1/2 revolution in opposite direction at 10 RPM speed." << endl;

    ms.setStepperDirection(AdafruitMS1438::STEPMOTOR_M12, AdafruitMS1438::DIR_CCW);
    ms.stepperSteps(AdafruitMS1438::STEPMOTOR_M12, 100);

    cout << "Disabling..." << endl;
    ms.disableStepper(AdafruitMS1438::STEPMOTOR_M12);

    cout << "Exiting" << endl;

    //! [Interesting]

    return 0;
}
