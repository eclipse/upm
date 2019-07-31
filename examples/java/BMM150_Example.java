/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_interfaces.*;
import upm_bmm150.BMM150;
import java.util.AbstractList;
import java.lang.Float;


public class BMM150_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a BMM150 instance using default i2c bus and address
        BMM150 sensor = new BMM150();

        // For SPI, bus 0, you would pass -1 as the address, and a
        // valid pin for CS:
        // BMM150(0, -1, 10);

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                AbstractList<Float> data = sensor.getMagnetometer();

                System.out.println("Magnetometer x: " + data.get(0)
                                   + " y: " + data.get(1)
                                   + " z: " + data.get(2)
                                   + " uT");

                System.out.println();
                Thread.sleep(250);
            }

// ! [Interesting]
    }
}
