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

import upm_mma7361.MMA7361;

public class MMA7361_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a MMA7361 sensor on analog pins A0 (X), A1 (Y)
        // A2 (Z), selftest pin on D2, sleep pin on D3 nd an analog
        // reference value of 5.0.  The freefall pin and the range pin
        // are unused (-1).
        MMA7361 sensor = new MMA7361(0, 1, 2, 2, 3, -1, -1, 5.0f);

        // 1.5g (true = 6g)
        sensor.setRange(false);

        // Every 10th of a second, update and print values
        while (true)
            {
                // update our values from the sensor
                sensor.update();

                float dataA[] = sensor.getAcceleration();

                System.out.println("Accelerometer x: " + dataA[0]
                                   + " y: " + dataA[1]
                                   + " z: " + dataA[2]);

                float dataV[] = sensor.getVolts();

                System.out.println("Volts x: " + dataV[0]
                                   + " y: " + dataV[1]
                                   + " z: " + dataV[2]);

                System.out.println();
                Thread.sleep(100);
            }

// ! [Interesting]
    }
}
