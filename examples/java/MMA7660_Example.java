/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 *         Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
