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
