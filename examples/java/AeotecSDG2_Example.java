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

import upm_ozw.AeotecSDG2;

public class AeotecSDG2_Example
{
    private static String defaultDev = "/dev/ttyACM0";

    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        if (args.length > 0)
            defaultDev = args[0];
        System.out.println("Using device " + defaultDev);

        // Instantiate an Aeotec Smart Dimmer Gen2 instance, on device node
        // 9.  You will almost certainly need to change this to reflect your
        // own network.  Use the ozwdump example to see what nodes are
        // available.
        AeotecSDG2 sensor = new AeotecSDG2(9);

        // The first thing to do is create options, then lock them when done.
        sensor.optionsCreate();
        sensor.optionsLock();

        System.out.println("Initializing, this may take awhile depending "
                           + "on your ZWave network");

        sensor.init(defaultDev);
        System.out.println("Initialization complete");

        // turn light on
        System.out.println("Turning switch on, then sleeping for 5 secs");
        sensor.on();
        Thread.sleep(5000);

        System.out.println("Querying data...");

        boolean dim = false;
        while (true)
            {
                // put on a light show...
                if (dim)
                    sensor.setLevel(25);
                else
                    sensor.on();

                dim = !dim;

                sensor.update();

                System.out.println("Switch status: "
                                   + sensor.isOn());

                System.out.println("Volts: "
                                   + sensor.getVolts()
                                   + " volts");

                System.out.println("Energy Consumption: "
                                   + sensor.getEnergy()
                                   + " kWh");

                System.out.println("Watts: "
                                   + sensor.getWatts());

                System.out.println("Current: "
                                   + sensor.getCurrent()
                                   + " amps");

                System.out.println();

                Thread.sleep(5000);
            }

// ! [Interesting]
    }
}
