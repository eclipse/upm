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

import upm_mb704x.MB704X;

public class MB704X_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate a MB704X sensor using default parameters (bus 0,
        // address 112)
        MB704X sensor = new MB704X();

        while (true)
            {
                System.out.println("Range: "
                                   + sensor.getRange()
                                   + " cm");

                Thread.sleep(500);
            }

// ! [Interesting]
    }
}
