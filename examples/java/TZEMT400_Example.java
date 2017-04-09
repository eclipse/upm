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
