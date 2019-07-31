/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_bmp280.*;
import upm_interfaces.*;

public class BME280_Interface_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a BME280 instance using default i2c bus and address
        BME280 sensor = new BME280();

        iTemperature t_sensor = sensor;
        iHumidity h_sensor = sensor;
        iPressure p_sensor = sensor;

        // For SPI, bus 0, you would pass -1 as the address, and a
        // valid pin for CS:
        // BME280(0, -1, 10);

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                System.out.println("Calling Interface Functions: ");

                System.out.println("Compensation Temperature: "
                                   + t_sensor.getTemperature()
                                   + " C / ");

                System.out.println("Pressure: "
                                   + p_sensor.getPressure()
                                   + " Pa");

                System.out.println("Humidity: "
                                   + h_sensor.getHumidity()
                                   + " %RH");

                System.out.println();
                Thread.sleep(1000);
            }

// ! [Interesting]
    }
}
