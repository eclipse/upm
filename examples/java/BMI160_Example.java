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

import java.util.AbstractList;
import upm_interfaces.*;

import upm_bmi160.BMI160;

public class BMI160_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        System.out.println("Initializing...");

        // Instantiate a BMI160 instance using default i2c bus and address
        BMI160 sensor = new BMI160();

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                AbstractList<Float> dataA = sensor.getAcceleration();

                System.out.println("Accelerometer: "
                                   + "AX: "
                                   + dataA.get(0)
                                   + " AY: "
                                   + dataA.get(1)
                                   + " AZ: "
                                   + dataA.get(2));

                AbstractList<Float> dataG = sensor.getGyroscope();

                System.out.println("Gryoscope:     "
                                   + "GX: "
                                   + dataG.get(0)
                                   + " GY: "
                                   + dataG.get(1)
                                   + " GZ: "
                                   + dataG.get(2));

                AbstractList<Float> dataM = sensor.getMagnetometer();

                System.out.println("Magnetometer:  "
                                   + "MX: "
                                   + dataM.get(0)
                                   + " MY: "
                                   + dataM.get(1)
                                   + " MZ: "
                                   + dataM.get(2));

                System.out.println();
                Thread.sleep(500);
            }

// ! [Interesting]
    }
}
