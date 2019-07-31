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

import upm_dfrec.DFREC;

public class DFREC_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a DFRobot EC sensor on analog pin A0, with a ds18b20
        // temperature sensor connected to UART 0, and a device index (for
        // the ds1820b uart bus) of 0, and an analog reference voltage of
        // 5.0.
        DFREC sensor = new DFREC(0, 0, 0, 5.0f);

        // Every 2 seconds, update and print values
        while (true)
            {
                sensor.update();

                System.out.println("EC = "
                                   + sensor.getEC()
                                   + " ms/cm");
                System.out.println("Volts = "
                                   + sensor.getVolts()
                                   + ", Temperature = "
                                   + sensor.getTemperature()
                                   + " C");

                System.out.println();
                Thread.sleep(2000);
            }

// ! [Interesting]
    }
}
