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

import upm_ozw.TZEMT400;

public class TZEMT400_Example
{
    private static String defaultDev = "/dev/ttyACM0";

    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        if (args.length > 0)
            defaultDev = args[0];
        System.out.println("Using device " + defaultDev);

        // Instantiate a TZEMT400 instance, on device node 13.  You
        // will almost certainly need to change this to reflect your
        // own network.  Use the ozwdump example to see what nodes are
        // available.
        TZEMT400 sensor = new TZEMT400(13);

        // The first thing to do is create options, then lock them when done.
        sensor.optionsCreate();
        sensor.optionsLock();

        System.out.println("Initializing, this may take awhile depending "
                           + "on your ZWave network");

        sensor.init(defaultDev);
        System.out.println("Initialization complete");

        System.out.println("Querying data...");

        while (true)
            {
                sensor.update();

                System.out.println("Temperature: "
                                   + sensor.getTemperature()
                                   + " C / "
                                   + sensor.getTemperature(true)
                                   + " F");

                System.out.println("Mode: "
                                   + sensor.getMode());

                System.out.println("Operating State: "
                                   + sensor.getOperatingState());

                System.out.println("Heating Point: "
                                   + sensor.getHeatingPointTemperature()
                                   + " C / "
                                   + sensor.getHeatingPointTemperature(true)
                                   + " F");

                System.out.println("Cooling Point: "
                                   + sensor.getCoolingPointTemperature()
                                   + " C / "
                                   + sensor.getCoolingPointTemperature(true)
                                   + " F");

                System.out.println("Fan Mode: "
                                   + sensor.getFanMode());

                System.out.println("Fan State: "
                                   + sensor.getFanState());

                System.out.println();

                Thread.sleep(5000);
            }

// ! [Interesting]
    }
}
