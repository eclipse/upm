/*
 * Author: Noel Eck <noel.eck@intel.com>
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

import upm_max30100.*;

public class MAX30100_Example
{
    public static void main(String[] args) throws InterruptedException
    {
        // ! [Interesting]

        // Instantiate a MAX30100 instance using bus 0
        MAX30100 sensor = new MAX30100((short)0);

        System.out.println("Oximeter sensor example...");

        // Read the temperature and version
        System.out.format("Temperature: %f C\n", sensor.temperature());
        System.out.format("Version: 0x%04x\n", sensor.version());

        // Set high-res (50 Hz, 16-bit)
        sensor.high_res_enable(true);

        // Set to sample SpO2
        sensor.mode(MAX30100_MODE.MAX30100_MODE_SPO2_EN);

        Callback cb = new JavaCallback();

        // Read continuously, stepping up the LED current every second,
        // us GPIO 0 as the interrupt pin
        sensor.sample_continuous(0, false, cb);
        for (int i = 0; i <= 15; i++)
        {
            // Toggle the LED current
            System.out.format("Setting LED current = %d\n", i);

            sensor.current(MAX30100_LED_CURRENT.swigToEnum(i),
                           MAX30100_LED_CURRENT.swigToEnum(i));

            Thread.sleep(1000);
        }

        sensor.sample_stop();

        // Read individual samples
        for (int i = 0; i < 10; i++) {
            max30100_value val = sensor.sample();
            System.out.format("Single value IR: %d R: %d\n", val.getIR(), val.getR());
        }

        // ! [Interesting]
    }
}

class JavaCallback extends Callback
{
    public JavaCallback()
    {
        super();
    }

    public void run(max30100_value val)
    {
        System.out.format("My callback sample IR: %d R: %d\n", val.getIR(), val.getR());
    }
}

