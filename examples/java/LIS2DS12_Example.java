/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import java.util.AbstractList;

import upm_lis2ds12.*;

public class LIS2DS12_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a LIS2DS12 instance using default i2c bus and address
        LIS2DS12 sensor = new LIS2DS12();

        // For SPI, bus 0, you would pass -1 as the address, and a
        // valid pin for CS:
        // LIS2DS12(0, -1, 10);

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                AbstractList<Float> dataA = sensor.getAccelerometer();

                System.out.println("Accelerometer x: " + dataA.get(0)
                                   + " y: " + dataA.get(1)
                                   + " z: " + dataA.get(2)
                                   + " g");

                System.out.println("Compensation Temperature: "
                                   + sensor.getTemperature()
                                   + " C / "
                                   + sensor.getTemperature(true)
                                   + " F");

                System.out.println();
                Thread.sleep(250);
            }

// ! [Interesting]
    }
}
