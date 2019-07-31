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

import upm_ecezo.ECEZO;

public class ECEZO_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

         // Instantiate a ECEZO sensor on uart 0 at 9600 baud.
        ECEZO sensor = new ECEZO(0, 9600, false);

        // For I2C, assuming the device is configured for address 0x64 on
        // I2C bus 0, you could use something like:
        //
        // ECEZO sensor = new ECEZO(0, 0x64, true);

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                System.out.println("EC "
                                   + sensor.getEC()
                                   + " uS/cm, TDS "
                                   + sensor.getTDS()
                                   + " mg/L, Salinity "
                                   + sensor.getSalinity()
                                   + " PSS-78, SG "
                                   + sensor.getSG());

                Thread.sleep(5000);
            }

// ! [Interesting]
    }
}
