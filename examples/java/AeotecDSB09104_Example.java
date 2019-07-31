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

import upm_ozw.AeotecDSB09104;

public class AeotecDSB09104_Example
{
    private static String defaultDev = "/dev/ttyACM0";

    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        if (args.length > 0)
            defaultDev = args[0];
        System.out.println("Using device " + defaultDev);

        // Instantiate an Aeotec DSB09104 instance, on device node 12.
        // You will almost certainly need to change this to reflect
        // your own network.  Use the ozwdump example to see what
        // nodes are available.
        AeotecDSB09104 sensor = new AeotecDSB09104(12);

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

                System.out.println("Watts, Channel 1: "
                                   + sensor.getWattsC1()
                                   + " W");

                System.out.println("Watts, Channel 2: "
                                   + sensor.getWattsC2()
                                   + " W");

                System.out.println("Watts, Channel 3: "
                                   + sensor.getWattsC3()
                                   + " W");

                System.out.println("Energy, Channel 1: "
                                   + sensor.getEnergyC1()
                                   + " kWh");

                System.out.println("Energy, Channel 2: "
                                   + sensor.getEnergyC2()
                                   + " kWh");

                System.out.println("Energy, Channel 3: "
                                   + sensor.getEnergyC3()
                                   + " kWh");

                System.out.println("Battery Level: "
                                   + sensor.getBatteryLevel()
                                   + "%");

                System.out.println();

                Thread.sleep(3000);
            }

// ! [Interesting]
    }
}
