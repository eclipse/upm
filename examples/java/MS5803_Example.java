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

import upm_ms5803.MS5803;

public class MS5803_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a MS5803 instance using bus 0 and default i2c address
        MS5803 sensor = new MS5803(0);

        // For SPI, bus 0, you would pass -1 as the address, and a
        // valid pin for CS:
        // MS5803(0, -1, 10);

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                System.out.println("Temperature: "
                                   + sensor.getTemperature()
                                   + " C, Pressure: "
                                   + sensor.getPressure()
                                   + " mbar");

                Thread.sleep(1000);
            }

// ! [Interesting]
    }
}
