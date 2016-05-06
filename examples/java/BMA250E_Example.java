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

import upm_bmx055.BMA250E;

public class BMA250E_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a BMA250E instance using default i2c bus and address
        BMA250E sensor = new BMA250E();

        // For SPI, bus 0, you would pass -1 as the address, and a
        // valid pin for CS:
        // BMA250E(0, -1, 10);

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                float dataA[] = sensor.getAccelerometer();

                System.out.println("Accelerometer x: " + dataA[0]
                                   + " y: " + dataA[1]
                                   + " z: " + dataA[2]
                                   + " g");

                System.out.println("Compensation Temperature: "
                                   + sensor.getTemperature()
                                   + " C / "
                                   + sensor.getTemperature(true)
                                   + " F");

                System.out.println();
                Thread.sleep(250);
            }

// ! [Interesting]
    }
}
