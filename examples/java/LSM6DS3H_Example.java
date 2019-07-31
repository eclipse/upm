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

import upm_lsm6ds3h.*;

public class LSM6DS3H_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a LSM6DS3H instance using default i2c bus and address
        LSM6DS3H sensor = new LSM6DS3H();

        // For SPI, bus 0, you would pass -1 as the address, and a
        // valid pin for CS:
        // LSM6DS3H(0, -1, 10);

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                AbstractList<Float> data = sensor.getAccelerometer();

                System.out.println("Accelerometer x: " + data.get(0)
                                   + " y: " + data.get(1)
                                   + " z: " + data.get(2)
                                   + " g");

                data = sensor.getGyroscope();

                System.out.println("Gyroscope x: " + data.get(0)
                                   + " y: " + data.get(1)
                                   + " z: " + data.get(2)
                                   + " dps");

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
