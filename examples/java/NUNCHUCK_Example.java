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

public class NUNCHUCK_Example {

    public static void main(String[] args) throws InterruptedException {
        // ! [Interesting]
        // Instantiate a nunchuck controller bus 0
        upm_nunchuck.NUNCHUCK nunchuck = new upm_nunchuck.NUNCHUCK(3);

        while (true)
        {
            nunchuck.update();
            System.out.println("stickX: "
                               + nunchuck.getStickX()
                               + ", stickY: "
                               + nunchuck.getStickY());
            System.out.println("accelX: "
                               + nunchuck.getAccelX()
                               + ", accelY: "
                               + nunchuck.getAccelY()
                               + ", accelZ: "
                               + nunchuck.getAccelZ());

            if (nunchuck.getButtonC())
                System.out.println("Button C pressed");
            else
                System.out.println("Button C not pressed");

            if (nunchuck.getButtonZ())
                System.out.println("Button Z pressed");
            else
                System.out.println("Button Z not pressed");

            Thread.sleep(100);
        }
        // ! [Interesting]
    }

}
