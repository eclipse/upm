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
