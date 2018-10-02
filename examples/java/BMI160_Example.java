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

import java.util.AbstractList;
import upm_interfaces.*;

import upm_bmi160.BMI160;

public class BMI160_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        System.out.println("Initializing...");

        // Instantiate a BMI160 instance using default i2c bus and address
        BMI160 sensor = new BMI160();

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                AbstractList<Float> dataA = sensor.getAcceleration();

                System.out.println("Accelerometer: "
                                   + "AX: "
                                   + dataA.get(0)
                                   + " AY: "
                                   + dataA.get(1)
                                   + " AZ: "
                                   + dataA.get(2));

                AbstractList<Float> dataG = sensor.getGyroscope();

                System.out.println("Gryoscope:     "
                                   + "GX: "
                                   + dataG.get(0)
                                   + " GY: "
                                   + dataG.get(1)
                                   + " GZ: "
                                   + dataG.get(2));

                float dataM[] = sensor.getMagnetometer();

                System.out.println("Magnetometer:  "
                                   + "MX: "
                                   + dataM[0]
                                   + " MY: "
                                   + dataM[1]
                                   + " MZ: "
                                   + dataM[2]);

                System.out.println();
                Thread.sleep(500);
            }

// ! [Interesting]
    }
}
