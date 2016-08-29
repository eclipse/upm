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

import upm_hka5.HKA5;

public class HKA5_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        System.out.println("Initializing...");

        // Instantiate a HKA5 sensor on uart 0.  We don't use the set or
        // reset pins, so we pass -1 for them.
        HKA5 sensor = new HKA5(0, -1, -1);

        // update once every 2 seconds and output data
        while (true)
            {
                sensor.update();

                System.out.println("PM 1  : "
                                   + sensor.getPM1()
                                   + " ug/m3");
                System.out.println("PM 2.5: "
                                   + sensor.getPM2_5()
                                   + " ug/m3");
                System.out.println("PM 10 : "
                                   + sensor.getPM10()
                                   + " ug/m3");

                System.out.println();
                Thread.sleep(2000);
            }

// ! [Interesting]
    }
}
