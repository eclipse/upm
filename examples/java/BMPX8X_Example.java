/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class BMPX8X_Example {

    public static void main(String[] args) throws InterruptedException {
// ! [Interesting]

        // Instantiate a BMPX8X sensor on I2C using defaults.
        upm_bmpx8x.BMPX8X sensor = new upm_bmpx8x.BMPX8X();

        // Print the pressure, altitude, sea level, and
        // temperature values every .5 seconds
        while (true)
        {
            System.out.println("Pressure: "
                               + sensor.getPressure()
                               + " Pa, Temperature: "
                               + sensor.getTemperature()
                               + " C, Altitude: "
                               + sensor.getAltitude()
                               + " m, Sea level: "
                               + sensor.getSealevelPressure()
                               + " Pa");

            Thread.sleep(500);
        }
// ! [Interesting]
    }

}
