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

import upm_nmea_gps.NMEAGPS;

public class NMEAGPS_I2C_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        System.out.println("Initializing...");

        // Instantiate a NMEA_GPS UBLOX based i2c sensor on i2c bus 0 at
        // address 0x42
        NMEAGPS sensor = new NMEAGPS(0, (byte)0x42);

        // loop, dumping NMEA data out as fast as it comes in
        while (true)
            {
                if (sensor.dataAvailable(0))
                    System.out.print(sensor.readStr(256));
                else
                    Thread.sleep(100);
            }

// ! [Interesting]
    }
}
