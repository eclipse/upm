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
public class Lcm1602_i2c_Example {

    public static void main(String[] args) throws InterruptedException {
        // ! [Interesting]
        upm_lcm1602.Lcm1602 lcd = new upm_lcm1602.Lcm1602(0, 0x27);

        lcd.setCursor(0, 0);
        lcd.write("Hello World");
        Thread.sleep(3000);

        lcd.setCursor(1, 2);
        lcd.write("Hello World");
        Thread.sleep(3000);

        // ! [Interesting]
    }
}
