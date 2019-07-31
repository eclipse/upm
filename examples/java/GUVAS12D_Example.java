/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 *         Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class GUVAS12D_Example {
    // analog voltage, usually 3.3 or 5.0
    private static final float GUVAS12D_AREF = 5;

    public static void main(String[] args) throws InterruptedException {
        // ! [Interesting]
        // Instantiate a GUVAS12D on analog pin A0
        upm_guvas12d.GUVAS12D volts = new upm_guvas12d.GUVAS12D(0,
                                                                GUVAS12D_AREF);

        while (true)
        {
            System.out.println("Volts: "
                               + volts.volts()
                               + ", Intensity: "
                               + volts.intensity()
                               + " mW/m^2");

            Thread.sleep(1000);
        }
        // ! [Interesting]
    }

}
