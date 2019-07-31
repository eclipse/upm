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

import upm_hka5.HKA5;

public class HKA5_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        System.out.println("Initializing...");

        // Instantiate a HKA5 sensor on uart 0.  We don't use the set or
        // reset pins, so we pass -1 for them.
        HKA5 sensor = new HKA5(0, -1, -1);

        // update once every 2 seconds and output data
        while (true)
            {
                sensor.update();

                System.out.println("PM 1  : "
                                   + sensor.getPM1()
                                   + " ug/m3");
                System.out.println("PM 2.5: "
                                   + sensor.getPM2_5()
                                   + " ug/m3");
                System.out.println("PM 10 : "
                                   + sensor.getPM10()
                                   + " ug/m3");

                System.out.println();
                Thread.sleep(2000);
            }

// ! [Interesting]
    }
}
