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

import upm_teams.TEAMS;

public class TEAMS_Example 
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        System.out.println("Initializing...");

        // Instantiate an TEAMS instance, using A0 for temperature, and
        // 165.0 ohms for the rResistor value (for the libelium 4-20ma
        // interface)
        TEAMS sensor = new TEAMS(0, 165.0f);
        
        while (true)
            {
                // update our values from the sensor
                sensor.update();

                // is the sensor connected? (current >= 4ma)
                System.out.println("Is Connected: " + sensor.isConnected());
    
                // print computed current on the loop.  This includes
                // the offset, if one was set by setOffsetMilliamps().
                System.out.println("Milliamps: " + sensor.getRawMilliamps());

                // we show both C and F for temperature
                System.out.println("Temperature: " 
                                   + sensor.getTemperature()
                                   + " C / "
                                   + sensor.getTemperature(true)
                                   + " F");

                System.out.println();
                Thread.sleep(1000);
            }

// ! [Interesting]
    }
}
