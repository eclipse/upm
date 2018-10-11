/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
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

import upm_bmp280.*;
import upm_new_interfaces.*;

public class BME280_Interface_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a BME280 instance using default i2c bus and address
        BME280 sensor = new BME280();

        iTemperature t_sensor = sensor;
        iHumidity h_sensor = sensor;
        iPressure p_sensor = sensor;

        // For SPI, bus 0, you would pass -1 as the address, and a
        // valid pin for CS:
        // BME280(0, -1, 10);

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                System.out.println("Calling Interface Functions: ");

                System.out.println("Compensation Temperature: "
                                   + t_sensor.getTemperature()
                                   + " C / ");

                System.out.println("Pressure: "
                                   + p_sensor.getPressure()
                                   + " Pa");

                System.out.println("Humidity: "
                                   + h_sensor.getHumidity()
                                   + " %RH");

                System.out.println();
                Thread.sleep(1000);
            }

// ! [Interesting]
    }
}
