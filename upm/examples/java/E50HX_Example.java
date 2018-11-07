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

import upm_e50hx.E50HX;

public class E50HX_Example
{
    private static String defaultDev = "/dev/ttyUSB0";

    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // You will need to edit this example to conform to your site
        // and your devices, specifically the Device Object Instance
        // ID passed to the constructor, and the arguments to
        // initMaster() that are appropriate for your BACnet network.

        if (args.length > 0)
            defaultDev = args[0];
        System.out.println("Using device " + defaultDev);
        System.out.println("Initializing...");

        // Instantiate an E50HX object for an E50HX device that has
        // 1075425 as it's unique Device Object Instance ID.  NOTE:
        // You will certainly want to change this to the correct value
        // for your device(s).
        E50HX sensor = new E50HX(1075425);

        // Initialize our BACnet master, if it has not already been
        // initialized, with the device and baudrate, choosing 1000001 as
        // our unique Device Object Instance ID, 2 as our MAC address and
        // using default values for maxMaster and maxInfoFrames
        sensor.initMaster(defaultDev, 38400, 1000001, 2);

        // Uncomment to enable debugging output
        // sensor.setDebug(true);

        System.out.println();
        System.out.println("Device Description: "
                           + sensor.getDeviceDescription());
        System.out.println("Device Location: "
                           + sensor.getDeviceLocation());
        System.out.println();

        // update and print a few values every 5 seconds
        while (true)
            {
                System.out.println("System Voltage: "
                                   + sensor.getAnalogValue(E50HX.ANALOG_VALUES_T.AV_System_Voltage)
                                   + " "
                                   + sensor.getAnalogValueUnits(E50HX.ANALOG_VALUES_T.AV_System_Voltage));

                System.out.println("System Type: "
                                   + sensor.getAnalogValue(E50HX.ANALOG_VALUES_T.AV_System_Type));

                System.out.println("Energy Consumption: "
                                   + sensor.getAnalogInput(E50HX.ANALOG_INPUTS_T.AI_Energy)
                                   + " "
                                   + sensor.getAnalogInputUnits(E50HX.ANALOG_INPUTS_T.AI_Energy));

                System.out.println("Power Up Counter: "
                                   + sensor.getAnalogInput(E50HX.ANALOG_INPUTS_T.AI_Power_Up_Count));

                System.out.println();
                Thread.sleep(5000);
            }
// ! [Interesting]
    }
}
