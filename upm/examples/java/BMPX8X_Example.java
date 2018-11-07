/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2017 Intel Corporation.
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

public class BMPX8X_Example {

    public static void main(String[] args) throws InterruptedException {
// ! [Interesting]

        // Instantiate a BMPX8X sensor on I2C using defaults.
        upm_bmpx8x.BMPX8X sensor = new upm_bmpx8x.BMPX8X();

        // Print the pressure, altitude, sea level, and
        // temperature values every .5 seconds
        while (true)
        {
            sensor.update();

            System.out.println("Pressure: "
                               + sensor.getPressure()
                               + " Pa, Temperature: "
                               + sensor.getTemperature()
                               + " C, Altitude: "
                               + sensor.getAltitude()
                               + " m, Sea level: "
                               + sensor.getSealevelPressure()
                               + " Pa");

            Thread.sleep(500);
        }
// ! [Interesting]
    }

}
