/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 *         Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

import upm_mma7660.MMA7660;

public class MMA7660_Example
{
    public static void main(String[] args) throws InterruptedException
        {
            // ! [Interesting]
            // Instantiate an MMA7660 on I2C bus 0
            MMA7660 accel = new MMA7660(0);

            // place device in standby mode so we can write registers
            accel.setModeStandby();

            // enable 64 samples per second
            accel.setSampleRate(upm_mma7660.MMA7660_AUTOSLEEP_T.MMA7660_AUTOSLEEP_64);

            // place device into active mode
            accel.setModeActive();

            while (true)
            {
                AbstractList<Float> acceleration = accel.getAcceleration();
                System.out.println("Acceleration: x = "
                                   + acceleration.get(0)
                                   + " y = "
                                   + acceleration.get(1)
                                   + " x = "
                                   + acceleration.get(2));

                System.out.println();

                Thread.sleep(500);
            }
            // ! [Interesting]
	}
}
