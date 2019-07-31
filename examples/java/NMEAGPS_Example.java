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

public class NMEAGPS_Example 
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        System.out.println("Initializing...");

        // Instantiate a NMEAGPS sensor on uart 0 at 9600 baud with
        // enable pin on D3
        NMEAGPS sensor = new NMEAGPS(0, 9600, 3);

        // loop, dumping NMEA data out as fast as it comes in
        while (sensor.dataAvailable(5000))
            {
                System.out.print(sensor.readStr(256));
            }
        System.out.println("Timed out");

        
// ! [Interesting]
    }
}
