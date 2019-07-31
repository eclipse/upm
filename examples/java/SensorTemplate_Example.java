/*
 * The MIT License (MIT)
 *
 * Author: Your Full Name <your@email.address>
 * Copyright (c) <year> <copyright holder>
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


public class SensorTemplate_Example {
    public static void main (String args[]) throws InterruptedException {
        //! [Interesting]
        // Instantiate new sensor instance
        upm_sensortemplate.SensorTemplate sensor = new upm_sensortemplate.SensorTemplate(0);

        while (true) {

            System.out.println("SensorTemplate says: " + sensor.helloWorld());

            // Repeate every 2 seconds
            Thread.sleep(2000);
        }
        //! [Interesting]
    }
}
