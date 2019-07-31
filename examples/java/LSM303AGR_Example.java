/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_lsm303agr.LSM303AGR;
import java.util.AbstractList;
import java.lang.Float;

public class LSM303AGR_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a LSM303AGR instance using default i2c bus and address
        LSM303AGR sensor = new LSM303AGR();

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                AbstractList<Float> data = sensor.getAccelerometer();

                System.out.println("Accelerometer x: " + data.get(0)
                                   + " y: " + data.get(1)
                                   + " z: " + data.get(2)
                                   + " g");

                data = sensor.getMagnetometer();
                System.out.println("Magnetometer x: " + data.get(0)
                                   + " y: " + data.get(1)
                                   + " z: " + data.get(2)
                                   + " uT");

                System.out.println("Temperature: "
                                   + sensor.getTemperature());

                System.out.println();
                Thread.sleep(250);
            }

// ! [Interesting]
    }
}
