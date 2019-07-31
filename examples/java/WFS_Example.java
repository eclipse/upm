/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

//NOT TESTED!!!
public class WFS_Example {

    public static void main(String[] args) throws InterruptedException {
// ! [Interesting]

        // Instantiate a Water Flow Sensor on
        // digital pin D2.  This must be an
        // interrupt capable pin.
        upm_wfs.WFS flow = new upm_wfs.WFS(2);

        flow.clearFlowCounter();
        flow.startFlowCounter();

        while (true) {
            System.out.print("Millis: " + flow.getMillis() + " FlowCount: " + flow.flowCounter());
            System.out.println(" Flow Rate: " + flow.flowRate() + " LPM");

            Thread.sleep(2000);
        }
// ! [Interesting]
    }
}
