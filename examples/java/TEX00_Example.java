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

import upm_tex00.TEX00;

public class TEX00_Example 
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        System.out.println("Initializing...");

        // Instantiate an TEX00 instance, using A0 for the analog input.  In
        // this example, we are using a 10K Ohm balance resistor and a TED
        // (10k type 2) thermistor.
        TEX00 sensor = new TEX00(0, 10000.0f, 
                                 TEX00.SENSOR_TYPES_T.STYPE_THERMISTOR_TED);

        System.out.println("Minimum temperature: " 
                           + sensor.getTemperatureRangeMin());
        System.out.println("Maximum temperature: " 
                           + sensor.getTemperatureRangeMax());
        System.out.println();

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                if (sensor.isOutOfRange())
                    {
                        System.out.println("Temperature out of range");
                    }
                else
                    {
                        // we show both C and F for temperature
                        System.out.println("Temperature: " 
                                           + sensor.getTemperature()
                                           + " C / "
                                           + sensor.getTemperature(true)
                                           + " F");
                    }

                System.out.println();
                Thread.sleep(1000);
            }

// ! [Interesting]
    }
}
