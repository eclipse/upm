/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
