/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>
#include <string>

#include "t3311.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    string defaultDev = "/dev/ttyUSB0";

    // if an argument was specified, use it as the device instead
    if (argc > 1)
        defaultDev = string(argv[1]);

    cout << "Initializing..." << endl;

    // Instantiate an T3311 instance, using MODBUS slave address 1, and
    // default comm parameters (9600, 8, N, 2)
    upm::T3311 sensor(defaultDev, 1);

    // output the serial number and firmware revision
    cout << "Serial Number: " << sensor.getSerialNumber() << endl;
    cout << "Firmware Revision: " << sensor.getFirmwareMajor() << "." << sensor.getFirmwareMinor()
         << endl;
    cout << endl;

    // update and print available values every second
    while (shouldRun) {
        // update our values from the sensor
        sensor.update();

        // we show both C and F for temperature
        cout << "Temperature: " << sensor.getTemperature() << " C / " << sensor.getTemperature(true)
             << " F" << endl;

        cout << "Humidity: " << sensor.getHumidity() << " %" << endl;

        // this value depends on the sensor configuration -- by default
        // it is the dew point temperature
        cout << "Computed Value: " << sensor.getComputedValue() << endl;

        // with FW revisions > 2.44, extended computed data is available
        if (sensor.extendedDataAvailable()) {
            cout << "Dew Point Temperature: " << sensor.getDewPointTemperature() << " C / "
                 << sensor.getDewPointTemperature(true) << " F" << endl;

            cout << "Absolute Humidity: " << sensor.getAbsoluteHumidity() << " g/m3" << endl;

            cout << "Specific Humidity: " << sensor.getSpecificHumidity() << " g/kg" << endl;

            cout << "Mixing Ratio: " << sensor.getMixingRatio() << " g/kg" << endl;

            cout << "Specific Enthalpy: " << sensor.getSpecificEnthalpy() << " kJ/kg" << endl;
        }

        cout << endl;

        upm_delay(1);
    }

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
