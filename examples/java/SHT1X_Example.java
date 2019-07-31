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

import upm_sht1x.SHT1X;
import upm_interfaces.*;

public class SHT1X_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a SHT1X sensor using D2 as the clock, and D3 as the
        // data pin.
        iTemperature temp = new SHT1X(2, 3);
        SHT1X sensor = new SHT1X(2, 3);


        // Every 2 seconds, update and print values
        while (true)
            {
                sensor.update();

                System.out.println("Temperature: "
                                   + sensor.getTemperature()
                                   + " C");
                System.out.println("Humidity:    "
                                   + sensor.getHumidity()
                                   + " RH");

                System.out.println();
                Thread.sleep(2000);
            }

// ! [Interesting]
    }
}
