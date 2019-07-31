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

import upm_cwlsxxa.CWLSXXA;

public class CWLSXXA_Example 
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]

        // Instantiate an CWLSXXA instance, using A0 for CO2, A1 for
        // humidity and A2 for temperature
        CWLSXXA sensor = new CWLSXXA(0, 1, 2);
        
        while (true)
            {
                // update our values from the sensor
                sensor.update();

                // we show both C and F for temperature
                System.out.println("Temperature: " 
                                   + sensor.getTemperature()
                                   + " C / "
                                   + sensor.getTemperature(true)
                                   + " F");

                System.out.println("Humidity: "
                                   + sensor.getHumidity()
                                   + " %");

                System.out.println("CO2: "
                                   + sensor.getCO2()
                                   + " ppm");
                
                Thread.sleep(1000);
            }

// ! [Interesting]
    }
}
