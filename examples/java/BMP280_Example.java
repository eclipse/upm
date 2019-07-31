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

import upm_bmp280.BMP280;

public class BMP280_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a BMP280 instance using default i2c bus and address
        BMP280 sensor = new BMP280();

        // For SPI, bus 0, you would pass -1 as the address, and a
        // valid pin for CS:
        // BMP280(0, -1, 10);

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                System.out.println("Compensation Temperature: "
                                   + sensor.getTemperature()
                                   + " C / "
                                   + sensor.getTemperature(true)
                                   + " F");

                System.out.println("Pressure: "
                                   + sensor.getPressure()
                                   + " Pa");
                
                System.out.println("Computed Altitude: "
                                   + sensor.getAltitude()
                                   + " m");

                System.out.println();
                Thread.sleep(1000);
            }

// ! [Interesting]
    }
}
