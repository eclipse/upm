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

import upm_lsm303d.LSM303D;
import java.util.AbstractList;
import java.lang.Float;

public class LSM303D_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a LSM303D instance using default i2c bus and address
        LSM303D sensor = new LSM303D();

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
