/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_bh1750.BH1750;

public class BH1750_Example 
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        System.out.println("Initializing...");

        // Instantiate a BH1750 sensor using defaults (I2C bus (0), using
        // the default I2C address (0x23), and setting the mode to highest
        // resolution, lowest power mode).
        BH1750 sensor = new BH1750();

        // Every second, sample the BH1750 and output the measured lux
        // value

        while (true)
            {
                System.out.println("Detected Light Level (lux): "
                                   + sensor.getLux());

                Thread.sleep(1000);
            }

// ! [Interesting]
    }
}
