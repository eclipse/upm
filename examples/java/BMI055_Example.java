/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_bmx055.BMI055;
import java.util.AbstractList;
import java.lang.Float;

public class BMI055_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a BMI055 instance using default i2c bus and address
        BMI055 sensor = new BMI055();

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
                                   + " degrees/s");

                System.out.println();
                Thread.sleep(250);
            }

// ! [Interesting]
    }
}
